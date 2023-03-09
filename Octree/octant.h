#pragma once
#include <vector>
#include "point.h"

using namespace std;

struct Octant{
	vector<Point> points;
	double range[3][2]; 
	double mid[3];
	Octant* childs[8];

	Octant(double x_min, double x_max,double y_min,double y_max,double z_min,double z_max){
		for (int i = 0; i < 8; ++i) childs[i] = NULL;
		range[0][0] = x_min; range[0][1] = x_max;
		range[1][0] = y_min; range[1][1] = y_max;
		range[2][0] = z_min; range[2][1] = z_max;
		mid[0] = x_min + ((x_max- x_min)/2); mid[1] = y_min + ((y_max - y_min) / 2); mid[2] = z_min + ((z_max - z_min) / 2);
	}

	bool found(Point x) {
		vector<Point>::iterator it = find(points.begin(), points.end(), x);
		if (it != points.end()) {
			return 1;
		}
		return 0;
	}

	bool found(Point x, vector<Point>::iterator & it){
		it = find(points.begin(), points.end(), x);
		if (it != points.end()) {
			return 1;
		}
		return 0;
	}

	bool insert(Point x) {
		if (found(x)) { return 0; }
		points.push_back(x);
		return 1;
	}

	bool del(Point x) {
		vector<Point>::iterator it;
		if (!found(x)) { return 0; }
		points.erase(it);
		return 1;
	}

};
