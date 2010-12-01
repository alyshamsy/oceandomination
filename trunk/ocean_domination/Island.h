#ifndef Island_H
#define Island_H

#include "Vertex.h"

class Island {
public:
	Island();
	~Island();

	int InitializeIsland(Vertex location, char island_type, int island_number);
	int UpdateIsland(bool under_attack, int health);
	
	Vertex getLocation() { return location; }
	char getIslandType() { return island_type; }
	int getIslandNumber() { return island_number; }

	bool isUnderAttack() { return under_attack; }
	int getHealth() { return health; }
	
private:
	Vertex location;
	char island_type;
	int island_number;

	bool under_attack;
	int health;
};

#endif Island_H