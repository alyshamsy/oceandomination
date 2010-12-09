#ifndef Island_H
#define Island_H

#include "Vertex.h"

#define small_island_radius 10.0f
#define medium_island_radius 15.0f
#define large_island_radius 25.0f

#define small_island_defence_radius 30.0f
#define medium_island_defence_radius 45.0f
#define large_island_defence_radius 75.0f

class Island {
public:
	Island();
	~Island();

	int InitializeIsland(Vertex location, char island_type);
	void UpdateHealth(int health);
	void UpdateUnderAttack( bool under_attack);
	void UpdateWeaponFire(bool weapon_fire);
	void UpdateWeaponRotationAngle(float weapon_rotation_angle);
	void UpdateAmmoLocation(Vertex island_ammo_location);

	void ResetAmmoLocation();
	void CreateHealthBar();

	Vertex getLocation() { return location; }
	Vertex getAmmoLocation() { return island_ammo_location; }
	char getIslandType() { return island_type; }
	bool doWeaponFire() { return weapon_fire; }
	float getIslandRadius() { return island_radius; }	
	float getDefenceRadius() { return defence_radius; }
	float getWeaponRotationAngle() { return weapon_rotation_angle; }

	bool isUnderAttack() { return under_attack; }
	int getHealth() { return health; }
	
private:
	Vertex location;
	Vertex island_ammo_location;
	char island_type;
	bool weapon_fire;
	float island_radius;
	float defence_radius;
	float weapon_rotation_angle;

	bool under_attack;
	int health;
};

#endif Island_H