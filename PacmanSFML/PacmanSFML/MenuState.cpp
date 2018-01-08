#include "MenuState.h"
#include "StateManager.h"
#include <SFML/Window/Event.hpp>
#include "ScoreState.h"
#include "GameState.h"

MenuState::MenuState(){}

void MenuState::run(PointerPack& pack)
{
	mAtlas.loadFromFile("bibreman.png");
	yoram.setTexture(mAtlas);
	yoram.setPosition(10.f, 600.f);
	sf::SoundBuffer introSong;
	introSong.loadFromFile("intro.ogg");
	sf::Sound intro;
	intro.setBuffer(introSong);
	//intro.play();
	mTitle.setFont(*pack.Font);
	mPlay.setFont(*pack.Font);
	mHigh.setFont(*pack.Font);
	mQuit.setFont(*pack.Font);

	mTitle.setString("Pacman Biberman");
	mPlay.setString("Press P to play!");
	mHigh.setString("Press H to highscors");
	mQuit.setString("Press Q to quit");

	mTitle.setPosition(0.f,75.f);
	mPlay.setPosition(150.f,200.f);
	mHigh.setPosition(150.f,235.f);
	mQuit.setPosition(150.f,270.f);
	sf::Event eve;

	sf::Clock yoramClock;
	float yoramY = 600.f;

	while(true)
	{
		while(pack.Window->pollEvent(eve))
		{
			if(eve.type==sf::Event::Closed||(eve.type==sf::Event::KeyPressed&&eve.key.code==sf::Keyboard::Q))
			{
				pack.Manager->popTop(100);
				return;
			}
			else if(eve.type == sf::Event::KeyPressed)
			{
				if(eve.key.code==sf::Keyboard::P)
				{
					pack.Manager->pushTop(new GameState());
					return;
				}
				if(eve.key.code == sf::Keyboard::H)
				{
					pack.Manager->pushTop(new ScoreState(-1));
					return;
				}
			}
		}
		pack.Window->clear();
		pack.Window->draw(mTitle);
		pack.Window->draw(mPlay);
		pack.Window->draw(mHigh);
		pack.Window->draw(mQuit);
		pack.Window->draw(yoram);
		if(yoramY != 200.f)
		{
			yoram.setPosition(10.f, yoramY);
			yoramY--;
		}
		pack.Window->display();
	}
}
