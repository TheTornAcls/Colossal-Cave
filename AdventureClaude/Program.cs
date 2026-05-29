namespace AdventureClaude;

/// <summary>
/// Main program entry point for the Adventure game.
/// This is the C# port of the classic Colossal Cave Adventure.
/// </summary>
internal class Program
    {
        /// <summary>
        /// Main entry point for the adventure game.
        /// </summary>
        /// <param name="args">Command line arguments</param>
        static void Main(string[] args)
        {
            Console.Title = "Colossal Cave Adventure - C# Port";
            Console.WriteLine("===========================================");
            Console.WriteLine("   COLOSSAL CAVE ADVENTURE");
            Console.WriteLine("   C# Port by Claude");
            Console.WriteLine("   Based on the original by Crowther & Woods");
            Console.WriteLine("===========================================");
            Console.WriteLine();

            try
            {
                AdventureGame game = new ();
                game.StartNewGame();
            }
            catch (Exception ex)
            {
                Console.WriteLine($"An error occurred: {ex.Message}");
                Console.WriteLine($"Stack trace: {ex.StackTrace}");
                Console.WriteLine("Press any key to exit...");
                Console.ReadKey();
            }

            Console.WriteLine();
            Console.WriteLine("Thanks for playing!");
            Console.WriteLine("Press any key to exit...");
            Console.ReadKey();
        }
    }
