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
        public const int MaxName = 256;
        public const int MaxObjects = 100;
        public const int MaxWords = 295;
        public const int MaxLocations = 140;
        public const int MaxWordSize = 20;
        public const int MaxMessages = 209;
        public const int ActionMessageSize = 33;
        public const int MaxTravel = 17;
        public const int MaxDwarves = 7;
        public const int MaxDeaths = 3;
        public const int MaxTreasures = 79;

        // Array lengths include a spare slot because the original data is addressed with 1-based ids.
        public const int ObjectArraySize = MaxObjects + 1;
        public const int LocationArraySize = MaxLocations + 1;
        public const int DwarfArraySize = MaxDwarves;
        public const int HintArraySize = MaxHint + 1;

        // Object IDs from ADVENT.H.
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
        public const int Message = 36;
        public const int Vend = 38;
        public const int Batteries = 39;
        public const int Nugget = 50;
        public const int Coins = 54;
        public const int Chest = 55;
        public const int Eggs = 56;
        public const int Trident = 57;
        public const int Vase = 58;
        public const int Emerald = 59;
        public const int Pyramid = 60;
        public const int Pearl = 61;
        public const int Rug = 62;
        public const int Spices = 63;
        public const int Chain = 64;

        // Motion values from ADVENT.H.
        public const int NullMotion = 21;
        public const int Back = 8;
        public const int Look = 57;
        public const int Cave = 67;
        public const int Entrance = 64;
        public const int Depression = 63;

        // Action verb values from ADVENT.H.
        public const int Take = 1;
        public const int Drop = 2;
        public const int Say = 3;
        public const int Open = 4;
        public const int Nothing = 5;
        public const int Lock = 6;
        public const int On = 7;
        public const int Off = 8;
        public const int Wave = 9;
        public const int Calm = 10;
        public const int Walk = 11;
        public const int Kill = 12;
        public const int Pour = 13;
        public const int Eat = 14;
        public const int Drink = 15;
        public const int Rub = 16;
        public const int Throw = 17;
        public const int Quit = 18;
        public const int Find = 19;
        public const int Inventory = 20;
        public const int Feed = 21;
        public const int Fill = 22;
        public const int Blast = 23;
        public const int Score = 24;
        public const int Brief = 26;
        public const int Read = 27;
        public const int Break = 28;
        public const int Wake = 29;
        public const int Suspend = 30;
        public const int Hours = 31;
        public const int Log = 32;
        public const int Fee = 33;
        public const int Fie = 34;
        public const int Foe = 35;
        public const int Foo = 36;
        public const int Fum = 37;
        public const int Help = 51;

        // Location condition flags from ADVENT.H.
        public const int Light = 1;
        public const int WatOil = 2;
        public const int Liquid = 4;
        public const int NoPirat = 8;
        public const int Forced = 16;
        public const int HintC = 32;
        public const int HintB = 64;
        public const int HintS = 128;
        public const int HintM = 256;
        public const int HintP = 512;
        public const int HintW = 1024;
        public const int HintF = 2048;
        public const int HintO = 4096;
        public const int Hint = 8160;

        public const int HintAreaC = 0;
        public const int HintAreaB = 1;
        public const int HintAreaS = 2;
        public const int HintAreaM = 3;
        public const int HintAreaP = 4;
        public const int HintAreaW = 5;
        public const int HintAreaF = 6;
        public const int MaxHint = 6;

        public const int StartLocation = 1;
        public const int WellHouse = 3;
        public const int EndOfRoad = 1;
        public const int DepressionLocation = 8;
    }
