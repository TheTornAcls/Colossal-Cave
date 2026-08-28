namespace AdventureClaude.Data;

using System.Collections.Generic;
using AdventureClaude.Models;

/// <summary>
/// Central lookup facade over the generated Adventure data tables.
/// </summary>
public static class AdventureData
{
    public static string Message(int messageId)
    {
        return GameMessages.GetMessage(messageId);
    }

    public static bool TryGetObject(int objectId, out GameObjectData objectData)
    {
        return GameObjects.Objects.TryGetValue(objectId, out objectData!);
    }

    public static string ObjectNameOrDefault(int objectId)
    {
        return TryGetObject(objectId, out GameObjectData objectData)
            ? objectData.Name
            : $"object #{objectId}";
    }

    public static bool TryGetObjectRoomDescription(int objectId, int state, out string roomDescription)
    {
        roomDescription = string.Empty;
        if (!TryGetObject(objectId, out GameObjectData objectData))
            return false;

        if (state < 0 || state >= objectData.States.Count)
            return false;

        roomDescription = objectData.States[state].RoomDescription;
        return !string.IsNullOrEmpty(roomDescription);
    }

    public static bool TryGetLongLocationDescription(int locationId, out string description)
    {
        return LocationDescriptions.LongDescriptions.TryGetValue(locationId, out description!);
    }

    public static bool TryGetShortLocationDescription(int locationId, out string description)
    {
        return LocationDescriptions.ShortDescriptions.TryGetValue(locationId, out description!);
    }

    public static List<TravelEntry> GetTravelOptions(int locationId)
    {
        return TravelData.GetTravelOptions(locationId);
    }

    public static bool AnalyzeWord(string word, out int type, out int value)
    {
        return Vocabulary.AnalyzeWord(word, out type, out value);
    }

    public static IEnumerable<string> GetMotionAndVerbWords()
    {
        return Vocabulary.GetMotionAndVerbWords();
    }
}
