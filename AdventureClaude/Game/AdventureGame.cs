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
                gameState.Cave.LampLimit = 1000;
                gameState.TreasureProgress.HintsAccepted++;
            }
            else
            {
                gameState.Cave.LampLimit = 330;
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
                DoMove();
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
        /// C TURN.C domove(): dispatches special motion words and normal travel.
        /// </summary>
        private void DoMove()
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
                        Console.WriteLine(GameMessages.GetMessage(15));
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
                    Console.WriteLine(GameMessages.GetMessage(position.Location < 8 ? 57 : 58));
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
                Console.WriteLine(GameMessages.GetMessage(91));
                return;
            }

            List<TravelEntry> travel = TravelData.GetTravelOptions(position.Location);
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
                    List<TravelEntry> destinationTravel = TravelData.GetTravelOptions(destination);
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
                Console.WriteLine(GameMessages.GetMessage(140));
            }
        }

        /// <summary>
        /// C TURN.C dotrav(): evaluates travel table entries and sets NewLocation.
        /// </summary>
        private void DoTravel()
        {
            GamePositionState position = gameState.Position;
            ParsedCommandState command = gameState.Command;

            List<TravelEntry> travel = TravelData.GetTravelOptions(position.Location);
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

                if (verb != 1 && verb != command.Motion && !hit)
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
            else if (selectedDestination > 500)
            {
                Console.WriteLine(GameMessages.GetMessage(selectedDestination - 500));
            }
            else if (selectedDestination > 300)
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
            int referencedObject = condition % 100;
            int conditionType = condition / 100;

            return conditionType switch
            {
                0 => condition == 0 || roll < condition,
                1 => referencedObject == 0 || gameState.Toting(referencedObject),
                2 => gameState.Toting(referencedObject) || gameState.At(referencedObject),
                3 or 4 or 5 or 7 => gameState.ObjectProperties[referencedObject] != conditionType - 3,
                _ => false,
            };
        }

        /// <summary>
        /// C TURN.C badmove(): chooses the best failed-movement message.
        /// </summary>
        private void BadMove()
        {
            ParsedCommandState command = gameState.Command;

            int message = 12;
            if (command.Motion >= 43 && command.Motion <= 50)
                message = 9;
            if (command.Motion == 29 || command.Motion == 30)
                message = 9;
            if (command.Motion == 7 || command.Motion == 36 || command.Motion == 37)
                message = 10;
            if (command.Motion == 11 || command.Motion == 19)
                message = 11;
            if (command.Verb == GameConstants.Find || command.Verb == GameConstants.Inventory)
                message = 59;
            if (command.Motion == 62 || command.Motion == 65)
                message = 42;
            if (command.Motion == 17)
                message = 80;

            Console.WriteLine(GameMessages.GetMessage(message));
        }

        /// <summary>
        /// C TURN.C spcmove(): handles plover and troll bridge travel destinations.
        /// </summary>
        private void SpecialMove(int destination)
        {
            switch (destination - 300)
            {
                case 1:
                    if (gameState.Holding == 0 ||
                        (gameState.Holding == 1 && gameState.Toting(GameConstants.Emerald)))
                    {
                        gameState.NewLocation = 199 - gameState.Location;
                    }
                    else
                    {
                        Console.WriteLine(GameMessages.GetMessage(117));
                    }
                    break;
                case 2:
                    gameState.Drop(GameConstants.Emerald, gameState.Location);
                    Console.WriteLine(GameMessages.GetMessage(54));
                    break;
                case 3:
                    if (gameState.ObjectProperties[GameConstants.Troll] == 1)
                    {
                        PrintObjectMessage(GameConstants.Troll, 1);
                        gameState.SetObjectProperty(GameConstants.Troll, 0);
                        gameState.MoveObject(GameConstants.Troll2, 0);
                        gameState.MoveObject(GameConstants.Troll2 + GameConstants.MaxObjects, 0);
                        gameState.MoveObject(GameConstants.Troll, 117);
                        gameState.MoveObject(GameConstants.Troll + GameConstants.MaxObjects, 122);
                        GameState.Juggle(GameConstants.Chasm);
                        gameState.NewLocation = gameState.Location;
                    }
                    else
                    {
                        gameState.NewLocation = gameState.Location == 117 ? 122 : 117;
                        if (gameState.ObjectProperties[GameConstants.Troll] == 0)
                            gameState.SetObjectProperty(GameConstants.Troll, gameState.ObjectProperties[GameConstants.Troll] + 1);

                        if (!gameState.Toting(GameConstants.Bear))
                            return;

                        Console.WriteLine(GameMessages.GetMessage(162));
                        gameState.SetObjectProperty(GameConstants.Chasm, 1);
                        gameState.SetObjectProperty(GameConstants.Troll, 2);
                        gameState.Drop(GameConstants.Bear, gameState.NewLocation);
                        gameState.FixedObjectLocations[GameConstants.Bear] = -1;
                        gameState.SetObjectProperty(GameConstants.Bear, 3);
                        if (gameState.ObjectProperties[GameConstants.Spices] < 0)
                            gameState.Tally2++;
                        gameState.OldLocation2 = gameState.NewLocation;
                        HandleDeath();
                    }
                    break;
                default:
                    Console.WriteLine($"Fatal error number 38");
                    gameState.SaveFlag = true;
                    break;
            }
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

            if (position.NewLocation >= 9 || position.NewLocation == 0 || !cave.Closing)
                return;

            Speak(130);
            position.NewLocation = position.Location;
            if (!cave.Panic)
                cave.Clock2 = 15;
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
                    Speak(2);
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
                if (position.NewLocation > 15)
                    dwarves.ActivationLevel++;
                return;
            }

            if (dwarves.ActivationLevel == 1)
            {
                if (position.NewLocation < 15 || GameState.Pct(random, 95))
                    return;

                dwarves.ActivationLevel++;
                for (int i = 1; i < 3; i++)
                {
                    if (GameState.Pct(random, 50))
                        dwarves.Locations[GameState.RRand(random, 1, 5)] = 0;
                }

                for (int i = 1; i < GameConstants.MaxDwarves - 1; i++)
                {
                    if (dwarves.Locations[i] == position.NewLocation)
                        dwarves.Locations[i] = dwarves.AlternateLocation;
                    dwarves.PreviousLocations[i] = dwarves.Locations[i];
                }

                Speak(3);
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
                for (int attempt = 1; attempt < 20; attempt++)
                {
                    candidateLocation = GameState.RRand(random, 15, 120);
                    if (candidateLocation != dwarves.PreviousLocations[i] &&
                        candidateLocation != dwarves.Locations[i])
                    {
                        break;
                    }
                }

                dwarves.PreviousLocations[i] = dwarves.Locations[i];
                dwarves.Locations[i] = candidateLocation;

                dwarves.Seen[i] =
                    (dwarves.Seen[i] && position.NewLocation >= 15) ||
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
                    if (GameState.RRand(random, 0, 999) < 95 * (dwarves.ActivationLevel - 2))
                        hits++;
                }
            }

            if (dwarfCount == 0)
                return;

            if (dwarfCount > 1)
                Console.WriteLine($"There are {dwarfCount} threatening little dwarves in the room with you!");
            else
                Speak(4);

            if (attacks == 0)
                return;

            if (dwarves.ActivationLevel == 2)
                dwarves.ActivationLevel++;

            int messageBase;
            if (attacks > 1)
            {
                Console.WriteLine($"{attacks} of them throw knives at you!!");
                messageBase = 6;
            }
            else
            {
                Speak(5);
                messageBase = 52;
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
                objects.Properties[GameConstants.Chest] >= 0)
            {
                return;
            }

            int nearbyTreasures = 0;
            for (int treasure = GameConstants.Nugget; treasure <= GameConstants.MaxTreasures; treasure++)
            {
                if (treasure == GameConstants.Pyramid &&
                    (position.NewLocation == objects.Locations[GameConstants.Pyramid] ||
                    position.NewLocation == objects.Locations[GameConstants.Emerald]))
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
                objects.Locations[GameConstants.Chest] == 0 &&
                gameState.Here(GameConstants.Lamp) &&
                objects.Properties[GameConstants.Lamp] == 1)
            {
                Speak(186);
                gameState.MoveObject(GameConstants.Chest, objects.ChestLocation);
                gameState.MoveObject(GameConstants.Message, objects.ChestLocation2);
                dwarves.Locations[GameConstants.MaxDwarves - 1] = objects.ChestLocation;
                dwarves.PreviousLocations[GameConstants.MaxDwarves - 1] = objects.ChestLocation;
                dwarves.Seen[GameConstants.MaxDwarves - 1] = false;
                return;
            }

            if (dwarves.PreviousLocations[GameConstants.MaxDwarves - 1] !=
                dwarves.Locations[GameConstants.MaxDwarves - 1] &&
                GameState.Pct(random, 20))
            {
                Speak(127);
            }
        }

        private void PirateStealsTreasure()
        {
            GamePositionState position = gameState.Position;
            DwarfPirateState dwarves = gameState.Dwarves;
            ObjectPlacementState objects = gameState.Objects;

            Speak(128);
            if (objects.Locations[GameConstants.Message] == 0)
                gameState.MoveObject(GameConstants.Chest, objects.ChestLocation);
            gameState.MoveObject(GameConstants.Message, objects.ChestLocation2);

            for (int treasure = GameConstants.Nugget; treasure <= GameConstants.MaxTreasures; treasure++)
            {
                if (treasure == GameConstants.Pyramid &&
                    (position.NewLocation == objects.Locations[GameConstants.Pyramid] ||
                    position.NewLocation == objects.Locations[GameConstants.Emerald]))
                {
                    continue;
                }

                if (gameState.At(treasure) && objects.FixedLocations[treasure] == 0)
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
            const int MaxForcedMoves = 20;
            int forcedMoves = 0;

            while (gameState.Location != gameState.NewLocation && !gameState.SaveFlag)
            {
                bool forceLongDescription = gameState.Location == 0;
                if (forceLongDescription)
                    gameState.VisitedLocations[gameState.NewLocation] =
                        (short)((gameState.VisitedLocations[gameState.NewLocation] + 3) & ~3);

                gameState.Turns++;
                gameState.Location = gameState.NewLocation;

                if (gameState.Location == 0)
                {
                    HandleDeath();
                    return;
                }

                if (gameState.Forced(gameState.Location))
                {
                    ShowLocationDescription(forceLongDescription);
                    gameState.Motion = 1;
                    DoMove();
                    if (++forcedMoves >= MaxForcedMoves)
                    {
                        Console.WriteLine("[Warning: Maximum forced movement cascade depth reached]");
                        return;
                    }
                    continue;
                }

                if (gameState.WizardDark && DarknessManager.IsDark(gameState) && GameState.Pct(random, 35))
                {
                    Console.WriteLine(GameMessages.GetMessage(23));
                    gameState.OldLocation2 = gameState.Location;
                    HandleDeath();
                    return;
                }

                ShowLocationDescription(forceLongDescription);
                if (!DarknessManager.IsDark(gameState))
                    gameState.VisitedLocations[gameState.Location]++;
            }
        }

        private void PrintObjectMessage(int objectId, int state)
        {
            if (!GameObjects.Objects.TryGetValue(objectId, out GameObjectData? objectData))
                return;

            if (state < 0 || state >= objectData.States.Count)
                return;

            string message = objectData.States[state].RoomDescription;
            if (!string.IsNullOrEmpty(message))
                Console.WriteLine(message);
        }

        private void ApplyClosedInventoryState()
        {
            if (!gameState.Closed)
                return;

            if (gameState.ObjectProperties[GameConstants.Oyster] < 0 && gameState.Toting(GameConstants.Oyster))
                PrintObjectMessage(GameConstants.Oyster, 1);

            for (int item = 1; item <= GameConstants.MaxObjects; item++)
            {
                if (gameState.Toting(item) && gameState.ObjectProperties[item] < 0)
                    gameState.SetObjectProperty(item, -1 - gameState.ObjectProperties[item]);
            }
        }

        private bool RunSpecialTimer()
        {
            gameState.FooBar = gameState.FooBar > 0 ? -gameState.FooBar : 0;
            gameState.TestBr = 2;

            if (gameState.Tally == 0 && gameState.Location >= 15 && gameState.Location != 33)
                gameState.Clock1--;

            if (gameState.Clock1 == 0)
            {
                gameState.SetObjectProperty(GameConstants.Grate, 0);
                gameState.SetObjectProperty(GameConstants.Fissure, 0);
                for (int i = 1; i < GameConstants.MaxDwarves; i++)
                    gameState.DwarfSeen[i] = false;

                gameState.MoveObject(GameConstants.Troll, 0);
                gameState.MoveObject(GameConstants.Troll + GameConstants.MaxObjects, 0);
                gameState.MoveObject(GameConstants.Troll2, 117);
                gameState.MoveObject(GameConstants.Troll2 + GameConstants.MaxObjects, 122);
                GameState.Juggle(GameConstants.Chasm);
                if (gameState.ObjectProperties[GameConstants.Bear] != 3)
                    gameState.Destroy(GameConstants.Bear);
                gameState.SetObjectProperty(GameConstants.Chain, 0);
                gameState.FixedObjectLocations[GameConstants.Chain] = 0;
                gameState.SetObjectProperty(GameConstants.Axe, 0);
                gameState.FixedObjectLocations[GameConstants.Axe] = 0;
                Speak(129);
                gameState.Clock1 = -1;
                gameState.Closing = true;
                return false;
            }

            if (gameState.Clock1 < 0)
                gameState.Clock2--;

            if (gameState.Clock2 == 0)
            {
                CloseCave();
                return true;
            }

            if (gameState.ObjectProperties[GameConstants.Lamp] == 1)
                gameState.Limit--;

            if (gameState.Limit <= 30 &&
                gameState.Here(GameConstants.Batteries) &&
                gameState.ObjectProperties[GameConstants.Batteries] == 0 &&
                gameState.Here(GameConstants.Lamp))
            {
                Speak(188);
                gameState.SetObjectProperty(GameConstants.Batteries, 1);
                if (gameState.Toting(GameConstants.Batteries))
                    gameState.Drop(GameConstants.Batteries, gameState.Location);
                gameState.Limit += 2500;
                gameState.LampWarning = 0;
                return false;
            }

            if (gameState.Limit == 0)
            {
                gameState.Limit--;
                gameState.SetObjectProperty(GameConstants.Lamp, 0);
                if (gameState.Here(GameConstants.Lamp))
                    Speak(184);
                return false;
            }

            if (gameState.Limit < 0 && gameState.Location <= 8)
            {
                Speak(185);
                gameState.GaveUp = true;
                NormalEnd();
                return true;
            }

            if (gameState.Limit <= 30)
            {
                if (gameState.LampWarning != 0 || !gameState.Here(GameConstants.Lamp))
                    return false;

                gameState.LampWarning = 1;
                int message = 187;
                if (gameState.ObjectLocations[GameConstants.Batteries] == 0)
                    message = 183;
                if (gameState.ObjectProperties[GameConstants.Batteries] == 1)
                    message = 189;
                Speak(message);
            }

            return false;
        }

        private void CloseCave()
        {
            gameState.SetObjectProperty(GameConstants.Bottle, gameState.Put(GameConstants.Bottle, 115, 1));
            gameState.SetObjectProperty(GameConstants.Plant, gameState.Put(GameConstants.Plant, 115, 0));
            gameState.SetObjectProperty(GameConstants.Oyster, gameState.Put(GameConstants.Oyster, 115, 0));
            gameState.SetObjectProperty(GameConstants.Lamp, gameState.Put(GameConstants.Lamp, 115, 0));
            gameState.SetObjectProperty(GameConstants.Rod, gameState.Put(GameConstants.Rod, 115, 0));
            gameState.SetObjectProperty(GameConstants.Dwarf, gameState.Put(GameConstants.Dwarf, 115, 0));

            gameState.Location = 115;
            gameState.OldLocation = 115;
            gameState.NewLocation = 115;

            gameState.Put(GameConstants.Grate, 116, 0);
            gameState.SetObjectProperty(GameConstants.Snake, gameState.Put(GameConstants.Snake, 116, 1));
            gameState.SetObjectProperty(GameConstants.Bird, gameState.Put(GameConstants.Bird, 116, 1));
            gameState.SetObjectProperty(GameConstants.Cage, gameState.Put(GameConstants.Cage, 116, 0));
            gameState.SetObjectProperty(GameConstants.Rod2, gameState.Put(GameConstants.Rod2, 116, 0));
            gameState.SetObjectProperty(GameConstants.Pillow, gameState.Put(GameConstants.Pillow, 116, 0));
            gameState.SetObjectProperty(GameConstants.Mirror, gameState.Put(GameConstants.Mirror, 115, 0));
            gameState.FixedObjectLocations[GameConstants.Mirror] = 116;

            for (int item = 1; item <= GameConstants.MaxObjects; item++)
            {
                if (gameState.Toting(item))
                    gameState.Destroy(item);
            }

            Speak(132);
            gameState.Closed = true;
            gameState.Location = 0;
        }

        private void TryLocationHint()
        {
            if ((gameState.LocationConditions[gameState.Location] & gameState.HintAvailable) == 0)
            {
                Array.Clear(gameState.HintLocations);
                return;
            }

            switch (gameState.LocationConditions[gameState.Location] & GameConstants.Hint)
            {
                case GameConstants.HintF:
                    gameState.HintLocations[GameConstants.HintAreaF]++;
                    if (gameState.HintLocations[GameConstants.HintAreaF] > 20 && gameState.VisitedLocations[8] == 0)
                        TryHint(56, GameConstants.HintF, GameConstants.HintAreaF);
                    break;
                case GameConstants.HintC:
                    gameState.HintLocations[GameConstants.HintAreaC]++;
                    if (gameState.HintLocations[GameConstants.HintAreaC] > 3 &&
                        gameState.ObjectProperties[GameConstants.Grate] == 0 &&
                        !gameState.Toting(GameConstants.Keys))
                    {
                        TryHint(62, GameConstants.HintC, GameConstants.HintAreaC);
                    }
                    break;
                case GameConstants.HintB:
                    gameState.HintLocations[GameConstants.HintAreaB]++;
                    if (gameState.HintLocations[GameConstants.HintAreaB] > 4 &&
                        gameState.ObjectLocations[GameConstants.Bird] == gameState.Location &&
                        gameState.Toting(GameConstants.Rod))
                    {
                        TryHint(18, GameConstants.HintB, GameConstants.HintAreaB);
                    }
                    break;
                case GameConstants.HintS:
                    gameState.HintLocations[GameConstants.HintAreaS]++;
                    if (gameState.HintLocations[GameConstants.HintAreaS] > 5 &&
                        gameState.ObjectLocations[GameConstants.Snake] == gameState.Location &&
                        !gameState.Toting(GameConstants.Bird))
                    {
                        TryHint(20, GameConstants.HintS, GameConstants.HintAreaS);
                    }
                    break;
                case GameConstants.HintM:
                    gameState.HintLocations[GameConstants.HintAreaM]++;
                    if (gameState.HintLocations[GameConstants.HintAreaM] > 15)
                        TryHint(176, GameConstants.HintM, GameConstants.HintAreaM);
                    break;
                case GameConstants.HintP:
                    gameState.HintLocations[GameConstants.HintAreaP]++;
                    if (gameState.HintLocations[GameConstants.HintAreaP] > 5 &&
                        gameState.ObjectLocations[GameConstants.Emerald] != 100)
                    {
                        TryHint(178, GameConstants.HintP, GameConstants.HintAreaP);
                    }
                    break;
                case GameConstants.HintW:
                    gameState.HintLocations[GameConstants.HintAreaW]++;
                    if (gameState.HintLocations[GameConstants.HintAreaW] > 15)
                        TryHint(180, GameConstants.HintW, GameConstants.HintAreaW);
                    break;
            }
        }

        private void TryHint(int promptMessage, int mask, int hintArea)
        {
            Console.WriteLine();
            if (AskYesNo(promptMessage, 0, 54) &&
                AskYesNo(87, promptMessage + 1, 54))
            {
                gameState.HintTaken++;
                gameState.HintAvailable &= ~mask;
            }

            gameState.HintLocations[hintArea] = 0;
        }

        private void DoObject()
        {
            int objectId = gameState.Object;

            if (gameState.FixedObjectLocations[objectId] == gameState.Location || gameState.Here(objectId))
            {
                TransitiveObject();
                return;
            }

            if (objectId == GameConstants.Grate)
            {
                if (gameState.Location == 1 || gameState.Location == 4 || gameState.Location == 7)
                {
                    gameState.Motion = GameConstants.Depression;
                    DoMove();
                    return;
                }

                if (gameState.Location > 9 && gameState.Location < 15)
                {
                    gameState.Motion = GameConstants.Entrance;
                    DoMove();
                    return;
                }
            }
            else if (gameState.DCheck() != 0 && gameState.DwarfFlag >= 2)
            {
                gameState.Object = GameConstants.Dwarf;
                TransitiveObject();
                return;
            }
            else if ((gameState.Liq() == objectId && gameState.Here(GameConstants.Bottle)) ||
                gameState.LiqLoc(gameState.Location) == objectId)
            {
                TransitiveObject();
                return;
            }
            else if (objectId == GameConstants.Plant &&
                gameState.At(GameConstants.Plant2) &&
                gameState.ObjectProperties[GameConstants.Plant2] == 0)
            {
                gameState.Object = GameConstants.Plant2;
                TransitiveObject();
                return;
            }
            else if (objectId == GameConstants.Knife && gameState.KnifeLocation == gameState.Location)
            {
                Speak(116);
                gameState.KnifeLocation = -1;
                return;
            }
            else if (objectId == GameConstants.Rod && gameState.Here(GameConstants.Rod2))
            {
                gameState.Object = GameConstants.Rod2;
                TransitiveObject();
                return;
            }
            else if (objectId == GameConstants.Troll && gameState.Here(GameConstants.Troll2))
            {
                gameState.Object = GameConstants.Troll2;
                TransitiveObject();
                return;
            }
            else if (objectId == GameConstants.Plant && gameState.Here(GameConstants.Plant2))
            {
                gameState.Object = GameConstants.Plant2;
                TransitiveObject();
                return;
            }

            SpeakObjectNotHere(objectId);
        }

        private void TransitiveObject()
        {
            if (gameState.Verb != 0)
                TransitiveVerb();
            else
                Console.WriteLine($"What do you want to do with the {GetObjectWord(gameState.Object)}?");
        }

        private void TransitiveVerb()
        {
            switch (gameState.Verb)
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
                    ActSpeak(gameState.Verb);
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
                    Speak(54);
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
                    if (gameState.Object != GameConstants.Lamp)
                        Speak(76);
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
            switch (gameState.Verb)
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
                    Speak(54);
                    break;
                case GameConstants.On:
                case GameConstants.Off:
                case GameConstants.Pour:
                    TransitiveVerb();
                    break;
                case GameConstants.Walk:
                    ActSpeak(gameState.Verb);
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
                    gameState.SaveFlag = true;
                    break;
                case GameConstants.Read:
                    IVRead();
                    break;
                case GameConstants.Inventory:
                    ShowInventory();
                    break;
                case GameConstants.Brief:
                    gameState.Detail |= 2;
                    ActSpeak(gameState.Verb);
                    break;
                case GameConstants.Help:
                    Speak(51);
                    break;
                default:
                    Console.WriteLine("This intransitive verb is not implemented yet.");
                    break;
            }
        }

        private void IVTake()
        {
            int candidate = 0;
            for (int item = 1; item < GameConstants.MaxObjects; item++)
            {
                if (gameState.ObjectLocations[item] != gameState.Location)
                    continue;

                if (candidate != 0)
                {
                    NeedObject();
                    return;
                }

                candidate = item;
            }

            if (candidate == 0 || (gameState.DCheck() != 0 && gameState.DwarfFlag >= 2))
            {
                NeedObject();
                return;
            }

            gameState.Object = candidate;
            VTake();
        }

        private void IVOpen()
        {
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
                Speak(28);
                return;
            }

            gameState.Object = candidate;
            VOpen();
        }

        private void IVKill()
        {
            int candidate = 0;
            bool ambiguous = false;

            if (gameState.DCheck() != 0 && gameState.DwarfFlag >= 2)
                candidate = GameConstants.Dwarf;
            AddCandidate(GameConstants.Snake, gameState.Here(GameConstants.Snake), ref candidate, ref ambiguous);
            AddCandidate(GameConstants.Dragon, gameState.At(GameConstants.Dragon) && gameState.ObjectProperties[GameConstants.Dragon] == 0, ref candidate, ref ambiguous);
            AddCandidate(GameConstants.Troll, gameState.At(GameConstants.Troll), ref candidate, ref ambiguous);
            AddCandidate(GameConstants.Bear, gameState.Here(GameConstants.Bear) && gameState.ObjectProperties[GameConstants.Bear] == 0, ref candidate, ref ambiguous);

            if (ambiguous)
            {
                NeedObject();
                return;
            }

            if (candidate != 0)
            {
                gameState.Object = candidate;
                VKill();
                return;
            }

            if (gameState.Here(GameConstants.Bird) && gameState.Verb != GameConstants.Throw)
                candidate = GameConstants.Bird;
            AddCandidate(GameConstants.Clam, gameState.Here(GameConstants.Clam) || gameState.Here(GameConstants.Oyster), ref candidate, ref ambiguous);

            if (ambiguous)
            {
                NeedObject();
                return;
            }

            gameState.Object = candidate;
            VKill();
        }

        private void IVEat()
        {
            if (!gameState.Here(GameConstants.Food))
            {
                NeedObject();
                return;
            }

            gameState.Object = GameConstants.Food;
            VEat();
        }

        private void IVDrink()
        {
            if (gameState.LiqLoc(gameState.Location) != GameConstants.Water &&
                (gameState.Liq() != GameConstants.Water || !gameState.Here(GameConstants.Bottle)))
            {
                NeedObject();
                return;
            }

            gameState.Object = GameConstants.Water;
            VDrink();
        }

        private void IVQuit()
        {
            gameState.GaveUp = AskYesNo(22, 0, 54);
            if (gameState.GaveUp)
                NormalEnd();
        }

        private void IVFill()
        {
            if (!gameState.Here(GameConstants.Bottle))
            {
                NeedObject();
                return;
            }

            gameState.Object = GameConstants.Bottle;
            VFill();
        }

        private void IVFoo()
        {
            int k = gameState.Verb - GameConstants.Fee + 1;
            int message = 42;

            if (gameState.FooBar != 1 - k)
            {
                if (gameState.FooBar != 0)
                    message = 151;
                Speak(message);
                return;
            }

            gameState.FooBar = k;
            if (k != 4)
                return;

            gameState.FooBar = 0;
            if (gameState.ObjectLocations[GameConstants.Eggs] == 92 ||
                (gameState.Toting(GameConstants.Eggs) && gameState.Location == 92))
            {
                Speak(message);
                return;
            }

            if (gameState.ObjectLocations[GameConstants.Eggs] == 0 &&
                gameState.ObjectLocations[GameConstants.Troll] == 0 &&
                gameState.ObjectProperties[GameConstants.Troll] == 0)
            {
                gameState.SetObjectProperty(GameConstants.Troll, 1);
            }

            if (gameState.Here(GameConstants.Eggs))
                k = 1;
            else if (gameState.Location == 92)
                k = 0;
            else
                k = 2;

            gameState.MoveObject(GameConstants.Eggs, 92);
            PrintObjectMessage(GameConstants.Eggs, k);
        }

        private void IVRead()
        {
            int candidate = 0;
            if (gameState.Here(GameConstants.Magazine))
                candidate = GameConstants.Magazine;
            if (gameState.Here(GameConstants.Tablet))
                candidate = candidate * 100 + GameConstants.Tablet;
            if (gameState.Here(GameConstants.Message))
                candidate = candidate * 100 + GameConstants.Message;

            if (candidate > 100 || candidate == 0 || DarknessManager.IsDark(gameState))
            {
                NeedObject();
                return;
            }

            gameState.Object = candidate;
            VRead();
        }

        private void VTake()
        {
            int objectId = gameState.Object;
            if (gameState.Toting(objectId))
            {
                ActSpeak(gameState.Verb);
                return;
            }

            int message = 25;
            if (objectId == GameConstants.Plant && gameState.ObjectProperties[GameConstants.Plant] <= 0)
                message = 115;
            if (objectId == GameConstants.Bear && gameState.ObjectProperties[GameConstants.Bear] == 1)
                message = 169;
            if (objectId == GameConstants.Chain && gameState.ObjectProperties[GameConstants.Bear] != 0)
                message = 170;
            if (gameState.FixedObjectLocations[objectId] != 0)
            {
                Speak(message);
                return;
            }

            if (objectId == GameConstants.Water || objectId == GameConstants.Oil)
            {
                if (!gameState.Here(GameConstants.Bottle) || gameState.Liq() != objectId)
                {
                    gameState.Object = GameConstants.Bottle;
                    if (gameState.Toting(GameConstants.Bottle) && gameState.ObjectProperties[GameConstants.Bottle] == 1)
                    {
                        VFill();
                        return;
                    }

                    if (gameState.ObjectProperties[GameConstants.Bottle] != 1)
                        message = 105;
                    if (!gameState.Toting(GameConstants.Bottle))
                        message = 104;
                    Speak(message);
                    return;
                }

                objectId = GameConstants.Bottle;
                gameState.Object = objectId;
            }

            if (gameState.Holding >= 7)
            {
                Speak(92);
                return;
            }

            if (objectId == GameConstants.Bird && gameState.ObjectProperties[GameConstants.Bird] == 0)
            {
                if (gameState.Toting(GameConstants.Rod))
                {
                    Speak(26);
                    return;
                }

                if (!gameState.Toting(GameConstants.Cage))
                {
                    Speak(27);
                    return;
                }

                gameState.SetObjectProperty(GameConstants.Bird, 1);
            }

            if ((objectId == GameConstants.Bird || objectId == GameConstants.Cage) &&
                gameState.ObjectProperties[GameConstants.Bird] != 0)
            {
                gameState.Carry(GameConstants.Bird + GameConstants.Cage - objectId, gameState.Location);
            }

            gameState.Carry(objectId, gameState.Location);
            int liquid = gameState.Liq();
            if (objectId == GameConstants.Bottle && liquid != 0)
                gameState.SetObjectLocation(liquid, -1);

            Speak(54);
        }

        private void VDrop()
        {
            int objectId = gameState.Object;
            if (gameState.Toting(GameConstants.Rod2) && objectId == GameConstants.Rod && !gameState.Toting(GameConstants.Rod))
            {
                objectId = GameConstants.Rod2;
                gameState.Object = objectId;
            }

            if (!gameState.Toting(objectId))
            {
                ActSpeak(gameState.Verb);
                return;
            }

            int message = 54;
            if (objectId == GameConstants.Bird && gameState.Here(GameConstants.Snake))
            {
                Speak(30);
                message = 0;
                if (gameState.Closed)
                    DwarfEnd(136);
                gameState.Destroy(GameConstants.Snake);
                gameState.SetObjectProperty(GameConstants.Snake, -1);
            }
            else if (objectId == GameConstants.Coins && gameState.Here(GameConstants.Vend))
            {
                gameState.Destroy(GameConstants.Coins);
                gameState.Drop(GameConstants.Batteries, gameState.Location);
                PrintObjectMessage(GameConstants.Batteries, 0);
                return;
            }
            else if (objectId == GameConstants.Bird &&
                gameState.At(GameConstants.Dragon) &&
                gameState.ObjectProperties[GameConstants.Dragon] == 0)
            {
                Speak(154);
                gameState.Destroy(GameConstants.Bird);
                gameState.SetObjectProperty(GameConstants.Bird, 0);
                if (gameState.ObjectLocations[GameConstants.Snake] != 0)
                    gameState.Tally2++;
                return;
            }

            if (objectId == GameConstants.Bear && gameState.At(GameConstants.Troll))
            {
                Speak(163);
                message = 0;
                gameState.MoveObject(GameConstants.Troll, 0);
                gameState.MoveObject(GameConstants.Troll + GameConstants.MaxObjects, 0);
                gameState.MoveObject(GameConstants.Troll2, 117);
                gameState.MoveObject(GameConstants.Troll2 + GameConstants.MaxObjects, 122);
                GameState.Juggle(GameConstants.Chasm);
                gameState.SetObjectProperty(GameConstants.Troll, 2);
            }
            else if (objectId == GameConstants.Vase)
            {
                if (gameState.Location == 96)
                    Speak(54);
                else
                {
                    int vaseProperty = gameState.At(GameConstants.Pillow) ? 0 : 2;
                    gameState.SetObjectProperty(GameConstants.Vase, vaseProperty);
                    PrintObjectMessage(GameConstants.Vase, vaseProperty + 1);
                    if (vaseProperty != 0)
                        gameState.FixedObjectLocations[GameConstants.Vase] = -1;
                }

                message = 0;
            }

            int liquid = gameState.Liq();
            if (liquid == objectId)
            {
                objectId = GameConstants.Bottle;
                gameState.Object = objectId;
            }

            if (objectId == GameConstants.Bottle && liquid != 0)
                gameState.SetObjectLocation(liquid, 0);
            if (objectId == GameConstants.Cage && gameState.ObjectProperties[GameConstants.Bird] != 0)
                gameState.Drop(GameConstants.Bird, gameState.Location);
            if (objectId == GameConstants.Bird)
                gameState.SetObjectProperty(GameConstants.Bird, 0);

            gameState.Drop(objectId, gameState.Location);
            if (message != 0)
                Speak(message);
        }

        private void VOpen()
        {
            int message;
            switch (gameState.Object)
            {
                case GameConstants.Clam:
                case GameConstants.Oyster:
                    int oysterOffset = gameState.Object == GameConstants.Oyster ? 1 : 0;
                    if (gameState.Verb == GameConstants.Lock)
                        message = 61;
                    else if (!gameState.Toting(GameConstants.Trident))
                        message = 122 + oysterOffset;
                    else if (gameState.Toting(gameState.Object))
                        message = 120 + oysterOffset;
                    else
                    {
                        message = 124 + oysterOffset;
                        gameState.Destroy(GameConstants.Clam);
                        gameState.Drop(GameConstants.Oyster, gameState.Location);
                        gameState.Drop(GameConstants.Pearl, 105);
                    }
                    break;
                case GameConstants.Door:
                    message = gameState.ObjectProperties[GameConstants.Door] == 1 ? 54 : 111;
                    break;
                case GameConstants.Cage:
                    message = 32;
                    break;
                case GameConstants.Keys:
                    message = 55;
                    break;
                case GameConstants.Chain:
                    message = OpenChain();
                    break;
                case GameConstants.Grate:
                    message = OpenGrate();
                    break;
                default:
                    message = 33;
                    break;
            }

            Speak(message);
        }

        private int OpenChain()
        {
            if (!gameState.Here(GameConstants.Keys))
                return 31;

            if (gameState.Verb == GameConstants.Lock)
            {
                if (gameState.ObjectProperties[GameConstants.Chain] != 0)
                    return 34;
                if (gameState.Location != 130)
                    return 173;

                gameState.SetObjectProperty(GameConstants.Chain, 2);
                if (gameState.Toting(GameConstants.Chain))
                    gameState.Drop(GameConstants.Chain, gameState.Location);
                gameState.FixedObjectLocations[GameConstants.Chain] = -1;
                return 172;
            }

            if (gameState.ObjectProperties[GameConstants.Bear] == 400)
                return 41;
            if (gameState.ObjectProperties[GameConstants.Chain] == 0)
                return 37;

            gameState.SetObjectProperty(GameConstants.Chain, 0);
            gameState.FixedObjectLocations[GameConstants.Chain] = 0;
            if (gameState.ObjectProperties[GameConstants.Bear] != 3)
                gameState.SetObjectProperty(GameConstants.Bear, 2);
            gameState.FixedObjectLocations[GameConstants.Bear] = 2 - gameState.ObjectProperties[GameConstants.Bear];
            return 171;
        }

        private int OpenGrate()
        {
            if (!gameState.Here(GameConstants.Keys))
                return 31;

            if (gameState.Closing)
            {
                if (!gameState.Panic)
                {
                    gameState.Clock2 = 15;
                    gameState.Panic = true;
                }

                return 130;
            }

            int message = 34 + gameState.ObjectProperties[GameConstants.Grate];
            gameState.SetObjectProperty(GameConstants.Grate, gameState.Verb == GameConstants.Lock ? 0 : 1);
            return message + 2 * gameState.ObjectProperties[GameConstants.Grate];
        }

        private void VSay()
        {
            string said = string.Equals(gameState.Word1, "say", StringComparison.OrdinalIgnoreCase)
                ? gameState.Word2
                : gameState.Word1;
            Console.WriteLine("Okay.");
            if (!string.IsNullOrWhiteSpace(said))
                Console.WriteLine(said);
        }

        private void VOn()
        {
            if (!gameState.Here(GameConstants.Lamp))
            {
                ActSpeak(gameState.Verb);
                return;
            }

            if (gameState.Limit < 0)
            {
                Speak(184);
                return;
            }

            gameState.SetObjectProperty(GameConstants.Lamp, 1);
            Speak(39);
            if (gameState.WizardDark)
            {
                gameState.WizardDark = false;
                ShowLocationDescription();
            }
        }

        private void VOff()
        {
            if (!gameState.Here(GameConstants.Lamp))
            {
                ActSpeak(gameState.Verb);
                return;
            }

            gameState.SetObjectProperty(GameConstants.Lamp, 0);
            Speak(40);
        }

        private void VWave()
        {
            int objectId = gameState.Object;
            if (!gameState.Toting(objectId) &&
                (objectId != GameConstants.Rod || !gameState.Toting(GameConstants.Rod2)))
            {
                Speak(29);
            }
            else if (objectId != GameConstants.Rod ||
                !gameState.At(GameConstants.Fissure) ||
                !gameState.Toting(objectId) ||
                gameState.Closing)
            {
                ActSpeak(gameState.Verb);
            }
            else
            {
                gameState.SetObjectProperty(GameConstants.Fissure, 1 - gameState.ObjectProperties[GameConstants.Fissure]);
                PrintObjectMessage(GameConstants.Fissure, 2 - gameState.ObjectProperties[GameConstants.Fissure]);
            }
        }

        private void VKill()
        {
            int message;
            switch (gameState.Object)
            {
                case GameConstants.Bird:
                    if (gameState.Closed)
                        message = 137;
                    else
                    {
                        gameState.Destroy(GameConstants.Bird);
                        gameState.SetObjectProperty(GameConstants.Bird, 0);
                        if (gameState.ObjectLocations[GameConstants.Snake] == 19)
                            gameState.Tally2++;
                        message = 45;
                    }
                    break;
                case 0:
                    message = 44;
                    break;
                case GameConstants.Clam:
                case GameConstants.Oyster:
                    message = 150;
                    break;
                case GameConstants.Snake:
                    message = 46;
                    break;
                case GameConstants.Dwarf:
                    if (gameState.Closed)
                        DwarfEnd(136);
                    message = 49;
                    break;
                case GameConstants.Troll:
                    message = 157;
                    break;
                case GameConstants.Bear:
                    message = 165 + (gameState.ObjectProperties[GameConstants.Bear] + 1) / 2;
                    break;
                case GameConstants.Dragon:
                    if (gameState.ObjectProperties[GameConstants.Dragon] != 0)
                    {
                        message = 167;
                        break;
                    }

                    if (!AskYesNo(49, 0, 0))
                        return;

                    PrintObjectMessage(GameConstants.Dragon, 1);
                    gameState.SetObjectProperty(GameConstants.Dragon, 2);
                    gameState.SetObjectProperty(GameConstants.Rug, 0);
                    gameState.MoveObject(GameConstants.Dragon + GameConstants.MaxObjects, -1);
                    gameState.MoveObject(GameConstants.Rug + GameConstants.MaxObjects, 0);
                    gameState.MoveObject(GameConstants.Dragon, 120);
                    gameState.MoveObject(GameConstants.Rug, 120);
                    for (int item = 1; item < GameConstants.MaxObjects; item++)
                    {
                        if (gameState.ObjectLocations[item] == 119 || gameState.ObjectLocations[item] == 121)
                            gameState.MoveObject(item, 120);
                    }

                    gameState.NewLocation = 120;
                    return;
                default:
                    ActSpeak(gameState.Verb);
                    return;
            }

            Speak(message);
        }

        private void VPour()
        {
            int objectId = gameState.Object;
            if (objectId == GameConstants.Bottle || objectId == 0)
                objectId = gameState.Liq();
            if (objectId == 0)
            {
                NeedObject();
                return;
            }

            if (!gameState.Toting(objectId))
            {
                ActSpeak(gameState.Verb);
                return;
            }

            if (objectId != GameConstants.Oil && objectId != GameConstants.Water)
            {
                Speak(78);
                return;
            }

            gameState.SetObjectProperty(GameConstants.Bottle, 1);
            gameState.SetObjectLocation(objectId, 0);

            if (gameState.At(GameConstants.Plant))
            {
                if (objectId != GameConstants.Water)
                {
                    Speak(112);
                }
                else
                {
                    PrintObjectMessage(GameConstants.Plant, gameState.ObjectProperties[GameConstants.Plant] + 1);
                    gameState.SetObjectProperty(GameConstants.Plant, (gameState.ObjectProperties[GameConstants.Plant] + 2) % 6);
                    gameState.SetObjectProperty(GameConstants.Plant2, gameState.ObjectProperties[GameConstants.Plant] / 2);
                    ShowLocationDescription();
                }
            }
            else if (gameState.At(GameConstants.Door))
            {
                gameState.SetObjectProperty(GameConstants.Door, objectId == GameConstants.Oil ? 1 : 0);
                Speak(113 + gameState.ObjectProperties[GameConstants.Door]);
            }
            else
            {
                Speak(77);
            }
        }

        private void VEat()
        {
            switch (gameState.Object)
            {
                case GameConstants.Food:
                    gameState.Destroy(GameConstants.Food);
                    Speak(72);
                    break;
                case GameConstants.Bird:
                case GameConstants.Snake:
                case GameConstants.Clam:
                case GameConstants.Oyster:
                case GameConstants.Dwarf:
                case GameConstants.Dragon:
                case GameConstants.Troll:
                case GameConstants.Bear:
                    Speak(71);
                    break;
                default:
                    ActSpeak(gameState.Verb);
                    break;
            }
        }

        private void VDrink()
        {
            if (gameState.Object != GameConstants.Water)
            {
                Speak(110);
            }
            else if (gameState.Liq() != GameConstants.Water || !gameState.Here(GameConstants.Bottle))
            {
                ActSpeak(gameState.Verb);
            }
            else
            {
                gameState.SetObjectProperty(GameConstants.Bottle, 1);
                gameState.SetObjectLocation(GameConstants.Water, 0);
                Speak(74);
            }
        }

        private void VThrow()
        {
            int objectId = gameState.Object;
            if (gameState.Toting(GameConstants.Rod2) && objectId == GameConstants.Rod && !gameState.Toting(GameConstants.Rod))
            {
                objectId = GameConstants.Rod2;
                gameState.Object = objectId;
            }

            if (!gameState.Toting(objectId))
            {
                ActSpeak(gameState.Verb);
                return;
            }

            if (gameState.At(GameConstants.Troll) && objectId >= GameConstants.Nugget && objectId < GameConstants.MaxObjects)
            {
                Speak(159);
                gameState.Drop(objectId, 0);
                gameState.MoveObject(GameConstants.Troll, 0);
                gameState.MoveObject(GameConstants.Troll + GameConstants.MaxObjects, 0);
                gameState.Drop(GameConstants.Troll2, 117);
                gameState.Drop(GameConstants.Troll2 + GameConstants.MaxObjects, 122);
                GameState.Juggle(GameConstants.Chasm);
                return;
            }

            if (objectId == GameConstants.Food && gameState.Here(GameConstants.Bear))
            {
                gameState.Object = GameConstants.Bear;
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
                message = 48;
                if (GameState.Pct(random, 33))
                {
                    gameState.DwarfSeen[dwarf] = false;
                    gameState.DwarfLocations[dwarf] = 0;
                    message = 47;
                    gameState.DwarfKill++;
                    if (gameState.DwarfKill == 1)
                        message = 149;
                }
            }
            else if (gameState.At(GameConstants.Dragon) && gameState.ObjectProperties[GameConstants.Dragon] == 0)
            {
                message = 152;
            }
            else if (gameState.At(GameConstants.Troll))
            {
                message = 158;
            }
            else if (gameState.Here(GameConstants.Bear) && gameState.ObjectProperties[GameConstants.Bear] == 0)
            {
                Speak(164);
                gameState.Drop(GameConstants.Axe, gameState.Location);
                gameState.FixedObjectLocations[GameConstants.Axe] = -1;
                gameState.SetObjectProperty(GameConstants.Axe, 1);
                GameState.Juggle(GameConstants.Bear);
                return;
            }
            else
            {
                gameState.Verb = GameConstants.Kill;
                gameState.Object = 0;
                IVKill();
                return;
            }

            Speak(message);
            gameState.Drop(GameConstants.Axe, gameState.Location);
            ShowLocationDescription();
        }

        private void VFind()
        {
            int objectId = gameState.Object;
            if (gameState.Toting(objectId))
                Speak(24);
            else if (gameState.Closed)
                Speak(138);
            else if (gameState.DCheck() != 0 && gameState.DwarfFlag >= 2 && objectId == GameConstants.Dwarf)
                Speak(94);
            else if (gameState.At(objectId) ||
                (gameState.Liq() == objectId && gameState.Here(GameConstants.Bottle)) ||
                objectId == gameState.LiqLoc(gameState.Location))
                Speak(94);
            else
                ActSpeak(gameState.Verb);
        }

        private void VFill()
        {
            switch (gameState.Object)
            {
                case GameConstants.Bottle:
                    if (gameState.Liq() != 0)
                    {
                        Speak(105);
                    }
                    else
                    {
                        int liquidHere = gameState.LiqLoc(gameState.Location);
                        if (liquidHere == 0)
                        {
                            Speak(106);
                        }
                        else
                        {
                            gameState.SetObjectProperty(GameConstants.Bottle, gameState.LocationConditions[gameState.Location] & GameConstants.WatOil);
                            int liquid = gameState.Liq();
                            if (gameState.Toting(GameConstants.Bottle))
                                gameState.SetObjectLocation(liquid, -1);
                            Speak(liquid == GameConstants.Oil ? 108 : 107);
                        }
                    }
                    break;
                case GameConstants.Vase:
                    if (gameState.LiqLoc(gameState.Location) == 0)
                    {
                        Speak(144);
                    }
                    else if (!gameState.Toting(GameConstants.Vase))
                    {
                        Speak(29);
                    }
                    else
                    {
                        Speak(145);
                        VDrop();
                    }
                    break;
                default:
                    Speak(29);
                    break;
            }
        }

        private void VFeed()
        {
            int message;
            switch (gameState.Object)
            {
                case GameConstants.Bird:
                    message = 100;
                    break;
                case GameConstants.Dwarf:
                    if (!gameState.Here(GameConstants.Food))
                    {
                        ActSpeak(gameState.Verb);
                        return;
                    }
                    gameState.DwarfFlag++;
                    message = 103;
                    break;
                case GameConstants.Bear:
                    if (!gameState.Here(GameConstants.Food))
                    {
                        if (gameState.ObjectProperties[GameConstants.Bear] == 0)
                            message = 102;
                        else if (gameState.ObjectProperties[GameConstants.Bear] == 3)
                            message = 110;
                        else
                        {
                            ActSpeak(gameState.Verb);
                            return;
                        }
                        break;
                    }

                    gameState.Destroy(GameConstants.Food);
                    gameState.SetObjectProperty(GameConstants.Bear, 1);
                    gameState.FixedObjectLocations[GameConstants.Axe] = 0;
                    gameState.SetObjectProperty(GameConstants.Axe, 0);
                    message = 168;
                    break;
                case GameConstants.Dragon:
                    message = gameState.ObjectProperties[GameConstants.Dragon] != 0 ? 110 : 102;
                    break;
                case GameConstants.Troll:
                    message = 182;
                    break;
                case GameConstants.Snake:
                    if (gameState.Closed || !gameState.Here(GameConstants.Bird))
                    {
                        message = 102;
                        break;
                    }

                    message = 101;
                    gameState.Destroy(GameConstants.Bird);
                    gameState.SetObjectProperty(GameConstants.Bird, 0);
                    gameState.Tally2++;
                    break;
                default:
                    message = 14;
                    break;
            }

            Speak(message);
        }

        private void VRead()
        {
            if (DarknessManager.IsDark(gameState))
            {
                SpeakObjectNotHere(gameState.Object);
                return;
            }

            switch (gameState.Object)
            {
                case GameConstants.Magazine:
                    Speak(190);
                    break;
                case GameConstants.Tablet:
                    Speak(196);
                    break;
                case GameConstants.Message:
                    Speak(191);
                    break;
                case GameConstants.Oyster:
                    if (!gameState.Toting(GameConstants.Oyster) || !gameState.Closed)
                    {
                        ActSpeak(gameState.Verb);
                        break;
                    }

                    if ((gameState.HintAvailable & GameConstants.HintO) != 0)
                    {
                        if (AskYesNo(192, 193, 54))
                        {
                            gameState.HintTaken++;
                            gameState.HintAvailable &= ~GameConstants.HintO;
                        }
                    }
                    else
                    {
                        Speak(194);
                    }
                    break;
                default:
                    ActSpeak(gameState.Verb);
                    break;
            }
        }

        private void VBlast()
        {
            if (gameState.ObjectProperties[GameConstants.Rod2] < 0 || !gameState.Closed)
            {
                ActSpeak(gameState.Verb);
                return;
            }

            gameState.Bonus = 133;
            if (gameState.Location == 115)
                gameState.Bonus = 134;
            if (gameState.Here(GameConstants.Rod2))
                gameState.Bonus = 135;

            Speak(gameState.Bonus);
            NormalEnd();
        }

        private void VBreak()
        {
            if (gameState.Object == GameConstants.Mirror)
            {
                if (gameState.Closed)
                {
                    Speak(197);
                    DwarfEnd(136);
                    return;
                }

                Speak(148);
            }
            else if (gameState.Object == GameConstants.Vase && gameState.ObjectProperties[GameConstants.Vase] == 0)
            {
                if (gameState.Toting(GameConstants.Vase))
                    gameState.Drop(GameConstants.Vase, gameState.Location);
                gameState.SetObjectProperty(GameConstants.Vase, 2);
                gameState.FixedObjectLocations[GameConstants.Vase] = -1;
                Speak(198);
            }
            else
            {
                ActSpeak(gameState.Verb);
            }
        }

        private void VWake()
        {
            if (gameState.Object != GameConstants.Dwarf || !gameState.Closed)
                ActSpeak(gameState.Verb);
            else
                DwarfEnd(199);
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

            if (messageId == 54)
            {
                Console.WriteLine("OK");
                return;
            }

            Console.WriteLine(GameMessages.GetMessage(messageId));
        }

        private void SpeakObjectNotHere(int objectId)
        {
            Console.WriteLine($"I see no {GetObjectWord(objectId)} here.");
        }

        private void NeedObject()
        {
            string word = IsVerbWord(gameState.Word1) ? gameState.Word1 : gameState.Word2;
            if (string.IsNullOrWhiteSpace(word))
                word = "do";
            Console.WriteLine($"{word} what?");
        }

        private bool IsVerbWord(string word)
        {
            return !string.IsNullOrWhiteSpace(word) &&
                Vocabulary.AnalyzeWord(word, out int type, out _) &&
                type == Vocabulary.WordTypes.Verb;
        }

        private string GetObjectWord(int objectId)
        {
            if (IsObjectWord(gameState.Word1, objectId))
                return gameState.Word1;
            if (IsObjectWord(gameState.Word2, objectId))
                return gameState.Word2;

            return GameObjects.Objects.TryGetValue(objectId, out GameObjectData? objectData)
                ? objectData.Name.ToLowerInvariant()
                : $"object #{objectId}";
        }

        private static bool IsObjectWord(string word, int objectId)
        {
            return !string.IsNullOrWhiteSpace(word) &&
                Vocabulary.AnalyzeWord(word, out int type, out int value) &&
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
                    Speak(89);
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

                Speak(89);
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
            int[] limits = [35, 100, 200, 250, 300, 330, 350, 1000];
            int ratingIndex = 0;
            while (ratingIndex < limits.Length && limits[ratingIndex] <= total)
                ratingIndex++;

            Console.WriteLine();
            int ratingMessage = 202 + ratingIndex;
            if (ratingMessage <= 209)
                Speak(ratingMessage);

            int next = ratingIndex < limits.Length ? limits[ratingIndex] - total : 0;
            if (next > 0)
                Console.WriteLine($"To achieve the next higher rating, you need {next} more point{(next == 1 ? string.Empty : "s")}.");

            gameState.SaveFlag = true;
        }

        private int PrintScore()
        {
            int score = 0;
            int treasures = 0;

            for (int item = GameConstants.Nugget; item <= GameConstants.MaxTreasures; item++)
            {
                int itemScore = item == GameConstants.Chest ? 14 : item > GameConstants.Chest ? 16 : 12;
                if (gameState.ObjectProperties[item] >= 0)
                    treasures += 2;
                if (gameState.ObjectLocations[item] == GameConstants.WellHouse && gameState.ObjectProperties[item] == 0)
                    treasures += itemScore - 2;
            }

            PrintScoreLine("Treasures:", treasures);
            score += treasures;

            int survival = (GameConstants.MaxDeaths - gameState.NumDie) * 10;
            if (survival != 0)
                PrintScoreLine("Survival:", survival);
            score += survival;

            if (!gameState.GaveUp)
                score += 4;

            int gettingIn = gameState.VisitedLocations[19] != 0 ? 25 : 0;
            if (gettingIn != 0)
                PrintScoreLine("Getting well in:", gettingIn);
            score += gettingIn;

            int masters = gameState.Closing ? 25 : 0;
            if (masters != 0)
                PrintScoreLine("Masters section:", masters);
            score += masters;

            if (gameState.Closed)
            {
                int bonus = gameState.Bonus == 0 ? 10 :
                    gameState.Bonus == 135 ? 25 :
                    gameState.Bonus == 134 ? 30 :
                    gameState.Bonus == 133 ? 45 : 0;
                PrintScoreLine("Bonus:", bonus);
                score += bonus;
            }

            if (gameState.ObjectLocations[GameConstants.Magazine] == 108)
                score += 1;

            int hints = -15 * gameState.HintTaken;
            if (hints != 0)
            {
                PrintScoreLine("Hints & instructions:", hints);
                score += hints;
            }

            score += 2;
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
                if (GameObjects.Objects.TryGetValue(objectId, out GameObjectData? objectData))
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
            if (gameState.Toting(GameConstants.Bear))
                Speak(141);

            if (DarknessManager.IsDark(gameState))
            {
                Speak(16);
            }
            else
            {
                bool useShortDescription = !forceLong &&
                    ((gameState.VisitedLocations[gameState.Location] & 3) != 0 ||
                    ((gameState.Detail & gameState.TestBr) != 0 && gameState.VisitedLocations[gameState.Location] != 0));

                if (!useShortDescription &&
                    LocationDescriptions.LongDescriptions.TryGetValue(gameState.Location, out string? longDesc))
                {
                    Console.WriteLine(longDesc);
                }
                else if (LocationDescriptions.ShortDescriptions.TryGetValue(gameState.Location, out string? shortDesc))
                {
                    Console.WriteLine(shortDesc);
                }
                else
                {
                    Console.WriteLine($"You are in location {gameState.Location}.");
                }

                if (!DarknessManager.IsDark(gameState))
                    ShowObjectsHere();
            }

            if (gameState.Location == 33 && GameState.Pct(random, 25) && !gameState.Closing)
                Speak(8);
        }

        /// <summary>
        /// Shows objects present at the current location.
        /// </summary>
        private void ShowObjectsHere()
        {
            bool printedAny = false;

            for (int objectId = 1; objectId < GameConstants.MaxObjects; objectId++)
            {
                if (!gameState.At(objectId))
                    continue;

                if (objectId == GameConstants.Steps && gameState.Toting(GameConstants.Nugget))
                    continue;

                if (gameState.ObjectProperties[objectId] < 0)
                {
                    if (gameState.Closed)
                        continue;

                    gameState.SetObjectProperty(objectId, 0);
                    if (objectId == GameConstants.Rug || objectId == GameConstants.Chain)
                        gameState.SetObjectProperty(objectId, gameState.ObjectProperties[objectId] + 1);
                    gameState.Tally--;
                }

                int state = objectId == GameConstants.Steps &&
                    gameState.Location == gameState.FixedObjectLocations[GameConstants.Steps]
                    ? 1
                    : gameState.ObjectProperties[objectId];

                if (!printedAny && (gameState.Detail & 2) == 0)
                {
                    Console.WriteLine();
                    printedAny = true;
                }

                PrintObjectMessage(objectId, state);
            }

            if (gameState.Tally == gameState.Tally2 && gameState.Tally != 0 && gameState.Limit > 35)
                gameState.Limit = 35;
        }

        /// <summary>
        /// Shows game instructions.
        /// </summary>
        private void ShowInstructions()
        {
            Console.WriteLine(GameMessages.GetMessage(1));
        }

        /// <summary>
        /// Updates the game state after each turn.
        /// </summary>
        private void UpdateGameState()
        {
            gameState.FooBar = gameState.FooBar > 0 ? -gameState.FooBar : 0;
            gameState.TestBr = 2;

            // Check lamp battery status and decrement if lamp is on
            string? batteryMessage = DarknessManager.CheckBatteryStatus(gameState);
            if (batteryMessage != null)
            {
                Console.WriteLine(batteryMessage);
            }

            // Check for game end conditions
            if (gameState.Turns >= gameState.Limit)
            {
                Console.WriteLine("You have exceeded the turn limit.");
                gameState.SaveFlag = true;
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
            if (!gameState.Closing)
            {
                bool reincarnate = AskYesNo(81 + gameState.NumDie * 2, 82 + gameState.NumDie * 2, 54);
                gameState.NumDie++;

                if (gameState.NumDie >= GameConstants.MaxDeaths || !reincarnate)
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
                        gameState.Drop(item, item == GameConstants.Lamp ? GameConstants.EndOfRoad : gameState.OldLocation2);
                }

                gameState.NewLocation = GameConstants.WellHouse;
                gameState.OldLocation = gameState.Location;
                gameState.Location = 0;
                return;
            }

            Speak(131);
            gameState.NumDie++;
            NormalEnd();
        }
    }
