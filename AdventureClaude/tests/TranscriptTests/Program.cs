namespace TranscriptTests;

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;

internal static class Program
{
    private static int Main()
    {
        string repositoryRoot = FindRepositoryRoot();
        string gameProjectPath = Path.Combine(repositoryRoot, "AdventureClaude.csproj");

        TestCase[] tests =
        [
            new TestCase(
                "Parity quit_score",
                InputLines("n", "quit", "y"),
                [
                    "You are standing at the end of a road before a small brick building.",
                    "Do you really want to quit now?",
                    "Treasures:",
                    "Score:",
                    "You are obviously a rank amateur. Better luck next time.",
                    "Thanks for playing!",
                ]),
            new TestCase(
                "Parity building_inventory",
                InputLines("n", "enter", "take lamp", "inventory", "drop lamp", "quit", "y"),
                [
                    "You are inside a building, a well house for a large spring.",
                    "There are some keys on the ground here.",
                    "There is a shiny brass lamp nearby.",
                    "There is tasty food here.",
                    "There is a bottle of water here.",
                    "You are currently holding the following:",
                    "Brass lantern",
                    "You are obviously a rank amateur. Better luck next time.",
                ]),
            new TestCase(
                "Parity magic_noop_and_missing_lamp",
                InputLines("n", "xyzzy", "take lamp", "quit", "y"),
                [
                    "Nothing happens.",
                    "I see no lamp here.",
                    "You are obviously a rank amateur. Better luck next time.",
                ]),
            new TestCase(
                "Parity get_keys_unlock_grate",
                InputLines("n", "enter", "take keys", "exit", "down", "unlock grate", "down", "quit", "y"),
                [
                    "You are inside a building, a well house for a large spring.",
                    "There are some keys on the ground here.",
                    "OK",
                    "You're at end of road again.",
                    "You are in a valley in the forest beside a stream tumbling along a rocky bed.",
                    "The grate is locked.",
                    "You can't go through a locked steel grate!",
                    "You are obviously a rank amateur. Better luck next time.",
                ]),
            new TestCase(
                "Parity take_food_eat",
                InputLines("n", "enter", "take food", "eat food", "quit", "y"),
                [
                    "You are inside a building, a well house for a large spring.",
                    "There is tasty food here.",
                    "OK",
                    "You are obviously a rank amateur. Better luck next time.",
                ]),
            new TestCase(
                "Parity basic_movement_cycle",
                InputLines("n", "enter", "out", "south", "north", "west", "east", "quit", "y"),
                [
                    "You are inside a building, a well house for a large spring.",
                    "You're at end of road again.",
                    "You are in a valley in the forest beside a stream tumbling along a rocky bed.",
                    "You have walked up a hill, still in the forest.",
                    "There is a building in the distance.",
                    "You are obviously a rank amateur. Better luck next time.",
                ]),
            new TestCase(
                "Parity lamp_on_off",
                InputLines("n", "enter", "take lamp", "light lamp", "extinguish lamp", "quit", "y"),
                [
                    "You are inside a building, a well house for a large spring.",
                    "There is a shiny brass lamp nearby.",
                    "OK",
                    "Your lamp is now on.",
                    "Your lamp is now off.",
                    "You are obviously a rank amateur. Better luck next time.",
                ]),
            new TestCase(
                "Parity keys_lock_unlock",
                InputLines("n", "enter", "take keys", "out", "down", "unlock grate", "lock grate", "quit", "y"),
                [
                    "You are inside a building, a well house for a large spring.",
                    "There are some keys on the ground here.",
                    "OK",
                    "The grate is locked.",
                    "It was already locked.",
                    "You are obviously a rank amateur. Better luck next time.",
                ]),
            new TestCase(
                "Parity inventory_empty",
                InputLines("n", "inventory", "quit", "y"),
                [
                    "You are standing at the end of a road before a small brick building.",
                    "You're not carrying anything.",
                    "You are obviously a rank amateur. Better luck next time.",
                ]),
            new TestCase(
                "Parity unknown_words",
                InputLines("n", "foobar", "take foobar", "quit", "y"),
                [
                    "I don't know that word.",
                    "I don't understand that!",
                    "You are obviously a rank amateur. Better luck next time.",
                ]),
            new TestCase(
                "Parity deeper_cave_entry_lamp_on",
                InputLines("n", "enter", "take lamp", "take keys", "light lamp", "out", "depression", "unlock grate", "down", "quit", "y"),
                [
                    "You are in a 20-foot depression floored with bare dirt.",
                    "The grate is locked.",
                    "The grate is now unlocked.",
                    "You are in a small chamber beneath a 3x3 steel grate to the surface.",
                    "The grate is open.",
                    "You are obviously a rank amateur. Better luck next time.",
                ]),
            new TestCase(
                "Parity deeper_cobble_debris_xyzzy",
                InputLines("n", "enter", "take lamp", "take keys", "light lamp", "out", "depression", "unlock grate", "down", "west", "west", "xyzzy", "quit", "y"),
                [
                    "You are crawling over cobbles in a low passage.",
                    "There is a small wicker cage discarded nearby.",
                    "You are in a debris room filled with stuff washed in from the surface.",
                    "Magic Word \"XYZZY\"",
                    "A three foot black rod with a rusty star on an end lies nearby.",
                    "You're inside building.",
                    "You are obviously a rank amateur. Better luck next time.",
                ]),
            new TestCase(
                "Parity deeper_cage_rod_inventory",
                InputLines("n", "enter", "take lamp", "take keys", "light lamp", "out", "depression", "unlock grate", "down", "west", "take cage", "west", "take rod", "inventory", "quit", "y"),
                [
                    "There is a small wicker cage discarded nearby.",
                    "A three foot black rod with a rusty star on an end lies nearby.",
                    "You are currently holding the following:",
                    "Set of keys.",
                    "Brass lantern",
                    "Wicker cage",
                    "Black rod",
                    "You are obviously a rank amateur. Better luck next time.",
                ]),
            new TestCase(
                "Parity deeper_bird_room_attempt",
                InputLines("n", "enter", "take lamp", "take keys", "light lamp", "out", "depression", "unlock grate", "down", "west", "west", "west", "take bird", "quit", "y"),
                [
                    "You are in an awkward sloping east/west canyon.",
                    "I see no bird here.",
                    "You are obviously a rank amateur. Better luck next time.",
                ]),
            new TestCase(
                "Parity deeper_nugget_score",
                InputLines("n", "enter", "take lamp", "take keys", "light lamp", "out", "depression", "unlock grate", "down", "west", "west", "west", "take nugget", "score", "quit", "y"),
                [
                    "You are in an awkward sloping east/west canyon.",
                    "I see no nugget here.",
                    "Treasures:               0",
                    "Score:                  36",
                    "You are obviously a rank amateur. Better luck next time.",
                ]),
            new TestCase(
                "Parity deeper_plugh_from_bird_chamber",
                InputLines("n", "enter", "take lamp", "take keys", "light lamp", "out", "depression", "unlock grate", "down", "west", "west", "west", "west", "plugh", "quit", "y"),
                [
                    "You are in a splendid chamber thirty feet high.",
                    "A cheerful little bird is sitting here singing.",
                    "Nothing happens.",
                    "You are obviously a rank amateur. Better luck next time.",
                ]),
            new TestCase(
                "Parity deeper_lamp_off_below_grate",
                InputLines("n", "enter", "take lamp", "take keys", "light lamp", "out", "depression", "unlock grate", "extinguish lamp", "down", "west", "quit", "y"),
                [
                    "The grate is now unlocked.",
                    "Your lamp is now off.",
                    "You are in a small chamber beneath a 3x3 steel grate to the surface.",
                    "You are crawling over cobbles in a low passage.",
                    "There is a small wicker cage discarded nearby.",
                    "You are obviously a rank amateur. Better luck next time.",
                ]),
            new TestCase(
                "Parity deeper_hall_king_snake_score",
                InputLines("n", "enter", "take lamp", "take keys", "light lamp", "out", "depression", "unlock grate", "down", "west", "west", "west", "west", "west", "down", "n", "quit", "y"),
                [
                    "You are at one end of a vast hall stretching forward out of sight to the",
                    "You are in the hall of the mountain king, with passages off in all directions.",
                    "A huge green fierce snake bars the way!",
                    "Getting well in:        25",
                    "Score:                  57",
                    "Your score qualifies you as a novice-class adventurer.",
                ]),
            new TestCase(
                "Parity deeper_nugget_inventory_score",
                InputLines("n", "enter", "take lamp", "take keys", "light lamp", "out", "depression", "unlock grate", "down", "west", "west", "west", "west", "west", "down", "south", "take nugget", "inventory", "score", "quit", "y"),
                [
                    "There is a large sparkling nugget of gold here!",
                    "You are currently holding the following:",
                    "Large gold nugget",
                    "Treasures:               2",
                    "Score:                  38",
                    "Score:                  34",
                    "You are obviously a rank amateur. Better luck next time.",
                ]),
            new TestCase(
                "Parity deeper_dwarf_pirate_warning",
                InputLines(
                    [
                        "n", "enter", "take lamp", "take keys", "light lamp", "out", "depression", "unlock grate", "down",
                        "west", "west", "west", "west", "west", "down",
                        "west", "east", "west", "east", "west", "east", "west", "east", "west", "east",
                        "west", "east", "west", "east", "west", "east", "west", "east", "west", "east",
                        "west", "east", "west", "east", "west", "east", "west", "east", "west", "east",
                        "quit", "y", "n", "y",
                    ]),
                [
                    "A little dwarf just walked around a corner, saw you, threw a little axe at",
                    "There is a little axe here.",
                    "There are faint rustling noises from the darkness behind you.",
                    "There is a threatening little dwarf in the room with you!",
                    "One sharp, nasty knife is thrown at you!",
                    "It misses!",
                    "You are obviously a rank amateur. Better luck next time.",
                ]),
            new TestCase(
                "Parity deeper_pirate_steals_nugget_inventory",
                InputLines(
                    [
                        "n", "enter", "take lamp", "take keys", "light lamp", "out", "depression", "unlock grate", "down",
                        "west", "west", "west", "west", "west", "down", "south", "take nugget", "north",
                        "west", "east", "west", "east", "west", "east", "west", "east", "west", "east",
                        "take axe",
                        "west", "east", "throw axe", "west", "east", "throw axe", "west", "east", "throw axe",
                        "west", "east", "throw axe", "west", "east", "throw axe", "west", "east", "throw axe",
                        "inventory", "quit", "y",
                    ]),
                [
                    "There is a large sparkling nugget of gold here!",
                    "Out from the shadows behind you pounces a bearded pirate!",
                    "I'll just take all this booty and hide it away with me chest deep",
                    "He snatches your treasure and vanishes into the gloom.",
                    "You are currently holding the following:",
                    "Set of keys.",
                    "Brass lantern",
                    "Treasures:               2",
                    "Score:                  34",
                    "You are obviously a rank amateur. Better luck next time.",
                ]),
            new TestCase(
                "Parity deeper_fissure_jump_death_no",
                InputLines("n", "enter", "take lamp", "take keys", "light lamp", "out", "depression", "unlock grate", "down", "west", "west", "west", "west", "west", "down", "west", "jump", "n"),
                [
                    "You are on the east bank of a fissure slicing clear across the hall.",
                    "You didn't make it.",
                    "Oh dear, you seem to have gotten yourself killed.",
                    "reincarnate you?",
                    "Score:                  26",
                    "You are obviously a rank amateur. Better luck next time.",
                ]),
            new TestCase(
                "Parity deeper_fissure_jump_reincarnate_yes",
                InputLines("n", "enter", "take lamp", "take keys", "light lamp", "out", "depression", "unlock grate", "down", "west", "west", "west", "west", "west", "down", "west", "jump", "y", "look", "inventory", "quit", "y"),
                [
                    "You are on the east bank of a fissure slicing clear across the hall.",
                    "You didn't make it.",
                    "Oh dear, you seem to have gotten yourself killed.",
                    "reincarnate you?",
                    "All right.  But don't blame me if something goes wr......",
                    "--- POOF !! ---",
                    "You are engulfed in a cloud of orange smoke.",
                    "You are inside a building, a well house for a large spring.",
                    "You're not carrying anything.",
                    "Survival:               20",
                    "Score:                  22",
                    "You are obviously a rank amateur. Better luck next time.",
                ]),
        ];

        int failures = 0;
        foreach (TestCase test in tests)
        {
            TestResult result = RunTranscript(gameProjectPath, test);
            if (result.Passed)
            {
                Console.WriteLine($"PASS {test.Name}");
                continue;
            }

            failures++;
            Console.WriteLine($"FAIL {test.Name}");
            Console.WriteLine(result.ErrorMessage);
            Console.WriteLine("---- output ----");
            Console.WriteLine(result.Output);
            Console.WriteLine("---- end output ----");
        }

        Console.WriteLine();
        Console.WriteLine($"{tests.Length - failures}/{tests.Length} transcript tests passed.");
        return failures == 0 ? 0 : 1;
    }

    private static TestResult RunTranscript(string gameProjectPath, TestCase test)
    {
        ProcessStartInfo startInfo = new()
        {
            FileName = "dotnet",
            Arguments = $"run --project \"{gameProjectPath}\" --no-launch-profile",
            RedirectStandardInput = true,
            RedirectStandardOutput = true,
            RedirectStandardError = true,
            UseShellExecute = false,
            CreateNoWindow = true,
            StandardOutputEncoding = Encoding.UTF8,
            StandardErrorEncoding = Encoding.UTF8,
        };

        using Process process = new() { StartInfo = startInfo };
        process.Start();
        process.StandardInput.Write(test.Input);
        process.StandardInput.Close();

        string output = process.StandardOutput.ReadToEnd();
        string error = process.StandardError.ReadToEnd();
        if (!process.WaitForExit(milliseconds: 30000))
        {
            process.Kill(entireProcessTree: true);
            return TestResult.Fail(output, "Game process timed out.");
        }

        string combinedOutput = Normalize(output + error);
        if (process.ExitCode != 0)
            return TestResult.Fail(combinedOutput, $"Expected exit code 0, got {process.ExitCode}.");

        List<string> missingSnippets = test.ExpectedSnippets
            .Where(snippet => !combinedOutput.Contains(Normalize(snippet), StringComparison.Ordinal))
            .ToList();

        if (missingSnippets.Count != 0)
        {
            string message = "Missing expected snippets:" + Environment.NewLine +
                string.Join(Environment.NewLine, missingSnippets.Select(snippet => $"  - {snippet}"));
            return TestResult.Fail(combinedOutput, message);
        }

        return TestResult.Pass(combinedOutput);
    }

    private static string FindRepositoryRoot()
    {
        DirectoryInfo? directory = new(Directory.GetCurrentDirectory());
        while (directory != null)
        {
            if (File.Exists(Path.Combine(directory.FullName, "AdventureClaude.csproj")))
                return directory.FullName;

            directory = directory.Parent;
        }

        throw new DirectoryNotFoundException("Could not find AdventureClaude.csproj from the current directory.");
    }

    private static string InputLines(params string[] lines)
    {
        return string.Join(Environment.NewLine, lines) + Environment.NewLine;
    }

    private static string Normalize(string value)
    {
        return value.Replace("\r\n", "\n").Replace('\r', '\n');
    }

    private sealed record TestCase(string Name, string Input, IReadOnlyList<string> ExpectedSnippets);

    private sealed record TestResult(bool Passed, string Output, string ErrorMessage)
    {
        public static TestResult Pass(string output)
        {
            return new TestResult(true, output, string.Empty);
        }

        public static TestResult Fail(string output, string errorMessage)
        {
            return new TestResult(false, output, errorMessage);
        }
    }
}
