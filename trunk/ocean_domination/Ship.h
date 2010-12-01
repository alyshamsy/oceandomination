#ifndef Ship_H
#define Ship_H

#include "Vertex.h"

class Ship {
public:
	Ship();
	~Ship();

	int InitializeShip(Vertex location);
	int UpdateShipLocation(Vertex location);
	int UpdateShip(int current_health, int current_missiles, int current_super_missiles, int current_sniper_bullets);
	
	Vertex getLocation() { return location; }
	int getHealth() { return health; }
	
	struct Ammo {
		int missiles;
		int super_missilies;
		int sniper_bullets;
	} player_ammo;

private:
	Vertex location;
	int health; 
};

#endif Island_H