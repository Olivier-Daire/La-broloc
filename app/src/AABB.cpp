#include "AABB.hpp"
#include <iostream>
#include <math.h>

AABB::AABB() : x(0),y(0),z(0),w(0),h(0),d(0) {

}

AABB::AABB(float x, float y, float z, float w, float h, float d)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
	this->h = h;
	this->d = d;
}

bool AABB::collision(float x,float y,float z)
{
	if (x >= this->x && x < this->x + this->w && y >= this->y && y < this->y + this->h && z >= this->z && z < this->z + this->d) {
		return true;
	}

	else {
		return false;
	}
}

bool AABB::collision(AABB box2)
{
	if((box2.x - box2.w >= x + w/2.0) // trop à droite

		|| (box2.x + box2.w <= x - w/2.0) // trop à gauche

		|| (box2.y - box2.h >= y + h) // trop en bas

		|| (box2.y + box2.h <= y ) // trop en haut

		|| (box2.z - box2.d >= z + d/2.0) // trop derrière

		|| (box2.z + box2.d <= z - d/2.0)) // trop devant
			return false;
		
	else{
		return true;
	}
}