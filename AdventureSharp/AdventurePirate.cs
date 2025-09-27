namespace AdventureSharp;

using System;

public class AdventurePirate
{
    private readonly AdventureGame _game;
    private readonly AdventureDatabase _db;
    private int _pirateLoc = 0;
    private bool _chestStolen = false;

    public AdventurePirate(AdventureGame game, AdventureDatabase db)
    {
        this._game = game;
        this._db = db;
    }

    // Call this at the end of each turn
    public void ProcessPirateEvents()
    {
        // Pirate appears after turn 20, if player has treasures and is in maze
        if (!_chestStolen && this._game.turns > 20 && IsInMaze(this._game.loc) && PlayerHasTreasure())
        {
            Console.WriteLine("Out from the shadows behind you pounces a bearded pirate! 'Har, har' he chortles, 'I'll just take all this booty and hide it away with me chest deep in the maze!'. He snatches your treasure and vanishes into the gloom.");
            this.StealTreasures();
            _chestStolen = true;
            _pirateLoc = this._game.loc;
        }
        // Pirate chest can be found in maze after theft
        if (_chestStolen && this._game.loc == _pirateLoc)
        {
            AdventureDatabase.GameObject chest = this._db.Objects.Find(o => o.Name.ToLower().Contains("chest"));
            if (chest != null && this._game.place[chest.Id] == 0)
            {
                this._game.place[chest.Id] = (short)this._game.loc;
                Console.WriteLine("You have found the pirate's treasure chest hidden here!");
            }
        }
    }

    private bool IsInMaze(int loc)
    {
        // Simple check: maze rooms are 90-110 (adjust as needed)
        return loc >= 90 && loc <= 110;
    }

    private bool PlayerHasTreasure()
    {
        foreach (AdventureDatabase.GameObject obj in this._db.Objects)
        {
            string nameLower = obj.Name.ToLower();
            if ((nameLower.Contains("treasure") || nameLower.Contains("chest") || nameLower.Contains("crystal") || nameLower.Contains("emerald") || nameLower.Contains("coins") || nameLower.Contains("pearl")) && this._game.place[obj.Id] == -1)
            {
                return true;
            }
        }
        return false;
    }

    private void StealTreasures()
    {
        foreach (AdventureDatabase.GameObject obj in this._db.Objects)
        {
            string nameLower = obj.Name.ToLower();
            if ((nameLower.Contains("treasure") || nameLower.Contains("chest") || nameLower.Contains("crystal") || nameLower.Contains("emerald") || nameLower.Contains("coins") || nameLower.Contains("pearl")) && this._game.place[obj.Id] == -1)
            {
                this._game.place[obj.Id] = 0; // Remove from inventory
            }
        }
    }
}
