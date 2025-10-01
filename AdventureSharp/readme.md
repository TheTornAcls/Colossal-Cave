# AdventureSharp

AdventureSharp is a C# port of the classic Colossal Cave Adventure game. This project aims to faithfully recreate the original gameplay, puzzles, and experience using modern .NET technologies.

## Features
- Text-based adventure gameplay
- Faithful recreation of rooms, objects, and puzzles
- Save and restore game state
- Modular subsystems for scoring, events, puzzles, and more
- Data-driven design using embedded resources

## Getting Started

### Prerequisites
- .NET 9 SDK

### Building and Running
1. Clone the repository:
   ```sh
   git clone https://github.com/TheTornAcls/Colossal-Cave.git
   ```
2. Navigate to the project directory:
   ```sh
   cd Colossal-Cave/AdventureSharp
   ```
3. Build the project:
   ```sh
   dotnet build
   ```
4. Run the game:
   ```sh
   dotnet run
   ```
   - Use the `-r` flag to restore a saved game: `dotnet run -- -r`

## Project Structure
- `AdventureSharp/` - Main C# source code
- `Advent1Data.cs`, `Advent3Data.cs`, `Advent4Data.cs` - Embedded game data
- `AdventureGame.cs` - Core game logic
- `AdventureDatabase.cs` - Data models and loading
- `AdventureVerbs.cs`, `AdventurePuzzles.cs`, `AdventureEvents.cs`, `AdventureScoring.cs`, `AdventurePirate.cs` - Game subsystems

## Contributing
Contributions are welcome! Please open issues or pull requests for bug fixes, enhancements, or new features.

## License
This project is open source. See the repository for license details.

## Credits
- Original game by Willie Crowther and Don Woods
- C# port by the AdventureSharp Team
