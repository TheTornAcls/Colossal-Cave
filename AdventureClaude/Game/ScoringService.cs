namespace AdventureClaude.Game;

using System;
using AdventureClaude.Models;

/// <summary>
/// Calculates score, rating, bonus, and normal game-end bookkeeping.
/// </summary>
public sealed class ScoringService
{
    private readonly GameState gameState;
    private readonly Action<int> speak;

    public ScoringService(GameState gameState, Action<int> speak)
    {
        this.gameState = gameState;
        this.speak = speak;
    }

    public void NormalEnd()
    {
        int total = PrintScore();
        TreasureProgressState progress = gameState.TreasureProgress;
        int[] limits = GameConstants.RatingThresholds;
        int ratingIndex = 0;
        while (ratingIndex < limits.Length && limits[ratingIndex] <= total)
            ratingIndex++;

        Console.WriteLine();
        int ratingMessage = GameConstants.MsgFirstRating + ratingIndex;
        if (ratingMessage <= GameConstants.MsgLastRating)
            speak(ratingMessage);

        int next = ratingIndex < limits.Length ? limits[ratingIndex] - total : 0;
        if (next > 0)
            Console.WriteLine($"To achieve the next higher rating, you need {next} more point{(next == 1 ? string.Empty : "s")}.");

        progress.SaveRequested = true;
    }

    public int PrintScore()
    {
        WorldMapState world = gameState.World;
        ObjectPlacementState objects = gameState.Objects;
        CaveTimingState cave = gameState.Cave;
        TreasureProgressState progress = gameState.TreasureProgress;

        int score = 0;
        int treasures = 0;

        for (int item = GameConstants.Nugget; item <= GameConstants.MaxTreasures; item++)
        {
            int itemScore = item == GameConstants.Chest
                ? GameConstants.ChestTreasureScore
                : item > GameConstants.Chest
                    ? GameConstants.TreasureScoreAfterChest
                    : GameConstants.TreasureScoreBeforeChest;
            if (objects.PropertyOf(item) >= 0)
                treasures += GameConstants.TreasureDiscoveryScore;
            if (objects.LocationOf(item) == GameConstants.WellHouse && objects.PropertyOf(item) == 0)
                treasures += itemScore - GameConstants.TreasureDiscoveryScore;
        }

        PrintScoreLine("Treasures:", treasures);
        score += treasures;

        int survival = (GameConstants.MaxDeaths - progress.DeathCount) * GameConstants.SurvivalScorePerDeathRemaining;
        if (survival != 0)
            PrintScoreLine("Survival:", survival);
        score += survival;

        if (!progress.GaveUp)
            score += GameConstants.DidNotQuitScore;

        int gettingIn = world.VisitedLocations[GameConstants.HallOfMountainKingLocation] != 0
            ? GameConstants.GettingWellInScore
            : 0;
        if (gettingIn != 0)
            PrintScoreLine("Getting well in:", gettingIn);
        score += gettingIn;

        int masters = cave.Closing ? GameConstants.MastersSectionScore : 0;
        if (masters != 0)
            PrintScoreLine("Masters section:", masters);
        score += masters;

        if (cave.Closed)
        {
            int bonus = progress.Bonus == 0 ? GameConstants.ClosedBonusScoreDefault :
                progress.Bonus == GameConstants.MsgBlastSelfDeath ? GameConstants.ClosedBonusScoreSelfBlast :
                progress.Bonus == GameConstants.MsgBlastLavaDeath ? GameConstants.ClosedBonusScoreLava :
                progress.Bonus == GameConstants.MsgBlastWins ? GameConstants.ClosedBonusScoreWin : 0;
            PrintScoreLine("Bonus:", bonus);
            score += bonus;
        }

        if (objects.LocationOf(GameConstants.Magazine) == GameConstants.MagazineBonusLocation)
            score += GameConstants.MagazineInWittsEndScore;

        int hints = -GameConstants.HintScorePenalty * progress.HintsAccepted;
        if (hints != 0)
        {
            PrintScoreLine("Hints & instructions:", hints);
            score += hints;
        }

        score += GameConstants.BaseScore;
        if (score < 0)
            score = 0;

        PrintScoreLine("Score:", score);
        return score;
    }

    private static void PrintScoreLine(string label, int value)
    {
        Console.WriteLine($"{label,-22}{value,4}");
    }
}
