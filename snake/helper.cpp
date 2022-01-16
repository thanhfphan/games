#include "helper.hpp"

float min(float num1, float num2){
	if (num1 > num2){
		return num2;
	}

	return num1;
}

float max(float num1, float num2){
	if (num1 > num2){
		return num1;
	}

	return num2;
}

bool line_intersect(float x1, float y1, float x2, float y2)
{
	return min(x1, y1) < max(x2, y2) && max(x1,y1) > min(x2, y2);
}

bool rectangle_intersect(SDL_Rect r1, SDL_Rect r2)
 {
	return line_intersect((float)r1.x, (float)(r1.x + r1.w), (float)r2.x, (float)(r2.x + r2.w)) && line_intersect(r1.y, r1.y + r1.h, r2.y, r2.y + r2.h);
}