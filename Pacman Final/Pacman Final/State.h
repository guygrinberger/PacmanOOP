#pragma once
#include "PointerPack.h"

class State
{
public:
	virtual void run(PointerPack& pack)=0; // abstract class to make in different ways
	virtual ~State(){}
};
