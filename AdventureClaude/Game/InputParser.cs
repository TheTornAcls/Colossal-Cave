using System;
using AdventureClaude.Data;
using AdventureClaude.Models;

namespace AdventureClaude.Game
{
    /// <summary>
    /// Handles parsing and analysis of player input.
    /// Converted from ENGLISH.C functions.
    /// </summary>
    public class InputParser
    {
        public InputParser()
        {
        }

        /// <summary>
        /// Parses a two-word player command and sets verb, object, and motion variables.
        /// Converted from the english() function in ENGLISH.C.
        /// </summary>
        /// <param name="input">The input string from the player</param>
        /// <param name="verb">Output: the parsed verb</param>
        /// <param name="objectId">Output: the parsed object ID</param>
        /// <param name="motion">Output: the parsed motion</param>
        /// <returns>True if the input was successfully parsed, false otherwise</returns>
        public bool ParseInput(string input, out int verb, out int objectId, out int motion)
        {
            verb = 0;
            objectId = 0;
            motion = 0;

            if (string.IsNullOrWhiteSpace(input))
                return false;

            string[] words = GetWords(input);
            string word1 = words.Length > 0 ? words[0] : string.Empty;
            string word2 = words.Length > 1 ? words[1] : string.Empty;

            if (string.IsNullOrEmpty(word1))
                return false;

            // Analyze first word
            if (!VocabularySimple.AnalyzeWord(word1, out int type1, out int val1))
            {
                Console.WriteLine($"I don't know the word \"{word1}\".");
                return false;
            }

            // Handle SAY command specially
            if (type1 == VocabularySimple.WordTypes.Verb && val1 == GameConstants.Say)
            {
                verb = GameConstants.Say;
                objectId = 1;
                return true;
            }

            int type2 = -1, val2 = -1;

            // Analyze second word if present
            if (!string.IsNullOrEmpty(word2))
            {
                if (!VocabularySimple.AnalyzeWord(word2, out type2, out val2))
                {
                    Console.WriteLine($"I don't know the word \"{word2}\".");
                    return false;
                }
            }

            // Determine verb, object, and motion based on word types
            return AnalyzeWordTypes(type1, val1, type2, val2, out verb, out objectId, out motion);
        }

        /// <summary>
        /// Splits input into individual words.
        /// Converted from getwords() function.
        /// </summary>
        /// <param name="input">The input string</param>
        /// <returns>Array of words</returns>
        private string[] GetWords(string input)
        {
            if (string.IsNullOrWhiteSpace(input))
                return new string[0];

            // Convert to lowercase and split on whitespace, taking only first two words
            string[] allWords = input.ToLowerInvariant().Split(new char[] { ' ', '\t' }, 
                StringSplitOptions.RemoveEmptyEntries);

            string[] result = new string[Math.Min(2, allWords.Length)];
            for (int i = 0; i < result.Length; i++)
            {
                result[i] = allWords[i];
            }

            return result;
        }

        /// <summary>
        /// Analyzes word types and determines appropriate verb, object, and motion.
        /// Converted from the grammar analysis logic in english().
        /// </summary>
        private bool AnalyzeWordTypes(int type1, int val1, int type2, int val2, 
                                    out int verb, out int objectId, out int motion)
        {
            verb = 0;
            objectId = 0;
            motion = 0;

            // Single word commands
            if (type2 == -1)
            {
                switch (type1)
                {
                    case VocabularySimple.WordTypes.Motion:
                        motion = val1;
                        return true;
                    case VocabularySimple.WordTypes.Verb:
                        verb = val1;
                        return true;
                    case VocabularySimple.WordTypes.Object:
                        Console.WriteLine("What do you want to do with it?");
                        return false;
                    default:
                        Console.WriteLine("I don't understand that!");
                        return false;
                }
            }

            // Two word commands - analyze combinations
            if (IsValidTwoWordCombination(type1, val1, type2, val2, out verb, out objectId, out motion))
            {
                return true;
            }

            Console.WriteLine("Bad grammar...");
            return false;
        }

        /// <summary>
        /// Validates and processes two-word command combinations.
        /// </summary>
        private bool IsValidTwoWordCombination(int type1, int val1, int type2, int val2,
                                             out int verb, out int objectId, out int motion)
        {
            verb = 0;
            objectId = 0;
            motion = 0;

            // verb + object
            if (type1 == VocabularySimple.WordTypes.Verb && type2 == VocabularySimple.WordTypes.Object)
            {
                verb = val1;
                objectId = val2;
                return true;
            }

            // object + verb
            if (type1 == VocabularySimple.WordTypes.Object && type2 == VocabularySimple.WordTypes.Verb)
            {
                verb = val2;
                objectId = val1;
                return true;
            }

            // motion + object (like "west building")
            if (type1 == VocabularySimple.WordTypes.Motion && type2 == VocabularySimple.WordTypes.Object)
            {
                motion = val1;
                objectId = val2;
                return true;
            }

            // object + motion (like "building west")
            if (type1 == VocabularySimple.WordTypes.Object && type2 == VocabularySimple.WordTypes.Motion)
            {
                motion = val2;
                objectId = val1;
                return true;
            }

            // verb + motion (like "go east")
            if (type1 == VocabularySimple.WordTypes.Verb && type2 == VocabularySimple.WordTypes.Motion)
            {
                // If first word is "go", just use the motion
                if (val1 == 1) // "go" verb
                {
                    motion = val2;
                    return true;
                }
                verb = val1;
                motion = val2;
                return true;
            }

            // motion + verb (like "east go")
            if (type1 == VocabularySimple.WordTypes.Motion && type2 == VocabularySimple.WordTypes.Verb)
            {
                motion = val1;
                verb = val2;
                return true;
            }

            return false;
        }
    }
}