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
        // Example: 10 points per treasure in treasure room
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
        // Example: 2 points per unique room visited
        for (int i = 0; i < AdventureConstants.MAXLOC; i++)
        {
            if (this._game.visited[i] != 0)
            {
                score += 2;
            }
        }
        // Example: 1 point per object carried at end
        for (int i = 0; i < AdventureConstants.MAXOBJ; i++)
        {
            if (this._game.place[i] == -1)
            {
                score += 1;
            }
        }
        return score;
    }

    public void ShowScore()
    {
        int score = this.CalculateScore();
        Console.WriteLine($"Your score is: {score}");
    }
}
