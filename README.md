This is a simple install to get AI Patrols up and running on you offline.Chenarusplus map
All loadouts were made with vanilla equipment and the init.c file has only been altered 
to include the needed lines of code to make the AI mod work.

Start and run the server one time to generate the needed files then shutdown first
This is designed to be a drop and go package for a fresh server install.
For a vanilla Chenarus map, copy the included init.c file over top of your existing file 
or manually edit the top line of your file to include the following line of code:

		#include "$CurrentDir:mpmissions/dayzoffline.chernarusplus/AI/init_eAI.c"

Next find the Main method and copy the following code somewhere before the closing bracket
 of the main method. Keep it outside of any for loops inside of main.
 
		//Initialize Patrols----------------------------------
		InitDynamicPatrols();
		

close init.c

Next copy the included directory structres with files into your server directory. 
If your Server profiles directory is named anything other than 
profiles be sure to place the contents of profiles into your servers profiles directory.
If you are using an older version of eAI or previous versions of the AI_init.c you can
remove the directories and files if you no longer need the data inside

You can edit the loadout.json files and the PatrolSettings.json now to suit your servers need
or Launch the server and enjoy.


