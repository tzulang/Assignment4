#include "Scene.h"


Scene::Scene(void) :SceneLocation(0, 0, 0), SceneDelta(0, 0, 0), CameraLocation(0, 0, 0),
					CameraLocDelta(0, 0, 0), SceneRotate(0, 0, 0), SceneRotDelta(0, 0, 0), 
					CameraRotate(0, 0, 0), CameraRotDelta(0, 0, 0), 
					fieldOfViewAngle(0), fieldofViewChaned(false), 
					scaleFactor(1),	 scaleFactorChanged(false)
{
}


Scene::~Scene(void)
{
}
