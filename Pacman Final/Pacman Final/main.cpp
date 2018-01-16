#include "StateManager.h"
#include "MenuState.h"
#include <time.h>
int main()
{
	srand(NULL);
	StateManager app;
	app.pushTop(new MenuState((float)800, (float)800));
	app.gameRun();
}