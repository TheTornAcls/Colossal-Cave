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
                // Special movement handlers (forced movement, etc.)
                // For now, treat as regular movement to destination % 300
                destination = destination % 300;
            }

            // Perform the movement
            if (destination > 0 && destination <= GameConstants.MaxLocations)
            {
                gameState.OldLocation2 = gameState.OldLocation;
                gameState.OldLocation = gameState.Location;
                gameState.Location = destination;
                gameState.NewLocation = destination;
                ShowLocationDescription();
            }
            else
            {
                Console.WriteLine(GameMessages.GetMessage(9)); // "There is no way to go that direction."
            }
        }

        /// <summary>
        /// Handles verb commands.
        /// </summary>
        private void HandleVerb(int verb, int objectId)
        {
            switch (verb)
            {
                case 20: // Inventory
                    ShowInventory();
                    break;
                case 57: // Look
                    ShowLocationDescription(true);
                    break;
                case 1: // Take/Get
                    HandleTake(objectId);
                    break;
                case 2: // Drop/Put
                    HandleDrop(objectId);
                    break;
                case 18: // Quit
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
        /// </summary>
        private void HandleTake(int objectId)
        {
            if (objectId == 0)
            {
                Console.WriteLine(GameMessages.GetMessage(43)); // "Where?"
                return;
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
            Console.WriteLine(GameMessages.GetMessage(54)); // "OK"
        }

        /// <summary>
        /// Handles dropping an object.
        /// </summary>
        private void HandleDrop(int objectId)
        {
            if (objectId == 0)
            {
                Console.WriteLine(GameMessages.GetMessage(43)); // "Where?"
                return;
            }

            if (!gameState.IsCarrying(objectId))
            {
                Console.WriteLine(GameMessages.GetMessage(29)); // "You aren't carrying it!"
                return;
            }

            gameState.ObjectLocations[objectId] = gameState.Location;
            gameState.Holding--;
            Console.WriteLine(GameMessages.GetMessage(54)); // "OK"
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
    }