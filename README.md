# Mario Kart Clone

## Group Members
* Emma Choi pd. 4
* Darius Jankauskas pd. 4
* Sunan Tajwar pd. 4
* Greg Zborovsky pd. 4

## Description
We will be making a top-down version of a game similar to Mario Kart where we plan to implement kart racing with decent graphics, collisions, typical game logic, and networked multiplayer, with four concurrent players supported. As a stretch goal, we will aim to adjust the graphics so that they are psuedo-3D in the style of the SNES’s F-Zero and Super Mario Kart.

## User Interface
We will be using SDL to display graphics using basic sprites to represent each kart and with a scrolling background image to represent the track. Keyboard input will be used to control the user interface and kart. One person will start the server.

## Technical Description
* Topics Covered in Class
  * Networking: one user will be able to host a server allowing up to three others to connect to it. The networking code will synchronize game state between the four clients, with the server acting as the authoritative source of state. We will pass around only a single struct. Any messages must be encoded in that struct.
  * Forking: at startup of the client, a separate process will be started, in which a server instance will be run if the user elects.
  * Signals: if the client elects to start hosting a multiplayer game, a signal will be sent to the other process to indicate a server should start running. To stop it, the main process will send SIGINT.
  * Working with files: Saving data (config, save files)
* Breaking Up the Project
  * Greg → collisions, graphics
  * Darius → main networking, reconciliation
  * Emma → physics, collisions
  * Sunan → input handling, networking
* Data structures/Algorithms
  * Arrays: holding points and structures (like each player)
  * Reconciliation Algorithm: the client will send inputs to the server for processing but will then act on them locally. The potential resulting mismatch when the server sends back authoritative positions must be handled. 
  * AABB Collision and Collision Handling: Determining whether a car is colliding with another car or a wall and what to do after.
  * Separating Axis Theorem *(Maybe)*: this will only be included if we are to account for non-rectangular shapes or any non-axis aligned rectangles
  * Basic Physics: acceleration and turning
  
## Expected Timeline
* Write out code for foundational input handling, simple graphics, and simple physics to test if the car can move → 1/6/20-1/8/20
* Expand collision and physics handling to account for proper bounce and direction during collisions → 1/8/20 - 1/10/20
* Write code that creates a fork for the server, and allow clients to connect to the server → 1/8/20 - 1/10/20
* Send inputs to the server, process them, and send back authoritative position data. → 1/10/20 - 1/12/20
* Reconciliation → 1/13
* Add support for collision between carts → 1/15
* final checks → 1/17

## Required Libraries
* SDL 2.0
  * `sudo apt-get install libsdl2-dev`
