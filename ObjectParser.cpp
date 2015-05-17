#include "ObjectParser.h"
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

#include "Vector3f.h"




ObjectParser::ObjectParser()
{
}


ObjectParser::~ObjectParser()
{
}


vector<string> &nsplit(const string &s, char delim, std::vector<string> &elems, bool ignoreEmpty) {

	stringstream ss(s);
	string item;

	while (std::getline(ss, item, delim)) {
		if (ignoreEmpty&&item.empty())
			continue;
		elems.push_back(item);
	}
	return elems;
}

void ObjectParser::ParseFile(string fileName, vector<Vector3f> vertices, vector<Vector3f> normals){

	std::ifstream file;
	file.open(fileName, ios::in);
	string line;


	if (!file.is_open()){
		printf("error cannot open file %s\n", fileName);
		return;
	}

	while (std::getline(file, line))
	{
		switch (line[0])
		{
		case 'o':
			break;

		case 'g':
			break;

		case 'f':
			break;

		case 'v':
			break;

		case 'vn':
			break;
		default:
			continue;
			break;
		}
		
	
	}

	file.close();

}

