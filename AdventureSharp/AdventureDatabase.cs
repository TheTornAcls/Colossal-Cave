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
        this.Rooms.Add(new Room { Id = 4, Name = "Crystal Chamber", Description = "You are in a chamber glittering with crystals.", ShortDescription = "In a crystal chamber." });
        this.Rooms.Add(new Room { Id = 5, Name = "Narrow Passage", Description = "You are in a narrow passage. The walls are close.", ShortDescription = "In a narrow passage." });
        this.Rooms.Add(new Room { Id = 6, Name = "Treasure Room", Description = "You are in a room filled with ancient treasures.", ShortDescription = "In the treasure room." });

        // Sample objects
        this.Objects.Add(new GameObject { Id = 1, Name = "Lamp", Description = "A brass lantern.", InitialLocation = 1, FixedLocation = 0 });
        this.Objects.Add(new GameObject { Id = 2, Name = "Book", Description = "An ancient book with faded writing.", InitialLocation = 2, FixedLocation = 0 });
        this.Objects.Add(new GameObject { Id = 3, Name = "Food", Description = "Some preserved food.", InitialLocation = 3, FixedLocation = 0 });
        this.Objects.Add(new GameObject { Id = 4, Name = "Crystal", Description = "A sparkling crystal.", InitialLocation = 4, FixedLocation = 0 });
        this.Objects.Add(new GameObject { Id = 5, Name = "Key", Description = "A small rusty key.", InitialLocation = 5, FixedLocation = 0 });
        this.Objects.Add(new GameObject { Id = 6, Name = "Treasure Chest", Description = "A heavy chest filled with gold coins.", InitialLocation = 6, FixedLocation = 0 });
        this.Objects.Add(new GameObject { Id = 7, Name = "Sword", Description = "A sharp sword with a jeweled hilt.", InitialLocation = 2, FixedLocation = 0 });
        this.Objects.Add(new GameObject { Id = 8, Name = "Shield", Description = "A sturdy iron shield.", InitialLocation = 3, FixedLocation = 0 });
        this.Objects.Add(new GameObject { Id = 9, Name = "Map", Description = "A faded map showing the cave layout.", InitialLocation = 1, FixedLocation = 0 });

        // Sample travel table
        this.TravelTable.Add(new TravelEntry { FromLocation = 1, ToLocation = 2, Verb = 45, Condition = 0 }); // north
        this.TravelTable.Add(new TravelEntry { FromLocation = 2, ToLocation = 1, Verb = 46, Condition = 0 }); // south
        this.TravelTable.Add(new TravelEntry { FromLocation = 2, ToLocation = 3, Verb = 43, Condition = 0 }); // east
        this.TravelTable.Add(new TravelEntry { FromLocation = 3, ToLocation = 2, Verb = 44, Condition = 0 }); // west
        this.TravelTable.Add(new TravelEntry { FromLocation = 3, ToLocation = 4, Verb = 45, Condition = 0 }); // north
        this.TravelTable.Add(new TravelEntry { FromLocation = 4, ToLocation = 3, Verb = 46, Condition = 0 }); // south
        this.TravelTable.Add(new TravelEntry { FromLocation = 4, ToLocation = 5, Verb = 43, Condition = 0 }); // east
        this.TravelTable.Add(new TravelEntry { FromLocation = 5, ToLocation = 4, Verb = 44, Condition = 0 }); // west
        this.TravelTable.Add(new TravelEntry { FromLocation = 5, ToLocation = 6, Verb = 45, Condition = 0 }); // north
        this.TravelTable.Add(new TravelEntry { FromLocation = 6, ToLocation = 5, Verb = 46, Condition = 0 }); // south
        this.TravelTable.Add(new TravelEntry { FromLocation = 1, ToLocation = 5, Verb = 43, Condition = 0 }); // east from entrance to passage
        this.TravelTable.Add(new TravelEntry { FromLocation = 5, ToLocation = 1, Verb = 44, Condition = 0 }); // west from passage to entrance

        // Sample messages
        this.Messages.Add(new GameMessage { Id = 1, Text = "Welcome to Adventure!" });
    }
}
