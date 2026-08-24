namespace AdventureClaude.Models;

using System.Collections.Generic;

/// <summary>
/// Represents the complete game state for the adventure.
/// Converted from the C global variables and structs in ADVDEC.H.
/// </summary>
public class GameState
    {
        private static readonly short[] InitialLocationConditions =
        [
            0, 2053, 2049, 2053, 2053, 2049, 2049, 2053, 33, 1,
            1, 0, 0, 64, 0, 0, 16, 0, 0, 128,
            16, 16, 16, 0, 6, 0, 16, 0, 0, 0,
            0, 16, 16, 0, 0, 0, 0, 0, 4, 0,
            16, 0, 256, 256, 256, 256, 264, 264, 264, 256,
            256, 256, 256, 264, 256, 264, 0, 8, 0, 16,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
            256, 256, 264, 0, 0, 8, 264, 256, 0, 16,
            16, 0, 0, 0, 0, 4, 0, 0, 0, 512,
            513, 0, 0, 0, 0, 0, 0, 0, 1024, 0,
            0, 0, 0, 4, 0, 1, 1, 0, 0, 0,
            0, 0, 8, 8, 8, 8, 8, 8, 8, 8,
            8, 0, 0, 0, 0, 0, 0, 0, 0, 0
        ];

        private static readonly int[] InitialObjectLocations =
        [
            0, 3, 3, 8, 10, 11, 0, 14, 13, 94,
            96, 19, 17, 101, 103, 0, 106, 0, 0, 3,
            3, 0, 0, 109, 25, 23, 111, 35, 0, 97,
            0, 119, 117, 117, 0, 130, 0, 126, 140, 0,
            96, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            18, 27, 28, 29, 30, 0, 92, 95, 97, 100,
            101, 0, 119, 127, 130, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        ];

        private static readonly int[] InitialFixedObjectLocations =
        [
            0, 0, 0, 9, 0, 0, 0, 15, 0, -1,
            0, -1, 27, -1, 0, 0, 0, -1, 0, 0,
            0, 0, 0, -1, -1, 67, -1, 110, 0, -1,
            -1, 121, 122, 122, 0, -1, -1, -1, -1, 0,
            -1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 121, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        ];

        private static readonly short[] InitialActionMessages =
        [
            0, 24, 29, 0, 33, 0, 33, 38, 38, 42,
            14, 43, 110, 29, 110, 73, 75, 29, 13, 59,
            59, 174, 109, 67, 13, 0, 90, 195, 146, 110,
            13, 13, 155
        ];

        private static readonly int[] InitialDwarfLocations = [0, 19, 27, 33, 44, 64, 114];

        // Location-related variables
        public int Turns { get; set; } = 0;
        public int Location { get; set; } = GameConstants.StartLocation;
        public int OldLocation { get; set; } = GameConstants.StartLocation;
        public int OldLocation2 { get; set; } = GameConstants.StartLocation;
        public int NewLocation { get; set; } = GameConstants.StartLocation;

        // Arrays for location and object status
        public short[] LocationConditions { get; set; } = new short[GameConstants.LocationArraySize];
        public int[] ObjectLocations { get; set; } = new int[GameConstants.ObjectArraySize];
        public int[] FixedObjectLocations { get; set; } = new int[GameConstants.ObjectArraySize];
        public short[] VisitedLocations { get; set; } = new short[GameConstants.LocationArraySize];
        public short[] ObjectProperties { get; set; } = new short[GameConstants.ObjectArraySize];

        // Game progress variables
        public int Tally { get; set; } = 0;
        public int Tally2 { get; set; } = 0;
        public bool WizardDark { get; set; } = false;
        public bool Closed { get; set; } = false;
        public bool Closing { get; set; } = false;
        public int Holding { get; set; } = 0;
        public int Detail { get; set; } = 0;
        public int Limit { get; set; } = 330;
        public int KnifeLocation { get; set; } = 0;
        public int ChestLocation { get; set; } = 0;
        public int ChestLocation2 { get; set; } = 0;

        // Dwarf-related variables
        public int[] DwarfLocations { get; set; } = new int[GameConstants.DwarfArraySize];
        public int[] OldDwarfLocations { get; set; } = new int[GameConstants.DwarfArraySize];
        public int DwarfKill { get; set; } = 0;
        public bool[] DwarfSeen { get; set; } = new bool[GameConstants.DwarfArraySize];

        // Time and event variables
        public int Clock1 { get; set; } = 0;
        public int Clock2 { get; set; } = 0;
        public bool Panic { get; set; } = false;
        public int Bonus { get; set; } = 0;
        public int NumDie { get; set; } = 0;
        public int DwarfAlternateLocation { get; set; } = 0;
        public int LampWarning { get; set; } = 0;
        public int FooBar { get; set; } = 0;

        // Game flags
        public int DwarfFlag { get; set; } = 0;
        public bool GaveUp { get; set; } = false;
        public bool SaveFlag { get; set; } = false;
        public int HintTaken { get; set; } = 0;
        public int HintAvailable { get; set; } = 0;
        public int[] HintLocations { get; set; } = new int[GameConstants.HintArraySize];
        public int TestBr { get; set; } = 0;

        // Action messages
        public short[] ActionMessages { get; set; } = new short[GameConstants.ActionMessageSize];

        // Current travel options
        public List<TravelOption> CurrentTravelOptions { get; set; } = new List<TravelOption>();

        // English parsing variables
        public int Verb { get; set; } = 0;
        public int Object { get; set; } = 0;
        public int Motion { get; set; } = 0;
        public string Word1 { get; set; } = string.Empty;
        public string Word2 { get; set; } = string.Empty;

        // Debug flag
        public int DebugFlag { get; set; } = 0;

        /// <summary>
        /// Initializes the game state to starting values.
        /// Equivalent to the initplay() function in the C version.
        /// </summary>
        public void InitializeGame()
        {
            LocationConditions = CopyLocationConditions();
            ObjectLocations = CopyObjectLocations(InitialObjectLocations);
            FixedObjectLocations = CopyObjectLocations(InitialFixedObjectLocations);
            VisitedLocations = new short[GameConstants.LocationArraySize];
            ObjectProperties = new short[GameConstants.ObjectArraySize];
            for (int i = GameConstants.Nugget; i < GameConstants.MaxObjects; i++)
            {
                ObjectProperties[i] = -1;
            }

            DwarfLocations = CopyDwarfLocations();
            OldDwarfLocations = new int[GameConstants.DwarfArraySize];
            DwarfSeen = new bool[GameConstants.DwarfArraySize];
            HintLocations = new int[GameConstants.HintArraySize];
            ActionMessages = CopyActionMessages();

            // Set default values
            Turns = 0;
            NewLocation = 1;
            Location = 2;
            OldLocation = 2;
            OldLocation2 = 2;
            
            WizardDark = false;
            Closed = false;
            Closing = false;
            Holding = 0;
            Detail = 0;
            Limit = 100;
            Tally = 15;
            Tally2 = 0;
            
            KnifeLocation = 0;
            ChestLocation = 114;
            ChestLocation2 = 140;
            
            DwarfKill = 0;
            Clock1 = 30;
            Clock2 = 50;
            Panic = false;
            Bonus = 0;
            NumDie = 0;
            DwarfAlternateLocation = 18;
            LampWarning = 0;
            FooBar = 0;
            
            DwarfFlag = 0;
            GaveUp = false;
            SaveFlag = false;
            HintTaken = 0;
            HintAvailable = GameConstants.Hint;
            TestBr = 2;
            
            Verb = 0;
            Object = 0;
            Motion = 0;
            Word1 = string.Empty;
            Word2 = string.Empty;
            
            DebugFlag = 0;
        }

        private static short[] CopyLocationConditions()
        {
            short[] result = new short[GameConstants.LocationArraySize];
            Array.Copy(InitialLocationConditions, result, InitialLocationConditions.Length);
            return result;
        }

        private static int[] CopyObjectLocations(int[] source)
        {
            int[] result = new int[GameConstants.ObjectArraySize];
            Array.Copy(source, result, source.Length);
            return result;
        }

        private static short[] CopyActionMessages()
        {
            short[] result = new short[GameConstants.ActionMessageSize];
            Array.Copy(InitialActionMessages, result, InitialActionMessages.Length);
            return result;
        }

        private static int[] CopyDwarfLocations()
        {
            int[] result = new int[GameConstants.DwarfArraySize];
            Array.Copy(InitialDwarfLocations, result, InitialDwarfLocations.Length);
            return result;
        }

        /// <summary>
        /// Checks if the player is carrying a specific object.
        /// </summary>
        /// <param name="objectId">The ID of the object to check</param>
        /// <returns>True if the player is carrying the object</returns>
        public bool IsCarrying(int objectId)
        {
            return ObjectLocations[objectId] == -1;
        }

        /// <summary>
        /// Checks if an object is at the player's current location.
        /// </summary>
        /// <param name="objectId">The ID of the object to check</param>
        /// <returns>True if the object is at the current location</returns>
        public bool IsObjectHere(int objectId)
        {
            return ObjectLocations[objectId] == Location || 
                   FixedObjectLocations[objectId] == Location;
        }

        /// <summary>
        /// Gets a list of all objects the player is currently carrying.
        /// </summary>
        /// <returns>List of object IDs being carried</returns>
        public List<int> GetCarriedObjects()
        {
            List<int> carriedObjects = new ();
            for (int i = 1; i <= GameConstants.MaxObjects; i++)
            {
                if (IsCarrying(i))
                {
                    carriedObjects.Add(i);
                }
            }
            return carriedObjects;
        }

        /// <summary>
        /// Gets a list of all objects at the current location.
        /// </summary>
        /// <returns>List of object IDs at the current location</returns>
        public List<int> GetObjectsHere()
        {
            List<int> objectsHere = new ();
            for (int i = 1; i <= GameConstants.MaxObjects; i++)
            {
                if (IsObjectHere(i))
                {
                    objectsHere.Add(i);
                }
            }
            return objectsHere;
        }

        /// <summary>
        /// Gets available travel options from the current location,
        /// filtering out options whose conditions are not met.
        /// </summary>
        /// <param name="verb">The verb/direction being attempted</param>
        /// <param name="random">Random number generator for probability checks</param>
        /// <returns>List of valid travel entries</returns>
        public List<TravelEntry> GetAvailableTravelOptions(int verb, Random random)
        {
            var allOptions = AdventureClaude.Data.TravelData.GetTravelOptions(Location);
            var availableOptions = new List<TravelEntry>();

            foreach (var option in allOptions)
            {
                // Check if this option matches the requested verb (or verb 0 = any)
                if (option.Verb == verb || option.Verb == 0)
                {
                    if (EvaluateCondition(option, random))
                    {
                        availableOptions.Add(option);
                    }
                }
            }

            return availableOptions;
        }

        /// <summary>
        /// Evaluates whether a travel entry's condition is satisfied.
        /// Based on TURN.C condition evaluation logic.
        /// </summary>
        /// <param name="entry">The travel entry to evaluate</param>
        /// <param name="random">Random number generator for probability checks</param>
        /// <returns>True if the condition is met</returns>
        public bool EvaluateCondition(TravelEntry entry, Random random)
        {
            int condition = entry.Condition;
            
            // Condition 0 = always allowed
            if (condition == 0)
                return true;

            // Condition 1-99 = probability check (percentage)
            if (condition < 100)
            {
                int roll = random.Next(100); // 0-99
                return roll < condition;
            }

            // Condition 100+ = object-based checks
            int conditionType = entry.GetConditionType();
            int objectId = entry.GetObjectId();

            switch (conditionType)
            {
                case 0: // Should never happen for condition >= 100
                    return true;

                case 1: // Must be carrying object (or objectId=0 means always)
                    return objectId == 0 || IsCarrying(objectId);

                case 2: // Object must be present (carried or at location)
                    return IsCarrying(objectId) || IsObjectHere(objectId);

                case 3: // Object property must NOT be 0
                    return ObjectProperties[objectId] != 0;

                case 4: // Object property must NOT be 1
                    return ObjectProperties[objectId] != 1;

                case 5: // Object property must NOT be 2
                    return ObjectProperties[objectId] != 2;

                case 7: // Object property must NOT be 4
                    return ObjectProperties[objectId] != 4;

                default:
                    return false;
            }
        }
    }
