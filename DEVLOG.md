# Mario Kart Dev Log

## Emma Choi, Darius Jankauskas, Sunan Tajwar, Greg Zborovsky


SAMPLE LOG:
__1/[date]/20__ [NAME]:

[WHAT DID YOU DO?]

__1/19/20__ Darius: Modified level loading code so that it can work on server as well as client. Adjusted level loading code with error and sanity checks. Also introduced collision support into the multiplayer server, allowing for collision support both in single and multiplayer. Allow the level to be selected in the multiplayer server and sent to the client. (Multiple level support augmented)

__1/19/20__ Sunan: Helped find and test use of a generic racing soundtrack in the game background.

__1/19/20__ Darius: Server process now kills itself when its parent is dead. Removed redundant killing from main.

__1/18/20__ Darius: Kill the server process when main exits normally, so it does not remain as a zombie process preventing future servers.

__1/18/20__ Sunan: Tried to adjust collision code after realization on how collisions work traditionally on Mario Kart. Tried to implement it so that collision head on would result in the kart stopping, and collision at an angle would cause it to slide along the terrain. 

__1/16/20__ Sunan:
Researched and got familiar with SDL Rect and functions such as SDL_IntersectRect in order to make an attempt at basic collision code between kart and terrain.


__1/15/20__ Darius:

Added on to Sunan's client input handling on server, and send back authoratative position data to clients. Use Greg's spin loop code instead of mine in the server, allowing for sub-one-second loop iteration times essential for real-time game logic. Set the server to loop for 1/60 seconds per iteration when race starts.


__1/15/20__ Greg:

Added text rendering. Added the click variable. 

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


__1/12/20__ Greg:

idk.


__1/11/20__ Darius:

Finished initial version of server game start code. The game start code listens for SIGUSR1, and then creates a socket and begins listening for client connections. Fixed spin_time, which is a variable used to ensure one loop iteration takes at least 1 second. Fixed debug printf formatting specifier. 


__1/11/20__ Greg:

doot.goot. Some refactoring, added level type, added level struct, Made the Kart rotate around the ship. 

__1/9/20__ Sunan:
Worked on some basic networking and servers based off of MR. DWs boilerplate and Darius' progress. Looked into listening for server and countdown functions based off Darius' skeleton.

__1/8/20__ Greg:

Worked on the collision generator. Refactored SOMETHING SOMETHING SOMETHING. Update py collision code. 


__1/8/20__ Emma:

Realized new method will still not mimic the Mario Kart physics properly. Completely rewrote again using the vectors file added by Greg, combined with the first method.


__1/8/20__ Darius:

Merged incoming and local. Completed skeleton of potential networking protocol. The networking protocol will use two packet types: one sent by the server and one by clients. A type field will store which field of a data union is active, ensuring saved packet space. Also added skeleton of future code within server_main. Added. Mr. DW's boilerplate to create a server socket. 


__1/7/20__ TEAM:

Each played a round of Mario Kart to get a feel of the game and discussed how we could implement basic features.


__1/6/20__ Emma:

Realized current method of physics will not work. Rewrote into a completely new method. Added cases handling for physics.


__1/5/20__ Emma:

Fixed up errors found in the initial physics code.


__1/4/20__ Emma:

Made initial draft of the basic physics code. Was able to get particle to move in all four directions, and accelerate using jerk.
