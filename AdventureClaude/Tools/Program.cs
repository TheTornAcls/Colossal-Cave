using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace TravelDataGenerator;

class Program
{
    static void Main(string[] args)
    {
        string advCaveHPath = @"c:\Testing\source\repos\Colossal-Cave\Adventure\ADVCAVE.H";
        string outputPath = @"c:\Testing\source\repos\Colossal-Cave\AdventureClaude\Data\TravelData.cs";

        Console.WriteLine("Parsing ADVCAVE.H...");
        string content = File.ReadAllText(advCaveHPath);
        Dictionary<int, List<long>> travelData = [];

        Regex cavePattern = new(@"cave(\d{3})\[\]\s*=\s*\{([^}]+)\}");
        foreach (Match match in cavePattern.Matches(content))
        {
            int loc = int.Parse(match.Groups[1].Value);
            List<long> values = Regex.Matches(match.Groups[2].Value, @"-?\d+")
                .Cast<Match>()
                .Select(m => long.Parse(m.Value))
                .Where(v => v != -1)
                .ToList();
            travelData[loc] = values;
        }

        Console.WriteLine($"Found {travelData.Count} locations. Generating C# code...");

        StringBuilder sb = new();
        sb.AppendLine("namespace AdventureClaude.Data;");
        sb.AppendLine();
        sb.AppendLine("using System.Collections.Generic;");
        sb.AppendLine("using AdventureClaude.Models;");
        sb.AppendLine();
        sb.AppendLine("/// <summary>");
        sb.AppendLine("/// Contains all travel table data for cave navigation.");
        sb.AppendLine("/// Auto-generated from ADVCAVE.H - DO NOT EDIT MANUALLY");
        sb.AppendLine("/// Each location has a list of possible travel options (destination, verb, condition).");
        sb.AppendLine("/// </summary>");
        sb.AppendLine("public static class TravelData");
        sb.AppendLine("{");
        sb.AppendLine("    /// <summary>");
        sb.AppendLine("    /// Dictionary mapping location IDs (1-140) to their travel options.");
        sb.AppendLine("    /// </summary>");
        sb.AppendLine("    public static readonly Dictionary<int, List<TravelEntry>> TravelTable = new()");
        sb.AppendLine("    {");

        for (int loc = 1; loc <= 140; loc++)
        {
            if (travelData.TryGetValue(loc, out List<long>? values))
            {
                sb.AppendLine($"        {{{loc}, new List<TravelEntry>");
                sb.AppendLine("        {");
                foreach (long val in values)
                {
                    int cond = (int)(val % 1000);
                    int verb = (int)((val / 1000) % 1000);
                    int dest = (int)((val / 1000000) % 1000);
                    string comment = cond == 0 ? "always" : cond < 100 ? $"{cond}%" : $"cond:{cond}";
                    sb.AppendLine($"            new TravelEntry({dest}, {verb}, {cond}), // →{dest} v:{verb} {comment}");
                }
                sb.AppendLine("        }},");
            }
            else
            {
                sb.AppendLine($"        {{{loc}, new List<TravelEntry>()}}, // No travel options");
            }
        }

        sb.AppendLine("    };");
        sb.AppendLine();
        sb.AppendLine("    /// <summary>");
        sb.AppendLine("    /// Gets the travel options for a given location.");
        sb.AppendLine("    /// </summary>");
        sb.AppendLine("    public static List<TravelEntry> GetTravelOptions(int locationId)");
        sb.AppendLine("    {");
        sb.AppendLine("        if (TravelTable.TryGetValue(locationId, out List<TravelEntry>? options))");
        sb.AppendLine("            return options;");
        sb.AppendLine("        return new List<TravelEntry>();");
        sb.AppendLine("    }");
        sb.AppendLine("}");

        File.WriteAllText(outputPath, sb.ToString());
        Console.WriteLine($"✓ Generated: {outputPath}");
        Console.WriteLine($"  Total locations: {travelData.Count}");
        Console.WriteLine($"  Total travel entries: {travelData.Values.Sum(v => v.Count)}");
        Console.WriteLine();
        Console.WriteLine("Press any key to exit...");
        Console.ReadKey();
    }
}
