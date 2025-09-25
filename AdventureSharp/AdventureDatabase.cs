namespace AdventureSharp;

using System.Collections.Generic;
using System.IO;

public class AdventureDatabase
{
    public class Room
    {
        public int Id { get; set; }
        public string Name { get; set; } = string.Empty;
        public string Description { get; set; } = string.Empty;
        public string ShortDescription { get; set; } = string.Empty;
        public List<int> Exits { get; set; } = new();
    }

    public class GameObject
    {
        public int Id { get; set; }
        public string Name { get; set; } = string.Empty;
        public string Description { get; set; } = string.Empty;
        public int InitialLocation { get; set; }
        public int FixedLocation { get; set; }
    }

    public class GameMessage
    {
        public int Id { get; set; }
        public string Text { get; set; } = string.Empty;
    }

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

    public void LoadRooms(string filePath)
    {
        // Example: Each line in file: id|name|desc|shortdesc
        foreach (string line in File.ReadLines(filePath))
        {
            if (string.IsNullOrWhiteSpace(line) || line.StartsWith("#")) continue;
            string[] parts = line.Split('|');
            if (parts.Length < 4) continue;
            this.Rooms.Add(new Room
            {
                Id = int.Parse(parts[0]),
                Name = parts[1],
                Description = parts[2],
                ShortDescription = parts[3]
            });
        }
    }

    public void LoadObjects(string filePath)
    {
        // Example: Each line in file: id|name|desc|initloc|fixedloc
        foreach (string line in File.ReadLines(filePath))
        {
            if (string.IsNullOrWhiteSpace(line) || line.StartsWith("#")) continue;
            string[] parts = line.Split('|');
            if (parts.Length < 5) continue;
            this.Objects.Add(new GameObject
            {
                Id = int.Parse(parts[0]),
                Name = parts[1],
                Description = parts[2],
                InitialLocation = int.Parse(parts[3]),
                FixedLocation = int.Parse(parts[4])
            });
        }
    }

    public void LoadMessages(string filePath)
    {
        // Example: Each line in file: id|text
        foreach (string line in File.ReadLines(filePath))
        {
            if (string.IsNullOrWhiteSpace(line) || line.StartsWith("#")) continue;
            string[] parts = line.Split('|');
            if (parts.Length < 2) continue;
            this.Messages.Add(new GameMessage
            {
                Id = int.Parse(parts[0]),
                Text = parts[1]
            });
        }
    }

    public void LoadTravelTable(string filePath)
    {
        // Example: Each line in file: from|to|verb|cond
        foreach (string line in File.ReadLines(filePath))
        {
            if (string.IsNullOrWhiteSpace(line) || line.StartsWith("#")) continue;
            string[] parts = line.Split('|');
            if (parts.Length < 4) continue;
            this.TravelTable.Add(new TravelEntry
            {
                FromLocation = int.Parse(parts[0]),
                ToLocation = int.Parse(parts[1]),
                Verb = int.Parse(parts[2]),
                Condition = int.Parse(parts[3])
            });
        }
    }

    public AdventureDatabase()
    {
        // Sample rooms
        this.Rooms.Add(new Room { Id = 1, Name = "Cave Entrance", Description = "You are standing at the entrance of a dark cave.", ShortDescription = "At cave entrance." });
        this.Rooms.Add(new Room { Id = 2, Name = "Dark Tunnel", Description = "You are in a dark tunnel. The air smells damp.", ShortDescription = "In a dark tunnel." });
        this.Rooms.Add(new Room { Id = 3, Name = "Underground Lake", Description = "You are at the edge of an underground lake.", ShortDescription = "By an underground lake." });
        // Sample objects
        this.Objects.Add(new GameObject { Id = 1, Name = "Lamp", Description = "A brass lantern.", InitialLocation = 1, FixedLocation = 0 });
        this.Objects.Add(new GameObject { Id = 2, Name = "Book", Description = "An ancient book with faded writing.", InitialLocation = 2, FixedLocation = 0 });
        this.Objects.Add(new GameObject { Id = 3, Name = "Food", Description = "Some preserved food.", InitialLocation = 3, FixedLocation = 0 });
        // Sample travel table
        this.TravelTable.Add(new TravelEntry { FromLocation = 1, ToLocation = 2, Verb = 45, Condition = 0 }); // north
        this.TravelTable.Add(new TravelEntry { FromLocation = 2, ToLocation = 1, Verb = 46, Condition = 0 }); // south
        this.TravelTable.Add(new TravelEntry { FromLocation = 2, ToLocation = 3, Verb = 43, Condition = 0 }); // east
        this.TravelTable.Add(new TravelEntry { FromLocation = 3, ToLocation = 2, Verb = 44, Condition = 0 }); // west
        // Sample messages
        this.Messages.Add(new GameMessage { Id = 1, Text = "Welcome to Adventure!" });
    }
}
