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

        public GamePositionState Position { get; } = new();
        public WorldMapState World { get; } = new();
        public ObjectPlacementState Objects { get; } = new();
        public CaveTimingState Cave { get; } = new();
        public TreasureProgressState TreasureProgress { get; } = new();
        public DwarfPirateState Dwarves { get; } = new();
        public HintTrackingState Hints { get; } = new();
        public ParsedCommandState Command { get; } = new();
        public DebugOptions Debug { get; } = new();

        // Compatibility properties keep the existing port call sites stable while the
        // state is grouped into debugger-friendly domains.
        public int Turns { get => Position.Turns; set => Position.Turns = value; }
        public int Location { get => Position.Location; set => Position.Location = value; }
        public int OldLocation { get => Position.OldLocation; set => Position.OldLocation = value; }
        public int OldLocation2 { get => Position.OldLocation2; set => Position.OldLocation2 = value; }
        public int NewLocation { get => Position.NewLocation; set => Position.NewLocation = value; }
        public int Detail { get => Position.Detail; set => Position.Detail = value; }

        public short[] LocationConditions { get => World.LocationConditions; set => World.LocationConditions = value; }
        public short[] VisitedLocations { get => World.VisitedLocations; set => World.VisitedLocations = value; }
        public List<TravelOption> CurrentTravelOptions { get => World.CurrentTravelOptions; set => World.CurrentTravelOptions = value; }

        public int[] ObjectLocations { get => Objects.Locations; set => Objects.Locations = value; }
        public int[] FixedObjectLocations { get => Objects.FixedLocations; set => Objects.FixedLocations = value; }
        public short[] ObjectProperties { get => Objects.Properties; set => Objects.Properties = value; }
        public short[] ActionMessages { get => Objects.ActionMessages; set => Objects.ActionMessages = value; }
        public int Holding { get => Objects.Holding; set => Objects.Holding = value; }
        public int KnifeLocation { get => Objects.KnifeLocation; set => Objects.KnifeLocation = value; }
        public int ChestLocation { get => Objects.ChestLocation; set => Objects.ChestLocation = value; }
        public int ChestLocation2 { get => Objects.ChestLocation2; set => Objects.ChestLocation2 = value; }

        public bool WizardDark { get => Cave.WizardDark; set => Cave.WizardDark = value; }
        public bool Closed { get => Cave.Closed; set => Cave.Closed = value; }
        public bool Closing { get => Cave.Closing; set => Cave.Closing = value; }
        public int Limit { get => Cave.LampLimit; set => Cave.LampLimit = value; }
        public int Clock1 { get => Cave.Clock1; set => Cave.Clock1 = value; }
        public int Clock2 { get => Cave.Clock2; set => Cave.Clock2 = value; }
        public bool Panic { get => Cave.Panic; set => Cave.Panic = value; }
        public int LampWarning { get => Cave.LampWarning; set => Cave.LampWarning = value; }

        public int Tally { get => TreasureProgress.UndiscoveredTreasureCount; set => TreasureProgress.UndiscoveredTreasureCount = value; }
        public int Tally2 { get => TreasureProgress.TreasuresLostToEndgame; set => TreasureProgress.TreasuresLostToEndgame = value; }
        public int Bonus { get => TreasureProgress.Bonus; set => TreasureProgress.Bonus = value; }
        public int NumDie { get => TreasureProgress.DeathCount; set => TreasureProgress.DeathCount = value; }
        public bool GaveUp { get => TreasureProgress.GaveUp; set => TreasureProgress.GaveUp = value; }
        public bool SaveFlag { get => TreasureProgress.SaveRequested; set => TreasureProgress.SaveRequested = value; }
        public int HintTaken { get => TreasureProgress.HintsAccepted; set => TreasureProgress.HintsAccepted = value; }
        public int FooBar { get => TreasureProgress.FooBar; set => TreasureProgress.FooBar = value; }
        public int TestBr { get => TreasureProgress.DescriptionDetailMask; set => TreasureProgress.DescriptionDetailMask = value; }

        public int[] DwarfLocations { get => Dwarves.Locations; set => Dwarves.Locations = value; }
        public int[] OldDwarfLocations { get => Dwarves.PreviousLocations; set => Dwarves.PreviousLocations = value; }
        public int DwarfKill { get => Dwarves.KillCount; set => Dwarves.KillCount = value; }
        public bool[] DwarfSeen { get => Dwarves.Seen; set => Dwarves.Seen = value; }
        public int DwarfAlternateLocation { get => Dwarves.AlternateLocation; set => Dwarves.AlternateLocation = value; }
        public int DwarfFlag { get => Dwarves.ActivationLevel; set => Dwarves.ActivationLevel = value; }

        public int HintAvailable { get => Hints.AvailableMask; set => Hints.AvailableMask = value; }
        public int[] HintLocations { get => Hints.LocationCounters; set => Hints.LocationCounters = value; }

        public int Verb { get => Command.Verb; set => Command.Verb = value; }
        public int Object { get => Command.Object; set => Command.Object = value; }
        public int Motion { get => Command.Motion; set => Command.Motion = value; }
        public string Word1 { get => Command.Word1; set => Command.Word1 = value; }
        public string Word2 { get => Command.Word2; set => Command.Word2 = value; }

        public int DebugFlag { get => Debug.Flag; set => Debug.Flag = value; }

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
                Objects.SetProperty(i, -1);
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
        /// C DATABASE.C toting(): true when an object is being carried.
        /// </summary>
        public bool Toting(int item)
        {
            return Objects.IsCarried(item);
        }

        /// <summary>
        /// C DATABASE.C here(): true when an object is at the current location or carried.
        /// </summary>
        public bool Here(int item)
        {
            return Objects.IsAtLocation(item, Location) || Toting(item);
        }

        /// <summary>
        /// C DATABASE.C at(): true when an object is physically or fixed at the current location.
        /// </summary>
        public bool At(int item)
        {
            return Objects.IsAt(item, Location);
        }

        /// <summary>
        /// C DATABASE.C forced(): true when a location forces automatic movement.
        /// </summary>
        public bool Forced(int atLocation)
        {
            return (LocationConditions[atLocation] & GameConstants.Forced) != 0;
        }

        public bool LocationHasFlag(int location, int flag)
        {
            return (LocationConditions[location] & flag) != 0;
        }

        /// <summary>
        /// C DATABASE.C dstroy(): removes an object from the game.
        /// </summary>
        public void Destroy(int obj)
        {
            MoveObject(obj, 0);
        }

        /// <summary>
        /// C DATABASE.C carry(): marks an object as carried and updates Holding.
        /// </summary>
        public void Carry(int obj, int fromLocation)
        {
            if (obj >= GameConstants.MaxObjects)
                return;

            if (Objects.IsCarried(obj))
                return;

            Objects.SetLocation(obj, -1);
            Holding++;
        }

        /// <summary>
        /// C DATABASE.C drop(): places an object or fixed-object side at a location.
        /// </summary>
        public void Drop(int obj, int where)
        {
            if (obj < GameConstants.MaxObjects)
            {
                if (Objects.IsCarried(obj))
                    Holding--;

                Objects.SetLocation(obj, where);
            }
            else
            {
                Objects.SetFixedLocation(obj - GameConstants.MaxObjects, where);
            }
        }

        /// <summary>
        /// C DATABASE.C move(): moves an object, preserving Holding bookkeeping.
        /// </summary>
        public void MoveObject(int obj, int where)
        {
            int from = obj < GameConstants.MaxObjects
                ? Objects.LocationOf(obj)
                : Objects.FixedLocationOf(obj - GameConstants.MaxObjects);

            if (from > 0 && from <= 300)
                Carry(obj, from);

            Drop(obj, where);
        }

        /// <summary>
        /// C DATABASE.C put(): moves an object and returns the encoded repository property.
        /// </summary>
        public int Put(int obj, int where, int propertyValue)
        {
            MoveObject(obj, where);
            return -1 - propertyValue;
        }

        /// <summary>
        /// C DATABASE.C dcheck(): returns the first dwarf in the player's location.
        /// </summary>
        public int DCheck()
        {
            for (int i = 1; i < GameConstants.MaxDwarves - 1; i++)
            {
                if (DwarfLocations[i] == Location)
                    return i;
            }

            return 0;
        }

        /// <summary>
        /// C DATABASE.C liq(): returns WATER, OIL, or 0 for the bottle contents.
        /// </summary>
        public int Liq()
        {
            int bottleProperty = Objects.PropertyOf(GameConstants.Bottle);
            int validatedProperty = -1 - bottleProperty;
            return Liq2(bottleProperty > validatedProperty ? bottleProperty : validatedProperty);
        }

        /// <summary>
        /// C DATABASE.C liqloc(): returns WATER, OIL, or 0 for a location's available liquid.
        /// </summary>
        public int LiqLoc(int location)
        {
            if ((LocationConditions[location] & GameConstants.Liquid) != 0)
                return Liq2(LocationConditions[location] & GameConstants.WatOil);

            return Liq2(1);
        }

        /// <summary>
        /// C DATABASE.C liq2(): converts a bottle/location liquid code to an object id.
        /// </summary>
        public static int Liq2(int pbottle)
        {
            return (1 - pbottle) * GameConstants.Water +
                (pbottle >> 1) * (GameConstants.Water + GameConstants.Oil);
        }

        /// <summary>
        /// C DATABASE.C pct(): true with the requested percentage chance.
        /// </summary>
        public static bool Pct(Random random, int percent)
        {
            return random.Next(100) < percent;
        }

        /// <summary>
        /// C DATABASE.C rrand(): random inclusive integer helper.
        /// </summary>
        public static int RRand(Random random, int low, int high)
        {
            return random.Next(low, high + 1);
        }

        /// <summary>
        /// C DATABASE.C juggle(): intentionally a no-op in this C port.
        /// </summary>
        public static void Juggle(int location)
        {
        }

        public int GetActionMessageId(int verb)
        {
            return Objects.ActionMessageFor(verb);
        }

        public void SetObjectLocation(int objectId, int location)
        {
            Objects.SetLocation(objectId, location);
        }

        public void SetObjectProperty(int objectId, int property)
        {
            Objects.SetProperty(objectId, property);
        }

        /// <summary>
        /// Checks if the player is carrying a specific object.
        /// </summary>
        /// <param name="objectId">The ID of the object to check</param>
        /// <returns>True if the player is carrying the object</returns>
        public bool IsCarrying(int objectId)
        {
            return Toting(objectId);
        }

        /// <summary>
        /// Checks if an object is at the player's current location.
        /// </summary>
        /// <param name="objectId">The ID of the object to check</param>
        /// <returns>True if the object is at the current location</returns>
        public bool IsObjectHere(int objectId)
        {
            return Here(objectId) || Objects.IsFixedAtLocation(objectId, Location);
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
            var allOptions = AdventureClaude.Data.AdventureData.GetTravelOptions(Location);
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

                case 3:
                case 4:
                case 5:
                case 7:
                    return Objects.PropertyOf(objectId) != conditionType - 3;

                default:
                    return false;
            }
        }
    }

public sealed class GamePositionState
{
    public int Turns { get; set; }
    public int Location { get; set; } = GameConstants.StartLocation;
    public int OldLocation { get; set; } = GameConstants.StartLocation;
    public int OldLocation2 { get; set; } = GameConstants.StartLocation;
    public int NewLocation { get; set; } = GameConstants.StartLocation;
    public int Detail { get; set; }
}

public sealed class WorldMapState
{
    public short[] LocationConditions { get; set; } = new short[GameConstants.LocationArraySize];
    public short[] VisitedLocations { get; set; } = new short[GameConstants.LocationArraySize];
    public List<TravelOption> CurrentTravelOptions { get; set; } = new();
}

public sealed class ObjectPlacementState
{
    public int[] Locations { get; set; } = new int[GameConstants.ObjectArraySize];
    public int[] FixedLocations { get; set; } = new int[GameConstants.ObjectArraySize];
    public short[] Properties { get; set; } = new short[GameConstants.ObjectArraySize];
    public short[] ActionMessages { get; set; } = new short[GameConstants.ActionMessageSize];
    public int Holding { get; set; }
    public int KnifeLocation { get; set; }
    public int ChestLocation { get; set; }
    public int ChestLocation2 { get; set; }

    public int LocationOf(int objectId)
    {
        return Locations[objectId];
    }

    public void SetLocation(int objectId, int location)
    {
        Locations[objectId] = location;
    }

    public int FixedLocationOf(int objectId)
    {
        return FixedLocations[objectId];
    }

    public void SetFixedLocation(int objectId, int location)
    {
        FixedLocations[objectId] = location;
    }

    public short PropertyOf(int objectId)
    {
        return Properties[objectId];
    }

    public void SetProperty(int objectId, int property)
    {
        Properties[objectId] = (short)property;
    }

    public int ActionMessageFor(int verb)
    {
        return verb >= 0 && verb < ActionMessages.Length ? ActionMessages[verb] : 0;
    }

    public bool IsCarried(int objectId)
    {
        return LocationOf(objectId) == -1;
    }

    public bool IsAtLocation(int objectId, int location)
    {
        return LocationOf(objectId) == location;
    }

    public bool IsFixedAtLocation(int objectId, int location)
    {
        return FixedLocationOf(objectId) == location;
    }

    public bool IsAt(int objectId, int location)
    {
        return IsAtLocation(objectId, location) || IsFixedAtLocation(objectId, location);
    }

    public bool HasFixedLocation(int objectId)
    {
        return FixedLocationOf(objectId) != 0;
    }

    public bool IsPropertyNegative(int objectId)
    {
        return PropertyOf(objectId) < 0;
    }
}

public sealed class CaveTimingState
{
    public bool WizardDark { get; set; }
    public bool Closed { get; set; }
    public bool Closing { get; set; }
    public int LampLimit { get; set; } = 330;
    public int Clock1 { get; set; }
    public int Clock2 { get; set; }
    public bool Panic { get; set; }
    public int LampWarning { get; set; }
}

public sealed class TreasureProgressState
{
    public int UndiscoveredTreasureCount { get; set; }
    public int TreasuresLostToEndgame { get; set; }
    public int Bonus { get; set; }
    public int DeathCount { get; set; }
    public bool GaveUp { get; set; }
    public bool SaveRequested { get; set; }
    public int HintsAccepted { get; set; }
    public int FooBar { get; set; }
    public int DescriptionDetailMask { get; set; }
}

public sealed class DwarfPirateState
{
    public int[] Locations { get; set; } = new int[GameConstants.DwarfArraySize];
    public int[] PreviousLocations { get; set; } = new int[GameConstants.DwarfArraySize];
    public int KillCount { get; set; }
    public bool[] Seen { get; set; } = new bool[GameConstants.DwarfArraySize];
    public int AlternateLocation { get; set; }
    public int ActivationLevel { get; set; }
}

public sealed class HintTrackingState
{
    public int AvailableMask { get; set; }
    public int[] LocationCounters { get; set; } = new int[GameConstants.HintArraySize];
}

public sealed class ParsedCommandState
{
    public int Verb { get; set; }
    public int Object { get; set; }
    public int Motion { get; set; }
    public string Word1 { get; set; } = string.Empty;
    public string Word2 { get; set; } = string.Empty;
}

public sealed class DebugOptions
{
    public int Flag { get; set; }
}
