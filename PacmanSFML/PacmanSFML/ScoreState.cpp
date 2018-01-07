#include "ScoreState.h"
#include "StateManager.h"
#include <fstream>
#include <sstream>
#include <SFML/Window/Event.hpp>

ScoreState::ScoreState(int score) : mWritingTo(-1),mScore(score)
{
	mLines.resize(10);
	std::ifstream file("High.txt");
	int tmpscores[10];
	for(int i = 0; i < 10; ++i)
	{
		mLines[i].setCharacterSize(20);
		std::string temp;
		file>>temp;
		file>>tmpscores[i]; 
		mLines[i].setString(temp + " " + std::to_string(tmpscores[i]));
	}
	for(int i = 0; i < 10; ++i)
	{
		if(tmpscores[i] < mScore && mWritingTo < 0)
		{
			sf::Text temp;
			temp.setCharacterSize(20);
			temp.setString(" " + std::to_string(mScore));
			mWritingTo=i;
			mLines.insert(mLines.begin()+i,temp);
		}
	}

	mLines.resize(10);

	for(int i = 0;i < 10; ++i)
	{
		mLines[i].setPosition(100.f,25.f+i*25.f);
	}
}

ScoreState::~ScoreState()
{
	std::ofstream file("High.txt");
	for(int i = 0; i < 10; ++i)
	{
		file << mLines[i].getString().operator std::string() << std::endl;
	}
}

void ScoreState::run(PointerPack& pack)
{
	for(int i = 0; i < 10; ++i)
	{
		mLines[i].setFont(*pack.Font);
	}
	sf::Event eve;

	while(1)
	{
		while(pack.Window->pollEvent(eve))
		{
			if(eve.type == sf::Event::Closed)
			{
				pack.Manager->leaveBottom(1);
				return;
			}
			if(eve.type == sf::Event::KeyPressed && eve.key.code==sf::Keyboard::Return)
			{
				if(mWritingTo >= 0)
				{
					mWritingTo = -1;
				} 
				else
				{
					pack.Manager->leaveBottom(1);
					return;
				}
			}
			if(eve.type == sf::Event::KeyPressed && eve.key.code == sf::Keyboard::BackSpace)
			{
				if(!mWritingLine.empty())
				{
					mWritingLine.erase(--mWritingLine.end());
					mLines[mWritingTo].setString(mWritingLine + std::to_string(mScore));
				}
			}
			if(eve.type == sf::Event::TextEntered && eve.text.unicode > 81 && eve.text.unicode < 127 && eve.text.unicode != ' ')
			{
				if(mWritingTo>=0)
				{
					mWritingLine+=static_cast<char>(eve.text.unicode);
					mLines[mWritingTo].setString(mWritingLine+" " + std::to_string(mScore));
				}
			}
		}
		pack.Window->clear();
		for(int i=0;i<10;++i) pack.Window->draw(mLines[i]);
		pack.Window->display();
	}
}
