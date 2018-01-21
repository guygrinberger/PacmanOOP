#pragma once
class gameSettings
{
public:
	static gameSettings & getInstance(); // singalton
	void setSclaeSize(int maxWindowSize, int maxBoardSize); // makes the size of the windows
	float getScaleSize(); // gets the size
	float scaleSize;

private:
	gameSettings();
	~gameSettings();
};

