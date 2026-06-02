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
        // Lamp timer event
        if (this._game.place[1] == -1)
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
        // Cave closing event
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
        // Bonus event
        if (this._game.closed && !this._game.gaveup && this._game.loc == 1)
        {
            Console.WriteLine("You have escaped the cave after it closed! Bonus awarded.");
            this._game.bonus += 10;
            this._game.gaveup = true;
        }
        // Dwarf encounter event (simple random chance)
        if (!this._game.closed && this._game.turns > 5 && this._game.turns % 7 == 0)
        {
            Console.WriteLine("A dwarf appears and throws a small axe at you! Luckily, it misses.");
        }
        // Pirate encounter event (simple random chance)
        if (!this._game.closed && this._game.turns > 10 && this._game.turns % 13 == 0)
        {
            Console.WriteLine("A sneaky pirate tries to steal your treasures, but you fend him off!");
        }
        // Hint system (if player lingers in a room)
        if (this._game.turns > 15 && this._game.loc == 2 && this._game.turns % 8 == 0)
        {
            Console.WriteLine("Hint: Sometimes, reading old books can reveal secrets.");
        }
        // Plant growth event (if player has water in room 4)
        if (this._game.loc == 4 && this._game.place.Length > 3 && this._game.place[3] == 4 && this._game.turns % 9 == 0)
        {
            Console.WriteLine("The crystal in this chamber glows brighter as you linger.");
        }
        // Vase breaking event (if player drops vase in room 5)
        if (this._game.loc == 5 && this._game.place.Length > 6 && this._game.place[6] == 5 && this._game.turns % 11 == 0)
        {
            Console.WriteLine("The treasure chest makes a loud thud as you drop it, but it doesn't break.");
        }
        // Endgame event (player in treasure room after cave closed)
        if (this._game.closed && this._game.loc == 6)
        {
            Console.WriteLine("You are trapped in the treasure room as the cave is closed! Find a way out or accept your fate.");
        }
    }
}
