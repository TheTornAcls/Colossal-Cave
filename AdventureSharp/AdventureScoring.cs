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
            if (obj.Name.ToLower().Contains("treasure") || obj.Name.ToLower().Contains("chest") || obj.Name.ToLower().Contains("crystal"))
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
        return score;
    }

    public void ShowScore()
    {
        int score = this.CalculateScore();
        int maxScore = 10 * 3 + 2 * 6 + 9 + 25; // Example: treasures + rooms + objects + win bonus
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
}
