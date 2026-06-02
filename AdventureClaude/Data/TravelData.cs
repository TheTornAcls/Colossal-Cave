namespace AdventureClaude.Data;

using System.Collections.Generic;
using AdventureClaude.Models;

/// <summary>
/// Contains all travel table data for cave navigation.
/// Auto-generated from ADVCAVE.H - DO NOT EDIT MANUALLY
/// Each location has a list of possible travel options (destination, verb, condition).
/// </summary>
public static class TravelData
{
    /// <summary>
    /// Dictionary mapping location IDs (1-140) to their travel options.
    /// </summary>
    public static readonly Dictionary<int, List<TravelEntry>> TravelTable = new()
    {
        {1, new List<TravelEntry>
        {
            new TravelEntry(2, 2, 0), // →2 v:2 always
            new TravelEntry(2, 44, 0), // →2 v:44 always
            new TravelEntry(2, 29, 0), // →2 v:29 always
            new TravelEntry(3, 3, 0), // →3 v:3 always
            new TravelEntry(3, 12, 0), // →3 v:12 always
            new TravelEntry(3, 19, 0), // →3 v:19 always
            new TravelEntry(3, 43, 0), // →3 v:43 always
            new TravelEntry(4, 13, 0), // →4 v:13 always
            new TravelEntry(4, 14, 0), // →4 v:14 always
            new TravelEntry(4, 46, 0), // →4 v:46 always
            new TravelEntry(4, 30, 0), // →4 v:30 always
            new TravelEntry(4, 4, 0), // →4 v:4 always
            new TravelEntry(5, 6, 0), // →5 v:6 always
            new TravelEntry(5, 45, 0), // →5 v:45 always
            new TravelEntry(8, 63, 0), // →8 v:63 always
        }},
        {2, new List<TravelEntry>
        {
            new TravelEntry(1, 2, 0), // →1 v:2 always
            new TravelEntry(1, 12, 0), // →1 v:12 always
            new TravelEntry(1, 7, 0), // →1 v:7 always
            new TravelEntry(1, 43, 0), // →1 v:43 always
            new TravelEntry(1, 45, 0), // →1 v:45 always
            new TravelEntry(1, 30, 0), // →1 v:30 always
            new TravelEntry(5, 6, 0), // →5 v:6 always
            new TravelEntry(5, 45, 0), // →5 v:45 always
            new TravelEntry(5, 46, 0), // →5 v:46 always
        }},
        {3, new List<TravelEntry>
        {
            new TravelEntry(1, 3, 0), // →1 v:3 always
            new TravelEntry(1, 11, 0), // →1 v:11 always
            new TravelEntry(1, 32, 0), // →1 v:32 always
            new TravelEntry(1, 44, 0), // →1 v:44 always
            new TravelEntry(11, 62, 0), // →11 v:62 always
            new TravelEntry(33, 65, 0), // →33 v:65 always
            new TravelEntry(79, 4, 0), // →79 v:4 always
            new TravelEntry(79, 5, 0), // →79 v:5 always
            new TravelEntry(79, 14, 0), // →79 v:14 always
        }},
        {4, new List<TravelEntry>
        {
            new TravelEntry(1, 12, 0), // →1 v:12 always
            new TravelEntry(1, 45, 0), // →1 v:45 always
            new TravelEntry(5, 6, 0), // →5 v:6 always
            new TravelEntry(5, 43, 0), // →5 v:43 always
            new TravelEntry(5, 44, 0), // →5 v:44 always
            new TravelEntry(5, 29, 0), // →5 v:29 always
            new TravelEntry(7, 4, 0), // →7 v:4 always
            new TravelEntry(1, 5, 0), // →1 v:5 always
            new TravelEntry(7, 46, 0), // →7 v:46 always
            new TravelEntry(7, 30, 0), // →7 v:30 always
            new TravelEntry(8, 63, 0), // →8 v:63 always
        }},
        {5, new List<TravelEntry>
        {
            new TravelEntry(4, 9, 0), // →4 v:9 always
            new TravelEntry(4, 43, 0), // →4 v:43 always
            new TravelEntry(4, 30, 0), // →4 v:30 always
            new TravelEntry(5, 6, 50), // →5 v:6 50%
            new TravelEntry(5, 7, 50), // →5 v:7 50%
            new TravelEntry(5, 45, 50), // →5 v:45 50%
            new TravelEntry(6, 6, 0), // →6 v:6 always
            new TravelEntry(6, 7, 0), // →6 v:7 always
            new TravelEntry(6, 45, 0), // →6 v:45 always
            new TravelEntry(5, 44, 0), // →5 v:44 always
            new TravelEntry(5, 46, 0), // →5 v:46 always
        }},
        {6, new List<TravelEntry>
        {
            new TravelEntry(1, 2, 0), // →1 v:2 always
            new TravelEntry(1, 45, 0), // →1 v:45 always
            new TravelEntry(4, 9, 0), // →4 v:9 always
            new TravelEntry(4, 43, 0), // →4 v:43 always
            new TravelEntry(4, 44, 0), // →4 v:44 always
            new TravelEntry(4, 30, 0), // →4 v:30 always
            new TravelEntry(5, 6, 0), // →5 v:6 always
            new TravelEntry(5, 46, 0), // →5 v:46 always
        }},
        {7, new List<TravelEntry>
        {
            new TravelEntry(1, 12, 0), // →1 v:12 always
            new TravelEntry(4, 45, 0), // →4 v:45 always
            new TravelEntry(5, 6, 0), // →5 v:6 always
            new TravelEntry(5, 43, 0), // →5 v:43 always
            new TravelEntry(5, 44, 0), // →5 v:44 always
            new TravelEntry(8, 4, 0), // →8 v:4 always
            new TravelEntry(4, 5, 0), // →4 v:5 always
            new TravelEntry(8, 15, 0), // →8 v:15 always
            new TravelEntry(8, 16, 0), // →8 v:16 always
            new TravelEntry(8, 46, 0), // →8 v:46 always
            new TravelEntry(595, 60, 0), // →595 v:60 always
            new TravelEntry(595, 14, 0), // →595 v:14 always
            new TravelEntry(595, 30, 0), // →595 v:30 always
        }},
        {8, new List<TravelEntry>
        {
            new TravelEntry(5, 6, 0), // →5 v:6 always
            new TravelEntry(5, 43, 0), // →5 v:43 always
            new TravelEntry(5, 46, 0), // →5 v:46 always
            new TravelEntry(5, 44, 0), // →5 v:44 always
            new TravelEntry(1, 12, 0), // →1 v:12 always
            new TravelEntry(7, 5, 0), // →7 v:5 always
            new TravelEntry(7, 13, 0), // →7 v:13 always
            new TravelEntry(7, 45, 0), // →7 v:45 always
            new TravelEntry(9, 3, 303), // →9 v:3 cond:303
            new TravelEntry(9, 19, 303), // →9 v:19 cond:303
            new TravelEntry(9, 30, 303), // →9 v:30 cond:303
            new TravelEntry(593, 3, 0), // →593 v:3 always
        }},
        {9, new List<TravelEntry>
        {
            new TravelEntry(8, 11, 303), // →8 v:11 cond:303
            new TravelEntry(8, 29, 303), // →8 v:29 cond:303
            new TravelEntry(593, 11, 0), // →593 v:11 always
            new TravelEntry(10, 17, 0), // →10 v:17 always
            new TravelEntry(10, 18, 0), // →10 v:18 always
            new TravelEntry(10, 19, 0), // →10 v:19 always
            new TravelEntry(10, 44, 0), // →10 v:44 always
            new TravelEntry(14, 31, 0), // →14 v:31 always
            new TravelEntry(11, 51, 0), // →11 v:51 always
        }},
        {10, new List<TravelEntry>
        {
            new TravelEntry(9, 11, 0), // →9 v:11 always
            new TravelEntry(9, 20, 0), // →9 v:20 always
            new TravelEntry(9, 21, 0), // →9 v:21 always
            new TravelEntry(9, 43, 0), // →9 v:43 always
            new TravelEntry(11, 19, 0), // →11 v:19 always
            new TravelEntry(11, 22, 0), // →11 v:22 always
            new TravelEntry(11, 44, 0), // →11 v:44 always
            new TravelEntry(11, 51, 0), // →11 v:51 always
            new TravelEntry(14, 31, 0), // →14 v:31 always
        }},
        {11, new List<TravelEntry>
        {
            new TravelEntry(8, 63, 303), // →8 v:63 cond:303
            new TravelEntry(9, 64, 0), // →9 v:64 always
            new TravelEntry(10, 17, 0), // →10 v:17 always
            new TravelEntry(10, 18, 0), // →10 v:18 always
            new TravelEntry(10, 23, 0), // →10 v:23 always
            new TravelEntry(10, 24, 0), // →10 v:24 always
            new TravelEntry(10, 43, 0), // →10 v:43 always
            new TravelEntry(12, 25, 0), // →12 v:25 always
            new TravelEntry(12, 19, 0), // →12 v:19 always
            new TravelEntry(12, 29, 0), // →12 v:29 always
            new TravelEntry(12, 44, 0), // →12 v:44 always
            new TravelEntry(3, 62, 0), // →3 v:62 always
            new TravelEntry(14, 31, 0), // →14 v:31 always
        }},
        {12, new List<TravelEntry>
        {
            new TravelEntry(8, 63, 303), // →8 v:63 cond:303
            new TravelEntry(9, 64, 0), // →9 v:64 always
            new TravelEntry(11, 30, 0), // →11 v:30 always
            new TravelEntry(11, 43, 0), // →11 v:43 always
            new TravelEntry(11, 51, 0), // →11 v:51 always
            new TravelEntry(13, 19, 0), // →13 v:19 always
            new TravelEntry(13, 29, 0), // →13 v:29 always
            new TravelEntry(13, 44, 0), // →13 v:44 always
            new TravelEntry(14, 31, 0), // →14 v:31 always
        }},
        {13, new List<TravelEntry>
        {
            new TravelEntry(8, 63, 303), // →8 v:63 cond:303
            new TravelEntry(9, 64, 0), // →9 v:64 always
            new TravelEntry(11, 51, 0), // →11 v:51 always
            new TravelEntry(12, 25, 0), // →12 v:25 always
            new TravelEntry(12, 43, 0), // →12 v:43 always
            new TravelEntry(14, 23, 0), // →14 v:23 always
            new TravelEntry(14, 31, 0), // →14 v:31 always
            new TravelEntry(14, 44, 0), // →14 v:44 always
        }},
        {14, new List<TravelEntry>
        {
            new TravelEntry(8, 63, 303), // →8 v:63 cond:303
            new TravelEntry(9, 64, 0), // →9 v:64 always
            new TravelEntry(11, 51, 0), // →11 v:51 always
            new TravelEntry(13, 23, 0), // →13 v:23 always
            new TravelEntry(13, 43, 0), // →13 v:43 always
            new TravelEntry(20, 30, 150), // →20 v:30 cond:150
            new TravelEntry(20, 31, 150), // →20 v:31 cond:150
            new TravelEntry(20, 34, 150), // →20 v:34 cond:150
            new TravelEntry(15, 30, 0), // →15 v:30 always
            new TravelEntry(16, 33, 0), // →16 v:33 always
            new TravelEntry(16, 44, 0), // →16 v:44 always
        }},
        {15, new List<TravelEntry>
        {
            new TravelEntry(18, 36, 0), // →18 v:36 always
            new TravelEntry(18, 46, 0), // →18 v:46 always
            new TravelEntry(17, 7, 0), // →17 v:7 always
            new TravelEntry(17, 38, 0), // →17 v:38 always
            new TravelEntry(17, 44, 0), // →17 v:44 always
            new TravelEntry(19, 10, 0), // →19 v:10 always
            new TravelEntry(19, 30, 0), // →19 v:30 always
            new TravelEntry(19, 45, 0), // →19 v:45 always
            new TravelEntry(22, 29, 150), // →22 v:29 cond:150
            new TravelEntry(22, 31, 150), // →22 v:31 cond:150
            new TravelEntry(22, 34, 150), // →22 v:34 cond:150
            new TravelEntry(22, 35, 150), // →22 v:35 cond:150
            new TravelEntry(22, 23, 150), // →22 v:23 cond:150
            new TravelEntry(22, 43, 150), // →22 v:43 cond:150
            new TravelEntry(14, 29, 0), // →14 v:29 always
            new TravelEntry(34, 55, 0), // →34 v:55 always
        }},
        {16, new List<TravelEntry>
        {
            new TravelEntry(14, 1, 0), // →14 v:1 always
        }},
        {17, new List<TravelEntry>
        {
            new TravelEntry(15, 38, 0), // →15 v:38 always
            new TravelEntry(15, 43, 0), // →15 v:43 always
            new TravelEntry(596, 39, 312), // →596 v:39 cond:312
            new TravelEntry(21, 7, 412), // →21 v:7 cond:412
            new TravelEntry(597, 41, 412), // →597 v:41 cond:412
            new TravelEntry(597, 42, 412), // →597 v:42 cond:412
            new TravelEntry(597, 44, 412), // →597 v:44 cond:412
            new TravelEntry(597, 69, 412), // →597 v:69 cond:412
            new TravelEntry(27, 41, 0), // →27 v:41 always
        }},
        {18, new List<TravelEntry>
        {
            new TravelEntry(15, 38, 0), // →15 v:38 always
            new TravelEntry(15, 11, 0), // →15 v:11 always
            new TravelEntry(15, 45, 0), // →15 v:45 always
        }},
        {19, new List<TravelEntry>
        {
            new TravelEntry(15, 10, 0), // →15 v:10 always
            new TravelEntry(15, 29, 0), // →15 v:29 always
            new TravelEntry(15, 43, 0), // →15 v:43 always
            new TravelEntry(28, 45, 311), // →28 v:45 cond:311
            new TravelEntry(28, 36, 311), // →28 v:36 cond:311
            new TravelEntry(29, 46, 311), // →29 v:46 cond:311
            new TravelEntry(29, 37, 311), // →29 v:37 cond:311
            new TravelEntry(30, 44, 311), // →30 v:44 cond:311
            new TravelEntry(30, 7, 311), // →30 v:7 cond:311
            new TravelEntry(32, 45, 0), // →32 v:45 always
            new TravelEntry(74, 49, 35), // →74 v:49 35%
            new TravelEntry(32, 49, 211), // →32 v:49 cond:211
            new TravelEntry(74, 66, 0), // →74 v:66 always
        }},
        {20, new List<TravelEntry>
        {
            new TravelEntry(0, 1, 0), // →0 v:1 always
        }},
        {21, new List<TravelEntry>
        {
            new TravelEntry(0, 1, 0), // →0 v:1 always
        }},
        {22, new List<TravelEntry>
        {
            new TravelEntry(15, 1, 0), // →15 v:1 always
        }},
        {23, new List<TravelEntry>
        {
            new TravelEntry(67, 43, 0), // →67 v:43 always
            new TravelEntry(67, 42, 0), // →67 v:42 always
            new TravelEntry(68, 44, 0), // →68 v:44 always
            new TravelEntry(68, 61, 0), // →68 v:61 always
            new TravelEntry(25, 30, 0), // →25 v:30 always
            new TravelEntry(25, 31, 0), // →25 v:31 always
            new TravelEntry(648, 52, 0), // →648 v:52 always
        }},
        {24, new List<TravelEntry>
        {
            new TravelEntry(67, 29, 0), // →67 v:29 always
            new TravelEntry(67, 11, 0), // →67 v:11 always
        }},
        {25, new List<TravelEntry>
        {
            new TravelEntry(23, 29, 0), // →23 v:29 always
            new TravelEntry(23, 11, 0), // →23 v:11 always
            new TravelEntry(31, 56, 724), // →31 v:56 cond:724
            new TravelEntry(26, 56, 0), // →26 v:56 always
        }},
        {26, new List<TravelEntry>
        {
            new TravelEntry(88, 1, 0), // →88 v:1 always
        }},
        {27, new List<TravelEntry>
        {
            new TravelEntry(596, 39, 312), // →596 v:39 cond:312
            new TravelEntry(21, 7, 412), // →21 v:7 cond:412
            new TravelEntry(597, 41, 412), // →597 v:41 cond:412
            new TravelEntry(597, 42, 412), // →597 v:42 cond:412
            new TravelEntry(597, 43, 412), // →597 v:43 cond:412
            new TravelEntry(597, 69, 412), // →597 v:69 cond:412
            new TravelEntry(17, 41, 0), // →17 v:41 always
            new TravelEntry(40, 45, 0), // →40 v:45 always
            new TravelEntry(41, 44, 0), // →41 v:44 always
        }},
        {28, new List<TravelEntry>
        {
            new TravelEntry(19, 38, 0), // →19 v:38 always
            new TravelEntry(19, 11, 0), // →19 v:11 always
            new TravelEntry(19, 46, 0), // →19 v:46 always
            new TravelEntry(33, 45, 0), // →33 v:45 always
            new TravelEntry(33, 55, 0), // →33 v:55 always
            new TravelEntry(36, 30, 0), // →36 v:30 always
            new TravelEntry(36, 52, 0), // →36 v:52 always
        }},
        {29, new List<TravelEntry>
        {
            new TravelEntry(19, 38, 0), // →19 v:38 always
            new TravelEntry(19, 11, 0), // →19 v:11 always
            new TravelEntry(19, 45, 0), // →19 v:45 always
        }},
        {30, new List<TravelEntry>
        {
            new TravelEntry(19, 38, 0), // →19 v:38 always
            new TravelEntry(19, 11, 0), // →19 v:11 always
            new TravelEntry(19, 43, 0), // →19 v:43 always
            new TravelEntry(62, 44, 0), // →62 v:44 always
            new TravelEntry(62, 29, 0), // →62 v:29 always
        }},
        {31, new List<TravelEntry>
        {
            new TravelEntry(89, 1, 524), // →89 v:1 cond:524
            new TravelEntry(90, 1, 0), // →90 v:1 always
        }},
        {32, new List<TravelEntry>
        {
            new TravelEntry(19, 1, 0), // →19 v:1 always
        }},
        {33, new List<TravelEntry>
        {
            new TravelEntry(3, 65, 0), // →3 v:65 always
            new TravelEntry(28, 46, 0), // →28 v:46 always
            new TravelEntry(34, 43, 0), // →34 v:43 always
            new TravelEntry(34, 53, 0), // →34 v:53 always
            new TravelEntry(34, 54, 0), // →34 v:54 always
            new TravelEntry(35, 44, 0), // →35 v:44 always
            new TravelEntry(302, 71, 159), // →302 v:71 cond:159
            new TravelEntry(100, 71, 0), // →100 v:71 always
        }},
        {34, new List<TravelEntry>
        {
            new TravelEntry(33, 30, 0), // →33 v:30 always
            new TravelEntry(33, 55, 0), // →33 v:55 always
            new TravelEntry(15, 29, 0), // →15 v:29 always
        }},
        {35, new List<TravelEntry>
        {
            new TravelEntry(33, 43, 0), // →33 v:43 always
            new TravelEntry(33, 55, 0), // →33 v:55 always
            new TravelEntry(20, 39, 0), // →20 v:39 always
        }},
        {36, new List<TravelEntry>
        {
            new TravelEntry(37, 43, 0), // →37 v:43 always
            new TravelEntry(37, 17, 0), // →37 v:17 always
            new TravelEntry(28, 29, 0), // →28 v:29 always
            new TravelEntry(28, 52, 0), // →28 v:52 always
            new TravelEntry(39, 44, 0), // →39 v:44 always
            new TravelEntry(65, 70, 0), // →65 v:70 always
        }},
        {37, new List<TravelEntry>
        {
            new TravelEntry(36, 44, 0), // →36 v:44 always
            new TravelEntry(36, 17, 0), // →36 v:17 always
            new TravelEntry(38, 30, 0), // →38 v:30 always
            new TravelEntry(38, 31, 0), // →38 v:31 always
            new TravelEntry(38, 56, 0), // →38 v:56 always
        }},
        {38, new List<TravelEntry>
        {
            new TravelEntry(37, 56, 0), // →37 v:56 always
            new TravelEntry(37, 29, 0), // →37 v:29 always
            new TravelEntry(37, 11, 0), // →37 v:11 always
            new TravelEntry(595, 60, 0), // →595 v:60 always
            new TravelEntry(595, 14, 0), // →595 v:14 always
            new TravelEntry(595, 30, 0), // →595 v:30 always
            new TravelEntry(595, 4, 0), // →595 v:4 always
            new TravelEntry(595, 5, 0), // →595 v:5 always
        }},
        {39, new List<TravelEntry>
        {
            new TravelEntry(36, 43, 0), // →36 v:43 always
            new TravelEntry(36, 23, 0), // →36 v:23 always
            new TravelEntry(64, 30, 0), // →64 v:30 always
            new TravelEntry(64, 52, 0), // →64 v:52 always
            new TravelEntry(64, 58, 0), // →64 v:58 always
            new TravelEntry(65, 70, 0), // →65 v:70 always
        }},
        {40, new List<TravelEntry>
        {
            new TravelEntry(41, 1, 0), // →41 v:1 always
        }},
        {41, new List<TravelEntry>
        {
            new TravelEntry(42, 46, 0), // →42 v:46 always
            new TravelEntry(42, 29, 0), // →42 v:29 always
            new TravelEntry(42, 23, 0), // →42 v:23 always
            new TravelEntry(42, 56, 0), // →42 v:56 always
            new TravelEntry(27, 43, 0), // →27 v:43 always
            new TravelEntry(59, 45, 0), // →59 v:45 always
            new TravelEntry(60, 44, 0), // →60 v:44 always
            new TravelEntry(60, 17, 0), // →60 v:17 always
        }},
        {42, new List<TravelEntry>
        {
            new TravelEntry(41, 29, 0), // →41 v:29 always
            new TravelEntry(42, 45, 0), // →42 v:45 always
            new TravelEntry(43, 43, 0), // →43 v:43 always
            new TravelEntry(45, 46, 0), // →45 v:46 always
            new TravelEntry(80, 44, 0), // →80 v:44 always
        }},
        {43, new List<TravelEntry>
        {
            new TravelEntry(42, 44, 0), // →42 v:44 always
            new TravelEntry(44, 46, 0), // →44 v:46 always
            new TravelEntry(45, 43, 0), // →45 v:43 always
        }},
        {44, new List<TravelEntry>
        {
            new TravelEntry(43, 43, 0), // →43 v:43 always
            new TravelEntry(48, 30, 0), // →48 v:30 always
            new TravelEntry(50, 46, 0), // →50 v:46 always
            new TravelEntry(82, 45, 0), // →82 v:45 always
        }},
        {45, new List<TravelEntry>
        {
            new TravelEntry(42, 44, 0), // →42 v:44 always
            new TravelEntry(43, 45, 0), // →43 v:45 always
            new TravelEntry(46, 43, 0), // →46 v:43 always
            new TravelEntry(47, 46, 0), // →47 v:46 always
            new TravelEntry(87, 29, 0), // →87 v:29 always
            new TravelEntry(87, 30, 0), // →87 v:30 always
        }},
        {46, new List<TravelEntry>
        {
            new TravelEntry(45, 44, 0), // →45 v:44 always
            new TravelEntry(45, 11, 0), // →45 v:11 always
        }},
        {47, new List<TravelEntry>
        {
            new TravelEntry(45, 43, 0), // →45 v:43 always
            new TravelEntry(45, 11, 0), // →45 v:11 always
        }},
        {48, new List<TravelEntry>
        {
            new TravelEntry(44, 29, 0), // →44 v:29 always
            new TravelEntry(44, 11, 0), // →44 v:11 always
        }},
        {49, new List<TravelEntry>
        {
            new TravelEntry(50, 43, 0), // →50 v:43 always
            new TravelEntry(51, 44, 0), // →51 v:44 always
        }},
        {50, new List<TravelEntry>
        {
            new TravelEntry(44, 43, 0), // →44 v:43 always
            new TravelEntry(49, 44, 0), // →49 v:44 always
            new TravelEntry(51, 30, 0), // →51 v:30 always
            new TravelEntry(52, 46, 0), // →52 v:46 always
        }},
        {51, new List<TravelEntry>
        {
            new TravelEntry(49, 44, 0), // →49 v:44 always
            new TravelEntry(50, 29, 0), // →50 v:29 always
            new TravelEntry(52, 43, 0), // →52 v:43 always
            new TravelEntry(53, 46, 0), // →53 v:46 always
        }},
        {52, new List<TravelEntry>
        {
            new TravelEntry(50, 44, 0), // →50 v:44 always
            new TravelEntry(51, 43, 0), // →51 v:43 always
            new TravelEntry(52, 46, 0), // →52 v:46 always
            new TravelEntry(53, 29, 0), // →53 v:29 always
            new TravelEntry(55, 45, 0), // →55 v:45 always
            new TravelEntry(86, 30, 0), // →86 v:30 always
        }},
        {53, new List<TravelEntry>
        {
            new TravelEntry(51, 44, 0), // →51 v:44 always
            new TravelEntry(52, 45, 0), // →52 v:45 always
            new TravelEntry(54, 46, 0), // →54 v:46 always
        }},
        {54, new List<TravelEntry>
        {
            new TravelEntry(53, 44, 0), // →53 v:44 always
            new TravelEntry(53, 11, 0), // →53 v:11 always
        }},
        {55, new List<TravelEntry>
        {
            new TravelEntry(52, 44, 0), // →52 v:44 always
            new TravelEntry(55, 45, 0), // →55 v:45 always
            new TravelEntry(56, 30, 0), // →56 v:30 always
            new TravelEntry(57, 43, 0), // →57 v:43 always
        }},
        {56, new List<TravelEntry>
        {
            new TravelEntry(55, 29, 0), // →55 v:29 always
            new TravelEntry(55, 11, 0), // →55 v:11 always
        }},
        {57, new List<TravelEntry>
        {
            new TravelEntry(13, 30, 0), // →13 v:30 always
            new TravelEntry(13, 56, 0), // →13 v:56 always
            new TravelEntry(55, 44, 0), // →55 v:44 always
            new TravelEntry(58, 46, 0), // →58 v:46 always
            new TravelEntry(83, 45, 0), // →83 v:45 always
            new TravelEntry(84, 43, 0), // →84 v:43 always
        }},
        {58, new List<TravelEntry>
        {
            new TravelEntry(57, 43, 0), // →57 v:43 always
            new TravelEntry(57, 11, 0), // →57 v:11 always
        }},
        {59, new List<TravelEntry>
        {
            new TravelEntry(27, 1, 0), // →27 v:1 always
        }},
        {60, new List<TravelEntry>
        {
            new TravelEntry(41, 43, 0), // →41 v:43 always
            new TravelEntry(41, 29, 0), // →41 v:29 always
            new TravelEntry(41, 17, 0), // →41 v:17 always
            new TravelEntry(61, 44, 0), // →61 v:44 always
            new TravelEntry(62, 45, 0), // →62 v:45 always
            new TravelEntry(62, 30, 0), // →62 v:30 always
            new TravelEntry(62, 52, 0), // →62 v:52 always
        }},
        {61, new List<TravelEntry>
        {
            new TravelEntry(60, 43, 0), // →60 v:43 always
            new TravelEntry(62, 45, 0), // →62 v:45 always
            new TravelEntry(107, 46, 100), // →107 v:46 cond:100
        }},
        {62, new List<TravelEntry>
        {
            new TravelEntry(60, 44, 0), // →60 v:44 always
            new TravelEntry(63, 45, 0), // →63 v:45 always
            new TravelEntry(30, 43, 0), // →30 v:43 always
            new TravelEntry(61, 46, 0), // →61 v:46 always
        }},
        {63, new List<TravelEntry>
        {
            new TravelEntry(62, 46, 0), // →62 v:46 always
            new TravelEntry(62, 11, 0), // →62 v:11 always
        }},
        {64, new List<TravelEntry>
        {
            new TravelEntry(39, 29, 0), // →39 v:29 always
            new TravelEntry(39, 56, 0), // →39 v:56 always
            new TravelEntry(39, 59, 0), // →39 v:59 always
            new TravelEntry(65, 44, 0), // →65 v:44 always
            new TravelEntry(65, 70, 0), // →65 v:70 always
            new TravelEntry(103, 45, 0), // →103 v:45 always
            new TravelEntry(103, 74, 0), // →103 v:74 always
            new TravelEntry(106, 43, 0), // →106 v:43 always
        }},
        {65, new List<TravelEntry>
        {
            new TravelEntry(64, 43, 0), // →64 v:43 always
            new TravelEntry(66, 44, 0), // →66 v:44 always
            new TravelEntry(588, 46, 80), // →588 v:46 80%
            new TravelEntry(68, 61, 0), // →68 v:61 always
            new TravelEntry(588, 29, 80), // →588 v:29 80%
            new TravelEntry(70, 29, 50), // →70 v:29 50%
            new TravelEntry(39, 29, 0), // →39 v:29 always
            new TravelEntry(588, 45, 60), // →588 v:45 60%
            new TravelEntry(72, 45, 75), // →72 v:45 75%
            new TravelEntry(71, 45, 0), // →71 v:45 always
            new TravelEntry(588, 30, 80), // →588 v:30 80%
            new TravelEntry(106, 30, 0), // →106 v:30 always
        }},
        {66, new List<TravelEntry>
        {
            new TravelEntry(65, 47, 0), // →65 v:47 always
            new TravelEntry(67, 44, 0), // →67 v:44 always
            new TravelEntry(588, 46, 80), // →588 v:46 80%
            new TravelEntry(77, 25, 0), // →77 v:25 always
            new TravelEntry(96, 43, 0), // →96 v:43 always
            new TravelEntry(588, 50, 50), // →588 v:50 50%
            new TravelEntry(97, 72, 0), // →97 v:72 always
        }},
        {67, new List<TravelEntry>
        {
            new TravelEntry(66, 43, 0), // →66 v:43 always
            new TravelEntry(23, 44, 0), // →23 v:44 always
            new TravelEntry(23, 42, 0), // →23 v:42 always
            new TravelEntry(24, 30, 0), // →24 v:30 always
            new TravelEntry(24, 31, 0), // →24 v:31 always
        }},
        {68, new List<TravelEntry>
        {
            new TravelEntry(23, 46, 0), // →23 v:46 always
            new TravelEntry(69, 29, 0), // →69 v:29 always
            new TravelEntry(69, 56, 0), // →69 v:56 always
            new TravelEntry(65, 45, 0), // →65 v:45 always
        }},
        {69, new List<TravelEntry>
        {
            new TravelEntry(68, 30, 0), // →68 v:30 always
            new TravelEntry(68, 61, 0), // →68 v:61 always
            new TravelEntry(120, 46, 331), // →120 v:46 cond:331
            new TravelEntry(119, 46, 0), // →119 v:46 always
            new TravelEntry(109, 45, 0), // →109 v:45 always
            new TravelEntry(113, 75, 0), // →113 v:75 always
        }},
        {70, new List<TravelEntry>
        {
            new TravelEntry(71, 45, 0), // →71 v:45 always
            new TravelEntry(65, 30, 0), // →65 v:30 always
            new TravelEntry(65, 23, 0), // →65 v:23 always
            new TravelEntry(111, 46, 0), // →111 v:46 always
        }},
        {71, new List<TravelEntry>
        {
            new TravelEntry(65, 48, 0), // →65 v:48 always
            new TravelEntry(70, 46, 0), // →70 v:46 always
            new TravelEntry(110, 45, 0), // →110 v:45 always
        }},
        {72, new List<TravelEntry>
        {
            new TravelEntry(65, 70, 0), // →65 v:70 always
            new TravelEntry(118, 49, 0), // →118 v:49 always
            new TravelEntry(73, 45, 0), // →73 v:45 always
            new TravelEntry(97, 48, 0), // →97 v:48 always
            new TravelEntry(97, 72, 0), // →97 v:72 always
        }},
        {73, new List<TravelEntry>
        {
            new TravelEntry(72, 46, 0), // →72 v:46 always
            new TravelEntry(72, 17, 0), // →72 v:17 always
            new TravelEntry(72, 11, 0), // →72 v:11 always
        }},
        {74, new List<TravelEntry>
        {
            new TravelEntry(19, 43, 0), // →19 v:43 always
            new TravelEntry(120, 44, 331), // →120 v:44 cond:331
            new TravelEntry(121, 44, 0), // →121 v:44 always
            new TravelEntry(75, 30, 0), // →75 v:30 always
        }},
        {75, new List<TravelEntry>
        {
            new TravelEntry(76, 46, 0), // →76 v:46 always
            new TravelEntry(77, 45, 0), // →77 v:45 always
        }},
        {76, new List<TravelEntry>
        {
            new TravelEntry(75, 45, 0), // →75 v:45 always
        }},
        {77, new List<TravelEntry>
        {
            new TravelEntry(75, 43, 0), // →75 v:43 always
            new TravelEntry(78, 44, 0), // →78 v:44 always
            new TravelEntry(66, 45, 0), // →66 v:45 always
            new TravelEntry(66, 17, 0), // →66 v:17 always
        }},
        {78, new List<TravelEntry>
        {
            new TravelEntry(77, 46, 0), // →77 v:46 always
        }},
        {79, new List<TravelEntry>
        {
            new TravelEntry(3, 1, 0), // →3 v:1 always
        }},
        {80, new List<TravelEntry>
        {
            new TravelEntry(42, 45, 0), // →42 v:45 always
            new TravelEntry(80, 44, 0), // →80 v:44 always
            new TravelEntry(80, 46, 0), // →80 v:46 always
            new TravelEntry(81, 43, 0), // →81 v:43 always
        }},
        {81, new List<TravelEntry>
        {
            new TravelEntry(80, 44, 0), // →80 v:44 always
            new TravelEntry(80, 11, 0), // →80 v:11 always
        }},
        {82, new List<TravelEntry>
        {
            new TravelEntry(44, 46, 0), // →44 v:46 always
            new TravelEntry(44, 11, 0), // →44 v:11 always
        }},
        {83, new List<TravelEntry>
        {
            new TravelEntry(57, 46, 0), // →57 v:46 always
            new TravelEntry(84, 43, 0), // →84 v:43 always
            new TravelEntry(85, 44, 0), // →85 v:44 always
        }},
        {84, new List<TravelEntry>
        {
            new TravelEntry(57, 45, 0), // →57 v:45 always
            new TravelEntry(83, 44, 0), // →83 v:44 always
            new TravelEntry(114, 50, 0), // →114 v:50 always
        }},
        {85, new List<TravelEntry>
        {
            new TravelEntry(83, 43, 0), // →83 v:43 always
            new TravelEntry(83, 11, 0), // →83 v:11 always
        }},
        {86, new List<TravelEntry>
        {
            new TravelEntry(52, 29, 0), // →52 v:29 always
            new TravelEntry(52, 11, 0), // →52 v:11 always
        }},
        {87, new List<TravelEntry>
        {
            new TravelEntry(45, 29, 0), // →45 v:29 always
            new TravelEntry(45, 30, 0), // →45 v:30 always
        }},
        {88, new List<TravelEntry>
        {
            new TravelEntry(25, 30, 0), // →25 v:30 always
            new TravelEntry(25, 56, 0), // →25 v:56 always
            new TravelEntry(25, 43, 0), // →25 v:43 always
            new TravelEntry(20, 39, 0), // →20 v:39 always
            new TravelEntry(92, 44, 0), // →92 v:44 always
            new TravelEntry(92, 27, 0), // →92 v:27 always
        }},
        {89, new List<TravelEntry>
        {
            new TravelEntry(25, 1, 0), // →25 v:1 always
        }},
        {90, new List<TravelEntry>
        {
            new TravelEntry(23, 1, 0), // →23 v:1 always
        }},
        {91, new List<TravelEntry>
        {
            new TravelEntry(95, 45, 0), // →95 v:45 always
            new TravelEntry(95, 73, 0), // →95 v:73 always
            new TravelEntry(95, 23, 0), // →95 v:23 always
            new TravelEntry(72, 30, 0), // →72 v:30 always
            new TravelEntry(72, 56, 0), // →72 v:56 always
        }},
        {92, new List<TravelEntry>
        {
            new TravelEntry(88, 46, 0), // →88 v:46 always
            new TravelEntry(93, 43, 0), // →93 v:43 always
            new TravelEntry(94, 45, 0), // →94 v:45 always
        }},
        {93, new List<TravelEntry>
        {
            new TravelEntry(92, 46, 0), // →92 v:46 always
            new TravelEntry(92, 27, 0), // →92 v:27 always
            new TravelEntry(92, 11, 0), // →92 v:11 always
        }},
        {94, new List<TravelEntry>
        {
            new TravelEntry(92, 46, 0), // →92 v:46 always
            new TravelEntry(92, 27, 0), // →92 v:27 always
            new TravelEntry(92, 23, 0), // →92 v:23 always
            new TravelEntry(95, 45, 309), // →95 v:45 cond:309
            new TravelEntry(95, 3, 309), // →95 v:3 cond:309
            new TravelEntry(95, 73, 309), // →95 v:73 cond:309
            new TravelEntry(611, 45, 0), // →611 v:45 always
        }},
        {95, new List<TravelEntry>
        {
            new TravelEntry(94, 46, 0), // →94 v:46 always
            new TravelEntry(94, 11, 0), // →94 v:11 always
            new TravelEntry(92, 27, 0), // →92 v:27 always
            new TravelEntry(91, 44, 0), // →91 v:44 always
        }},
        {96, new List<TravelEntry>
        {
            new TravelEntry(66, 44, 0), // →66 v:44 always
            new TravelEntry(66, 11, 0), // →66 v:11 always
        }},
        {97, new List<TravelEntry>
        {
            new TravelEntry(66, 48, 0), // →66 v:48 always
            new TravelEntry(72, 44, 0), // →72 v:44 always
            new TravelEntry(72, 17, 0), // →72 v:17 always
            new TravelEntry(98, 29, 0), // →98 v:29 always
            new TravelEntry(98, 45, 0), // →98 v:45 always
            new TravelEntry(98, 73, 0), // →98 v:73 always
        }},
        {98, new List<TravelEntry>
        {
            new TravelEntry(97, 46, 0), // →97 v:46 always
            new TravelEntry(97, 72, 0), // →97 v:72 always
            new TravelEntry(99, 44, 0), // →99 v:44 always
        }},
        {99, new List<TravelEntry>
        {
            new TravelEntry(98, 50, 0), // →98 v:50 always
            new TravelEntry(98, 73, 0), // →98 v:73 always
            new TravelEntry(301, 43, 0), // →301 v:43 always
            new TravelEntry(301, 23, 0), // →301 v:23 always
            new TravelEntry(100, 43, 0), // →100 v:43 always
        }},
        {100, new List<TravelEntry>
        {
            new TravelEntry(301, 44, 0), // →301 v:44 always
            new TravelEntry(301, 23, 0), // →301 v:23 always
            new TravelEntry(301, 11, 0), // →301 v:11 always
            new TravelEntry(99, 44, 0), // →99 v:44 always
            new TravelEntry(302, 71, 159), // →302 v:71 cond:159
            new TravelEntry(33, 71, 0), // →33 v:71 always
            new TravelEntry(101, 47, 0), // →101 v:47 always
            new TravelEntry(101, 22, 0), // →101 v:22 always
        }},
        {101, new List<TravelEntry>
        {
            new TravelEntry(100, 46, 0), // →100 v:46 always
            new TravelEntry(100, 71, 0), // →100 v:71 always
            new TravelEntry(100, 11, 0), // →100 v:11 always
        }},
        {102, new List<TravelEntry>
        {
            new TravelEntry(103, 30, 0), // →103 v:30 always
            new TravelEntry(103, 74, 0), // →103 v:74 always
            new TravelEntry(103, 11, 0), // →103 v:11 always
        }},
        {103, new List<TravelEntry>
        {
            new TravelEntry(102, 29, 0), // →102 v:29 always
            new TravelEntry(102, 38, 0), // →102 v:38 always
            new TravelEntry(104, 30, 0), // →104 v:30 always
            new TravelEntry(618, 46, 114), // →618 v:46 cond:114
            new TravelEntry(619, 46, 115), // →619 v:46 cond:115
            new TravelEntry(64, 46, 0), // →64 v:46 always
        }},
        {104, new List<TravelEntry>
        {
            new TravelEntry(103, 29, 0), // →103 v:29 always
            new TravelEntry(103, 74, 0), // →103 v:74 always
            new TravelEntry(105, 30, 0), // →105 v:30 always
        }},
        {105, new List<TravelEntry>
        {
            new TravelEntry(104, 29, 0), // →104 v:29 always
            new TravelEntry(104, 11, 0), // →104 v:11 always
            new TravelEntry(103, 74, 0), // →103 v:74 always
        }},
        {106, new List<TravelEntry>
        {
            new TravelEntry(64, 29, 0), // →64 v:29 always
            new TravelEntry(65, 44, 0), // →65 v:44 always
            new TravelEntry(108, 43, 0), // →108 v:43 always
        }},
        {107, new List<TravelEntry>
        {
            new TravelEntry(131, 46, 0), // →131 v:46 always
            new TravelEntry(132, 49, 0), // →132 v:49 always
            new TravelEntry(133, 47, 0), // →133 v:47 always
            new TravelEntry(134, 48, 0), // →134 v:48 always
            new TravelEntry(135, 29, 0), // →135 v:29 always
            new TravelEntry(136, 50, 0), // →136 v:50 always
            new TravelEntry(137, 43, 0), // →137 v:43 always
            new TravelEntry(138, 44, 0), // →138 v:44 always
            new TravelEntry(139, 45, 0), // →139 v:45 always
            new TravelEntry(61, 30, 0), // →61 v:30 always
        }},
        {108, new List<TravelEntry>
        {
            new TravelEntry(588, 43, 95), // →588 v:43 95%
            new TravelEntry(588, 45, 95), // →588 v:45 95%
            new TravelEntry(588, 46, 95), // →588 v:46 95%
            new TravelEntry(588, 47, 95), // →588 v:47 95%
            new TravelEntry(588, 48, 95), // →588 v:48 95%
            new TravelEntry(588, 49, 95), // →588 v:49 95%
            new TravelEntry(588, 50, 95), // →588 v:50 95%
            new TravelEntry(588, 29, 95), // →588 v:29 95%
            new TravelEntry(588, 30, 95), // →588 v:30 95%
            new TravelEntry(106, 43, 0), // →106 v:43 always
            new TravelEntry(626, 44, 0), // →626 v:44 always
        }},
        {109, new List<TravelEntry>
        {
            new TravelEntry(69, 46, 0), // →69 v:46 always
            new TravelEntry(113, 45, 0), // →113 v:45 always
            new TravelEntry(113, 75, 0), // →113 v:75 always
        }},
        {110, new List<TravelEntry>
        {
            new TravelEntry(71, 44, 0), // →71 v:44 always
            new TravelEntry(20, 39, 0), // →20 v:39 always
        }},
        {111, new List<TravelEntry>
        {
            new TravelEntry(70, 45, 0), // →70 v:45 always
            new TravelEntry(50, 30, 40), // →50 v:30 40%
            new TravelEntry(50, 39, 40), // →50 v:39 40%
            new TravelEntry(50, 56, 40), // →50 v:56 40%
            new TravelEntry(53, 30, 50), // →53 v:30 50%
            new TravelEntry(45, 30, 0), // →45 v:30 always
        }},
        {112, new List<TravelEntry>
        {
            new TravelEntry(131, 49, 0), // →131 v:49 always
            new TravelEntry(132, 45, 0), // →132 v:45 always
            new TravelEntry(133, 43, 0), // →133 v:43 always
            new TravelEntry(134, 50, 0), // →134 v:50 always
            new TravelEntry(135, 48, 0), // →135 v:48 always
            new TravelEntry(136, 47, 0), // →136 v:47 always
            new TravelEntry(137, 44, 0), // →137 v:44 always
            new TravelEntry(138, 30, 0), // →138 v:30 always
            new TravelEntry(139, 29, 0), // →139 v:29 always
            new TravelEntry(140, 46, 0), // →140 v:46 always
        }},
        {113, new List<TravelEntry>
        {
            new TravelEntry(109, 46, 0), // →109 v:46 always
            new TravelEntry(109, 11, 0), // →109 v:11 always
            new TravelEntry(109, 109, 0), // →109 v:109 always
        }},
        {114, new List<TravelEntry>
        {
            new TravelEntry(84, 48, 0), // →84 v:48 always
        }},
        {115, new List<TravelEntry>
        {
            new TravelEntry(116, 49, 0), // →116 v:49 always
        }},
        {116, new List<TravelEntry>
        {
            new TravelEntry(115, 47, 0), // →115 v:47 always
            new TravelEntry(593, 30, 0), // →593 v:30 always
        }},
        {117, new List<TravelEntry>
        {
            new TravelEntry(118, 49, 0), // →118 v:49 always
            new TravelEntry(660, 41, 233), // →660 v:41 cond:233
            new TravelEntry(660, 42, 233), // →660 v:42 cond:233
            new TravelEntry(660, 69, 233), // →660 v:69 cond:233
            new TravelEntry(660, 47, 233), // →660 v:47 cond:233
            new TravelEntry(661, 41, 332), // →661 v:41 cond:332
            new TravelEntry(303, 41, 0), // →303 v:41 always
            new TravelEntry(21, 39, 332), // →21 v:39 cond:332
            new TravelEntry(596, 39, 0), // →596 v:39 always
        }},
        {118, new List<TravelEntry>
        {
            new TravelEntry(72, 30, 0), // →72 v:30 always
            new TravelEntry(117, 29, 0), // →117 v:29 always
        }},
        {119, new List<TravelEntry>
        {
            new TravelEntry(69, 45, 0), // →69 v:45 always
            new TravelEntry(69, 11, 0), // →69 v:11 always
            new TravelEntry(653, 43, 0), // →653 v:43 always
            new TravelEntry(65, 307, 0), // →65 v:307 always
        }},
        {120, new List<TravelEntry>
        {
            new TravelEntry(69, 45, 0), // →69 v:45 always
            new TravelEntry(74, 43, 0), // →74 v:43 always
        }},
        {121, new List<TravelEntry>
        {
            new TravelEntry(74, 43, 0), // →74 v:43 always
            new TravelEntry(74, 11, 0), // →74 v:11 always
            new TravelEntry(653, 45, 0), // →653 v:45 always
            new TravelEntry(653, 7, 0), // →653 v:7 always
        }},
        {122, new List<TravelEntry>
        {
            new TravelEntry(123, 47, 0), // →123 v:47 always
            new TravelEntry(660, 41, 233), // →660 v:41 cond:233
            new TravelEntry(660, 42, 233), // →660 v:42 cond:233
            new TravelEntry(660, 69, 233), // →660 v:69 cond:233
            new TravelEntry(660, 49, 233), // →660 v:49 cond:233
            new TravelEntry(303, 41, 0), // →303 v:41 always
            new TravelEntry(596, 39, 0), // →596 v:39 always
            new TravelEntry(124, 77, 0), // →124 v:77 always
            new TravelEntry(126, 28, 0), // →126 v:28 always
            new TravelEntry(129, 40, 0), // →129 v:40 always
        }},
        {123, new List<TravelEntry>
        {
            new TravelEntry(122, 44, 0), // →122 v:44 always
            new TravelEntry(124, 43, 0), // →124 v:43 always
            new TravelEntry(124, 77, 0), // →124 v:77 always
            new TravelEntry(126, 28, 0), // →126 v:28 always
            new TravelEntry(129, 40, 0), // →129 v:40 always
        }},
        {124, new List<TravelEntry>
        {
            new TravelEntry(123, 44, 0), // →123 v:44 always
            new TravelEntry(125, 47, 0), // →125 v:47 always
            new TravelEntry(125, 36, 0), // →125 v:36 always
            new TravelEntry(128, 48, 0), // →128 v:48 always
            new TravelEntry(128, 37, 0), // →128 v:37 always
            new TravelEntry(128, 30, 0), // →128 v:30 always
            new TravelEntry(126, 28, 0), // →126 v:28 always
            new TravelEntry(129, 40, 0), // →129 v:40 always
        }},
        {125, new List<TravelEntry>
        {
            new TravelEntry(124, 46, 0), // →124 v:46 always
            new TravelEntry(124, 77, 0), // →124 v:77 always
            new TravelEntry(126, 45, 0), // →126 v:45 always
            new TravelEntry(126, 28, 0), // →126 v:28 always
            new TravelEntry(127, 43, 0), // →127 v:43 always
            new TravelEntry(127, 17, 0), // →127 v:17 always
        }},
        {126, new List<TravelEntry>
        {
            new TravelEntry(125, 46, 0), // →125 v:46 always
            new TravelEntry(125, 23, 0), // →125 v:23 always
            new TravelEntry(125, 11, 0), // →125 v:11 always
            new TravelEntry(124, 77, 0), // →124 v:77 always
            new TravelEntry(610, 30, 0), // →610 v:30 always
            new TravelEntry(610, 39, 0), // →610 v:39 always
        }},
        {127, new List<TravelEntry>
        {
            new TravelEntry(125, 44, 0), // →125 v:44 always
            new TravelEntry(125, 11, 0), // →125 v:11 always
            new TravelEntry(125, 17, 0), // →125 v:17 always
            new TravelEntry(124, 77, 0), // →124 v:77 always
            new TravelEntry(126, 28, 0), // →126 v:28 always
        }},
        {128, new List<TravelEntry>
        {
            new TravelEntry(124, 45, 0), // →124 v:45 always
            new TravelEntry(124, 29, 0), // →124 v:29 always
            new TravelEntry(124, 77, 0), // →124 v:77 always
            new TravelEntry(129, 46, 0), // →129 v:46 always
            new TravelEntry(129, 30, 0), // →129 v:30 always
            new TravelEntry(129, 40, 0), // →129 v:40 always
            new TravelEntry(126, 28, 0), // →126 v:28 always
        }},
        {129, new List<TravelEntry>
        {
            new TravelEntry(128, 44, 0), // →128 v:44 always
            new TravelEntry(128, 29, 0), // →128 v:29 always
            new TravelEntry(124, 77, 0), // →124 v:77 always
            new TravelEntry(130, 43, 0), // →130 v:43 always
            new TravelEntry(130, 19, 0), // →130 v:19 always
            new TravelEntry(130, 40, 0), // →130 v:40 always
            new TravelEntry(130, 3, 0), // →130 v:3 always
            new TravelEntry(126, 28, 0), // →126 v:28 always
        }},
        {130, new List<TravelEntry>
        {
            new TravelEntry(129, 44, 0), // →129 v:44 always
            new TravelEntry(124, 77, 0), // →124 v:77 always
            new TravelEntry(126, 28, 0), // →126 v:28 always
        }},
        {131, new List<TravelEntry>
        {
            new TravelEntry(107, 44, 0), // →107 v:44 always
            new TravelEntry(132, 48, 0), // →132 v:48 always
            new TravelEntry(133, 50, 0), // →133 v:50 always
            new TravelEntry(134, 49, 0), // →134 v:49 always
            new TravelEntry(135, 47, 0), // →135 v:47 always
            new TravelEntry(136, 29, 0), // →136 v:29 always
            new TravelEntry(137, 30, 0), // →137 v:30 always
            new TravelEntry(138, 45, 0), // →138 v:45 always
            new TravelEntry(139, 46, 0), // →139 v:46 always
            new TravelEntry(112, 43, 0), // →112 v:43 always
        }},
        {132, new List<TravelEntry>
        {
            new TravelEntry(107, 50, 0), // →107 v:50 always
            new TravelEntry(131, 29, 0), // →131 v:29 always
            new TravelEntry(133, 45, 0), // →133 v:45 always
            new TravelEntry(134, 46, 0), // →134 v:46 always
            new TravelEntry(135, 44, 0), // →135 v:44 always
            new TravelEntry(136, 49, 0), // →136 v:49 always
            new TravelEntry(137, 47, 0), // →137 v:47 always
            new TravelEntry(138, 43, 0), // →138 v:43 always
            new TravelEntry(139, 30, 0), // →139 v:30 always
            new TravelEntry(112, 48, 0), // →112 v:48 always
        }},
        {133, new List<TravelEntry>
        {
            new TravelEntry(107, 29, 0), // →107 v:29 always
            new TravelEntry(131, 30, 0), // →131 v:30 always
            new TravelEntry(132, 44, 0), // →132 v:44 always
            new TravelEntry(134, 47, 0), // →134 v:47 always
            new TravelEntry(135, 49, 0), // →135 v:49 always
            new TravelEntry(136, 43, 0), // →136 v:43 always
            new TravelEntry(137, 45, 0), // →137 v:45 always
            new TravelEntry(138, 50, 0), // →138 v:50 always
            new TravelEntry(139, 48, 0), // →139 v:48 always
            new TravelEntry(112, 46, 0), // →112 v:46 always
        }},
        {134, new List<TravelEntry>
        {
            new TravelEntry(107, 47, 0), // →107 v:47 always
            new TravelEntry(131, 45, 0), // →131 v:45 always
            new TravelEntry(132, 50, 0), // →132 v:50 always
            new TravelEntry(133, 48, 0), // →133 v:48 always
            new TravelEntry(135, 43, 0), // →135 v:43 always
            new TravelEntry(136, 30, 0), // →136 v:30 always
            new TravelEntry(137, 46, 0), // →137 v:46 always
            new TravelEntry(138, 29, 0), // →138 v:29 always
            new TravelEntry(139, 44, 0), // →139 v:44 always
            new TravelEntry(112, 49, 0), // →112 v:49 always
        }},
        {135, new List<TravelEntry>
        {
            new TravelEntry(107, 45, 0), // →107 v:45 always
            new TravelEntry(131, 48, 0), // →131 v:48 always
            new TravelEntry(132, 30, 0), // →132 v:30 always
            new TravelEntry(133, 46, 0), // →133 v:46 always
            new TravelEntry(134, 43, 0), // →134 v:43 always
            new TravelEntry(136, 44, 0), // →136 v:44 always
            new TravelEntry(137, 49, 0), // →137 v:49 always
            new TravelEntry(138, 47, 0), // →138 v:47 always
            new TravelEntry(139, 50, 0), // →139 v:50 always
            new TravelEntry(112, 29, 0), // →112 v:29 always
        }},
        {136, new List<TravelEntry>
        {
            new TravelEntry(107, 43, 0), // →107 v:43 always
            new TravelEntry(131, 44, 0), // →131 v:44 always
            new TravelEntry(132, 29, 0), // →132 v:29 always
            new TravelEntry(133, 49, 0), // →133 v:49 always
            new TravelEntry(134, 30, 0), // →134 v:30 always
            new TravelEntry(135, 46, 0), // →135 v:46 always
            new TravelEntry(137, 50, 0), // →137 v:50 always
            new TravelEntry(138, 48, 0), // →138 v:48 always
            new TravelEntry(139, 47, 0), // →139 v:47 always
            new TravelEntry(112, 45, 0), // →112 v:45 always
        }},
        {137, new List<TravelEntry>
        {
            new TravelEntry(107, 48, 0), // →107 v:48 always
            new TravelEntry(131, 47, 0), // →131 v:47 always
            new TravelEntry(132, 46, 0), // →132 v:46 always
            new TravelEntry(133, 30, 0), // →133 v:30 always
            new TravelEntry(134, 29, 0), // →134 v:29 always
            new TravelEntry(135, 50, 0), // →135 v:50 always
            new TravelEntry(136, 45, 0), // →136 v:45 always
            new TravelEntry(138, 49, 0), // →138 v:49 always
            new TravelEntry(139, 43, 0), // →139 v:43 always
            new TravelEntry(112, 44, 0), // →112 v:44 always
        }},
        {138, new List<TravelEntry>
        {
            new TravelEntry(107, 30, 0), // →107 v:30 always
            new TravelEntry(131, 43, 0), // →131 v:43 always
            new TravelEntry(132, 47, 0), // →132 v:47 always
            new TravelEntry(133, 29, 0), // →133 v:29 always
            new TravelEntry(134, 44, 0), // →134 v:44 always
            new TravelEntry(135, 45, 0), // →135 v:45 always
            new TravelEntry(136, 46, 0), // →136 v:46 always
            new TravelEntry(137, 48, 0), // →137 v:48 always
            new TravelEntry(139, 49, 0), // →139 v:49 always
            new TravelEntry(112, 50, 0), // →112 v:50 always
        }},
        {139, new List<TravelEntry>
        {
            new TravelEntry(107, 49, 0), // →107 v:49 always
            new TravelEntry(131, 50, 0), // →131 v:50 always
            new TravelEntry(132, 43, 0), // →132 v:43 always
            new TravelEntry(133, 44, 0), // →133 v:44 always
            new TravelEntry(134, 45, 0), // →134 v:45 always
            new TravelEntry(135, 30, 0), // →135 v:30 always
            new TravelEntry(136, 48, 0), // →136 v:48 always
            new TravelEntry(137, 29, 0), // →137 v:29 always
            new TravelEntry(138, 46, 0), // →138 v:46 always
            new TravelEntry(112, 47, 0), // →112 v:47 always
        }},
        {140, new List<TravelEntry>
        {
            new TravelEntry(112, 45, 0), // →112 v:45 always
            new TravelEntry(112, 11, 0), // →112 v:11 always
        }},
    };

    /// <summary>
    /// Gets the travel options for a given location.
    /// </summary>
    public static List<TravelEntry> GetTravelOptions(int locationId)
    {
        if (TravelTable.TryGetValue(locationId, out List<TravelEntry>? options))
            return options;
        return [];
    }
}
