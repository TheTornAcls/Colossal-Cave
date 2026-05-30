namespace AdventureClaude.Models;

/// <summary>
/// Manages darkness and lamp mechanics for the adventure game.
/// Ported from DATABASE.C dark() function and TURN.C battery logic.
/// </summary>
public static class DarknessManager
{
    /// <summary>
    /// Determines if the current location is dark.
    /// Ported from dark() in DATABASE.C.
    /// </summary>
    /// <param name="gameState">Current game state</param>
    /// <returns>True if location is dark, false if there is light</returns>
    public static bool IsDark(GameState gameState)
    {
        // Original C logic:
        // return(!(cond[loc] & LIGHT) &&
        //     (!prop[LAMP] ||
        //         !here(LAMP)));
        
        // Location is dark if:
        // 1. Location doesn't have LIGHT flag, AND
        // 2. Either lamp is off (prop[LAMP] == 0) OR lamp is not present here
        
        bool locationHasLight = (gameState.LocationConditions[gameState.Location] & GameConstants.Light) != 0;
        bool lampIsOn = gameState.ObjectProperties[GameConstants.Lamp] == 1;
        bool lampIsHere = gameState.IsObjectHere(GameConstants.Lamp);
        
        // If location has natural light, it's not dark
        if (locationHasLight)
            return false;
        
        // Location is dark unless lamp is on AND present
        return !(lampIsOn && lampIsHere);
    }

    /// <summary>
    /// Checks and handles lamp battery status.
    /// Ported from stimer() in TURN.C.
    /// </summary>
    /// <param name="gameState">Current game state</param>
    /// <returns>Message to display, or null if no message</returns>
    public static string? CheckBatteryStatus(GameState gameState)
    {
        // Only process if lamp is on
        if (gameState.ObjectProperties[GameConstants.Lamp] != 1)
            return null;

        // Decrement battery life
        gameState.Limit--;

        // Check for battery depletion (0 turns remaining)
        if (gameState.Limit == 0)
        {
            gameState.ObjectProperties[GameConstants.Lamp] = 0; // Turn lamp off
            return Data.GameMessages.GetMessage(184); // "Your lamp has run out of power."
        }

        // Check for battery warning (≤30 turns remaining)
        if (gameState.Limit <= 30 && gameState.LampWarning == 0)
        {
            gameState.LampWarning = 1; // Set flag to prevent repeated warnings
            
            // Check if batteries are available for replacement
            bool batteriesExist = gameState.ObjectLocations[GameConstants.Batteries] != 0;
            bool batteriesHere = gameState.IsObjectHere(GameConstants.Batteries);
            bool lampIsHere = gameState.IsObjectHere(GameConstants.Lamp);
            
            // Automatic battery replacement if batteries are at this location
            if (batteriesHere && lampIsHere)
            {
                gameState.Limit += 2500; // Add 2500 turns from fresh batteries
                gameState.LampWarning = 0; // Reset warning flag
                gameState.ObjectLocations[GameConstants.Batteries] = 0; // Remove batteries from game
                return Data.GameMessages.GetMessage(188); // "Your lamp is getting dim.. I'm taking the liberty of replacing the batteries."
            }
            
            // Warning messages based on battery availability
            if (!batteriesExist)
            {
                return Data.GameMessages.GetMessage(183); // "Your lamp is getting dim.. You'd best start wrapping this up..."
            }
            else if (gameState.ObjectProperties[GameConstants.Batteries] != 0)
            {
                return Data.GameMessages.GetMessage(189); // "Your lamp is getting dim, and you're out of spare batteries..."
            }
            else
            {
                return Data.GameMessages.GetMessage(187); // "Your lamp is getting dim. You'd best go back for those batteries."
            }
        }

        return null;
    }

    /// <summary>
    /// Checks if player fell into a pit while moving in darkness.
    /// </summary>
    /// <param name="gameState">Current game state</param>
    /// <param name="random">Random number generator</param>
    /// <returns>True if player fell, false otherwise</returns>
    public static bool CheckDarknessDanger(GameState gameState, Random random)
    {
        // Only check if previous location was dark AND current location is dark
        if (!gameState.WizardDark || !IsDark(gameState))
            return false;

        // 35% chance of falling into pit
        int roll = random.Next(100);
        return roll < 35;
    }

    /// <summary>
    /// Updates the WizardDark flag for the next turn.
    /// Should be called after movement.
    /// </summary>
    /// <param name="gameState">Current game state</param>
    public static void UpdateDarknessState(GameState gameState)
    {
        gameState.WizardDark = IsDark(gameState);
    }
}
