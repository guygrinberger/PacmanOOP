#pragma once
class gameSettings
{
public:
	static gameSettings & getInstance();
	void setSclaeSize(int maxWindowSize, int maxBoardSize);
	float getScaleSize();
	float scaleSize;

private:
	gameSettings();
	~gameSettings();
};

