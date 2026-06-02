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
            if (!inputParser.ParseInput(input, out int verb, out int objectId, out int motion))
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
                HandleMotion(gameState.Motion);
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
        /// Handles movement commands using the travel table system.
        /// Includes support for forced movement and special destinations.
        /// </summary>
        private void HandleMotion(int motion)
        {
            // Get available travel options for this verb/direction
            List<TravelEntry> availableOptions = gameState.GetAvailableTravelOptions(motion, random);

            if (availableOptions.Count == 0)
            {
                Console.WriteLine(GameMessages.GetMessage(9)); // "There is no way to go that direction."
                return;
            }

            // Use the first available option (conditions already evaluated)
            TravelEntry selectedOption = availableOptions[0];
            int destination = selectedOption.Destination;

            // Handle special destinations
            if (destination >= 500)
            {
                // Destination 500+ means print message and stay in place
                int messageId = destination - 500;
                if (messageId >= 1 && messageId <= GameConstants.MaxMessages)
                {
                    Console.WriteLine(GameMessages.GetMessage(messageId));
                }
                return;
            }

            if (destination >= 300)
            {
                // Special movement handlers (plover, troll bridge, etc.)
                destination = HandleSpecialDestination(destination);
                if (destination == 0)
                {
                    return; // Special handler aborted movement
                }
            }

            // Perform the movement
            if (destination > 0 && destination <= GameConstants.MaxLocations)
            {
                PerformMove(destination);
            }
            else
            {
                Console.WriteLine(GameMessages.GetMessage(9)); // "There is no way to go that direction."
            }
        }

        /// <summary>
        /// Handles special destination codes (301-303).
        /// Ported from spcmove() in TURN.C.
        /// </summary>
        /// <param name="specialCode">Special destination code (301-303)</param>
        /// <returns>Actual destination location, or 0 to abort movement</returns>
        private int HandleSpecialDestination(int specialCode)
        {
            switch (specialCode)
            {
                case 301:
                    // Plover alcove teleportation
                    // TODO: Implement full plover logic (emerald check, etc.)
                    // For MVP: treat as regular movement
                    Console.WriteLine("[Special: Plover alcove]");
                    return specialCode % 300;

                case 302:
                    // Plover removal (bad route)
                    // TODO: Implement emerald drop logic
                    Console.WriteLine("[Special: Plover removal]");
                    return specialCode % 300;

                case 303:
                    // Troll bridge
                    // TODO: Implement full troll bridge logic
                    Console.WriteLine("[Special: Troll bridge]");
                    return specialCode % 300;

                default:
                    // Unknown special code, treat as regular movement
                    return specialCode % 300;
            }
        }

        /// <summary>
        /// Performs the actual movement to a destination, handling forced movement cascades.
        /// </summary>
        /// <param name="destination">Destination location ID</param>
        private void PerformMove(int destination)
        {
            const int MaxForcedMoves = 10; // Safety limit to prevent infinite loops
            int forcedMoveCount = 0;

            while (forcedMoveCount < MaxForcedMoves)
            {
                // Update location tracking
                gameState.OldLocation2 = gameState.OldLocation;
                gameState.OldLocation = gameState.Location;
                gameState.Location = destination;
                gameState.NewLocation = destination;

                // Check for falling into pit in darkness
                if (DarknessManager.CheckDarknessDanger(gameState, random))
                {
                    Console.WriteLine(GameMessages.GetMessage(23)); // "You fell into a pit and broke every bone in your body!"
                    HandleDeath();
                    return;
                }

                // Show location description
                ShowLocationDescription();

                // Update darkness state for next move
                DarknessManager.UpdateDarknessState(gameState);

                // Check if this location forces automatic movement
                if (!DarknessManager.IsForced(gameState, gameState.Location))
                {
                    // Not forced, movement complete
                    return;
                }

                // Location is forced - automatically move to next location
                forcedMoveCount++;

                // Get forced travel option (usually the first/only option)
                List<TravelEntry> forcedOptions = gameState.GetAvailableTravelOptions(1, random);
                if (forcedOptions.Count == 0)
                {
                    Console.WriteLine("[Warning: Forced location has no travel options]");
                    return;
                }

                // Use first available forced movement option
                destination = forcedOptions[0].Destination;

                // Handle special destinations in forced movement
                if (destination >= 500)
                {
                    int messageId = destination - 500;
                    if (messageId >= 1 && messageId <= GameConstants.MaxMessages)
                    {
                        Console.WriteLine(GameMessages.GetMessage(messageId));
                    }
                    return;
                }

                if (destination >= 300)
                {
                    destination = HandleSpecialDestination(destination);
                    if (destination == 0)
                    {
                        return;
                    }
                }

                // Continue loop to move to forced destination
            }

            // Safety limit reached
            Console.WriteLine("[Warning: Maximum forced movement cascade depth reached]");
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

            gameState.ObjectLocations[objectId] = -1; // -1 means carried
            gameState.Holding++;
            
            // If taking bottle with liquid, mark liquid as being carried
            if (objectId == GameConstants.Bottle)
            {
                int liquid = LiquidManager.Liq(gameState);
                if (liquid != 0)
                {
                    gameState.ObjectLocations[liquid] = -1;
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
                gameState.ObjectLocations[liquid] = 0; // Remove from game
            }

            gameState.ObjectLocations[objectId] = gameState.Location;
            gameState.Holding--;
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
            gameState.ObjectProperties[GameConstants.Bottle] = (short)LiquidManager.GetBottlePropertyForLiquid(liquidHere);
            
            if (gameState.IsCarrying(GameConstants.Bottle))
            {
                gameState.ObjectLocations[liquidHere] = -1; // Mark liquid as carried
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
                    
                    gameState.ObjectProperties[GameConstants.Plant] = (short)((currentProp + 2) % 6);
                    gameState.ObjectProperties[GameConstants.Plant2] = (short)(gameState.ObjectProperties[GameConstants.Plant] / 2);
                }
            }
            // Special case: pouring on door
            else if (gameState.IsObjectHere(GameConstants.Door))
            {
                gameState.ObjectProperties[GameConstants.Door] = (short)((objectId == GameConstants.Oil) ? 1 : 0);
                Console.WriteLine(GameMessages.GetMessage(113 + gameState.ObjectProperties[GameConstants.Door]));
            }
            else
            {
                Console.WriteLine(GameMessages.GetMessage(78)); // "The bottle is now empty."
            }

            // Empty the bottle and remove liquid from game
            gameState.ObjectProperties[GameConstants.Bottle] = 1; // Empty
            gameState.ObjectLocations[objectId] = 0; // Remove from game
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
            gameState.ObjectProperties[GameConstants.Bottle] = 1; // Empty bottle
            gameState.ObjectLocations[GameConstants.Water] = 0; // Remove water from game
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

            gameState.ObjectProperties[GameConstants.Lamp] = 1; // Turn lamp on
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

            gameState.ObjectProperties[GameConstants.Lamp] = 0; // Turn lamp off
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