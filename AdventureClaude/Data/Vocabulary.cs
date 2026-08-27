namespace AdventureClaude.Data;

using System.Collections.Generic;

/// <summary>
/// Represents a word entry in the adventure vocabulary.
/// </summary>
public class WordEntry
    {
        public string Word { get; set; } = string.Empty;
        public int Type { get; set; }
        public int Value { get; set; }
    }

    /// <summary>
    /// Contains the adventure game vocabulary.
    /// Converted from ADVWORD.H.
    /// </summary>
    public static class Vocabulary
    {
        /// <summary>
        /// Dictionary of all known words mapped to their decoded original type and value.
        /// </summary>
        public static readonly Dictionary<string, WordEntry> Words = new();

        private static readonly List<WordEntry> WordList = [];

        static Vocabulary()
        {
            InitializeWords();
        }

        private static void InitializeWords()
        {
            AddWord("?", 3, 51);
            AddWord("above", 0, 29);
            AddWord("abra", 3, 50);
            AddWord("abracadabra", 3, 50);
            AddWord("across", 0, 42);
            AddWord("ascend", 0, 29);
            AddWord("attack", 2, 12);
            AddWord("awkward", 0, 26);
            AddWord("axe", 1, 28);
            AddWord("back", 0, 8);
            AddWord("barren", 0, 40);
            AddWord("bars", 1, 52);
            AddWord("batteries", 1, 39);
            AddWord("battery", 1, 39);
            AddWord("beans", 1, 24);
            AddWord("bear", 1, 35);
            AddWord("bed", 0, 16);
            AddWord("bedquilt", 0, 70);
            AddWord("bird", 1, 8);
            AddWord("blast", 2, 23);
            AddWord("blowup", 2, 23);
            AddWord("bottle", 1, 20);
            AddWord("box", 1, 55);
            AddWord("break", 2, 28);
            AddWord("brief", 2, 26);
            AddWord("broken", 0, 54);
            AddWord("building", 0, 12);
            AddWord("cage", 1, 4);
            AddWord("calm", 2, 10);
            AddWord("canyon", 0, 25);
            AddWord("capture", 2, 1);
            AddWord("carpet", 1, 40);
            AddWord("carry", 2, 1);
            AddWord("catch", 2, 1);
            AddWord("cave", 0, 67);
            AddWord("cavern", 0, 73);
            AddWord("chain", 1, 64);
            AddWord("chant", 2, 3);
            AddWord("chasm", 1, 32);
            AddWord("chest", 1, 55);
            AddWord("clam", 1, 14);
            AddWord("climb", 0, 56);
            AddWord("close", 2, 6);
            AddWord("cobblestone", 0, 18);
            AddWord("coins", 1, 54);
            AddWord("continue", 2, 11);
            AddWord("crack", 0, 33);
            AddWord("crap", 3, 79);
            AddWord("crawl", 0, 17);
            AddWord("cross", 0, 69);
            AddWord("d", 0, 30);
            AddWord("damn", 3, 79);
            AddWord("damnit", 3, 79);
            AddWord("dark", 0, 22);
            AddWord("debris", 0, 51);
            AddWord("depression", 0, 63);
            AddWord("descend", 0, 30);
            AddWord("describe", 0, 57);
            AddWord("detonate", 2, 23);
            AddWord("devour", 2, 14);
            AddWord("diamonds", 1, 51);
            AddWord("dig", 3, 66);
            AddWord("discard", 2, 2);
            AddWord("disturb", 2, 29);
            AddWord("dome", 0, 35);
            AddWord("door", 1, 9);
            AddWord("down", 0, 30);
            AddWord("downstream", 0, 4);
            AddWord("downward", 0, 30);
            AddWord("dragon", 1, 31);
            AddWord("drawing", 1, 29);
            AddWord("drink", 2, 15);
            AddWord("drop", 2, 2);
            AddWord("dump", 2, 2);
            AddWord("dwarf", 1, 17);
            AddWord("dwarves", 1, 17);
            AddWord("e", 0, 43);
            AddWord("east", 0, 43);
            AddWord("eat", 2, 14);
            AddWord("egg", 1, 56);
            AddWord("eggs", 1, 56);
            AddWord("emerald", 1, 59);
            AddWord("enter", 0, 3);
            AddWord("entrance", 0, 64);
            AddWord("examine", 0, 57);
            AddWord("excavate", 3, 66);
            AddWord("exit", 0, 11);
            AddWord("explore", 2, 11);
            AddWord("extinguish", 2, 8);
            AddWord("fee", 2, 33);
            AddWord("feed", 2, 21);
            AddWord("fie", 2, 34);
            AddWord("fight", 2, 12);
            AddWord("figure", 1, 27);
            AddWord("fill", 2, 22);
            AddWord("find", 2, 19);
            AddWord("fissure", 1, 12);
            AddWord("floor", 0, 58);
            AddWord("foe", 2, 35);
            AddWord("follow", 2, 11);
            AddWord("foo", 2, 36);
            AddWord("food", 1, 19);
            AddWord("forest", 0, 6);
            AddWord("fork", 0, 77);
            AddWord("forward", 0, 7);
            AddWord("free", 2, 2);
            AddWord("fuck", 3, 79);
            AddWord("fum", 2, 37);
            AddWord("get", 2, 1);
            AddWord("geyser", 1, 37);
            AddWord("giant", 0, 27);
            AddWord("go", 2, 11);
            AddWord("gold", 1, 50);
            AddWord("goto", 2, 11);
            AddWord("grate", 1, 3);
            AddWord("gully", 0, 13);
            AddWord("h2o", 1, 21);
            AddWord("hall", 0, 38);
            AddWord("headlamp", 1, 2);
            AddWord("help", 3, 51);
            AddWord("hill", 0, 2);
            AddWord("hit", 2, 12);
            AddWord("hocus", 3, 50);
            AddWord("hole", 0, 52);
            AddWord("hours", 2, 31);
            AddWord("house", 0, 12);
            AddWord("i", 2, 20);
            AddWord("ignite", 2, 23);
            AddWord("in", 0, 19);
            AddWord("info", 3, 142);
            AddWord("information", 3, 142);
            AddWord("inside", 0, 19);
            AddWord("inventory", 2, 20);
            AddWord("inward", 0, 19);
            AddWord("issue", 1, 16);
            AddWord("jar", 1, 20);
            AddWord("jewel", 1, 53);
            AddWord("jewelry", 1, 53);
            AddWord("jewels", 1, 53);
            AddWord("jump", 0, 39);
            AddWord("keep", 2, 1);
            AddWord("key", 1, 1);
            AddWord("keys", 1, 1);
            AddWord("kill", 2, 12);
            AddWord("knife", 1, 18);
            AddWord("knives", 1, 18);
            AddWord("l", 0, 57);
            AddWord("lamp", 1, 2);
            AddWord("lantern", 1, 2);
            AddWord("leave", 0, 11);
            AddWord("left", 0, 36);
            AddWord("light", 2, 7);
            AddWord("lock", 2, 6);
            AddWord("log", 2, 32);
            AddWord("look", 0, 57);
            AddWord("lost", 3, 68);
            AddWord("low", 0, 24);
            AddWord("machine", 1, 38);
            AddWord("magazine", 1, 16);
            AddWord("main", 0, 76);
            AddWord("message", 1, 36);
            AddWord("ming", 1, 58);
            AddWord("mirror", 1, 23);
            AddWord("mist", 3, 69);
            AddWord("moss", 1, 40);
            AddWord("mumble", 2, 3);
            AddWord("n", 0, 45);
            AddWord("ne", 0, 47);
            AddWord("nest", 1, 56);
            AddWord("north", 0, 45);
            AddWord("nothing", 2, 5);
            AddWord("nowhere", 0, 21);
            AddWord("nugget", 1, 50);
            AddWord("null", 0, 21);
            AddWord("nw", 0, 50);
            AddWord("off", 2, 8);
            AddWord("office", 0, 76);
            AddWord("oil", 1, 22);
            AddWord("on", 2, 7);
            AddWord("onward", 0, 7);
            AddWord("open", 2, 4);
            AddWord("opensesame", 3, 50);
            AddWord("oriental", 0, 72);
            AddWord("out", 0, 11);
            AddWord("outdoors", 0, 32);
            AddWord("outside", 0, 11);
            AddWord("over", 0, 41);
            AddWord("oyster", 1, 15);
            AddWord("passage", 0, 23);
            AddWord("pause", 2, 30);
            AddWord("pearl", 1, 61);
            AddWord("persian", 1, 62);
            AddWord("peruse", 2, 27);
            AddWord("pillow", 1, 10);
            AddWord("pirate", 1, 30);
            AddWord("pit", 0, 31);
            AddWord("placate", 2, 10);
            AddWord("plant", 1, 24);
            AddWord("platinum", 1, 60);
            AddWord("plover", 0, 71);
            AddWord("plugh", 0, 65);
            AddWord("pocus", 3, 50);
            AddWord("pottery", 1, 58);
            AddWord("pour", 2, 13);
            AddWord("proceed", 2, 11);
            AddWord("pyramid", 1, 60);
            AddWord("q", 2, 18);
            AddWord("quit", 2, 18);
            AddWord("rations", 1, 19);
            AddWord("read", 2, 27);
            AddWord("release", 2, 2);
            AddWord("reservoir", 0, 75);
            AddWord("retreat", 0, 8);
            AddWord("return", 0, 8);
            AddWord("right", 0, 37);
            AddWord("road", 0, 2);
            AddWord("rock", 0, 15);
            AddWord("rod", 1, 5);
            AddWord("room", 0, 59);
            AddWord("rub", 2, 16);
            AddWord("rug", 1, 62);
            AddWord("run", 2, 11);
            AddWord("s", 0, 46);
            AddWord("save", 2, 30);
            AddWord("say", 2, 3);
            AddWord("score", 2, 24);
            AddWord("se", 0, 48);
            AddWord("secret", 0, 66);
            AddWord("sesame", 3, 50);
            AddWord("shadow", 1, 27);
            AddWord("shake", 2, 9);
            AddWord("shard", 1, 58);
            AddWord("shatter", 2, 28);
            AddWord("shazam", 3, 50);
            AddWord("shell", 0, 74);
            AddWord("shit", 3, 79);
            AddWord("silver", 1, 52);
            AddWord("sing", 2, 3);
            AddWord("slab", 0, 61);
            AddWord("slit", 0, 60);
            AddWord("smash", 2, 28);
            AddWord("snake", 1, 11);
            AddWord("south", 0, 46);
            AddWord("spelunker", 1, 16);
            AddWord("spice", 1, 63);
            AddWord("spices", 1, 63);
            AddWord("stairs", 0, 10);
            AddWord("stalactite", 1, 26);
            AddWord("steal", 2, 1);
            AddWord("steps", 1, 7);
            AddWord("steps", 0, 34);
            AddWord("stop", 3, 139);
            AddWord("stream", 0, 14);
            AddWord("strike", 2, 12);
            AddWord("surface", 0, 20);
            AddWord("suspend", 2, 30);
            AddWord("sw", 0, 49);
            AddWord("swim", 3, 147);
            AddWord("swing", 2, 9);
            AddWord("tablet", 1, 13);
            AddWord("take", 2, 1);
            AddWord("tame", 2, 10);
            AddWord("throw", 2, 17);
            AddWord("toss", 2, 17);
            AddWord("tote", 2, 1);
            AddWord("touch", 0, 57);
            AddWord("travel", 2, 11);
            AddWord("treasure", 1, 55);
            AddWord("tree", 3, 64);
            AddWord("trees", 3, 64);
            AddWord("trident", 1, 57);
            AddWord("troll", 1, 33);
            AddWord("tunnel", 0, 23);
            AddWord("turn", 2, 11);
            AddWord("u", 0, 29);
            AddWord("unlock", 2, 4);
            AddWord("up", 0, 29);
            AddWord("upstream", 0, 5);
            AddWord("upward", 0, 29);
            AddWord("utter", 2, 3);
            AddWord("valley", 0, 9);
            AddWord("vase", 1, 58);
            AddWord("velvet", 1, 10);
            AddWord("vending", 1, 38);
            AddWord("view", 0, 28);
            AddWord("volcano", 1, 37);
            AddWord("w", 0, 44);
            AddWord("wake", 2, 29);
            AddWord("walk", 2, 11);
            AddWord("wall", 0, 53);
            AddWord("water", 1, 21);
            AddWord("wave", 2, 9);
            AddWord("west", 0, 44);
            AddWord("xyzzy", 0, 62);
            AddWord("y2", 0, 55);
        }

        private static void AddWord(string word, int type, int value)
        {
            WordEntry entry = new() { Word = word, Type = type, Value = value };
            WordList.Add(entry);
            Words[word] = entry;
        }

        /// <summary>
        /// Word types from ENGLISH.C: 0=motion, 1=object, 2=verb, 3=special message.
        /// </summary>
        public static class WordTypes
        {
            public const int Motion = 0;
            public const int Object = 1;
            public const int Verb = 2;
            public const int Special = 3;
        }

        public static bool AnalyzeWord(string word, out int type, out int value)
        {
            type = -1;
            value = -1;

            if (string.IsNullOrWhiteSpace(word))
                return false;

            string normalizedWord = word.ToLowerInvariant().Trim();
            if (!Words.TryGetValue(normalizedWord, out WordEntry? entry))
                return false;

            type = entry.Type;
            value = entry.Value;
            return true;
        }

        public static IEnumerable<string> GetMotionAndVerbWords()
        {
            foreach (WordEntry entry in WordList)
            {
                if (entry.Type == WordTypes.Motion || entry.Type == WordTypes.Verb)
                    yield return entry.Word;
            }
        }
    }
