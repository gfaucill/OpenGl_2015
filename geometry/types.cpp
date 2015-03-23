#include <iostream>

#include "types.hpp"

using namespace std;

ostream & operator << (ostream & out, const Coord & c) {
	out << '(' << c.coord[0]
	    << ' ' << c.coord[1]
	    << ' ' << c.coord[2]
	    << ')';
	return out;
}

void types_test() {
	Vector v(2.0, 3.5, 7.123);
	Point p(1., 1., 1.);
	
	v *= 5;
	p += v;
	
	cout << v << endl;
	cout << p << endl;
	
	v.normalize();
	cout << v << endl;
	
	Point a(0., 0., 0.), b(1., 0., 0.), c(0., 1., 0.);
	v = Vector(a, b) ^ Vector(a, c);
	cout << v << endl;
}
