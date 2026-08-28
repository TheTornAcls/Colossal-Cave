namespace AdventureClaude.Game;

using System;
using AdventureClaude.Data;
using AdventureClaude.Models;

/// <summary>
/// Handles the per-turn lifecycle around player input: movement resolution, cave timers, and closed-cave setup.
/// </summary>
public sealed class TurnLifecycleEngine
{
    private readonly GameState gameState;
    private readonly Random random;
    private readonly Action applyDwarfBlock;
    private readonly Action runDwarves;
    private readonly Action tryLocationHint;
    private readonly Action<int, int> printObjectMessage;
    private readonly Action<bool> showLocationDescription;
    private readonly Action doMove;
    private readonly Action handleDeath;
    private readonly Action normalEnd;
    private readonly Action<int> speak;

    public TurnLifecycleEngine(
        GameState gameState,
        Random random,
        Action applyDwarfBlock,
        Action runDwarves,
        Action tryLocationHint,
        Action<int, int> printObjectMessage,
        Action<bool> showLocationDescription,
        Action doMove,
        Action handleDeath,
        Action normalEnd,
        Action<int> speak)
    {
        this.gameState = gameState;
        this.random = random;
        this.applyDwarfBlock = applyDwarfBlock;
        this.runDwarves = runDwarves;
        this.tryLocationHint = tryLocationHint;
        this.printObjectMessage = printObjectMessage;
        this.showLocationDescription = showLocationDescription;
        this.doMove = doMove;
        this.handleDeath = handleDeath;
        this.normalEnd = normalEnd;
        this.speak = speak;
    }

    public bool RunBeforeInput()
    {
        ApplyClosingExitGuard();
        applyDwarfBlock();
        runDwarves();
        ApplyLocationChange();

        if (gameState.TreasureProgress.SaveRequested || gameState.Position.Location == 0)
            return false;

        ApplyClosedInventoryState();
        gameState.Cave.WizardDark = DarknessManager.IsDark(gameState);
        if (gameState.Objects.KnifeLocation > 0 && gameState.Objects.KnifeLocation != gameState.Position.Location)
            gameState.Objects.KnifeLocation = 0;

        if (RunSpecialTimer())
            return false;

        tryLocationHint();
        return !gameState.TreasureProgress.SaveRequested;
    }

    public void ApplyClosingExitGuard()
    {
        GamePositionState position = gameState.Position;
        CaveTimingState cave = gameState.Cave;

        if (position.NewLocation >= GameConstants.BelowGrateLocation || position.NewLocation == 0 || !cave.Closing)
            return;

        speak(GameConstants.MsgExitClosedUseMainOffice);
        position.NewLocation = position.Location;
        if (!cave.Panic)
            cave.Clock2 = GameConstants.ClosingExitPanicClock;
        cave.Panic = true;
    }

    public void ApplyClosedInventoryState()
    {
        ObjectPlacementState objects = gameState.Objects;

        if (!gameState.Cave.Closed)
            return;

        if (objects.IsPropertyNegative(GameConstants.Oyster) && gameState.Toting(GameConstants.Oyster))
            printObjectMessage(GameConstants.Oyster, 1);

        for (int item = 1; item <= GameConstants.MaxObjects; item++)
        {
            if (gameState.Toting(item) && objects.IsPropertyNegative(item))
                gameState.SetObjectProperty(item, -1 - objects.PropertyOf(item));
        }
    }

    private void ApplyLocationChange()
    {
        int forcedMoves = 0;
        GamePositionState position = gameState.Position;
        WorldMapState world = gameState.World;
        CaveTimingState cave = gameState.Cave;
        ParsedCommandState command = gameState.Command;

        while (position.Location != position.NewLocation && !gameState.TreasureProgress.SaveRequested)
        {
            bool forceLongDescription = position.Location == 0;
            if (forceLongDescription)
                world.VisitedLocations[position.NewLocation] =
                    (short)((world.VisitedLocations[position.NewLocation] + 3) & ~3);

            position.Turns++;
            position.Location = position.NewLocation;

            if (position.Location == 0)
            {
                handleDeath();
                return;
            }

            if (gameState.Forced(position.Location))
            {
                showLocationDescription(forceLongDescription);
                command.Motion = GameConstants.DefaultTravelVerb;
                doMove();
                if (++forcedMoves >= GameConstants.MaxForcedMoves)
                {
                    Console.WriteLine("[Warning: Maximum forced movement cascade depth reached]");
                    return;
                }
                continue;
            }

            if (cave.WizardDark && DarknessManager.IsDark(gameState) && GameState.Pct(random, 35))
            {
                Console.WriteLine(AdventureData.Message(GameConstants.MsgFellIntoPit));
                position.OldLocation2 = position.Location;
                handleDeath();
                return;
            }

            showLocationDescription(forceLongDescription);
            if (!DarknessManager.IsDark(gameState))
                world.VisitedLocations[position.Location]++;
        }
    }

    public bool RunSpecialTimer()
    {
        GamePositionState position = gameState.Position;
        ObjectPlacementState objects = gameState.Objects;
        CaveTimingState cave = gameState.Cave;
        TreasureProgressState progress = gameState.TreasureProgress;
        DwarfPirateState dwarves = gameState.Dwarves;

        progress.FooBar = progress.FooBar > 0 ? -progress.FooBar : 0;
        progress.DescriptionDetailMask = 2;

        if (progress.UndiscoveredTreasureCount == 0 &&
            position.Location >= GameConstants.HallOfMistsLocation &&
            position.Location != GameConstants.Y2Location)
            cave.Clock1--;

        if (cave.Clock1 == 0)
        {
            gameState.SetObjectProperty(GameConstants.Grate, 0);
            gameState.SetObjectProperty(GameConstants.Fissure, 0);
            for (int i = 1; i < GameConstants.MaxDwarves; i++)
                dwarves.Seen[i] = false;

            gameState.MoveObject(GameConstants.Troll, 0);
            gameState.MoveObject(GameConstants.Troll + GameConstants.MaxObjects, 0);
            gameState.MoveObject(GameConstants.Troll2, GameConstants.TrollBridgeNearSideLocation);
            gameState.MoveObject(GameConstants.Troll2 + GameConstants.MaxObjects, GameConstants.TrollBridgeFarSideLocation);
            GameState.Juggle(GameConstants.Chasm);
            if (objects.PropertyOf(GameConstants.Bear) != 3)
                gameState.Destroy(GameConstants.Bear);
            gameState.SetObjectProperty(GameConstants.Chain, 0);
            objects.SetFixedLocation(GameConstants.Chain, 0);
            gameState.SetObjectProperty(GameConstants.Axe, 0);
            objects.SetFixedLocation(GameConstants.Axe, 0);
            speak(GameConstants.MsgCaveClosingSoon);
            cave.Clock1 = -1;
            cave.Closing = true;
            return false;
        }

        if (cave.Clock1 < 0)
            cave.Clock2--;

        if (cave.Clock2 == 0)
        {
            CloseCave();
            return true;
        }

        if (objects.PropertyOf(GameConstants.Lamp) == 1)
            cave.LampLimit--;

        if (cave.LampLimit <= GameConstants.LampWarningTurns &&
            gameState.Here(GameConstants.Batteries) &&
            objects.PropertyOf(GameConstants.Batteries) == 0 &&
            gameState.Here(GameConstants.Lamp))
        {
            speak(GameConstants.MsgReplaceBatteries);
            gameState.SetObjectProperty(GameConstants.Batteries, 1);
            if (gameState.Toting(GameConstants.Batteries))
                gameState.Drop(GameConstants.Batteries, position.Location);
            cave.LampLimit += GameConstants.FreshBatteryExtraTurns;
            cave.LampWarning = 0;
            return false;
        }

        if (cave.LampLimit == 0)
        {
            cave.LampLimit--;
            gameState.SetObjectProperty(GameConstants.Lamp, 0);
            if (gameState.Here(GameConstants.Lamp))
                speak(GameConstants.MsgLampOut);
            return false;
        }

        if (cave.LampLimit < 0 && position.Location <= GameConstants.DepressionLocation)
        {
            speak(GameConstants.MsgLampOutAboveGroundEnd);
            progress.GaveUp = true;
            normalEnd();
            return true;
        }

        if (cave.LampLimit <= GameConstants.LampWarningTurns)
        {
            if (cave.LampWarning != 0 || !gameState.Here(GameConstants.Lamp))
                return false;

            cave.LampWarning = 1;
            int message = GameConstants.MsgLampGettingDimNeedBatteries;
            if (objects.LocationOf(GameConstants.Batteries) == 0)
                message = GameConstants.MsgLampDimNeedFreshBatteries;
            if (objects.PropertyOf(GameConstants.Batteries) == 1)
                message = GameConstants.MsgLampDimNoBatteries;
            speak(message);
        }

        return false;
    }

    private void CloseCave()
    {
        GamePositionState position = gameState.Position;
        ObjectPlacementState objects = gameState.Objects;
        CaveTimingState cave = gameState.Cave;

        gameState.SetObjectProperty(GameConstants.Bottle, gameState.Put(GameConstants.Bottle, GameConstants.RepositoryNortheastLocation, 1));
        gameState.SetObjectProperty(GameConstants.Plant, gameState.Put(GameConstants.Plant, GameConstants.RepositoryNortheastLocation, 0));
        gameState.SetObjectProperty(GameConstants.Oyster, gameState.Put(GameConstants.Oyster, GameConstants.RepositoryNortheastLocation, 0));
        gameState.SetObjectProperty(GameConstants.Lamp, gameState.Put(GameConstants.Lamp, GameConstants.RepositoryNortheastLocation, 0));
        gameState.SetObjectProperty(GameConstants.Rod, gameState.Put(GameConstants.Rod, GameConstants.RepositoryNortheastLocation, 0));
        gameState.SetObjectProperty(GameConstants.Dwarf, gameState.Put(GameConstants.Dwarf, GameConstants.RepositoryNortheastLocation, 0));

        position.Location = GameConstants.RepositoryNortheastLocation;
        position.OldLocation = GameConstants.RepositoryNortheastLocation;
        position.NewLocation = GameConstants.RepositoryNortheastLocation;

        gameState.Put(GameConstants.Grate, GameConstants.RepositorySouthwestLocation, 0);
        gameState.SetObjectProperty(GameConstants.Snake, gameState.Put(GameConstants.Snake, GameConstants.RepositorySouthwestLocation, 1));
        gameState.SetObjectProperty(GameConstants.Bird, gameState.Put(GameConstants.Bird, GameConstants.RepositorySouthwestLocation, 1));
        gameState.SetObjectProperty(GameConstants.Cage, gameState.Put(GameConstants.Cage, GameConstants.RepositorySouthwestLocation, 0));
        gameState.SetObjectProperty(GameConstants.Rod2, gameState.Put(GameConstants.Rod2, GameConstants.RepositorySouthwestLocation, 0));
        gameState.SetObjectProperty(GameConstants.Pillow, gameState.Put(GameConstants.Pillow, GameConstants.RepositorySouthwestLocation, 0));
        gameState.SetObjectProperty(GameConstants.Mirror, gameState.Put(GameConstants.Mirror, GameConstants.RepositoryNortheastLocation, 0));
        objects.SetFixedLocation(GameConstants.Mirror, GameConstants.RepositorySouthwestLocation);

        for (int item = 1; item <= GameConstants.MaxObjects; item++)
        {
            if (gameState.Toting(item))
                gameState.Destroy(item);
        }

        speak(GameConstants.MsgCaveNowClosed);
        cave.Closed = true;
        position.Location = 0;
    }
}
