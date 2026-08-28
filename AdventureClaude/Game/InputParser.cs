namespace AdventureClaude.Game;

using System;
using AdventureClaude.Data;
using AdventureClaude.Models;

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
            return ParseInput(input, null, out verb, out objectId, out motion);
        }

        /// <summary>
        /// Parses a two-word player command using the original Adventure grammar.
        /// </summary>
        public bool ParseInput(string input, GameState? gameState, out int verb, out int objectId, out int motion)
        {
            verb = 0;
            objectId = 0;
            motion = 0;

            if (string.IsNullOrWhiteSpace(input))
                return false;

            string[] words = GetWords(input);
            string word1 = words.Length > 0 ? words[0] : string.Empty;
            string word2 = words.Length > 1 ? words[1] : string.Empty;
            if (gameState != null)
            {
                gameState.Command.Word1 = word1;
                gameState.Command.Word2 = word2;
            }

            if (string.IsNullOrEmpty(word1))
                return false;

            if (!AdventureData.AnalyzeWord(word1, out int type1, out int val1))
            {
                Console.WriteLine("I don't know that word.");
                return false;
            }

            if (type1 == Vocabulary.WordTypes.Verb && val1 == GameConstants.Say)
            {
                verb = GameConstants.Say;
                objectId = 1;
                return true;
            }

            int type2 = -1, val2 = -1;

            // Analyze second word if present
            if (!string.IsNullOrEmpty(word2))
            {
                if (!AdventureData.AnalyzeWord(word2, out type2, out val2))
                {
                    Console.WriteLine(AdventureData.Message(13));
                    return false;
                }
            }

            return AnalyzeWordTypes(
                type1,
                val1,
                type2,
                val2,
                gameState,
                out verb,
                out objectId,
                out motion);
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

            string[] allWords = input.ToLowerInvariant().Split(new char[] { ' ', '\t' },
                StringSplitOptions.RemoveEmptyEntries);

            string[] result = new string[Math.Min(2, allWords.Length)];
            for (int i = 0; i < result.Length; i++)
            {
                result[i] = allWords[i].Length >= GameConstants.MaxWordSize
                    ? allWords[i][..(GameConstants.MaxWordSize - 1)]
                    : allWords[i];
            }

            return result;
        }

        /// <summary>
        /// Analyzes word types and determines appropriate verb, object, and motion.
        /// Converted from the grammar analysis logic in english().
        /// </summary>
        private bool AnalyzeWordTypes(int type1, int val1, int type2, int val2, GameState? gameState,
                                    out int verb, out int objectId, out int motion)
        {
            verb = 0;
            objectId = 0;
            motion = 0;

            if (type1 == Vocabulary.WordTypes.Special &&
                type2 == Vocabulary.WordTypes.Special &&
                val1 == GameConstants.Help &&
                val2 == GameConstants.Help)
            {
                ShowKnownWords();
                return false;
            }

            if (type1 == Vocabulary.WordTypes.Special)
            {
                Console.WriteLine(AdventureData.Message(val1));
                return false;
            }

            if (type2 == Vocabulary.WordTypes.Special)
            {
                Console.WriteLine(AdventureData.Message(val2));
                return false;
            }

            if (type1 == Vocabulary.WordTypes.Motion)
            {
                if (type2 == Vocabulary.WordTypes.Motion)
                {
                    Console.WriteLine("bad grammar...");
                    return false;
                }

                motion = val1;
                return true;
            }

            if (type2 == Vocabulary.WordTypes.Motion)
            {
                motion = val2;
                return true;
            }

            if (type1 == Vocabulary.WordTypes.Object)
            {
                objectId = val1;
                if (type2 == Vocabulary.WordTypes.Verb)
                {
                    verb = val2;
                    return true;
                }

                if (type2 == Vocabulary.WordTypes.Object)
                {
                    if ((objectId == GameConstants.Water || objectId == GameConstants.Oil) &&
                        gameState != null &&
                        gameState.IsObjectHere(val2))
                    {
                        verb = GameConstants.Pour;
                        return true;
                    }

                    Console.WriteLine("bad grammar...");
                    return false;
                }

                return true;
            }

            if (type1 == Vocabulary.WordTypes.Verb)
            {
                verb = val1;
                if (type2 == Vocabulary.WordTypes.Object)
                {
                    objectId = val2;
                    return true;
                }

                if (type2 == Vocabulary.WordTypes.Verb)
                {
                    Console.WriteLine("bad grammar...");
                    return false;
                }

                return true;
            }

            Console.WriteLine("Fatal parser error.");
            return false;
        }

        private void ShowKnownWords()
        {
            int column = 0;
            foreach (string word in AdventureData.GetMotionAndVerbWords())
            {
                Console.Write($"{word,-12}");
                column++;
                if (column == 6)
                {
                    Console.WriteLine();
                    column = 0;
                }
            }

            if (column != 0)
                Console.WriteLine();
        }
    }
