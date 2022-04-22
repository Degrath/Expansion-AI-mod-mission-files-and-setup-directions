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
			startpos[1] = group.StartPos[1]

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
    Data = new ExpAIGroups();
    Data.Group.Insert( new ExpAIGroup(5, "WALK",      "LOOP",     "WEST",     "SoldierCDFLoadout.json",  "6137.18 8.1828 14667.3",               {"6137.18 8.1828 14667.3", "6127.65 8.1598 14655.7", "6127.19 6.34518 14647.1"} ));
    Data.Group.Insert( new ExpAIGroup(5, "WALK",     "LOOP",     "EAST",     "SoldierMOTRSLoadout.json",  "6167.18 8.1828 14667.3",               {"6167.18 8.1828 14667.3", "6127.65 8.1598 14655.7", "6127.19 6.34518 14647.1"} ));
    Data.Group.Insert( new ExpAIGroup(10,"JOG",      "REVERSE",  "EAST",     "SoldierMOTRSLoadout.json",   "13342.375977 27.020344 11228.225586",  {"13342.375977 27.020344 11228.225586", "13469.069336 11.282786 11163.261719", "13567.762695 6.128973 11127.067383"} ));
    Data.Group.Insert( new ExpAIGroup(10, "SPRINT",   "LOOP",     "INSURGENT","Raiderloadout.json",    "7332.387695 5.712450 2651.121338",     {"7332.387695 5.712450 2651.121338", "7632.803223 5.507639 3075.661865", "7621.013184 5.892049 3083.053223"} ));
}
