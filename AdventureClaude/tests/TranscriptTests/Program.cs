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
                "Quit shows original score rating",
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
                "Building inventory flow",
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
                "Parser and object-not-here responses",
                InputLines("n", "xyzzy", "take lamp", "quit", "y"),
                [
                    "Nothing happens.",
                    "I see no lamp here.",
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
