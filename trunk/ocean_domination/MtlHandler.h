#ifndef MTL_Handler_H
#define MTL_Handler_H

#include <string>
#include <iostream>

#define number_of_colors 3

using namespace std;

class MtlHandler {
public:
	MtlHandler();
	~MtlHandler();

	string newmtl;
	float Ka[number_of_colors];
	float Kd[number_of_colors];
	float Ks[number_of_colors];
	float Ns;
	float d;
	int illum;
	string map_Ka;
	string map_Kd;
	string map_Ks;
	string map_d;
	string map_bump;
};

#endif