#ifndef Island_H
#define Island_H

#include "Vertex.h"

#define small_island_radius 10.0f
#define medium_island_radius 15.0f
#define large_island_radius 25.0f

class Island {
public:
	Island();
	~Island();

	int InitializeIsland(Vertex location, char island_type);
	void UpdateHealth(int health);
	void UpdateUnderAttack( bool under_attack);
	void UpdateWeaponFire(bool weapon_fire);
	
	void UpdateWeaponRotationAngleSide(float weapon_rotation_angle_side);
	void UpdateWeaponRotationAngleUp(float weapon_rotation_angle_up);

	void UpdateWeaponMovementAngleSide(float weapon_movement_angle_side);
	void UpdateWeaponMovementAngleUp(float weapon_movement_angle_up);

	Vertex getLocation() { return location; }
	char getIslandType() { return island_type; }
	bool doWeaponFire() { return weapon_fire; }
	float getIslandRadius() { return island_radius; }
	
	float getWeaponRotationAngleSide() { return weapon_rotation_angle_side; }
	float getWeaponRotationAngleUp() { return weapon_rotation_angle_up; }

	float getWeaponMovementAngleSide() { return weapon_movement_angle_side; }
	float getWeaponMovementAngleUp() { return weapon_movement_angle_up; }

	bool isUnderAttack() { return under_attack; }
	int getHealth() { return health; }
	
private:
	Vertex location;
	char island_type;
	bool weapon_fire;
	float island_radius;

	float weapon_rotation_angle_side;
	float weapon_rotation_angle_up;

	float weapon_movement_angle_side;
	float weapon_movement_angle_up;

	bool under_attack;
	int health;
};

#endif Island_H