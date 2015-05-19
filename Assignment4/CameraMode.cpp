#include "CameraMode.h"

static int pressX=0;
static int pressY=0;
int pressState = 0;

Vector3f zeroVec(0,0,0);

void CameraMode::mouse(int button, int state, int x, int y){
	switch (button) {
		case GLUT_RIGHT_BUTTON:
		if(state ==GLUT_DOWN){
			pressState = GLUT_RIGHT_BUTTON;
			pressX =x;
			pressY = y;
			return;
		}
		break;
		
		case GLUT_LEFT_BUTTON:
			if(state ==GLUT_DOWN){
				pressState = GLUT_LEFT_BUTTON;
				pressX =x;
				pressY = y;
				return;
			}

			break;
	}
	this->scene.SceneLocation+= this->scene.SceneDelta;

	this->scene.SceneDelta = zeroVec;

	

}
void CameraMode::mouseMotion(int x, int y){
	if(pressState == GLUT_RIGHT_BUTTON){
		this->scene.SceneDelta.x = x-pressX;
		this->scene.SceneDelta.z = y-pressY;
	}
	else if(pressState == GLUT_LEFT_BUTTON){
		this->scene.SceneDelta.y = y-pressY;
	}


}


CameraMode::CameraMode(Scene& scene) : State(scene)
{
}


CameraMode::~CameraMode(void)
{
}
