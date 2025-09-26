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

    private readonly List<WordAction> _vocabulary = new()
    {
        new("look", 57),
        new("inventory", 2020),
        new("i", 2020),
        new("north", 45),
        new("n", 45),
        new("south", 46),
        new("s", 46),
        new("east", 43),
        new("e", 43),
        new("west", 44),
        new("w", 44),
        new("up", 29),
        new("u", 29),
        new("down", 30),
        new("d", 30),
        new("take", 2001),
        new("drop", 2002),
        new("save", 2030),
        new("restore", 2030),
        new("quit", 2018),
        new("exit", 2018),
        new("help", 3051),
        // ... add more as needed ...
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

    public bool Analyze(string word, out int type, out int value)
    {
        foreach (WordAction wa in this._vocabulary)
        {
            if (string.Equals(wa.Word, word, StringComparison.OrdinalIgnoreCase))
            {
                type = wa.Code / 1000;
                value = wa.Code % 1000;
                return true;
            }
        }
        type = -1;
        value = -1;
        return false;
    }

    public (string, string) GetWords()
    {
        Console.Write("> ");
        string? input = Console.ReadLine();
        if (string.IsNullOrWhiteSpace(input))
            return (string.Empty, string.Empty);
        string[] parts = input.Trim().ToLowerInvariant().Split(' ', 2, StringSplitOptions.RemoveEmptyEntries);
        string word1 = parts.Length > 0 ? parts[0] : string.Empty;
        string word2 = parts.Length > 1 ? parts[1] : string.Empty;
        return (word1, word2);
    }

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
