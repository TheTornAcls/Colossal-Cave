# AdventureClaude Port Handoff

Last updated: 2026-08-28

## Current Status

We are porting the C/C++ Adventure reference in:

- `C:\Testing\source\repos\Colossal-Cave\Adventure`

to the C# port in:

- `C:\Testing\source\repos\Colossal-Cave\AdventureClaude`

The C reference executable was repaired and rebuilt. The original C source had an object-location initialization bug in `Adventure\ADVENT.C`: `place` and `fixed` are `int[]`, but were initialized from `short int[]` using `memcpy(... sizeof(short int) ...)`. That corrupted object locations in the C executable. The fix changed those two copies to explicit assignment loops and rebuilt:

- `C:\Testing\source\repos\Colossal-Cave\Adventure\Debug\x64\Adventure.exe`

After that, C vs C# parity was clean for the starter routes.

## Verified Test State

The C# regression transcript suite is in:

- `tests\TranscriptTests\Program.cs`

Run it with:

```powershell
dotnet run --project tests\TranscriptTests\TranscriptTests.csproj
```

Most recent result:

```text
23/23 transcript tests passed.
11/11 in-process tests passed.
```

The suite now includes 10 starter parity routes:

- `quit_score`
- `building_inventory`
- `magic_noop_and_missing_lamp`
- `get_keys_unlock_grate`
- `take_food_eat`
- `basic_movement_cycle`
- `lamp_on_off`
- `keys_lock_unlock`
- `inventory_empty`
- `unknown_words`

It also includes 13 deeper parity routes:

- `deeper_cave_entry_lamp_on`
- `deeper_cobble_debris_xyzzy`
- `deeper_cage_rod_inventory`
- `deeper_bird_room_attempt`
- `deeper_nugget_score`
- `deeper_plugh_from_bird_chamber`
- `deeper_lamp_off_below_grate`
- `deeper_hall_king_snake_score`
- `deeper_nugget_inventory_score`
- `deeper_dwarf_pirate_warning`
- `deeper_pirate_steals_nugget_inventory`
- `deeper_fissure_jump_death_no`
- `deeper_fissure_jump_reincarnate_yes`

These deeper routes were checked against both the rebuilt C executable and the C# port before being added to the regression suite.

The transcript test project also includes focused in-process tests for parser/vocabulary behavior, object placement/holding bookkeeping, score bookkeeping, dwarf/pirate state, turn lifecycle location changes, and cave closing / closed-state behavior. The closing coverage exercises the C# port's closing timers, blocked exits during closing, final repository setup, closed-inventory encoded property decoding, and a route-style turn script that starts from a near-closing cave state and then uses normal player commands (`y2`, `down`, `plugh`, repeated `look`) to reach the closed repository. This was added as narrow in-process coverage because a natural public transcript route to discover every treasure and run out both closing clocks is much longer than the current parity suite.

The C# port now supports both native C# randomness for normal gameplay and `Models\CReferenceRandom.cs` for parity/regression runs. The transcript harness launches the game with `--reference-random`, which reproduces the MSVC C runtime `rand()` sequence used by the rebuilt C reference. You can also set `ADVENTURE_REFERENCE_RANDOM=1` for deterministic reference-random runs.

`Models\GameState.cs` has started moving away from one flat C-style bag of globals. It now exposes named debugger-friendly groups (`Position`, `World`, `Objects`, `Cave`, `TreasureProgress`, `Dwarves`, `Hints`, `Command`, and `Debug`) while keeping the old property names as compatibility pass-throughs. This keeps current gameplay code stable and gives future refactors a safer migration path.

The first `AdventureGame` call-site migration slice is complete. Startup flags, parsed command storage/dispatch, basic motion handling, travel selection, failed-move messaging, the pre-input lifecycle save/location checks, and the closing-exit guard now use the named `GameState` groups directly. Object-heavy verb logic still mostly uses the compatibility pass-throughs and should be migrated in smaller follow-up slices.

The dwarf/pirate lifecycle slice is complete. `ApplyDwarfBlock`, `RunDwarves`, `DoPirate`, and `PirateStealsTreasure` now use the named `Position`, `Dwarves`, `Objects`, and `TreasureProgress` groups directly while keeping object movement through the existing `GameState` helpers so carrying/holding bookkeeping remains centralized.

The location-change and cave timer lifecycle slice is complete. `ApplyLocationChange`, `ApplyClosedInventoryState`, `RunSpecialTimer`, and `CloseCave` now use the named `Position`, `World`, `Objects`, `Cave`, `TreasureProgress`, `Dwarves`, and `Command` groups directly while preserving the existing movement and object bookkeeping helpers.

The hint and score/endgame bookkeeping slice is complete. `TryLocationHint`, `TryHint`, quit/suspend/brief bookkeeping, oyster hint reading, blast bonus selection, `NormalEnd`, `PrintScore`, `ShowLocationDescription`, `ShowObjectsHere`, `UpdateGameState`, and `HandleDeath` now use the named `Hints`, `TreasureProgress`, `Position`, `World`, `Objects`, and `Cave` groups directly. Remaining compatibility-property uses are mostly in object-heavy verb handlers and should move in the next `AdventureGame` refactor slice.

The object-heavy `AdventureGame` handler migration is complete. Object dispatch, intransitive object selection, take/drop/open/lock, magic words, read setup, lamp actions, wave, kill, pour, eat/drink, throw, find, fill, feed, read/blast/break/wake, prompt helpers, and the object-heavy special movement branches now use the named `Command`, `Position`, `World`, `Objects`, `Cave`, `TreasureProgress`, and `Dwarves` groups directly. A targeted scan no longer finds legacy compatibility-property usage in `Game\AdventureGame.cs`.

Clearer object/location helper APIs have been added to `ObjectPlacementState`: `LocationOf`, `SetLocation`, `FixedLocationOf`, `SetFixedLocation`, `PropertyOf`, `SetProperty`, `ActionMessageFor`, `IsCarried`, `IsAtLocation`, `IsFixedAtLocation`, `IsAt`, `HasFixedLocation`, and `IsPropertyNegative`. `GameState`, `AdventureGame`, `DarknessManager`, and the closing state regressions now use these helpers instead of direct object array indexing outside `ObjectPlacementState`.

The data lookup refactor is complete. `Data\AdventureData.cs` is now the central facade for generated table lookups: messages, object data, object room descriptions, long/short location descriptions, travel options, vocabulary analysis, and known motion/verb words. `AdventureGame`, `InputParser`, `DarknessManager`, and `GameState` now call through this facade instead of reaching into individual generated data tables directly.

## Recent C# Wording Fixes

The C# port was adjusted to match C transcript wording:

- Empty inventory: `You're not carrying anything.`
- Inventory heading: `You are currently holding the following:`
- Unknown first word: `I don't know that word.`
- Unknown second word after a known first word: `I don't understand that!`

Files involved:

- `Game\AdventureGame.cs`
- `Game\InputParser.cs`
- `tests\TranscriptTests\Program.cs`
- `Models\CReferenceRandom.cs`

## Useful Commands

Build C#:

```powershell
dotnet build C:\Testing\source\repos\Colossal-Cave\AdventureClaude\AdventureClaude.csproj
```

Run C# transcript tests:

```powershell
dotnet run --project C:\Testing\source\repos\Colossal-Cave\AdventureClaude\tests\TranscriptTests\TranscriptTests.csproj
```

Run C# game:

```powershell
dotnet C:\Testing\source\repos\Colossal-Cave\AdventureClaude\bin\Debug\net9.0\AdventureClaude.dll
```

Run rebuilt C reference:

```powershell
C:\Testing\source\repos\Colossal-Cave\Adventure\Debug\x64\Adventure.exe
```

Rebuild C reference, if Visual Studio command-line tools are available:

```powershell
& 'C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\IDE\devenv.com' 'C:\Testing\source\repos\Colossal-Cave\Adventure\Adventure.vcxproj' /Build 'Debug|x64'
```

## Next Steps

1. Continue adding/fixing deeper parity routes for systems not yet fully exercised, if more behavior gaps are found:
   - optionally add a full natural public C-vs-C# transcript route for cave closing and closed-state behavior if/when we want all-treasure endgame transcript coverage beyond the focused route-style state regression

2. For each new route:
   - run C vs C# parity first
   - fix C# behavior if parity fails
   - add the passing route to `tests\TranscriptTests\Program.cs`
   - rerun `dotnet run --project tests\TranscriptTests\TranscriptTests.csproj`

3. Improve readability by replacing remaining magic numeric ids with named constants:
   - start with message ids in `AdventureGame.cs`, such as `AdventureData.Message(15)`
   - continue through key location ids, special travel destinations/thresholds, timer thresholds, and scoring/status values
   - preserve the original numeric values in the constants so C reference parity remains easy to audit

## Important Notes

- Do not make the C# port mimic known-broken C behavior from before the C reference rebuild.
- Keep transcript tests focused on behavior that has passed C vs C# parity.
- The current transcript test harness checks expected snippets rather than exact full transcripts.
- The current priority is behavior parity first, refactoring second.
