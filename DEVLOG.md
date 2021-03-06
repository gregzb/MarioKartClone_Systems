# Mario Kart Dev Log

## Emma Choi, Darius Jankauskas, Sunan Tajwar, Greg Zborovsky


SAMPLE LOG:
__1/[date]/20__ [NAME]:

[WHAT DID YOU DO?]


__1/21/20__ Greg:

Added text to show win/loss scenario. Added kart size to level file. Added remaining levels into code. Added timer on end screen to boot player back to menu. Fixed audio pausing issues. Reset rest of necessary client data. Adjusted server kill logic. Added random level selection. Added game name to menu. Fixed a memory leak on the server.

__1/21/20__ Emma: Created another track for a second alternate "level" using GIMP. Created both the basic collision box map and the "pretty" version with a completely different theme.


__1/21/20__ Darius: Move lap logic from main into a separate set of files. Remove dependence on global variables, and add lap logic to the server. Also introduce messaging to tell clients when the game is over, utilizing lap count.


__1/20/20__ Greg:

Added new test level. Created floating point rectangle and rectangle intersection. Switched to floating point collision code. No more issues with jittery collision. Again, slightly refactored client code. Added level logic. Using a system of a goal and 2 other checkpoints, players are forced to drive through the map in the correct direction to gain a lap. Added lap counter and win/loss states. Fixed issue with missing audio. Fixed issue with incorrect multiplayer state. Reset some player data on win. 


__1/20/20__ Darius: Add music file support to the level data format. Also introduce wrapper library for SDL Audio, and call it to provide in-game music as dictated by currently active level. Finally, better handle pressing the escape key, "restarting" the server process (aka resetting its state) by sending it `SIGQUIT`. Tackle some of the subsequently introduced bugs.


__1/19/20__ Greg:

Updated python collision box generator (switched to little endian). Rehauled kart collision, removed unnecessary code. Adjusted connection logic in client. Added spawnpoints, goal lines, and checkpoints to the level generator code. Updated game to parse the new level format. Zero'ed out level struct memory to avoid a pesky bug.


__1/19/20__ Emma: Created the "pretty" version, the version that the users will see as the track for the alternate level created the day before. Lots of experimentation had to be done with learning how to use GIMP.


__1/19/20__ Darius: Modified level loading code so that it can work on server as well as client. Adjusted level loading code with error and sanity checks. Also introduced collision support into the multiplayer server, allowing for collision support both in single and multiplayer. Allow the level to be selected in the multiplayer server and sent to the client. (Multiple level support augmented)


__1/19/20__ Emma: Created another track for an alternate "level" using GIMP. Created the basic collision box map.


__1/19/20__ Sunan: Helped find and test use of a generic racing soundtrack in the game background.


__1/19/20__ Darius: Server process now kills itself when its parent is dead. Removed redundant killing from main.


__1/18/20__ Greg:

Added image-based positions instead of resolution-based positions, adjusted rendering code accordingly. Also fixed rotation of non-local karts in multiplayer. Removed unnecessary setter functions. Updated some movement code to no longer depend on framerate.


__1/18/20__ Darius: Kill the server process when main exits normally, so it does not remain as a zombie process preventing future servers.


__1/18/20__ Sunan + Emma: Discussed a new way to make collisions work properly. Tried to adjust collision code after realization on how collisions work traditionally on Mario Kart (whcih differed significantly from our first two methods). Tried to implement it so that collision head on would result in the kart stopping, and collision at an angle would cause it to slide along the terrain. 


__1/17/20__ Emma:
Realized the current collision methods will not work and added in new physics code for the second idea for collisions. 

__1/16/20__ Greg:

Added multiplayer state for client (waiting or playing). Added the multiplayer wait screen. Created a general function for text rendering. Completed a merge further integrating server and client.


__1/16/20__ Sunan:
Researched and got familiar with SDL Rect and functions such as SDL_IntersectRect in order to make an attempt at basic collision code between kart and terrain.


__1/16/20__ Emma: 
Added in extra physics code so that current ideas for collisions could be done, such as reversing the current direction and velocity.


__1/15/20__ Darius:

Added on to Sunan's client input handling on server, and send back authoratative position data to clients. Use Greg's spin loop code instead of mine in the server, allowing for sub-one-second loop iteration times essential for real-time game logic. Set the server to loop for 1/60 seconds per iteration when race starts.


__1/15/20__ Greg:

Added more library install requirements. Added text rendering. Allowed ship rotation at any time. Somewhat refactored client code. Allowed for multiple kart rendering, and figured out math for rendering other objects on screen. Created basic menu with buttons to select game mode. Now tracking mouse position and click state. Added basic UI for when connecting to server. Added option to use escape key to go back to menu. Refactored buttons into a button function. Server ip can now be entered in terminal.


__1/15/20__ Sunan:
Worked on the collision code using SDL_IntersectRect. This involved implementing change in velocity and direction when there is a collision between thr kart and terrain.


__1/14/20__ Darius:

Integrated networking into the main game loop, and added basic state management to client and server using enums to represent connection and menu states. Removed debugging, fixed connecting client to server.


__1/13/20__ Sunan:
Worked off of Darius' notes in netowrking and server code. Worked on adding fields for clients, handling basic messages, and updating current_inputs.


__1/13/20__ Darius:

Added boilerplate and notes for Sunan, indicating data types for inputs sent from clients and where to call functions that process those inputs. Also begin authoratative position data design. Ran formatter and fixed a return value after server.main. 


__1/12/20__ Darius:

Added function prototypes for adapted Mr. DW networking code to networking.h. Added networking code into the client-side codebase, which is currently segregated from but will be integrated into the main game loop. Switched to using pipe rather than signals to signify server start. Moved networking code from server.c.


__1/11/20__ Darius:

Finished initial version of server game start code. The game start code listens for SIGUSR1, and then creates a socket and begins listening for client connections. Fixed spin_time, which is a variable used to ensure one loop iteration takes at least 1 second. Fixed debug printf formatting specifier. 


__1/11/20__ Greg:

Adjusted kart movement. Figured out math to allow world to correctly rotate around ship.


__1/10/20__ Greg:

Reworked kart struct functions. Removed several functions, updated movement logic, added drag. Began rendering kart to screen. Added level file creation to python script. Added level file struct to game and began working on a level file parsing function. Allow screen to move around point instead of letting poitn move around screen.


__1/9/20__ Sunan:
Worked on some basic networking and servers based off of MR. DWs boilerplate and Darius' progress. Looked into listening for server and countdown functions based off Darius' skeleton.


__1/8/20__ Greg:

Updated/added/refactored timing code. Frames should now be more consistently 1/60 seconds long. Added image loading code. Integrated my code with Darius's networking code. Created a python script to generate level files for the game. Updated several vector functions. Edited makefile. Slightly refactored project structure. Added code to python script to actually recognize collision boxes from an image.


__1/8/20__ Emma:

Realized new method will still not mimic the Mario Kart physics properly. Completely rewrote again using the vectors file added by Greg, combined with the first method.


__1/8/20__ Darius:

Merged incoming and local. Completed skeleton of potential networking protocol. The networking protocol will use two packet types: one sent by the server and one by clients. A type field will store which field of a data union is active, ensuring saved packet space. Also added skeleton of future code within server_main. Added. Mr. DW's boilerplate to create a server socket. 


__1/7/20__ Greg:

Added support for SDL2, dealt with linking and compiling. Basic window and rectangle drawn on screen. Basic keyboard input is also supported. Added mathematical vectors and function to more easily deal with direction, velocity, acceleration, position, etc.


__1/7/20__ TEAM:

Added make


__1/7/20__ TEAM:

Each played a round of Mario Kart to get a feel of the game and discussed how we could implement basic features.


__1/6/20__ Emma:

Realized current method of physics will not work. Rewrote into a completely new method. Added cases handling for physics.


__1/5/20__ Emma:

Fixed up errors found in the initial physics code.


__1/4/20__ Emma:

Made initial draft of the basic physics code. Was able to get particle to move in all four directions, and accelerate using jerk.
