using System;
using System.IO;

namespace AdventureSharp
{
    class Program
    {
        static void Main(string[] args)
        {
            int rflag = 0;
            string szFilePath = string.Empty;

            if (args.Length > 0)
            {
                szFilePath = Path.GetDirectoryName(args[0]) + Path.DirectorySeparatorChar;
            }
            else
            {
                szFilePath = Directory.GetCurrentDirectory() + Path.DirectorySeparatorChar;
            }

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

            AdventureGame game = new AdventureGame();
            game.InitPlay();
            game.OpenTextFiles(szFilePath);
            Console.WriteLine("AdventureSharp: Game initialized and files opened. Starting main game loop...");
            game.MainGameLoop(rflag > 0);
            game.CloseTextFiles();
        }
    }
}
