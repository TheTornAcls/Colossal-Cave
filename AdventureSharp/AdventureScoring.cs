namespace AdventureSharp;

using System;

public class AdventureScoring
{
    private readonly AdventureGame _game;
    private readonly AdventureDatabase _db;

    public AdventureScoring(AdventureGame game, AdventureDatabase db)
    {
        this._game = game;
        this._db = db;
    }

    public int CalculateScore()
    {
        int score = 0;
        // 10 points per treasure in treasure room
        foreach (AdventureDatabase.GameObject obj in this._db.Objects)
        {
            string nameLower = obj.Name.ToLower();
            if (nameLower.Contains("treasure") || nameLower.Contains("chest") || nameLower.Contains("crystal") || nameLower.Contains("emerald") || nameLower.Contains("coins") || nameLower.Contains("pearl"))
            {
                if (this._game.place[obj.Id] == 6) // 6 = Treasure Room
                {
                    score += 10;
                }
            }
        }
        // 2 points per unique room visited
        for (int i = 0; i < AdventureConstants.MAXLOC; i++)
        {
            if (this._game.visited[i] != 0)
            {
                score += 2;
            }
        }
        // 1 point per object carried at end
        for (int i = 0; i < AdventureConstants.MAXOBJ; i++)
        {
            if (this._game.place[i] == -1)
            {
                score += 1;
            }
        }
        // Bonus for winning (closed=true)
        if (this._game.closed)
        {
            score += 25;
        }
        // Penalty for giving up
        if (this._game.gaveup)
        {
            score -= 10;
        }
        // Penalty for dying
        score -= this._game.numdie * 5;
        // Bonus for escaping after cave closed
        score += this._game.bonus;
        return score;
    }

    public void ShowScore()
    {
        int score = this.CalculateScore();
        int maxScore = 10 * 6 + 2 * 50 + 20 + 25 + 10; // Estimate: treasures + rooms + objects + win bonus + escape bonus
        Console.WriteLine($"Your score is: {score}");
        Console.WriteLine($"Maximum possible score: {maxScore}");
        if (score == maxScore)
        {
            Console.WriteLine("Congratulations! You have achieved the maximum score!");
        }
        else if (score > maxScore * 0.7)
        {
            Console.WriteLine("Great job! You're a master adventurer.");
        }
        else if (score > maxScore * 0.4)
        {
            Console.WriteLine("Not bad! Keep exploring for a higher score.");
        }
        else
        {
            Console.WriteLine("You have much more to discover!");
        }
    }

    public void EndGame(string scenario)
    {
        int score = this.CalculateScore();
        Console.WriteLine();
        switch (scenario)
        {
            case "win":
                Console.WriteLine("You have won! The cave closes behind you as you escape with your treasures.");
                break;
            case "lose":
                Console.WriteLine("You have lost. The cave closes and you are trapped forever.");
                break;
            case "quit":
                Console.WriteLine("You have quit the adventure. Maybe next time you'll go further!");
                break;
            case "death":
                Console.WriteLine("You have died in the cave. Your adventure ends here.");
                break;
            case "escape":
                Console.WriteLine("You escaped after the cave closed! Bonus points awarded.");
                break;
            default:
                Console.WriteLine("Game over.");
                break;
        }
        this.ShowScore();
        Console.WriteLine("Thank you for playing AdventureSharp!");
        Environment.Exit(0);
    }
}
