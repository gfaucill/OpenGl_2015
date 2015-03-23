/**
	Types - Coord Point & Vector
**/
#ifndef TYPES_HPP
#define TYPES_HPP

#include <iostream>
#include <cmath>

#define SQR(a) ((a)*(a))

/** Classe Coordinate **/
class Coord {
public:
	/** Champs **/
	double coord[3];
	
	/** Constructeur **/
	Coord(const double & x=0., const double & y=0., const double & z=0.) {
		coord[0] = x;
		coord[1] = y;
		coord[2] = z;
	}
	
	/** Fonctions membres inlinées **/
	/* Addition */
	void operator += (const Coord & v) {
		coord[0] += v.coord[0];
		coord[1] += v.coord[1];
		coord[2] += v.coord[2];
	}
	
	/* Soustraction */
	void operator -= (const Coord & v) {
		coord[0] -= v.coord[0];
		coord[1] -= v.coord[1];
		coord[2] -= v.coord[2];	
	}
	
	/* Multiplication par un scalaire */
	void operator *= (const double & s) {
		coord[0] *= s;
		coord[1] *= s;
		coord[2] *= s;
	}
	
	/* Affecte les coordonnées */
	void set(const double & x, const double & y, const double & z) {
		coord[0] = x;
		coord[1] = y;
		coord[2] = z;
	}

	/* Cast en tableau de double */
	operator double*() {
		return coord;
	}
	
	/* Ecrit (x y z) sur le flot */
	friend std::ostream & operator << (std::ostream &, const Coord &);
};

/** Classe Point hérite de Coord **/
class Point : public Coord {
public:
	/** Constructeur **/
	Point(const double & x=0., const double & y=0., const double & z=0.) : Coord(x, y, z) {}
	
	/** Fonctions membres inlinées **/
	/* Carré de la distance avec un autre point */
	double sqrDist(const Point & p) const {
		return SQR(coord[0] - p.coord[0]) +
		       SQR(coord[1] - p.coord[1]) +
		       SQR(coord[2] - p.coord[2]);
	}
	
	/* Distance avec un autre point */
	double dist(const Point & p) const {
		return std::sqrt(sqrDist(p));
	}
};

/** Classe Vector hérite de Coord **/
class Vector : public Coord {
public:
	/** Constructeur **/
	Vector(const double & x=0., const double & y=0., const double & z=0.) : Coord(x, y, z) {}
	
	/* 2 Points -> Vecteur */
	Vector(const Point & A, const Point & B) : 
		Coord(B.coord[0] - A.coord[0],
		      B.coord[1] - A.coord[1], 
		      B.coord[2] - A.coord[2]) {}
	
	/** Fonctions membres inlinées **/
	/* Produit Scalaire */
	double operator * (const Vector & v) const {
		return coord[0] * v.coord[0] +
		       coord[1] * v.coord[1] +
		       coord[2] * v.coord[2];
	}
	
	/* Produit Vectoriel */
	Vector operator ^ (const Vector & v) const {
		Vector res (coord[1] * v.coord[2] - coord[2] * v.coord[1],
		            coord[2] * v.coord[0] - coord[0] * v.coord[2],
		            coord[0] * v.coord[1] - coord[1] * v.coord[0]);
		return res;
	}
	
	/* Carré de la norme */
	double sqrNorm() const {
		return coord[0] * coord[0] +
		       coord[1] * coord[1] +
		       coord[2] * coord[2];
	}
	
	/* Norme */
	double norm() const {
		return std::sqrt(sqrNorm());
	}
	
	/* Normalise le vecteur */
	void normalize() {
		*this *= 1.0 / this->norm();
	}
};

void types_test();

#endif /* TYPES_HPP */
