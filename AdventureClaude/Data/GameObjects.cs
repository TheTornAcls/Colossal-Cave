using System.Collections.Generic;

namespace AdventureClaude.Data
{
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
        public List<ObjectState> States { get; set; } = new List<ObjectState>();
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
        public static readonly Dictionary<int, GameObjectData> Objects = new Dictionary<int, GameObjectData>
        {
            {1, new GameObjectData 
            {
                Id = 1,
                Name = "Set of keys",
                IsFixed = false,
                States = new List<ObjectState>
                {
                    new ObjectState { Description = "Set of keys.", RoomDescription = "There are some keys on the ground here." }
                }
            }},
            {2, new GameObjectData 
            {
                Id = 2,
                Name = "Brass lantern",
                IsFixed = false,
                States = new List<ObjectState>
                {
                    new ObjectState { Description = "Brass lantern", RoomDescription = "There is a shiny brass lamp nearby." },
                    new ObjectState { Description = "Brass lantern", RoomDescription = "There is a lamp shining nearby." }
                }
            }},
            {3, new GameObjectData 
            {
                Id = 3,
                Name = "Grate",
                IsFixed = true,
                States = new List<ObjectState>
                {
                    new ObjectState { Description = "*Grate", RoomDescription = "The grate is locked." },
                    new ObjectState { Description = "*Grate", RoomDescription = "The grate is open." }
                }
            }},
            {4, new GameObjectData 
            {
                Id = 4,
                Name = "Wicker cage",
                IsFixed = false,
                States = new List<ObjectState>
                {
                    new ObjectState { Description = "Wicker cage", RoomDescription = "There is a small wicker cage discarded nearby." }
                }
            }},
            {5, new GameObjectData 
            {
                Id = 5,
                Name = "Black rod",
                IsFixed = false,
                States = new List<ObjectState>
                {
                    new ObjectState { Description = "Black rod", RoomDescription = "A three foot black rod with a rusty star on an end lies nearby." }
                }
            }},
            {6, new GameObjectData 
            {
                Id = 6,
                Name = "Black rod",
                IsFixed = false,
                States = new List<ObjectState>
                {
                    new ObjectState { Description = "Black rod", RoomDescription = "A three foot black rod with a rusty mark on an end lies nearby." }
                }
            }},
            {7, new GameObjectData 
            {
                Id = 7,
                Name = "Steps",
                IsFixed = true,
                States = new List<ObjectState>
                {
                    new ObjectState { Description = "*Steps", RoomDescription = "Rough stone steps lead down the pit." },
                    new ObjectState { Description = "*Steps", RoomDescription = "Rough stone steps lead up the dome." }
                }
            }},
            {8, new GameObjectData 
            {
                Id = 8,
                Name = "Little bird in cage",
                IsFixed = false,
                States = new List<ObjectState>
                {
                    new ObjectState { Description = "Little bird in cage", RoomDescription = "A cheerful little bird is sitting here singing." },
                    new ObjectState { Description = "Little bird in cage", RoomDescription = "There is a little bird in the cage." }
                }
            }},
            {9, new GameObjectData 
            {
                Id = 9,
                Name = "Rusty door",
                IsFixed = true,
                States = new List<ObjectState>
                {
                    new ObjectState { Description = "*Rusty door", RoomDescription = "The way north is barred by a massive, rusty, iron door." },
                    new ObjectState { Description = "*Rusty door", RoomDescription = "The way north leads through a massive, rusty, iron door." }
                }
            }},
            {10, new GameObjectData 
            {
                Id = 10,
                Name = "Velvet pillow",
                IsFixed = false,
                States = new List<ObjectState>
                {
                    new ObjectState { Description = "Velvet pillow", RoomDescription = "A small velvet pillow lies on the floor." }
                }
            }},
            {11, new GameObjectData 
            {
                Id = 11,
                Name = "Snake",
                IsFixed = true,
                States = new List<ObjectState>
                {
                    new ObjectState { Description = "*Snake", RoomDescription = "A huge green fierce snake bars the way!" }
                }
            }},
            {12, new GameObjectData 
            {
                Id = 12,
                Name = "Fissure",
                IsFixed = true,
                States = new List<ObjectState>
                {
                    new ObjectState { Description = "*Fissure", RoomDescription = "" },
                    new ObjectState { Description = "*Fissure", RoomDescription = "A crystal bridge now spans the fissure." },
                    new ObjectState { Description = "*Fissure", RoomDescription = "The crystal bridge has vanished!" }
                }
            }},
            {13, new GameObjectData 
            {
                Id = 13,
                Name = "Stone tablet",
                IsFixed = true,
                States = new List<ObjectState>
                {
                    new ObjectState { Description = "*Stone tablet", RoomDescription = "A massive stone tablet imbedded in the wall reads:\n\"Congratulations on bringing light into the dark-room!\"" }
                }
            }},
            {14, new GameObjectData 
            {
                Id = 14,
                Name = "Giant clam >Grunt!<",
                IsFixed = false,
                States = new List<ObjectState>
                {
                    new ObjectState { Description = "Giant clam >Grunt!<", RoomDescription = "There is an enormous clam here with its shell tightly closed." }
                }
            }},
            {15, new GameObjectData 
            {
                Id = 15,
                Name = "Giant oyster >Groan!<",
                IsFixed = false,
                States = new List<ObjectState>
                {
                    new ObjectState { Description = "Giant oyster >Groan!<", RoomDescription = "There is an enormous oyster here with its shell tightly closed." },
                    new ObjectState { Description = "Giant oyster >Groan!<", RoomDescription = "Interesting.  There seems to be something written on the underside of the\noyster." }
                }
            }},
            {16, new GameObjectData 
            {
                Id = 16,
                Name = "\"Spelunker Today\"",
                IsFixed = false,
                States = new List<ObjectState>
                {
                    new ObjectState { Description = "\"Spelunker Today\"", RoomDescription = "There are a few recent issues of \"Spelunker Today\" magazine here." }
                }
            }},
            {19, new GameObjectData 
            {
                Id = 19,
                Name = "Tasty food",
                IsFixed = false,
                States = new List<ObjectState>
                {
                    new ObjectState { Description = "Tasty food", RoomDescription = "There is tasty food here." }
                }
            }},
            {20, new GameObjectData 
            {
                Id = 20,
                Name = "Small bottle",
                IsFixed = false,
                States = new List<ObjectState>
                {
                    new ObjectState { Description = "Small bottle", RoomDescription = "There is a bottle of water here." },
                    new ObjectState { Description = "Small bottle", RoomDescription = "There is an empty bottle here." },
                    new ObjectState { Description = "Small bottle", RoomDescription = "There is a bottle of oil here." }
                }
            }},
            {21, new GameObjectData 
            {
                Id = 21,
                Name = "Water in the bottle",
                IsFixed = false,
                States = new List<ObjectState>
                {
                    new ObjectState { Description = "Water in the bottle.", RoomDescription = "" }
                }
            }},
            {22, new GameObjectData 
            {
                Id = 22,
                Name = "Oil in the bottle",
                IsFixed = false,
                States = new List<ObjectState>
                {
                    new ObjectState { Description = "Oil in the bottle", RoomDescription = "" }
                }
            }},
            {23, new GameObjectData 
            {
                Id = 23,
                Name = "Mirror",
                IsFixed = true,
                States = new List<ObjectState>
                {
                    new ObjectState { Description = "*Mirror", RoomDescription = "" }
                }
            }},
            {24, new GameObjectData 
            {
                Id = 24,
                Name = "Plant",
                IsFixed = true,
                States = new List<ObjectState>
                {
                    new ObjectState { Description = "*Plant", RoomDescription = "There is a tiny little plant in the pit, murmuring \"Water, Water, ...\"" },
                    new ObjectState { Description = "*Plant", RoomDescription = "The plant spurts into furious growth for a few seconds." },
                    new ObjectState { Description = "*Plant", RoomDescription = "There is a 12-foot-tall beanstalk stretching up out of the pit, bellowing\n\"Water!! Water!!\"" }
                }
            }}
        };

        // Continue with more objects - this represents the key objects from ADVENT3.txt
        // The full file contains more objects but these are the main ones referenced in the game logic
    }
}