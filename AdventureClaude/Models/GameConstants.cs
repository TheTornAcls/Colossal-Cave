namespace AdventureClaude.Models;

/// <summary>
/// Represents the travel options from a location.
/// Converted from the C struct trav.
/// </summary>
public class TravelOption
    {
        public int Destination { get; set; } = -1;  // tdest
        public int Verb { get; set; } = 0;          // tverb
        public int Condition { get; set; } = 0;     // tcond
    }

    /// <summary>
    /// Contains constants for the adventure game.
    /// Converted from ADVENT.H defines.
    /// </summary>
    public static class GameConstants
    {
        public const int MaxObjects = 100;
        public const int MaxLocations = 140;
        public const int MaxWordSize = 20;
        public const int MaxMessages = 209;
        public const int ActionMessageSize = 33;
        public const int MaxTravel = 17;    // 16 + 1 for terminator
        public const int MaxDwarves = 7;
        public const int MaxDeaths = 3;
        public const int MaxTreasures = 79;

        // Object IDs
        public const int Keys = 1;
        public const int Lamp = 2;
        public const int Grate = 3;
        public const int Cage = 4;
        public const int Rod = 5;
        public const int Rod2 = 6;
        public const int Steps = 7;
        public const int Bird = 8;
        public const int Door = 9;
        public const int Pillow = 10;
        public const int Snake = 11;
        public const int Fissure = 12;
        public const int Tablet = 13;
        public const int Clam = 14;
        public const int Oyster = 15;
        public const int Magazine = 16;
        public const int Dwarf = 17;
        public const int Knife = 18;
        public const int Food = 19;
        public const int Bottle = 20;
        public const int Water = 21;
        public const int Oil = 22;
        public const int Mirror = 23;
        public const int Plant = 24;
        public const int Plant2 = 25;
        public const int Axe = 28;
        public const int Dragon = 31;
        public const int Chasm = 32;
        public const int Troll = 33;
        public const int Troll2 = 34;
        public const int Bear = 35;

        // Special location values
        public const int StartLocation = 1;
        public const int WellHouse = 3;
        public const int EndOfRoad = 1;

        // Verb constants (action verbs from ADVENT.H)
        public const int Take = 1;
        public const int Drop = 2;
        public const int Say = 3;
        public const int Open = 4;
        public const int Lock = 6;
        public const int On = 7;
        public const int Off = 8;
        public const int Wave = 9;
        public const int Kill = 12;
        public const int Pour = 13;
        public const int Eat = 14;
        public const int Drink = 15;
        public const int Quit = 18;
        public const int Inventory = 20;
        public const int Fill = 22;
        public const int Look = 57;

        // Location condition flags (bit mapping of cond array from ADVENT.H)
        public const int Light = 1;      // Location has light
        public const int WatOil = 2;     // Determines liquid type (0=water, 1=oil)
        public const int Liquid = 4;     // Location has liquid available
        public const int NoPirat = 8;    // Pirate cannot access
        public const int Forced = 16;    // Forced movement location
    }