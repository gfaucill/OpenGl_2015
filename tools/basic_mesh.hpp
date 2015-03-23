/***************************************************************************
                        basic_mesh.hpp  -  description
                             -------------------
    copyright            : (C) 2012 by Biri
    email                : biri@univ-mlv.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _STP3D_BASIC_MESH_HPP_
#define _STP3D_BASIC_MESH_HPP_


#include <iostream>
#include <vector>
#include <cmath>
#include "mesh.hpp"
#include "indexed_mesh.hpp"
#include <math.h>

namespace STP3D {

/** BASIC MESH CREATION TOOLS
 * All these functions create mesh
 * Data are stored in buffers but VBO must be build by the application.
 * Index chosen for each buffer follow this convention.
 * Index 0 : coordinates (mandatory)
 * Index 1 : normals
 * Index 2 : texture coordinates
 * Index 3 : colors
 */

/** Frame creation
 * This is a basic frame visualization. It shows the 3 axis with red,green,blue colors.
 * \param lg is the length of each axis
 */
StandardMesh* createRepere(float lg=1.0f);

/** Create a basic rectangle on the xz plane
 * This function create a mesh (2 triangles) for a basic quad set on the (xz)-plane.
 * The mesh stores only the coordinates, leaving all material properties to the application
 * The position and size of the rectangle is (0,0) to (sizex,sizez).
 * \param sizex,sizez size of the rectangle along x and z planes
 */
StandardMesh* basicRect(float sizex,float sizez);

/** Create a basic cone on the y axis
 * This function create a mesh for a basic cone along the y axis.
 * The mesh stores only the coordinates, leaving all material properties to the application
 * The top of the cone is at height \a h.
 * The problem reside in the fact that each triangle is dependant of the following one
 * (due to the use of GL_TRIANGLE_STRIP) therefore we see some inconsistency in the illumination
 * \param h height of the cone
 * \param radius radius of the cone
 * \param nb_div number of division (in triangles) for the cone
 */
StandardMesh* basicCone(float h,float radius,uint nb_div = 64);

/** Create a basic cylinder on the y axis
 * This function create a mesh for a basic cylinder along the y axis without top and bottom closure.
 * The mesh stores only the coordinates, leaving all material properties to the application
 * The height of the cylinder is \a h.
 * \param h height of the cylinder
 * \param radius radius of the cylinder
 * \param div_round number of division for the cylinder
 * \param div_height number of division for the cylinder
 */
IndexedMesh* basicCylinder(float h,float radius,uint div_round = 64,uint div_height = 1);

/** Create a basic cube surounding the origine
 * This function create a cube centered at the origine with a specific size
 * The mesh stores only the coordinates, leaving all material properties to the application
 * The size of one edge is \a width.
 * \param width size of one edge, therefore along an axe, the cube is located at -size/2 to size/2
 */
IndexedMesh* basicCube(float width = 1.0f);

inline StandardMesh* createRepere(float lg) {
	StandardMesh* repere = new StandardMesh(6,GL_LINES);
	float coord[18] = {0.0,0.0,0.0,lg,0.0,0.0,
			0.0,0.0,0.0,0.0,lg,0.0,
			0.0,0.0,0.0,0.0,0.0,lg};
	float color[18] = {1.0,0.0,0.0,1.0,0.0,0.0,
			0.0,1.0,0.0,0.0,1.0,0.0,
			0.0,0.0,1.0,0.0,0.0,1.0};
	repere->addOneBuffer(0,3,coord,"coordinates",true);
	repere->addOneBuffer(3,3,color,"colors",true);
	return repere;
}

inline StandardMesh* basicRect(float sizex,float sizez) {
	StandardMesh* rect = new StandardMesh(4,GL_TRIANGLE_STRIP);
	float coord[12] = {0.0,0.0,0.0,
			0.0,0.0,sizez,
			sizex,0.0,0.0,
			sizex,0.0,sizez};
	float normals[12] = {0.0,1.0,0.0,0.0,1.0,0.0,
			0.0,1.0,0.0,0.0,1.0,0.0};
	rect->addOneBuffer(0,3,coord,"coordinates",true);
	rect->addOneBuffer(1,3,normals,"normals",true);
	return rect;
}

inline StandardMesh* basicCone(float h,float radius,uint nb_div) {
	StandardMesh* cone = new StandardMesh((nb_div+1)*2,GL_TRIANGLE_STRIP);
	float* coord = new float[(nb_div+1)*2*3];
	float* normals = new float[(nb_div+1)*2*3];
	float* uv = new float[(nb_div+1)*2*2];
	double cos_pt,sin_pt;
	double angle=0;
	double angle_to_plane_xz = atan(h/radius);
	double cos_to_xz = cos(angle_to_plane_xz);
	double sin_to_xz = sin(angle_to_plane_xz);
	for(uint i=0;i<=nb_div;i++,angle+=2*M_PI/nb_div) {
		// Head of the cone (repeated)
		cos_pt = cos(angle);
		sin_pt = sin(angle);
		coord[6*i  ] = radius*cos_pt; coord[6*i+1] = 0.0; coord[6*i+2] = radius*sin_pt;
		coord[6*i+3] = 0.0;           coord[6*i+4] = h;   coord[6*i+5] = 0.0;
		normals[6*i  ] = cos_pt*sin_to_xz; normals[6*i+1] = cos_to_xz; normals[6*i+2] = sin_pt*cos_to_xz;
		normals[6*i+3] = cos_pt*sin_to_xz; normals[6*i+4] = cos_to_xz; normals[6*i+5] = sin_pt*cos_to_xz;
		uv[4*i  ] = (float)i/nb_div; uv[4*i+1] = 0.0;
		uv[4*i+2] = (float)i/nb_div; uv[4*i+3] = 1.0;
	}
	cone->addOneBuffer(0,3,coord,"coordinates",false);
	cone->addOneBuffer(1,3,normals,"normals",false);
	cone->addOneBuffer(2,2,uv,"uvs",false);
	return cone;
}

inline IndexedMesh* basicCylinder(float h,float radius,uint div_round,uint div_height) {
	uint nb_points = (div_round+1)*(div_height+1);
	uint nb_prim = 2*div_round*div_height;
	IndexedMesh* cyl = new IndexedMesh(nb_prim,nb_points,GL_TRIANGLES);
	float* coord = new float[nb_points*3];
	float* normals = new float[nb_points*3];
	float* uv = new float[nb_points*2];
	uint* indexes = new uint[3*nb_prim];
	double cos_pt,sin_pt;
	double angle = 0.0;
	double height = 0.0;
	double incr_angle = 2*M_PI/div_round;
	// Calcul des points
	for(uint j=0;j<=div_height;j++,height+=h/div_height) {
		for(uint i=0;i<=div_round;i++,angle+=incr_angle) {
			cos_pt = cos(angle);
			sin_pt = sin(-angle);
			coord[3*j*(div_round+1)+3*i  ] = radius*cos_pt;
			coord[3*j*(div_round+1)+3*i+1] = height;
			coord[3*j*(div_round+1)+3*i+2] = radius*sin_pt;
			normals[3*j*(div_round+1)+3*i  ] = cos_pt;
			normals[3*j*(div_round+1)+3*i+1] = 0.0;
			normals[3*j*(div_round+1)+3*i+2] = sin_pt;
			uv[2*j*(div_round+1)+2*i  ] = i/(float)div_round;
			uv[2*j*(div_round+1)+2*i+1] = j/(float)div_height;
		}
	}

	// Computation of indexes
	for(uint  j=0;j<div_height;j++) {
		for(uint i=0;i<div_round;i++) {
			indexes[j*div_round*6+6*i+0] = j*(div_round+1)+i;         // A
			indexes[j*div_round*6+6*i+1] = j*(div_round+1)+(i+1);     // B
			indexes[j*div_round*6+6*i+2] = (j+1)*(div_round+1)+i;     // C
			indexes[j*div_round*6+6*i+3] = (j+1)*(div_round+1)+i;     // C
			indexes[j*div_round*6+6*i+4] = j*(div_round+1)+(i+1);     // B
			indexes[j*div_round*6+6*i+5] = (j+1)*(div_round+1)+(i+1); // D
		}
	}

	cyl->addOneBuffer(0,3,coord,"coordinates",false);
	cyl->addOneBuffer(1,3,normals,"normals",false);
	cyl->addOneBuffer(2,2,uv,"uvs",false);
	cyl->addIndexBuffer(indexes,false);
	return cyl;
}

inline IndexedMesh* basicPlan( uint n = 64) {
	uint nb_points = (n+1) * (n+1);
	uint nb_prim = n*n*2;
	IndexedMesh* plan = new IndexedMesh(nb_prim,nb_points,GL_TRIANGLES);
	float* coord = new float[nb_points*3];
	float* normals = new float[nb_points*3];
	float* uv = new float[nb_points*2];
	uint* indexes = new uint[3*nb_prim];

	double div_X = (4.) / n;
	double div_Y = (2.) / n;

	// Calcul des points
	for(uint j=0;j<=n;j++) {
		for(uint i=0;i<=n;i++) {

			coord[3*j*(n+1)+3*i  ] = div_X * j - 2;
			coord[3*j*(n+1)+3*i+1] = 0;
			coord[3*j*(n+1)+3*i+2] = div_Y * i - 1;

			normals[3*j*(n+1)+3*i  ] = 0.;
			normals[3*j*(n+1)+3*i+1] = 1.;
			normals[3*j*(n+1)+3*i+2] = 0.;

			uv[2*j*(n+1)+2*i  ] = j/(float)n;
			uv[2*j*(n+1)+2*i+1] = i/(float)n;
		}
	}

	// Computation of indexes
	for(uint  j=0; j<n; j++) {
		for(uint i=0; i<n; i++) {
			indexes[j*n*6+6*i+0] = j*(n+1)+i;         // A
			indexes[j*n*6+6*i+1] = j*(n+1)+(i+1);     // B
			indexes[j*n*6+6*i+2] = (j+1)*(n+1)+i;     // C

			indexes[j*n*6+6*i+3] = (j+1)*(n+1)+i;     // C
			indexes[j*n*6+6*i+4] = j*(n+1)+(i+1);     // B
			indexes[j*n*6+6*i+5] = (j+1)*(n+1)+(i+1); // D
		}
	}

	plan->addOneBuffer(0,3,coord,"coordinates",false);
	plan->addOneBuffer(1,3,normals,"normals",false);
	plan->addOneBuffer(2,2,uv,"uvs",false);
	plan->addIndexBuffer(indexes,false);

	return plan;
}



inline IndexedMesh* basicCube(float width) {
	uint nb_points = 24;
	uint nb_prim = 12;
	IndexedMesh* cube = new IndexedMesh(nb_prim,nb_points,GL_TRIANGLES);
	float coord[3*24] = {
			// Face front (z = width/2) (A,B,C,D)
			-width/2.0f,-width/2.0f,width/2.0f,         // A
			width/2.0f,-width/2.0f,width/2.0f,          // B
			width/2.0f,width/2.0f,width/2.0f,           // C
			-width/2.0f,width/2.0f,width/2.0f,          // D
			// Face back (z = -width/2) (E,F,G,H)
			-width/2.0f,-width/2.0f,-width/2.0f,        // E
			-width/2.0f,width/2.0f,-width/2.0f,         // H
			width/2.0f,width/2.0f,-width/2.0f,          // G
			width/2.0f,-width/2.0f,-width/2.0f,         // F
			// Face left (x = -width/2) (E,A,D,H)
			-width/2.0f,-width/2.0f,-width/2.0f,        // E
			-width/2.0f,-width/2.0f,width/2.0f,         // A
			-width/2.0f,width/2.0f,width/2.0f,          // D
			-width/2.0f,width/2.0f,-width/2.0f,         // H
			// Face right (x = +width/2) (B,F,G,C)
			width/2.0f,-width/2.0f,width/2.0f,          // B
			width/2.0f,-width/2.0f,-width/2.0f,         // F
			width/2.0f,width/2.0f,-width/2.0f,			// G
			width/2.0f,width/2.0f,width/2.0f,			// C
			// Face top (y = +width/2) (D,C,G,H)
			-width/2.0f,width/2.0f,width/2.0f,          // D
			width/2.0f,width/2.0f,width/2.0f,           // C
			width/2.0f,width/2.0f,-width/2.0f,          // G
			-width/2.0f,width/2.0f,-width/2.0f,         // H
			// Face down (y = -width/2) (A,B,F,E)
			-width/2.0f,-width/2.0f,width/2.0f,         // A
			-width/2.0f,-width/2.0f,-width/2.0f,        // E
			width/2.0f,-width/2.0f,-width/2.0f,         // F
			width/2.0f,-width/2.0f,width/2.0f           // B
	};
	float normals[3*24] = {
			// Face front (z = width/2) (A,B,C,D)
			0.0f,0.0f,1.0f,		0.0f,0.0f,1.0f,		0.0f,0.0f,1.0f,		0.0f,0.0f,1.0f,
			// Face back (z = -width/2) (E,H,G,F)
			0.0f,0.0f,-1.0f,	0.0f,0.0f,-1.0f,	0.0f,0.0f,-1.0f,	0.0f,0.0f,-1.0f,
			// Face left (x = -width/2) (E,A,D,H)
			-1.0,0.0,0.0,		-1.0,0.0,0.0,		-1.0,0.0,0.0,		-1.0,0.0,0.0,
			// Face right (x = +width/2) (B,F,G,C)
			1.0,0.0,0.0,		1.0,0.0,0.0,		1.0,0.0,0.0,		1.0,0.0,0.0,
			// Face top (y = +width/2) (D,C,G,H)
			0.0,1.0,0.0,		0.0,1.0,0.0,		0.0,1.0,0.0,		0.0,1.0,0.0,
			// Face down (y = -width/2) (A,B,F,E)
			0.0,-1.0,0.0,		0.0,-1.0,0.0,		0.0,-1.0,0.0,		0.0,-1.0,0.0
	};
	float uv[2*24] = {
			// Face front (z = width/2) (A,B,C,D)
			0.0,0.0,		1.0,0.0,		1.0,1.0,		0.0,1.0,
			// Face back (z = -width/2) (E,F,G,H)
			1.0,0.0,		0.0,0.0,		0.0,1.0,		1.0,1.0,
			// Face left (x = -width/2) (E,A,D,H)
			1.0,0.0,		0.0,0.0,		0.0,1.0,		1.0,1.0,
			// Face right (x = +width/2) (B,F,G,C)
			1.0,0.0,		0.0,0.0,		0.0,1.0,		1.0,1.0,
			// Face top (y = +width/2) (D,C,G,H)
			0.0,1.0,		1.0,1.0,		1.0,0.0,		0.0,0.0,
			// Face down (y = -width/2) (A,E,F,B)
			0.0,0.0,		1.0,0.0,		1.0,1.0,		0.0,1.0
	};
	uint indexes[3*12] = {
			// Face front (z = width/2) (A,B,C,D)
			0,1,2,		0,2,3,
			// Face back (z = -width/2) (E,F,G,H)
			4,5,6,		4,6,7,
			// Face left (x = -width/2) (E,A,D,H)
			8,9,10,		8,10,11,
			// Face right (x = +width/2) (B,F,G,C)
			12,13,14,		12,14,15,
			// Face top (y = +width/2) (D,C,G,H)
			16,17,18,		16,18,19,
			// Face down (y = -width/2) (A,E,F,B)
			20,21,22,		20,22,23
	};

	cube->addOneBuffer(0,3,coord,"coordinates",true);
	cube->addOneBuffer(1,3,normals,(char*)"normals",true);
	cube->addOneBuffer(2,2,uv,std::string("uvs"),true);
	cube->addIndexBuffer(indexes,true);
	return cube;
}

//////////////////////////////////////////////////////////////////
///         Mesh Mesh
//////////////////////////////////////////////////////////////////

/**
 * @brief meshSphere Create a mesh sphere of raduis 1 positioned on the center of the world (0,0,0)
 * @param div_round round mesh division
 * @param div_height height mesh division
 * @return an IndexedMesh
 */
IndexedMesh* meshSphere(uint div_round, uint div_height);

/**
 * @brief meshCylinder Create a mesh cylinder of raduis 1 positioned on the center of the world (0,0,0) closed on the top and the bottom
 * @param div_round round mesh division
 * @param div_height mesh division
 * @return an IndexedMesh
 */
IndexedMesh* meshCylinder(uint div_round, uint div_height);

/**
 * @brief meshCone Create a mesh Cone of raduis 1 positioned on the center of the world (0,0,0) closed on the bottom
 * @param div_round round mesh division
 * @param div_height height mesh division
 * @param height height of the cone 1 by default
 * @return an IndexedMesh
 */
IndexedMesh* meshCone(uint div_round, uint div_height, uint height=1);

/**
 * @brief meshMobius Create a kind of mobius ruban in 3D
 * @param d1 litle diametre
 * @param div_round round mesh division
 * @param div_height height mesh division
 * @return an IndexedMesh
 */
IndexedMesh* meshMobius(uint div_round, uint div_height, float d1);

/**
 * @brief meshSquareDoughnut Create a squareDoughnut positioned on the center of the world (0,0,0)
 * @param div_round round mesh division
 * @param div_height height mesh division
 * @return an IndexedMesh
 */
IndexedMesh* meshSquareDoughnut(uint div_round, uint div_height);

/**
 * @brief meshTore Create a tore positioned on the center of the world (0,0,0)
 * @param div_round round mesh division
 * @param div_height height mesh division
 * @param d1 litle diametre
 * @return an IndexedMesh
 */
IndexedMesh* meshTore(uint div_round, uint div_height, float d1);

/**
 * @brief meshCube Create a Cube positioned on the center of the world (0,0,0) with edge of 2 (2*2*2)
 * @param div_round round mesh division
 * @param div_height height mesh division
 * @return an IndexedMesh
 */
IndexedMesh * meshCube(uint div_round, uint div_height);


inline IndexedMesh* meshSphere(uint div_round, uint div_height) {
	uint nb_points = (div_round+1)*(div_height+1) + 1;
	uint nb_prim = 2*div_round*div_height + div_round;
	IndexedMesh* sphere = new IndexedMesh(nb_prim,nb_points,GL_TRIANGLES);
	float* coord = new float[nb_points*3];
	float* normals = new float[nb_points*3];
	float* uv = new float[nb_points*2];
	uint* indexes = new uint[3*nb_prim];
	float alpha = (2*M_PI)/ div_round, phi = M_PI/(div_height+1);

	for(uint j=0;j<=div_height;j++) {
		for(uint i=0;i<=div_round;i++) {
			coord[3*j*(div_round+1)+3*i  ] = cos(i*alpha)*sin(j*phi);
			coord[3*j*(div_round+1)+3*i+1] = sin(i*alpha)*sin(j*phi);
			coord[3*j*(div_round+1)+3*i+2] = cos(j*phi);
			normals[3*j*(div_round+1)+3*i  ] = cos(i*alpha)*sin(j*phi);
			normals[3*j*(div_round+1)+3*i+1] = sin(i*alpha)*sin(j*phi);
			normals[3*j*(div_round+1)+3*i+2] = cos(j*phi);
			uv[2*j*(div_round+1)+2*i  ] = i/(float)div_round;
			uv[2*j*(div_round+1)+2*i+1] = j/(float)div_height;
		}
	}
	coord[nb_points*3-3] = 0.;
	coord[nb_points*3-2] = 0.;
	coord[nb_points*3-1] = -1.;
	normals[nb_points*3-3] = 0.;
	normals[nb_points*3-2] = 0.;
	normals[nb_points*3-1] = -1.;
	uv[2*nb_points-2] = 1;
	uv[2*nb_points-1] = 1;

	for(uint  j=0;j<div_height;j++) {
		for(uint i=0;i<div_round ;i++) {
			indexes[j*div_round*6+6*i+0] = j*(div_round+1)+i;         // A
			indexes[j*div_round*6+6*i+1] = j*(div_round+1)+(i+1);     // B
			indexes[j*div_round*6+6*i+2] = (j+1)*(div_round+1)+i;     // C
			indexes[j*div_round*6+6*i+3] = (j+1)*(div_round+1)+i;     // C
			indexes[j*div_round*6+6*i+4] = j*(div_round+1)+(i+1);     // B
			indexes[j*div_round*6+6*i+5] = (j+1)*(div_round+1)+(i+1); // D
		}
	}

	uint j = div_height;
	for(uint i=0;i<div_round ;i++) {
		indexes[j*div_round*6+3*i+0] = nb_points - 1;
		indexes[j*div_round*6+3*i+1] = j*(div_round+1)+i;
		indexes[j*div_round*6+3*i+2] = j*(div_round+1)+(i+1);
	}

	sphere->addOneBuffer(0,3,coord,"coordinates",false);
	sphere->addOneBuffer(1,3,normals,"normals",false);
	sphere->addOneBuffer(2,2,uv,"uvs",false);
	sphere->addIndexBuffer(indexes,false);

	return sphere;
}

inline IndexedMesh* meshCylinder(uint div_round, uint div_height) {
	uint nb_points = (div_round+1)*(div_height+1) + 2;
	uint nb_prim = 2*div_round*div_height + 2*div_round;
	IndexedMesh* cylinder = new IndexedMesh(nb_prim,nb_points,GL_TRIANGLES);
	float* coord = new float[nb_points*3];
	float* normals = new float[nb_points*3];
	float* uv = new float[nb_points*2];
	uint* indexes = new uint[3*nb_prim];
	float alpha = (2*M_PI)/ div_round, phi = M_PI/(div_height+1);

	for(uint j=0;j<=div_height;j++) {
		for(uint i=0;i<=div_round;i++) {
			coord[3*j*(div_round+1)+3*i  ] = normals[3*j*(div_round+1)+3*i  ] = cos(i*alpha);
			coord[3*j*(div_round+1)+3*i+1] = normals[3*j*(div_round+1)+3*i+1] = sin(i*alpha);
			coord[3*j*(div_round+1)+3*i+2] = cos(j*phi);
			normals[3*j*(div_round+1)+3*i+2] = 0;
			uv[2*j*(div_round+1)+2*i  ] = i/(float)div_round;
			uv[2*j*(div_round+1)+2*i+1] = j/(float)div_height;
		}
	}
	coord[nb_points*3-3] = 0.;
	coord[nb_points*3-2] = 0.;
	coord[nb_points*3-1] = -1.;
	normals[nb_points*3-3] = 0.;
	normals[nb_points*3-2] = 0.;
	normals[nb_points*3-1] = -1.;
	uv[2*nb_points-2] = 1;
	uv[2*nb_points-1] = 1;

	coord[nb_points*3-6] = 0.;
	coord[nb_points*3-5] = 0.;
	coord[nb_points*3-4] = 1.;
	normals[nb_points*3-6] = 0.;
	normals[nb_points*3-5] = 0.;
	normals[nb_points*3-4] = 1.;
	uv[2*nb_points-4] = 0;
	uv[2*nb_points-3] = 0;

	for(uint  j=0;j<div_height;j++) {
		for(uint i=0;i<div_round ;i++) {
			indexes[j*div_round*6+6*i+0] = j*(div_round+1)+i;         // A
			indexes[j*div_round*6+6*i+1] = j*(div_round+1)+(i+1);     // B
			indexes[j*div_round*6+6*i+2] = (j+1)*(div_round+1)+i;     // C
			indexes[j*div_round*6+6*i+3] = (j+1)*(div_round+1)+i;     // C
			indexes[j*div_round*6+6*i+4] = j*(div_round+1)+(i+1);     // B
			indexes[j*div_round*6+6*i+5] = (j+1)*(div_round+1)+(i+1); // D
		}
	}

	uint j = div_height;
	for(uint i=0;i<div_round ;i++) {
		indexes[j*div_round*6+3*i+0] = nb_points - 1;
		indexes[j*div_round*6+3*i+1] = j*(div_round+1)+i;
		indexes[j*div_round*6+3*i+2] = j*(div_round+1)+(i+1);
	}
	j = 0;
	for(uint i=0;i<div_round ;i++) {
		indexes[j*div_round*6+3*i+0] = nb_points - 2;
		indexes[j*div_round*6+3*i+1] = j*(div_round+1)+i;
		indexes[j*div_round*6+3*i+2] = j*(div_round+1)+(i+1);
	}

	cylinder->addOneBuffer(0,3,coord,"coordinates",false);
	cylinder->addOneBuffer(1,3,normals,"normals",false);
	cylinder->addOneBuffer(2,2,uv,"uvs",false);
	cylinder->addIndexBuffer(indexes,false);

	return cylinder;
}

inline IndexedMesh* meshCone(uint div_round, uint div_height, uint height) {
	uint nb_points = (div_round+1)*(div_height+1) + 1;
	uint nb_prim = 2*div_round*div_height + div_round;
	IndexedMesh* cone = new IndexedMesh(nb_prim,nb_points,GL_TRIANGLES);
	float* coord = new float[nb_points*3];
	float* normals = new float[nb_points*3];
	float* uv = new float[nb_points*2];
	uint* indexes = new uint[3*nb_prim];
	float alpha = (2*M_PI)/ div_round, phi = M_PI/(div_height+1);
	double sqrt2 = std::sqrt(2);
	double _1_sqrt2 = 1 / sqrt2;

	for(uint j=0;j<=div_height;j++) {
		for(uint i=0;i<=div_round;i++) {
			coord[3*j*(div_round+1)+3*i  ] =  cos(i*alpha) * (height - cos(j*phi));
			coord[3*j*(div_round+1)+3*i+1] = sin(i*alpha) * (height - cos(j*phi));
			coord[3*j*(div_round+1)+3*i+2] = cos(j*phi);
			normals[3*j*(div_round+1)+3*i  ] = cos(i*alpha) * height / sqrt2;
			normals[3*j*(div_round+1)+3*i+1] = sin(i*alpha) * height / sqrt2;
			normals[3*j*(div_round+1)+3*i+2] = _1_sqrt2;
			uv[2*j*(div_round+1)+2*i  ] = i/(float)div_round;
			uv[2*j*(div_round+1)+2*i+1] = j/(float)div_height;
		}
	}
	coord[nb_points*3-3] = 0.;
	coord[nb_points*3-2] = 0.;
	coord[nb_points*3-1] = -1.;
	normals[nb_points*3-3] = 0.;
	normals[nb_points*3-2] = 0.;
	normals[nb_points*3-1] = -1.;
	uv[2*nb_points-2] = 1;
	uv[2*nb_points-1] = 1;

	for(uint  j=0;j<div_height;j++) {
		for(uint i=0;i<div_round ;i++) {
			indexes[j*div_round*6+6*i+0] = j*(div_round+1)+i;         // A
			indexes[j*div_round*6+6*i+1] = j*(div_round+1)+(i+1);     // B
			indexes[j*div_round*6+6*i+2] = (j+1)*(div_round+1)+i;     // C
			indexes[j*div_round*6+6*i+3] = (j+1)*(div_round+1)+i;     // C
			indexes[j*div_round*6+6*i+4] = j*(div_round+1)+(i+1);     // B
			indexes[j*div_round*6+6*i+5] = (j+1)*(div_round+1)+(i+1); // D
		}
	}

	uint j = div_height;
	for(uint i=0;i<div_round ;i++) {
		indexes[j*div_round*6+3*i+0] = nb_points - 1;
		indexes[j*div_round*6+3*i+1] = j*(div_round+1)+i;
		indexes[j*div_round*6+3*i+2] = j*(div_round+1)+(i+1);
	}

	cone->addOneBuffer(0,3,coord,"coordinates",false);
	cone->addOneBuffer(1,3,normals,"normals",false);
	cone->addOneBuffer(2,2,uv,"uvs",false);
	cone->addIndexBuffer(indexes,false);

	return cone;
}

inline IndexedMesh* meshMobius(uint div_round, uint div_height, float d1) {
	uint nb_points = (div_round+1)*(div_height+1);
	uint nb_prim = 2*div_round*div_height;
	IndexedMesh* mobius = new IndexedMesh(nb_prim,nb_points,GL_TRIANGLES);
	float* coord = new float[nb_points*3];
	float* normals = new float[nb_points*3];
	float* uv = new float[nb_points*2];
	uint* indexes = new uint[3*nb_prim];
	float alpha = (2*M_PI)/div_round;

	for(uint j=0;j<=div_height;j++) {
		for(uint i=0;i<=div_round;i++) {
			coord[3*j*(div_round+1)+3*i  ] = (d1 + cos(i*alpha)) * cos(j*alpha) * sin(j*alpha) ;
			coord[3*j*(div_round+1)+3*i+1] = (d1 + cos(i*alpha)) * sin(j*alpha);
			coord[3*j*(div_round+1)+3*i+2] = sin(i*alpha);
			normals[3*j*(div_round+1)+3*i  ] = cos(i*alpha) * cos(j*alpha) * sin(j*alpha);
			normals[3*j*(div_round+1)+3*i+1] = cos(i*alpha) * sin(j*alpha);
			normals[3*j*(div_round+1)+3*i+2] = sin(i*alpha);
			uv[2*j*(div_round+1)+2*i  ] = i/(float)div_round;
			uv[2*j*(div_round+1)+2*i+1] = j/(float)div_height;
		}
	}

	for(uint  j=0;j<div_height;j++) {
		for(uint i=0;i<div_round ;i++) {
			indexes[j*div_round*6+6*i+0] = j*(div_round+1)+i;         // A
			indexes[j*div_round*6+6*i+1] = j*(div_round+1)+(i+1);     // B
			indexes[j*div_round*6+6*i+2] = (j+1)*(div_round+1)+i;     // C
			indexes[j*div_round*6+6*i+3] = (j+1)*(div_round+1)+i;     // C
			indexes[j*div_round*6+6*i+4] = j*(div_round+1)+(i+1);     // B
			indexes[j*div_round*6+6*i+5] = (j+1)*(div_round+1)+(i+1); // D
		}
	}

	mobius->addOneBuffer(0,3,coord,"coordinates",false);
	mobius->addOneBuffer(1,3,normals,"normals",false);
	mobius->addOneBuffer(2,2,uv,"uvs",false);
	mobius->addIndexBuffer(indexes,false);

	return mobius;
}

inline IndexedMesh* meshSquareDoughnut(uint div_round, uint div_height) {
	uint nb_points = 6*((div_round+1)*(div_height+1));
	uint nb_prim = 6*(2*div_round*div_height);
	IndexedMesh* squareDoughnut = new IndexedMesh(nb_prim,nb_points,GL_TRIANGLES);
	float* coord = new float[nb_points*3];
	float* normals = new float[nb_points*3];
	uint* indexes = new uint[3*nb_prim];
	double teta = 2.0*M_PI/(double)(div_round), phi = 1/(double)(div_height);
	double cositeta, siniteta;

	for(uint j=0;j<=div_height;j++) {
		cositeta = cos(j*teta);
		siniteta = sin(j*teta);
		for(uint i=0;i<=div_round;i++) {
			coord[3*j*(div_round+1)+3*i  ] = (i*phi+.5) * cositeta;
			coord[3*j*(div_round+1)+3*i+1] = (i*phi+.5) * siniteta;
			coord[3*j*(div_round+1)+3*i+2] = .5;
			normals[3*j*(div_round+1)+3*i  ] = 0.;
			normals[3*j*(div_round+1)+3*i+1] = 0.;
			normals[3*j*(div_round+1)+3*i+2] = -1.;

			coord[((div_round+1)*(div_height+1)*3) + 3*j*(div_round+1)+3*i  ] = (i*phi+.5) * cositeta;
			coord[((div_round+1)*(div_height+1)*3) + 3*j*(div_round+1)+3*i+1] = (i*phi+.5) * siniteta;
			coord[((div_round+1)*(div_height+1)*3) + 3*j*(div_round+1)+3*i+2] = -0.5;
			normals[((div_round+1)*(div_height+1)*3) + 3*j*(div_round+1)+3*i  ] = 0.;
			normals[((div_round+1)*(div_height+1)*3) + 3*j*(div_round+1)+3*i+1] = 0.;
			normals[((div_round+1)*(div_height+1)*3) + 3*j*(div_round+1)+3*i+2] = 1.;

			coord[((div_round+1)*(div_height+1)*3)*2 + 3*j*(div_round+1)+3*i  ] = .5 * cositeta;
			coord[((div_round+1)*(div_height+1)*3)*2 + 3*j*(div_round+1)+3*i+1] = .5 * siniteta;
			coord[((div_round+1)*(div_height+1)*3)*2 + 3*j*(div_round+1)+3*i+2] = i*phi - .5;
			normals[((div_round+1)*(div_height+1)*3)*2 + 3*j*(div_round+1)+3*i  ] = -cositeta;
			normals[((div_round+1)*(div_height+1)*3)*2 + 3*j*(div_round+1)+3*i+1] = -siniteta;
			normals[((div_round+1)*(div_height+1)*3)*2 + 3*j*(div_round+1)+3*i+2] = 0.;

			coord[((div_round+1)*(div_height+1)*3)*3 + 3*j*(div_round+1)+3*i  ] = 1.5 * cositeta;
			coord[((div_round+1)*(div_height+1)*3)*3 + 3*j*(div_round+1)+3*i+1] = 1.5 * siniteta;
			coord[((div_round+1)*(div_height+1)*3)*3 + 3*j*(div_round+1)+3*i+2] = i*phi - .5;
			normals[((div_round+1)*(div_height+1)*3)*3 + 3*j*(div_round+1)+3*i  ] = cositeta;
			normals[((div_round+1)*(div_height+1)*3)*3 + 3*j*(div_round+1)+3*i+1] = siniteta;
			normals[((div_round+1)*(div_height+1)*3)*3 + 3*j*(div_round+1)+3*i+2] = 0.;
		}
	}

	for(uint  j=0;j<div_height;j++) {
		for(uint i=0;i<div_round;i++) {
			indexes[j*div_round*6+6*i+0] = j*(div_round+1)+i;         // A
			indexes[j*div_round*6+6*i+1] = j*(div_round+1)+(i+1);     // B
			indexes[j*div_round*6+6*i+2] = (j+1)*(div_round+1)+i;     // C
			indexes[j*div_round*6+6*i+3] = (j+1)*(div_round+1)+i;     // C
			indexes[j*div_round*6+6*i+4] = j*(div_round+1)+(i+1);     // B
			indexes[j*div_round*6+6*i+5] = (j+1)*(div_round+1)+(i+1); // D

			indexes[(2*div_round*div_height)*3 +j*div_round*6+6*i+0] = (div_round+1)*(div_height+1) + j*(div_round+1)+i;         // A
			indexes[(2*div_round*div_height)*3 +j*div_round*6+6*i+1] = (div_round+1)*(div_height+1) + j*(div_round+1)+(i+1);     // B
			indexes[(2*div_round*div_height)*3 +j*div_round*6+6*i+2] = (div_round+1)*(div_height+1) + (j+1)*(div_round+1)+i;     // C
			indexes[(2*div_round*div_height)*3 +j*div_round*6+6*i+3] = (div_round+1)*(div_height+1) + (j+1)*(div_round+1)+i;     // C
			indexes[(2*div_round*div_height)*3 +j*div_round*6+6*i+4] = (div_round+1)*(div_height+1) + j*(div_round+1)+(i+1);     // B
			indexes[(2*div_round*div_height)*3 +j*div_round*6+6*i+5] = (div_round+1)*(div_height+1) + (j+1)*(div_round+1)+(i+1); // D

			indexes[(2*div_round*div_height)*3*2 +j*div_round*6+6*i+0] = (div_round+1)*(div_height+1)*2 + j*(div_round+1)+i;         // A
			indexes[(2*div_round*div_height)*3*2 +j*div_round*6+6*i+1] = (div_round+1)*(div_height+1)*2 + j*(div_round+1)+(i+1);     // B
			indexes[(2*div_round*div_height)*3*2 +j*div_round*6+6*i+2] = (div_round+1)*(div_height+1)*2 + (j+1)*(div_round+1)+i;     // C
			indexes[(2*div_round*div_height)*3*2 +j*div_round*6+6*i+3] = (div_round+1)*(div_height+1)*2 + (j+1)*(div_round+1)+i;     // C
			indexes[(2*div_round*div_height)*3*2 +j*div_round*6+6*i+4] = (div_round+1)*(div_height+1)*2 + j*(div_round+1)+(i+1);     // B
			indexes[(2*div_round*div_height)*3*2 +j*div_round*6+6*i+5] = (div_round+1)*(div_height+1)*2 + (j+1)*(div_round+1)+(i+1); // D

			indexes[(2*div_round*div_height)*3*3 +j*div_round*6+6*i+0] = (div_round+1)*(div_height+1)*3 + j*(div_round+1)+i;         // A
			indexes[(2*div_round*div_height)*3*3 +j*div_round*6+6*i+1] = (div_round+1)*(div_height+1)*3 + j*(div_round+1)+(i+1);     // B
			indexes[(2*div_round*div_height)*3*3 +j*div_round*6+6*i+2] = (div_round+1)*(div_height+1)*3 + (j+1)*(div_round+1)+i;     // C
			indexes[(2*div_round*div_height)*3*3 +j*div_round*6+6*i+3] = (div_round+1)*(div_height+1)*3 + (j+1)*(div_round+1)+i;     // C
			indexes[(2*div_round*div_height)*3*3 +j*div_round*6+6*i+4] = (div_round+1)*(div_height+1)*3 + j*(div_round+1)+(i+1);     // B
			indexes[(2*div_round*div_height)*3*3 +j*div_round*6+6*i+5] = (div_round+1)*(div_height+1)*3 + (j+1)*(div_round+1)+(i+1); // D
		}
	}

	squareDoughnut->addOneBuffer(0,3,coord,"coordinates",false);
	squareDoughnut->addOneBuffer(1,3,normals,"normals",false);
	squareDoughnut->addIndexBuffer(indexes,false);

	return squareDoughnut;
}

inline IndexedMesh * meshCube(uint div_round, uint div_height) {

	uint nb_points = 6*((div_round+1)*(div_height+1));
	uint nb_prim =  6*(2*div_round*div_height);
	IndexedMesh* cube = new IndexedMesh(nb_prim,nb_points,GL_TRIANGLES);
	float* coord = new float[nb_points*3];
	float* normals = new float[nb_points*3];
	float* uv = new float[nb_points*2];
	uint* indexes = new uint[3*nb_prim];

	double ipas = 2./div_round, jpas = 2./div_height;

	for(uint j=0;j<=div_height;j++) {
		for(uint i=0;i<=div_round;i++) {
			coord[3*j*(div_round+1)+3*i  ] = 1;
			coord[3*j*(div_round+1)+3*i+1] = j*jpas-1;
			coord[3*j*(div_round+1)+3*i+2] = 1-i*ipas;
			normals[3*j*(div_round+1)+3*i  ] = 1;
			normals[3*j*(div_round+1)+3*i+1] = 0.;
			normals[3*j*(div_round+1)+3*i+2] = 0.;
			uv[2*j*(div_round+1)+2*i  ] = i/(float)div_round;
			uv[2*j*(div_round+1)+2*i+1] = j/(float)div_height;

			coord[((div_round+1)*(div_height+1)*3) + 3*j*(div_round+1)+3*i  ] = -1;
			coord[((div_round+1)*(div_height+1)*3) + 3*j*(div_round+1)+3*i+1] = 1 - j*jpas;
			coord[((div_round+1)*(div_height+1)*3) + 3*j*(div_round+1)+3*i+2] = i * ipas -1;
			normals[((div_round+1)*(div_height+1)*3) + 3*j*(div_round+1)+3*i  ] = -1.;
			normals[((div_round+1)*(div_height+1)*3) + 3*j*(div_round+1)+3*i+1] = 0.;
			normals[((div_round+1)*(div_height+1)*3) + 3*j*(div_round+1)+3*i+2] = 0.;
			uv[((div_round+1)*(div_height+1)*2) + 2*j*(div_round+1)+2*i  ] = i/(float)div_round;
			uv[((div_round+1)*(div_height+1)*2) + 2*j*(div_round+1)+2*i+1] = j/(float)div_height;

			coord[((div_round+1)*(div_height+1)*3)*2 + 3*j*(div_round+1)+3*i  ] = 1 - j*jpas;
			coord[((div_round+1)*(div_height+1)*3)*2 + 3*j*(div_round+1)+3*i+1] = 1;
			coord[((div_round+1)*(div_height+1)*3)*2 + 3*j*(div_round+1)+3*i+2] = i*ipas -1;
			normals[((div_round+1)*(div_height+1)*3)*2 + 3*j*(div_round+1)+3*i  ] = 0;
			normals[((div_round+1)*(div_height+1)*3)*2 + 3*j*(div_round+1)+3*i+1] = 1;
			normals[((div_round+1)*(div_height+1)*3)*2 + 3*j*(div_round+1)+3*i+2] = 0;
			uv[((div_round+1)*(div_height+1)*2)*2 + 2*j*(div_round+1)+2*i  ] = i/(float)div_round;
			uv[((div_round+1)*(div_height+1)*2)*2 + 2*j*(div_round+1)+2*i+1] = j/(float)div_height;

			coord[((div_round+1)*(div_height+1)*3)*3 + 3*j*(div_round+1)+3*i  ] = j*jpas -1;
			coord[((div_round+1)*(div_height+1)*3)*3 + 3*j*(div_round+1)+3*i+1] = -1;
			coord[((div_round+1)*(div_height+1)*3)*3 + 3*j*(div_round+1)+3*i+2] = i*ipas -1;
			normals[((div_round+1)*(div_height+1)*3)*3 + 3*j*(div_round+1)+3*i  ] = 0;
			normals[((div_round+1)*(div_height+1)*3)*3 + 3*j*(div_round+1)+3*i+1] = -1;
			normals[((div_round+1)*(div_height+1)*3)*3 + 3*j*(div_round+1)+3*i+2] = 0;
			uv[((div_round+1)*(div_height+1)*2)*3 + 2*j*(div_round+1)+2*i  ] = i/(float)div_round;
			uv[((div_round+1)*(div_height+1)*2)*3 + 2*j*(div_round+1)+2*i+1] = j/(float)div_height;

			coord[((div_round+1)*(div_height+1)*3)*4 + 3*j*(div_round+1)+3*i  ] = j*jpas-1;
			coord[((div_round+1)*(div_height+1)*3)*4 + 3*j*(div_round+1)+3*i+1] = i*ipas -1;
			coord[((div_round+1)*(div_height+1)*3)*4 + 3*j*(div_round+1)+3*i+2] = 1;
			normals[((div_round+1)*(div_height+1)*3)*4 + 3*j*(div_round+1)+3*i  ] = 0;
			normals[((div_round+1)*(div_height+1)*3)*4 + 3*j*(div_round+1)+3*i+1] = 0;
			normals[((div_round+1)*(div_height+1)*3)*4 + 3*j*(div_round+1)+3*i+2] = 1;
			uv[((div_round+1)*(div_height+1)*2)*4 + 2*j*(div_round+1)+2*i  ] = i/(float)div_round;
			uv[((div_round+1)*(div_height+1)*2)*4 + 2*j*(div_round+1)+2*i+1] = j/(float)div_height;

			coord[((div_round+1)*(div_height+1)*3)*5 + 3*j*(div_round+1)+3*i  ] = j*jpas -1;
			coord[((div_round+1)*(div_height+1)*3)*5 + 3*j*(div_round+1)+3*i+1] = i*ipas -1;
			coord[((div_round+1)*(div_height+1)*3)*5 + 3*j*(div_round+1)+3*i+2] = -1;
			normals[((div_round+1)*(div_height+1)*3)*5 + 3*j*(div_round+1)+3*i  ] = 0;
			normals[((div_round+1)*(div_height+1)*3)*5 + 3*j*(div_round+1)+3*i+1] = 0;
			normals[((div_round+1)*(div_height+1)*3)*5 + 3*j*(div_round+1)+3*i+2] = -1;
			uv[((div_round+1)*(div_height+1)*2)*5 + 2*j*(div_round+1)+2*i  ] = i/(float)div_round;
			uv[((div_round+1)*(div_height+1)*2)*5 + 2*j*(div_round+1)+2*i+1] = j/(float)div_height;
		}
	}

	for(uint  j=0;j<div_height;j++) {
		for(uint i=0;i<div_round;i++) {
			indexes[j*div_round*6+6*i+0] = j*(div_round+1)+i;         // A
			indexes[j*div_round*6+6*i+1] = j*(div_round+1)+(i+1);     // B
			indexes[j*div_round*6+6*i+2] = (j+1)*(div_round+1)+i;     // C
			indexes[j*div_round*6+6*i+3] = (j+1)*(div_round+1)+i;     // C
			indexes[j*div_round*6+6*i+4] = j*(div_round+1)+(i+1);     // B
			indexes[j*div_round*6+6*i+5] = (j+1)*(div_round+1)+(i+1); // D

			indexes[(2*div_round*div_height)*3 +j*div_round*6+6*i+0] = (div_round+1)*(div_height+1) + j*(div_round+1)+i;         // A
			indexes[(2*div_round*div_height)*3 +j*div_round*6+6*i+1] = (div_round+1)*(div_height+1) + j*(div_round+1)+(i+1);     // B
			indexes[(2*div_round*div_height)*3 +j*div_round*6+6*i+2] = (div_round+1)*(div_height+1) + (j+1)*(div_round+1)+i;     // C
			indexes[(2*div_round*div_height)*3 +j*div_round*6+6*i+3] = (div_round+1)*(div_height+1) + (j+1)*(div_round+1)+i;     // C
			indexes[(2*div_round*div_height)*3 +j*div_round*6+6*i+4] = (div_round+1)*(div_height+1) + j*(div_round+1)+(i+1);     // B
			indexes[(2*div_round*div_height)*3 +j*div_round*6+6*i+5] = (div_round+1)*(div_height+1) + (j+1)*(div_round+1)+(i+1); // D

			indexes[(2*div_round*div_height)*3*2 +j*div_round*6+6*i+0] = (div_round+1)*(div_height+1)*2 + j*(div_round+1)+i;         // A
			indexes[(2*div_round*div_height)*3*2 +j*div_round*6+6*i+1] = (div_round+1)*(div_height+1)*2 + j*(div_round+1)+(i+1);     // B
			indexes[(2*div_round*div_height)*3*2 +j*div_round*6+6*i+2] = (div_round+1)*(div_height+1)*2 + (j+1)*(div_round+1)+i;     // C
			indexes[(2*div_round*div_height)*3*2 +j*div_round*6+6*i+3] = (div_round+1)*(div_height+1)*2 + (j+1)*(div_round+1)+i;     // C
			indexes[(2*div_round*div_height)*3*2 +j*div_round*6+6*i+4] = (div_round+1)*(div_height+1)*2 + j*(div_round+1)+(i+1);     // B
			indexes[(2*div_round*div_height)*3*2 +j*div_round*6+6*i+5] = (div_round+1)*(div_height+1)*2 + (j+1)*(div_round+1)+(i+1); // D

			indexes[(2*div_round*div_height)*3*3 +j*div_round*6+6*i+0] = (div_round+1)*(div_height+1)*3 + j*(div_round+1)+i;         // A
			indexes[(2*div_round*div_height)*3*3 +j*div_round*6+6*i+1] = (div_round+1)*(div_height+1)*3 + j*(div_round+1)+(i+1);     // B
			indexes[(2*div_round*div_height)*3*3 +j*div_round*6+6*i+2] = (div_round+1)*(div_height+1)*3 + (j+1)*(div_round+1)+i;     // C
			indexes[(2*div_round*div_height)*3*3 +j*div_round*6+6*i+3] = (div_round+1)*(div_height+1)*3 + (j+1)*(div_round+1)+i;     // C
			indexes[(2*div_round*div_height)*3*3 +j*div_round*6+6*i+4] = (div_round+1)*(div_height+1)*3 + j*(div_round+1)+(i+1);     // B
			indexes[(2*div_round*div_height)*3*3 +j*div_round*6+6*i+5] = (div_round+1)*(div_height+1)*3 + (j+1)*(div_round+1)+(i+1); // D

			indexes[(2*div_round*div_height)*3*4 +j*div_round*6+6*i+0] = (div_round+1)*(div_height+1)*4 + j*(div_round+1)+i;         // A
			indexes[(2*div_round*div_height)*3*4 +j*div_round*6+6*i+1] = (div_round+1)*(div_height+1)*4 + j*(div_round+1)+(i+1);     // B
			indexes[(2*div_round*div_height)*3*4 +j*div_round*6+6*i+2] = (div_round+1)*(div_height+1)*4 + (j+1)*(div_round+1)+i;     // C
			indexes[(2*div_round*div_height)*3*4 +j*div_round*6+6*i+3] = (div_round+1)*(div_height+1)*4 + (j+1)*(div_round+1)+i;     // C
			indexes[(2*div_round*div_height)*3*4 +j*div_round*6+6*i+4] = (div_round+1)*(div_height+1)*4 + j*(div_round+1)+(i+1);     // B
			indexes[(2*div_round*div_height)*3*4 +j*div_round*6+6*i+5] = (div_round+1)*(div_height+1)*4 + (j+1)*(div_round+1)+(i+1); // D

			indexes[(2*div_round*div_height)*3*5 +j*div_round*6+6*i+0] = (div_round+1)*(div_height+1)*5 + j*(div_round+1)+i;         // A
			indexes[(2*div_round*div_height)*3*5 +j*div_round*6+6*i+1] = (div_round+1)*(div_height+1)*5 + j*(div_round+1)+(i+1);     // B
			indexes[(2*div_round*div_height)*3*5 +j*div_round*6+6*i+2] = (div_round+1)*(div_height+1)*5 + (j+1)*(div_round+1)+i;     // C
			indexes[(2*div_round*div_height)*3*5 +j*div_round*6+6*i+3] = (div_round+1)*(div_height+1)*5 + (j+1)*(div_round+1)+i;     // C
			indexes[(2*div_round*div_height)*3*5 +j*div_round*6+6*i+4] = (div_round+1)*(div_height+1)*5 + j*(div_round+1)+(i+1);     // B
			indexes[(2*div_round*div_height)*3*5 +j*div_round*6+6*i+5] = (div_round+1)*(div_height+1)*5 + (j+1)*(div_round+1)+(i+1); // D
		}
	}

	cube->addOneBuffer(0,3,coord,"coordinates",false);
	cube->addOneBuffer(1,3,normals,"normals",false);
	cube->addOneBuffer(2,2,uv,"uvs",false);
	cube->addIndexBuffer(indexes,false);

	return cube;
}

inline IndexedMesh* meshTore(uint div_round, uint div_height, float d1) {
	uint nb_points = (div_round+1)*(div_height+1);
	uint nb_prim = 2*div_round*div_height;
	IndexedMesh* tore = new IndexedMesh(nb_prim,nb_points,GL_TRIANGLES);
	float* coord = new float[nb_points*3];
	float* normals = new float[nb_points*3];
	float* uv = new float[nb_points*2];
	uint* indexes = new uint[3*nb_prim];
	float alpha = (2*M_PI)/div_round;

	for(uint j=0;j<=div_height;j++) {
		for(uint i=0;i<=div_round;i++) {
			coord[3*j*(div_round+1)+3*i  ] = (d1 + cos(i*alpha)) * cos(j*alpha);
			coord[3*j*(div_round+1)+3*i+1] = (d1 + cos(i*alpha)) * sin(j*alpha);
			coord[3*j*(div_round+1)+3*i+2] = sin(i*alpha);
			normals[3*j*(div_round+1)+3*i  ] = cos(i*alpha) * cos(j*alpha);
			normals[3*j*(div_round+1)+3*i+1] = cos(i*alpha) * sin(j*alpha);
			normals[3*j*(div_round+1)+3*i+2] = sin(i*alpha);
			uv[2*j*(div_round+1)+2*i  ] = i/(float)div_round;
			uv[2*j*(div_round+1)+2*i+1] = j/(float)div_height;
		}
	}

	for(uint  j=0;j<div_height;j++) {
		for(uint i=0;i<div_round ;i++) {
			indexes[j*div_round*6+6*i+0] = j*(div_round+1)+i;         // A
			indexes[j*div_round*6+6*i+1] = j*(div_round+1)+(i+1);     // B
			indexes[j*div_round*6+6*i+2] = (j+1)*(div_round+1)+i;     // C
			indexes[j*div_round*6+6*i+3] = (j+1)*(div_round+1)+i;     // C
			indexes[j*div_round*6+6*i+4] = j*(div_round+1)+(i+1);     // B
			indexes[j*div_round*6+6*i+5] = (j+1)*(div_round+1)+(i+1); // D
		}
	}
	tore->addOneBuffer(0,3,coord,"coordinates",false);
	tore->addOneBuffer(1,3,normals,"normals",false);
	tore->addOneBuffer(2,2,uv,"uvs",false);
	tore->addIndexBuffer(indexes,false);
	return tore;
}
};

#endif
