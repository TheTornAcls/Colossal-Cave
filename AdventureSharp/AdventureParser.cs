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
        new ("read", 2027)
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
        int type1, type2, val1, val2;
        (string word1, string word2) = this.GetWords();
        if (string.IsNullOrEmpty(word1))
            return false;
        if (!this.Analyze(word1, out type1, out val1))
            return false;
        if (!string.IsNullOrEmpty(word2))
        {
            if (!this.Analyze(word2, out type2, out val2))
                return false;
        }
        else
        {
            type2 = val2 = -1;
        }
        // Simple grammar: verb-object or motion
        if (type1 == 2)
        {
            verb = val1;
            if (type2 == 1)
                obj = val2;
        }
        else if (type1 == 0)
        {
            motion = val1;
        }
        else if (type1 == 1 && type2 == 2)
        {
            obj = val1;
            verb = val2;
        }
        else
        {
            return false;
        }
        return true;
    }
}
