#include "ScoreState.h"
#include "StateManager.h"
#include <fstream>
#include <sstream>
#include <SFML/Window/Event.hpp>

ScoreState::ScoreState(int score) : mWritingTo(-1), mScore(score)
{
	mLines.resize(10); // make the text size just 10
	std::ifstream file("High.txt");
	int tmpscores[10] = {0}; // array for the all scores
	for(int i = 0; i < 10; ++i)
	{
		mLines[i].setCharacterSize(20); // limit of the string
		std::string temp;
		file>>temp; // gets the name
		file>>tmpscores[i]; //gets the score
		mLines[i].setString(temp + " " + std::to_string(tmpscores[i])); // concatenation the name & score
	}
	if (mScore != -1) // if is from the menu - not need to check if the score is can be on the scoreboard
	{
		for (int i = 0; i < 10; ++i) // if is new score - input 
		{
			if (tmpscores[i] < mScore && mWritingTo < 0)
			{
				sf::Text temp;
				temp.setCharacterSize(20);
				temp.setString(" " + std::to_string(mScore));
				mWritingTo = i; // line for input the score
				mLines.insert(mLines.begin() + i, temp);
			}
		}
	}
	mLines.resize(10); // make sure there is 10 scores

	for(int i = 0;i < 10; ++i)
	{
		// change the color of the first 3 scores
		mLines[i].setPosition(250.f,50.f + i * 50.f);
		mLines[0].setFillColor(sf::Color::Red);
		mLines[1].setFillColor(sf::Color::Yellow);
		mLines[2].setFillColor(sf::Color::Green);
		if (i >= 3)
			mLines[i].setFillColor(sf::Color::White);
	}
}

ScoreState::~ScoreState()
{
	std::ofstream file("High.txt");
	for(int i = 0; i < 10; ++i) //save the string that player wrote when the player exit from the scoreboard windows
		file << mLines[i].getString().operator std::string() << std::endl;
}

void ScoreState::run(PointerPack& pack)
{
	for(int i = 0; i < 10; ++i) //font for the all scores
		mLines[i].setFont(*pack.Font);
	sf::Event eve;
	BG.loadFromFile("pacmoon1.jpg", sf::IntRect(0, 0, pic, pic));
	background.setColor(sf::Color(255, 255, 255, 128));
	background.setTexture(BG);
	background.setScale((float)board / pic, (float)board / pic);
	while(1)
	{
		while(pack.Window->pollEvent(eve))
		{
			if(eve.type == sf::Event::Closed)
			{
				pack.Manager->leaveBottom(1); // clean all the stack
				return;
			}
			if((eve.type == sf::Event::KeyPressed && eve.key.code==sf::Keyboard::Escape) || (eve.type == sf::Event::KeyPressed && eve.key.code == sf::Keyboard::Return))
			{
				if(mWritingTo >= 0) // the score have been wrote
					mWritingTo = -1;
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
					mWritingLine.erase(--mWritingLine.end()); // delete the last char
					mLines[mWritingTo].setString(mWritingLine + std::to_string(mScore)); // display 
				}
			}
			if(eve.type == sf::Event::TextEntered && eve.text.unicode > 81 && eve.text.unicode < 127 && eve.text.unicode != ' ')// when the player is writing
			{
				if(mWritingTo>=0)
				{
					mWritingLine+=static_cast<char>(eve.text.unicode);
					mLines[mWritingTo].setString(mWritingLine+" " + std::to_string(mScore));
				}
			}
		}
		pack.Window->clear();
		pack.Window->draw(background);
		for(int i=0;i<10;++i) //draw all the scores
			pack.Window->draw(mLines[i]);
		pack.Window->display();
	}
}
