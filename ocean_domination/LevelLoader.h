#ifndef Level_Loader_H
#define Level_Loader_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "Vertex.h"

using namespace std;

class LevelLoader {
public:
	LevelLoader();
	~LevelLoader();

	int LoadLevels();
	
	vector<string>* getLevelFileNames() { return level_files; }
	vector<Vertex>* getIslandCoordiantes() { return island_coordinates; }
	int getLargeIslandNumbers() { return number_large_islands; }
	int getMediumIslandNumbers() { return number_medium_islands; }
	int getSmallIslandNumbers() { return number_small_islands; }

private:
	int LoadLevel(string& level_file_name);

	fstream level_reader;
	vector<string>* level_files;
	vector<Vertex>* island_coordinates;
	
	int number_large_islands;
	int number_medium_islands;
	int number_small_islands;
};

#endif