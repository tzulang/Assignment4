#pragma once
#include <string>
#include "Vector3f.h"
#include "glut.h"
#include <vector>

using namespace std;

typedef struct Face{
	GLuint vertice[3];
	GLuint normal [3];
};

typedef struct Group{

	int name;
	vector<Face> *faces;
};

typedef struct Point{

	int x;
	int y;
};

typedef struct object3D{ 
	int name;
	vector<Group*> *groups;
};


class Scene
{
public:
	 //State * state;
	 vector<Vector3f> vertices;
	 vector<Vector3f> normals;
	 vector<object3D*> objects;

	 int Buttom;

	 Vector3f SceneLocation;
	 Vector3f SceneDelta;
	 Vector3f RotateLocation;
	 Vector3f RotateDelta;

	Scene(void);
	~Scene(void);
};

