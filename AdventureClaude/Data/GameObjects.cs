namespace AdventureClaude.Data;

/// <summary>
/// Represents different states an object can be in.
/// </summary>
public class ObjectState
{
    public string Description { get; set; } = string.Empty;
    public string RoomDescription { get; set; } = string.Empty;
}

/// <summary>
/// Represents a game object with its various states.
/// Converted from ADVENT3.txt object descriptions.
/// </summary>
public class GameObjectData
{
    public int Id { get; set; }
    public string Name { get; set; } = string.Empty;
    public bool IsFixed { get; set; } // Objects marked with * are fixed
    public List<ObjectState> States { get; set; } = [];
}

/// <summary>
/// Contains all game object descriptions and states.
/// Converted from ADVENT3.txt file.
/// </summary>
public static class GameObjects
{
    /// <summary>
    /// Dictionary of all game objects indexed by their ID.
    /// </summary>
    public static readonly Dictionary<int, GameObjectData> Objects = new()
    {
        {1, new GameObjectData 
        {
            Id = 1,
            Name = "Set of keys",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Set of keys.", RoomDescription = "There are some keys on the ground here." }
            ]
        }},
        {2, new GameObjectData 
        {
            Id = 2,
            Name = "Brass lantern",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Brass lantern", RoomDescription = "There is a shiny brass lamp nearby." },
                new ObjectState { Description = "Brass lantern", RoomDescription = "There is a lamp shining nearby." }
            ]
        }},
        {3, new GameObjectData 
        {
            Id = 3,
            Name = "Grate",
            IsFixed = true,
            States =
            [
                new ObjectState { Description = "*Grate", RoomDescription = "The grate is locked." },
                new ObjectState { Description = "*Grate", RoomDescription = "The grate is open." }
            ]
        }},
        {4, new GameObjectData 
        {
            Id = 4,
            Name = "Wicker cage",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Wicker cage", RoomDescription = "There is a small wicker cage discarded nearby." }
            ]
        }},
        {5, new GameObjectData 
        {
            Id = 5,
            Name = "Black rod",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Black rod", RoomDescription = "A three foot black rod with a rusty star on an end lies nearby." }
            ]
        }},
        {6, new GameObjectData 
        {
            Id = 6,
            Name = "Black rod",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Black rod", RoomDescription = "A three foot black rod with a rusty mark on an end lies nearby." }
            ]
        }},
        {7, new GameObjectData 
        {
            Id = 7,
            Name = "Steps",
            IsFixed = true,
            States =
            [
                new ObjectState { Description = "*Steps", RoomDescription = "Rough stone steps lead down the pit." },
                new ObjectState { Description = "*Steps", RoomDescription = "Rough stone steps lead up the dome." }
            ]
        }},
        {8, new GameObjectData 
        {
            Id = 8,
            Name = "Little bird in cage",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Little bird in cage", RoomDescription = "A cheerful little bird is sitting here singing." },
                new ObjectState { Description = "Little bird in cage", RoomDescription = "There is a little bird in the cage." }
            ]
        }},
        {9, new GameObjectData 
        {
            Id = 9,
            Name = "Rusty door",
            IsFixed = true,
            States =
            [
                new ObjectState { Description = "*Rusty door", RoomDescription = "The way north is barred by a massive, rusty, iron door." },
                new ObjectState { Description = "*Rusty door", RoomDescription = "The way north leads through a massive, rusty, iron door." }
            ]
        }},
        {10, new GameObjectData 
        {
            Id = 10,
            Name = "Velvet pillow",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Velvet pillow", RoomDescription = "A small velvet pillow lies on the floor." }
            ]
        }},
        {11, new GameObjectData 
        {
            Id = 11,
            Name = "Snake",
            IsFixed = true,
            States =
            [
                new() { Description = "*Snake", RoomDescription = "A huge green fierce snake bars the way!" }
            ]
        }},
        {12, new GameObjectData 
        {
            Id = 12,
            Name = "Fissure",
            IsFixed = true,
            States =
            [
                new ObjectState { Description = "*Fissure", RoomDescription = "" },
                new ObjectState { Description = "*Fissure", RoomDescription = "A crystal bridge now spans the fissure." },
                new ObjectState { Description = "*Fissure", RoomDescription = "The crystal bridge has vanished!" }
            ]
        }},
        {13, new GameObjectData 
        {
            Id = 13,
            Name = "Stone tablet",
            IsFixed = true,
            States =
            [
                new ObjectState { Description = "*Stone tablet", RoomDescription = "A massive stone tablet imbedded in the wall reads:\n\"Congratulations on bringing light into the dark-room!\"" }
            ]
        }},
        {14, new GameObjectData 
        {
            Id = 14,
            Name = "Giant clam >Grunt!<",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Giant clam >Grunt!<", RoomDescription = "There is an enormous clam here with its shell tightly closed." }
            ]
        }},
        {15, new GameObjectData 
        {
            Id = 15,
            Name = "Giant oyster >Groan!<",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Giant oyster >Groan!<", RoomDescription = "There is an enormous oyster here with its shell tightly closed." },
                new ObjectState { Description = "Giant oyster >Groan!<", RoomDescription = "Interesting.  There seems to be something written on the underside of the\noyster." }
            ]
        }},
        {16, new GameObjectData 
        {
            Id = 16,
            Name = "\"Spelunker Today\"",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "\"Spelunker Today\"", RoomDescription = "There are a few recent issues of \"Spelunker Today\" magazine here." }
            ]
        }},
        {19, new GameObjectData 
        {
            Id = 19,
            Name = "Tasty food",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Tasty food", RoomDescription = "There is tasty food here." }
            ]
        }},
        {20, new GameObjectData 
        {
            Id = 20,
            Name = "Small bottle",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Small bottle", RoomDescription = "There is a bottle of water here." },
                new ObjectState { Description = "Small bottle", RoomDescription = "There is an empty bottle here." },
                new ObjectState { Description = "Small bottle", RoomDescription = "There is a bottle of oil here." }
            ]
        }},
        {21, new GameObjectData 
        {
            Id = 21,
            Name = "Water in the bottle",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Water in the bottle.", RoomDescription = "" }
            ]
        }},
        {22, new GameObjectData 
        {
            Id = 22,
            Name = "Oil in the bottle",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Oil in the bottle", RoomDescription = "" }
            ]
        }},
        {23, new GameObjectData 
        {
            Id = 23,
            Name = "Mirror",
            IsFixed = true,
            States =
            [
                new ObjectState { Description = "*Mirror", RoomDescription = "" }
            ]
        }},
        {24, new GameObjectData 
        {
            Id = 24,
            Name = "Plant",
            IsFixed = true,
            States =
            [
                new ObjectState { Description = "*Plant", RoomDescription = "There is a tiny little plant in the pit, murmuring \"Water, Water, ...\"" },
                new ObjectState { Description = "*Plant", RoomDescription = "The plant spurts into furious growth for a few seconds." },
                new ObjectState { Description = "*Plant", RoomDescription = "There is a 12-foot-tall beanstalk stretching up out of the pit, bellowing\n\"Water!! Water!!\"" },
                new ObjectState { Description = "*Plant", RoomDescription = "The plant grows explosively, almost filling the bottom of the pit." },
                new ObjectState { Description = "*Plant", RoomDescription = "There is a gigantic beanstalk stretching all the way up to the hole." },
                new ObjectState { Description = "*Plant", RoomDescription = "You've over-watered the plant!  It's shriveling up! It's, It's..." }
            ]
        }},
        {25, new GameObjectData 
        {
            Id = 25,
            Name = "Phony plant",
            IsFixed = true,
            States =
            [
                new ObjectState { Description = "*Phony plant", RoomDescription = "" },
                new ObjectState { Description = "*Phony plant", RoomDescription = "The top of a 12-foot-tall beanstalk is poking up out of the west pit." },
                new ObjectState { Description = "*Phony plant", RoomDescription = "There is a huge beanstalk growing out of the west pit up to the hole." }
            ]
        }},
        {26, new GameObjectData 
        {
            Id = 26,
            Name = "Stalactite",
            IsFixed = true,
            States =
            [
                new ObjectState { Description = "*Stalactite", RoomDescription = "" }
            ]
        }},
        {27, new GameObjectData 
        {
            Id = 27,
            Name = "Shadowy figure",
            IsFixed = true,
            States =
            [
                new ObjectState { Description = "*Shadowy figure", RoomDescription = "The shadowy figure seems to be trying to attract your attention." }
            ]
        }},
        {28, new GameObjectData 
        {
            Id = 28,
            Name = "Dwarf's axe",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Dwarf's axe", RoomDescription = "There is a little axe here." },
                new ObjectState { Description = "Dwarf's axe", RoomDescription = "There is a little axe lying beside the bear." }
            ]
        }},
        {29, new GameObjectData 
        {
            Id = 29,
            Name = "Cave drawings",
            IsFixed = true,
            States =
            [
                new ObjectState { Description = "*Cave drawings", RoomDescription = "" }
            ]
        }},
        {30, new GameObjectData 
        {
            Id = 30,
            Name = "Pirate",
            IsFixed = true,
            States =
            [
                new ObjectState { Description = "*Pirate", RoomDescription = "" }
            ]
        }},
        {31, new GameObjectData 
        {
            Id = 31,
            Name = "Dragon",
            IsFixed = true,
            States =
            [
                new ObjectState { Description = "*Dragon", RoomDescription = "A huge green fierce dragon bars the way!" },
                new ObjectState { Description = "*Dragon", RoomDescription = "Congratulations!  You have just vanquished a dragon with your bare hands!\n(Unbelievable, isn't it?)" },
                new ObjectState { Description = "*Dragon", RoomDescription = "The body of a huge green dead dragon is lying off to one side." }
            ]
        }},
        {32, new GameObjectData 
        {
            Id = 32,
            Name = "Chasm",
            IsFixed = true,
            States =
            [
                new ObjectState { Description = "*Chasm", RoomDescription = "A rickety wooden bridge extends across the chasm, vanishing into the mist.\nA sign posted on the bridge reads:\n          \"Stop!  Pay Troll!\"" },
                new ObjectState { Description = "*Chasm", RoomDescription = "The wreckage of a bridge (and a dead bear) can be seen at the bottom of the\nchasm." }
            ]
        }},
        {33, new GameObjectData 
        {
            Id = 33,
            Name = "Troll",
            IsFixed = true,
            States =
            [
                new ObjectState { Description = "*Troll", RoomDescription = "A burly troll stands by the bridge and insists you throw him a treasure\nbefore you may cross." },
                new ObjectState { Description = "*Troll", RoomDescription = "The troll steps out from beneath the bridge and blocks your way." },
                new ObjectState { Description = "*Troll", RoomDescription = "" }
            ]
        }},
        {34, new GameObjectData 
        {
            Id = 34,
            Name = "Phony troll",
            IsFixed = true,
            States =
            [
                new ObjectState { Description = "*Phony troll", RoomDescription = "The troll is nowhere to be seen." }
            ]
        }},
        {35, new GameObjectData 
        {
            Id = 35,
            Name = "Bear",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Bear", RoomDescription = "" },
                new ObjectState { Description = "Bear", RoomDescription = "There is a ferocious cave bear eyeing you from the far end of the room!" },
                new ObjectState { Description = "Bear", RoomDescription = "There is a gentle cave bear sitting placidly in one corner." },
                new ObjectState { Description = "Bear", RoomDescription = "There is a contented-looking bear wandering about nearby." },
                new ObjectState { Description = "Bear", RoomDescription = "" }
            ]
        }},
        {36, new GameObjectData 
        {
            Id = 36,
            Name = "Message in second maze",
            IsFixed = true,
            States =
            [
                new ObjectState { Description = "*Message in second maze", RoomDescription = "There is a message scrawled in the dust in a flowery script, reading:\n           \"This is not the maze where the\"\n           \"pirate leaves his treasure chest\"" }
            ]
        }},
        {37, new GameObjectData 
        {
            Id = 37,
            Name = "Volcano and,or Geyser",
            IsFixed = true,
            States =
            [
                new ObjectState { Description = "*Volcano and,or Geyser", RoomDescription = "" }
            ]
        }},
        {38, new GameObjectData 
        {
            Id = 38,
            Name = "Vending machine",
            IsFixed = true,
            States =
            [
                new ObjectState { Description = "*Vending machine", RoomDescription = "There is a massive vending machine here.  The instructions on it read:\n     \"Drop coins here to receive fresh batteries.\"" }
            ]
        }},
        {39, new GameObjectData 
        {
            Id = 39,
            Name = "Batteries",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Batteries", RoomDescription = "There are fresh batteries here." },
                new ObjectState { Description = "Batteries", RoomDescription = "Some worn-out batteries have been discarded nearby." }
            ]
        }},
        {40, new GameObjectData 
        {
            Id = 40,
            Name = "Carpet and,or moss",
            IsFixed = true,
            States =
            [
                new ObjectState { Description = "*Carpet and,or moss", RoomDescription = "" }
            ]
        }},
        {50, new GameObjectData 
        {
            Id = 50,
            Name = "Large gold nugget",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Large gold nugget", RoomDescription = "There is a large sparkling nugget of gold here!" }
            ]
        }},
        {51, new GameObjectData 
        {
            Id = 51,
            Name = "Several diamonds",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Several diamonds", RoomDescription = "There are diamonds here!" }
            ]
        }},
        {52, new GameObjectData 
        {
            Id = 52,
            Name = "Bars of silver",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Bars of silver", RoomDescription = "There are bars of silver here!" }
            ]
        }},
        {53, new GameObjectData 
        {
            Id = 53,
            Name = "Precious jewelry",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Precious jewelry", RoomDescription = "There is precious jewelry here!" }
            ]
        }},
        {54, new GameObjectData 
        {
            Id = 54,
            Name = "Rare coins",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Rare coins", RoomDescription = "There are many coins here!" }
            ]
        }},
        {55, new GameObjectData 
        {
            Id = 55,
            Name = "Treasure chest",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Treasure chest", RoomDescription = "The pirate's treasure chest is here!" }
            ]
        }},
        {56, new GameObjectData 
        {
            Id = 56,
            Name = "Golden eggs",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Golden eggs", RoomDescription = "There is a large nest here, full of golden eggs!" },
                new ObjectState { Description = "Golden eggs", RoomDescription = "The nest of golden eggs has vanished!" },
                new ObjectState { Description = "Golden eggs", RoomDescription = "Done!" }
            ]
        }},
        {57, new GameObjectData 
        {
            Id = 57,
            Name = "Jeweled trident",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Jeweled trident", RoomDescription = "There is a jewel-encrusted trident here!" }
            ]
        }},
        {58, new GameObjectData 
        {
            Id = 58,
            Name = "Ming vase",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Ming vase", RoomDescription = "There is a delicate, precious, Ming vase here!" },
                new ObjectState { Description = "Ming vase", RoomDescription = "The vase is now resting, delicately, on a velvet pillow." },
                new ObjectState { Description = "Ming vase", RoomDescription = "The floor is littered with worthless shards of pottery." },
                new ObjectState { Description = "Ming vase", RoomDescription = "The Ming vase drops with a delicate crash." }
            ]
        }},
        {59, new GameObjectData 
        {
            Id = 59,
            Name = "Egg-sized emerald",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Egg-sized emerald", RoomDescription = "There is an emerald here the size of a plover's egg!" }
            ]
        }},
        {60, new GameObjectData 
        {
            Id = 60,
            Name = "Platinum pyramid",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Platinum pyramid", RoomDescription = "There is a platinum pyramid here, 8 inches on a side!" }
            ]
        }},
        {61, new GameObjectData 
        {
            Id = 61,
            Name = "Glistening pearl",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Glistening pearl", RoomDescription = "Off to one side lies a glistening pearl!" }
            ]
        }},
        {62, new GameObjectData 
        {
            Id = 62,
            Name = "Persian rug",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Persian rug", RoomDescription = "There is a persian rug spread out on the floor!" },
                new ObjectState { Description = "Persian rug", RoomDescription = "The dragon is sprawled out on a persian rug!!" }
            ]
        }},
        {63, new GameObjectData 
        {
            Id = 63,
            Name = "Rare spices",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Rare spices", RoomDescription = "There are rare spices here!" }
            ]
        }},
        {64, new GameObjectData 
        {
            Id = 64,
            Name = "Golden chain",
            IsFixed = false,
            States =
            [
                new ObjectState { Description = "Golden chain", RoomDescription = "There is a golden chain lying in a heap on the floor!" },
                new ObjectState { Description = "Golden chain", RoomDescription = "The bear is locked to the wall with a golden chain!" },
                new ObjectState { Description = "Golden chain", RoomDescription = "There is a golden chain locked to the wall!" }
            ]
        }}
    };
}