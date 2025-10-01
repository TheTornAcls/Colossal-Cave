namespace AdventureSharp;

using System;

public class AdventurePuzzles
{
    private readonly AdventureGame _game;
    private readonly AdventureDatabase _db;

    // Key object ID for treasure room puzzle
    private const int PUZZLE_KEY_OBJECT_ID = 5; // The object ID for the key used to unlock the treasure chest in the treasure room
    // Chest object ID for treasure room puzzle
    private const int PUZZLE_CHEST_OBJECT_ID = 6; // The object ID for the treasure chest in the treasure room

    public AdventurePuzzles(AdventureGame game, AdventureDatabase db)
    {
        this._game = game;
        this._db = db;
    }

    // Call this at the end of each turn
    public void ProcessPuzzles()
    {
        // Puzzle 1: Unlock treasure room with key
        if (this._game.loc == 6)
        {
            if (this._game.place[PUZZLE_KEY_OBJECT_ID] == -1 && this._game.place[PUZZLE_CHEST_OBJECT_ID] == 6 && this._game.prop[PUZZLE_CHEST_OBJECT_ID] == 0)
            {
                Console.WriteLine("You use the rusty key to unlock the treasure chest!");
                Console.WriteLine("The chest opens, revealing a pile of gold coins!");
                this._game.prop[PUZZLE_CHEST_OBJECT_ID] = 1; // Mark chest as opened
                this._game.tally += 1; // Award progress
            }
            else if (this._game.place[PUZZLE_CHEST_OBJECT_ID] == 6 && this._game.prop[PUZZLE_CHEST_OBJECT_ID] == 0)
            {
                Console.WriteLine("A locked treasure chest sits here. You need a key to open it.");
            }
        }
        // Puzzle 2: Read book in tunnel to reveal secret
        if (this._game.loc == 2)
        {
            int bookId = 2;
            if (this._game.place[bookId] == -1 && this._game.visited[100] == 0)
            {
                Console.WriteLine("As you read the ancient book, a secret passage opens to the east!");
                this._game.visited[100] = 1;
                this._db.TravelTable.Add(new AdventureDatabase.TravelEntry { FromLocation = 2, ToLocation = 7, Verb = 43, Condition = 0 });
                this._db.Rooms.Add(new AdventureDatabase.Room { Id = 7, Name = "Secret Alcove", Description = "You have discovered a hidden alcove filled with mysterious artifacts.", ShortDescription = "In a secret alcove." });
                this._game.tally += 1;
            }
        }
        // Puzzle 3: Place crystal in crystal chamber
        if (this._game.loc == 4)
        {
            int crystalId = 4;
            if (this._game.place[crystalId] == 4 && this._game.visited[101] == 0)
            {
                Console.WriteLine("You place the crystal on a pedestal. The room glows and a hidden door opens north!");
                this._game.visited[101] = 1;
                this._db.TravelTable.Add(new AdventureDatabase.TravelEntry { FromLocation = 4, ToLocation = 8, Verb = 45, Condition = 0 });
                this._db.Rooms.Add(new AdventureDatabase.Room { Id = 8, Name = "Glowing Hall", Description = "A hall illuminated by the power of the crystal.", ShortDescription = "In a glowing hall." });
                this._game.tally += 1;
            }
        }
        // Puzzle 4: Feed food to bear in narrow passage
        if (this._game.loc == 5)
        {
            int foodId = 3;
            int bearId = 10; // Assume bear is object 10
            if (this._game.place[foodId] == 5 && this._game.place.Length > bearId && this._game.place[bearId] == 5 && this._game.prop[bearId] == 0)
            {
                Console.WriteLine("You feed the food to the bear. The bear becomes friendly and lets you pass!");
                this._game.prop[bearId] = 1; // Bear is now friendly
                this._game.tally += 1;
            }
        }
        // Puzzle 5: Use lamp in dark tunnel
        if (this._game.loc == 2)
        {
            int lampId = 1;
            if (this._game.place[lampId] == -1 && this._game.wzdark)
            {
                Console.WriteLine("You turn on the lamp, illuminating the tunnel. You can see a safe path forward.");
                this._game.wzdark = false;
                this._game.tally += 1;
            }
        }
        // Puzzle 6: Escape after cave closes
        if (this._game.closed && this._game.loc == 1 && this._game.bonus < 20)
        {
            Console.WriteLine("You have escaped the cave after it closed! You win a special bonus.");
            this._game.bonus += 10;
            this._game.tally += 1;
        }
        // Puzzle 7: Use sword in glowing hall
        if (this._game.loc == 8)
        {
            int swordId = 7;
            if (this._game.place[swordId] == -1 && this._game.visited[102] == 0)
            {
                Console.WriteLine("You raise the jeweled sword. A secret compartment opens, revealing a magical amulet!");
                this._game.visited[102] = 1;
                int amuletId = 11;
                this._db.Objects.Add(new AdventureDatabase.GameObject { Id = amuletId, Name = "Amulet", Description = "A magical amulet that glows with inner light.", InitialLocation = 8, FixedLocation = 0 });
                this._game.place[amuletId] = 8;
                this._game.tally += 1;
            }
        }
        // Puzzle 8: Map reveals shortcut from entrance
        if (this._game.loc == 1)
        {
            int mapId = 9;
            if (this._game.place[mapId] == -1 && this._game.visited[103] == 0)
            {
                Console.WriteLine("You study the map and discover a shortcut to the treasure room!");
                this._game.visited[103] = 1;
                this._db.TravelTable.Add(new AdventureDatabase.TravelEntry { FromLocation = 1, ToLocation = 6, Verb = 45, Condition = 0 });
                this._game.tally += 1;
            }
        }
    }
}
