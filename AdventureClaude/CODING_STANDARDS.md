# AdventureClaude Coding Standards

This document outlines the coding standards and conventions used in the AdventureClaude project, a C# port of the classic Colossal Cave Adventure game.

## Project Structure

### File Organization
- **One class per file**: Each class should be defined in its own separate file
- **File naming**: File names should match the class name exactly (e.g., `GameState.cs` for the `GameState` class)
- **Folder structure**: Organize classes into logical folders:
  - `Data/` - Static data classes (LocationDescriptions, GameMessages, etc.)
  - `Models/` - Core data models and game state (GameState, GameConstants, etc.)
  - `Game/` - Game logic and engine classes (AdventureGame, InputParser, etc.)

### Namespace Conventions
- **File-scoped namespaces**: Use C# 10+ file-scoped namespace syntax
- **Namespace structure**: Follow the folder hierarchy
  - Root: `namespace AdventureClaude;`
  - Models: `namespace AdventureClaude.Models;`
  - Game logic: `namespace AdventureClaude.Game;`
  - Data classes: `namespace AdventureClaude.Data;`

#### File-Scoped Namespace Example:
```csharp
using System;
using System.Collections.Generic;

namespace AdventureClaude.Models;

/// <summary>
/// Class documentation here
/// </summary>
public class GameState
{
    // Class implementation
}
```

**NOT:**
```csharp
namespace AdventureClaude.Models
{
    public class GameState
    {
        // Class implementation
    }
}
```

## Code Style Guidelines

### Type Usage
- **Explicit types**: Use explicit type declarations instead of `var`
  - ✅ `Dictionary<int, string> locations = new Dictionary<int, string>();`
  - ❌ `var locations = new Dictionary<int, string>();`

### Access Modifiers
- **Explicit access modifiers**: Always specify access modifiers
- **Public members**: Use for classes that need external access
- **Internal members**: Use for classes only accessed within the assembly
- **Private members**: Use for implementation details

### Documentation
- **XML documentation**: All public classes and members should have XML documentation
- **Summary tags**: Provide clear, concise descriptions
- **Conversion notes**: Document which original C files the class was converted from

#### Documentation Example:
```csharp
/// <summary>
/// Represents the complete game state for the adventure.
/// Converted from the C global variables and structs in ADVDEC.H.
/// </summary>
public class GameState
{
    /// <summary>
    /// Gets or sets the current player location.
    /// </summary>
    public int Location { get; set; } = GameConstants.StartLocation;
}
```

### Collection Initialization
- **Explicit collection types**: Use explicit Dictionary/List types
- **Target-typed new**: Use `new()` for collection initialization when type is explicit
  - ✅ `public static readonly Dictionary<int, string> Messages = new();`

### Method and Property Naming
- **PascalCase**: Use PascalCase for public members
- **camelCase**: Use camelCase for private fields and local variables
- **Descriptive names**: Use clear, descriptive names that indicate purpose

## Class Design Patterns

### Static Data Classes
- Use `static` classes for data that doesn't change during runtime
- Use `readonly` collections for immutable data
- Example: `LocationDescriptions`, `GameMessages`

### Game State Management
- Use instance classes for mutable game state
- Provide clear initialization methods
- Example: `GameState`, `InputParser`

### Constants and Configuration
- Group related constants in dedicated classes
- Use `const` for compile-time constants
- Use `static readonly` for runtime constants
- Example: `GameConstants`

## File Template

When creating new classes, use this template:

```csharp
using System;
using System.Collections.Generic;
// Additional using statements as needed

namespace AdventureClaude.[FolderName];

/// <summary>
/// Brief description of the class purpose.
/// Note any original C file this was converted from.
/// </summary>
public class ClassName
{
    // Private fields (camelCase)
    private readonly string exampleField;
    
    // Public properties (PascalCase)
    public string ExampleProperty { get; set; }
    
    // Constructor
    public ClassName()
    {
        // Initialization logic
    }
    
    // Public methods (PascalCase)
    public void ExampleMethod()
    {
        // Method implementation
    }
    
    // Private methods (PascalCase)
    private void HelperMethod()
    {
        // Helper implementation
    }
}
```

## Conversion Guidelines

When porting from the original C code:

1. **Maintain functionality**: Preserve the original game behavior
2. **Modern C# patterns**: Use appropriate C# idioms and patterns
3. **Clear mapping**: Document which C files/functions each class represents
4. **Type safety**: Leverage C#'s type system for better code safety
5. **Object-oriented design**: Group related data and functionality together

## Dependencies and Imports

- **Minimal using statements**: Only include necessary using directives
- **System namespaces first**: Order System namespaces before project namespaces
- **Alphabetical ordering**: Sort using statements alphabetically within each group

## Example File Structure

```
AdventureClaude/
├── Program.cs                    (namespace AdventureClaude)
├── Data/
│   ├── LocationDescriptions.cs  (namespace AdventureClaude.Data)
│   ├── GameMessages.cs         (namespace AdventureClaude.Data)
│   ├── GameObjects.cs          (namespace AdventureClaude.Data)
│   └── VocabularySimple.cs     (namespace AdventureClaude.Data)
├── Models/
│   ├── GameState.cs            (namespace AdventureClaude.Models)
│   └── GameConstants.cs        (namespace AdventureClaude.Models)
└── Game/
    ├── AdventureGame.cs        (namespace AdventureClaude.Game)
    └── InputParser.cs          (namespace AdventureClaude.Game)
```

## Enforcement

These standards should be followed for:
- All new classes and files
- Any modifications to existing code
- Code reviews and pull requests

Following these conventions ensures consistency, maintainability, and readability throughout the AdventureClaude codebase.