***There is an issue with the lastest version of Expansion-AI for the time being you will need to place your Loadout files into the profiles/eai/ directory
with the Master.c and AISettings.json files until we can get it worked out. Also set the respawn time in both the init_eAI.c and patrolsettings.json to -1 or your ai patrols will not respawn until the next update***

## DROP AND GO SETUP
This is a simple install to get AI Patrols up and running on your servers map. Thanks to truedolphin
it should now auto detect your map and generate some basic patrols for all of the supported maps listed
below[^Maps].
[^Maps]:
    Chernarusplus, chiemsee, deerisle, enoch, esseker, namalsk, takistanplus, more soon
    
<p>All loadouts were made with vanilla equipment and the init.c file has only been altered to include 
the needed lines of code to make the AI mod work.</p>
<p>This is designed to be a drop and go package for a fresh server install using the default mission,
and assumes that you have all the necessary expasion mods and their dependancies installed.</p>
<p>Start and run the server one time to generate the needed files then shutdown first.</p>
<p>Next copy the included directory structres with files into the root of your server directory. 
If your Server profiles directory is named anything other than profiles be sure to place the contents
of profiles into your servers profiles directory.</p>

<p>Open your server profiles directory and in the eAISettings.json file edit the following line of
code with your steamID for Admin settings:</p>

		"Admins":["YOUR STEAMID HERE"]
save and close eAIsettings.json.
Launch and enjoy.

**Note: To use the defaults patrols listed in the init_eAI.c delete the PatrolSettings.json 
that is part of this package and the server will generate a new one based upon the defaults.**


## MANUAL EDITING FOR EXISTING SERVERS

Open your servers profiles directory and copy the supplied eAI and ExpansionMod directories into
your profiles directory.

open the init.c found in your servers mpmissions directory.
Edit the top line of your file to include the following line of code:

		#include "$CurrentDir:mpmissions/dayzOffline.chernarusplus/AI/init_eAI.c"
		(Be sure to change the mission name to the mission you are using if needed.)

Next find the Main method and copy the following code somewhere before the closing bracket
 of the main method. Keep it outside of any for loops inside of main.
 
		//Initialize Patrols----------------------------------
		InitDynamicPatrols();		

save and close init.c

 Now open the supplied /mpmissions/dayzOffline.chernarus/ directory and copy the AI directory into your /mpmissions/"your map"/ directory.
 
 If you are using an older version of eAI or previous versions of the AI_init.c you can
remove the directories and files if you no longer need the data inside.

 You can edit the loadout.json files and the PatrolSettings.json now to suit your servers need
or Launch the server and enjoy.

   All files were provided are from the Expansion-AI Mod, The init_eAI.c courtesy of LieutenantMaster and the "any map" compatibility code by truedolphin
