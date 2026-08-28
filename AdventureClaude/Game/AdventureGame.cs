namespace AdventureClaude.Game;

using System;
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
        private readonly DwarfPirateEngine dwarfPirateEngine;
        private readonly TurnLifecycleEngine turnLifecycleEngine;
        private readonly VerbHandlers verbHandlers;
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
            dwarfPirateEngine = new DwarfPirateEngine(gameState, this.random, Speak, HandleDeath);
            verbHandlers = new VerbHandlers(
                gameState,
                this.random,
                travelEngine,
                PrintObjectMessage,
                ShowLocationDescription,
                AskYesNo,
                DwarfEnd,
                NormalEnd,
                PrintScore);
            turnLifecycleEngine = new TurnLifecycleEngine(
                gameState,
                this.random,
                dwarfPirateEngine.ApplyDwarfBlock,
                dwarfPirateEngine.RunDwarves,
                TryLocationHint,
                PrintObjectMessage,
                ShowLocationDescription,
                travelEngine.DoMove,
                HandleDeath,
                NormalEnd,
                Speak);
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
            verbHandlers.ProcessCommand();
        }

        /// <summary>
        /// Applies the NewLocation chosen by DoTravel. This is the movement subset of TURN.C turn().
        /// </summary>
        private bool RunTurnLifecycleBeforeInput()
        {
            return turnLifecycleEngine.RunBeforeInput();
        }

        private void ApplyClosingExitGuard()
        {
            turnLifecycleEngine.ApplyClosingExitGuard();
        }

        private void ApplyDwarfBlock()
        {
            dwarfPirateEngine.ApplyDwarfBlock();
        }

        private void RunDwarves()
        {
            dwarfPirateEngine.RunDwarves();
        }

        private void DoPirate()
        {
            dwarfPirateEngine.DoPirate();
        }

        private void PrintObjectMessage(int objectId, int state)
        {
            if (!AdventureData.TryGetObjectRoomDescription(objectId, state, out string message))
                return;

            Console.WriteLine(message);
        }

        private void ApplyClosedInventoryState()
        {
            turnLifecycleEngine.ApplyClosedInventoryState();
        }

        private bool RunSpecialTimer()
        {
            return turnLifecycleEngine.RunSpecialTimer();
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

