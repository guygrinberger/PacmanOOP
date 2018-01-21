#include "StateManager.h"
#include "State.h"

StateManager::StateManager() : mWindow(sf::VideoMode(800,800),"Pacman By Guy Grinberger & Gerard Verbun", sf::Style::Close | sf::Style::Titlebar)
{
	mWindow.setFramerateLimit(60);//60 fps max
	mFont.loadFromFile("diablo_h.ttf");
	mPack.Font = &mFont;
	mPack.Manager = this;
	mPack.Window = &mWindow;
	mPack.Sound = &mSound;
}

StateManager::~StateManager()
{
	popTop(int(mStates.size()));
	clearStacks();
}

void StateManager::popTop(int amount)
{
	amount = std::min(amount, static_cast<int>(mStates.size()));
	for(int i = 0; i < amount ;++i)
	{
		mClear.push(mStates.top());
		mStates.pop();
	}
}

void StateManager::leaveBottom(int amount)
{
	popTop(int(mStates.size() - amount));
}

void StateManager::pushTop(State * state)
{
	mStates.push(state);
}

void StateManager::gameRun()
{
	while(!mStates.empty())
	{
		mStates.top()->run(mPack);
		clearStacks();
	}
}

void StateManager::clearStacks()
{
	while(!mClear.empty())
	{
		delete mClear.top();
		mClear.pop();
	}
}
