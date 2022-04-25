/**
 * init_eAI.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 *	This file is not the final version and is not guarranted to function correctly in later updates
 *	You are free to edit, share and use this file as much as you want
 *	Special thanks to @truedolphin#1001 for the idea and early prototype of this file.
 *
*/

private const static string EXP_AI_FOLDER = EXPANSION_FOLDER + "AI\\"; // root directory for SpawnLoadout
private const static string EXP_AI_SETTINGS = EXP_AI_FOLDER + "PatrolSettings.json"; // json for waypoints and loadouts

class ExpAIGroup
{
    string Faction;             // WEST, EAST, INSURGENT, CIVILIAN
    string LoadoutFile;         // a json file containing the loadout of this team
    int NumberOfAI;             // How many bots, -x will make it random between 0 and x
    string Behaviour;           // HALT, LOOP, REVERSE
    string Speed;               // WALK, JOG, SPRINT 
    vector StartPos;            // where the group is going to spawn
    ref TVectorArray Waypoints; // a list of positions to create a path to follow

    void ExpAIGroup(int bod, string spd, string beh, string fac, string loa, vector startpos, TVectorArray way)
    {
        NumberOfAI = bod;
        Speed = spd;
        Behaviour = beh;
        Faction = fac;
        LoadoutFile = loa;
        StartPos = startpos;
        Waypoints = way;
    }
}

class ExpAIGroups
{
	float RespawnTime = 600;				// Time in seconds before the dead group will respawn. If set to -1, they won't respawn
	float MinDistRadius = 300;			// If the player is closer than MinDistRadius from the spawn point, the group won't spawn
	float MaxDistRadius = 1200;			// Same but if the player is further away than MaxDistRadius, the bots won't spawn
	ref array< ref ExpAIGroup > Group;
	void ExpAIGroups()
	{
		Group = new array< ref ExpAIGroup >;
	}
}

void InitDynamicPatrols()
{
	Print("=================== Expansion AI ===================");
    ExpAIGroups m_ExpAIGroups;
    
    if ( !FileExist(EXP_AI_SETTINGS) )
    {
		if ( !FileExist(EXP_AI_FOLDER) )
			MakeDirectory(EXP_AI_FOLDER);

        DefaultDynamicPatrols(m_ExpAIGroups);
		JsonFileLoader<ExpAIGroups>.JsonSaveFile(EXP_AI_SETTINGS, m_ExpAIGroups);
		Print("WARNING: Couldn't find config file !");
		Print("Generating default config (will be located in: "+EXP_AI_SETTINGS+")");
    } else {
		m_ExpAIGroups = new ExpAIGroups();
		JsonFileLoader<ExpAIGroups>.JsonLoadFile(EXP_AI_SETTINGS, m_ExpAIGroups);
		Print("Loading config ("+EXP_AI_SETTINGS+")");
	}

    foreach(ExpAIGroup group: m_ExpAIGroups.Group)
    {
		int aiSum;
		if ( group.NumberOfAI != 0 )
		{
			if ( group.NumberOfAI < 0 )
			{
				aiSum = Math.RandomInt(1,-group.NumberOfAI);
			} else {
				aiSum = group.NumberOfAI;
			}
		} else {
			aiSum = 1;
		}

		vector startpos = group.StartPos;
		if ( !startpos || startpos == "0 0 0" )
		{
			startpos = group.Waypoints[0];
		}

		// Safety in case the Y is bellow the ground
		startpos = ExpansionStatic.GetSurfacePosition(startpos);
		if ( startpos[1] < group.StartPos[1] )
			startpos[1] = group.StartPos[1];

		Print("Spawning "+aiSum+" "+group.Faction+" bots at "+group.StartPos+" and will patrol at "+group.Waypoints);
 		eAIDynamicPatrol.Create(startpos, group.Waypoints, Expansion_GetAIBehaviour(group.Behaviour), group.LoadoutFile, aiSum, m_ExpAIGroups.RespawnTime, Expansion_GetAIFaction(group.Faction), true, m_ExpAIGroups.MinDistRadius, m_ExpAIGroups.MaxDistRadius, Expansion_GetAISpeed(group.Speed));
	}
	Print("=================== Expansion AI ===================");
}

float Expansion_GetAISpeed(string speed)
{
    switch (speed)
    {
        case "WALK":
        {
            return 1.0;
            break;
        }
        case "JOG":
        {
            return 2.0;
            break;
        }
        case "SPRINT":
        {
            return 3.0;
            break;
        }
    }

    //! Unknown speed, sending default speed
    return 2.0;
}

int Expansion_GetAIBehaviour(string beh)
{
    switch (beh)
    {
        case "REVERSE":
        {
            return eAIWaypointBehavior.REVERSE;
            break;
        }
        case "HALT":
        {
            return eAIWaypointBehavior.HALT;
            break;
        }
        case "LOOP":
        {
            return eAIWaypointBehavior.LOOP;
            break;
        }
    }

    //! Unknown Behaviour, sending default behaviour
    return eAIWaypointBehavior.REVERSE;
}

eAIFaction Expansion_GetAIFaction(string faction)
{
    switch (faction)
    {
        case "WEST":
        {
            return new eAIFactionWest();
            break;
        }
        case "EAST":
        {
            return new eAIFactionEast();
            break;
        }
        case "INSURGENT":
        {
            return new eAIFactionRaiders();
            break;
        }
        case "CIVILIAN":
        {
            return new eAIFactionCivilian();
            break;
        }
    }

    //! Unknown Faction, sending default faction
    return new eAIFactionCivilian();
}

void DefaultDynamicPatrols(out ExpAIGroups Data)
{
		string worldName;
		GetGame().GetWorldName(worldName);
		worldName.ToLower();
		if ( worldName.IndexOf("gloom") == worldName.Length() - 5 )
		{
			worldName = worldName.Substring(0, worldName.Length() - 5);
		}
    Data = new ExpAIGroups();
    switch (worldName)
    {
    case "banov":
        Print("World = "+worldName+" Found.");
        Print("--Generating Template--");
        Data.Group.Insert( new ExpAIGroup(3, "WALK",      "LOOP",     "WEST",     "SoldierCDFLoadout.json",  "976.139 263.979 12683.9",               {"976.139 263.979 12683.9", "1094.76 269.075 12877.5", "976.139 263.979 12683.9", "1094.76 269.075 12877.5","976.139 263.979 12683.9", "1094.76 269.075 12877.5","976.139 263.979 12683.9", "1094.76 269.075 12877.5","976.139 263.979 12683.9", "1094.76 269.075 12877.5"} ));
        Data.Group.Insert( new ExpAIGroup(4, "WALK",      "LOOP",     "WEST",     "SoldierCDFLoadout.json",  "671.794 245.434 11457.6",               {"671.794 245.434 11457.6", "831.525 245.301 11211.7", "671.794 245.434 11457.6", "831.525 245.301 11211.7", "671.794 245.434 11457.6", "831.525 245.301 11211.7", "671.794 245.434 11457.6", "831.525 245.301 11211.7", "671.794 245.434 11457.6", "831.525 245.301 11211.7"} ));
        Data.Group.Insert( new ExpAIGroup(3, "WALK",      "LOOP",     "WEST",     "SoldierCDFLoadout.json",  "1743.85 268.621 11658.2",               {"1743.85 268.621 11658.2", "1829.22 261.877 12054.7", "1743.85 268.621 11658.2", "1829.22 261.877 12054.7", "1743.85 268.621 11658.2", "1829.22 261.877 12054.7", "1743.85 268.621 11658.2", "1829.22 261.877 12054.7", "1743.85 268.621 11658.2", "1829.22 261.877 12054.7"} ));
        Data.Group.Insert( new ExpAIGroup(3, "WALK",      "LOOP",     "WEST",     "SoldierCDFLoadout.json",  "1520.8 229.459 9879.21",               {"1520.8 229.459 9879.21", "1742.91 229.232 9592.4", "1520.8 229.459 9879.21", "1742.91 229.232 9592.4", "1520.8 229.459 9879.21", "1742.91 229.232 9592.4", "1520.8 229.459 9879.21", "1742.91 229.232 9592.4", "1520.8 229.459 9879.21", "1742.91 229.232 9592.4"} ));
        Data.Group.Insert( new ExpAIGroup(3, "WALK",      "LOOP",     "WEST",     "SoldierCDFLoadout.json",  "1880.51 224.479 8392.91",               {"1880.51 224.479 8392.91", "1921.05 221.434 8027.86", "1880.51 224.479 8392.91", "1921.05 221.434 8027.86", "1880.51 224.479 8392.91", "1921.05 221.434 8027.86", "1880.51 224.479 8392.91", "1921.05 221.434 8027.86", "1880.51 224.479 8392.91", "1921.05 221.434 8027.86"} ));
        Data.Group.Insert( new ExpAIGroup(3, "WALK",      "LOOP",     "WEST",     "SoldierCDFLoadout.json",  "1884.05 227.035 6599.31",               {"1884.05 227.035 6599.31", "2117.14 222.043 6418.1", "1884.05 227.035 6599.31", "2117.14 222.043 6418.1", "1884.05 227.035 6599.31", "2117.14 222.043 6418.1", "1884.05 227.035 6599.31", "2117.14 222.043 6418.1", "1884.05 227.035 6599.31", "2117.14 222.043 6418.1"} ));
        Data.Group.Insert( new ExpAIGroup(5, "WALK",      "LOOP",     "WEST",     "SoldierCDFLoadout.json",  "1226.16 234.056 4521.42",               {"1226.16 234.056 4521.42", "978.184 229.308 4361.1", "1226.16 234.056 4521.42", "978.184 229.308 4361.1", "1226.16 234.056 4521.42", "978.184 229.308 4361.1", "1226.16 234.056 4521.42", "978.184 229.308 4361.1", "1226.16 234.056 4521.42", "978.184 229.308 4361.1"} ));
        Data.Group.Insert( new ExpAIGroup(5, "WALK",      "LOOP",     "WEST",     "SoldierCDFLoadout.json",  "640.75 250.928 3287.15",               {"640.75 250.928 3287.15", "479.013 252.843 3354.61", "640.75 250.928 3287.15", "479.013 252.843 3354.61", "640.75 250.928 3287.15", "479.013 252.843 3354.61", "640.75 250.928 3287.15", "479.013 252.843 3354.61", "640.75 250.928 3287.15", "479.013 252.843 3354.61"} ));
        Data.Group.Insert( new ExpAIGroup(3, "WALK",      "LOOP",     "WEST",     "SoldierCDFLoadout.json",  "2576.35 237.181 1903.83",               {"2576.35 237.181 1903.83", "2440.35 233.075 2074.31", "2576.35 237.181 1903.83", "2440.35 233.075 2074.31", "2576.35 237.181 1903.83", "2440.35 233.075 2074.31", "2576.35 237.181 1903.83", "2440.35 233.075 2074.31", "2576.35 237.181 1903.83", "2440.35 233.075 2074.31"} ));
        Data.Group.Insert( new ExpAIGroup(4, "WALK",      "LOOP",     "WEST",     "SoldierCDFLoadout.json",  "3623.74 214.088 3556.15",               {"3623.74 214.088 3556.15", "3760.02 214.088 3307.33", "3623.74 214.088 3556.15", "3760.02 214.088 3307.33", "3623.74 214.088 3556.15", "3760.02 214.088 3307.33", "3623.74 214.088 3556.15", "3760.02 214.088 3307.33", "3623.74 214.088 3556.15", "3760.02 214.088 3307.33"} ));
        Data.Group.Insert( new ExpAIGroup(4, "WALK",      "LOOP",     "WEST",     "SoldierCDFLoadout.json",  "5200.22 201.686 4526.44",               {"5200.22 201.686 4526.44", "5063.23 205.131 5062.29", "5200.22 201.686 4526.44", "5063.23 205.131 5062.29", "5200.22 201.686 4526.44", "5063.23 205.131 5062.29", "5200.22 201.686 4526.44", "5063.23 205.131 5062.29", "5200.22 201.686 4526.44", "5063.23 205.131 5062.29"} ));
        Data.Group.Insert( new ExpAIGroup(4, "WALK",      "LOOP",     "WEST",     "SoldierCDFLoadout.json",  "4554.5 211.798 7601.49",               {"4554.5 211.798 7601.49", "4658.85 217.283 7921.67", "4554.5 211.798 7601.49", "4658.85 217.283 7921.67", "4554.5 211.798 7601.49", "4658.85 217.283 7921.67", "4554.5 211.798 7601.49", "4658.85 217.283 7921.67", "4554.5 211.798 7601.49", "4658.85 217.283 7921.67"} ));
       
        break;
    case "chernarusplus":
        Print("World = "+worldName+" Found.");
        Print("--Generating Template--");
        Data.Group.Insert( new ExpAIGroup(4, "WALK",      "REVERSE",     "WEST",     "SoldierCDFLoadout.json",  "4211.222656 109.023384 6382.064453",               {"4211.222656 109.023384 6382.064453", "4151.662598 105.450653 6080.294434", "4160.971680 105.412598 6035.103027", "4160.699219 106.251480 5906.830078", "4107.862793 108.930527 5898.482910", "4057.258057 114.174736 5584.595703", "4084.560059 113.232422 5494.540039", "4079.308105 113.801163 5435.953125", "4081.735840 113.402504 5385.576172", "4067.266113 109.788383 4904.508789", "4126.504883 107.371178 4647.128906"} ));
        Data.Group.Insert( new ExpAIGroup(3, "WALK",      "REVERSE",     "WEST",     "SoldierCDFLoadout.json",  "4508.860352 109.347275 6230.751465",               {"4508.860352 109.347275 6230.751465", "4448.490723 109.377792 6025.892578", "4546.902832 112.057549 6126.590332", "4613.975586 110.163887 6112.869629", "4563.825684 110.696106 5797.450195", "4551.805664 110.555084 5652.876953", "4312.203613 110.752151 5366.941895"} ));
        Data.Group.Insert( new ExpAIGroup(3, "WALK",      "REVERSE",     "WEST",     "SoldierCDFLoadout.json",  "10545.687500 38.037155 10384.205078",               {"10545.687500 38.037155 10384.205078", "10502.615234 40.377762 10464.754883", "10700.634766 34.346542 10461.470703", "10645.350586 36.451836 10377.769531"} ));
        Data.Group.Insert( new ExpAIGroup(5, "WALK",      "REVERSE",     "WEST",     "SoldierCDFLoadout.json", "8588.209961 103.304726 13439.002930",               {"8588.209961 103.304726 13439.002930", "8578.585938 106.485222 13465.281250", "8605.717773 112.455276 13521.625977", "8640.509766 120.118225 13590.360352", "8641.976563 127.327774 13644.069336", "8643.523438 123.581627 13609.848633", "8724.077148 121.008499 13532.352539", "8792.484375 119.344810 13479.867188", "8843.358398 122.085854 13469.464844", "8881.161133 121.293594 13413.066406", "8837.004883 121.072372 13470.006836", "8742.039063 121.246811 13516.587891", "8704.238281 119.762787 13546.516602", "8609.144531 113.184517 13527.601563", "8573.016602 112.247055 13530.662109", "8563.083984 118.173904 13576.212891", "8500.525391 135.590210 13653.769531", "8456.375000 139.365845 13677.127930", "8546.610352 126.129440 13615.544922", "8566.747070 116.355377 13563.688477", "8573.123047 112.562714 13532.739258", "8563.700195 107.298088 13476.465820", "8586.047852 103.664093 13442.43557"} ));
        Data.Group.Insert( new ExpAIGroup(5, "WALK",      "REVERSE",     "WEST",     "SoldierCDFLoadout.json",  "7841.334961 76.696114 105.506462",               {"7841.334961 76.696114 105.506462", "7874.433594 86.500465 194.182739", "7881.091797 88.818947 247.873428", "7804.406250 88.379280 365.877625", "7792.430176 88.650299 383.001404", "7762.807617 83.737671 334.874634", "7760.653809 84.683838 353.211792", "7768.957031 88.445908 421.012054", "7781.338867 89.113235 430.440430", "7769.263672 90.858253 472.488281", "7776.356934 91.006439 493.633820", "7762.374023 91.014488 534.747681", "7739.280762 91.134277 564.848328", "7639.000000 120.328026 769.771545", "7378.479980 94.512924 764.417480", "7325.039063 93.792953 820.421448", "7323.554199 95.076019 869.669067", "6920.266602 84.642204 926.835693", "6942.748047 84.112991 977.515381", "6901.287598 82.878380 1002.580750", "6848.884766 91.492271 967.189453", "6835.840820 84.595589 942.760620", "6655.309082 92.318848 793.134338", "6639.655273 91.830742 792.534058", "6635.730957 92.249802 780.763000"} ));
        Data.Group.Insert( new ExpAIGroup(2, "WALK",      "REVERSE",     "WEST",     "SoldierCDFLoadout.json",  "9963.346680 55.640099 10900.844727",               {"9963.346680 55.640099 10900.844727", "9965.398438 54.729034 10969.536133", "9924.380859 57.232151 10901.967773"} ));
        Data.Group.Insert( new ExpAIGroup(4, "WALK",      "REVERSE",     "WEST",     "SoldierCDFLoadout.json",  "8796.84 59.8781 2547",               {"8796.84 59.8781 2547", "8745 59.1367 2523.2", "8710.44 57.5696 2499", "8794.23 55.9719 2417", "8756.99 55.8529 2370", "8782.76 56.1916 2345", "8740.17 56.1662 2304", "8751.15 56.1216 2284", "8828.47 56.6495 2234", "8764.02 53.2922 2139", "8656.02 47.4257 2009"} ));
        Data.Group.Insert( new ExpAIGroup(2, "WALK",      "REVERSE",     "WEST",     "SoldierCDFLoadout.json",  "1583.58 292.804 2961",               {"1583.58 292.804 2961", "1600.84 294.292 3006", "1631.44 295.408 3029", "1652.89 297.99 3079.6", "1685.63 297.555 3080"} ));
        Data.Group.Insert( new ExpAIGroup(2, "WALK",      "REVERSE",     "WEST",     "SoldierCDFLoadout.json",  "1824.94 294.066 3075",               {"1824.94 294.066 3075", "1884.95 293.222 3047", "1846.69 289.888 2996", "1812.78 290.333 3001", "1796.65 287.975 2990"} ));
        Data.Group.Insert( new ExpAIGroup(4, "WALK",      "REVERSE",     "WEST",     "SoldierCDFLoadout.json",  "7160.85 84.5561 585.604",               {"7160.85 84.5561 585.604", "6677.51 95.0799 770.113"} ));
        break;
    case "chiemsee":
        Print("World = "+worldName+" Found.");
        Print("--Generating Template--");
        Data.Group.Insert( new ExpAIGroup(5, "WALK",      "LOOP",     "WEST",     "SoldierCDFLoadout.json",  "6137.18 8.1828 14667.3",               {"6137.18 8.1828 14667.3", "6127.65 8.1598 14655.7", "6127.19 6.34518 14647.1"} ));
        Data.Group.Insert( new ExpAIGroup(5, "WALK",     "LOOP",     "EAST",     "SoldierMOTRSLoadout.json",  "6167.18 8.1828 14667.3",               {"6167.18 8.1828 14667.3", "6127.65 8.1598 14655.7", "6127.19 6.34518 14647.1"} ));
        Data.Group.Insert( new ExpAIGroup(10,"JOG",      "REVERSE",  "EAST",     "SoldierMOTRSLoadout.json",   "13342.375977 27.020344 11228.225586",  {"13342.375977 27.020344 11228.225586", "13469.069336 11.282786 11163.261719", "13567.762695 6.128973 11127.067383"} ));
        Data.Group.Insert( new ExpAIGroup(10, "SPRINT",   "LOOP",     "INSURGENT","Raiderloadout.json",    "7332.387695 5.712450 2651.121338",     {"7332.387695 5.712450 2651.121338", "7632.803223 5.507639 3075.661865", "7621.013184 5.892049 3083.053223"} ));
        break;
    case "deerisle":
        Print("World = "+worldName+" Found.");
        Print("--Generating Template--");
        Data.Group.Insert( new ExpAIGroup(4, "WALK",      "REVERSE",     "WEST",     "SoldierCDFLoadout.json",  "2614.28 35.1122 3482.59",               {"2614.28 35.1122 3482.59", "2637.89 34.7324 3502.45", "2649.33 33.0476 3525.77", "2649.33 33.0476 3525.77", "2705.26 22.8186 3574.8", "2707.79 22.4635 3585.09", "2752.8 22.4625 3629.68", "2823.14 22.4625 3699.33", "2918.73 22.7593 3797.69", "2923.53 22.4625 3810.79", "2973.94 22.4625 3883.61", "3003.54 22.4625 3948.92", "3007.75 22.7279 3989.39", "3025.54 22.7257 4031.3", "3053.0 22.4625 4058.19", "3126.36 22.7551 4233.04", "3112.06 22.723 4235.07"} ));
        Data.Group.Insert( new ExpAIGroup(4, "WALK",      "REVERSE",     "WEST",     "SoldierCDFLoadout.json",  "6137.18 8.1828 14667.3",               {"6137.18 8.1828 14667.3", "6127.65 8.1598 14655.7", "6127.19 6.34518 14647.1", "6148.16 6.02 14647.3", "6148.73 6.02 14702.1", "6168.66 6.00306 14728.6", "6169.38 6.014 14824.4", "6165.39 6.014 14827.5", "6053.98 6.031 14827.0", "6037.31 6.019 14826.6", "6037.33 6.02 14830.5", "5950.27 6.02 14830.1", "5937.34 6.0199 14822.4", "5933.13 6.01682 14809.5", "5933.28 6.01995 14585.2", "5961.84 6.01966 14568.8", "6145.52 6.02156 14568.7", "6145.21 6.01999 14584.8", "6148.13 6.01999 14584.8", "6147.86 6.02 14638.8", "6132.03 6.34517 14638.9", "6135.35 8.18288 14612"} ));
        Data.Group.Insert( new ExpAIGroup(3, "WALK",      "REVERSE",     "WEST",     "SoldierCDFLoadout.json",  "13469.2 12.9525 9603.42",               {"13469.2 12.9525 9603.42", "13432.9 12.9525 9608.83", "13438.3 12.9172 9657.9", "13489.3 12.9525 9651.27", "13488.5 12.9525 9637.62", "13542.0 12.9525 9630.26"} ));
        Data.Group.Insert( new ExpAIGroup(5, "WALK",      "REVERSE",     "WEST",     "SoldierCDFLoadout.json",  "13605.3 12.922 9866.76",               {"13605.3 12.922 9866.76", "13493.5 12.922 9873.86", "13489.6 12.922 9884.22", "13498.3 12.922 9931.7", "13482.2 12.8804 9946.66", "13426 12.95 9973.23", "13402.3 12.95 9978.1", "13399.0 12.95 9919.61", "13394.2 12.92 9857.08", "13397.2 12.922 9842.91", "13401.9 13.13 9843.68"} ));
       
        break;
    case "enoch":
        Print("World = "+worldName+" Found.");
        Print("--Generating Template--");
        Data.Group.Insert( new ExpAIGroup(5, "WALK",      "LOOP",     "WEST",     "SoldierCDFLoadout.json",  "6137.18 8.1828 14667.3",               {"6137.18 8.1828 14667.3", "6127.65 8.1598 14655.7", "6127.19 6.34518 14647.1"} ));
        Data.Group.Insert( new ExpAIGroup(5, "WALK",     "LOOP",     "EAST",     "SoldierMOTRSLoadout.json",  "6167.18 8.1828 14667.3",               {"6167.18 8.1828 14667.3", "6127.65 8.1598 14655.7", "6127.19 6.34518 14647.1"} ));
        Data.Group.Insert( new ExpAIGroup(10,"JOG",      "REVERSE",  "EAST",     "SoldierMOTRSLoadout.json",   "13342.375977 27.020344 11228.225586",  {"13342.375977 27.020344 11228.225586", "13469.069336 11.282786 11163.261719", "13567.762695 6.128973 11127.067383"} ));
        Data.Group.Insert( new ExpAIGroup(10, "SPRINT",   "LOOP",     "INSURGENT","Raiderloadout.json",    "7332.387695 5.712450 2651.121338",     {"7332.387695 5.712450 2651.121338", "7632.803223 5.507639 3075.661865", "7621.013184 5.892049 3083.053223"} ));
        break;
    case "esseker":
        Print("World = "+worldName+" Found.");
        Print("--Generating Template--");
        Data.Group.Insert( new ExpAIGroup(5, "WALK",      "LOOP",     "WEST",     "SoldierCDFLoadout.json",  "6137.18 8.1828 14667.3",               {"6137.18 8.1828 14667.3", "6127.65 8.1598 14655.7", "6127.19 6.34518 14647.1"} ));
        Data.Group.Insert( new ExpAIGroup(5, "WALK",     "LOOP",     "EAST",     "SoldierMOTRSLoadout.json",  "6167.18 8.1828 14667.3",               {"6167.18 8.1828 14667.3", "6127.65 8.1598 14655.7", "6127.19 6.34518 14647.1"} ));
        Data.Group.Insert( new ExpAIGroup(10,"JOG",      "REVERSE",  "EAST",     "SoldierMOTRSLoadout.json",   "13342.375977 27.020344 11228.225586",  {"13342.375977 27.020344 11228.225586", "13469.069336 11.282786 11163.261719", "13567.762695 6.128973 11127.067383"} ));
        Data.Group.Insert( new ExpAIGroup(10, "SPRINT",   "LOOP",     "INSURGENT","Raiderloadout.json",    "7332.387695 5.712450 2651.121338",     {"7332.387695 5.712450 2651.121338", "7632.803223 5.507639 3075.661865", "7621.013184 5.892049 3083.053223"} ));
        break;
    case "namalsk":
        Print("World = "+worldName+" Found.");
        Print("--Generating Template--");
        Data.Group.Insert( new ExpAIGroup(4, "WALK",      "REVERSE",     "WEST",     "SoldierCDFLoadout.json",  "6307.7 14.400 11810.6",               {"6307.7 14.4 11810.6", "6519.9 17.3 11901.6", "6967.4 6.5 11884.6", "7216.6 10.147 11843.1", "7405.78 23.18 11655.8"} ));
        Data.Group.Insert( new ExpAIGroup(3, "WALK",      "REVERSE",     "WEST",     "SoldierCDFLoadout.json",  "5028.91 7.420 11359.5",               {"5028.91 7.42 11359.5", "4971.8 9.96 11305.5", "4852.96 13.88 11257.2", "4727.41 14.78 11099.7", "4690.78 21.86 10948.9", "4513.78 22.17 10819", "4333.31 15.53 11054.3", "4269.85 9.43 11046"} ));
        Data.Group.Insert( new ExpAIGroup(4, "WALK",      "REVERSE",     "WEST",     "SoldierCDFLoadout.json",  "4492.01 3.430 10429.7",               {"4492.01 3.43 10429.7", "4533.44 5.01 10371.7", "4624.5 13.35 10400.1", "4865.77 10.34 10446.7", "4964.16 2.78 10533.5", "5129.82 9.57 10510.6", "5315.19 15.77 10591", "5493.19 5.28 10593", "5524.4 14.20 10447.9", "5483.9 21.08 10348.9", "5389.42 4.64 10169.5", "5547.42 5.48 10047", "5637.03 1.09 9960", "5650.19 2.91 9899.06", "5574.11 8.27 9843.13", "5343.65 10.77 9940.64", "5154.59 14.7 9905.64", "4920.51 11.26 9685.87", "4749.81 7.86 9834.98", "4795.58 6.04 10146.9", "4580.36 6.88 10154", "4398.7 5.98 10033.1"} ));
        Data.Group.Insert( new ExpAIGroup(5, "WALK",      "REVERSE",     "WEST",     "SoldierCDFLoadout.json",  "6805.86 15.01 11660.4",               {"6805.86 15.01 11660.4", "6867.16 15.13 11474", "6826.18 15.12 11427.5", "6765.64 15.01 11304.2", "6713.69 15 11202.7", "6605.51 15.2 10994", "6625.24 15.94 10943.1", "6634.26 16.12 10924.8", "6583.29 20.79 10864.8"} ));
        Data.Group.Insert( new ExpAIGroup(3, "WALK",      "REVERSE",     "WEST",     "SoldierCDFLoadout.json",  "7052.16 6.390 10051.4",               {"7052.16 6.39 10051.4", "6997.08 4.0 10063.2", "6955.39 4.45 9904.53", "6881.63 5.3 9743.56", "6760.12 9.25 9758.2", "6686.01 11.36 9858.15", "6658.22 9.26 9958.59", "6762.43 7.05 10087"} ));
        Data.Group.Insert( new ExpAIGroup(5, "WALK",      "REVERSE",     "WEST",     "SoldierCDFLoadout.json",  "4756.11 153.16 7399.41",               {"4756.11 153.16 7399.41", "5243.13 209.35 7437.1", "5272.81 271.206 7190.86", "5270.04 256.98 7017.9", "5456.37 196.30 6618.96", "5677.01 107.66 6280.14"} ));
        Data.Group.Insert( new ExpAIGroup(4, "WALK",      "REVERSE",     "WEST",     "SoldierCDFLoadout.json",  "7674.4 75.960 8643.82",               {"7674.4 75.96 8643.82", "7864.4 41.8 8563.62", "7965.19 30 8887.57", "7696.5 47.78 9120.63", "7315.1 26.6 8972.65"} ));
        Data.Group.Insert( new ExpAIGroup(4, "WALK",      "REVERSE",     "WEST",     "SoldierCDFLoadout.json",  "7977.68 3.930 7430.08",               {"7977.68 3.93 7430.08", "7731.84 54.31 7202.23", "7475.49 72.2477 7486.18", "7667.53 33.30 7918.47", "7888.6 3.20 7997.02"} ));
        Data.Group.Insert( new ExpAIGroup(2, "WALK",      "REVERSE",     "WEST",     "SoldierCDFLoadout.json",  "5301.82 58.87 9602.39",               {"5301.82 58.87 9602.39", "5548.07 43.18 9543.59", "5725.36 19.7 9450.14", "5476.45 29.84 9738.32"} ));
        Data.Group.Insert( new ExpAIGroup(2, "WALK",      "REVERSE",     "WEST",     "SoldierCDFLoadout.json",  "1198.73 2.000 11635.4",               {"1198.73 2 11635.4", "1353.84 2 11604", "1351.76 2 11960.4", "1112.03 2 12009.2", "1112.48 8.92 11898.3"} ));
        Data.Group.Insert( new ExpAIGroup(5, "WALK",      "REVERSE",     "WEST",     "SoldierCDFLoadout.json",  "5407.11 65.41 6072.06",               {"5407.11 65.41 6072.06", "5246 40.67 6230.45", "4805 88.73 6460.97", "4785.95 96.61 6655.96"} ));
        Data.Group.Insert( new ExpAIGroup(2, "WALK",      "REVERSE",     "WEST",     "SoldierCDFLoadout.json",  "2914.75 4.57 11032.5",               {"2914.75 4.57 11032.5", "2442.75 2 11160.6", "1997.79 2 10807", "2762.99 2 10394.4"} ));
        Data.Group.Insert( new ExpAIGroup(2, "WALK",      "REVERSE",     "WEST",     "SoldierCDFLoadout.json",  "5304.6 40.85 10803.6",               {"5304.6 40.85 10803.6", "5612.07 47.78 10952.4", "5713.27 58.28 11090.8", "6148.22 7.7 11587.3"} ));
        Data.Group.Insert( new ExpAIGroup(2, "WALK",      "REVERSE",     "WEST",     "SoldierCDFLoadout.json",  "4717.85 111.52 7672.94",               {"4717.85 111.52 7672.94", "4311.86 129.89 7290.57", "3957.02 106.42 7239.99", "3498.03 139.88 6967.42", "3241.25 104.45 7299.5", "3663.37 90.21 7627.72", "3894.5 78.88 7776.37"} ));
        break;
    case "takistanplus":
        Print("World = "+worldName+" Found.");
        Print("--Generating Template--");
        Data.Group.Insert( new ExpAIGroup(5, "WALK",      "LOOP",     "WEST",     "SoldierCDFLoadout.json",  "6137.18 8.1828 14667.3",               {"6137.18 8.1828 14667.3", "6127.65 8.1598 14655.7", "6127.19 6.34518 14647.1"} ));
        Data.Group.Insert( new ExpAIGroup(5, "WALK",     "LOOP",     "EAST",     "SoldierMOTRSLoadout.json",  "6167.18 8.1828 14667.3",               {"6167.18 8.1828 14667.3", "6127.65 8.1598 14655.7", "6127.19 6.34518 14647.1"} ));
        Data.Group.Insert( new ExpAIGroup(10,"JOG",      "REVERSE",  "EAST",     "SoldierMOTRSLoadout.json",   "13342.375977 27.020344 11228.225586",  {"13342.375977 27.020344 11228.225586", "13469.069336 11.282786 11163.261719", "13567.762695 6.128973 11127.067383"} ));
        Data.Group.Insert( new ExpAIGroup(10, "SPRINT",   "LOOP",     "INSURGENT","Raiderloadout.json",    "7332.387695 5.712450 2651.121338",     {"7332.387695 5.712450 2651.121338", "7632.803223 5.507639 3075.661865", "7621.013184 5.892049 3083.053223"} ));
        break;
    default:
    Print("--Default list for world not found--");
    Print("--Generating Default Template--");
        Data.Group.Insert( new ExpAIGroup(5, "WALK",      "LOOP",     "WEST",     "SoldierCDFLoadout.json",  "6137.18 8.1828 14667.3",               {"6137.18 8.1828 14667.3", "6127.65 8.1598 14655.7", "6127.19 6.34518 14647.1"} ));
        Data.Group.Insert( new ExpAIGroup(5, "WALK",     "LOOP",     "EAST",     "SoldierMOTRSLoadout.json",  "6167.18 8.1828 14667.3",               {"6167.18 8.1828 14667.3", "6127.65 8.1598 14655.7", "6127.19 6.34518 14647.1"} ));
        Data.Group.Insert( new ExpAIGroup(10,"JOG",      "REVERSE",  "EAST",     "SoldierMOTRSLoadout.json",   "13342.375977 27.020344 11228.225586",  {"13342.375977 27.020344 11228.225586", "13469.069336 11.282786 11163.261719", "13567.762695 6.128973 11127.067383"} ));
        Data.Group.Insert( new ExpAIGroup(10, "SPRINT",   "LOOP",     "INSURGENT","Raiderloadout.json",    "7332.387695 5.712450 2651.121338",     {"7332.387695 5.712450 2651.121338", "7632.803223 5.507639 3075.661865", "7621.013184 5.892049 3083.053223"} ));
        break;
    }
}
