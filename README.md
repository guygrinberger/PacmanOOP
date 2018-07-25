# PacmanSFML
Pacman game coded OOP style using SFML libaray for uni project.
This does **NOT** work like the original pacman game.

![Alt text](https://image.ibb.co/di4C8o/Pac_Man_logo_3_E5987_B905_seeklogo_com_copy.png "Logo")
***
## **General**

In this exercise we were instructed to build a Pacman like game.

***
## **Design**

The pointer pack consists of number of pointer:

1.  Pointer to the window
2.   Pointer to the manager (hold stack of states)
3.  Pointer to font
4.  Pointer to Sound

We used the pointer pack so we can simply use the same window for the entire game and not opening other windows. To be simpler, we use it to add new layers on the same window.

Most of the game is run and managed by states.

The state class has a virtual function "run" which gets a pointer pack.  All kinds of game states inherit from the state class. Lets explain those classes:

**GameState** – Manages the game states. In charge of changing different states during the game. The class draws all objects, loads the map, prints and manages the hud. Its in charge of the controls.
           
**MenuState** – Manages the main menu of the game. The first one to be shown to the user. Its in charge of loading the option picked by the user. 

**ScoreState** – Manages the scoreboard. Gets a text file of the top 10 users that got the highest score. When a new score is needed to be inserted to the top 10, this class does the job.	

**SpecialState** – This class consists of 3 special states: 
1.  ErrState - In charge of types of errors.
2.  PauseState – In charge of pausing the game.
3.  QuitState – In charge of the quit state.

For all states there is 1 manager:

**StateManager** – Manages all the states. Pop, push and other stack actions are here.

We have 1 singleton class:

**GameSettings** – in charge of calculating the right size in pixels for each object so that the map will be adjusted to the size of the windows (which is const).  

We have 1 class that manages the entity:

PacEntity – In charge of returning directions from vector and vectors from direction. Also calculates next step.

And finally, a class that cats as the engine of the game:

PacEngine – Manages the entire engine. Moves, collisions and all physical acts are done here.


***
## **Data Structures and Role**

**STL DUST**- Static and dynamic objects had their own vector of 'shared_ptr'. Stack that holds and manages the game states of 'unique_ptr' type. Many 'multimaps' to hold in an organized way the files with labels so we could find them easily. Furthermore, the collision map is being used in the 'CollisionMnager' to quickly grab the right function for the current collision.

***
## **Algorithms Worth Mentioning**

We made a 'Pixel Perfect' collision in the game. Instead of the regular SFML style collision which only uses the bounding rectangle of the texture to check for collision, our game takes every texture, Process the image so it makes a mask of bits of the actual pixels in the sprite and checks the collision by the pixel.
The jumping mechanism of the game works by a simple physic formula which calculates the distance and height of the character should go depending on the mass of the character and how long the jump key was pressed.

***
## **Known Bugs**
In some places, the player character can get stuck. This caused by the 'pixel perfect' collision.

