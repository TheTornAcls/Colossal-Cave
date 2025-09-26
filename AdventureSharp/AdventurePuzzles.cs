namespace AdventureSharp;

using System;

public class AdventurePuzzles
{
    private readonly AdventureGame _game;
    private readonly AdventureDatabase _db;

    public AdventurePuzzles(AdventureGame game, AdventureDatabase db)
    {
        this._game = game;
        this._db = db;
    }

    // Call this at the end of each turn
    public void ProcessPuzzles()
    {
        // Example puzzle: Unlock treasure room with key
        if (this._game.loc == 6) // Treasure Room
        {
            int keyId = 5; // Key object
            if (this._game.place[keyId] == -1)
            {
                Console.WriteLine("You use the rusty key to unlock the treasure chest!");
                int chestId = 6;
                if (this._game.place[chestId] == 6)
                {
                    Console.WriteLine("The chest opens, revealing a pile of gold coins!");
                    // Optionally, update state to mark chest as opened
                }
            }
            else
            {
                Console.WriteLine("A locked treasure chest sits here. You need a key to open it.");
            }
        }
        // Example puzzle: Read book in tunnel to reveal secret
        if (this._game.loc == 2)
        {
            int bookId = 2;
            if (this._game.place[bookId] == -1 && this._game.visited[100] == 0) // Use visited[100] as a flag
            {
                Console.WriteLine("As you read the ancient book, a secret passage opens to the east!");
                this._game.visited[100] = 1;
                // Add a new travel entry for the secret passage
                this._db.TravelTable.Add(new AdventureDatabase.TravelEntry { FromLocation = 2, ToLocation = 7, Verb = 43, Condition = 0 });
                this._db.Rooms.Add(new AdventureDatabase.Room { Id = 7, Name = "Secret Alcove", Description = "You have discovered a hidden alcove filled with mysterious artifacts.", ShortDescription = "In a secret alcove." });
            }
        }
        // Example puzzle: Place crystal in crystal chamber
        if (this._game.loc == 4)
        {
            int crystalId = 4;
            if (this._game.place[crystalId] == 4 && this._game.visited[101] == 0)
            {
                Console.WriteLine("You place the crystal on a pedestal. The room glows and a hidden door opens north!");
                this._game.visited[101] = 1;
                this._db.TravelTable.Add(new AdventureDatabase.TravelEntry { FromLocation = 4, ToLocation = 8, Verb = 45, Condition = 0 });
                this._db.Rooms.Add(new AdventureDatabase.Room { Id = 8, Name = "Glowing Hall", Description = "A hall illuminated by the power of the crystal.", ShortDescription = "In a glowing hall." });
            }
        }
    }
}
