This is a simple install to get AI Patrols up and running on your servers offline.Chenarusplus map.
All loadouts were made with vanilla equipment and the init.c file has only been altered to include 
the needed lines of code to make the AI mod work.

This is designed to be a drop and go package for a fresh server install using the default mission.

Start and run the server one time to generate the needed files then shutdown first.

Next copy the included directory structres with files into the root of your server directory. 
If your Server profiles directory is named anything other than profiles be sure to place the contents
of profiles into your servers profiles directory.

Open your server profiles directory and in the eAISettings.json file edit the following line of
code with your steamID for Admin settings:

		"Admins":["YOUR STEAMID HERE"]
save and close eAIsettings.json.
Launch and enjoy.


MANUAL EDITING FOR EXISTING SERVERS

Open your servers profiles directory and copy the supplied eAI and ExpansionMod directories into
your profiles directory.

Edit the top line of your file to include the following line of code:

		#include "$CurrentDir:mpmissions/dayzoffline.chernarusplus/AI/init_eAI.c"
		(Be sure to change the mission name to the mission you are using if needed.)

Next find the Main method and copy the following code somewhere before the closing bracket
 of the main method. Keep it outside of any for loops inside of main.
 
		//Initialize Patrols----------------------------------
		InitDynamicPatrols();		

save and close init.c

 Now open the supplied mpmissions directory can copy the AI directory into your mpmissions directory.
 
 If you are using an older version of eAI or previous versions of the AI_init.c you can
remove the directories and files if you no longer need the data inside.

 You can edit the loadout.json files and the PatrolSettings.json now to suit your servers need
or Launch the server and enjoy.
