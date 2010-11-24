#include "MtlHandler.h"
#include <iostream>

using namespace std;

MtlHandler::MtlHandler() {
	newmtl = "";	
	for(int i = 0; i < number_of_colors; i++)
		Ka[i] = 0.0;
	for(int i = 0; i < number_of_colors; i++)
		Kd[i] = 0.0;
	for(int i = 0; i < number_of_colors; i++)
		Ks[i] = 0.0;
	Ns =  0.0;
	d = 0.0;
	illum = 0;
	map_Ka = "";
	map_Kd = "";
	map_Ks = "";
	map_d = "";
	map_bump = "";
}

MtlHandler::~MtlHandler() {

}