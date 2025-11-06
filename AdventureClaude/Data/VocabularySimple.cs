using System.Collections.Generic;

namespace AdventureClaude.Data
{
    /// <summary>
    /// Contains the adventure game vocabulary.
    /// Converted from ADVWORD.H file.
    /// </summary>
    public static class VocabularySimple
    {
        /// <summary>
        /// Dictionary of all known words mapped to their types and values.
        /// </summary>
        public static readonly Dictionary<string, WordEntry> Words = new Dictionary<string, WordEntry>();

        static VocabularySimple()
        {
            InitializeWords();
        }

        private static void InitializeWords()
        {
            // Movement words
            Words["north"] = new WordEntry { Word = "north", Type = 1, Value = 1 };
            Words["n"] = new WordEntry { Word = "n", Type = 1, Value = 1 };
            Words["south"] = new WordEntry { Word = "south", Type = 1, Value = 2 };
            Words["s"] = new WordEntry { Word = "s", Type = 1, Value = 2 };
            Words["east"] = new WordEntry { Word = "east", Type = 1, Value = 3 };
            Words["e"] = new WordEntry { Word = "e", Type = 1, Value = 3 };
            Words["west"] = new WordEntry { Word = "west", Type = 1, Value = 4 };
            Words["w"] = new WordEntry { Word = "w", Type = 1, Value = 4 };
            Words["up"] = new WordEntry { Word = "up", Type = 1, Value = 5 };
            Words["u"] = new WordEntry { Word = "u", Type = 1, Value = 5 };
            Words["down"] = new WordEntry { Word = "down", Type = 1, Value = 6 };
            Words["d"] = new WordEntry { Word = "d", Type = 1, Value = 6 };
            Words["northeast"] = new WordEntry { Word = "northeast", Type = 1, Value = 7 };
            Words["ne"] = new WordEntry { Word = "ne", Type = 1, Value = 7 };
            Words["northwest"] = new WordEntry { Word = "northwest", Type = 1, Value = 8 };
            Words["nw"] = new WordEntry { Word = "nw", Type = 1, Value = 8 };
            Words["southeast"] = new WordEntry { Word = "southeast", Type = 1, Value = 9 };
            Words["se"] = new WordEntry { Word = "se", Type = 1, Value = 9 };
            Words["southwest"] = new WordEntry { Word = "southwest", Type = 1, Value = 10 };
            Words["sw"] = new WordEntry { Word = "sw", Type = 1, Value = 10 };

            // Common verbs
            Words["go"] = new WordEntry { Word = "go", Type = 2, Value = 1 };
            Words["get"] = new WordEntry { Word = "get", Type = 2, Value = 1 };
            Words["take"] = new WordEntry { Word = "take", Type = 2, Value = 1 };
            Words["drop"] = new WordEntry { Word = "drop", Type = 2, Value = 2 };
            Words["put"] = new WordEntry { Word = "put", Type = 2, Value = 2 };
            Words["open"] = new WordEntry { Word = "open", Type = 2, Value = 4 };
            Words["close"] = new WordEntry { Word = "close", Type = 2, Value = 5 };
            Words["quit"] = new WordEntry { Word = "quit", Type = 2, Value = 18 };
            Words["inventory"] = new WordEntry { Word = "inventory", Type = 2, Value = 20 };
            Words["i"] = new WordEntry { Word = "i", Type = 2, Value = 20 };
            Words["look"] = new WordEntry { Word = "look", Type = 2, Value = 57 };
            Words["l"] = new WordEntry { Word = "l", Type = 2, Value = 57 };
            Words["help"] = new WordEntry { Word = "help", Type = 2, Value = 51 };

            // Common objects
            Words["keys"] = new WordEntry { Word = "keys", Type = 3, Value = 1 };
            Words["lamp"] = new WordEntry { Word = "lamp", Type = 3, Value = 2 };
            Words["lantern"] = new WordEntry { Word = "lantern", Type = 3, Value = 2 };
            Words["grate"] = new WordEntry { Word = "grate", Type = 3, Value = 3 };
            Words["cage"] = new WordEntry { Word = "cage", Type = 3, Value = 4 };
            Words["rod"] = new WordEntry { Word = "rod", Type = 3, Value = 5 };
            Words["bird"] = new WordEntry { Word = "bird", Type = 3, Value = 8 };
            Words["food"] = new WordEntry { Word = "food", Type = 3, Value = 19 };
            Words["bottle"] = new WordEntry { Word = "bottle", Type = 3, Value = 20 };

            // Yes/No responses
            Words["yes"] = new WordEntry { Word = "yes", Type = 0, Value = 1 };
            Words["y"] = new WordEntry { Word = "y", Type = 0, Value = 1 };
            Words["no"] = new WordEntry { Word = "no", Type = 0, Value = 0 };
        }

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
}