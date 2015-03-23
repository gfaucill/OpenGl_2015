/***************************************************************************
                      simple_move.hpp  -  description
                             -------------------
    copyright            : (C) 2012 by Biri
    email                : biri@univ-mlv.fr
 ***************************************************************************/
/***************************************************************************
 *   Copyright (C) 2012 by Venceslas BIRI                                  *
 *   biri@univ-mlv.fr                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef ENS_PLATEFORM_INTERFACE_H
#define ENS_PLATEFORM_INTERFACE_H

#include "sceneGraph/skyBox.hpp"
#include "sceneGraph/objectfactory.h"
#include "tools/basic_mesh.hpp"
#include "tools/camera.hpp"
#include "tools/col3f.hpp"
#include "tools/matrix_stack.hpp"
#include "tools/texture.hpp"
#include "sceneGraph/lights.hpp"
#include "sceneGraph/tree.hpp"
#include "sceneGraph/texture.h"
#include "tools/cameraManager.hpp"


using namespace STP3D;

//////////////////////////////////////////////////////////////////////////////
/// globals
//////////////////////////////////////////////////////////////////////////////

class Interface {
public:
    Interface(unsigned int w,unsigned int h) : width_ecran(w), height_ecran(h) {};

    /// Window size
    unsigned int width_ecran;
    unsigned int height_ecran;

    int flagFilaire;
    int flagAnimation;
    int flagLight;

    GLuint idShader;
    StandardMesh* repere;
    LightFactory lights;//lights
    ObjectFactory objFactory; // Objects factory
    TransfoFactory transFactory; //Tranfo Factory
    TextureFactory textureFactory; //Texture Factory
    CameraManager camManager; //Camera manager
    FPSCamera * currentCam;
    Tree sceneGraph;  //Scene Graph
    Tree * rep; // 3D repere

	/// ///////////////////////////////////////////////////////////////////////////
	///             HMI and callback functions
	/// ///////////////////////////////////////////////////////////////////////////
	void kbdFunc(unsigned char,int,int);
	void kbdSpFunc(int,int,int);
	void mouseFunc(int button,int state,int x,int y);
	void idleFunc();

	void initEngine();

	/// ///////////////////////////////////////////////////////////////////////////
	///             Sub object Function
	/// ///////////////////////////////////////////////////////////////////////////
	Tree *patinHelico(ObjMaterial mat);
	Tree *queueHelico(ObjMaterial mat);
	Tree *makeHelico();

	/// ///////////////////////////////////////////////////////////////////////////
	///             Display functions
	/// ///////////////////////////////////////////////////////////////////////////
    void reshape(int,int);
    void drawScene();
    void initDisplay();
    Tree * create3DRepere();
private:

    void drawGround();
};


#endif
