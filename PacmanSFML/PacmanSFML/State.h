#pragma once
#include "PointerPack.h"

class State
{
public:
	virtual void run(PointerPack& pack)=0;
	virtual ~State(){}
};
