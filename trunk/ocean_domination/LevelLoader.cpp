#include "LevelLoader.h"

LevelLoader::LevelLoader() {
	island_coordinates = new vector<Vertex>();
	level_files = new vector<string>();
}

LevelLoader::~LevelLoader() {
	level_files->clear();
	delete level_files;

	island_coordinates->clear();
	delete island_coordinates;
}

int LevelLoader::LoadLevels() {
	string levels;

	level_reader.open("levels.txt", ios::in);

	if(!level_reader) {
		cout << "could not read file: levels.txt" << endl;
		return 1;
	}

	//ignore the first line in the file which describes the file
	level_reader.ignore(1024, '\n');

	while(!level_reader.eof()) {
		//read the file name
		level_reader >> levels;

		level_files->push_back(levels);
	}

	level_reader.close();

	LoadLevel(level_files->at(0));

	return 0;
}

int LevelLoader::LoadLevel(string& level_file_name) {
	island_coordinates->clear();

	string island_type;
	int island_number = 0.0;
	Vertex island_location;

	level_reader.open(level_file_name.c_str(), ios::in);

	if(!level_reader) {
		cout << "could not read file: " << level_file_name << endl;
		return 1;
	}

	//ignore the first line in the file which describes the file
	level_reader.ignore(1024, '\n');

	while(!level_reader.eof()) {
		level_reader >> island_type;
		level_reader >> island_number;

		if(island_type.compare("large-island") == 0) {
			number_large_islands = island_number;
		} else if(island_type.compare("medium-island") == 0) {
			number_medium_islands = island_number;
		} else if(island_type.compare("small-island") == 0) {
			number_small_islands = island_number;
		} 

		for(int i = 0; i < island_number; i++) {
			level_reader >> island_location.x >> island_location.z;
			island_location.y = -3.5;

			island_coordinates->push_back(island_location);
		}
	}

	level_reader.close();

	return 0;
}