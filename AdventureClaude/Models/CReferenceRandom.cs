namespace AdventureClaude.Models;

using System;

/// <summary>
/// Reproduces the MSVC C runtime rand()/srand() sequence used by the C reference build.
/// </summary>
public sealed class CReferenceRandom : Random
{
    private uint state;

    public CReferenceRandom(int seed)
    {
        state = unchecked((uint)seed);
    }

    public override int Next()
    {
        return NextRaw();
    }

    public override int Next(int maxValue)
    {
        if (maxValue <= 0)
            throw new ArgumentOutOfRangeException(nameof(maxValue), "maxValue must be positive.");

        return Next(0, maxValue);
    }

    public override int Next(int minValue, int maxValue)
    {
        if (minValue > maxValue)
            throw new ArgumentOutOfRangeException(nameof(minValue), "minValue cannot be greater than maxValue.");

        uint range = unchecked((uint)(maxValue - minValue));
        if (range == 0)
            return minValue;

        uint partition = 32768u / range;
        uint discardLimit = partition * range - 1;
        uint value;

        do
        {
            value = unchecked((uint)NextRaw());
        }
        while (value > discardLimit);

        return minValue + unchecked((int)(value / partition));
    }

    protected override double Sample()
    {
        return NextRaw() / 32768.0;
    }

    private int NextRaw()
    {
        state = unchecked(state * 214013u + 2531011u);
        return unchecked((int)((state >> 16) & 0x7fffu));
    }
}
