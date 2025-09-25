namespace AdventureSharp;

using System;

public class AdventureVerbs
{
    private readonly AdventureGame _game;
    private readonly AdventureDatabase _db;

    public AdventureVerbs(AdventureGame game, AdventureDatabase db)
    {
        this._game = game;
        this._db = db;
    }

    public void Look()
    {
        string desc = this._game.GetCurrentRoomDescription();
        Console.WriteLine(desc);
        // Show objects in the room
        bool found = false;
        for (int i = 0; i < AdventureConstants.MAXOBJ; i++)
        {
            if (this._game.place[i] == this._game.loc)
            {
                AdventureDatabase.GameObject? obj = this._db.Objects.Find(o => o.Id == i);
                if (obj != null)
                {
                    Console.WriteLine($"You see a {obj.Name} here.");
                    found = true;
                }
            }
        }
        if (!found)
        {
            Console.WriteLine("There is nothing of interest here.");
        }
    }

    public void Inventory()
    {
        bool any = false;
        for (int i = 0; i < AdventureConstants.MAXOBJ; i++)
        {
            if (this._game.place[i] == -1)
            {
                AdventureDatabase.GameObject? obj = this._db.Objects.Find(o => o.Id == i);
                if (obj != null)
                {
                    Console.WriteLine(obj.Name);
                    any = true;
                }
            }
        }
        if (!any)
        {
            Console.WriteLine("You are carrying nothing.");
        }
    }

    public void Take(int objId)
    {
        AdventureDatabase.GameObject? obj = this._db.Objects.Find(o => o.Id == objId);
        if (obj == null)
        {
            Console.WriteLine("You don't see that here.");
            return;
        }
        if (this._game.place[objId] == -1)
        {
            Console.WriteLine($"You already have the {obj.Name}.");
            return;
        }
        if (this._game.place[objId] != this._game.loc)
        {
            Console.WriteLine($"You can't take the {obj.Name}.");
            return;
        }
        // Inventory limit
        int count = 0;
        for (int i = 0; i < AdventureConstants.MAXOBJ; i++)
        {
            if (this._game.place[i] == -1)
                count++;
        }
        if (count >= 7)
        {
            Console.WriteLine("You are carrying too much.");
            return;
        }
        this._game.place[objId] = (short)(-1);
        Console.WriteLine($"You take the {obj.Name}.");
    }

    public void Drop(int objId)
    {
        AdventureDatabase.GameObject? obj = this._db.Objects.Find(o => o.Id == objId);
        if (obj == null)
        {
            Console.WriteLine("You don't have that.");
            return;
        }
        if (this._game.place[objId] != -1)
        {
            Console.WriteLine($"You are not carrying the {obj.Name}.");
            return;
        }
        this._game.place[objId] = (short)this._game.loc;
        Console.WriteLine($"You drop the {obj.Name}.");
    }

    public void Examine(int objId)
    {
        AdventureDatabase.GameObject? obj = this._db.Objects.Find(o => o.Id == objId);
        if (obj != null)
        {
            Console.WriteLine(obj.Description);
        }
        else
        {
            Console.WriteLine("You see nothing special.");
        }
    }

    public void Light(int objId)
    {
        AdventureDatabase.GameObject? obj = this._db.Objects.Find(o => o.Id == objId);
        if (obj != null && obj.Name.ToLower().Contains("lamp"))
        {
            Console.WriteLine("You turn on the lamp. The cave is illuminated.");
        }
        else
        {
            Console.WriteLine("You can't light that.");
        }
    }

    public void Move(int directionVerb)
    {
        int currentLoc = this._game.loc;
        bool moved = false;
        foreach (AdventureDatabase.TravelEntry entry in this._db.TravelTable)
        {
            if (entry.FromLocation == currentLoc && entry.Verb == directionVerb)
            {
                this._game.loc = entry.ToLocation;
                this.Look();
                moved = true;
                break;
            }
        }
        if (!moved)
        {
            Console.WriteLine("You can't go that way.");
        }
    }

    public void Eat(int objId)
    {
        AdventureDatabase.GameObject? obj = this._db.Objects.Find(o => o.Id == objId);
        if (obj != null && obj.Name.ToLower().Contains("food"))
        {
            Console.WriteLine("You eat the food. Delicious!");
            this._game.place[objId] = 0;
        }
        else
        {
            Console.WriteLine("You can't eat that.");
        }
    }

    public void Read(int objId)
    {
        AdventureDatabase.GameObject? obj = this._db.Objects.Find(o => o.Id == objId);
        if (obj != null && obj.Name.ToLower().Contains("book"))
        {
            Console.WriteLine("You read the book. It contains mysterious runes.");
        }
        else
        {
            Console.WriteLine("You can't read that.");
        }
    }

    public void Wave(int objId)
    {
        AdventureDatabase.GameObject? obj = this._db.Objects.Find(o => o.Id == objId);
        if (obj != null && this._game.place[objId] == -1)
        {
            Console.WriteLine($"You wave the {obj.Name}. Nothing happens.");
        }
        else
        {
            Console.WriteLine("You need to be holding that to wave it.");
        }
    }

    public void Kill(int objId)
    {
        AdventureDatabase.GameObject? obj = this._db.Objects.Find(o => o.Id == objId);
        if (obj != null)
        {
            Console.WriteLine($"You attack the {obj.Name}, but nothing happens.");
        }
        else
        {
            Console.WriteLine("There's nothing to attack.");
        }
    }
}
