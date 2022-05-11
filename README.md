
## INCOMING CHANGES - I cant do it all tonigt and the readme will be updated last, I am uploading the new files structure and files so you can follow along. Nearly everything has moved to its final home. Currently the init_eAI.c has a small issue with DeerIsle so the default partols have been commented out until I can fix them. The drop and go setup should work still with the new package, I just tested a new server to be sure but I only ran it once. Hope I can get everything else up to date by tomorrow evening at the latest.

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

		## UPDATED 5/10/2022-----
		#include "$CurrentDir:ServerProfile/ExpansionMod/AI/Patrols/init_eAI.c"
		(This file is no longer in your missions folder.)
		## -------------
Next find the Main method and copy the following code somewhere before the closing bracket
 of the main method. Keep it outside of any for loops inside of main.
 
		//Initialize Patrols----------------------------------
		InitDynamicPatrols();		

save and close init.c

 
 You can edit the loadout.json files and the PatrolSettings.json now to suit your servers need
or Launch the server and enjoy.

   All files were provided are from the Expansion-AI Mod, The init_eAI.c courtesy of LieutenantMaster and the "any map" compatibility code by truedolphin
   
