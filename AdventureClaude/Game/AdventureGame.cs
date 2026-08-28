namespace AdventureClaude.Game;

using System;
using System.Collections.Generic;
using AdventureClaude.Data;
using AdventureClaude.Models;

/// <summary>
/// Core adventure game engine.
/// Converted from the main game logic in ADVENT.C and related files.
/// </summary>
public class AdventureGame
    {
        // Matches the C reference's srand(511) call for deterministic parity/regression routes.
        private const int ReferenceRandomSeed = 511;

        private readonly GameState gameState;
        private readonly InputParser inputParser;
        private readonly TravelEngine travelEngine;
        private readonly Random random;

        public AdventureGame()
            : this(new Random())
        {
        }

        public AdventureGame(Random random)
        {
            gameState = new GameState();
            inputParser = new InputParser();
            this.random = random;
            travelEngine = new TravelEngine(gameState, this.random, PrintObjectMessage, HandleDeath);
        }

        public static AdventureGame CreateForReferenceParity()
        {
            return new AdventureGame(new CReferenceRandom(ReferenceRandomSeed));
        }

        /// <summary>
        /// Initializes and starts a new game.
        /// Equivalent to main() and initplay() functions.
        /// </summary>
        public void StartNewGame()
        {
            gameState.InitializeGame();
            
            // Ask if player wants instructions
            Console.WriteLine("Welcome to Adventure!!  Would you like instructions?");
            string response = Console.ReadLine() ?? string.Empty;
            
            if (IsYesResponse(response))
            {
                ShowInstructions();
                gameState.Cave.LampLimit = GameConstants.InstructionLampLimit;
                gameState.TreasureProgress.HintsAccepted++;
            }
            else
            {
                gameState.Cave.LampLimit = GameConstants.DefaultLampLimit;
            }

            // Main game loop
            while (!gameState.TreasureProgress.SaveRequested)
            {
                Turn();
            }
        }

        /// <summary>
        /// Handles one turn of the game.
        /// Converted from turn() function.
        /// </summary>
        private void Turn()
        {
            if (!RunTurnLifecycleBeforeInput())
                return;

            // Get player input
            int verb;
            int objectId;
            int motion;
            while (true)
            {
                Console.Write("> ");
                string input = Console.ReadLine() ?? string.Empty;

                if (inputParser.ParseInput(input, gameState, out verb, out objectId, out motion))
                    break;

                if (gameState.TreasureProgress.SaveRequested)
                    return;
            }

            // Store parsed values in game state
            gameState.Command.Verb = verb;
            gameState.Command.Object = objectId;
            gameState.Command.Motion = motion;

            // Process the command
            ProcessCommand();
        }

        /// <summary>
        /// Processes the parsed command.
        /// </summary>
        private void ProcessCommand()
        {
            ParsedCommandState command = gameState.Command;

            // Handle motion commands
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

        /// <summary>
        /// Applies the NewLocation chosen by DoTravel. This is the movement subset of TURN.C turn().
        /// </summary>
        private bool RunTurnLifecycleBeforeInput()
        {
            ApplyClosingExitGuard();
            ApplyDwarfBlock();
            RunDwarves();
            ApplyLocationChange();

            if (gameState.TreasureProgress.SaveRequested || gameState.Position.Location == 0)
                return false;

            ApplyClosedInventoryState();
            gameState.Cave.WizardDark = DarknessManager.IsDark(gameState);
            if (gameState.Objects.KnifeLocation > 0 && gameState.Objects.KnifeLocation != gameState.Position.Location)
                gameState.Objects.KnifeLocation = 0;

            if (RunSpecialTimer())
                return false;

            TryLocationHint();
            return !gameState.TreasureProgress.SaveRequested;
        }

        private void ApplyClosingExitGuard()
        {
            GamePositionState position = gameState.Position;
            CaveTimingState cave = gameState.Cave;

            if (position.NewLocation >= GameConstants.BelowGrateLocation || position.NewLocation == 0 || !cave.Closing)
                return;

            Speak(GameConstants.MsgExitClosedUseMainOffice);
            position.NewLocation = position.Location;
            if (!cave.Panic)
                cave.Clock2 = GameConstants.ClosingExitPanicClock;
            cave.Panic = true;
        }

        private void ApplyDwarfBlock()
        {
            GamePositionState position = gameState.Position;
            DwarfPirateState dwarves = gameState.Dwarves;

            if (position.NewLocation == position.Location ||
                gameState.Forced(position.Location) ||
                gameState.LocationHasFlag(position.Location, GameConstants.NoPirat))
            {
                return;
            }

            for (int i = 1; i < GameConstants.MaxDwarves - 1; i++)
            {
                if (dwarves.PreviousLocations[i] == position.NewLocation && dwarves.Seen[i])
                {
                    position.NewLocation = position.Location;
                    Speak(GameConstants.MsgDwarfBlocksWay);
                    return;
                }
            }
        }

        private void RunDwarves()
        {
            GamePositionState position = gameState.Position;
            DwarfPirateState dwarves = gameState.Dwarves;
            ObjectPlacementState objects = gameState.Objects;

            if (position.NewLocation == 0 ||
                gameState.Forced(position.NewLocation) ||
                gameState.LocationHasFlag(position.NewLocation, GameConstants.NoPirat))
            {
                return;
            }

            if (dwarves.ActivationLevel == 0)
            {
                if (position.NewLocation > GameConstants.DwarfActiveLocationThreshold)
                    dwarves.ActivationLevel++;
                return;
            }

            if (dwarves.ActivationLevel == 1)
            {
                if (position.NewLocation < GameConstants.DwarfActiveLocationThreshold ||
                    GameState.Pct(random, GameConstants.DwarfActivationChance))
                    return;

                dwarves.ActivationLevel++;
                for (int i = 1; i < 3; i++)
                {
                    if (GameState.Pct(random, 50))
                        dwarves.Locations[GameState.RRand(random, 1, GameConstants.DwarfCullRollUpperBound)] = 0;
                }

                for (int i = 1; i < GameConstants.MaxDwarves - 1; i++)
                {
                    if (dwarves.Locations[i] == position.NewLocation)
                        dwarves.Locations[i] = dwarves.AlternateLocation;
                    dwarves.PreviousLocations[i] = dwarves.Locations[i];
                }

                Speak(GameConstants.MsgDwarfWarningAxeMissed);
                gameState.Drop(GameConstants.Axe, position.NewLocation);
                return;
            }

            int dwarfCount = 0;
            int attacks = 0;
            int hits = 0;

            for (int i = 1; i < GameConstants.MaxDwarves; i++)
            {
                if (dwarves.Locations[i] == 0)
                    continue;

                int candidateLocation = dwarves.PreviousLocations[i];
                for (int attempt = 1; attempt < GameConstants.DwarfLocationRollAttempts; attempt++)
                {
                    candidateLocation = GameState.RRand(
                        random,
                        GameConstants.DwarfLocationRollLow,
                        GameConstants.DwarfLocationRollHigh);
                    if (candidateLocation != dwarves.PreviousLocations[i] &&
                        candidateLocation != dwarves.Locations[i])
                    {
                        break;
                    }
                }

                dwarves.PreviousLocations[i] = dwarves.Locations[i];
                dwarves.Locations[i] = candidateLocation;

                dwarves.Seen[i] =
                    (dwarves.Seen[i] && position.NewLocation >= GameConstants.DwarfActiveLocationThreshold) ||
                    dwarves.Locations[i] == position.NewLocation ||
                    dwarves.PreviousLocations[i] == position.NewLocation;

                if (!dwarves.Seen[i])
                    continue;

                dwarves.Locations[i] = position.NewLocation;
                if (i == GameConstants.MaxDwarves - 1)
                {
                    DoPirate();
                    continue;
                }

                dwarfCount++;
                if (dwarves.PreviousLocations[i] == dwarves.Locations[i])
                {
                    attacks++;
                    if (objects.KnifeLocation >= 0)
                        objects.KnifeLocation = position.NewLocation;
                    if (GameState.RRand(random, 0, GameConstants.DwarfKnifeRollHigh) <
                        GameConstants.DwarfKnifeHitChancePerLevel * (dwarves.ActivationLevel - 2))
                        hits++;
                }
            }

            if (dwarfCount == 0)
                return;

            if (dwarfCount > 1)
                Console.WriteLine($"There are {dwarfCount} threatening little dwarves in the room with you!");
            else
                Speak(GameConstants.MsgThreateningDwarfHere);

            if (attacks == 0)
                return;

            if (dwarves.ActivationLevel == 2)
                dwarves.ActivationLevel++;

            int messageBase;
            if (attacks > 1)
            {
                Console.WriteLine($"{attacks} of them throw knives at you!!");
                messageBase = GameConstants.MsgNoDwarvesHit;
            }
            else
            {
                Speak(GameConstants.MsgKnifeThrown);
                messageBase = GameConstants.MsgKnifeMisses;
            }

            if (hits <= 1)
            {
                Speak(hits + messageBase);
                if (hits == 0)
                    return;
            }
            else
            {
                Console.WriteLine($"{hits} of them get you !!!");
            }

            position.OldLocation2 = position.NewLocation;
            HandleDeath();
        }

        private void DoPirate()
        {
            GamePositionState position = gameState.Position;
            DwarfPirateState dwarves = gameState.Dwarves;
            ObjectPlacementState objects = gameState.Objects;
            TreasureProgressState progress = gameState.TreasureProgress;

            if (position.NewLocation == objects.ChestLocation ||
                objects.PropertyOf(GameConstants.Chest) >= 0)
            {
                return;
            }

            int nearbyTreasures = 0;
            for (int treasure = GameConstants.Nugget; treasure <= GameConstants.MaxTreasures; treasure++)
            {
                if (treasure == GameConstants.Pyramid &&
                    (position.NewLocation == objects.LocationOf(GameConstants.Pyramid) ||
                    position.NewLocation == objects.LocationOf(GameConstants.Emerald)))
                {
                    continue;
                }

                if (gameState.Toting(treasure))
                {
                    PirateStealsTreasure();
                    return;
                }

                if (gameState.Here(treasure))
                    nearbyTreasures++;
            }

            if (progress.UndiscoveredTreasureCount == progress.TreasuresLostToEndgame + 1 &&
                nearbyTreasures == 0 &&
                objects.LocationOf(GameConstants.Chest) == 0 &&
                gameState.Here(GameConstants.Lamp) &&
                objects.PropertyOf(GameConstants.Lamp) == 1)
            {
                Speak(GameConstants.MsgPirateSpottedChest);
                gameState.MoveObject(GameConstants.Chest, objects.ChestLocation);
                gameState.MoveObject(GameConstants.Message, objects.ChestLocation2);
                dwarves.Locations[GameConstants.MaxDwarves - 1] = objects.ChestLocation;
                dwarves.PreviousLocations[GameConstants.MaxDwarves - 1] = objects.ChestLocation;
                dwarves.Seen[GameConstants.MaxDwarves - 1] = false;
                return;
            }

            if (dwarves.PreviousLocations[GameConstants.MaxDwarves - 1] !=
                dwarves.Locations[GameConstants.MaxDwarves - 1] &&
                GameState.Pct(random, GameConstants.PirateRustleChance))
            {
                Speak(GameConstants.MsgPirateRustling);
            }
        }

        private void PirateStealsTreasure()
        {
            GamePositionState position = gameState.Position;
            DwarfPirateState dwarves = gameState.Dwarves;
            ObjectPlacementState objects = gameState.Objects;

            Speak(GameConstants.MsgPirateStealsBooty);
            if (objects.LocationOf(GameConstants.Message) == 0)
                gameState.MoveObject(GameConstants.Chest, objects.ChestLocation);
            gameState.MoveObject(GameConstants.Message, objects.ChestLocation2);

            for (int treasure = GameConstants.Nugget; treasure <= GameConstants.MaxTreasures; treasure++)
            {
                if (treasure == GameConstants.Pyramid &&
                    (position.NewLocation == objects.LocationOf(GameConstants.Pyramid) ||
                    position.NewLocation == objects.LocationOf(GameConstants.Emerald)))
                {
                    continue;
                }

                if (gameState.At(treasure) && objects.FixedLocationOf(treasure) == 0)
                    gameState.Carry(treasure, position.NewLocation);
                if (gameState.Toting(treasure))
                    gameState.Drop(treasure, objects.ChestLocation);
            }

            dwarves.Locations[GameConstants.MaxDwarves - 1] = objects.ChestLocation;
            dwarves.PreviousLocations[GameConstants.MaxDwarves - 1] = objects.ChestLocation;
            dwarves.Seen[GameConstants.MaxDwarves - 1] = false;
        }

        private void ApplyLocationChange()
        {
            int forcedMoves = 0;
            GamePositionState position = gameState.Position;
            WorldMapState world = gameState.World;
            CaveTimingState cave = gameState.Cave;
            ParsedCommandState command = gameState.Command;

            while (position.Location != position.NewLocation && !gameState.TreasureProgress.SaveRequested)
            {
                bool forceLongDescription = position.Location == 0;
                if (forceLongDescription)
                    world.VisitedLocations[position.NewLocation] =
                        (short)((world.VisitedLocations[position.NewLocation] + 3) & ~3);

                position.Turns++;
                position.Location = position.NewLocation;

                if (position.Location == 0)
                {
                    HandleDeath();
                    return;
                }

                if (gameState.Forced(position.Location))
                {
                    ShowLocationDescription(forceLongDescription);
                    command.Motion = GameConstants.DefaultTravelVerb;
                    travelEngine.DoMove();
                    if (++forcedMoves >= GameConstants.MaxForcedMoves)
                    {
                        Console.WriteLine("[Warning: Maximum forced movement cascade depth reached]");
                        return;
                    }
                    continue;
                }

                if (cave.WizardDark && DarknessManager.IsDark(gameState) && GameState.Pct(random, 35))
                {
                    Console.WriteLine(AdventureData.Message(GameConstants.MsgFellIntoPit));
                    position.OldLocation2 = position.Location;
                    HandleDeath();
                    return;
                }

                ShowLocationDescription(forceLongDescription);
                if (!DarknessManager.IsDark(gameState))
                    world.VisitedLocations[position.Location]++;
            }
        }

        private void PrintObjectMessage(int objectId, int state)
        {
            if (!AdventureData.TryGetObjectRoomDescription(objectId, state, out string message))
                return;

            Console.WriteLine(message);
        }

        private void ApplyClosedInventoryState()
        {
            ObjectPlacementState objects = gameState.Objects;

            if (!gameState.Cave.Closed)
                return;

            if (objects.IsPropertyNegative(GameConstants.Oyster) && gameState.Toting(GameConstants.Oyster))
                PrintObjectMessage(GameConstants.Oyster, 1);

            for (int item = 1; item <= GameConstants.MaxObjects; item++)
            {
                if (gameState.Toting(item) && objects.IsPropertyNegative(item))
                    gameState.SetObjectProperty(item, -1 - objects.PropertyOf(item));
            }
        }

        private bool RunSpecialTimer()
        {
            GamePositionState position = gameState.Position;
            ObjectPlacementState objects = gameState.Objects;
            CaveTimingState cave = gameState.Cave;
            TreasureProgressState progress = gameState.TreasureProgress;
            DwarfPirateState dwarves = gameState.Dwarves;

            progress.FooBar = progress.FooBar > 0 ? -progress.FooBar : 0;
            progress.DescriptionDetailMask = 2;

            if (progress.UndiscoveredTreasureCount == 0 &&
                position.Location >= GameConstants.HallOfMistsLocation &&
                position.Location != GameConstants.Y2Location)
                cave.Clock1--;

            if (cave.Clock1 == 0)
            {
                gameState.SetObjectProperty(GameConstants.Grate, 0);
                gameState.SetObjectProperty(GameConstants.Fissure, 0);
                for (int i = 1; i < GameConstants.MaxDwarves; i++)
                    dwarves.Seen[i] = false;

                gameState.MoveObject(GameConstants.Troll, 0);
                gameState.MoveObject(GameConstants.Troll + GameConstants.MaxObjects, 0);
                gameState.MoveObject(GameConstants.Troll2, GameConstants.TrollBridgeNearSideLocation);
                gameState.MoveObject(GameConstants.Troll2 + GameConstants.MaxObjects, GameConstants.TrollBridgeFarSideLocation);
                GameState.Juggle(GameConstants.Chasm);
                if (objects.PropertyOf(GameConstants.Bear) != 3)
                    gameState.Destroy(GameConstants.Bear);
                gameState.SetObjectProperty(GameConstants.Chain, 0);
                objects.SetFixedLocation(GameConstants.Chain, 0);
                gameState.SetObjectProperty(GameConstants.Axe, 0);
                objects.SetFixedLocation(GameConstants.Axe, 0);
                Speak(GameConstants.MsgCaveClosingSoon);
                cave.Clock1 = -1;
                cave.Closing = true;
                return false;
            }

            if (cave.Clock1 < 0)
                cave.Clock2--;

            if (cave.Clock2 == 0)
            {
                CloseCave();
                return true;
            }

            if (objects.PropertyOf(GameConstants.Lamp) == 1)
                cave.LampLimit--;

            if (cave.LampLimit <= GameConstants.LampWarningTurns &&
                gameState.Here(GameConstants.Batteries) &&
                objects.PropertyOf(GameConstants.Batteries) == 0 &&
                gameState.Here(GameConstants.Lamp))
            {
                Speak(GameConstants.MsgReplaceBatteries);
                gameState.SetObjectProperty(GameConstants.Batteries, 1);
                if (gameState.Toting(GameConstants.Batteries))
                    gameState.Drop(GameConstants.Batteries, position.Location);
                cave.LampLimit += GameConstants.FreshBatteryExtraTurns;
                cave.LampWarning = 0;
                return false;
            }

            if (cave.LampLimit == 0)
            {
                cave.LampLimit--;
                gameState.SetObjectProperty(GameConstants.Lamp, 0);
                if (gameState.Here(GameConstants.Lamp))
                    Speak(GameConstants.MsgLampOut);
                return false;
            }

            if (cave.LampLimit < 0 && position.Location <= GameConstants.DepressionLocation)
            {
                Speak(GameConstants.MsgLampOutAboveGroundEnd);
                progress.GaveUp = true;
                NormalEnd();
                return true;
            }

            if (cave.LampLimit <= GameConstants.LampWarningTurns)
            {
                if (cave.LampWarning != 0 || !gameState.Here(GameConstants.Lamp))
                    return false;

                cave.LampWarning = 1;
                int message = GameConstants.MsgLampGettingDimNeedBatteries;
                if (objects.LocationOf(GameConstants.Batteries) == 0)
                    message = GameConstants.MsgLampDimNeedFreshBatteries;
                if (objects.PropertyOf(GameConstants.Batteries) == 1)
                    message = GameConstants.MsgLampDimNoBatteries;
                Speak(message);
            }

            return false;
        }

        private void CloseCave()
        {
            GamePositionState position = gameState.Position;
            ObjectPlacementState objects = gameState.Objects;
            CaveTimingState cave = gameState.Cave;

            gameState.SetObjectProperty(GameConstants.Bottle, gameState.Put(GameConstants.Bottle, GameConstants.RepositoryNortheastLocation, 1));
            gameState.SetObjectProperty(GameConstants.Plant, gameState.Put(GameConstants.Plant, GameConstants.RepositoryNortheastLocation, 0));
            gameState.SetObjectProperty(GameConstants.Oyster, gameState.Put(GameConstants.Oyster, GameConstants.RepositoryNortheastLocation, 0));
            gameState.SetObjectProperty(GameConstants.Lamp, gameState.Put(GameConstants.Lamp, GameConstants.RepositoryNortheastLocation, 0));
            gameState.SetObjectProperty(GameConstants.Rod, gameState.Put(GameConstants.Rod, GameConstants.RepositoryNortheastLocation, 0));
            gameState.SetObjectProperty(GameConstants.Dwarf, gameState.Put(GameConstants.Dwarf, GameConstants.RepositoryNortheastLocation, 0));

            position.Location = GameConstants.RepositoryNortheastLocation;
            position.OldLocation = GameConstants.RepositoryNortheastLocation;
            position.NewLocation = GameConstants.RepositoryNortheastLocation;

            gameState.Put(GameConstants.Grate, GameConstants.RepositorySouthwestLocation, 0);
            gameState.SetObjectProperty(GameConstants.Snake, gameState.Put(GameConstants.Snake, GameConstants.RepositorySouthwestLocation, 1));
            gameState.SetObjectProperty(GameConstants.Bird, gameState.Put(GameConstants.Bird, GameConstants.RepositorySouthwestLocation, 1));
            gameState.SetObjectProperty(GameConstants.Cage, gameState.Put(GameConstants.Cage, GameConstants.RepositorySouthwestLocation, 0));
            gameState.SetObjectProperty(GameConstants.Rod2, gameState.Put(GameConstants.Rod2, GameConstants.RepositorySouthwestLocation, 0));
            gameState.SetObjectProperty(GameConstants.Pillow, gameState.Put(GameConstants.Pillow, GameConstants.RepositorySouthwestLocation, 0));
            gameState.SetObjectProperty(GameConstants.Mirror, gameState.Put(GameConstants.Mirror, GameConstants.RepositoryNortheastLocation, 0));
            objects.SetFixedLocation(GameConstants.Mirror, GameConstants.RepositorySouthwestLocation);

            for (int item = 1; item <= GameConstants.MaxObjects; item++)
            {
                if (gameState.Toting(item))
                    gameState.Destroy(item);
            }

            Speak(GameConstants.MsgCaveNowClosed);
            cave.Closed = true;
            position.Location = 0;
        }

        private void TryLocationHint()
        {
            GamePositionState position = gameState.Position;
            WorldMapState world = gameState.World;
            ObjectPlacementState objects = gameState.Objects;
            HintTrackingState hints = gameState.Hints;

            if ((world.LocationConditions[position.Location] & hints.AvailableMask) == 0)
            {
                Array.Clear(hints.LocationCounters);
                return;
            }

            switch (world.LocationConditions[position.Location] & GameConstants.Hint)
            {
                case GameConstants.HintF:
                    hints.LocationCounters[GameConstants.HintAreaF]++;
                    if (hints.LocationCounters[GameConstants.HintAreaF] > GameConstants.HintFindCaveTurns &&
                        world.VisitedLocations[GameConstants.DepressionLocation] == 0)
                    {
                        TryHint(GameConstants.MsgHintFindCave, GameConstants.HintF, GameConstants.HintAreaF);
                    }
                    break;
                case GameConstants.HintC:
                    hints.LocationCounters[GameConstants.HintAreaC]++;
                    if (hints.LocationCounters[GameConstants.HintAreaC] > GameConstants.HintGetIntoCaveTurns &&
                        objects.PropertyOf(GameConstants.Grate) == 0 &&
                        !gameState.Toting(GameConstants.Keys))
                    {
                        TryHint(GameConstants.MsgHintGetIntoCave, GameConstants.HintC, GameConstants.HintAreaC);
                    }
                    break;
                case GameConstants.HintB:
                    hints.LocationCounters[GameConstants.HintAreaB]++;
                    if (hints.LocationCounters[GameConstants.HintAreaB] > GameConstants.HintCatchBirdTurns &&
                        objects.LocationOf(GameConstants.Bird) == position.Location &&
                        gameState.Toting(GameConstants.Rod))
                    {
                        TryHint(GameConstants.MsgHintCatchBird, GameConstants.HintB, GameConstants.HintAreaB);
                    }
                    break;
                case GameConstants.HintS:
                    hints.LocationCounters[GameConstants.HintAreaS]++;
                    if (hints.LocationCounters[GameConstants.HintAreaS] > GameConstants.HintDealWithSnakeTurns &&
                        objects.LocationOf(GameConstants.Snake) == position.Location &&
                        !gameState.Toting(GameConstants.Bird))
                    {
                        TryHint(GameConstants.MsgHintDealWithSnake, GameConstants.HintS, GameConstants.HintAreaS);
                    }
                    break;
                case GameConstants.HintM:
                    hints.LocationCounters[GameConstants.HintAreaM]++;
                    if (hints.LocationCounters[GameConstants.HintAreaM] > GameConstants.HintOutOfMazeTurns)
                        TryHint(GameConstants.MsgHintOutOfMaze, GameConstants.HintM, GameConstants.HintAreaM);
                    break;
                case GameConstants.HintP:
                    hints.LocationCounters[GameConstants.HintAreaP]++;
                    if (hints.LocationCounters[GameConstants.HintAreaP] > GameConstants.HintExplorePloverTurns &&
                        objects.LocationOf(GameConstants.Emerald) != GameConstants.PloverRoomLocation)
                    {
                        TryHint(GameConstants.MsgHintExplorePlover, GameConstants.HintP, GameConstants.HintAreaP);
                    }
                    break;
                case GameConstants.HintW:
                    hints.LocationCounters[GameConstants.HintAreaW]++;
                    if (hints.LocationCounters[GameConstants.HintAreaW] > GameConstants.HintOutOfHereTurns)
                        TryHint(GameConstants.MsgHintOutOfHere, GameConstants.HintW, GameConstants.HintAreaW);
                    break;
            }
        }

        private void TryHint(int promptMessage, int mask, int hintArea)
        {
            HintTrackingState hints = gameState.Hints;
            TreasureProgressState progress = gameState.TreasureProgress;

            Console.WriteLine();
            if (AskYesNo(promptMessage, 0, GameConstants.MsgOk) &&
                AskYesNo(GameConstants.MsgPromptOfferHint, promptMessage + 1, GameConstants.MsgOk))
            {
                progress.HintsAccepted++;
                hints.AvailableMask &= ~mask;
            }

            hints.LocationCounters[hintArea] = 0;
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
                    PrintScore();
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
            gameState.TreasureProgress.GaveUp = AskYesNo(GameConstants.MsgPromptQuit, 0, GameConstants.MsgOk);
            if (gameState.TreasureProgress.GaveUp)
                NormalEnd();
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
            PrintObjectMessage(GameConstants.Eggs, k);
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
                    DwarfEnd(GameConstants.MsgDwarvesAwakenGetYou);
                gameState.Destroy(GameConstants.Snake);
                gameState.SetObjectProperty(GameConstants.Snake, -1);
            }
            else if (objectId == GameConstants.Coins && gameState.Here(GameConstants.Vend))
            {
                gameState.Destroy(GameConstants.Coins);
                gameState.Drop(GameConstants.Batteries, position.Location);
                PrintObjectMessage(GameConstants.Batteries, 0);
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
                    PrintObjectMessage(GameConstants.Vase, vaseProperty + 1);
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
                ShowLocationDescription();
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
                PrintObjectMessage(GameConstants.Fissure, 2 - objects.PropertyOf(GameConstants.Fissure));
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
                        DwarfEnd(GameConstants.MsgDwarvesAwakenGetYou);
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

                    if (!AskYesNo(GameConstants.MsgBareHands, 0, 0))
                        return;

                    PrintObjectMessage(GameConstants.Dragon, 1);
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
                    PrintObjectMessage(GameConstants.Plant, objects.PropertyOf(GameConstants.Plant) + 1);
                    gameState.SetObjectProperty(GameConstants.Plant, (objects.PropertyOf(GameConstants.Plant) + 2) % 6);
                    gameState.SetObjectProperty(GameConstants.Plant2, objects.PropertyOf(GameConstants.Plant) / 2);
                    ShowLocationDescription();
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
            ShowLocationDescription();
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
                        if (AskYesNo(GameConstants.MsgPromptReadOysterClue, GameConstants.MsgOysterClue, GameConstants.MsgOk))
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
            NormalEnd();
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
                    DwarfEnd(GameConstants.MsgDwarvesAwakenGetYou);
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
                DwarfEnd(GameConstants.MsgWakeDwarfClosed);
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

        private bool AskYesNo(int promptMessage, int yesMessage, int noMessage)
        {
            if (promptMessage != 0)
                Speak(promptMessage);

            while (true)
            {
                Console.Write("> ");
                string response = Console.ReadLine() ?? string.Empty;
                string normalized = response.ToLowerInvariant().Trim();

                if (string.IsNullOrEmpty(normalized))
                {
                    Speak(GameConstants.MsgPleaseAnswerQuestion);
                    continue;
                }

                if ("no".StartsWith(normalized, StringComparison.Ordinal))
                {
                    if (noMessage != 0)
                        Speak(noMessage);
                    return false;
                }

                if ("yes".StartsWith(normalized, StringComparison.Ordinal))
                {
                    if (yesMessage != 0)
                        Speak(yesMessage);
                    return true;
                }

                Speak(GameConstants.MsgPleaseAnswerQuestion);
            }
        }

        private void DwarfEnd(int message)
        {
            if (message != 0)
                Speak(message);
            HandleDeath();
            NormalEnd();
        }

        private void NormalEnd()
        {
            int total = PrintScore();
            TreasureProgressState progress = gameState.TreasureProgress;
            int[] limits = GameConstants.RatingThresholds;
            int ratingIndex = 0;
            while (ratingIndex < limits.Length && limits[ratingIndex] <= total)
                ratingIndex++;

            Console.WriteLine();
            int ratingMessage = GameConstants.MsgFirstRating + ratingIndex;
            if (ratingMessage <= GameConstants.MsgLastRating)
                Speak(ratingMessage);

            int next = ratingIndex < limits.Length ? limits[ratingIndex] - total : 0;
            if (next > 0)
                Console.WriteLine($"To achieve the next higher rating, you need {next} more point{(next == 1 ? string.Empty : "s")}.");

            progress.SaveRequested = true;
        }

        private int PrintScore()
        {
            WorldMapState world = gameState.World;
            ObjectPlacementState objects = gameState.Objects;
            CaveTimingState cave = gameState.Cave;
            TreasureProgressState progress = gameState.TreasureProgress;

            int score = 0;
            int treasures = 0;

            for (int item = GameConstants.Nugget; item <= GameConstants.MaxTreasures; item++)
            {
                int itemScore = item == GameConstants.Chest
                    ? GameConstants.ChestTreasureScore
                    : item > GameConstants.Chest
                        ? GameConstants.TreasureScoreAfterChest
                        : GameConstants.TreasureScoreBeforeChest;
                if (objects.PropertyOf(item) >= 0)
                    treasures += GameConstants.TreasureDiscoveryScore;
                if (objects.LocationOf(item) == GameConstants.WellHouse && objects.PropertyOf(item) == 0)
                    treasures += itemScore - GameConstants.TreasureDiscoveryScore;
            }

            PrintScoreLine("Treasures:", treasures);
            score += treasures;

            int survival = (GameConstants.MaxDeaths - progress.DeathCount) * GameConstants.SurvivalScorePerDeathRemaining;
            if (survival != 0)
                PrintScoreLine("Survival:", survival);
            score += survival;

            if (!progress.GaveUp)
                score += GameConstants.DidNotQuitScore;

            int gettingIn = world.VisitedLocations[GameConstants.HallOfMountainKingLocation] != 0
                ? GameConstants.GettingWellInScore
                : 0;
            if (gettingIn != 0)
                PrintScoreLine("Getting well in:", gettingIn);
            score += gettingIn;

            int masters = cave.Closing ? GameConstants.MastersSectionScore : 0;
            if (masters != 0)
                PrintScoreLine("Masters section:", masters);
            score += masters;

            if (cave.Closed)
            {
                int bonus = progress.Bonus == 0 ? GameConstants.ClosedBonusScoreDefault :
                    progress.Bonus == GameConstants.MsgBlastSelfDeath ? GameConstants.ClosedBonusScoreSelfBlast :
                    progress.Bonus == GameConstants.MsgBlastLavaDeath ? GameConstants.ClosedBonusScoreLava :
                    progress.Bonus == GameConstants.MsgBlastWins ? GameConstants.ClosedBonusScoreWin : 0;
                PrintScoreLine("Bonus:", bonus);
                score += bonus;
            }

            if (objects.LocationOf(GameConstants.Magazine) == GameConstants.MagazineBonusLocation)
                score += GameConstants.MagazineInWittsEndScore;

            int hints = -GameConstants.HintScorePenalty * progress.HintsAccepted;
            if (hints != 0)
            {
                PrintScoreLine("Hints & instructions:", hints);
                score += hints;
            }

            score += GameConstants.BaseScore;
            if (score < 0)
                score = 0;

            PrintScoreLine("Score:", score);
            return score;
        }

        private static void PrintScoreLine(string label, int value)
        {
            Console.WriteLine($"{label,-22}{value,4}");
        }

        /// <summary>
        /// Shows the player's inventory.
        /// </summary>
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

        /// <summary>
        /// Shows the current location description.
        /// </summary>
        private void ShowLocationDescription(bool forceLong = false)
        {
            GamePositionState position = gameState.Position;
            WorldMapState world = gameState.World;
            CaveTimingState cave = gameState.Cave;
            TreasureProgressState progress = gameState.TreasureProgress;

            if (gameState.Toting(GameConstants.Bear))
                Speak(GameConstants.MsgBearFollowing);

            if (DarknessManager.IsDark(gameState))
            {
                Speak(GameConstants.MsgPitchDark);
            }
            else
            {
                bool useShortDescription = !forceLong &&
                    ((world.VisitedLocations[position.Location] & 3) != 0 ||
                    ((position.Detail & progress.DescriptionDetailMask) != 0 && world.VisitedLocations[position.Location] != 0));

                if (!useShortDescription &&
                    AdventureData.TryGetLongLocationDescription(position.Location, out string longDesc))
                {
                    Console.WriteLine(longDesc);
                }
                else if (AdventureData.TryGetShortLocationDescription(position.Location, out string shortDesc))
                {
                    Console.WriteLine(shortDesc);
                }
                else
                {
                    Console.WriteLine($"You are in location {position.Location}.");
                }

                if (!DarknessManager.IsDark(gameState))
                    ShowObjectsHere();
            }

            if (position.Location == GameConstants.Y2Location &&
                GameState.Pct(random, GameConstants.Y2PlughChance) &&
                !cave.Closing)
            {
                Speak(GameConstants.MsgHollowVoicePlugh);
            }
        }

        /// <summary>
        /// Shows objects present at the current location.
        /// </summary>
        private void ShowObjectsHere()
        {
            GamePositionState position = gameState.Position;
            ObjectPlacementState objects = gameState.Objects;
            CaveTimingState cave = gameState.Cave;
            TreasureProgressState progress = gameState.TreasureProgress;

            bool printedAny = false;

            for (int objectId = 1; objectId < GameConstants.MaxObjects; objectId++)
            {
                if (!gameState.At(objectId))
                    continue;

                if (objectId == GameConstants.Steps && gameState.Toting(GameConstants.Nugget))
                    continue;

                if (objects.IsPropertyNegative(objectId))
                {
                    if (cave.Closed)
                        continue;

                    gameState.SetObjectProperty(objectId, 0);
                    if (objectId == GameConstants.Rug || objectId == GameConstants.Chain)
                        gameState.SetObjectProperty(objectId, objects.PropertyOf(objectId) + 1);
                    progress.UndiscoveredTreasureCount--;
                }

                int state = objectId == GameConstants.Steps &&
                    position.Location == objects.FixedLocationOf(GameConstants.Steps)
                    ? 1
                    : objects.PropertyOf(objectId);

                if (!printedAny && (position.Detail & 2) == 0)
                {
                    Console.WriteLine();
                    printedAny = true;
                }

                PrintObjectMessage(objectId, state);
            }

            if (progress.UndiscoveredTreasureCount == progress.TreasuresLostToEndgame &&
                progress.UndiscoveredTreasureCount != 0 &&
                cave.LampLimit > GameConstants.AllTreasuresDiscoveredLampLimit)
            {
                cave.LampLimit = GameConstants.AllTreasuresDiscoveredLampLimit;
            }
        }

        /// <summary>
        /// Shows game instructions.
        /// </summary>
        private void ShowInstructions()
        {
            Console.WriteLine(AdventureData.Message(GameConstants.MsgInstructions));
        }

        /// <summary>
        /// Updates the game state after each turn.
        /// </summary>
        private void UpdateGameState()
        {
            GamePositionState position = gameState.Position;
            CaveTimingState cave = gameState.Cave;
            TreasureProgressState progress = gameState.TreasureProgress;

            progress.FooBar = progress.FooBar > 0 ? -progress.FooBar : 0;
            progress.DescriptionDetailMask = 2;

            // Check lamp battery status and decrement if lamp is on
            string? batteryMessage = DarknessManager.CheckBatteryStatus(gameState);
            if (batteryMessage != null)
            {
                Console.WriteLine(batteryMessage);
            }

            // Check for game end conditions
            if (position.Turns >= cave.LampLimit)
            {
                Console.WriteLine("You have exceeded the turn limit.");
                progress.SaveRequested = true;
            }
        }

        /// <summary>
        /// Checks if a response indicates "yes".
        /// </summary>
        private bool IsYesResponse(string response)
        {
            if (string.IsNullOrWhiteSpace(response))
                return false;
                
            string normalized = response.ToLowerInvariant().Trim();
            return normalized.StartsWith("y") || normalized == "yes";
        }

        /// <summary>
        /// Handles player death.
        /// </summary>
        private void HandleDeath()
        {
            GamePositionState position = gameState.Position;
            CaveTimingState cave = gameState.Cave;
            TreasureProgressState progress = gameState.TreasureProgress;

            if (!cave.Closing)
            {
                bool reincarnate = AskYesNo(
                    GameConstants.MsgFirstDeathPrompt + progress.DeathCount * GameConstants.DeathMessageStep,
                    GameConstants.MsgFirstReincarnation + progress.DeathCount * GameConstants.DeathMessageStep,
                    GameConstants.MsgOk);
                progress.DeathCount++;

                if (progress.DeathCount >= GameConstants.MaxDeaths || !reincarnate)
                {
                    NormalEnd();
                    return;
                }

                gameState.SetObjectLocation(GameConstants.Water, 0);
                gameState.SetObjectLocation(GameConstants.Oil, 0);
                if (gameState.Toting(GameConstants.Lamp))
                    gameState.SetObjectProperty(GameConstants.Lamp, 0);

                for (int item = GameConstants.MaxObjects; item >= 1; item--)
                {
                    if (gameState.Toting(item))
                        gameState.Drop(item, item == GameConstants.Lamp ? GameConstants.EndOfRoad : position.OldLocation2);
                }

                position.NewLocation = GameConstants.WellHouse;
                position.OldLocation = position.Location;
                position.Location = 0;
                return;
            }

            Speak(GameConstants.MsgDeathNearClosing);
            progress.DeathCount++;
            NormalEnd();
        }
    }
