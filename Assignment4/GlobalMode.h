#pragma once
#include "state.h"


class GlobalMode :
	public State

{
	public:
		virtual void mouse(int button, int state, int x, int y);
		virtual void mouseMotion(int x, int y);
		GlobalMode(Scene& scene);
		virtual ~GlobalMode(void);
};

