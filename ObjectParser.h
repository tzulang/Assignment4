#pragma once

#include <sstream>
#include <fstream>
#include <vector>
#include <string>

#include "Vector3f.h"

using namespace std;

class ObjectParser
{
public:
	ObjectParser();
	~ObjectParser();

	void ParseFile(string fileName, vector<Vector3f> vertices, vector<Vector3f> normals);
};

