namespace AdventureSharp;

using System;
using System.IO;

class Program
{
    static void Main(string[] args)
    {
        int rflag = 0;

        int argIndex = 0;
        while (argIndex < args.Length)
        {
            string arg = args[argIndex];
            if (!arg.StartsWith("-") && !arg.StartsWith("/"))
                break;
            char flag = char.ToLower(arg.Length > 1 ? arg[1] : '\0');
            switch (flag)
            {
                case 'r':
                    rflag++;
                    break;
                default:
                    Console.WriteLine($"unknown flag: {flag}");
                    break;
            }
            argIndex++;
        }

        AdventureGame game = new();
        game.MainGameLoop(rflag > 0);
    }
}
