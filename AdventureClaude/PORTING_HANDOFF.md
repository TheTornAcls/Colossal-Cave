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
25/25 transcript tests passed.
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

It also includes 15 deeper parity routes:

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
- `deeper_plover_dark_room_pyramid`
- `deeper_giant_room_eggs_after_reincarnation`

These deeper routes were checked against both the rebuilt C executable and the C# port before being added to the regression suite.

The transcript test project also includes focused in-process tests for parser/vocabulary behavior, object placement/holding bookkeeping, score bookkeeping, dwarf/pirate state, turn lifecycle location changes, and cave closing / closed-state behavior. The closing coverage exercises the C# port's closing timers, blocked exits during closing, final repository setup, closed-inventory encoded property decoding, and a route-style turn script that starts from a near-closing cave state and then uses normal player commands (`y2`, `down`, `plugh`, repeated `look`) to reach the closed repository. This was added as narrow in-process coverage because a natural public transcript route to discover every treasure and run out both closing clocks is much longer than the current parity suite.

The C# port now supports both native C# randomness for normal gameplay and `Models\CReferenceRandom.cs` for parity/regression runs. The transcript harness launches the game with `--reference-random`, which reproduces the MSVC C runtime `rand()` sequence used by the rebuilt C reference. You can also set `ADVENTURE_REFERENCE_RANDOM=1` for deterministic reference-random runs.

`Models\GameState.cs` has moved away from one flat C-style bag of globals. It now exposes named debugger-friendly groups (`Position`, `World`, `Objects`, `Cave`, `TreasureProgress`, `Dwarves`, `Hints`, `Command`, and `Debug`). The old public compatibility pass-through properties have been removed, so new code must use the named state groups directly.

The first `AdventureGame` call-site migration slice is complete. Startup flags, parsed command storage/dispatch, basic motion handling, travel selection, failed-move messaging, the pre-input lifecycle save/location checks, and the closing-exit guard now use the named `GameState` groups directly.

The dwarf/pirate lifecycle slice is complete. `ApplyDwarfBlock`, `RunDwarves`, `DoPirate`, and `PirateStealsTreasure` now use the named `Position`, `Dwarves`, `Objects`, and `TreasureProgress` groups directly while keeping object movement through the existing `GameState` helpers so carrying/holding bookkeeping remains centralized.

The location-change and cave timer lifecycle slice is complete. `ApplyLocationChange`, `ApplyClosedInventoryState`, `RunSpecialTimer`, and `CloseCave` now use the named `Position`, `World`, `Objects`, `Cave`, `TreasureProgress`, `Dwarves`, and `Command` groups directly while preserving the existing movement and object bookkeeping helpers.

The hint and score/endgame bookkeeping slice is complete. `TryLocationHint`, `TryHint`, quit/suspend/brief bookkeeping, oyster hint reading, blast bonus selection, `NormalEnd`, `PrintScore`, `ShowLocationDescription`, `ShowObjectsHere`, `UpdateGameState`, and `HandleDeath` now use the named `Hints`, `TreasureProgress`, `Position`, `World`, `Objects`, and `Cave` groups directly.

The object-heavy `AdventureGame` handler migration is complete. Object dispatch, intransitive object selection, take/drop/open/lock, magic words, read setup, lamp actions, wave, kill, pour, eat/drink, throw, find, fill, feed, read/blast/break/wake, prompt helpers, and the object-heavy special movement branches now use the named `Command`, `Position`, `World`, `Objects`, `Cave`, `TreasureProgress`, and `Dwarves` groups directly. A targeted scan no longer finds legacy compatibility-property usage in `Game\AdventureGame.cs`.

Clearer object/location helper APIs have been added to `ObjectPlacementState`: `LocationOf`, `SetLocation`, `FixedLocationOf`, `SetFixedLocation`, `PropertyOf`, `SetProperty`, `ActionMessageFor`, `IsCarried`, `IsAtLocation`, `IsFixedAtLocation`, `IsAt`, `HasFixedLocation`, and `IsPropertyNegative`. `GameState`, `AdventureGame`, `DarknessManager`, and the closing state regressions now use these helpers instead of direct object array indexing outside `ObjectPlacementState`.

The data lookup refactor is complete. `Data\AdventureData.cs` is now the central facade for generated table lookups: messages, object data, object room descriptions, long/short location descriptions, travel options, vocabulary analysis, and known motion/verb words. `AdventureGame`, `InputParser`, `DarknessManager`, and `GameState` now call through this facade instead of reaching into individual generated data tables directly.

The readability pass for `AdventureGame` constants is complete. Direct message ids, key location ids, encoded travel offsets, cave/dwarf/pirate timer thresholds, hint thresholds, scoring values, rating thresholds, and the reference RNG seed now have named constants in `Models\GameConstants.cs`. The intent is to preserve C reference values while making the remaining game logic easier to audit and debug.

The first focused engine extraction is complete. `Game\TravelEngine.cs` now owns the original TURN.C travel slice: motion dispatch, `back` handling, travel-table evaluation, failed-move messages, and special plover/troll bridge travel. `AdventureGame` delegates motion commands and forced-move cascades to `TravelEngine`, while transcript parity remains green.

The second focused engine extraction is complete. `Game\TurnLifecycleEngine.cs` now owns the per-turn lifecycle around player input: cave-closing exit guards, location-change application, forced-move cascades, closed-inventory decoding, special timers, lamp warnings, and final closed-cave repository setup. `AdventureGame` keeps small private wrappers for reflection-based regression tests while delegating the behavior to `TurnLifecycleEngine`.

The third focused engine extraction is complete. `Game\DwarfPirateEngine.cs` now owns dwarf blocking, dwarf activation/movement/attacks, pirate lurking, pirate chest placement, and pirate treasure theft. `AdventureGame` keeps small private wrappers for reflection-based regression tests while `TurnLifecycleEngine` calls the new engine directly.

The fourth focused engine extraction is complete. `Game\VerbHandlers.cs` now owns parsed command dispatch, object resolution, transitive/intransitive verb dispatch, the object-heavy `IV*`/`V*` handlers, action-message fallback, object-not-here prompts, and inventory display. `AdventureGame` delegates command processing to `VerbHandlers` and keeps scoring/endgame, prompts, descriptions, and death handling as shared callbacks for now.

The fifth focused service extraction is complete. `Game\ScoringService.cs` now owns score calculation, rating output, closed-cave bonus calculation, and normal end bookkeeping. `AdventureGame` keeps thin private wrappers for reflection-based score/endgame tests and delegates scoring callbacks to `ScoringService`.

The `GameState` compatibility pass-through cleanup is complete. `Models\GameState.cs`, `Models\DarknessManager.cs`, `Game\InputParser.cs`, and the focused closing regressions now use the named state groups directly instead of aliases such as `Location`, `Clock1`, `Tally`, `Holding`, `Word1`, or `DwarfSeen`.

The next deeper natural transcript route is complete. `deeper_plover_dark_room_pyramid` was checked against the rebuilt C executable and added to `tests\TranscriptTests\Program.cs`; it covers bird-vs-snake progression, Hall of the Mountain King side treasures, fissure bridge/diamonds, Oriental Room vase, Plover Room emerald, Dark Room pyramid handling, and score output after eight treasure discoveries. The transcript harness now uses asynchronous stdout/stderr reads with timeout truncation so bad redirected-input routes fail cleanly instead of flooding output.

The full natural public cave-closing route has been extended past `deeper_plover_dark_room_pyramid`. `deeper_giant_room_eggs_after_reincarnation` was checked against the rebuilt C executable and added to `tests\TranscriptTests\Program.cs`; it leaves the Plover/Dark Room branch, waters the beanstalk twice, accepts the deterministic dwarf-death reincarnation after the second watering, recovers the lamp, returns via `plugh`, climbs to the Giant Room, discovers the golden eggs, and verifies the matching post-death score checkpoint.

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

1. Continue the full natural public C-vs-C# cave-closing route by extending the all-treasure walkthrough beyond `deeper_plover_dark_room_pyramid`:
   - remaining natural route coverage should include trident/waterfall, pearl/clam, dragon/rug, volcano spices, bear/chain, pirate chest, and then waiting out cave closing/closed-state behavior from normal commands

2. For each new route or refactor slice:
   - run C vs C# parity first
   - fix C# behavior if parity fails
   - add the passing route to `tests\TranscriptTests\Program.cs`
   - rerun `dotnet run --project tests\TranscriptTests\TranscriptTests.csproj`

## Important Notes

- Do not make the C# port mimic known-broken C behavior from before the C reference rebuild.
- Keep transcript tests focused on behavior that has passed C vs C# parity.
- The current transcript test harness checks expected snippets rather than exact full transcripts.
- The current priority is behavior parity first, refactoring second.
