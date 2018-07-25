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

**PacEntity** – In charge of returning directions from vector and vectors from direction. Also calculates next step.

And finally, a class that cats as the engine of the game:

**PacEngine** – Manages the entire engine. Moves, collisions and all physical acts are done here.

***
## **Level File Format**

The level file format is the same as the one in the map maker exercise.

If you want to build a new level from the map maker it will be added automatically with the correct index. 

If you have an external file you want to add, you need to change its index to the last level exists +1.

***
## **Data Structures and Role**

**gameObjects** are stored in vector of vectors.
**States** are stored in a stack.
**BFS** uses queue.

***
## **Algorithms Worth Mentioning**

The monsters chase the Pacman using BFS algorithm for shortest path in a graph.

***
## **Known Bugs**

No bugs.

