#ifndef Ship_H
#define Ship_H

#include "Vector.h"

#define ship_radius 4.0f
#define missile_radius 0.5f
#define bullet_radius 0.1f
#define super_missile_radius 1.5f

class Ship {
public:
	Ship();
	~Ship();

	int InitializeShip(Vector& location);
	int UpdateShipLocation(Vector& location);
	int UpdateHealth(int& current_health);
	int UpdateScore(int& current_score);
	
	Vector getLocation() { return location; }
	int getHealth() { return health; }
	
	float getShipRadius() { return ship_radius; }
	float getMissileRadius() { return missile_radius; }
	float getBulletRadius() { return bullet_radius; }
	float getSuperMissileRadius() { return super_missile_radius; }
	int getCurrentScore() { return score; }

	struct Ammo {
		int missiles;
		int super_missiles;
		int sniper_bullets;
	} player_ammo;

private:
	Vector location;
	int health; 
	int score;
};

#endif Island_H