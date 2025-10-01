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
        AdventureDatabase.GameObject obj = this._db.Objects.Find(o => o.Id == objId);
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
        // Special case: fixed objects
        if (this._game.fixedObj[objId] == this._game.loc)
        {
            Console.WriteLine($"The {obj.Name} is fixed in place and cannot be taken.");
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
        this._game.holding++;
        Console.WriteLine($"You take the {obj.Name}.");
    }

    public void Drop(int objId)
    {
        AdventureDatabase.GameObject obj = this._db.Objects.Find(o => o.Id == objId);
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
        this._game.holding--;
        Console.WriteLine($"You drop the {obj.Name}.");
        // Special case: dropping bird with snake present
        if (obj.Name.ToLower().Contains("bird") && this._db.Objects.Exists(o => o.Name.ToLower().Contains("snake") && this._game.place[o.Id] == this._game.loc))
        {
            Console.WriteLine("The bird attacks the snake and both disappear!");
            this._game.place[objId] = 0;
            AdventureDatabase.GameObject snakeObj = this._db.Objects.Find(o => o.Name.ToLower().Contains("snake"));
            if (snakeObj != null)
            {
                this._game.place[snakeObj.Id] = 0;
            }
        }
    }

    public void Combine(int objId1, int objId2)
    {
        AdventureDatabase.GameObject obj1 = this._db.Objects.Find(o => o.Id == objId1);
        AdventureDatabase.GameObject obj2 = this._db.Objects.Find(o => o.Id == objId2);
        if (obj1 == null || obj2 == null)
        {
            Console.WriteLine("You can't combine those.");
            return;
        }
        // Example: combine key and chest
        if (obj1.Name.ToLower().Contains("key") && obj2.Name.ToLower().Contains("chest") && this._game.place[objId1] == -1 && this._game.place[objId2] == this._game.loc)
        {
            Console.WriteLine("You unlock the chest with the key!");
            this._game.prop[objId2] = 1;
        }
        else
        {
            Console.WriteLine("Nothing happens.");
        }
    }

    public void Use(int objId)
    {
        AdventureDatabase.GameObject obj = this._db.Objects.Find(o => o.Id == objId);
        if (obj == null)
        {
            Console.WriteLine("You can't use that.");
            return;
        }
        if (this._game.place[objId] != -1)
        {
            Console.WriteLine($"You need to be carrying the {obj.Name} to use it.");
            return;
        }
        // Example: use lamp
        if (obj.Name.ToLower().Contains("lamp"))
        {
            Console.WriteLine("You turn on the lamp. The cave is illuminated.");
            this._game.wzdark = false;
        }
        else
        {
            Console.WriteLine("You can't use that here.");
        }
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
        for (int i = 0; i < this._db.TravelTable.Count; i++)
        {
            AdventureDatabase.TravelEntry entry = this._db.TravelTable[i];
            if (entry.FromLocation == currentLoc && (entry.Verb == directionVerb || entry.Verb == 0))
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

    public void Open(int objId)
    {
        AdventureDatabase.GameObject? obj = this._db.Objects.Find(o => o.Id == objId);
        if (obj == null)
        {
            Console.WriteLine("You don't see that here.");
            return;
        }
        if (obj.Name.ToLower().Contains("chest") && this._game.place[objId] == this._game.loc)
        {
            if (this._game.prop[objId] == 1)
                Console.WriteLine("The chest is already open.");
            else
                Console.WriteLine("It's locked. Maybe a key would help.");
        }
        else
        {
            Console.WriteLine($"You can't open the {obj.Name}.");
        }
    }

    public void Close(int objId)
    {
        AdventureDatabase.GameObject? obj = this._db.Objects.Find(o => o.Id == objId);
        if (obj == null)
        {
            Console.WriteLine("You don't see that here.");
            return;
        }
        if (obj.Name.ToLower().Contains("chest") && this._game.place[objId] == this._game.loc)
        {
            if (this._game.prop[objId] == 0)
                Console.WriteLine("The chest is already closed.");
            else
            {
                this._game.prop[objId] = 0;
                Console.WriteLine("You close the chest.");
            }
        }
        else
        {
            Console.WriteLine($"You can't close the {obj.Name}.");
        }
    }

    public void Lock(int objId)
    {
        AdventureDatabase.GameObject? obj = this._db.Objects.Find(o => o.Id == objId);
        if (obj == null)
        {
            Console.WriteLine("You don't see that here.");
            return;
        }
        if (obj.Name.ToLower().Contains("chest") && this._game.place[objId] == this._game.loc)
        {
            if (this._game.prop[objId] == 0)
                Console.WriteLine("The chest is already locked.");
            else
                Console.WriteLine("You need a key to lock the chest.");
        }
        else
        {
            Console.WriteLine($"You can't lock the {obj.Name}.");
        }
    }

    // Key object ID for chest operations
    private const int KEY_OBJECT_ID = 5; // The object ID for the key used to unlock/lock the chest

    public void Unlock(int objId)
    {
        AdventureDatabase.GameObject? obj = this._db.Objects.Find(o => o.Id == objId);
        if (obj == null)
        {
            Console.WriteLine("You don't see that here.");
            return;
        }
        if (obj.Name.ToLower().Contains("chest") && this._game.place[objId] == this._game.loc)
        {
            if (this._game.place[KEY_OBJECT_ID] == -1)
            {
                this._game.prop[objId] = 1;
                Console.WriteLine("You unlock and open the chest with the key!");
            }
            else
            {
                Console.WriteLine("You need a key to unlock the chest.");
            }
        }
        else
        {
            Console.WriteLine($"You can't unlock the {obj.Name}.");
        }
    }

    public void Pour(int objId)
    {
        AdventureDatabase.GameObject? obj = this._db.Objects.Find(o => o.Id == objId);
        if (obj != null && obj.Name.ToLower().Contains("food"))
        {
            Console.WriteLine("You pour out the food. Ants appear and quickly carry it away.");
            this._game.place[objId] = 0;
        }
        else
        {
            Console.WriteLine("You can't pour that.");
        }
    }

    public void Fill(int objId)
    {
        AdventureDatabase.GameObject? obj = this._db.Objects.Find(o => o.Id == objId);
        if (obj != null && obj.Name.ToLower().Contains("chest"))
        {
            Console.WriteLine("You try to fill the chest, but with what?");
        }
        else
        {
            Console.WriteLine("You can't fill that.");
        }
    }

    public void Throw(int objId)
    {
        AdventureDatabase.GameObject? obj = this._db.Objects.Find(o => o.Id == objId);
        if (obj != null && this._game.place[objId] == -1)
        {
            Console.WriteLine($"You throw the {obj.Name}. It lands nearby.");
            this._game.place[objId] = (short)this._game.loc;
        }
        else
        {
            Console.WriteLine("You need to be holding that to throw it.");
        }
    }

    // Food object ID for bear feeding operations
    private const int FOOD_OBJECT_ID = 3; // The object ID for the food used to feed the bear

    public void Feed(int objId)
    {
        AdventureDatabase.GameObject? obj = this._db.Objects.Find(o => o.Id == objId);
        if (obj != null && obj.Name.ToLower().Contains("bear"))
        {
            if (this._game.place[FOOD_OBJECT_ID] == -1)
            {
                Console.WriteLine("You feed the food to the bear. The bear seems happy!");
                this._game.prop[objId] = 1;
                this._game.place[FOOD_OBJECT_ID] = 0;
            }
            else
            {
                Console.WriteLine("You have nothing to feed the bear.");
            }
        }
        else
        {
            Console.WriteLine("You can't feed that.");
        }
    }

    public void Find(int objId)
    {
        AdventureDatabase.GameObject? obj = this._db.Objects.Find(o => o.Id == objId);
        if (obj != null)
        {
            if (this._game.place[objId] == -1)
                Console.WriteLine($"You are carrying the {obj.Name}.");
            else if (this._game.place[objId] == this._game.loc)
                Console.WriteLine($"You see the {obj.Name} here.");
            else
                Console.WriteLine($"You don't see the {obj.Name} here.");
        }
        else
        {
            Console.WriteLine("You can't find that.");
        }
    }

    public void Blast(int objId)
    {
        Console.WriteLine("A loud explosion echoes through the cave! (No effect.)");
    }

    public void Break(int objId)
    {
        AdventureDatabase.GameObject? obj = this._db.Objects.Find(o => o.Id == objId);
        if (obj != null && obj.Name.ToLower().Contains("chest") && this._game.place[objId] == this._game.loc)
        {
            Console.WriteLine("You smash the chest, but it's too sturdy to break.");
        }
        else
        {
            Console.WriteLine("You can't break that.");
        }
    }

    public void Wake(int objId)
    {
        AdventureDatabase.GameObject? obj = this._db.Objects.Find(o => o.Id == objId);
        if (obj != null && obj.Name.ToLower().Contains("bear"))
        {
            Console.WriteLine("You try to wake the bear. It grumbles and rolls over.");
        }
        else
        {
            Console.WriteLine("You can't wake that.");
        }
    }

    public void Say(int objId)
    {
        Console.WriteLine("You say some magic words, but nothing happens.");
    }

    public void On(int objId)
    {
        AdventureDatabase.GameObject? obj = this._db.Objects.Find(o => o.Id == objId);
        if (obj != null && obj.Name.ToLower().Contains("lamp"))
        {
            Console.WriteLine("You turn on the lamp. The cave is illuminated.");
            this._game.wzdark = false;
        }
        else
        {
            Console.WriteLine("You can't turn that on.");
        }
    }

    public void Off(int objId)
    {
        AdventureDatabase.GameObject? obj = this._db.Objects.Find(o => o.Id == objId);
        if (obj != null && obj.Name.ToLower().Contains("lamp"))
        {
            Console.WriteLine("You turn off the lamp. The cave grows dark.");
            this._game.wzdark = true;
        }
        else
        {
            Console.WriteLine("You can't turn that off.");
        }
    }

    public void Rub(int objId)
    {
        AdventureDatabase.GameObject? obj = this._db.Objects.Find(o => o.Id == objId);
        if (obj != null && obj.Name.ToLower().Contains("lamp"))
        {
            Console.WriteLine("You rub the lamp, but no genie appears.");
        }
        else
        {
            Console.WriteLine("You can't rub that.");
        }
    }

    public void Drink(int objId)
    {
        AdventureDatabase.GameObject? obj = this._db.Objects.Find(o => o.Id == objId);
        if (obj != null && obj.Name.ToLower().Contains("water"))
        {
            Console.WriteLine("You drink the water. Refreshing!");
            this._game.place[objId] = 0;
        }
        else
        {
            Console.WriteLine("You can't drink that.");
        }
    }
} // End of AdventureVerbs class
