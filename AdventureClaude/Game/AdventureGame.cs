using System;
using System.Collections.Generic;
using AdventureClaude.Data;
using AdventureClaude.Models;

namespace AdventureClaude.Game;

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
        /// Handles movement commands.
        /// </summary>
        private void HandleMotion(int motion)
        {
            // Simple movement implementation
            // In the full version, this would check travel tables
            
            switch (motion)
            {
                case 1: // North
                    AttemptMove("north");
                    break;
                case 2: // South
                    AttemptMove("south");
                    break;
                case 3: // East
                    AttemptMove("east");
                    break;
                case 4: // West
                    AttemptMove("west");
                    break;
                case 5: // Up
                    AttemptMove("up");
                    break;
                case 6: // Down
                    AttemptMove("down");
                    break;
                default:
                    Console.WriteLine(GameMessages.GetMessage(9)); // "There is no way to go that direction."
                    break;
            }
        }

        /// <summary>
        /// Attempts to move in a given direction.
        /// This is a simplified implementation - the full version would use travel tables.
        /// </summary>
        private void AttemptMove(string direction)
        {
            // Simplified movement logic
            int newLocation = gameState.Location;

            // Basic movement from starting location
            switch (gameState.Location)
            {
                case 1: // End of road
                    switch (direction)
                    {
                        case "west":
                            newLocation = 3; // Into building
                            break;
                        case "up":
                        case "north":
                            newLocation = 2; // Up hill
                            break;
                        case "south":
                            newLocation = 4; // Into valley
                            break;
                        case "east":
                            newLocation = 5; // Into forest
                            break;
                    }
                    break;
                    
                case 2: // Hill
                    switch (direction)
                    {
                        case "south":
                        case "down":
                            newLocation = 1; // Back to road
                            break;
                    }
                    break;
                    
                case 3: // Well house
                    switch (direction)
                    {
                        case "east":
                            newLocation = 1; // Back to road
                            break;
                        case "south":
                            if (gameState.IsCarrying(GameConstants.Keys) && gameState.IsObjectHere(GameConstants.Grate))
                            {
                                // Check if grate is unlocked
                                newLocation = 8; // To grate location
                            }
                            else
                            {
                                Console.WriteLine("The grate is locked and you have no keys.");
                                return;
                            }
                            break;
                    }
                    break;
                    
                case 4: // Valley
                    switch (direction)
                    {
                        case "north":
                            newLocation = 1; // Back to road
                            break;
                        case "west":
                            newLocation = 3; // To building
                            break;
                        case "east":
                            newLocation = 5; // To forest
                            break;
                    }
                    break;
                    
                case 5: // Forest
                    switch (direction)
                    {
                        case "west":
                            newLocation = 1; // Back to road
                            break;
                        case "south":
                            newLocation = 4; // To valley
                            break;
                    }
                    break;
                    
                case 8: // Outside grate
                    switch (direction)
                    {
                        case "north":
                            newLocation = 1; // Back to road
                            break;
                        case "down":
                            if (gameState.IsObjectHere(GameConstants.Grate) && gameState.ObjectProperties[GameConstants.Grate] == 1)
                            {
                                newLocation = 9; // Below grate
                            }
                            else
                            {
                                Console.WriteLine("You can't go through a locked grate!");
                                return;
                            }
                            break;
                    }
                    break;
                    
                case 9: // Below grate
                    switch (direction)
                    {
                        case "up":
                            newLocation = 8; // Back to grate
                            break;
                        case "west":
                            newLocation = 10; // Cobble crawl
                            break;
                    }
                    break;
            }

            if (newLocation != gameState.Location)
            {
                gameState.OldLocation2 = gameState.OldLocation;
                gameState.OldLocation = gameState.Location;
                gameState.Location = newLocation;
                gameState.NewLocation = newLocation;
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