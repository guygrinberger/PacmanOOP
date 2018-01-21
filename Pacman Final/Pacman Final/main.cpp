#include "StateManager.h"
#include "MenuState.h"
#include <time.h>
#include "ConstantsGame.h"
int main()
{
	srand(NULL); // random 
	StateManager app;
	app.pushTop(new MenuState((float)board, (float)board)); //push all the constructor in the stack
	app.gameRun(); // start the program
}