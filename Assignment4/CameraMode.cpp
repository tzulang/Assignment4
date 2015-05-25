#include "CameraMode.h"

static int pressX=0;
static int pressY=0;
static int pressState = 0;

static Vector3f zeroVec(0,0,0);

void CameraMode::mouse(int button, int state, int x, int y){
	if(state ==GLUT_DOWN){
		pressState = button;
		pressX =x;
		pressY = y;
		
	}
	else{
		this->scene.CameraLocation+= this->scene.CameraLocDelta;
		this->scene.CameraLocDelta = zeroVec;
		this->scene.CameraRotate += this->scene.CameraRotDelta;
		this->scene.CameraRotDelta = zeroVec;
	}


	

}
void CameraMode::mouseMotion(int x, int y){
	if(pressState == GLUT_RIGHT_BUTTON){
		this->scene.CameraLocDelta.x += x-pressX;
		this->scene.CameraLocDelta.y -= y-pressY;
		pressX = x;
		pressY = y;
	}
	else if(pressState == GLUT_MIDDLE_BUTTON){
		this->scene.CameraLocDelta.z += y-pressY;
		pressX = x;
		pressY = y;
	}
	else if(pressState == GLUT_LEFT_BUTTON)
	{
		this->scene.CameraRotDelta.x =+ ((0.0+x-pressX)/W_WIDTH);
		this->scene.CameraRotDelta.y =+ ((0.0+y-pressY)/W_HEIGHT);
		pressX = x;
		pressY = y;
	}


}


CameraMode::CameraMode(Scene& scene) : State(scene)
{
}


CameraMode::~CameraMode(void)
{
}
