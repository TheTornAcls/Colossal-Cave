namespace AdventureSharp;

using System;

public class AdventureEvents
{
    private readonly AdventureGame _game;
    private readonly AdventureDatabase _db;

    public AdventureEvents(AdventureGame game, AdventureDatabase db)
    {
        this._game = game;
        this._db = db;
    }

    // Call this at the end of each turn
    public void ProcessTurnEvents()
    {
        // Example: lamp timer
        if (this._game.place[1] == -1) // Lamp carried
        {
            if (this._game.clock1 > 0)
            {
                this._game.clock1--;
                if (this._game.clock1 == 10)
                {
                    Console.WriteLine("Your lamp is getting dim.");
                }
                else if (this._game.clock1 == 0)
                {
                    Console.WriteLine("Your lamp has run out of power.");
                }
            }
        }
        // Example: closing event
        if (!this._game.closed && this._game.turns > 50)
        {
            this._game.closing = true;
            Console.WriteLine("A distant rumbling signals the cave is closing soon!");
        }
        if (this._game.closing && !this._game.closed && this._game.turns > 60)
        {
            this._game.closed = true;
            Console.WriteLine("The cave is now closed. You sense you should leave soon!");
        }
        // Example: bonus event
        if (this._game.closed && !this._game.gaveup && this._game.loc == 1)
        {
            Console.WriteLine("You have escaped the cave after it closed! Bonus awarded.");
            this._game.bonus += 10;
            this._game.gaveup = true;
        }
    }
}
