namespace AdventureSharp;

using System;
using System.IO;
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

    // File handles for the main text files
    public StreamReader? fd1;
    public StreamReader? fd2;
    public StreamReader? fd3;
    public StreamReader? fd4;

    public bool SaveFlag { get { return saveflg != 0; } set { saveflg = value ? 1 : 0; } }

    public void InitPlay()
    {
        turns = 0;
        Array.Copy(icond, cond, AdventureConstants.MAXLOC);
        Array.Copy(iplace, place, AdventureConstants.MAXOBJ);
        Array.Copy(ifixed, fixedObj, AdventureConstants.MAXOBJ);
        Array.Copy(iactmsg, actmsg, AdventureConstants.ACTMSG);
        Array.Clear(visited, 0, AdventureConstants.MAXLOC);
        Array.Clear(prop, 0, AdventureConstants.MAXOBJ);
        for (int i = 50; i < AdventureConstants.MAXOBJ; i++)
            prop[i] = -1;
        wzdark = false;
        closed = false;
        closing = false;
        holding = detail = 0;
        limit = 100;
        tally = 15;
        tally2 = 0;
        newloc = 1;
        loc = oldloc = oldloc2 = 2;
        knfloc = 0;
        chloc = 114;
        chloc2 = 140;
        Array.Copy(idloc, dloc, AdventureConstants.DWARFMAX);
        Array.Clear(odloc, 0, AdventureConstants.DWARFMAX);
        dkill = 0;
        Array.Clear(dseen, 0, AdventureConstants.DWARFMAX);
        clock1 = 30;
        clock2 = 50;
        panic = 0;
        bonus = 0;
        numdie = 0;
        daltloc = 18;
        lmwarn = 0;
        foobar = 0;
        dflag = 0;
        gaveup = false;
        saveflg = 0;
        hinttaken = 0;
        hintavail = 0; // HINT constant to be defined if needed
        Array.Clear(hintloc, 0, AdventureConstants.MAXHINT + 1);
        testbr = 2;
    }

    public void OpenTextFiles(string filePath)
    {
        fd1 = this.OpenFile(Path.Combine(filePath, AdventureConstants.FD1));
        fd2 = this.OpenFile(Path.Combine(filePath, AdventureConstants.FD2));
        fd3 = this.OpenFile(Path.Combine(filePath, AdventureConstants.FD3));
        fd4 = this.OpenFile(Path.Combine(filePath, AdventureConstants.FD4));
    }

    public StreamReader OpenFile(string fullPath)
    {
        if (!File.Exists(fullPath))
        {
            Console.WriteLine($"Sorry, I can't open {fullPath}");
            Environment.Exit(1);
        }
        return new StreamReader(fullPath);
    }

    public void CloseTextFiles()
    {
        fd1?.Close();
        fd2?.Close();
        fd3?.Close();
        fd4?.Close();
    }

    private GameState GetCurrentState()
    {
        return new GameState
        {
            cond = (short[])cond.Clone(),
            place = (short[])place.Clone(),
            fixedObj = (short[])fixedObj.Clone(),
            actmsg = (short[])actmsg.Clone(),
            visited = (short[])visited.Clone(),
            prop = (short[])prop.Clone(),
            dloc = (short[])dloc.Clone(),
            odloc = (short[])odloc.Clone(),
            dseen = (short[])dseen.Clone(),
            hintloc = (short[])hintloc.Clone(),
            turns = turns, loc = loc, oldloc = oldloc, oldloc2 = oldloc2, newloc = newloc,
            tally = tally, tally2 = tally2, limit = limit, lmwarn = lmwarn, holding = holding, detail = detail, knfloc = knfloc, clock1 = clock1, clock2 = clock2, panic = panic, bonus = bonus, numdie = numdie, daltloc = daltloc, dkill = dkill, dflag = dflag, saveflg = saveflg, hinttaken = hinttaken, hintavail = hintavail, testbr = testbr,
            wzdark = wzdark, closing = closing, closed = closed, gaveup = gaveup,
            chloc = chloc, chloc2 = chloc2, foobar = foobar
        };
    }

    private void SetCurrentState(GameState state)
    {
        Array.Copy(state.cond, cond, cond.Length);
        Array.Copy(state.place, place, place.Length);
        Array.Copy(state.fixedObj, fixedObj, fixedObj.Length);
        Array.Copy(state.actmsg, actmsg, actmsg.Length);
        Array.Copy(state.visited, visited, visited.Length);
        Array.Copy(state.prop, prop, prop.Length);
        Array.Copy(state.dloc, dloc, dloc.Length);
        Array.Copy(state.odloc, odloc, odloc.Length);
        Array.Copy(state.dseen, dseen, dseen.Length);
        Array.Copy(state.hintloc, hintloc, hintloc.Length);
        turns = state.turns; loc = state.loc; oldloc = state.oldloc; oldloc2 = state.oldloc2; newloc = state.newloc;
        tally = state.tally; tally2 = state.tally2; limit = state.limit; lmwarn = state.lmwarn; holding = state.holding; detail = state.detail; knfloc = state.knfloc; clock1 = state.clock1; clock2 = state.clock2; panic = state.panic; bonus = state.bonus; numdie = state.numdie; daltloc = state.daltloc; dkill = state.dkill; dflag = state.dflag; saveflg = state.saveflg; hinttaken = state.hinttaken; hintavail = state.hintavail; testbr = state.testbr;
        wzdark = state.wzdark; closing = state.closing; closed = state.closed; gaveup = state.gaveup;
        chloc = state.chloc; chloc2 = state.chloc2; foobar = state.foobar;
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
        // Basic command loop for demonstration
        Console.Write("\nWhat do you want to do? ");
        string? input = Console.ReadLine();
        if (string.IsNullOrWhiteSpace(input))
        {
            Console.WriteLine("Please enter a command.");
            return;
        }
        string command = input.Trim().ToLowerInvariant();

        // Simple command parsing (expand as needed)
        switch (command)
        {
            case "quit":
            case "exit":
                Console.WriteLine("Thanks for playing!");
                this.SaveFlag = true;
                break;
            case "look":
                Console.WriteLine("You look around. (Room description would go here.)");
                break;
            case "save":
                this.SaveGame();
                break;
            case "restore":
                this.RestoreGame();
                break;
            default:
                Console.WriteLine($"Unknown command: {command}");
                break;
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
            limit = 1000;
            hinttaken++;
        }
        else
        {
            limit = 330;
        }
        this.SaveFlag = false;
        Random rng = new Random(511); // Seed random
        while (!this.SaveFlag)
        {
            this.Turn();
        }
        if (this.SaveFlag)
        {
            this.SaveGame();
        }
    }
}
