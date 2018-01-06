#pragma once

enum eType
{
	PacDied, ScoreChange, PillsExhausted, GameOver
};

class PacEvent
{
public:
	PacEvent(){}
	PacEvent(eType gtype, int gdata) : type(gtype), data(gdata){}
	eType type;
	int data;
};
