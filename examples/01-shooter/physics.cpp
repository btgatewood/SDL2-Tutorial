#include "main.h"


bool collision_point(const Entity& e, float x, float y)
{
	return (x > e.x && x < (e.x + e.w) &&
			y > e.y && y < (e.y + e.h));
}


bool collision_rect(const Entity& e1, const Entity& e2)
{
	return (std::max(e1.x, e2.x) < std::min(e1.x + e1.w, e2.x + e2.w) &&
			std::max(e1.y, e2.y) < std::min(e1.y + e1.h, e2.y + e2.h));
}


void calculate_slope(int x1, int y1, int x2, int y2, float& dx, float& dy)
{
	int steps = std::max(std::abs(x1 - x2), std::abs(y1 - y2));

	if (steps != 0)  // avoid division by zero
	{
		dx = (x1 - x2) / static_cast<float>(steps);
		dy = (y1 - y2) / static_cast<float>(steps);
	}
	else  // x1,y1 == x2,y2
	{
		dx = 0;
		dy = 0;
	}
}
