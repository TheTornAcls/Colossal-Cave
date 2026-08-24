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
        private readonly GameState gameState;
        private readonly InputParser inputParser;
        private readonly Random random;

        public AdventureGame()
        {
            gameState = new GameState();
            inputParser = new InputParser();
            random = new Random(511); // Same seed as original
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
                gameState.Limit = 1000;
                gameState.HintTaken++;
            }
            else
            {
                gameState.Limit = 330;
            }

            Console.WriteLine();
            ShowLocationDescription();
            
            // Main game loop
            while (!gameState.SaveFlag)
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
            gameState.Turns++;

            // Get player input
            Console.Write("> ");
            string input = Console.ReadLine() ?? string.Empty;

            // Parse input
            if (!inputParser.ParseInput(input, gameState, out int verb, out int objectId, out int motion))
            {
                return; // Invalid input, try again
            }

            // Store parsed values in game state
            gameState.Verb = verb;
            gameState.Object = objectId;
            gameState.Motion = motion;

            // Process the command
            ProcessCommand();

            // Update game state and check for end conditions
            UpdateGameState();
        }

        /// <summary>
        /// Processes the parsed command.
        /// </summary>
        private void ProcessCommand()
        {
            // Handle motion commands
            if (gameState.Motion > 0)
            {
                DoMove();
                ApplyLocationChange();
                return;
            }

            // Handle verb commands
            if (gameState.Verb > 0)
            {
                HandleVerb(gameState.Verb, gameState.Object);
                return;
            }
        }

        /// <summary>
        /// C TURN.C domove(): dispatches special motion words and normal travel.
        /// </summary>
        private void DoMove()
        {
            switch (gameState.Motion)
            {
                case GameConstants.NullMotion:
                    break;
                case GameConstants.Back:
                    GoBack();
                    break;
                case GameConstants.Look:
                    if (gameState.Detail == 0)
                    {
                        Console.WriteLine(GameMessages.GetMessage(15));
                        gameState.Detail |= 1;
                    }

                    gameState.WizardDark = false;
                    gameState.VisitedLocations[gameState.Location] =
                        (short)((gameState.VisitedLocations[gameState.Location] + 3) & ~3);
                    gameState.TestBr = 0;
                    gameState.NewLocation = gameState.Location;
                    gameState.Location = 0;
                    break;
                case GameConstants.Cave:
                    Console.WriteLine(GameMessages.GetMessage(gameState.Location < 8 ? 57 : 58));
                    break;
                default:
                    gameState.OldLocation2 = gameState.OldLocation;
                    gameState.OldLocation = gameState.Location;
                    DoTravel();
                    break;
            }
        }

        /// <summary>
        /// C TURN.C goback(): tries to infer the reverse route through the travel table.
        /// </summary>
        private void GoBack()
        {
            int want = gameState.Forced(gameState.OldLocation)
                ? gameState.OldLocation2
                : gameState.OldLocation;

            gameState.OldLocation2 = gameState.OldLocation;
            gameState.OldLocation = gameState.Location;

            if (want == gameState.Location)
            {
                Console.WriteLine(GameMessages.GetMessage(91));
                return;
            }

            List<TravelEntry> travel = TravelData.GetTravelOptions(gameState.Location);
            TravelEntry? fallback = null;

            foreach (TravelEntry entry in travel)
            {
                if (entry.Condition == 0 && entry.Destination == want)
                {
                    gameState.Motion = entry.Verb;
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
                gameState.Motion = fallback.Verb;
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
            List<TravelEntry> travel = TravelData.GetTravelOptions(gameState.Location);
            gameState.NewLocation = gameState.Location;
            bool hit = false;
            bool moved = false;
            int selectedDestination = gameState.Location;
            int roll = GameState.RRand(random, 0, 99);

            foreach (TravelEntry entry in travel)
            {
                int destination = entry.Destination;
                int verb = entry.Verb;
                int condition = entry.Condition;

                if (verb != 1 && verb != gameState.Motion && !hit)
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
                gameState.NewLocation = selectedDestination;
                if (gameState.NewLocation == gameState.Location)
                    gameState.Location = 0;
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
            int message = 12;
            if (gameState.Motion >= 43 && gameState.Motion <= 50)
                message = 9;
            if (gameState.Motion == 29 || gameState.Motion == 30)
                message = 9;
            if (gameState.Motion == 7 || gameState.Motion == 36 || gameState.Motion == 37)
                message = 10;
            if (gameState.Motion == 11 || gameState.Motion == 19)
                message = 11;
            if (gameState.Verb == GameConstants.Find || gameState.Verb == GameConstants.Inventory)
                message = 59;
            if (gameState.Motion == 62 || gameState.Motion == 65)
                message = 42;
            if (gameState.Motion == 17)
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

            gameState.WizardDark = DarknessManager.IsDark(gameState);
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

        /// <summary>
        /// Handles verb commands.
        /// </summary>
        private void HandleVerb(int verb, int objectId)
        {
            switch (verb)
            {
                case GameConstants.Inventory: // 20
                    ShowInventory();
                    break;
                case GameConstants.Look: // 57
                    ShowLocationDescription(true);
                    break;
                case GameConstants.Take: // 1
                    HandleTake(objectId);
                    break;
                case GameConstants.Drop: // 2
                    HandleDrop(objectId);
                    break;
                case GameConstants.Fill: // 22
                    HandleFill(objectId);
                    break;
                case GameConstants.Pour: // 13
                    HandlePour(objectId);
                    break;
                case GameConstants.Drink: // 15
                    HandleDrink(objectId);
                    break;
                case GameConstants.On: // 7
                    HandleLampOn();
                    break;
                case GameConstants.Off: // 8
                    HandleLampOff();
                    break;
                case GameConstants.Quit: // 18
                    HandleQuit();
                    break;
                case 51: // Help
                    Console.WriteLine(GameMessages.GetMessage(51));
                    break;
                default:
                    Console.WriteLine(GameMessages.GetMessage(12)); // "I don't know how to apply that word here."
                    break;
            }
        }

        /// <summary>
        /// Handles taking an object.
        /// Ported from vtake() in VERB.C with liquid logic.
        /// </summary>
        private void HandleTake(int objectId)
        {
            if (objectId == 0)
            {
                Console.WriteLine(GameMessages.GetMessage(43)); // "Where?"
                return;
            }

            // Special handling for water/oil - redirect to bottle
            if (objectId == GameConstants.Water || objectId == GameConstants.Oil)
            {
                if (!gameState.IsObjectHere(GameConstants.Bottle) || LiquidManager.Liq(gameState) != objectId)
                {
                    objectId = GameConstants.Bottle;
                    if (gameState.IsCarrying(GameConstants.Bottle) && gameState.ObjectProperties[GameConstants.Bottle] == 1)
                    {
                        // Bottle is empty, try to fill it
                        HandleFill(GameConstants.Bottle);
                        return;
                    }
                }
                objectId = GameConstants.Bottle;
            }

            if (gameState.IsCarrying(objectId))
            {
                Console.WriteLine(GameMessages.GetMessage(24)); // "You are already carrying it!"
                return;
            }

            if (!gameState.IsObjectHere(objectId))
            {
                Console.WriteLine(GameMessages.GetMessage(170)); // "I see no {object} here."
                return;
            }

            // Count carried objects
            List<int> carriedObjects = gameState.GetCarriedObjects();
            if (carriedObjects.Count >= 7) // Adventure limit
            {
                Console.WriteLine("You can't carry any more!");
                return;
            }

            gameState.Carry(objectId, gameState.Location);
            
            // If taking bottle with liquid, mark liquid as being carried
            if (objectId == GameConstants.Bottle)
            {
                int liquid = LiquidManager.Liq(gameState);
                if (liquid != 0)
                {
                    gameState.SetObjectLocation(liquid, -1);
                }
            }
            
            Console.WriteLine(GameMessages.GetMessage(54)); // "OK"
        }

        /// <summary>
        /// Handles dropping an object.
        /// Ported from vdrop() in VERB.C with liquid logic.
        /// </summary>
        private void HandleDrop(int objectId)
        {
            if (objectId == 0)
            {
                Console.WriteLine(GameMessages.GetMessage(43)); // "Where?"
                return;
            }

            // Special handling for water/oil - drop the bottle instead
            int liquid = LiquidManager.Liq(gameState);
            if (liquid == objectId)
            {
                objectId = GameConstants.Bottle;
            }

            if (!gameState.IsCarrying(objectId))
            {
                Console.WriteLine(GameMessages.GetMessage(29)); // "You aren't carrying it!"
                return;
            }

            // If dropping bottle with liquid, remove liquid from game
            if (objectId == GameConstants.Bottle && liquid != 0)
            {
                gameState.SetObjectLocation(liquid, 0);
            }

            gameState.Drop(objectId, gameState.Location);
            Console.WriteLine(GameMessages.GetMessage(54)); // "OK"
        }

        /// <summary>
        /// Handles filling the bottle.
        /// Ported from vfill() in VERB.C.
        /// </summary>
        private void HandleFill(int objectId)
        {
            // Default to bottle if no object specified
            if (objectId == 0)
            {
                objectId = GameConstants.Bottle;
            }

            if (objectId != GameConstants.Bottle)
            {
                Console.WriteLine(GameMessages.GetMessage(106)); // "You can't fill that."
                return;
            }

            if (LiquidManager.Liq(gameState) != 0)
            {
                Console.WriteLine(GameMessages.GetMessage(105)); // "Your bottle is already full."
                return;
            }

            int liquidHere = LiquidManager.LiqLoc(gameState, gameState.Location);
            if (liquidHere == 0)
            {
                Console.WriteLine(GameMessages.GetMessage(106)); // "There is nothing here with which to fill the bottle."
                return;
            }

            // Fill the bottle with the liquid at this location
            gameState.SetObjectProperty(GameConstants.Bottle, LiquidManager.GetBottlePropertyForLiquid(liquidHere));
            
            if (gameState.IsCarrying(GameConstants.Bottle))
            {
                gameState.SetObjectLocation(liquidHere, -1);
            }

            int messageId = (liquidHere == GameConstants.Oil) ? 108 : 107; // Oil or water message
            Console.WriteLine(GameMessages.GetMessage(messageId));
        }

        /// <summary>
        /// Handles pouring liquid from the bottle.
        /// Ported from vpour() in VERB.C.
        /// </summary>
        private void HandlePour(int objectId)
        {
            // Get what's in the bottle
            if (objectId == GameConstants.Bottle || objectId == 0)
            {
                objectId = LiquidManager.Liq(gameState);
            }

            if (objectId == 0 || !gameState.IsCarrying(objectId))
            {
                Console.WriteLine(GameMessages.GetMessage(104)); // "You aren't carrying it!"
                return;
            }

            // Special case: pouring on plant
            if (gameState.IsObjectHere(GameConstants.Plant) || gameState.IsObjectHere(GameConstants.Plant2))
            {
                if (objectId != GameConstants.Water)
                {
                    Console.WriteLine(GameMessages.GetMessage(112)); // "The plant indignantly shakes the oil off its leaves and asks, 'Water?'"
                }
                else
                {
                    // Water the plant (causes it to grow)
                    int plantId = gameState.IsObjectHere(GameConstants.Plant) ? GameConstants.Plant : GameConstants.Plant2;
                    int currentProp = gameState.ObjectProperties[plantId];
                    Console.WriteLine(GameMessages.GetMessage(112 + currentProp)); // Plant growth messages
                    
                    gameState.SetObjectProperty(GameConstants.Plant, (currentProp + 2) % 6);
                    gameState.SetObjectProperty(GameConstants.Plant2, gameState.ObjectProperties[GameConstants.Plant] / 2);
                }
            }
            // Special case: pouring on door
            else if (gameState.IsObjectHere(GameConstants.Door))
            {
                gameState.SetObjectProperty(GameConstants.Door, objectId == GameConstants.Oil ? 1 : 0);
                Console.WriteLine(GameMessages.GetMessage(113 + gameState.ObjectProperties[GameConstants.Door]));
            }
            else
            {
                Console.WriteLine(GameMessages.GetMessage(78)); // "The bottle is now empty."
            }

            // Empty the bottle and remove liquid from game
            gameState.SetObjectProperty(GameConstants.Bottle, 1);
            gameState.SetObjectLocation(objectId, 0);
        }

        /// <summary>
        /// Handles drinking liquid.
        /// Ported from vdrink() in VERB.C.
        /// </summary>
        private void HandleDrink(int objectId)
        {
            // Default to water if no object specified
            if (objectId == 0)
            {
                objectId = GameConstants.Water;
            }

            if (objectId != GameConstants.Water)
            {
                Console.WriteLine(GameMessages.GetMessage(110)); // "Drink what?"
                return;
            }

            if (LiquidManager.Liq(gameState) != GameConstants.Water || !gameState.IsObjectHere(GameConstants.Bottle))
            {
                Console.WriteLine(GameMessages.GetMessage(104)); // "You aren't carrying it!"
                return;
            }

            // Drink the water
            gameState.SetObjectProperty(GameConstants.Bottle, 1);
            gameState.SetObjectLocation(GameConstants.Water, 0);
            Console.WriteLine(GameMessages.GetMessage(74)); // "The bottle of water is now empty."
        }

        /// <summary>
        /// Handles turning the lamp on.
        /// Ported from von() in VERB.C.
        /// </summary>
        private void HandleLampOn()
        {
            if (!gameState.IsObjectHere(GameConstants.Lamp))
            {
                Console.WriteLine(GameMessages.GetMessage(28)); // "I see no lamp here."
                return;
            }

            if (gameState.Limit < 0)
            {
                Console.WriteLine(GameMessages.GetMessage(184)); // "Your lamp has run out of power."
                return;
            }

            gameState.SetObjectProperty(GameConstants.Lamp, 1);
            Console.WriteLine(GameMessages.GetMessage(39)); // "Your lamp is now on."

            // If location was dark, show the description
            if (gameState.WizardDark)
            {
                gameState.WizardDark = false;
                ShowLocationDescription();
            }
        }

        /// <summary>
        /// Handles turning the lamp off.
        /// Ported from voff() in VERB.C.
        /// </summary>
        private void HandleLampOff()
        {
            if (!gameState.IsObjectHere(GameConstants.Lamp))
            {
                Console.WriteLine(GameMessages.GetMessage(28)); // "I see no lamp here."
                return;
            }

            gameState.SetObjectProperty(GameConstants.Lamp, 0);
            Console.WriteLine(GameMessages.GetMessage(40)); // "Your lamp is now off."
        }

        /// <summary>
        /// Handles quitting the game.
        /// </summary>
        private void HandleQuit()
        {
            Console.WriteLine(GameMessages.GetMessage(22)); // "Do you really want to quit now?"
            string response = Console.ReadLine() ?? string.Empty;
            
            if (IsYesResponse(response))
            {
                gameState.SaveFlag = true;
            }
        }

        /// <summary>
        /// Shows the player's inventory.
        /// </summary>
        private void ShowInventory()
        {
            List<int> carriedObjects = gameState.GetCarriedObjects();
            
            if (carriedObjects.Count == 0)
            {
                Console.WriteLine("You are currently empty-handed.");
                return;
            }

            Console.WriteLine("You are currently carrying:");
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
            // Check if location is dark
            if (DarknessManager.IsDark(gameState))
            {
                Console.WriteLine(GameMessages.GetMessage(16)); // "It is now pitch dark. If you proceed you will likely fall into a pit."
                return;
            }

            bool showLong = forceLong || gameState.VisitedLocations[gameState.Location] == 0;
            
            if (showLong && LocationDescriptions.LongDescriptions.TryGetValue(gameState.Location, out string? longDesc))
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

            gameState.VisitedLocations[gameState.Location] = 1;

            // Show objects at this location
            ShowObjectsHere();
        }

        /// <summary>
        /// Shows objects present at the current location.
        /// </summary>
        private void ShowObjectsHere()
        {
            List<int> objectsHere = gameState.GetObjectsHere();
            
            foreach (int objectId in objectsHere)
            {
                if (!gameState.IsCarrying(objectId) && 
                    GameObjects.Objects.TryGetValue(objectId, out GameObjectData? objectData))
                {
                    if (objectData.States.Count > 0)
                    {
                        int stateIndex = Math.Min(gameState.ObjectProperties[objectId], objectData.States.Count - 1);
                        string description = objectData.States[stateIndex].RoomDescription;
                        if (!string.IsNullOrEmpty(description))
                        {
                            Console.WriteLine(description);
                        }
                    }
                }
            }
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
            // TODO: Implement full death mechanics with resurrection
            // For now, just end the game
            gameState.SaveFlag = true;
            gameState.NumDie++;
        }
    }
