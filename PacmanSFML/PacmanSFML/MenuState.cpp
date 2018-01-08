#include "MenuState.h"
#include "StateManager.h"
#include <SFML/Window/Event.hpp>
#include "ScoreState.h"
#include "GameState.h"

MenuState::MenuState(){}

void MenuState::run(PointerPack& pack)
{
	mAtlas.loadFromFile("bibreman2.png");
	yoram.setTexture(mAtlas);
	float yoramY = 600.f;
	yoram.setPosition(10.f, yoramY);
	sf::SoundBuffer introSong, shalomB;
	introSong.loadFromFile("intro.ogg");
	sf::Sound intro, shalom;
	intro.setBuffer(introSong);
	intro.setVolume(20);
	//intro.play();
	shalomB.loadFromFile("shalom.ogg");
	shalom.setBuffer(shalomB);
	mTitle.setFont(*pack.Font);
	mPlay.setFont(*pack.Font);
	mHigh.setFont(*pack.Font);
	mMapMaker.setFont(*pack.Font);
	mQuit.setFont(*pack.Font);

	mTitle.setString("Pacman Biberman");
	mPlay.setString("Press P to play");
	mHigh.setString("Press H to open highscors");
	mMapMaker.setString("Press M to open mapMaker");
	mQuit.setString("Press Q to quit");


	mTitle.setPosition(0.f,75.f);
	mPlay.setPosition(150.f,200.f);
	mHigh.setPosition(150.f,235.f);
	mMapMaker.setPosition(150.f, 270.f);
	mQuit.setPosition(150.f,305.f);
	sf::Event eve;
	sf::Clock biberclock;
	while(true)
	{
		while(pack.Window->pollEvent(eve))
		{
			if(eve.type==sf::Event::Closed||(eve.type==sf::Event::KeyPressed && eve.key.code==sf::Keyboard::Q))
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
		pack.Window->draw(mMapMaker);
		pack.Window->draw(mQuit);
		pack.Window->draw(yoram);
		if(yoramY != 235.f)
		{
			yoram.setPosition(10.f, yoramY);
			yoramY--;
		}
		else
		{

			if (int(biberclock.getElapsedTime().asSeconds()) % 2 == 0)
			{
				mAtlas.loadFromFile("bibreman.png");
				yoram.setTexture(mAtlas);
			}
			else
			{
				mAtlas.loadFromFile("bibreman2.png");
				yoram.setTexture(mAtlas);
				shalom.play();
			}
		}
		pack.Window->display();
	}
}
