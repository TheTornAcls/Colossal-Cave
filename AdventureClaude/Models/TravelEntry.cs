namespace AdventureClaude.Models;

/// <summary>
/// Represents a single travel option from a location.
/// Decoded from the cave array encoding: dest*1000000 + verb*1000 + cond
/// </summary>
public class TravelEntry
{
    /// <summary>
    /// Destination location (1-140) or special code (300+, 500+)
    /// </summary>
    public int Destination { get; set; }

    /// <summary>
    /// Verb/direction code for this travel option
    /// </summary>
    public int Verb { get; set; }

    /// <summary>
    /// Condition code (0 = always, 1-99 = probability, 100+ = object checks)
    /// Format: CRR where C = condition type, RR = object ID
    /// </summary>
    public int Condition { get; set; }

    public TravelEntry(int destination, int verb, int condition)
    {
        Destination = destination;
        Verb = verb;
        Condition = condition;
    }

    /// <summary>
    /// Decodes a travel entry from the encoded cave format.
    /// </summary>
    public static TravelEntry Decode(long encodedValue)
    {
        int condition = (int)(encodedValue % 1000);
        int verb = (int)((encodedValue / 1000) % 1000);
        int destination = (int)((encodedValue / 1000000) % 1000);
        
        return new TravelEntry(destination, verb, condition);
    }

    /// <summary>
    /// Gets the condition type (first digit of 3-digit condition code).
    /// 0 = none/probability, 1 = carrying, 2 = present, 3-7 = property checks
    /// </summary>
    public int GetConditionType() => Condition / 100;

    /// <summary>
    /// Gets the object ID referenced by this condition (last 2 digits).
    /// </summary>
    public int GetObjectId() => Condition % 100;

    /// <summary>
    /// Checks if this travel option has no conditions.
    /// </summary>
    public bool IsUnconditional() => Condition == 0;

    /// <summary>
    /// Checks if this is a probability-based condition (1-99%).
    /// </summary>
    public bool IsProbability() => Condition > 0 && Condition < 100;
}
