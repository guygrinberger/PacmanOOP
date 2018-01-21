#pragma once
#include "State.h"
#include "StateManager.h"
#include <string>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>
// class for error event
class ErrState : public State
{
public:
	ErrState(const std::string& errortext) : mTex(errortext){}

	virtual void run(PointerPack& pack)
	{
		sf::Text text(mTex, *pack.Font);
		sf::Event eve;

		while(1)
		{
			while(pack.Window->pollEvent(eve))
			{
				if(eve.type == sf::Event::KeyPressed)
				{
					pack.Manager->leaveBottom(one);
					return;
				}
				if(eve.type == sf::Event::Closed)
				{
					pack.Manager->popTop(clean);
					return;
				}
			}
			pack.Window->clear();
			pack.Window->draw(text);
			pack.Window->display();
		}
	}

private:
	std::string mTex;
};
// class for pause event
class PauseState : public State
{
public:
	virtual void run(PointerPack& pack)
	{
		sf::Text text("Paused, press p to resume",*pack.Font);
		sf::Event eve;

		while(1)
		{
			while(pack.Window->pollEvent(eve))
			{
				if(eve.type == sf::Event::KeyPressed && eve.key.code == sf::Keyboard::P)
				{
					pack.Manager->popTop(one);
					return;
				}
			}
			pack.Window->clear();
			pack.Window->draw(text);
			pack.Window->display();
		}
	}
};
// class for quit event
class QuitState : public State
{
public:
	virtual void run(PointerPack& pack)
	{
		sf::Text text("Quit? y/n", *pack.Font);
		sf::Event eve;

		while(1)
		{
			while(pack.Window->pollEvent(eve))
			{
				if(eve.type == sf::Event::KeyPressed && eve.key.code == sf::Keyboard::Y)
				{
					pack.Manager->leaveBottom(one);
					return;
				}
				if(eve.type == sf::Event::KeyPressed && eve.key.code == sf::Keyboard::N)
				{
					pack.Manager->popTop(one);
					return;
				}
			}
			pack.Window->clear();
			pack.Window->draw(text);
			pack.Window->display();
		}
	}
};
