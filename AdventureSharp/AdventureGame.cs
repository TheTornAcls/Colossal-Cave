namespace AdventureSharp;

using System;
using System.Text.Json;

public class GameState
{
    public short[] cond = new short[AdventureConstants.MAXLOC];
    public short[] place = new short[AdventureConstants.MAXOBJ];
    public short[] fixedObj = new short[AdventureConstants.MAXOBJ];
    public short[] actmsg = new short[AdventureConstants.ACTMSG];
    public short[] visited = new short[AdventureConstants.MAXLOC];
    public short[] prop = new short[AdventureConstants.MAXOBJ];
    public short[] dloc = new short[AdventureConstants.DWARFMAX];
    public short[] odloc = new short[AdventureConstants.DWARFMAX];
    public short[] dseen = new short[AdventureConstants.DWARFMAX];
    public short[] hintloc = new short[AdventureConstants.MAXHINT + 1];
    public int turns, loc, oldloc, oldloc2, newloc;
    public int tally, tally2, limit, lmwarn, holding, detail, knfloc, clock1, clock2, panic, bonus, numdie, daltloc, dkill, dflag, saveflg, hinttaken, hintavail, testbr;
    public bool wzdark, closing, closed, gaveup;
    public int chloc, chloc2, foobar;
}

public static class AdventureConstants
{
    public const int MAXLOC = 141;
    public const int MAXOBJ = 100;
    public const int ACTMSG = 33;
    public const int DWARFMAX = 7;
    public const int MAXHINT = 20;
    public const int WORDSIZE = 20;
    public const int MAXNAME = 32;
    // Add other constants as needed
    public const string FD1 = "advent1.txt";
    public const string FD2 = "advent2.txt";
    public const string FD3 = "advent3.txt";
    public const string FD4 = "advent4.txt";
    public const int RANDOM_SEED = 511;
}

public class AdventureGame
{
    // Static initialization arrays (from C code)
    private static readonly short[] icond = new short[AdventureConstants.MAXLOC]
    {
        0, 2053, 2049, 2053, 2053, 2049, 2049, 2053, 33, 1,
        1, 0, 0, 64, 0, 0, 16, 0, 0, 128,
        16, 16, 16, 0, 6, 0, 16, 0, 0, 0,
        0, 16, 16, 0, 0, 0, 0, 0, 4, 0,
        16, 0, 256, 256, 256, 256, 264, 264, 264, 256,
        256, 256, 256, 264, 256, 264, 0, 8, 0, 16,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        256, 256, 264, 0, 0, 8, 264, 256, 0, 16,
        16, 0, 0, 0, 0, 4, 0, 0, 0, 512,
        513, 0, 0, 0, 0, 0, 0, 0, 1024, 0,
        0, 0, 0, 4, 0, 1, 1, 0, 0, 0,
        0, 0, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0
    };
    private static readonly short[] iplace = new short[AdventureConstants.MAXOBJ]
    {
        0, 3, 3, 8, 10, 11, 0, 14, 13, 94,
        96, 19, 17, 101, 103, 0, 106, 0, 0, 3,
        3, 0, 0, 109, 25, 23, 111, 35, 0, 97,
        0, 119, 117, 117, 0, 130, 0, 126, 140, 0,
        96, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        18, 27, 28, 29, 30, 0, 92, 95, 97, 100,
        101, 0, 119, 127, 130, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    private static readonly short[] ifixed = new short[AdventureConstants.MAXOBJ]
    {
        0, 0, 0, 9, 0, 0, 0, 15, 0, -1,
        0, -1, 27, -1, 0, 0, 0, -1, 0, 0,
        0, 0, 0, -1, -1, 67, -1, 110, 0, -1,
        -1, 121, 122, 122, 0, -1, -1, -1, -1, 0,
        -1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 121, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    private static readonly short[] iactmsg = new short[AdventureConstants.ACTMSG]
    {
        0, 24, 29, 0, 33, 0, 33, 38, 38, 42,
        14, 43, 110, 29, 110, 73, 75, 29, 13, 59,
        59, 174, 109, 67, 13, 0, 90, 195, 146, 110,
        13, 13, 155
    };
    private static readonly short[] idloc = new short[AdventureConstants.DWARFMAX]
    {
        0, 19, 27, 33, 44, 64, 114
    };

    // Game state fields
    public short[] cond = new short[AdventureConstants.MAXLOC];
    public short[] place = new short[AdventureConstants.MAXOBJ];
    public short[] fixedObj = new short[AdventureConstants.MAXOBJ];
    public short[] actmsg = new short[AdventureConstants.ACTMSG];
    public short[] visited = new short[AdventureConstants.MAXLOC];
    public short[] prop = new short[AdventureConstants.MAXOBJ];
    public short[] dloc = new short[AdventureConstants.DWARFMAX];
    public short[] odloc = new short[AdventureConstants.DWARFMAX];
    public short[] dseen = new short[AdventureConstants.DWARFMAX];
    public short[] hintloc = new short[AdventureConstants.MAXHINT + 1];

    public int turns, loc, oldloc, oldloc2, newloc;
    public int tally, tally2, limit, lmwarn, holding, detail, knfloc, clock1, clock2, panic, bonus, numdie, daltloc, dkill, dflag, saveflg, hinttaken, hintavail, testbr;
    public bool wzdark, closing, closed, gaveup;
    public int chloc, chloc2, foobar;

    public bool SaveFlag { get { return saveflg != 0; } set { saveflg = value ? 1 : 0; } }

    // AdventureParser instance
    private readonly AdventureParser _parser = new();
    private readonly AdventureDatabase _db = new();
    private readonly AdventureVerbs _verbs;
    private readonly AdventureScoring _scoring;
    private readonly AdventureEvents _events;
    private readonly AdventurePuzzles _puzzles;
    private readonly AdventurePirate _pirate;

    public AdventureGame()
    {
        this._verbs = new AdventureVerbs(this, this._db);
        this._scoring = new AdventureScoring(this, this._db);
        this._events = new AdventureEvents(this, this._db);
        this._puzzles = new AdventurePuzzles(this, this._db);
        this._pirate = new AdventurePirate(this, this._db);
        this.LoadGameDataFromEmbedded();
        this.InitPlay();
    }

    public void InitPlay()
    {
        this.turns = 0;
        Array.Copy(icond, this.cond, AdventureConstants.MAXLOC);
        Array.Copy(iplace, this.place, AdventureConstants.MAXOBJ);
        Array.Copy(ifixed, this.fixedObj, AdventureConstants.MAXOBJ);
        Array.Copy(iactmsg, this.actmsg, AdventureConstants.ACTMSG);
        Array.Clear(this.visited, 0, AdventureConstants.MAXLOC);
        Array.Clear(this.prop, 0, AdventureConstants.MAXOBJ);
        for (int i = 50; i < AdventureConstants.MAXOBJ; i++)
            this.prop[i] = -1;
        this.wzdark = false;
        this.closed = false;
        this.closing = false;
        this.holding = this.detail = 0;
        this.limit = 100;
        this.tally = 15;
        this.tally2 = 0;
        this.newloc = 1;
        this.loc = this.oldloc = this.oldloc2 = 2;
        this.knfloc = 0;
        this.chloc = 114;
        this.chloc2 = 140;
        Array.Copy(idloc, this.dloc, AdventureConstants.DWARFMAX);
        Array.Clear(this.odloc, 0, AdventureConstants.DWARFMAX);
        this.dkill = 0;
        Array.Clear(this.dseen, 0, AdventureConstants.DWARFMAX);
        this.clock1 = 30;
        this.clock2 = 50;
        this.panic = 0;
        this.bonus = 0;
        this.numdie = 0;
        this.daltloc = 18;
        this.lmwarn = 0;
        this.foobar = 0;
        this.dflag = 0;
        this.gaveup = false;
        this.saveflg = 0;
        this.hinttaken = 0;
        this.hintavail = 0; // HINT constant to be defined if needed
        Array.Clear(this.hintloc, 0, AdventureConstants.MAXHINT + 1);
        this.testbr = 2;
    }

    private GameState GetCurrentState()
    {
        return new GameState
        {
            cond = (short[])this.cond.Clone(),
            place = (short[])this.place.Clone(),
            fixedObj = (short[])this.fixedObj.Clone(),
            actmsg = (short[])this.actmsg.Clone(),
            visited = (short[])this.visited.Clone(),
            prop = (short[])this.prop.Clone(),
            dloc = (short[])this.dloc.Clone(),
            odloc = (short[])this.odloc.Clone(),
            dseen = (short[])this.dseen.Clone(),
            hintloc = (short[])this.hintloc.Clone(),
            turns = this.turns, loc = this.loc, oldloc = this.oldloc, oldloc2 = this.oldloc2, newloc = this.newloc,
            tally = this.tally, tally2 = this.tally2, limit = this.limit, lmwarn = this.lmwarn, holding = this.holding, detail = this.detail, knfloc = this.knfloc, clock1 = this.clock1, clock2 = this.clock2, panic = this.panic, bonus = this.bonus, numdie = this.numdie, daltloc = this.daltloc, dkill = this.dkill, dflag = this.dflag, saveflg = this.saveflg, hinttaken = this.hinttaken, hintavail = this.hintavail, testbr = this.testbr,
            wzdark = this.wzdark, closing = this.closing, closed = this.closed, gaveup = this.gaveup,
            chloc = this.chloc, chloc2 = this.chloc2, foobar = this.foobar
        };
    }

    private void SetCurrentState(GameState state)
    {
        Array.Copy(state.cond, this.cond, this.cond.Length);
        Array.Copy(state.place, this.place, this.place.Length);
        Array.Copy(state.fixedObj, this.fixedObj, this.fixedObj.Length);
        Array.Copy(state.actmsg, this.actmsg, this.actmsg.Length);
        Array.Copy(state.visited, this.visited, this.visited.Length);
        Array.Copy(state.prop, this.prop, this.prop.Length);
        Array.Copy(state.dloc, this.dloc, this.dloc.Length);
        Array.Copy(state.odloc, this.odloc, this.odloc.Length);
        Array.Copy(state.dseen, this.dseen, this.dseen.Length);
        Array.Copy(state.hintloc, this.hintloc, this.hintloc.Length);
        this.turns = state.turns; this.loc = state.loc; this.oldloc = state.oldloc; this.oldloc2 = state.oldloc2; this.newloc = state.newloc;
        this.tally = state.tally; this.tally2 = state.tally2; this.limit = state.limit; this.lmwarn = state.lmwarn; this.holding = state.holding; this.detail = state.detail; this.knfloc = state.knfloc; this.clock1 = state.clock1; this.clock2 = state.clock2; this.panic = state.panic; this.bonus = state.bonus; this.numdie = state.numdie; this.daltloc = state.daltloc; this.dkill = state.dkill; this.dflag = state.dflag; this.saveflg = state.saveflg; this.hinttaken = state.hinttaken; this.hintavail = state.hintavail; this.testbr = state.testbr;
        this.wzdark = state.wzdark; this.closing = state.closing; this.closed = state.closed; this.gaveup = state.gaveup;
        this.chloc = state.chloc; this.chloc2 = state.chloc2; this.foobar = state.foobar;
    }

    public void SaveGame()
    {
        Console.Write("What do you want to name the saved game? ");
        string? filename = Console.ReadLine();
        if (string.IsNullOrWhiteSpace(filename))
        {
            Console.WriteLine("Invalid filename.");
            return;
        }
        if (filename.Length > 8)
            filename = filename.Substring(0, 8);
        filename += ".json";
        try
        {
            string json = JsonSerializer.Serialize(this.GetCurrentState());
            File.WriteAllText(filename, json);
            Console.WriteLine($"Game saved to {filename}.");
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error saving game: {ex.Message}");
        }
    }

    public void RestoreGame()
    {
        Console.Write("What is the name of the saved game? ");
        string? filename = Console.ReadLine();
        if (string.IsNullOrWhiteSpace(filename))
        {
            Console.WriteLine("Invalid filename.");
            return;
        }
        if (filename.Length > 8)
            filename = filename.Substring(0, 8);
        filename += ".json";
        try
        {
            string json = File.ReadAllText(filename);
            GameState? state = JsonSerializer.Deserialize<GameState>(json);
            if (state != null)
            {
                this.SetCurrentState(state);
                Console.WriteLine($"Game restored from {filename}.");
            }
            else
            {
                Console.WriteLine("Failed to restore game state.");
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error restoring game: {ex.Message}");
        }
    }

    public bool Yes(int question, int yesResponse, int noResponse)
    {
        // Placeholder: ask the user a yes/no question
        Console.Write($"Question {question}: (y/n)? ");
        string? input = Console.ReadLine();
        if (!string.IsNullOrEmpty(input) && (input.StartsWith("y", StringComparison.OrdinalIgnoreCase)))
        {
            return true;
        }
        return false;
    }

    public void Turn()
    {
        int verb, obj, motion;
        if (!this._parser.English(out verb, out obj, out motion))
        {
            Console.WriteLine("I don't understand that.");
            return;
        }
        // Dispatch based on parser output
        if (verb != 0)
        {
            switch (verb)
            {
                case 18: // quit
                    Console.WriteLine("Thanks for playing!");
                    this.SaveFlag = true;
                    break;
                case 57: // look
                    this._verbs.Look();
                    break;
                case 2020: // inventory
                    this._verbs.Inventory();
                    break;
                case 2030: // save/restore
                    if (obj == 0)
                        this.SaveGame();
                    else
                        this.RestoreGame();
                    break;
                case 3051: // help
                    Console.WriteLine("Available commands: look, inventory (i), save, restore, quit (exit), help, north, south, east, west, up, down, take, drop, examine, light, eat, read, wave, kill, open, close, lock, unlock, pour, fill, throw, feed, find, blast, break, wake, say, on, off, rub, drink");
                    break;
                case 2001: this._verbs.Take(obj); break;
                case 2002: this._verbs.Drop(obj); break;
                case 2003: this._verbs.Examine(obj); break;
                case 2004: this._verbs.Light(obj); break;
                case 2005: this._verbs.Wave(obj); break;
                case 2006: this._verbs.Kill(obj); break;
                case 2007: this._verbs.Open(obj); break;
                case 2008: this._verbs.Close(obj); break;
                case 2009: this._verbs.Lock(obj); break;
                case 2010: this._verbs.Unlock(obj); break;
                case 2011: this._verbs.Pour(obj); break;
                case 2012: this._verbs.Fill(obj); break;
                case 2013: this._verbs.Throw(obj); break;
                case 2014: this._verbs.Eat(obj); break;
                case 2015: this._verbs.Feed(obj); break;
                case 2016: this._verbs.Find(obj); break;
                case 2017: this._verbs.Blast(obj); break;
                case 2018: this._verbs.Break(obj); break;
                case 2019: this._scoring.ShowScore(); break;
                case 2021: this._verbs.Wake(obj); break;
                case 2022: this._verbs.Say(obj); break;
                case 2023: this._verbs.On(obj); break;
                case 2024: this._verbs.Off(obj); break;
                case 2025: this._verbs.Rub(obj); break;
                case 2026: this._verbs.Drink(obj); break;
                case 2027: this._verbs.Read(obj); break;
                default:
                    Console.WriteLine($"That verb is not implemented yet. (verb={verb}, obj={obj}, motion={motion})");
                    break;
            }
        }
        else if (motion != 0)
        {
            this._verbs.Move(motion);
        }
        else
        {
            Console.WriteLine("I don't understand that.");
        }
    }

    public void MainGameLoop(bool restoreRequested)
    {
        if (restoreRequested)
        {
            this.RestoreGame();
        }
        else if (this.Yes(65, 1, 0))
        {
            this.limit = 1000;
            this.hinttaken++;
        }
        else
        {
            this.limit = 330;
        }
        this.SaveFlag = false;
        Random rng = new(AdventureConstants.RANDOM_SEED); // Seed random
        this.ShowWelcomeScreen();
        while (!this.SaveFlag)
        {
            this.Turn();
            this._events.ProcessTurnEvents();
            this._pirate.ProcessPirateEvents();
            this._puzzles.ProcessPuzzles();
            this.turns++;
            // Endgame scenarios
            if (this.closed && this.loc == 1)
            {
                this._scoring.EndGame("win");
            }
            if (this.closed && this.loc == 6)
            {
                this._scoring.EndGame("lose");
            }
            if (this.gaveup)
            {
                this._scoring.EndGame("quit");
            }
            if (this.numdie > 0)
            {
                this._scoring.EndGame("death");
            }
        }
        if (this.SaveFlag)
        {
            this.SaveGame();
        }
    }

    public void LoadGameDataFromEmbedded()
    {
        this._db.LoadGameDataFromEmbedded();
    }

    public string GetCurrentRoomDescription()
    {
        AdventureDatabase.Room? room = this._db.Rooms.Find(r => r.Id == this.loc);
        return room != null ? room.Description : "You are nowhere.";
    }

    public void ShowWelcomeScreen()
    {
        Console.WriteLine("\n\t\tWelcome to ADVENTURE!\n");
        Console.WriteLine("\nOriginal development by Willie Crowther.");
        Console.WriteLine("Major features added by Don Woods.");
        Console.WriteLine("Conversion to C# by AdventureSharp Team.\n");
        Console.WriteLine("Would you like instructions? (y/n)");
        string? input = Console.ReadLine();
        if (!string.IsNullOrEmpty(input) && input.Trim().ToLower().StartsWith("y"))
        {
            ShowInstructions();
        }
    }

    private void ShowInstructions()
    {
        Console.WriteLine("\nSomewhere nearby is Colossal Cave, where others have found fortunes in treasure and gold, though it is rumored that some who enter are never seen again. Magic is said to work in the cave. I will be your eyes and hands. Direct me with commands of 1 or 2 words. (Should you get stuck, type 'help' for some general hints. For information on how to end your adventure, etc., type 'info'.)\n");
    }
}
