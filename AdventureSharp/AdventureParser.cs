namespace AdventureSharp;

using System;
using System.Collections.Generic;

public class AdventureParser
{
    private readonly struct WordAction
    {
        public string Word { get; init; }
        public int Code { get; init; }
        public WordAction(string word, int code)
        {
            this.Word = word;
            this.Code = code;
        }
    }

    // Expanded vocabulary: Add all major verbs, objects, directions, and synonyms from original Adventure
    private readonly List<WordAction> _vocabulary = new List<WordAction>
    {
        // Directions
        new WordAction("north", 45), new WordAction("n", 45),
        new WordAction("south", 46), new WordAction("s", 46),
        new WordAction("east", 43), new WordAction("e", 43),
        new WordAction("west", 44), new WordAction("w", 44),
        new WordAction("up", 29), new WordAction("u", 29),
        new WordAction("down", 30), new WordAction("d", 30),
        new WordAction("in", 31), new WordAction("out", 32),
        new WordAction("enter", 31), new WordAction("leave", 32),
        // Verbs
        new WordAction("look", 2057), new WordAction("l", 2057),
        new WordAction("inventory", 2020), new WordAction("i", 2020),
        new WordAction("take", 2001), new WordAction("get", 2001),
        new WordAction("drop", 2002), new WordAction("put", 2002),
        new WordAction("examine", 2003), new WordAction("x", 2003),
        new WordAction("light", 2004), new WordAction("wave", 2005),
        new WordAction("kill", 2006), new WordAction("attack", 2006),
        new WordAction("score", 2019), new WordAction("eat", 2014),
        new WordAction("read", 2027), new WordAction("open", 2007),
        new WordAction("close", 2008), new WordAction("lock", 2009),
        new WordAction("unlock", 2010), new WordAction("pour", 2011),
        new WordAction("fill", 2012), new WordAction("throw", 2013),
        new WordAction("feed", 2015), new WordAction("find", 2016),
        new WordAction("blast", 2017), new WordAction("break", 2018),
        new WordAction("wake", 2021), new WordAction("say", 2022),
        new WordAction("on", 2023), new WordAction("off", 2024),
        new WordAction("rub", 2025), new WordAction("drink", 2026),
        new WordAction("help", 3051), new WordAction("quit", 2018), new WordAction("exit", 2018),
        new WordAction("suspend", 3052), new WordAction("hours", 3053),
        // Common objects (add more as needed)
        new WordAction("lamp", 1001), new WordAction("lantern", 1001),
        new WordAction("key", 1002), new WordAction("keys", 1002),
        new WordAction("rod", 1003), new WordAction("cage", 1004),
        new WordAction("bird", 1005), new WordAction("snake", 1006),
        new WordAction("tablet", 1007), new WordAction("magazine", 1008),
        new WordAction("pillow", 1009), new WordAction("bottle", 1010),
        new WordAction("water", 1011), new WordAction("oil", 1012),
        new WordAction("food", 1013), new WordAction("clam", 1014),
        new WordAction("oyster", 1015), new WordAction("door", 1016),
        new WordAction("grate", 1017), new WordAction("fissure", 1018),
        new WordAction("chest", 1019), new WordAction("crystal", 1020),
        // Add more objects and synonyms as needed
    };

    private static readonly AdventureParser.WordAction[] extraVerbs = new AdventureParser.WordAction[]
    {
        new ("examine", 2003),
        new ("light", 2004),
        new ("wave", 2005),
        new ("kill", 2006),
        new ("score", 2019),
        new ("eat", 2014),
        new ("read", 2027),
        new ("open", 2007),
        new ("close", 2008),
        new ("lock", 2009),
        new ("unlock", 2010),
        new ("pour", 2011),
        new ("fill", 2012),
        new ("throw", 2013),
        new ("feed", 2015),
        new ("find", 2016),
        new ("blast", 2017),
        new ("break", 2018),
        new ("wake", 2021),
        new ("say", 2022),
        new ("on", 2023),
        new ("off", 2024),
        new ("rub", 2025),
        new ("drink", 2026)
    };

    public AdventureParser()
    {
        foreach (WordAction wa in extraVerbs)
        {
            this._vocabulary.Add(wa);
        }
    }

    // Analyze method: Detects word type (0=motion, 1=object, 2=verb, 3=special)
    public bool Analyze(string word, out int type, out int value)
    {
        foreach (WordAction wa in this._vocabulary)
        {
            if (string.Equals(wa.Word, word, StringComparison.OrdinalIgnoreCase))
            {
                // Type detection: 0=motion, 1=object, 2=verb, 3=special
                if (wa.Code < 1000) type = 0; // motion
                else if (wa.Code < 2000) type = 1; // object
                else if (wa.Code < 3000) type = 2; // verb
                else type = 3; // special
                value = wa.Code % 1000;
                return true;
            }
        }
        type = -1;
        value = -1;
        return false;
    }

    // GetWords: Splits user input into two words, handles abbreviations
    public (string, string) GetWords()
    {
        Console.Write("> ");
        string? input = Console.ReadLine();
        if (string.IsNullOrWhiteSpace(input))
            return (string.Empty, string.Empty);
        string[] parts = input.Trim().ToLowerInvariant().Split(' ', 2, StringSplitOptions.RemoveEmptyEntries);
        string word1 = parts.Length > 0 ? parts[0] : string.Empty;
        string word2 = parts.Length > 1 ? parts[1] : string.Empty;
        // Handle abbreviations (e.g., "n" for "north")
        if (word1.Length == 1 && string.IsNullOrEmpty(word2))
        {
            switch (word1)
            {
                case "n": word1 = "north"; break;
                case "s": word1 = "south"; break;
                case "e": word1 = "east"; break;
                case "w": word1 = "west"; break;
                case "u": word1 = "up"; break;
                case "d": word1 = "down"; break;
                case "l": word1 = "look"; break;
                case "i": word1 = "inventory"; break;
            }
        }
        return (word1, word2);
    }

    // English: Parses two-word command, sets verb/object/motion, checks grammar
    public bool English(out int verb, out int obj, out int motion)
    {
        verb = 0;
        obj = 0;
        motion = 0;
        int type1 = -1, type2 = -1, val1 = -1, val2 = -1;
        string msg = "bad grammar...";
        (string word1, string word2) = this.GetWords();
        if (string.IsNullOrEmpty(word1))
            return false; // ignore whitespace
        if (!this.Analyze(word1, out type1, out val1))
            return false; // didn't know it

        // Special case: SAY
        if (type1 == 2 && val1 == 22) // 22 = SAY
        {
            verb = 22;
            obj = 1;
            return true;
        }

        if (!string.IsNullOrEmpty(word2))
        {
            if (!this.Analyze(word2, out type2, out val2))
                return false; // didn't know it
        }
        else
        {
            type2 = val2 = -1;
        }

        // Grammar checks and special cases
        if ((type1 == 3) && (type2 == 3) && (val1 == 51) && (val2 == 51)) // HELP HELP
        {
            Console.WriteLine("Word list: " + string.Join(", ", this._vocabulary));
            return false;
        }
        else if (type1 == 3)
        {
            Console.WriteLine($"Special: {val1}");
            return false;
        }
        else if (type2 == 3)
        {
            Console.WriteLine($"Special: {val2}");
            return false;
        }
        else if (type1 == 0)
        {
            if (type2 == 0)
            {
                Console.WriteLine(msg);
                return false;
            }
            else
            {
                motion = val1;
                return true;
            }
        }
        else if (type2 == 0)
        {
            motion = val2;
            return true;
        }
        else if (type1 == 1)
        {
            if (type2 == 1)
            {
                Console.WriteLine(msg);
                return false;
            }
            else if (type2 == 2)
            {
                obj = val1;
                verb = val2;
                return true;
            }
            else
            {
                Console.WriteLine(msg);
                return false;
            }
        }
        else if (type1 == 2)
        {
            verb = val1;
            if (type2 == 1)
            {
                obj = val2;
            }
            return true;
        }
        else
        {
            Console.WriteLine(msg);
            return false;
        }
    }
}
