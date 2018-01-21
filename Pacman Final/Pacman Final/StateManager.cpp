#include "StateManager.h"
#include "State.h"

StateManager::StateManager() : mWindow(sf::VideoMode(800,800),"Pacman By Guy Grinberger & Gerard Verbun", sf::Style::Close | sf::Style::Titlebar) //initialize the window size and title
{
	mWindow.setFramerateLimit(60); //set fps limit to 60
	mFont.loadFromFile("diablo_h.ttf"); //load font
	mPack.Font = &mFont; //assign font to pack
	mPack.Manager = this; //assign 'this' manager to pack
	mPack.Window = &mWindow; //assign windows to pack
	mPack.Sound = &mSound; //assign background sound to pack
}

StateManager::~StateManager()//destructor for state manager
{
	popTop(int(mStates.size())); //basicaly just clears the stack (first pop all values then clear)
	clearStacks();
}

void StateManager::popTop(int amount) //pop 'amount' of items from stack
{
	amount = std::min(amount, static_cast<int>(mStates.size())); //if 'amount' is bigger then stack size, take the stack size so it wont throw exeption
	for(int i = 0; i < amount ;++i) //for loop to pop 'amount' number of items from stack
	{
		mClear.push(mStates.top());
		mStates.pop();
	}
}

void StateManager::leaveBottom(int amount) //leave 'amount' of items in the bottom of the stack
{
	popTop(int(mStates.size() - amount)); // size of stack - amount of items you want to remove = items you want to leave
}

void StateManager::pushTop(State * state) //push certain state to the top of the stack
{
	mStates.push(state);
}

void StateManager::gameRun() //run game
{
	while(!mStates.empty()) //as long as there are states in the stack
	{
		mStates.top()->run(mPack); //run game with the current pack
		clearStacks(); //then clear
	}
}

void StateManager::clearStacks() //empty stacks
{
	while(!mClear.empty())
	{
		delete mClear.top();
		mClear.pop();
	}
}
