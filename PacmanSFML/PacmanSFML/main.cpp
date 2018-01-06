#include "StateManager.h"
#include "MenuState.h"

int main()
{
	StateManager app;
	app.pushTop(new MenuState);
	app.gameRun();
}