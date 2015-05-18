#pragma once
#include "Scene.h"


class State
{
private:
	Scene& scene;

public:
	virtual void mouse(int button, int state, int x, int y) =0;
	virtual void mouseMotion(int x, int y) =0;
	State(Scene& scene);
	virtual ~State(void);
};

