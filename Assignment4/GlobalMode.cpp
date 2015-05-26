#include "GlobalMode.h"

static int pressX=0;
static int pressY=0;
static int pressState = 0;


static Vector3f zeroVec(0,0,0);


void GlobalMode::mouse(int button, int state, int x, int y){
	if(state ==GLUT_DOWN){
		pressState = button;
		pressX =x;
		pressY = y;
		
	}
	else{
		this->scene.SceneLocation+= this->scene.SceneDelta;
		this->scene.SceneDelta = zeroVec;
		this->scene.SceneRotate+= this->scene.SceneRotDelta;
		this->scene.SceneRotDelta = zeroVec;
	}

}
void GlobalMode::mouseMotion(int x, int y){
	if(pressState == GLUT_RIGHT_BUTTON){
		this->scene.SceneDelta.x += ((0.0+x-pressX)/W_WIDTH)*50;
		this->scene.SceneDelta.y -= ((0.0+y-pressY)/W_HEIGHT)*50;
		pressX = x;
		pressY = y;
	}
	else if(pressState == GLUT_MIDDLE_BUTTON){
		this->scene.SceneDelta.z += ((0.0+y-pressY)/W_HEIGHT)*50;
		pressX = x;
		pressY = y;
	}
	else if(pressState ==GLUT_LEFT_BUTTON )
	{
		this->scene.SceneRotDelta.x += ((0.0+x-pressX)/W_WIDTH);
		this->scene.SceneRotDelta.y += ((0.0+y-pressY)/W_HEIGHT);
		pressX = x;
		pressY = y;
	}
}


GlobalMode::GlobalMode(Scene& scene) :State(scene)
{
}


GlobalMode::~GlobalMode(void)
{
}
