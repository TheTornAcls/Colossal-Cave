namespace AdventureClaude.Game;

using System;
using AdventureClaude.Models;

/// <summary>
/// Handles dwarf movement/attacks and pirate treasure theft from the original turn lifecycle.
/// </summary>
public sealed class DwarfPirateEngine
{
    private readonly GameState gameState;
    private readonly Random random;
    private readonly Action<int> speak;
    private readonly Action handleDeath;

    public DwarfPirateEngine(
        GameState gameState,
        Random random,
        Action<int> speak,
        Action handleDeath)
    {
        this.gameState = gameState;
        this.random = random;
        this.speak = speak;
        this.handleDeath = handleDeath;
    }

    public void ApplyDwarfBlock()
    {
        GamePositionState position = gameState.Position;
        DwarfPirateState dwarves = gameState.Dwarves;

        if (position.NewLocation == position.Location ||
            gameState.Forced(position.Location) ||
            gameState.LocationHasFlag(position.Location, GameConstants.NoPirat))
        {
            return;
        }

        for (int i = 1; i < GameConstants.MaxDwarves - 1; i++)
        {
            if (dwarves.PreviousLocations[i] == position.NewLocation && dwarves.Seen[i])
            {
                position.NewLocation = position.Location;
                speak(GameConstants.MsgDwarfBlocksWay);
                return;
            }
        }
    }

    public void RunDwarves()
    {
        GamePositionState position = gameState.Position;
        DwarfPirateState dwarves = gameState.Dwarves;
        ObjectPlacementState objects = gameState.Objects;

        if (position.NewLocation == 0 ||
            gameState.Forced(position.NewLocation) ||
            gameState.LocationHasFlag(position.NewLocation, GameConstants.NoPirat))
        {
            return;
        }

        if (dwarves.ActivationLevel == 0)
        {
            if (position.NewLocation > GameConstants.DwarfActiveLocationThreshold)
                dwarves.ActivationLevel++;
            return;
        }

        if (dwarves.ActivationLevel == 1)
        {
            if (position.NewLocation < GameConstants.DwarfActiveLocationThreshold ||
                GameState.Pct(random, GameConstants.DwarfActivationChance))
                return;

            dwarves.ActivationLevel++;
            for (int i = 1; i < 3; i++)
            {
                if (GameState.Pct(random, 50))
                    dwarves.Locations[GameState.RRand(random, 1, GameConstants.DwarfCullRollUpperBound)] = 0;
            }

            for (int i = 1; i < GameConstants.MaxDwarves - 1; i++)
            {
                if (dwarves.Locations[i] == position.NewLocation)
                    dwarves.Locations[i] = dwarves.AlternateLocation;
                dwarves.PreviousLocations[i] = dwarves.Locations[i];
            }

            speak(GameConstants.MsgDwarfWarningAxeMissed);
            gameState.Drop(GameConstants.Axe, position.NewLocation);
            return;
        }

        int dwarfCount = 0;
        int attacks = 0;
        int hits = 0;

        for (int i = 1; i < GameConstants.MaxDwarves; i++)
        {
            if (dwarves.Locations[i] == 0)
                continue;

            int candidateLocation = dwarves.PreviousLocations[i];
            for (int attempt = 1; attempt < GameConstants.DwarfLocationRollAttempts; attempt++)
            {
                candidateLocation = GameState.RRand(
                    random,
                    GameConstants.DwarfLocationRollLow,
                    GameConstants.DwarfLocationRollHigh);
                if (candidateLocation != dwarves.PreviousLocations[i] &&
                    candidateLocation != dwarves.Locations[i])
                {
                    break;
                }
            }

            dwarves.PreviousLocations[i] = dwarves.Locations[i];
            dwarves.Locations[i] = candidateLocation;

            dwarves.Seen[i] =
                (dwarves.Seen[i] && position.NewLocation >= GameConstants.DwarfActiveLocationThreshold) ||
                dwarves.Locations[i] == position.NewLocation ||
                dwarves.PreviousLocations[i] == position.NewLocation;

            if (!dwarves.Seen[i])
                continue;

            dwarves.Locations[i] = position.NewLocation;
            if (i == GameConstants.MaxDwarves - 1)
            {
                DoPirate();
                continue;
            }

            dwarfCount++;
            if (dwarves.PreviousLocations[i] == dwarves.Locations[i])
            {
                attacks++;
                if (objects.KnifeLocation >= 0)
                    objects.KnifeLocation = position.NewLocation;
                if (GameState.RRand(random, 0, GameConstants.DwarfKnifeRollHigh) <
                    GameConstants.DwarfKnifeHitChancePerLevel * (dwarves.ActivationLevel - 2))
                    hits++;
            }
        }

        if (dwarfCount == 0)
            return;

        if (dwarfCount > 1)
            Console.WriteLine($"There are {dwarfCount} threatening little dwarves in the room with you!");
        else
            speak(GameConstants.MsgThreateningDwarfHere);

        if (attacks == 0)
            return;

        if (dwarves.ActivationLevel == 2)
            dwarves.ActivationLevel++;

        int messageBase;
        if (attacks > 1)
        {
            Console.WriteLine($"{attacks} of them throw knives at you!!");
            messageBase = GameConstants.MsgNoDwarvesHit;
        }
        else
        {
            speak(GameConstants.MsgKnifeThrown);
            messageBase = GameConstants.MsgKnifeMisses;
        }

        if (hits <= 1)
        {
            speak(hits + messageBase);
            if (hits == 0)
                return;
        }
        else
        {
            Console.WriteLine($"{hits} of them get you !!!");
        }

        position.OldLocation2 = position.NewLocation;
        handleDeath();
    }

    public void DoPirate()
    {
        GamePositionState position = gameState.Position;
        DwarfPirateState dwarves = gameState.Dwarves;
        ObjectPlacementState objects = gameState.Objects;
        TreasureProgressState progress = gameState.TreasureProgress;

        if (position.NewLocation == objects.ChestLocation ||
            objects.PropertyOf(GameConstants.Chest) >= 0)
        {
            return;
        }

        int nearbyTreasures = 0;
        for (int treasure = GameConstants.Nugget; treasure <= GameConstants.MaxTreasures; treasure++)
        {
            if (treasure == GameConstants.Pyramid &&
                (position.NewLocation == objects.LocationOf(GameConstants.Pyramid) ||
                position.NewLocation == objects.LocationOf(GameConstants.Emerald)))
            {
                continue;
            }

            if (gameState.Toting(treasure))
            {
                PirateStealsTreasure();
                return;
            }

            if (gameState.Here(treasure))
                nearbyTreasures++;
        }

        if (progress.UndiscoveredTreasureCount == progress.TreasuresLostToEndgame + 1 &&
            nearbyTreasures == 0 &&
            objects.LocationOf(GameConstants.Chest) == 0 &&
            gameState.Here(GameConstants.Lamp) &&
            objects.PropertyOf(GameConstants.Lamp) == 1)
        {
            speak(GameConstants.MsgPirateSpottedChest);
            gameState.MoveObject(GameConstants.Chest, objects.ChestLocation);
            gameState.MoveObject(GameConstants.Message, objects.ChestLocation2);
            dwarves.Locations[GameConstants.MaxDwarves - 1] = objects.ChestLocation;
            dwarves.PreviousLocations[GameConstants.MaxDwarves - 1] = objects.ChestLocation;
            dwarves.Seen[GameConstants.MaxDwarves - 1] = false;
            return;
        }

        if (dwarves.PreviousLocations[GameConstants.MaxDwarves - 1] !=
            dwarves.Locations[GameConstants.MaxDwarves - 1] &&
            GameState.Pct(random, GameConstants.PirateRustleChance))
        {
            speak(GameConstants.MsgPirateRustling);
        }
    }

    private void PirateStealsTreasure()
    {
        GamePositionState position = gameState.Position;
        DwarfPirateState dwarves = gameState.Dwarves;
        ObjectPlacementState objects = gameState.Objects;

        speak(GameConstants.MsgPirateStealsBooty);
        if (objects.LocationOf(GameConstants.Message) == 0)
            gameState.MoveObject(GameConstants.Chest, objects.ChestLocation);
        gameState.MoveObject(GameConstants.Message, objects.ChestLocation2);

        for (int treasure = GameConstants.Nugget; treasure <= GameConstants.MaxTreasures; treasure++)
        {
            if (treasure == GameConstants.Pyramid &&
                (position.NewLocation == objects.LocationOf(GameConstants.Pyramid) ||
                position.NewLocation == objects.LocationOf(GameConstants.Emerald)))
            {
                continue;
            }

            if (gameState.At(treasure) && objects.FixedLocationOf(treasure) == 0)
                gameState.Carry(treasure, position.NewLocation);
            if (gameState.Toting(treasure))
                gameState.Drop(treasure, objects.ChestLocation);
        }

        dwarves.Locations[GameConstants.MaxDwarves - 1] = objects.ChestLocation;
        dwarves.PreviousLocations[GameConstants.MaxDwarves - 1] = objects.ChestLocation;
        dwarves.Seen[GameConstants.MaxDwarves - 1] = false;
    }
}
