namespace AdventureClaude.Game;

using System;
using System.Collections.Generic;
using AdventureClaude.Data;
using AdventureClaude.Models;

/// <summary>
/// Handles motion commands and travel-table evaluation from the original TURN.C travel logic.
/// </summary>
public sealed class TravelEngine
{
    private readonly GameState gameState;
    private readonly Random random;
    private readonly Action<int, int> printObjectMessage;
    private readonly Action handleDeath;

    public TravelEngine(
        GameState gameState,
        Random random,
        Action<int, int> printObjectMessage,
        Action handleDeath)
    {
        this.gameState = gameState;
        this.random = random;
        this.printObjectMessage = printObjectMessage;
        this.handleDeath = handleDeath;
    }

    /// <summary>
    /// C TURN.C domove(): dispatches special motion words and normal travel.
    /// </summary>
    public void DoMove()
    {
        GamePositionState position = gameState.Position;
        ParsedCommandState command = gameState.Command;

        switch (command.Motion)
        {
            case GameConstants.NullMotion:
                break;
            case GameConstants.Back:
                GoBack();
                break;
            case GameConstants.Look:
                if (position.Detail == 0)
                {
                    Console.WriteLine(AdventureData.Message(GameConstants.MsgLookDetailLimit));
                    position.Detail |= 1;
                }

                gameState.Cave.WizardDark = false;
                gameState.World.VisitedLocations[position.Location] =
                    (short)((gameState.World.VisitedLocations[position.Location] + 3) & ~3);
                gameState.TreasureProgress.DescriptionDetailMask = 0;
                position.NewLocation = position.Location;
                position.Location = 0;
                break;
            case GameConstants.Cave:
                Console.WriteLine(AdventureData.Message(position.Location < GameConstants.DepressionLocation
                    ? GameConstants.MsgTryStreamForCave
                    : GameConstants.MsgNeedMoreDetailedInstructions));
                break;
            default:
                position.OldLocation2 = position.OldLocation;
                position.OldLocation = position.Location;
                DoTravel();
                break;
        }
    }

    /// <summary>
    /// C TURN.C goback(): tries to infer the reverse route through the travel table.
    /// </summary>
    private void GoBack()
    {
        GamePositionState position = gameState.Position;
        ParsedCommandState command = gameState.Command;

        int want = gameState.Forced(position.OldLocation)
            ? position.OldLocation2
            : position.OldLocation;

        position.OldLocation2 = position.OldLocation;
        position.OldLocation = position.Location;

        if (want == position.Location)
        {
            Console.WriteLine(AdventureData.Message(GameConstants.MsgCantRememberRoute));
            return;
        }

        List<TravelEntry> travel = AdventureData.GetTravelOptions(position.Location);
        TravelEntry? fallback = null;

        foreach (TravelEntry entry in travel)
        {
            if (entry.Condition == 0 && entry.Destination == want)
            {
                command.Motion = entry.Verb;
                DoTravel();
                return;
            }

            if (entry.Condition != 0)
                continue;

            fallback = entry;
            int destination = entry.Destination;
            if (destination <= GameConstants.MaxLocations)
            {
                List<TravelEntry> destinationTravel = AdventureData.GetTravelOptions(destination);
                if (gameState.Forced(destination) &&
                    destinationTravel.Count > 0 &&
                    destinationTravel[0].Destination == want)
                {
                    fallback = entry;
                }
            }
            else
            {
                fallback = null;
            }
        }

        if (fallback != null)
        {
            command.Motion = fallback.Verb;
            DoTravel();
        }
        else
        {
            Console.WriteLine(AdventureData.Message(GameConstants.MsgCantGetThereFromHere));
        }
    }

    /// <summary>
    /// C TURN.C dotrav(): evaluates travel table entries and sets NewLocation.
    /// </summary>
    private void DoTravel()
    {
        GamePositionState position = gameState.Position;
        ParsedCommandState command = gameState.Command;

        List<TravelEntry> travel = AdventureData.GetTravelOptions(position.Location);
        position.NewLocation = position.Location;
        bool hit = false;
        bool moved = false;
        int selectedDestination = position.Location;
        int roll = GameState.RRand(random, 0, 99);

        foreach (TravelEntry entry in travel)
        {
            int destination = entry.Destination;
            int verb = entry.Verb;
            int condition = entry.Condition;

            if (verb != GameConstants.DefaultTravelVerb && verb != command.Motion && !hit)
                continue;

            hit = true;
            if (IsTravelConditionMet(condition, roll))
            {
                selectedDestination = destination;
                moved = true;
                break;
            }
        }

        if (!moved)
        {
            BadMove();
        }
        else if (selectedDestination > GameConstants.TravelMessageDestinationOffset)
        {
            Console.WriteLine(AdventureData.Message(selectedDestination - GameConstants.TravelMessageDestinationOffset));
        }
        else if (selectedDestination > GameConstants.SpecialTravelDestinationOffset)
        {
            SpecialMove(selectedDestination);
        }
        else
        {
            position.NewLocation = selectedDestination;
            if (position.NewLocation == position.Location)
                position.Location = 0;
        }
    }

    private bool IsTravelConditionMet(int condition, int roll)
    {
        ObjectPlacementState objects = gameState.Objects;

        int referencedObject = condition % 100;
        int conditionType = condition / 100;

        return conditionType switch
        {
            0 => condition == 0 || roll < condition,
            1 => referencedObject == 0 || gameState.Toting(referencedObject),
            2 => gameState.Toting(referencedObject) || gameState.At(referencedObject),
            3 or 4 or 5 or 7 => objects.PropertyOf(referencedObject) != conditionType - 3,
            _ => false,
        };
    }

    /// <summary>
    /// C TURN.C badmove(): chooses the best failed-movement message.
    /// </summary>
    private void BadMove()
    {
        ParsedCommandState command = gameState.Command;

        int message = GameConstants.MsgCannotApplyWordHere;
        if (command.Motion >= GameConstants.FirstCompassDirectionMotion &&
            command.Motion <= GameConstants.LastCompassDirectionMotion)
        {
            message = GameConstants.MsgNoWayThatDirection;
        }
        if (command.Motion == GameConstants.MotionUp || command.Motion == GameConstants.MotionDown)
            message = GameConstants.MsgNoWayThatDirection;
        if (command.Motion == GameConstants.MotionForward ||
            command.Motion == GameConstants.MotionLeft ||
            command.Motion == GameConstants.MotionRight)
        {
            message = GameConstants.MsgUseCompassOrObjects;
        }
        if (command.Motion == GameConstants.MotionOut || command.Motion == GameConstants.MotionInside)
            message = GameConstants.MsgDontKnowInFromOut;
        if (command.Verb == GameConstants.Find || command.Verb == GameConstants.Inventory)
            message = GameConstants.MsgCannotLocateRemoteThings;
        if (command.Motion == GameConstants.MotionXyzzy || command.Motion == GameConstants.MotionPlugh)
            message = GameConstants.MsgNothingHappens;
        if (command.Motion == GameConstants.MotionCrawl)
            message = GameConstants.MsgWhichWay;

        Console.WriteLine(AdventureData.Message(message));
    }

    /// <summary>
    /// C TURN.C spcmove(): handles plover and troll bridge travel destinations.
    /// </summary>
    private void SpecialMove(int destination)
    {
        GamePositionState position = gameState.Position;
        ObjectPlacementState objects = gameState.Objects;
        TreasureProgressState progress = gameState.TreasureProgress;

        switch (destination - GameConstants.SpecialTravelDestinationOffset)
        {
            case GameConstants.SpecialMovePloverPassage:
                if (objects.Holding == 0 ||
                    (objects.Holding == 1 && gameState.Toting(GameConstants.Emerald)))
                {
                    position.NewLocation = GameConstants.PloverRoomLocationSum - position.Location;
                }
                else
                {
                    Console.WriteLine(AdventureData.Message(GameConstants.MsgCarriedObjectWontFitPlover));
                }
                break;
            case GameConstants.SpecialMovePloverDropEmerald:
                gameState.Drop(GameConstants.Emerald, position.Location);
                Console.WriteLine(AdventureData.Message(GameConstants.MsgOk));
                break;
            case GameConstants.SpecialMoveTrollBridge:
                if (objects.PropertyOf(GameConstants.Troll) == 1)
                {
                    printObjectMessage(GameConstants.Troll, 1);
                    gameState.SetObjectProperty(GameConstants.Troll, 0);
                    gameState.MoveObject(GameConstants.Troll2, 0);
                    gameState.MoveObject(GameConstants.Troll2 + GameConstants.MaxObjects, 0);
                    gameState.MoveObject(GameConstants.Troll, GameConstants.TrollBridgeNearSideLocation);
                    gameState.MoveObject(GameConstants.Troll + GameConstants.MaxObjects, GameConstants.TrollBridgeFarSideLocation);
                    GameState.Juggle(GameConstants.Chasm);
                    position.NewLocation = position.Location;
                }
                else
                {
                    position.NewLocation = position.Location == GameConstants.TrollBridgeNearSideLocation
                        ? GameConstants.TrollBridgeFarSideLocation
                        : GameConstants.TrollBridgeNearSideLocation;
                    if (objects.PropertyOf(GameConstants.Troll) == 0)
                        gameState.SetObjectProperty(GameConstants.Troll, objects.PropertyOf(GameConstants.Troll) + 1);

                    if (!gameState.Toting(GameConstants.Bear))
                        return;

                    Console.WriteLine(AdventureData.Message(GameConstants.MsgBearBreaksBridge));
                    gameState.SetObjectProperty(GameConstants.Chasm, 1);
                    gameState.SetObjectProperty(GameConstants.Troll, 2);
                    gameState.Drop(GameConstants.Bear, position.NewLocation);
                    objects.SetFixedLocation(GameConstants.Bear, -1);
                    gameState.SetObjectProperty(GameConstants.Bear, 3);
                    if (objects.IsPropertyNegative(GameConstants.Spices))
                        progress.TreasuresLostToEndgame++;
                    position.OldLocation2 = position.NewLocation;
                    handleDeath();
                }
                break;
            default:
                Console.WriteLine($"Fatal error number 38");
                progress.SaveRequested = true;
                break;
        }
    }
}
