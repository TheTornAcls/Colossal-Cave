namespace AdventureClaude.Models;

/// <summary>
/// Manages liquid mechanics for the adventure game.
/// Ported from DATABASE.C functions: liq(), liq2(), liqloc()
/// </summary>
public static class LiquidManager
{
    /// <summary>
    /// Converts an encoded bottle property value to a liquid object ID.
    /// Ported from liq2() in DATABASE.C.
    /// </summary>
    /// <param name="pbottle">Encoded value: 0=water, 1=empty, 2=oil</param>
    /// <returns>Object ID: WATER (21), 0 (empty), or OIL (22)</returns>
    public static int Liq2(int pbottle)
    {
        // Original C: return((1 - pbottle) * WATER + (pbottle >> 1) * (WATER + OIL));
        // When pbottle = 0: returns WATER (21)
        // When pbottle = 1: returns 0 (empty)
        // When pbottle = 2: returns OIL (22)
        return (1 - pbottle) * GameConstants.Water + (pbottle >> 1) * (GameConstants.Water + GameConstants.Oil);
    }

    /// <summary>
    /// Determines what liquid is currently in the bottle.
    /// Ported from liq() in DATABASE.C.
    /// </summary>
    /// <param name="gameState">Current game state</param>
    /// <returns>Object ID of liquid in bottle: WATER (21), OIL (22), or 0 (empty)</returns>
    public static int Liq(GameState gameState)
    {
        // Original C:
        // int i, j;
        // i = prop[BOTTLE];
        // j = -1 - i;
        // return(liq2(i > j ? i : j));
        
        int i = gameState.ObjectProperties[GameConstants.Bottle];
        int j = -1 - i;
        return Liq2(i > j ? i : j);
    }

    /// <summary>
    /// Determines what liquid exists at a specific location.
    /// Ported from liqloc() in DATABASE.C.
    /// </summary>
    /// <param name="gameState">Current game state</param>
    /// <param name="location">Location ID to check</param>
    /// <returns>Object ID of liquid at location: WATER (21), OIL (22), or 0 (none)</returns>
    public static int LiqLoc(GameState gameState, int location)
    {
        // Original C:
        // if (cond[iloc] & LIQUID)
        //     return(liq2(cond[iloc] & WATOIL));
        // else
        //     return(liq2(1));
        
        if ((gameState.LocationConditions[location] & GameConstants.Liquid) != 0)
        {
            return Liq2(gameState.LocationConditions[location] & GameConstants.WatOil);
        }
        else
        {
            return Liq2(1); // Returns 0 (no liquid)
        }
    }

    /// <summary>
    /// Checks if the bottle is empty.
    /// </summary>
    /// <param name="gameState">Current game state</param>
    /// <returns>True if bottle is empty, false otherwise</returns>
    public static bool IsBottleEmpty(GameState gameState)
    {
        return Liq(gameState) == 0;
    }

    /// <summary>
    /// Gets the bottle property value for a given liquid object ID.
    /// </summary>
    /// <param name="liquidId">Liquid object ID (WATER or OIL)</param>
    /// <returns>Property value to set on bottle: 0 for water, 2 for oil, 1 for empty</returns>
    public static int GetBottlePropertyForLiquid(int liquidId)
    {
        if (liquidId == GameConstants.Water)
            return 0;
        else if (liquidId == GameConstants.Oil)
            return 2;
        else
            return 1; // Empty
    }
}
