#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#include "glut.h"
#include "Vector3f.h"
#include "ObjectParser.h"
#include "Scene.h"
#include "State.h"
#include "GlobalMode.h"
#include "CameraMode.h"







using namespace std;

 GLfloat rot;
 Scene scene;


GlobalMode GlobalState(scene);

CameraMode CameraState(scene);

State * ScaneState = &CameraState;

vector<string> &split(const string &s, char delim, std::vector<string> &elems, bool ignoreEmpty) {

	stringstream ss(s);
	string item;

	while (std::getline(ss, item, delim)) {
		if (ignoreEmpty&&item.empty())
			continue;
		elems.push_back(item);
	}
	return elems;

}



inline Vector3f parseVec(vector<string> &params){

	return Vector3f(stof(params[1]), stof(params[2]), stof(params[3]));
	 
}

inline Face parsFace(vector<string> &params){


	Face face;
	for (int i = 0; i < 3; i++){
			
		int index = params[i + 1].find('/');
		if (index < 0)
			break;
		
		 
		
		face.vertice[i]  = stoi(params[i + 1].substr(0, index));
		face.normal[i]	 = stoi(params[i + 1].substr(index + 2));

	}
	return face;
}

void ParseFile(string fileName){

	std::ifstream file;
	file.open(fileName, ios::in);
	string line;

 

	if (!file.is_open()){
		printf("error cannot open file %s\n", fileName);
		return;
	}

	object3D* currentObject	= NULL;
	Group*	  currentGroup	= NULL; 

	while (std::getline(file, line))
	{
		if (line.size() == 0 || line[0] == '#')
			continue;

		vector<string> params;
		split(line, ' ', params, true);
	 
		 
		if (params[0] == "o"){
			currentObject = (object3D*)malloc(sizeof (object3D));
			currentObject->name = (params.size() > 1) ? stoi(params[1]) : -1;
			currentObject->groups = new vector<Group*>();
			scene.objects.push_back(currentObject);
			continue;
		}

		if (params[0] == "g"){
			if (currentObject == 0){
				currentObject = (object3D*)malloc(sizeof (object3D));
				currentObject->name = -1;
				currentObject->groups = new vector<Group*>();
				scene.objects.push_back(currentObject);
			}

			currentGroup = (Group*)malloc(sizeof (Group));
			currentGroup->name = (params.size() > 1) ? stoi(params[1]) : -1;
			
			currentObject->groups->push_back(currentGroup);

			currentGroup->faces = new vector<Face>();
			continue;
		}
		if (params[0] == "f"){
			currentGroup->faces->push_back(parsFace(params));
			continue;
		}
		if (params[0] == "v"){
			scene.vertices.push_back(parseVec(params));
			continue;
		}
		if (params[0] == "vn"){
			scene.normals.push_back(parseVec(params));
			continue;

		}
		
	}

	file.close();

}

void init()
{
 
	glClearColor(0, 0, 0, 1); //black background
	glReadBuffer(GL_BACK);


	glMatrixMode(GL_PROJECTION); /* switch matrix mode */
	glLoadIdentity();		//load Identity matrix
	
	//defines view mode
	gluPerspective(60, 1, 2, 200);
	glTranslatef(0, 0, -150);
 
	
	glEnable(GL_DEPTH_TEST);  //define in which order the scene will built
	//glEnable(GL_NORMALIZE);

	/* return to modelview mode */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}


void initLight()
{
	//lightning

	//glEnable(GL_COLOR_MATERIAL);
	
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glColorMaterial(GL_BACK, GL_AMBIENT_AND_DIFFUSE);

	//GLfloat emission[] = { 0, 0, 0.2 };
	GLfloat light_direction[] = { 0.0, -0.0, -1.0, 1.0 };
	GLfloat light_ambient[] =	{ 0.5,  0.5,  0.5, 1.0 }; //color
	GLfloat light_diffuse[] =	{ 0.0,  0.5,  0.5, 1.0 }; //color
	GLfloat light_specular[] =	{ 0.0,  0.0,  0.5, 1.0 };
	GLfloat light_position[] =	{ -0.0,  -1, -1, 0, 0.0 };
	//GLfloat angle[] = { 20.0 };


	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

//	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);
//	glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF,angle);

	
	

	GLfloat mat_a[] = { 0.3, 0.4, 0.3, 1.0 };
	GLfloat mat_d[] = { 0.7, 0.6, 0.7, 1.0 };
	GLfloat mat_s[] = { 0.5, 0.0, 0.8, 1.0 };
	GLfloat low_sh[] = { 5 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, low_sh);

	//	glEnable(GL_COLOR_MATERIAL);
	//glDisable(GL_LIGHTING);

	//GLfloat emission[] = { 0, 0, 0.2 };
	//GLfloat light_direction[] = { 0, 0.0, -1, 1.0 };
	//GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 }; //color
	//GLfloat light_diffuse[] = { 0.1, 0.7, 0.7, 1.0 }; //color
	//GLfloat light_specular[] = { 0.3, 0.3, 0.3, 1.0 };
	//GLfloat light_position[] = { -0.0, -1.5, 1, 0, 0.0 };
	//GLfloat angle[] = { 20.0 };

	//glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	////glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);
	////glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF,angle);

	//glShadeModel(GL_FLAT);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	//GLfloat mat_a[] = { 0.1, 0.5, 0.5, 1.0 };
	//GLfloat mat_d[] = { 0.1, 0.7, 0.7, 1.0 };
	//GLfloat mat_s[] = { 0.0, 0.0, 0.9, 1.0 };
	//GLfloat low_sh[] = { 5.0 };

	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, low_sh);

}



void printProj() //prints projection matrix
{
	float  projectionMatrix[16];
	glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);
	printf("projection matrix: \n");
	for (int i = 0; i<4; i++)
	{
		for (int j = 0; j<4; j++)
			printf("%f ", projectionMatrix[j * 4 + i]);
		printf("\n");
	}
}

void drawObj(){

	int objectNum = scene.objects.size();
	
	//glEnable(GL_LIGHTING);
	//glDisable(GL_COLOR_MATERIAL);
	
	for (size_t o = 0; o < objectNum; o++){

		int groupNum = scene.objects[o]->groups->size();
		
		for (size_t g = 0; g < groupNum; g++ ){
			vector<Face> *faces = scene.objects[o]->groups->at(g)->faces;
			int facesNum = faces->size();		
			//glPushMatrix();
			glBegin(GL_TRIANGLES);
			//glColor3f(0.3f, 0.5f, 0.13f);
			for (int f=0; f < facesNum; f++){
				
				Face *face = &faces->at(f);
				
				for (int i = 0; i < 3; i++){
					int n = face->normal[i]-1;
					int v = face->vertice[i]-1;
					float f = float(i) / facesNum;
					//glColor3f(f, f*2, f);
					Vector3f nn= scene.normals[n];
					Vector3f vv= scene.vertices[v];
					glNormal3f(scene.normals[n].x, scene.normals[n].y, scene.normals[n].z);
					glVertex3f(scene.vertices[v].x, scene.vertices[v].y, scene.vertices[v].z);
				}
			}
			glEnd();
		//	glPopMatrix();
		}

	}
//	glEnable(GL_LIGHTING);
//	glDisable(GL_COLOR_MATERIAL); 
	//glFlush();
}

void mydisplay()
{
	glDrawBuffer(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear back buffer	

 
	//glRotatef(0.1, 0, 1,0 ); //rotate scene


	glLoadIdentity();
	/*
	gluLookAt(scene.CameraLocation.x + scene.CameraLocDelta.x,
			scene.CameraLocation.y+ scene.CameraLocDelta.y,
			scene.CameraLocation.z+ scene.CameraLocDelta.z,
			scene.CameraLocation.x + scene.CameraLocDelta.x,
			scene.CameraLocation.y+ scene.CameraLocDelta.y,
			scene.CameraLocation.z+ scene.CameraLocDelta.z-300,
			0,1,0);*/
	float Sx = scene.SceneLocation.x + scene.SceneDelta.x;
	float Sy = scene.SceneLocation.y+ scene.SceneDelta.y;
	float Sz = scene.SceneLocation.z+ scene.SceneDelta.z;

	float Cx = scene.CameraLocation.x + scene.CameraLocDelta.x;
	float Cy = scene.CameraLocation.y+ scene.CameraLocDelta.y;
	float Cz = scene.CameraLocation.z+ scene.CameraLocDelta.z;

	//Translate camera + scene
	glTranslatef(scene.SceneLocation.x + scene.SceneDelta.x-(scene.CameraLocation.x + scene.CameraLocDelta.x),
				scene.SceneLocation.y+ scene.SceneDelta.y-(scene.CameraLocation.y + scene.CameraLocDelta.y),
				scene.SceneLocation.z+ scene.SceneDelta.z-(scene.CameraLocation.z + scene.CameraLocDelta.z));
	

	//Rotate Scene
	glTranslatef(Sx,Sy,Sz);
	glRotatef((scene.SceneRotate.x+scene.SceneRotDelta.x)*180,0,1,0);
	glRotatef((scene.SceneRotate.y+scene.SceneRotDelta.y)*180,1,0,0);
	
	glTranslatef(-1*Sx,-1*Sy,-1*Sz);
	//Rotate Camera
	glTranslatef(Cx,Cy,Cz+150);
	glRotatef((scene.CameraRotate.x+scene.CameraRotDelta.x)*180,0,1,0);
	glRotatef((scene.CameraRotate.y+scene.CameraRotDelta.y)*180,1,0,0);
	glTranslatef(-1*Cx,-1*Cy,-1*(Cz+150));

	


	drawObj();
 

	glutSwapBuffers();


}


void disp(int value)
{
	glutPostRedisplay(); //call to mydisplay functoin
	glutTimerFunc(1, disp, 0);

}

void mouseCamera(int button, int state, int x, int y) {
	
}
void mouseGlobal(int button, int state, int x, int y) {
	switch (button) {
	  case GLUT_LEFT_BUTTON:
		  rot=0;
		  break;
	  case GLUT_RIGHT_BUTTON:
		  if(rot==0)
			  if(x>y)
				rot=0.01;
			  else rot=-0.01;
		  else (rot+=rot);
		  break;
	  case GLUT_MIDDLE_BUTTON:
		  break;
   }
}

void mouseMotion(int x, int y){
	ScaneState->mouseMotion(x,y);

}


void mouse(int button, int state, int x, int y) 
{
	ScaneState->mouse(button, state, x, y);

}

void switchState(){
	if(ScaneState == &CameraState){
		ScaneState = &GlobalState;
	}
	else{
		ScaneState = &CameraState;
	}
}

void processNormalKeys(unsigned char key, int x, int y)
{

	if(key==' '){
		switchState();
	}
}

void processSpecialKeys(int key, int xx, int yy){

}

int main(int  argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutCreateWindow("Ass 4");

	
	 
	init();
	initLight();
	ParseFile("doll.obj");
	glutDisplayFunc(mydisplay);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutKeyboardFunc(processNormalKeys);

	glutTimerFunc(2, disp, 1);
	
	glutMainLoop();
	return 0;
}