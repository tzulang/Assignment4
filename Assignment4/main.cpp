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

#include <glm/glm.hpp>

using namespace std;

static Vector3f zeroVec(0,0,0);

 const float LINEWIDTH=1;
 const float DELTA_CHANGE_OF_VIEW = 0.05;
 const float SCALE_FACTOR = 1.05;
 GLfloat rot;
 Scene scene;


GlobalMode GlobalState(scene);
CameraMode CameraState(scene);
State * ScaneState = &CameraState;

/*
	splits the string by a given delimiter charachter 

	s- the string to split
	delim- the delimiter charachter
	elems - the output vector of the splited strings
	ignoreEmpty - if true ,dont push empty parsed elements
*/
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


/*
	parse a vector given a splited line string 
	Assuming the first element of the vector is the identifier char ( i.e v , vn .etc)
*/
inline Vector3f parseVec(vector<string> &params){

	return Vector3f(stof(params[1]), stof(params[2]), stof(params[3]));
	 
}



/*
parse a face given a splited line string
Assuming the first element of the vector is the identifier char ( i.e f)
*/

inline Face parsFace(vector<string> &params){


	Face face;
	face.normal = new vector<int>();
	face.vertice = new vector<int>();
	for (int i = 0; i < params.size()-1; i++){
			
		int index = params[i + 1].find('/');
		if (index < 0)
			break;
		
		 
		
		face.vertice->push_back( stoi(params[i + 1].substr(0, index)));
		face.normal->push_back( stoi(params[i + 1].substr(index + 2)));

	}
	return face;
}



/*
parse an obj file given a string filename
exits on error
*/

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


/*
	inits the buffers, modelview and projection matrices
*/
void init()
{
 
	glClearColor(0, 0, 0, 1); //black background
	glReadBuffer(GL_BACK);


	glMatrixMode(GL_PROJECTION); /* switch matrix mode */
	glLoadIdentity();		//load Identity matrix
	
	//defines view mode
	scene.fieldOfViewAngle = 60;
	gluPerspective(scene.fieldOfViewAngle, 1, 2, 200);
	glTranslatef(0, 0, -100);
 
	
	glEnable(GL_DEPTH_TEST);  //define in which order the scene will built
	 

	/* return to modelview mode */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}


/*
*
*	set  the projection matrix field of view
*
*/
inline void setProjectionMatrix(){

	if (scene.fieldofViewChaned){

		glMatrixMode(GL_PROJECTION); /* switch matrix mode */
		glLoadIdentity();		//load Identity matrix
		printf("fov angle: %f\n", scene.fieldOfViewAngle);
		gluPerspective(scene.fieldOfViewAngle, 1, 2, 200);
		glTranslatef(0, 0, -100);
		glMatrixMode(GL_MODELVIEW);
		scene.fieldofViewChaned = false;
	}
}


/*
	set scale to the current matrix
*/
inline void setScale(){

 
		glScalef(scene.scaleFactor, scene.scaleFactor, scene.scaleFactor);
  
}


/*
	moves camera 
*/
inline void moveCamera(){
	if (scene.CameraLocDelta.x!=0||scene.CameraLocDelta.y!=0||scene.CameraLocDelta.z!=0){
		glm::vec4 forward(0.0,0.0,-1.0,0.0);
		glm::vec4 up(0.0,1.0,0.0,0.0);
		glm::vec4 right(1.0,0.0,0.0,0.0);
		glm::mat4 curMetrix;
		glGetFloatv (GL_MODELVIEW_MATRIX, &curMetrix[0][0]);
		glm::vec4 forwardFinal = forward* curMetrix;
		glm::vec4 upFinal = up* curMetrix;
		glm::vec4 rightFinal = right* curMetrix;
		glm::vec4 finalVec = forwardFinal*scene.CameraLocDelta.z + upFinal*scene.CameraLocDelta.y + rightFinal*scene.CameraLocDelta.x;
		glTranslatef(finalVec[0],
				finalVec[1],
				finalVec[2]);
		scene.CameraLocDelta = zeroVec;
	}

}


/*
rotates the camera
*/
inline void rotateCamera(){
	if (scene.CameraRotDelta.x!=0||scene.CameraRotDelta.y!=0){
		glm::vec4 sceneOrgLoc(0.0,0.0,-100.0,1.0);
		glm::vec4 up(0.0,1.0,0.0,0.0);
		glm::vec4 right(1.0,0.0,0.0,0.0);
		glm::mat4 curMetrix;
		glGetFloatv (GL_MODELVIEW_MATRIX, &curMetrix[0][0]);
		glm::vec4 sceneFixedLoc = sceneOrgLoc*curMetrix;
		glm::vec4 upFixed = up*curMetrix;
		glm::vec4 rightFixed = right*curMetrix;
		glTranslatef(sceneFixedLoc[0]*-1,
			sceneFixedLoc[1]*-1,
			sceneFixedLoc[2]*-1);

		glRotatef((scene.CameraRotDelta.x)*180,upFixed[0],upFixed[1],upFixed[2]);
		glRotatef((scene.CameraRotDelta.y)*180,rightFixed[0],rightFixed[1],rightFixed[2]);


		glTranslatef(sceneFixedLoc[0],
			sceneFixedLoc[1],
			sceneFixedLoc[2]);
		scene.CameraRotDelta.x =0;
		scene.CameraRotDelta.y = 0;
	}
}
/*
rotate the scene 
*/

inline void rotateScene(){
	if (scene.SceneRotDelta.x!=0||scene.SceneRotDelta.y!=0){
		glm::vec4 sceneOrgLoc(0.0,0.0,0.0,1.0);
		glm::mat4 curMetrix;
		glGetFloatv (GL_MODELVIEW_MATRIX, &curMetrix[0][0]);
		glm::vec4 sceneFixedLoc = sceneOrgLoc*curMetrix;
		glTranslatef(sceneFixedLoc[0]*-1,
			sceneFixedLoc[1]*-1,
			sceneFixedLoc[2]*-1);

		glRotatef((scene.SceneRotDelta.x)*180,0,1,0);
		glRotatef((scene.SceneRotDelta.y)*180,1,0,0);


		glTranslatef(sceneFixedLoc[0],
			sceneFixedLoc[1],
			sceneFixedLoc[2]);
		scene.SceneRotDelta.x =0;
		scene.SceneRotDelta.y = 0;
	}
}






inline void moveScene(){

	if (scene.SceneDelta.x!=0||scene.SceneDelta.y!=0||scene.SceneDelta.z!=0){
			glTranslatef(scene.SceneDelta.x,
				scene.SceneDelta.y,
				scene.SceneDelta.z);
		scene.SceneDelta = zeroVec;
	}
}

void initLight()
{
	//lightning

	//glEnable(GL_COLOR_MATERIAL);
	
	glShadeModel(GL_SMOOTH);
 
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glColorMaterial(GL_BACK, GL_AMBIENT_AND_DIFFUSE);
 
	GLfloat light_ambient[] =	{ 0.5,  0.5,  0.5, 1.0 }; //color
	GLfloat light_diffuse[] =	{ 0.0,  0.5,  0.5, 1.0 }; //color
	GLfloat light_specular[] =	{ 0.0,  0.0,  0.5, 1.0 };
	GLfloat light_position[] =	{ 0.0, -1.0, -1.0, 0.0 };
	 

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
 
	
	

	GLfloat mat_a[] = { 0.3, 0.4, 0.3, 1.0 };
	GLfloat mat_d[] = { 0.7, 0.6, 0.7, 1.0 };
	GLfloat mat_s[] = { 0.5, 0.0, 0.8, 1.0 };
	GLfloat low_sh[] = { 5 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, low_sh);



}


 

void drawAxisLines(){

	glLineWidth(LINEWIDTH);
	
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(100.0, 0.0, 0.0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 110.0, 0.0);

	glColor3f(0.0, 0.0, 100.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 100.0);


	glEnd();
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

			//glColor3f(0.3f, 0.5f, 0.13f);
			for (int f=0; f < facesNum; f++){
				
				Face *face = &faces->at(f);
				glBegin(GL_POLYGON);
				for (int i = 0; i < face->normal->size(); i++){
					int n = face->normal->at(i)-1;
					int v = face->vertice->at(i)-1;
					float f = float(i) / facesNum;
					//glColor3f(f, f*2, f);
					Vector3f nn= scene.normals[n];
					Vector3f vv= scene.vertices[v];
					glNormal3f(scene.normals[n].x, scene.normals[n].y, scene.normals[n].z);
					glVertex3f(scene.vertices[v].x, scene.vertices[v].y, scene.vertices[v].z);
				}
				glEnd();
			}
			
	
		}

	}

}

void mydisplay()
{
	glDrawBuffer(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear back buffer	

 
	//glRotatef(0.1, 0, 1,0 ); //rotate scene

	setProjectionMatrix();

	moveScene();
	moveCamera();
	rotateScene();
	rotateCamera();
	 


	glEnable(GL_LIGHTING);
	glPushMatrix();
	setScale();
	drawObj();
	glPopMatrix();
	glDisable(GL_LIGHTING);

	drawAxisLines();

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


void resetAllMatrices(){

	glMatrixMode(GL_PROJECTION); /* switch matrix mode */
	glLoadIdentity();		//load Identity matrix

	//defines view mode
	scene.fieldOfViewAngle = 60;
	gluPerspective(scene.fieldOfViewAngle, 1, 2, 200);
	glTranslatef(0, 0, -100);

	/* return to modelview mode */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	scene.scaleFactor = 1;
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
	switch (key) {
	case ' ':
		switchState();
		break;
	case 'r':
		resetAllMatrices();
		break;

	
	}
}

void processSpecialKeys(int key, int xx, int yy){
	switch (key) {
	 
		 

	case GLUT_KEY_F2:
	 
		if (scene.fieldOfViewAngle < 180){


			scene.fieldOfViewAngle += DELTA_CHANGE_OF_VIEW;
			scene.fieldofViewChaned = true;
		}
		 
		break;

	case GLUT_KEY_F3:
	 

		if (scene.fieldOfViewAngle > 0){


			scene.fieldOfViewAngle -= DELTA_CHANGE_OF_VIEW;
			scene.fieldofViewChaned = true;

		}
	 
		break;
	case GLUT_KEY_UP:
	
		scene.scaleFactor*= SCALE_FACTOR;
		printf("scale factor: %f\n", scene.scaleFactor);
		break;
	case GLUT_KEY_DOWN:
	 
		

		scene.scaleFactor/=  SCALE_FACTOR;
		printf("scale factor: %f\n", scene.scaleFactor);
		break;
	}
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
	glutSpecialFunc(processSpecialKeys);
	glutTimerFunc(2, disp, 1);
	
	glutMainLoop();
	return 0;
}