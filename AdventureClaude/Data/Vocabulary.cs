namespace AdventureClaude.Data;

using System.Collections.Generic;

/// <summary>
/// Represents a word entry in the adventure vocabulary.
/// </summary>
public class WordEntry
    {
        public string Word { get; set; } = string.Empty;
        public int Type { get; set; }  // 1=motion, 2=verb, 3=object, etc.
        public int Value { get; set; } // The encoded value from the original
    }

    /// <summary>
    /// Contains the adventure game vocabulary.
    /// Converted from ADVWORD.H file.
    /// </summary>
    public static class Vocabulary
    {
        /// <summary>
        /// Dictionary of all known words mapped to their types and values.
        /// </summary>
        public static readonly Dictionary<string, WordEntry> Words = new Dictionary<string, WordEntry>
        {
            {"?", new WordEntry { Word = "?", Type = 3, Value = 51 }},
            {"above", new WordEntry { Word = "above", Type = 1, Value = 29 }},
            {"abra", new WordEntry { Word = "abra", Type = 3, Value = 50 }},
            {"abracadabra", new WordEntry { Word = "abracadabra", Type = 3, Value = 50 }},
            {"across", new WordEntry { Word = "across", Type = 1, Value = 42 }},
            {"ascend", new WordEntry { Word = "ascend", Type = 1, Value = 29 }},
            {"attack", new WordEntry { Word = "attack", Type = 2, Value = 12 }},
            {"awkward", new WordEntry { Word = "awkward", Type = 1, Value = 26 }},
            {"axe", new WordEntry { Word = "axe", Type = 1, Value = 28 }},
            {"back", new WordEntry { Word = "back", Type = 1, Value = 8 }},
            {"barren", new WordEntry { Word = "barren", Type = 1, Value = 40 }},
            {"bars", new WordEntry { Word = "bars", Type = 1, Value = 52 }},
            {"batteries", new WordEntry { Word = "batteries", Type = 1, Value = 39 }},
            {"battery", new WordEntry { Word = "battery", Type = 1, Value = 39 }},
            {"beans", new WordEntry { Word = "beans", Type = 1, Value = 24 }},
            {"bear", new WordEntry { Word = "bear", Type = 1, Value = 35 }},
            {"bed", new WordEntry { Word = "bed", Type = 1, Value = 16 }},
            {"bedquilt", new WordEntry { Word = "bedquilt", Type = 1, Value = 70 }},
            {"bird", new WordEntry { Word = "bird", Type = 1, Value = 8 }},
            {"blast", new WordEntry { Word = "blast", Type = 2, Value = 23 }},
            {"blowup", new WordEntry { Word = "blowup", Type = 2, Value = 23 }},
            {"bottle", new WordEntry { Word = "bottle", Type = 1, Value = 20 }},
            {"box", new WordEntry { Word = "box", Type = 1, Value = 55 }},
            {"break", new WordEntry { Word = "break", Type = 2, Value = 28 }},
            {"brief", new WordEntry { Word = "brief", Type = 2, Value = 26 }},
            {"broken", new WordEntry { Word = "broken", Type = 1, Value = 54 }},
            {"building", new WordEntry { Word = "building", Type = 1, Value = 12 }},
            {"cage", new WordEntry { Word = "cage", Type = 1, Value = 4 }},
            {"calm", new WordEntry { Word = "calm", Type = 2, Value = 10 }},
            {"canyon", new WordEntry { Word = "canyon", Type = 1, Value = 25 }},
            {"capture", new WordEntry { Word = "capture", Type = 2, Value = 1 }},
            {"carpet", new WordEntry { Word = "carpet", Type = 1, Value = 40 }},
            {"carry", new WordEntry { Word = "carry", Type = 2, Value = 1 }},
            {"catch", new WordEntry { Word = "catch", Type = 2, Value = 1 }},
            {"cave", new WordEntry { Word = "cave", Type = 1, Value = 67 }},
            {"cavern", new WordEntry { Word = "cavern", Type = 1, Value = 73 }},
            {"chain", new WordEntry { Word = "chain", Type = 1, Value = 64 }},
            {"chant", new WordEntry { Word = "chant", Type = 2, Value = 3 }},
            {"chasm", new WordEntry { Word = "chasm", Type = 1, Value = 32 }},
            {"chest", new WordEntry { Word = "chest", Type = 1, Value = 55 }},
            
            // Movement words
            {"north", new WordEntry { Word = "north", Type = 1, Value = 1 }},
            {"n", new WordEntry { Word = "n", Type = 1, Value = 1 }},
            {"south", new WordEntry { Word = "south", Type = 1, Value = 2 }},
            {"s", new WordEntry { Word = "s", Type = 1, Value = 2 }},
            {"east", new WordEntry { Word = "east", Type = 1, Value = 3 }},
            {"e", new WordEntry { Word = "e", Type = 1, Value = 3 }},
            {"west", new WordEntry { Word = "west", Type = 1, Value = 4 }},
            {"w", new WordEntry { Word = "w", Type = 1, Value = 4 }},
            {"up", new WordEntry { Word = "up", Type = 1, Value = 5 }},
            {"u", new WordEntry { Word = "u", Type = 1, Value = 5 }},
            {"down", new WordEntry { Word = "down", Type = 1, Value = 6 }},
            {"d", new WordEntry { Word = "d", Type = 1, Value = 6 }},
            {"northeast", new WordEntry { Word = "northeast", Type = 1, Value = 7 }},
            {"ne", new WordEntry { Word = "ne", Type = 1, Value = 7 }},
            {"northwest", new WordEntry { Word = "northwest", Type = 1, Value = 8 }},
            {"nw", new WordEntry { Word = "nw", Type = 1, Value = 8 }},
            {"southeast", new WordEntry { Word = "southeast", Type = 1, Value = 9 }},
            {"se", new WordEntry { Word = "se", Type = 1, Value = 9 }},
            {"southwest", new WordEntry { Word = "southwest", Type = 1, Value = 10 }},
            {"sw", new WordEntry { Word = "sw", Type = 1, Value = 10 }},
            
            // Common verbs
            {"get", new WordEntry { Word = "get", Type = 2, Value = 1 }},
            {"take", new WordEntry { Word = "take", Type = 2, Value = 1 }},
            {"drop", new WordEntry { Word = "drop", Type = 2, Value = 2 }},
            {"put", new WordEntry { Word = "put", Type = 2, Value = 2 }},
            {"open", new WordEntry { Word = "open", Type = 2, Value = 4 }},
            {"close", new WordEntry { Word = "close", Type = 2, Value = 5 }},
            {"on", new WordEntry { Word = "on", Type = 2, Value = 6 }},
            {"off", new WordEntry { Word = "off", Type = 2, Value = 7 }},
            {"wave", new WordEntry { Word = "wave", Type = 2, Value = 8 }},
            {"kill", new WordEntry { Word = "kill", Type = 2, Value = 12 }},
            {"pour", new WordEntry { Word = "pour", Type = 2, Value = 13 }},
            {"eat", new WordEntry { Word = "eat", Type = 2, Value = 14 }},
            {"drink", new WordEntry { Word = "drink", Type = 2, Value = 15 }},
            {"rub", new WordEntry { Word = "rub", Type = 2, Value = 16 }},
            {"throw", new WordEntry { Word = "throw", Type = 2, Value = 17 }},
            {"quit", new WordEntry { Word = "quit", Type = 2, Value = 18 }},
            {"find", new WordEntry { Word = "find", Type = 2, Value = 19 }},
            {"inventory", new WordEntry { Word = "inventory", Type = 2, Value = 20 }},
            {"inv", new WordEntry { Word = "inv", Type = 2, Value = 20 }},
            {"feed", new WordEntry { Word = "feed", Type = 2, Value = 21 }},
            {"fill", new WordEntry { Word = "fill", Type = 2, Value = 22 }},
            {"blastverb", new WordEntry { Word = "blastverb", Type = 2, Value = 23 }},
            {"score", new WordEntry { Word = "score", Type = 2, Value = 24 }},
            {"fee", new WordEntry { Word = "fee", Type = 2, Value = 25 }},
            {"fie", new WordEntry { Word = "fie", Type = 2, Value = 25 }},
            {"foe", new WordEntry { Word = "foe", Type = 2, Value = 25 }},
            {"foo", new WordEntry { Word = "foo", Type = 2, Value = 25 }},
            {"fum", new WordEntry { Word = "fum", Type = 2, Value = 25 }},
            {"briefverb", new WordEntry { Word = "briefverb", Type = 2, Value = 26 }},
            {"read", new WordEntry { Word = "read", Type = 2, Value = 27 }},
            {"breakverb", new WordEntry { Word = "breakverb", Type = 2, Value = 28 }},
            {"wake", new WordEntry { Word = "wake", Type = 2, Value = 29 }},
            {"save", new WordEntry { Word = "save", Type = 2, Value = 30 }},
            {"restore", new WordEntry { Word = "restore", Type = 2, Value = 31 }},
            {"suspend", new WordEntry { Word = "suspend", Type = 2, Value = 30 }},
            {"hours", new WordEntry { Word = "hours", Type = 2, Value = 32 }},
            {"lock", new WordEntry { Word = "lock", Type = 2, Value = 33 }},
            {"unlock", new WordEntry { Word = "unlock", Type = 2, Value = 34 }},
            
            // Magic words
            {"xyzzy", new WordEntry { Word = "xyzzy", Type = 2, Value = 62 }},
            {"plugh", new WordEntry { Word = "plugh", Type = 2, Value = 65 }},
            {"plover", new WordEntry { Word = "plover", Type = 2, Value = 71 }},
            
            // Help and info
            {"help", new WordEntry { Word = "help", Type = 2, Value = 51 }},
            {"info", new WordEntry { Word = "info", Type = 2, Value = 142 }},
            {"look", new WordEntry { Word = "look", Type = 2, Value = 57 }},
            {"l", new WordEntry { Word = "l", Type = 2, Value = 57 }},
            
            // Common words that map to multiple purposes
            {"go", new WordEntry { Word = "go", Type = 2, Value = 1 }}, // Verb for "go direction"
            {"enter", new WordEntry { Word = "enter", Type = 2, Value = 1 }},
            {"exit", new WordEntry { Word = "exit", Type = 2, Value = 1 }},
            
            // Object versions of common words
            {"keys", new WordEntry { Word = "keys", Type = 3, Value = 1 }},
            {"lamp", new WordEntry { Word = "lamp", Type = 3, Value = 2 }},
            {"lantern", new WordEntry { Word = "lantern", Type = 3, Value = 2 }},
            {"grate", new WordEntry { Word = "grate", Type = 3, Value = 3 }},
            {"cage", new WordEntry { Word = "cage", Type = 3, Value = 4 }},
            {"rod", new WordEntry { Word = "rod", Type = 3, Value = 5 }},
            {"steps", new WordEntry { Word = "steps", Type = 3, Value = 7 }},
            {"bird", new WordEntry { Word = "bird", Type = 3, Value = 8 }},
            {"door", new WordEntry { Word = "door", Type = 3, Value = 9 }},
            {"pillow", new WordEntry { Word = "pillow", Type = 3, Value = 10 }},
            {"snake", new WordEntry { Word = "snake", Type = 3, Value = 11 }},
            {"fissure", new WordEntry { Word = "fissure", Type = 3, Value = 12 }},
            {"tablet", new WordEntry { Word = "tablet", Type = 3, Value = 13 }},
            {"clam", new WordEntry { Word = "clam", Type = 3, Value = 14 }},
            {"oyster", new WordEntry { Word = "oyster", Type = 3, Value = 15 }},
            {"magazine", new WordEntry { Word = "magazine", Type = 3, Value = 16 }},
            {"dwarf", new WordEntry { Word = "dwarf", Type = 3, Value = 17 }},
            {"knife", new WordEntry { Word = "knife", Type = 3, Value = 18 }},
            {"food", new WordEntry { Word = "food", Type = 3, Value = 19 }},
            {"bottle", new WordEntry { Word = "bottle", Type = 3, Value = 20 }},
            {"water", new WordEntry { Word = "water", Type = 3, Value = 21 }},
            {"oil", new WordEntry { Word = "oil", Type = 3, Value = 22 }},
            {"mirror", new WordEntry { Word = "mirror", Type = 3, Value = 23 }},
            {"plant", new WordEntry { Word = "plant", Type = 3, Value = 24 }},
            
            // Yes/No responses
            {"yes", new WordEntry { Word = "yes", Type = 0, Value = 1 }},
            {"y", new WordEntry { Word = "y", Type = 0, Value = 1 }},
            {"no", new WordEntry { Word = "no", Type = 0, Value = 0 }}
        };

        /// <summary>
        /// Word types enumeration.
        /// </summary>
        public static class WordTypes
        {
            public const int YesNo = 0;
            public const int Motion = 1;
            public const int Verb = 2;
            public const int Object = 3;
        }

        /// <summary>
        /// Analyzes a word and returns its type and value.
        /// </summary>
        /// <param name="word">The word to analyze</param>
        /// <param name="type">Output: the word type</param>
        /// <param name="value">Output: the word value</param>
        /// <returns>True if the word was found, false otherwise</returns>
        public static bool AnalyzeWord(string word, out int type, out int value)
        {
            type = -1;
            value = -1;
            
            if (string.IsNullOrWhiteSpace(word))
                return false;
                
            string normalizedWord = word.ToLowerInvariant().Trim();
            
            if (Words.TryGetValue(normalizedWord, out WordEntry? entry))
            {
                type = entry.Type;
                value = entry.Value;
                return true;
            }
            
            return false;
        }
    }