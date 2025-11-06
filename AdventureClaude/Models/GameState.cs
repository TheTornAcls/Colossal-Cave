using System.Collections.Generic;

namespace AdventureClaude.Models;

/// <summary>
/// Represents the complete game state for the adventure.
/// Converted from the C global variables and structs in ADVDEC.H.
/// </summary>
public class GameState
    {
        // Location-related variables
        public int Turns { get; set; } = 0;
        public int Location { get; set; } = GameConstants.StartLocation;
        public int OldLocation { get; set; } = GameConstants.StartLocation;
        public int OldLocation2 { get; set; } = GameConstants.StartLocation;
        public int NewLocation { get; set; } = GameConstants.StartLocation;

        // Arrays for location and object status
        public short[] LocationConditions { get; set; } = new short[GameConstants.MaxLocations];
        public int[] ObjectLocations { get; set; } = new int[GameConstants.MaxObjects];
        public int[] FixedObjectLocations { get; set; } = new int[GameConstants.MaxObjects];
        public short[] VisitedLocations { get; set; } = new short[GameConstants.MaxLocations];
        public short[] ObjectProperties { get; set; } = new short[GameConstants.MaxObjects];

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
        public int[] DwarfLocations { get; set; } = new int[GameConstants.MaxDwarves];
        public int[] OldDwarfLocations { get; set; } = new int[GameConstants.MaxDwarves];
        public int DwarfKill { get; set; } = 0;
        public bool[] DwarfSeen { get; set; } = new bool[GameConstants.MaxDwarves];

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
        public int[] HintLocations { get; set; } = new int[GameConstants.MaxLocations];
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
            // Initialize arrays
            LocationConditions = new short[GameConstants.MaxLocations];
            ObjectLocations = new int[GameConstants.MaxObjects];
            FixedObjectLocations = new int[GameConstants.MaxObjects];
            VisitedLocations = new short[GameConstants.MaxLocations];
            ObjectProperties = new short[GameConstants.MaxObjects];
            DwarfLocations = new int[GameConstants.MaxDwarves];
            OldDwarfLocations = new int[GameConstants.MaxDwarves];
            DwarfSeen = new bool[GameConstants.MaxDwarves];
            HintLocations = new int[GameConstants.MaxLocations];
            ActionMessages = new short[GameConstants.ActionMessageSize];

            // Set default values
            Turns = 0;
            Location = GameConstants.StartLocation;
            OldLocation = GameConstants.StartLocation;
            OldLocation2 = GameConstants.StartLocation;
            NewLocation = GameConstants.StartLocation;
            
            WizardDark = false;
            Closed = false;
            Closing = false;
            Holding = 0;
            Detail = 0;
            Limit = 330;
            Tally = 0;
            Tally2 = 0;
            
            KnifeLocation = 0;
            ChestLocation = 0;
            ChestLocation2 = 0;
            
            DwarfKill = 0;
            Clock1 = 0;
            Clock2 = 0;
            Panic = false;
            Bonus = 0;
            NumDie = 0;
            DwarfAlternateLocation = 0;
            LampWarning = 0;
            FooBar = 0;
            
            DwarfFlag = 0;
            GaveUp = false;
            SaveFlag = false;
            HintTaken = 0;
            HintAvailable = 0;
            TestBr = 0;
            
            Verb = 0;
            Object = 0;
            Motion = 0;
            Word1 = string.Empty;
            Word2 = string.Empty;
            
            DebugFlag = 0;

            // Initialize object locations
            InitializeObjectLocations();
        }

        /// <summary>
        /// Sets up initial object locations.
        /// This would be populated from the original cave data.
        /// </summary>
        private void InitializeObjectLocations()
        {
            // Set initial object locations based on the original adventure
            ObjectLocations[GameConstants.Keys] = 3;      // Keys in well house
            ObjectLocations[GameConstants.Lamp] = 3;      // Lamp in well house
            ObjectLocations[GameConstants.Food] = 3;      // Food in well house
            ObjectLocations[GameConstants.Bottle] = 3;    // Bottle in well house
            
            // Set fixed objects
            FixedObjectLocations[GameConstants.Grate] = 8;    // Grate at location 8
            FixedObjectLocations[GameConstants.Steps] = 8;    // Steps at location 8
            
            // Initialize other objects as needed
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
            for (int i = 1; i < GameConstants.MaxObjects; i++)
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
            for (int i = 1; i < GameConstants.MaxObjects; i++)
            {
                if (IsObjectHere(i))
                {
                    objectsHere.Add(i);
                }
            }
            return objectsHere;
        }
    }