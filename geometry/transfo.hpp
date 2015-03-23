/**
	Transfo - Matrices & Transformations
**/
#ifndef TRANSFO_HPP
#define	TRANSFO_HPP

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstring>
#include "types.hpp"

#define X 0
#define Y 1
#define Z 2

/** Classe Matrix **/
class Matrix {
public:
	/** Champs **/
	double mat[16]; /* organisée en column-major */
	
	/** Constructeur : initialisée à l'identité **/
	Matrix() {
		memset(mat,0,16*sizeof(double));
		mat[0] = mat[5] = mat[10] = mat[15] = 1;
	}
	
	/* Multiplication */
	Matrix operator * (const Matrix & m) const {
		// ici : res = this * m;
		Matrix res;
		for (int i=0; i<4; i++) { // col
			for (int j=0; j<4; j++) { // row
				res.mat[i*4+j] = mat[j]    * m.mat[i*4]  + 
				                 mat[j+4]  * m.mat[i*4+1]+ 
				                 mat[j+8]  * m.mat[i*4+2]+ 
				                 mat[j+12] * m.mat[i*4+3];
			}
		}
		return res;
	}
	
	/* Multiplication avec un Vecteur */
	Vector operator * (const Vector & v) const {
		double x, y, z;
		x = v.coord[0] * mat[0] + v.coord[1] * mat[4] + v.coord[2] * mat[8];
		y = v.coord[0] * mat[1] + v.coord[1] * mat[5] + v.coord[2] * mat[9];
		z = v.coord[0] * mat[2] + v.coord[1] * mat[6] + v.coord[2] * mat[10];
		Vector res(x, y, z);
		return res;
	}
	
	/* Multiplication avec un Point */
	Point operator * (const Point & p) const {
		double x, y, z;
		x = p.coord[0] * mat[0] + p.coord[1] * mat[4] + p.coord[2] * mat[8]  + mat[12];
		y = p.coord[0] * mat[1] + p.coord[1] * mat[5] + p.coord[2] * mat[9]  + mat[13];
		z = p.coord[0] * mat[2] + p.coord[1] * mat[6] + p.coord[2] * mat[10] + mat[14];
		Point res(x, y, z);
		return res;
	}

	/* Cast en tableau de double */
	operator double*() {
		return mat;
	}
	
	/* Ecrit la matrice sur le flot */
	friend std::ostream & operator << (std::ostream &, const Matrix &);
};


/** Matrice de transformation standard **/
class StandardMatrix: public Matrix {
public:
	/* Renvoit la matrice inverse */
	virtual Matrix makeInverse() const = 0;
	/* Renvoit la matrice de transport des normales */
	virtual Matrix makeNormalTransportMatrix() const = 0;
};


/** Matrice de Rotation **/
class RotationMatrix: public StandardMatrix {
public:
	const int raxe; /* Axe de rotation */
	const double rangle; /* Angle (radians) */
	
	/** Constructeur: axe(X,Y,Z) = {0, 1, 2} ; angle en radians **/
	RotationMatrix(const int & axe, const double & angle): raxe(axe), rangle(angle) {
		
		double sin = std::sin(angle);
		double cos = std::cos(angle);
		
		mat[0] = mat[5] = mat[10] =  cos;
		mat[1] = mat[2] = mat[6]  = -sin;
		mat[4] = mat[8] = mat[9]  =  sin;
		
		mat[axe*4] = mat[axe*4+1] = mat[axe*4+2] = 0;
		mat[axe]   = mat[axe+4]   = mat[axe+8]   = 0;
		
		mat[axe*5] = 1;
		
		mat[3]  = mat[7]  = mat[11] = mat[12] = mat[13] = mat[14] = 0;
		mat[15] = 1;
	}
	
	virtual Matrix makeInverse() const {
		RotationMatrix res(raxe, -rangle);
		return res;
	}
	
	virtual Matrix makeNormalTransportMatrix() const {
		return *this;
	}
};


/** Matrice de Translation **/
class TranslationMatrix: public StandardMatrix {
public:
	/** Constructeur: valeurs de translation pour chaque axe **/
	TranslationMatrix(const double & xt, const double & yt, const double & zt) {
		memset(mat,0,16*sizeof(double));
		mat[0] = mat[5] = mat[10] = mat[15] = 1;
		
		mat[12]  = xt;
		mat[13]  = yt;
		mat[14] = zt;
	}
	
	virtual Matrix makeInverse() const {
		TranslationMatrix res(mat[12] * -1, mat[13] * -1, mat[14] * -1);
		return res;
	}
	
	virtual Matrix makeNormalTransportMatrix() const {
		return *this;
	}
};


/** Matrice d'Homothétie **/
class ScaleMatrix: public StandardMatrix {
public:
	/** Constructeur: valeurs d'homothétie pour chaque axe **/
	ScaleMatrix(const double & xs, const double & ys, const double & zs) {
		memset(mat,0,16*sizeof(double));
		mat[15] = 1;
		mat[0]  = xs;
		mat[5]  = ys;
		mat[10] = zs;
	}
	

	virtual Matrix makeInverse() const {
		ScaleMatrix res(1. / mat[0], 1. / mat[5], 1. / mat[10]);
		return res;
	}
	
	virtual Matrix makeNormalTransportMatrix() const {
		ScaleMatrix res(mat[5]*mat[10], mat[0]*mat[10], mat[0]*mat[5]);
		return res;
	}
};

void transfo_test();

#endif	/* TRANSFO_HPP */
