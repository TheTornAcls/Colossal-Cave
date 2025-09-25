namespace AdventureSharp;

using System;
using System.Collections.Generic;

public class AdventureDatabase
{
    // Room/Location structure
    public class Room
    {
        public int Id { get; set; }
        public string Name { get; set; } = string.Empty;
        public string Description { get; set; } = string.Empty;
        public string ShortDescription { get; set; } = string.Empty;
        public List<int> Exits { get; set; } = new();
    }

    // Object structure
    public class GameObject
    {
        public int Id { get; set; }
        public string Name { get; set; } = string.Empty;
        public string Description { get; set; } = string.Empty;
        public int InitialLocation { get; set; }
        public int FixedLocation { get; set; }
    }

    // Message structure
    public class GameMessage
    {
        public int Id { get; set; }
        public string Text { get; set; } = string.Empty;
    }

    // Travel table entry
    public class TravelEntry
    {
        public int FromLocation { get; set; }
        public int ToLocation { get; set; }
        public int Verb { get; set; }
        public int Condition { get; set; }
    }

    public List<Room> Rooms { get; } = new();
    public List<GameObject> Objects { get; } = new();
    public List<GameMessage> Messages { get; } = new();
    public List<TravelEntry> TravelTable { get; } = new();

    // TODO: Implement loading from text files or embedded resources
    // For now, add a few sample entries for demonstration
    public AdventureDatabase()
    {
        this.Rooms.Add(new Room { Id = 1, Name = "Cave Entrance", Description = "You are standing at the entrance of a dark cave.", ShortDescription = "At cave entrance." });
        this.Objects.Add(new GameObject { Id = 1, Name = "Lamp", Description = "A brass lantern.", InitialLocation = 1, FixedLocation = 0 });
        this.Messages.Add(new GameMessage { Id = 1, Text = "Welcome to Adventure!" });
        this.TravelTable.Add(new TravelEntry { FromLocation = 1, ToLocation = 2, Verb = 45, Condition = 0 }); // Example: north
    }
}
