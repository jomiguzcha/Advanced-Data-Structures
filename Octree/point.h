#pragma once

struct Point {
	double x = 0,y = 0,z = 0;

	Point(){}

	Point(double _x, double _y, double _z) {
		x = _x; y = _y; z = _z;
	}

	bool operator==(Point a){
		return (x == a.x && y == a.y && z == a.z) ? 1 : 0;
	}
};
