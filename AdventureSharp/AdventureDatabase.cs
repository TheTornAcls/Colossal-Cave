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
        // Remove sample data. All data is now loaded from files.
        this.Rooms.Clear();
        this.Objects.Clear();
        this.Messages.Clear();
        this.TravelTable.Clear();
        // Data is loaded by AdventureGame.LoadGameData()
    }
}
