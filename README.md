# Mario Kart Clone

## Group Members
* Emma Choi pd. 4
* Darius Jankauskas pd. 4
* Sunan Tajwar pd. 4
* Greg Zborovsky pd. 4

## Description
This is a top-down kart-racing game similar to Mario Kart or F-Zero with decent graphics, collisions, typical game logic, and networked multiplayer, with up to four concurrent players supported.

## How to Use:
To build the game, run `make` (assuming all required libraries have been installed). To run the game, run `make run`. 

### To play Single Player:
After game start up, click "Single Player" in the menu. Your kart will appear on one of three different tracks, each with its own music.

### To play Multiplayer: 
This is the primary mode. Have one person click the start "Create Multiplayer," while all other players click "Join Multiplayer." Players who click "Join Multiplayer" will be prompted to enter the IP address of the hosting computer on the command line in order to connect to it. A countdown will appear on the screen and when it reaches 0, the game will start. The countdown will be reset whenever a client joins the server until its maximum capacity is reached. On a randomly selected track, each of the karts will appear.
 
### Movement:
In the game, the A and D keys will rotate your kart while the W and S keys will allow you to move forward and backwards, respectively. You are unable to move through walls, but you can smooth move along them. You can only rotate while moving above a certain speed.

### End Game:
To win the game, be the first to finish three laps around the track. All other players lose.
 
### Returning to the menu:
 If at any time you wish to quit a game (or waiting period before a race starts) hit the Escape key to return to the menu.

## Known Bugs: 
* The server may occasionally not shutdown requiring manual killing.
* You may need to wait a minute between starting several servers.

## Required Libraries
* SDL 2.0
  * `sudo apt-get install libsdl2-dev`
* SDL 2.0 TTF
  * `sudo apt-get install libsdl2-ttf-dev`
* pkg-config
  * `sudo apt-get install pkg-config`
  
  ## Open source content 
  * We used SDL 2.0 to implement graphics and audio support.
  * We used [Simple-SDL2-Audio](https://github.com/jakebesworth/Simple-SDL2-Audio) as a wrapper around SDL audio functionality to facilitate development. The code was slightly modified, and is contained in `audio_utils.h` and `audio_utils.c`.
  * Used audio from https://www.youtube.com/watch?v=3UvgkDJV-YQ
  * Used audio from https://www.youtube.com/watch?v=PKQdWvVou1c
  * Used audio from https://www.youtube.com/watch?v=3nXl3re6vBI
