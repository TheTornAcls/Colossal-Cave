namespace AdventureClaude.Game;

using System;
using System.Collections.Generic;
using AdventureClaude.Data;
using AdventureClaude.Models;

/// <summary>
/// Handles parsed command dispatch and object-heavy verb behavior.
/// </summary>
public sealed class VerbHandlers
{
    private readonly GameState gameState;
    private readonly Random random;
    private readonly TravelEngine travelEngine;
    private readonly Action<int, int> printObjectMessage;
    private readonly Action<bool> showLocationDescription;
    private readonly Func<int, int, int, bool> askYesNo;
    private readonly Action<int> dwarfEnd;
    private readonly Action normalEnd;
    private readonly Func<int> printScore;

    public VerbHandlers(
        GameState gameState,
        Random random,
        TravelEngine travelEngine,
        Action<int, int> printObjectMessage,
        Action<bool> showLocationDescription,
        Func<int, int, int, bool> askYesNo,
        Action<int> dwarfEnd,
        Action normalEnd,
        Func<int> printScore)
    {
        this.gameState = gameState;
        this.random = random;
        this.travelEngine = travelEngine;
        this.printObjectMessage = printObjectMessage;
        this.showLocationDescription = showLocationDescription;
        this.askYesNo = askYesNo;
        this.dwarfEnd = dwarfEnd;
        this.normalEnd = normalEnd;
        this.printScore = printScore;
    }

    public void ProcessCommand()
    {
        ParsedCommandState command = gameState.Command;

        if (command.Motion > 0)
        {
            travelEngine.DoMove();
            return;
        }

        if (command.Verb == GameConstants.Say)
        {
            TransitiveVerb();
            return;
        }

        if (command.Object > 0)
        {
            DoObject();
            return;
        }

        if (command.Verb > 0)
            IntransitiveVerb();
    }
    private void DoObject()
    {
        GamePositionState position = gameState.Position;
        ObjectPlacementState objects = gameState.Objects;
        DwarfPirateState dwarves = gameState.Dwarves;
        ParsedCommandState command = gameState.Command;

        int objectId = command.Object;

        if (objects.FixedLocationOf(objectId) == position.Location || gameState.Here(objectId))
        {
            TransitiveObject();
            return;
        }

        if (objectId == GameConstants.Grate)
        {
            if (position.Location == GameConstants.EndOfRoad ||
                position.Location == GameConstants.ValleyLocation ||
                position.Location == GameConstants.SlitInStreambedLocation)
            {
                command.Motion = GameConstants.Depression;
                travelEngine.DoMove();
                return;
            }

            if (position.Location >= GameConstants.FirstLowerGrateApproachLocation &&
                position.Location <= GameConstants.LastLowerGrateApproachLocation)
            {
                command.Motion = GameConstants.Entrance;
                travelEngine.DoMove();
                return;
            }
        }
        else if (gameState.DCheck() != 0 && dwarves.ActivationLevel >= 2)
        {
            command.Object = GameConstants.Dwarf;
            TransitiveObject();
            return;
        }
        else if ((gameState.Liq() == objectId && gameState.Here(GameConstants.Bottle)) ||
            gameState.LiqLoc(position.Location) == objectId)
        {
            TransitiveObject();
            return;
        }
        else if (objectId == GameConstants.Plant &&
            gameState.At(GameConstants.Plant2) &&
            objects.PropertyOf(GameConstants.Plant2) == 0)
        {
            command.Object = GameConstants.Plant2;
            TransitiveObject();
            return;
        }
        else if (objectId == GameConstants.Knife && objects.KnifeLocation == position.Location)
        {
            Speak(GameConstants.MsgKnivesVanish);
            objects.KnifeLocation = -1;
            return;
        }
        else if (objectId == GameConstants.Rod && gameState.Here(GameConstants.Rod2))
        {
            command.Object = GameConstants.Rod2;
            TransitiveObject();
            return;
        }
        else if (objectId == GameConstants.Troll && gameState.Here(GameConstants.Troll2))
        {
            command.Object = GameConstants.Troll2;
            TransitiveObject();
            return;
        }
        else if (objectId == GameConstants.Plant && gameState.Here(GameConstants.Plant2))
        {
            command.Object = GameConstants.Plant2;
            TransitiveObject();
            return;
        }

        SpeakObjectNotHere(objectId);
    }

    private void TransitiveObject()
    {
        ParsedCommandState command = gameState.Command;

        if (command.Verb != 0)
            TransitiveVerb();
        else
            Console.WriteLine($"What do you want to do with the {GetObjectWord(command.Object)}?");
    }

    private void TransitiveVerb()
    {
        ParsedCommandState command = gameState.Command;

        switch (command.Verb)
        {
            case GameConstants.Calm:
            case GameConstants.Walk:
            case GameConstants.Quit:
            case GameConstants.Score:
            case GameConstants.Foo:
            case GameConstants.Brief:
            case GameConstants.Suspend:
            case GameConstants.Hours:
            case GameConstants.Log:
                ActSpeak(command.Verb);
                break;
            case GameConstants.Take:
                VTake();
                break;
            case GameConstants.Drop:
                VDrop();
                break;
            case GameConstants.Open:
            case GameConstants.Lock:
                VOpen();
                break;
            case GameConstants.Say:
                VSay();
                break;
            case GameConstants.Nothing:
                Speak(GameConstants.MsgOk);
                break;
            case GameConstants.On:
                VOn();
                break;
            case GameConstants.Off:
                VOff();
                break;
            case GameConstants.Wave:
                VWave();
                break;
            case GameConstants.Kill:
                VKill();
                break;
            case GameConstants.Pour:
                VPour();
                break;
            case GameConstants.Eat:
                VEat();
                break;
            case GameConstants.Drink:
                VDrink();
                break;
            case GameConstants.Rub:
                if (command.Object != GameConstants.Lamp)
                    Speak(GameConstants.MsgNothingUnexpected);
                else
                    ActSpeak(GameConstants.Rub);
                break;
            case GameConstants.Throw:
                VThrow();
                break;
            case GameConstants.Feed:
                VFeed();
                break;
            case GameConstants.Find:
            case GameConstants.Inventory:
                VFind();
                break;
            case GameConstants.Fill:
                VFill();
                break;
            case GameConstants.Read:
                VRead();
                break;
            case GameConstants.Blast:
                VBlast();
                break;
            case GameConstants.Break:
                VBreak();
                break;
            case GameConstants.Wake:
                VWake();
                break;
            default:
                Console.WriteLine("This verb is not implemented yet.");
                break;
        }
    }

    private void IntransitiveVerb()
    {
        ParsedCommandState command = gameState.Command;

        switch (command.Verb)
        {
            case GameConstants.Drop:
            case GameConstants.Say:
            case GameConstants.Wave:
            case GameConstants.Calm:
            case GameConstants.Rub:
            case GameConstants.Throw:
            case GameConstants.Find:
            case GameConstants.Feed:
            case GameConstants.Break:
            case GameConstants.Wake:
                NeedObject();
                break;
            case GameConstants.Take:
                IVTake();
                break;
            case GameConstants.Open:
            case GameConstants.Lock:
                IVOpen();
                break;
            case GameConstants.Nothing:
                Speak(GameConstants.MsgOk);
                break;
            case GameConstants.On:
            case GameConstants.Off:
            case GameConstants.Pour:
                TransitiveVerb();
                break;
            case GameConstants.Walk:
                ActSpeak(command.Verb);
                break;
            case GameConstants.Kill:
                IVKill();
                break;
            case GameConstants.Eat:
                IVEat();
                break;
            case GameConstants.Drink:
                IVDrink();
                break;
            case GameConstants.Quit:
                IVQuit();
                break;
            case GameConstants.Fill:
                IVFill();
                break;
            case GameConstants.Blast:
                VBlast();
                break;
            case GameConstants.Score:
                printScore();
                break;
            case GameConstants.Fee:
            case GameConstants.Fie:
            case GameConstants.Foe:
            case GameConstants.Foo:
            case GameConstants.Fum:
                IVFoo();
                break;
            case GameConstants.Suspend:
                gameState.TreasureProgress.SaveRequested = true;
                break;
            case GameConstants.Read:
                IVRead();
                break;
            case GameConstants.Inventory:
                ShowInventory();
                break;
            case GameConstants.Brief:
                gameState.Position.Detail |= 2;
                ActSpeak(command.Verb);
                break;
            case GameConstants.Help:
                Speak(GameConstants.MsgHelp);
                break;
            default:
                Console.WriteLine("This intransitive verb is not implemented yet.");
                break;
        }
    }

    private void IVTake()
    {
        GamePositionState position = gameState.Position;
        ObjectPlacementState objects = gameState.Objects;
        DwarfPirateState dwarves = gameState.Dwarves;
        ParsedCommandState command = gameState.Command;

        int candidate = 0;
        for (int item = 1; item < GameConstants.MaxObjects; item++)
        {
            if (objects.LocationOf(item) != position.Location)
                continue;

            if (candidate != 0)
            {
                NeedObject();
                return;
            }

            candidate = item;
        }

        if (candidate == 0 || (gameState.DCheck() != 0 && dwarves.ActivationLevel >= 2))
        {
            NeedObject();
            return;
        }

        command.Object = candidate;
        VTake();
    }

    private void IVOpen()
    {
        ParsedCommandState command = gameState.Command;

        int candidate = 0;
        if (gameState.Here(GameConstants.Clam))
            candidate = GameConstants.Clam;
        if (gameState.Here(GameConstants.Oyster))
            candidate = GameConstants.Oyster;
        if (gameState.At(GameConstants.Door))
            candidate = GameConstants.Door;
        if (gameState.At(GameConstants.Grate))
            candidate = GameConstants.Grate;
        if (gameState.Here(GameConstants.Chain))
        {
            if (candidate != 0)
            {
                NeedObject();
                return;
            }

            candidate = GameConstants.Chain;
        }

        if (candidate == 0)
        {
            Speak(GameConstants.MsgNoLockHere);
            return;
        }

        command.Object = candidate;
        VOpen();
    }

    private void IVKill()
    {
        ObjectPlacementState objects = gameState.Objects;
        DwarfPirateState dwarves = gameState.Dwarves;
        ParsedCommandState command = gameState.Command;

        int candidate = 0;
        bool ambiguous = false;

        if (gameState.DCheck() != 0 && dwarves.ActivationLevel >= 2)
            candidate = GameConstants.Dwarf;
        AddCandidate(GameConstants.Snake, gameState.Here(GameConstants.Snake), ref candidate, ref ambiguous);
        AddCandidate(GameConstants.Dragon, gameState.At(GameConstants.Dragon) && objects.PropertyOf(GameConstants.Dragon) == 0, ref candidate, ref ambiguous);
        AddCandidate(GameConstants.Troll, gameState.At(GameConstants.Troll), ref candidate, ref ambiguous);
        AddCandidate(GameConstants.Bear, gameState.Here(GameConstants.Bear) && objects.PropertyOf(GameConstants.Bear) == 0, ref candidate, ref ambiguous);

        if (ambiguous)
        {
            NeedObject();
            return;
        }

        if (candidate != 0)
        {
            command.Object = candidate;
            VKill();
            return;
        }

        if (gameState.Here(GameConstants.Bird) && command.Verb != GameConstants.Throw)
            candidate = GameConstants.Bird;
        AddCandidate(GameConstants.Clam, gameState.Here(GameConstants.Clam) || gameState.Here(GameConstants.Oyster), ref candidate, ref ambiguous);

        if (ambiguous)
        {
            NeedObject();
            return;
        }

        command.Object = candidate;
        VKill();
    }

    private void IVEat()
    {
        ParsedCommandState command = gameState.Command;

        if (!gameState.Here(GameConstants.Food))
        {
            NeedObject();
            return;
        }

        command.Object = GameConstants.Food;
        VEat();
    }

    private void IVDrink()
    {
        ParsedCommandState command = gameState.Command;

        if (gameState.LiqLoc(gameState.Position.Location) != GameConstants.Water &&
            (gameState.Liq() != GameConstants.Water || !gameState.Here(GameConstants.Bottle)))
        {
            NeedObject();
            return;
        }

        command.Object = GameConstants.Water;
        VDrink();
    }

    private void IVQuit()
    {
        gameState.TreasureProgress.GaveUp = askYesNo(GameConstants.MsgPromptQuit, 0, GameConstants.MsgOk);
        if (gameState.TreasureProgress.GaveUp)
            normalEnd();
    }

    private void IVFill()
    {
        ParsedCommandState command = gameState.Command;

        if (!gameState.Here(GameConstants.Bottle))
        {
            NeedObject();
            return;
        }

        command.Object = GameConstants.Bottle;
        VFill();
    }

    private void IVFoo()
    {
        GamePositionState position = gameState.Position;
        ObjectPlacementState objects = gameState.Objects;
        TreasureProgressState progress = gameState.TreasureProgress;
        ParsedCommandState command = gameState.Command;

        int k = command.Verb - GameConstants.Fee + 1;
        int message = GameConstants.MsgNothingHappens;

        if (progress.FooBar != 1 - k)
        {
            if (progress.FooBar != 0)
                message = GameConstants.MsgFooSequenceFailed;
            Speak(message);
            return;
        }

        progress.FooBar = k;
        if (k != 4)
            return;

        progress.FooBar = 0;
        if (objects.LocationOf(GameConstants.Eggs) == GameConstants.GiantRoomLocation ||
            (gameState.Toting(GameConstants.Eggs) && position.Location == GameConstants.GiantRoomLocation))
        {
            Speak(message);
            return;
        }

        if (objects.LocationOf(GameConstants.Eggs) == 0 &&
            objects.LocationOf(GameConstants.Troll) == 0 &&
            objects.PropertyOf(GameConstants.Troll) == 0)
        {
            gameState.SetObjectProperty(GameConstants.Troll, 1);
        }

        if (gameState.Here(GameConstants.Eggs))
            k = 1;
        else if (position.Location == GameConstants.GiantRoomLocation)
            k = 0;
        else
            k = 2;

        gameState.MoveObject(GameConstants.Eggs, GameConstants.GiantRoomLocation);
        printObjectMessage(GameConstants.Eggs, k);
    }

    private void IVRead()
    {
        ParsedCommandState command = gameState.Command;

        int candidate = 0;
        if (gameState.Here(GameConstants.Magazine))
            candidate = GameConstants.Magazine;
        if (gameState.Here(GameConstants.Tablet))
            candidate = candidate * 100 + GameConstants.Tablet;
        if (gameState.Here(GameConstants.Message))
            candidate = candidate * 100 + GameConstants.Message;

        if (candidate > GameConstants.ReadAmbiguousObjectAccumulatorThreshold ||
            candidate == 0 ||
            DarknessManager.IsDark(gameState))
        {
            NeedObject();
            return;
        }

        command.Object = candidate;
        VRead();
    }

    private void VTake()
    {
        GamePositionState position = gameState.Position;
        ObjectPlacementState objects = gameState.Objects;
        ParsedCommandState command = gameState.Command;

        int objectId = command.Object;
        if (gameState.Toting(objectId))
        {
            ActSpeak(command.Verb);
            return;
        }

        int message = GameConstants.MsgCantBeSerious;
        if (objectId == GameConstants.Plant && objects.PropertyOf(GameConstants.Plant) <= 0)
            message = GameConstants.MsgPlantRoots;
        if (objectId == GameConstants.Bear && objects.PropertyOf(GameConstants.Bear) == 1)
            message = GameConstants.MsgBearStillChained;
        if (objectId == GameConstants.Chain && objects.PropertyOf(GameConstants.Bear) != 0)
            message = GameConstants.MsgChainStillLocked;
        if (objects.HasFixedLocation(objectId))
        {
            Speak(message);
            return;
        }

        if (objectId == GameConstants.Water || objectId == GameConstants.Oil)
        {
            if (!gameState.Here(GameConstants.Bottle) || gameState.Liq() != objectId)
            {
                command.Object = GameConstants.Bottle;
                if (gameState.Toting(GameConstants.Bottle) && objects.PropertyOf(GameConstants.Bottle) == 1)
                {
                    VFill();
                    return;
                }

                if (objects.PropertyOf(GameConstants.Bottle) != 1)
                    message = GameConstants.MsgBottleAlreadyFull;
                if (!gameState.Toting(GameConstants.Bottle))
                    message = GameConstants.MsgNothingToCarryLiquid;
                Speak(message);
                return;
            }

            objectId = GameConstants.Bottle;
            command.Object = objectId;
        }

        if (objects.Holding >= GameConstants.MaxCarriedObjects)
        {
            Speak(GameConstants.MsgCantCarryMore);
            return;
        }

        if (objectId == GameConstants.Bird && objects.PropertyOf(GameConstants.Bird) == 0)
        {
            if (gameState.Toting(GameConstants.Rod))
            {
                Speak(GameConstants.MsgBirdDisturbedByRod);
                return;
            }

            if (!gameState.Toting(GameConstants.Cage))
            {
                Speak(GameConstants.MsgNeedCageForBird);
                return;
            }

            gameState.SetObjectProperty(GameConstants.Bird, 1);
        }

        if ((objectId == GameConstants.Bird || objectId == GameConstants.Cage) &&
            objects.PropertyOf(GameConstants.Bird) != 0)
        {
            gameState.Carry(GameConstants.Bird + GameConstants.Cage - objectId, position.Location);
        }

        gameState.Carry(objectId, position.Location);
        int liquid = gameState.Liq();
        if (objectId == GameConstants.Bottle && liquid != 0)
            gameState.SetObjectLocation(liquid, -1);

        Speak(GameConstants.MsgOk);
    }

    private void VDrop()
    {
        GamePositionState position = gameState.Position;
        ObjectPlacementState objects = gameState.Objects;
        CaveTimingState cave = gameState.Cave;
        TreasureProgressState progress = gameState.TreasureProgress;
        ParsedCommandState command = gameState.Command;

        int objectId = command.Object;
        if (gameState.Toting(GameConstants.Rod2) && objectId == GameConstants.Rod && !gameState.Toting(GameConstants.Rod))
        {
            objectId = GameConstants.Rod2;
            command.Object = objectId;
        }

        if (!gameState.Toting(objectId))
        {
            ActSpeak(command.Verb);
            return;
        }

        int message = GameConstants.MsgOk;
        if (objectId == GameConstants.Bird && gameState.Here(GameConstants.Snake))
        {
            Speak(GameConstants.MsgBirdDrivesSnakeAway);
            message = 0;
            if (cave.Closed)
                dwarfEnd(GameConstants.MsgDwarvesAwakenGetYou);
            gameState.Destroy(GameConstants.Snake);
            gameState.SetObjectProperty(GameConstants.Snake, -1);
        }
        else if (objectId == GameConstants.Coins && gameState.Here(GameConstants.Vend))
        {
            gameState.Destroy(GameConstants.Coins);
            gameState.Drop(GameConstants.Batteries, position.Location);
            printObjectMessage(GameConstants.Batteries, 0);
            return;
        }
        else if (objectId == GameConstants.Bird &&
            gameState.At(GameConstants.Dragon) &&
            objects.PropertyOf(GameConstants.Dragon) == 0)
        {
            Speak(GameConstants.MsgBirdBurnedByDragon);
            gameState.Destroy(GameConstants.Bird);
            gameState.SetObjectProperty(GameConstants.Bird, 0);
            if (objects.LocationOf(GameConstants.Snake) != 0)
                progress.TreasuresLostToEndgame++;
            return;
        }

        if (objectId == GameConstants.Bear && gameState.At(GameConstants.Troll))
        {
            Speak(GameConstants.MsgBearScaresTroll);
            message = 0;
            gameState.MoveObject(GameConstants.Troll, 0);
            gameState.MoveObject(GameConstants.Troll + GameConstants.MaxObjects, 0);
            gameState.MoveObject(GameConstants.Troll2, GameConstants.TrollBridgeNearSideLocation);
            gameState.MoveObject(GameConstants.Troll2 + GameConstants.MaxObjects, GameConstants.TrollBridgeFarSideLocation);
            GameState.Juggle(GameConstants.Chasm);
            gameState.SetObjectProperty(GameConstants.Troll, 2);
        }
        else if (objectId == GameConstants.Vase)
        {
            if (position.Location == GameConstants.SoftRoomLocation)
                Speak(GameConstants.MsgOk);
            else
            {
                int vaseProperty = gameState.At(GameConstants.Pillow) ? 0 : 2;
                gameState.SetObjectProperty(GameConstants.Vase, vaseProperty);
                printObjectMessage(GameConstants.Vase, vaseProperty + 1);
                if (vaseProperty != 0)
                    objects.SetFixedLocation(GameConstants.Vase, -1);
            }

            message = 0;
        }

        int liquid = gameState.Liq();
        if (liquid == objectId)
        {
            objectId = GameConstants.Bottle;
            command.Object = objectId;
        }

        if (objectId == GameConstants.Bottle && liquid != 0)
            gameState.SetObjectLocation(liquid, 0);
        if (objectId == GameConstants.Cage && objects.PropertyOf(GameConstants.Bird) != 0)
            gameState.Drop(GameConstants.Bird, position.Location);
        if (objectId == GameConstants.Bird)
            gameState.SetObjectProperty(GameConstants.Bird, 0);

        gameState.Drop(objectId, position.Location);
        if (message != 0)
            Speak(message);
    }

    private void VOpen()
    {
        GamePositionState position = gameState.Position;
        ObjectPlacementState objects = gameState.Objects;
        ParsedCommandState command = gameState.Command;

        int message;
        switch (command.Object)
        {
            case GameConstants.Clam:
            case GameConstants.Oyster:
                int oysterOffset = command.Object == GameConstants.Oyster ? 1 : 0;
                if (command.Verb == GameConstants.Lock)
                    message = GameConstants.MsgWhat;
                else if (!gameState.Toting(GameConstants.Trident))
                    message = GameConstants.MsgNoClamOpener + oysterOffset;
                else if (gameState.Toting(command.Object))
                    message = GameConstants.MsgPutDownClamBeforeOpen + oysterOffset;
                else
                {
                    message = GameConstants.MsgPearlFallsFromClam + oysterOffset;
                    gameState.Destroy(GameConstants.Clam);
                    gameState.Drop(GameConstants.Oyster, position.Location);
                    gameState.Drop(GameConstants.Pearl, GameConstants.PearlDropLocation);
                }
                break;
            case GameConstants.Door:
                message = objects.PropertyOf(GameConstants.Door) == 1
                    ? GameConstants.MsgOk
                    : GameConstants.MsgRustyDoorStuck;
                break;
            case GameConstants.Cage:
                message = GameConstants.MsgNoLock;
                break;
            case GameConstants.Keys:
                message = GameConstants.MsgCantUnlockKeys;
                break;
            case GameConstants.Chain:
                message = OpenChain();
                break;
            case GameConstants.Grate:
                message = OpenGrate();
                break;
            default:
                message = GameConstants.MsgDontLockThat;
                break;
        }

        Speak(message);
    }

    private int OpenChain()
    {
        GamePositionState position = gameState.Position;
        ObjectPlacementState objects = gameState.Objects;
        ParsedCommandState command = gameState.Command;

        if (!gameState.Here(GameConstants.Keys))
            return GameConstants.MsgNoKeys;

        if (command.Verb == GameConstants.Lock)
        {
            if (objects.PropertyOf(GameConstants.Chain) != 0)
                return GameConstants.MsgAlreadyLocked;
            if (position.Location != GameConstants.BearRoomLocation)
                return GameConstants.MsgNothingToLockChainTo;

            gameState.SetObjectProperty(GameConstants.Chain, 2);
            if (gameState.Toting(GameConstants.Chain))
                gameState.Drop(GameConstants.Chain, position.Location);
            objects.SetFixedLocation(GameConstants.Chain, -1);
            return GameConstants.MsgChainLocked;
        }

        if (objects.PropertyOf(GameConstants.Bear) == GameConstants.BearBlocksChainUnlockProperty)
            return GameConstants.MsgBearBlocksChain;
        if (objects.PropertyOf(GameConstants.Chain) == 0)
            return GameConstants.MsgAlreadyUnlocked;

        gameState.SetObjectProperty(GameConstants.Chain, 0);
        objects.SetFixedLocation(GameConstants.Chain, 0);
        if (objects.PropertyOf(GameConstants.Bear) != 3)
            gameState.SetObjectProperty(GameConstants.Bear, 2);
        objects.SetFixedLocation(GameConstants.Bear, 2 - objects.PropertyOf(GameConstants.Bear));
        return GameConstants.MsgChainUnlocked;
    }

    private int OpenGrate()
    {
        ObjectPlacementState objects = gameState.Objects;
        CaveTimingState cave = gameState.Cave;
        ParsedCommandState command = gameState.Command;

        if (!gameState.Here(GameConstants.Keys))
            return GameConstants.MsgNoKeys;

        if (cave.Closing)
        {
            if (!cave.Panic)
            {
                cave.Clock2 = GameConstants.ClosingExitPanicClock;
                cave.Panic = true;
            }

            return GameConstants.MsgExitClosedUseMainOffice;
        }

        int message = GameConstants.MsgAlreadyLocked + objects.PropertyOf(GameConstants.Grate);
        gameState.SetObjectProperty(GameConstants.Grate, command.Verb == GameConstants.Lock ? 0 : 1);
        return message + 2 * objects.PropertyOf(GameConstants.Grate);
    }

    private void VSay()
    {
        ParsedCommandState command = gameState.Command;

        string said = string.Equals(command.Word1, "say", StringComparison.OrdinalIgnoreCase)
            ? command.Word2
            : command.Word1;
        Console.WriteLine("Okay.");
        if (!string.IsNullOrWhiteSpace(said))
            Console.WriteLine(said);
    }

    private void VOn()
    {
        CaveTimingState cave = gameState.Cave;
        ParsedCommandState command = gameState.Command;

        if (!gameState.Here(GameConstants.Lamp))
        {
            ActSpeak(command.Verb);
            return;
        }

        if (cave.LampLimit < 0)
        {
            Speak(GameConstants.MsgLampOut);
            return;
        }

        gameState.SetObjectProperty(GameConstants.Lamp, 1);
        Speak(GameConstants.MsgLampOn);
        if (cave.WizardDark)
        {
            cave.WizardDark = false;
            showLocationDescription(false);
        }
    }

    private void VOff()
    {
        ParsedCommandState command = gameState.Command;

        if (!gameState.Here(GameConstants.Lamp))
        {
            ActSpeak(command.Verb);
            return;
        }

        gameState.SetObjectProperty(GameConstants.Lamp, 0);
        Speak(GameConstants.MsgLampOff);
    }

    private void VWave()
    {
        ObjectPlacementState objects = gameState.Objects;
        CaveTimingState cave = gameState.Cave;
        ParsedCommandState command = gameState.Command;

        int objectId = command.Object;
        if (!gameState.Toting(objectId) &&
            (objectId != GameConstants.Rod || !gameState.Toting(GameConstants.Rod2)))
        {
            Speak(GameConstants.MsgNotCarryingIt);
        }
        else if (objectId != GameConstants.Rod ||
            !gameState.At(GameConstants.Fissure) ||
            !gameState.Toting(objectId) ||
            cave.Closing)
        {
            ActSpeak(command.Verb);
        }
        else
        {
            gameState.SetObjectProperty(GameConstants.Fissure, 1 - objects.PropertyOf(GameConstants.Fissure));
            printObjectMessage(GameConstants.Fissure, 2 - objects.PropertyOf(GameConstants.Fissure));
        }
    }

    private void VKill()
    {
        GamePositionState position = gameState.Position;
        ObjectPlacementState objects = gameState.Objects;
        CaveTimingState cave = gameState.Cave;
        TreasureProgressState progress = gameState.TreasureProgress;
        ParsedCommandState command = gameState.Command;

        int message;
        switch (command.Object)
        {
            case GameConstants.Bird:
                if (cave.Closed)
                    message = GameConstants.MsgLeaveBirdAlone;
                else
                {
                    gameState.Destroy(GameConstants.Bird);
                    gameState.SetObjectProperty(GameConstants.Bird, 0);
                    if (objects.LocationOf(GameConstants.Snake) == GameConstants.HallOfMountainKingLocation)
                        progress.TreasuresLostToEndgame++;
                    message = GameConstants.MsgBirdDies;
                }
                break;
            case 0:
                message = GameConstants.MsgNothingToAttack;
                break;
            case GameConstants.Clam:
            case GameConstants.Oyster:
                message = GameConstants.MsgShellImpervious;
                break;
            case GameConstants.Snake:
                message = GameConstants.MsgAttackingSnakeDangerous;
                break;
            case GameConstants.Dwarf:
                if (cave.Closed)
                    dwarfEnd(GameConstants.MsgDwarvesAwakenGetYou);
                message = GameConstants.MsgBareHands;
                break;
            case GameConstants.Troll:
                message = GameConstants.MsgTrollTooTough;
                break;
            case GameConstants.Bear:
                message = GameConstants.MsgBearBareHandsBase + (objects.PropertyOf(GameConstants.Bear) + 1) / 2;
                break;
            case GameConstants.Dragon:
                if (objects.PropertyOf(GameConstants.Dragon) != 0)
                {
                    message = GameConstants.MsgDragonAlreadyDead;
                    break;
                }

                if (!askYesNo(GameConstants.MsgBareHands, 0, 0))
                    return;

                printObjectMessage(GameConstants.Dragon, 1);
                gameState.SetObjectProperty(GameConstants.Dragon, 2);
                gameState.SetObjectProperty(GameConstants.Rug, 0);
                gameState.MoveObject(GameConstants.Dragon + GameConstants.MaxObjects, -1);
                gameState.MoveObject(GameConstants.Rug + GameConstants.MaxObjects, 0);
                gameState.MoveObject(GameConstants.Dragon, GameConstants.DragonCenterLocation);
                gameState.MoveObject(GameConstants.Rug, GameConstants.DragonCenterLocation);
                for (int item = 1; item < GameConstants.MaxObjects; item++)
                {
                    if (objects.LocationOf(item) == GameConstants.DragonSouthSideLocation ||
                        objects.LocationOf(item) == GameConstants.DragonNorthSideLocation)
                    {
                        gameState.MoveObject(item, GameConstants.DragonCenterLocation);
                    }
                }

                position.NewLocation = GameConstants.DragonCenterLocation;
                return;
            default:
                ActSpeak(command.Verb);
                return;
        }

        Speak(message);
    }

    private void VPour()
    {
        GamePositionState position = gameState.Position;
        WorldMapState world = gameState.World;
        ObjectPlacementState objects = gameState.Objects;
        ParsedCommandState command = gameState.Command;

        int objectId = command.Object;
        if (objectId == GameConstants.Bottle || objectId == 0)
            objectId = gameState.Liq();
        if (objectId == 0)
        {
            NeedObject();
            return;
        }

        if (!gameState.Toting(objectId))
        {
            ActSpeak(command.Verb);
            return;
        }

        if (objectId != GameConstants.Oil && objectId != GameConstants.Water)
        {
            Speak(GameConstants.MsgCantPourThat);
            return;
        }

        gameState.SetObjectProperty(GameConstants.Bottle, 1);
        gameState.SetObjectLocation(objectId, 0);

        if (gameState.At(GameConstants.Plant))
        {
            if (objectId != GameConstants.Water)
            {
                Speak(GameConstants.MsgPlantWantsWater);
            }
            else
            {
                printObjectMessage(GameConstants.Plant, objects.PropertyOf(GameConstants.Plant) + 1);
                gameState.SetObjectProperty(GameConstants.Plant, (objects.PropertyOf(GameConstants.Plant) + 2) % 6);
                gameState.SetObjectProperty(GameConstants.Plant2, objects.PropertyOf(GameConstants.Plant) / 2);
                showLocationDescription(false);
            }
        }
        else if (gameState.At(GameConstants.Door))
        {
            gameState.SetObjectProperty(GameConstants.Door, objectId == GameConstants.Oil ? 1 : 0);
            Speak(GameConstants.MsgDoorRustedBase + objects.PropertyOf(GameConstants.Door));
        }
        else
        {
            Speak(GameConstants.MsgBottleEmptyGroundWet);
        }
    }

    private void VEat()
    {
        ParsedCommandState command = gameState.Command;

        switch (command.Object)
        {
            case GameConstants.Food:
                gameState.Destroy(GameConstants.Food);
                Speak(GameConstants.MsgFoodDelicious);
                break;
            case GameConstants.Bird:
            case GameConstants.Snake:
            case GameConstants.Clam:
            case GameConstants.Oyster:
            case GameConstants.Dwarf:
            case GameConstants.Dragon:
            case GameConstants.Troll:
            case GameConstants.Bear:
                Speak(GameConstants.MsgLostAppetite);
                break;
            default:
                ActSpeak(command.Verb);
                break;
        }
    }

    private void VDrink()
    {
        ParsedCommandState command = gameState.Command;

        if (command.Object != GameConstants.Water)
        {
            Speak(GameConstants.MsgDontBeRidiculous);
        }
        else if (gameState.Liq() != GameConstants.Water || !gameState.Here(GameConstants.Bottle))
        {
            ActSpeak(command.Verb);
        }
        else
        {
            gameState.SetObjectProperty(GameConstants.Bottle, 1);
            gameState.SetObjectLocation(GameConstants.Water, 0);
            Speak(GameConstants.MsgBottleWaterNowEmpty);
        }
    }

    private void VThrow()
    {
        GamePositionState position = gameState.Position;
        ObjectPlacementState objects = gameState.Objects;
        DwarfPirateState dwarves = gameState.Dwarves;
        ParsedCommandState command = gameState.Command;

        int objectId = command.Object;
        if (gameState.Toting(GameConstants.Rod2) && objectId == GameConstants.Rod && !gameState.Toting(GameConstants.Rod))
        {
            objectId = GameConstants.Rod2;
            command.Object = objectId;
        }

        if (!gameState.Toting(objectId))
        {
            ActSpeak(command.Verb);
            return;
        }

        if (gameState.At(GameConstants.Troll) && objectId >= GameConstants.Nugget && objectId < GameConstants.MaxObjects)
        {
            Speak(GameConstants.MsgTrollTakesTreasure);
            gameState.Drop(objectId, 0);
            gameState.MoveObject(GameConstants.Troll, 0);
            gameState.MoveObject(GameConstants.Troll + GameConstants.MaxObjects, 0);
            gameState.Drop(GameConstants.Troll2, GameConstants.TrollBridgeNearSideLocation);
            gameState.Drop(GameConstants.Troll2 + GameConstants.MaxObjects, GameConstants.TrollBridgeFarSideLocation);
            GameState.Juggle(GameConstants.Chasm);
            return;
        }

        if (objectId == GameConstants.Food && gameState.Here(GameConstants.Bear))
        {
            command.Object = GameConstants.Bear;
            VFeed();
            return;
        }

        if (objectId != GameConstants.Axe)
        {
            VDrop();
            return;
        }

        int message;
        int dwarf = gameState.DCheck();
        if (dwarf != 0)
        {
            message = GameConstants.MsgDwarfDodgesAxe;
            if (GameState.Pct(random, GameConstants.AxeKillsDwarfChance))
            {
                dwarves.Seen[dwarf] = false;
                dwarves.Locations[dwarf] = 0;
                message = GameConstants.MsgKilledDwarf;
                dwarves.KillCount++;
                if (dwarves.KillCount == 1)
                    message = GameConstants.MsgKilledDwarfSmoke;
            }
        }
        else if (gameState.At(GameConstants.Dragon) && objects.PropertyOf(GameConstants.Dragon) == 0)
        {
            message = GameConstants.MsgAxeBouncesOffDragon;
        }
        else if (gameState.At(GameConstants.Troll))
        {
            message = GameConstants.MsgTrollRejectsAxe;
        }
        else if (gameState.Here(GameConstants.Bear) && objects.PropertyOf(GameConstants.Bear) == 0)
        {
            Speak(GameConstants.MsgAxeMissesBear);
            gameState.Drop(GameConstants.Axe, position.Location);
            objects.SetFixedLocation(GameConstants.Axe, -1);
            gameState.SetObjectProperty(GameConstants.Axe, 1);
            GameState.Juggle(GameConstants.Bear);
            return;
        }
        else
        {
            command.Verb = GameConstants.Kill;
            command.Object = 0;
            IVKill();
            return;
        }

        Speak(message);
        gameState.Drop(GameConstants.Axe, position.Location);
        showLocationDescription(false);
    }

    private void VFind()
    {
        GamePositionState position = gameState.Position;
        DwarfPirateState dwarves = gameState.Dwarves;
        CaveTimingState cave = gameState.Cave;
        ParsedCommandState command = gameState.Command;

        int objectId = command.Object;
        if (gameState.Toting(objectId))
            Speak(GameConstants.MsgAlreadyCarryingIt);
        else if (cave.Closed)
            Speak(GameConstants.MsgDaresayNearby);
        else if (gameState.DCheck() != 0 && dwarves.ActivationLevel >= 2 && objectId == GameConstants.Dwarf)
            Speak(GameConstants.MsgWantRightHere);
        else if (gameState.At(objectId) ||
            (gameState.Liq() == objectId && gameState.Here(GameConstants.Bottle)) ||
            objectId == gameState.LiqLoc(position.Location))
            Speak(GameConstants.MsgWantRightHere);
        else
            ActSpeak(command.Verb);
    }

    private void VFill()
    {
        GamePositionState position = gameState.Position;
        WorldMapState world = gameState.World;
        ParsedCommandState command = gameState.Command;

        switch (command.Object)
        {
            case GameConstants.Bottle:
                if (gameState.Liq() != 0)
                {
                    Speak(GameConstants.MsgBottleAlreadyFull);
                }
                else
                {
                    int liquidHere = gameState.LiqLoc(position.Location);
                    if (liquidHere == 0)
                    {
                        Speak(GameConstants.MsgNothingToFillBottle);
                    }
                    else
                    {
                        gameState.SetObjectProperty(GameConstants.Bottle, world.LocationConditions[position.Location] & GameConstants.WatOil);
                        int liquid = gameState.Liq();
                        if (gameState.Toting(GameConstants.Bottle))
                            gameState.SetObjectLocation(liquid, -1);
                        Speak(liquid == GameConstants.Oil
                            ? GameConstants.MsgBottleFullOil
                            : GameConstants.MsgBottleFullWater);
                    }
                }
                break;
            case GameConstants.Vase:
                if (gameState.LiqLoc(position.Location) == 0)
                {
                    Speak(GameConstants.MsgNoWaterForVase);
                }
                else if (!gameState.Toting(GameConstants.Vase))
                {
                    Speak(GameConstants.MsgNotCarryingIt);
                }
                else
                {
                    Speak(GameConstants.MsgVaseShatteredByTemperature);
                    VDrop();
                }
                break;
            default:
                Speak(GameConstants.MsgNotCarryingIt);
                break;
        }
    }

    private void VFeed()
    {
        ObjectPlacementState objects = gameState.Objects;
        CaveTimingState cave = gameState.Cave;
        TreasureProgressState progress = gameState.TreasureProgress;
        DwarfPirateState dwarves = gameState.Dwarves;
        ParsedCommandState command = gameState.Command;

        int message;
        switch (command.Object)
        {
            case GameConstants.Bird:
                message = GameConstants.MsgBirdNotHungry;
                break;
            case GameConstants.Dwarf:
                if (!gameState.Here(GameConstants.Food))
                {
                    ActSpeak(command.Verb);
                    return;
                }
                dwarves.ActivationLevel++;
                message = GameConstants.MsgDwarfFoodAngers;
                break;
            case GameConstants.Bear:
                if (!gameState.Here(GameConstants.Food))
                {
                    if (objects.PropertyOf(GameConstants.Bear) == 0)
                        message = GameConstants.MsgNothingWantsFood;
                    else if (objects.PropertyOf(GameConstants.Bear) == 3)
                        message = GameConstants.MsgDontBeRidiculous;
                    else
                    {
                        ActSpeak(command.Verb);
                        return;
                    }
                    break;
                }

                gameState.Destroy(GameConstants.Food);
                gameState.SetObjectProperty(GameConstants.Bear, 1);
                objects.SetFixedLocation(GameConstants.Axe, 0);
                gameState.SetObjectProperty(GameConstants.Axe, 0);
                message = GameConstants.MsgBearEatsFood;
                break;
            case GameConstants.Dragon:
                message = objects.PropertyOf(GameConstants.Dragon) != 0
                    ? GameConstants.MsgDontBeRidiculous
                    : GameConstants.MsgNothingWantsFood;
                break;
            case GameConstants.Troll:
                message = GameConstants.MsgTrollWantsTreasure;
                break;
            case GameConstants.Snake:
                if (cave.Closed || !gameState.Here(GameConstants.Bird))
                {
                    message = GameConstants.MsgNothingWantsFood;
                    break;
                }

                message = GameConstants.MsgSnakeDevoursBird;
                gameState.Destroy(GameConstants.Bird);
                gameState.SetObjectProperty(GameConstants.Bird, 0);
                progress.TreasuresLostToEndgame++;
                break;
            default:
                message = GameConstants.MsgExplainHow;
                break;
        }

        Speak(message);
    }

    private void VRead()
    {
        CaveTimingState cave = gameState.Cave;
        HintTrackingState hints = gameState.Hints;
        TreasureProgressState progress = gameState.TreasureProgress;
        ParsedCommandState command = gameState.Command;

        if (DarknessManager.IsDark(gameState))
        {
            SpeakObjectNotHere(command.Object);
            return;
        }

        switch (command.Object)
        {
            case GameConstants.Magazine:
                Speak(GameConstants.MsgMagazineDwarvish);
                break;
            case GameConstants.Tablet:
                Speak(GameConstants.MsgTabletCongratulations);
                break;
            case GameConstants.Message:
                Speak(GameConstants.MsgPirateMazeMessage);
                break;
            case GameConstants.Oyster:
                if (!gameState.Toting(GameConstants.Oyster) || !cave.Closed)
                {
                    ActSpeak(command.Verb);
                    break;
                }

                if ((hints.AvailableMask & GameConstants.HintO) != 0)
                {
                    if (askYesNo(GameConstants.MsgPromptReadOysterClue, GameConstants.MsgOysterClue, GameConstants.MsgOk))
                    {
                        progress.HintsAccepted++;
                        hints.AvailableMask &= ~GameConstants.HintO;
                    }
                }
                else
                {
                    Speak(GameConstants.MsgOysterClueRepeated);
                }
                break;
            default:
                ActSpeak(command.Verb);
                break;
        }
    }

    private void VBlast()
    {
        ObjectPlacementState objects = gameState.Objects;
        CaveTimingState cave = gameState.Cave;
        TreasureProgressState progress = gameState.TreasureProgress;
        ParsedCommandState command = gameState.Command;

        if (objects.IsPropertyNegative(GameConstants.Rod2) || !cave.Closed)
        {
            ActSpeak(command.Verb);
            return;
        }

        progress.Bonus = GameConstants.MsgBlastWins;
        if (gameState.Position.Location == GameConstants.RepositoryNortheastLocation)
            progress.Bonus = GameConstants.MsgBlastLavaDeath;
        if (gameState.Here(GameConstants.Rod2))
            progress.Bonus = GameConstants.MsgBlastSelfDeath;

        Speak(progress.Bonus);
        normalEnd();
    }

    private void VBreak()
    {
        GamePositionState position = gameState.Position;
        ObjectPlacementState objects = gameState.Objects;
        CaveTimingState cave = gameState.Cave;
        ParsedCommandState command = gameState.Command;

        if (command.Object == GameConstants.Mirror)
        {
            if (cave.Closed)
            {
                Speak(GameConstants.MsgMirrorBreaks);
                dwarfEnd(GameConstants.MsgDwarvesAwakenGetYou);
                return;
            }

            Speak(GameConstants.MsgTooFarToReach);
        }
        else if (command.Object == GameConstants.Vase && objects.PropertyOf(GameConstants.Vase) == 0)
        {
            if (gameState.Toting(GameConstants.Vase))
                gameState.Drop(GameConstants.Vase, position.Location);
            gameState.SetObjectProperty(GameConstants.Vase, 2);
            objects.SetFixedLocation(GameConstants.Vase, -1);
            Speak(GameConstants.MsgVaseHurledGround);
        }
        else
        {
            ActSpeak(command.Verb);
        }
    }

    private void VWake()
    {
        ParsedCommandState command = gameState.Command;

        if (command.Object != GameConstants.Dwarf || !gameState.Cave.Closed)
            ActSpeak(command.Verb);
        else
            dwarfEnd(GameConstants.MsgWakeDwarfClosed);
    }

    private void AddCandidate(int objectId, bool condition, ref int candidate, ref bool ambiguous)
    {
        if (!condition || ambiguous)
            return;

        if (candidate != 0)
        {
            ambiguous = true;
            return;
        }

        candidate = objectId;
    }

    private void ActSpeak(int verb)
    {
        int message = gameState.GetActionMessageId(verb);
        if (message != 0)
            Speak(message);
    }

    private void Speak(int messageId)
    {
        if (messageId == 0)
            return;

        if (messageId == GameConstants.MsgOk)
        {
            Console.WriteLine("OK");
            return;
        }

        Console.WriteLine(AdventureData.Message(messageId));
    }

    private void SpeakObjectNotHere(int objectId)
    {
        Console.WriteLine($"I see no {GetObjectWord(objectId)} here.");
    }

    private void NeedObject()
    {
        ParsedCommandState command = gameState.Command;

        string word = IsVerbWord(command.Word1) ? command.Word1 : command.Word2;
        if (string.IsNullOrWhiteSpace(word))
            word = "do";
        Console.WriteLine($"{word} what?");
    }

    private bool IsVerbWord(string word)
    {
        return !string.IsNullOrWhiteSpace(word) &&
            AdventureData.AnalyzeWord(word, out int type, out _) &&
            type == Vocabulary.WordTypes.Verb;
    }

    private string GetObjectWord(int objectId)
    {
        ParsedCommandState command = gameState.Command;

        if (IsObjectWord(command.Word1, objectId))
            return command.Word1;
        if (IsObjectWord(command.Word2, objectId))
            return command.Word2;

        return AdventureData.ObjectNameOrDefault(objectId).ToLowerInvariant();
    }

    private static bool IsObjectWord(string word, int objectId)
    {
        return !string.IsNullOrWhiteSpace(word) &&
            AdventureData.AnalyzeWord(word, out int type, out int value) &&
            type == Vocabulary.WordTypes.Object &&
            value == objectId;
    }

    private void ShowInventory()
    {
        List<int> carriedObjects = gameState.GetCarriedObjects();

        if (carriedObjects.Count == 0)
        {
            Console.WriteLine("You're not carrying anything.");
            return;
        }

        Console.WriteLine("You are currently holding the following:");
        foreach (int objectId in carriedObjects)
        {
            if (AdventureData.TryGetObject(objectId, out GameObjectData? objectData))
            {
                Console.WriteLine($"  {objectData.Name}");
            }
            else
            {
                Console.WriteLine($"  Object #{objectId}");
            }
        }
    }

}

