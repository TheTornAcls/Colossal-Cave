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
        // Show the current room description
        string desc = this._game.GetCurrentRoomDescription();
        Console.WriteLine(desc);
    }

    public void Inventory()
    {
        // List items carried by the player
        bool any = false;
        for (int i = 0; i < AdventureConstants.MAXOBJ; i++)
        {
            if (this._game.place[i] == -1) // Convention: -1 means carried
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
        // Try to take an object
        AdventureDatabase.GameObject? obj = this._db.Objects.Find(o => o.Id == objId);
        if (obj == null)
        {
            Console.WriteLine("You don't see that here.");
            return;
        }
        if (this._game.place[objId] == this._game.loc)
        {
            this._game.place[objId] = (short)(-1); // Carry
            Console.WriteLine($"You take the {obj.Name}.");
        }
        else if (this._game.place[objId] == -1)
        {
            Console.WriteLine($"You already have the {obj.Name}.");
        }
        else
        {
            Console.WriteLine($"You can't take the {obj.Name}.");
        }
    }

    public void Drop(int objId)
    {
        // Try to drop an object
        AdventureDatabase.GameObject? obj = this._db.Objects.Find(o => o.Id == objId);
        if (obj == null)
        {
            Console.WriteLine("You don't have that.");
            return;
        }
        if (this._game.place[objId] == -1)
        {
            this._game.place[objId] = (short)this._game.loc;
            Console.WriteLine($"You drop the {obj.Name}.");
        }
        else
        {
            Console.WriteLine($"You are not carrying the {obj.Name}.");
        }
    }

    public void Examine(int objId)
    {
        // Examine an object more closely
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
        // Light a lamp or similar object
        AdventureDatabase.GameObject? obj = this._db.Objects.Find(o => o.Id == objId);
        if (obj != null && obj.Name.ToLower().Contains("lamp"))
        {
            Console.WriteLine("You turn on the lamp. (Lamp logic would go here.)");
        }
        else
        {
            Console.WriteLine("You can't light that.");
        }
    }

    public void Move(int directionVerb)
    {
        // Move in a direction
        int currentLoc = this._game.loc;
        bool moved = false;
        foreach (AdventureDatabase.TravelEntry entry in this._db.TravelTable)
        {
            if (entry.FromLocation == currentLoc && entry.Verb == directionVerb)
            {
                this._game.loc = entry.ToLocation;
                Console.WriteLine(this._game.GetCurrentRoomDescription());
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
        // Eat food
        AdventureDatabase.GameObject? obj = this._db.Objects.Find(o => o.Id == objId);
        if (obj != null && obj.Name.ToLower().Contains("food"))
        {
            Console.WriteLine("You eat the food. (Eating logic would go here.)");
            this._game.place[objId] = 0; // Remove from inventory
        }
        else
        {
            Console.WriteLine("You can't eat that.");
        }
    }

    public void Read(int objId)
    {
        // Read a book or similar object
        AdventureDatabase.GameObject? obj = this._db.Objects.Find(o => o.Id == objId);
        if (obj != null && obj.Name.ToLower().Contains("book"))
        {
            Console.WriteLine("You read the book. (Book text would go here.)");
        }
        else
        {
            Console.WriteLine("You can't read that.");
        }
    }
}
