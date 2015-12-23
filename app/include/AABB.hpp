#pragma once

class AABB 
{
	public:
		AABB();
		AABB(float x, float y, float z, float w, float h, float d);
		bool collision(float x,float y,float z);
		bool collision(AABB box2);

		float x,y,z;
		float w,h,d; // witdh, height, depth
		bool firstT = true, firstS = true;


};