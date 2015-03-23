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

#include "interface.hpp"
#include "tools/gl_tools.hpp"
#include "tools/shaders.hpp"
#include "tools/col3f.hpp"
#include <GL/glut.h>

#define PAS_ROT   3.0
#define PAS_DEPL  0.1

/// ///////////////////////////////////////////////////////////////////////////
/// Globals
/// ///////////////////////////////////////////////////////////////////////////

// ///////////////////////////////////////////////////////////////////////////
//        HMI functions
// ///////////////////////////////////////////////////////////////////////////

//  ///////////////////////////////////////////////////////////////////////////
/// keyboard interuption callback function.
/// \param c : key hit
/// \param x,y : mouse coordinates when hiting the key
void Interface::kbdFunc(unsigned char c, int x, int y)
{

	transFactory.trigStep(std::string((char*)&c));

	switch(c) {
	case 27: // quit
		exit(0);
		break;
	case 'f':
		flagFilaire = 1 - flagFilaire;
		std::cout<<"Mode wire is ";
		std::cout<<((flagFilaire)?"ON":"OFF")<<std::endl;
		break;
	case 'q': // rotation y (-)
		currentCam->lookRight(PAS_ROT);
		break;
	case 'd': // rotation y (+)
		currentCam->lookLeft(PAS_ROT);
		break;
	case 'z': // rotation x (-)
		currentCam->lookUp(PAS_ROT);
		break;
	case 's': // rotation x (+)
		currentCam->lookDown(PAS_ROT);
		break;
	case ' ':
		flagAnimation = 1 - flagAnimation;
		break;
	case 'l' : flagLight = (flagLight)?0:1;
		if (flagLight) lights.enableLigthing();
		else lights.disableLighting();
	break;
	// Camera switch
	case 'o' : camManager.nextCamera();
		break;
	case 'p' : camManager.previewCamera();
		break;
	default :
		//std::cerr<<"Key with no binding (mouse at "<<x<<"/"<<y<<")"<<std::endl;
		break;
	}
	glutPostRedisplay();
}

//  ///////////////////////////////////////////////////////////////////////////
/// special keyboard interuption callback function.
/// \param c : key hit
/// \param x,y : mouse coordinates when hiting the key
void Interface::kbdSpFunc(int c, int , int )
{
	switch(c)
	{
	case GLUT_KEY_UP :
		currentCam->stepFront(PAS_DEPL);
		break;
	case GLUT_KEY_DOWN :
		currentCam->stepBack(PAS_DEPL);
		break;
	case GLUT_KEY_LEFT :
		currentCam->stepLeft(PAS_DEPL);
		break;
	case GLUT_KEY_RIGHT :
		currentCam->stepRight(PAS_DEPL);
		break;
	case GLUT_KEY_PAGE_UP :
		currentCam->stepUp(PAS_DEPL);
		break;
	case GLUT_KEY_PAGE_DOWN :
		currentCam->stepDown(PAS_DEPL);
		break;
	default :
		std::cerr<<"Touche speciale non fonctionnelle"<<std::endl;
	}
	glutPostRedisplay();
}

//  ///////////////////////////////////////////////////////////////////////////
/// clic mouse button interuption callback function.
/// \param button : which button is clicked
/// \param state : button pushed or released
/// \param x,y : mouse coordinates when clicking
void Interface::mouseFunc(int button,int state,int x,int y)
{
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		glutPostRedisplay();
	}
}

//  ///////////////////////////////////////////////////////////////////////////
/// no event callback function.
void Interface::idleFunc(void)
{
	if (flagAnimation) {
		transFactory.step();
		glutPostRedisplay();
	}
}


//  ///////////////////////////////////////////////////////////////////////////
/// Initialisation of the engine (scene and HMI)
void Interface::initEngine()
{
	/// Parameters initialization
	flagFilaire = 0;
	flagAnimation = 0;
	flagLight = 1;

	/// GL initialization
	std::cerr<<"GL initialization"<<std::endl;
	initDisplay();
}

//  ///////////////////////////////////////////////////////////////////////////
/// reshape window interuption callback function.
/// \param width : width of the new window
/// \param height : height of the new window
void Interface::reshape(int width,int height)
{
	//double ratio=(double)height/(double)width;

	width_ecran = width;
	height_ecran = height;

	glViewport(0, 0, (GLint)width, (GLint)height);
}

Tree* Interface::patinHelico(ObjMaterial mat) {

	Tree *patin, *t, *r, *p;
	patin = new Tree();

	p = patin->add(transFactory.getTranslation(0,1,0));
	p = p->add(transFactory.getRotationd('z',180));

	r = t = p->add(transFactory.getRotationd('x', 90));
	t = t->add(transFactory.getTranslation(0,-1,0));
	t = t->add(transFactory.getScale3c(0.1,0.1,0.05));
	t->add(objFactory.createCylinder(mat));


	for (int i = 0; i <= 90; i += 5 ) {
		t = p->add(transFactory.getRotationd('x',i));
		t->add(r);
	}

	//longeur
	t = p->add(transFactory.getTranslation(0,1,-1.5));
	t = t->add(transFactory.getScale3c(0.1,0.1,1.5));
	t->add(objFactory.createCylinder(mat));

	//remonter
	t = patin->add(transFactory.getTranslation(0,0.8,-0.5));
	t = t->add(transFactory.getRotationd('x',90));
	t = t->add(transFactory.getScale3c(0.1,0.1,0.8));
	t->add(objFactory.createCylinder(mat));

	t = patin->add(transFactory.getTranslation(0,0.8,-2));
	t = t->add(transFactory.getRotationd('x',90));
	t = t->add(transFactory.getScale3c(0.1,0.1,0.8));
	t->add(objFactory.createCylinder(mat));

	//marche pied
	t = patin->add(transFactory.getTranslation(0,0.4,-1.25));
	t = t->add(transFactory.getScale3c(0.1,0.1,0.75));
	t->add(objFactory.createCylinder(mat));

	return patin;
}

Tree* Interface::queueHelico(ObjMaterial mat) {
	#define dia 0.05
	Tree * r = new Tree(), *t, *c;

	// noeud null pour liker un autre croision
	c = r->add(transFactory.getTranslation(0.,0,0));

	// barre principal le V
	t = c->add(transFactory.getRotationd('x', -7.5));
	t = t->add(transFactory.getTranslation(0,0,2.5));
	t = t->add(transFactory.getScale3c(dia*2,dia*2,2.5));
	t->add(objFactory.createCylinder(mat));

	t = c->add(transFactory.getRotationd('x', 7.5));
	t = t->add(transFactory.getTranslation(0,0,2.5));
	t = t->add(transFactory.getScale3c(dia*2,dia*2,2.5));
	t->add(objFactory.createCylinder(mat));


	// barre vertical
	t = c->add(transFactory.getTranslation(0.,0.,1.25));
	t = t->add(transFactory.getRotationd('x', 90));
	t = t->add(transFactory.getScale3c(dia,dia,0.16));
	t->add(objFactory.createCylinder(mat));

	t = c->add(transFactory.getTranslation(0.,0.,2.50));
	t = t->add(transFactory.getRotationd('x', 90));
	t = t->add(transFactory.getScale3c(dia,dia,0.30));
	t->add(objFactory.createCylinder(mat));

	t = c->add(transFactory.getTranslation(0.,0.,3.75));
	t = t->add(transFactory.getRotationd('x', 90));
	t = t->add(transFactory.getScale3c(dia,dia,0.5));
	t->add(objFactory.createCylinder(mat));

	t = c->add(transFactory.getTranslation(0.,0.,5));
	t = t->add(transFactory.getRotationd('x', 90));
	t = t->add(transFactory.getScale3c(dia,dia,0.65));
	t->add(objFactory.createCylinder(mat));

	// barre oblique
	t = c->add(transFactory.getTranslation(0.,0.,1.87));
	t = t->add(transFactory.getRotationd('x', -14.78));
	t = t->add(transFactory.getScale3c(dia,dia,0.64));
	t->add(objFactory.createCylinder(mat));

	t = c->add(transFactory.getTranslation(0.,0.,3.125));
	t = t->add(transFactory.getRotationd('x', 38.6));
	t = t->add(transFactory.getScale3c(dia,dia,0.67));
	t->add(objFactory.createCylinder(mat));

	t = c->add(transFactory.getTranslation(0.,0.,4.37));
	t = t->add(transFactory.getRotationd('x', -38.6));
	t = t->add(transFactory.getScale3c(dia,dia,0.87));
	t->add(objFactory.createCylinder(mat));
	// fin tringle

	// croision
	t = r->add(transFactory.getRotationd('z',90));
	t->add(c);

	return r;
}

Tree* Interface::makeHelico() {
	Tree * helico = new Tree();
	Tree * t;	Tree * r;

	// Macro Objet creation
	Tree * patin = patinHelico((ObjMaterial){Vector4D(0.4,0.4,0.4,0), 0.4, 0.5, 0.4, 20 });
	Tree * queue = queueHelico((ObjMaterial){Vector4D(0.8,0.1,0.1,0), 0.4, 0.5, 0.4, 10 });

	//queue
	helico->add(queue);

	//cabine
	r = t = helico->add(transFactory.getTranslation(0.,0.,5));
	t = t->add(transFactory.getRotationd('x',90));
	t = t->add(transFactory.getRotationd('z',-90));
	t = t->add(transFactory.getScale1c(1.3));
	t->add(objFactory.createSphere((ObjMaterial){Vector4D(0.93,0.94,0.11,0), 0.4, 0.5, 0.4, 20 }));

	//les patins
	t = r->add(transFactory.getTranslation(-0.7,-1.8,1.3));
	t = t->add(transFactory.getRotationd('z', 10));
	t->add(patin);

	t = r->add(transFactory.getTranslation(0.7,-1.8,1.3));
	t = t->add(transFactory.getRotationd('z', -10));
	t->add(patin);

	//helice
	r = t = r->add(transFactory.getTranslation(0.,1.5,0));
	t = t->add(transFactory.getRotationd('y', (char*)"a"));
	t = t->add(transFactory.getRotationd('x', 90));
	t = t->add(transFactory.getScale3c(0.4,3,0.05));
	t->add(objFactory.createMobius((ObjMaterial){Vector4D(0.31,0.67,0.07,0), 0.4, 0.5, 0.4, 20 }));


	//rotor principal
	t = r->add(transFactory.getTranslation(0,-0.8,0));
	t = t->add(transFactory.getRotationd('x', 90));
	t = t->add(transFactory.getScale3c(0.2,0.2,1));
	t->add(objFactory.createCylinder((ObjMaterial){Vector4D(0.31,0.67,0.07,0), 0.4, 0.5, 0.4, 20 }));

	t = r->add(transFactory.getRotationd('x',90));
	t = t->add(transFactory.getScale1c(0.1));
	t->add(objFactory.createTore((ObjMaterial){Vector4D(0.31,0.67,0.07,0), 0.4, 0.5, 0.4, 20 }));


	//rotor de queue
	t = helico->add(transFactory.getTranslation(0.2,0,0));
	t = t->add(transFactory.getRotationd('x', (char*)"b"));
	t = t->add(transFactory.getRotationd('y', 90));
	t = t->add(transFactory.getScale3c(0.4,2,0.1));
	t = t->add(transFactory.getScale1c(0.3));
	t->add(objFactory.createMobius((ObjMaterial){Vector4D(0.31,0.67,0.07,0), 0.4, 0.5, 0.4, 20 }));

	return helico;
}

Tree * Interface::create3DRepere() {
	Tree * t = new Tree(), *x, *y, *z, *r;

	z = t->add(transFactory.getScale3c(0.05,0.05,0.5));
	z = z->add(transFactory.getTranslation(0,0,1));
	z->add(objFactory.createCylinder((ObjMaterial){Vector4D(0,0,1,0), 0.5, 0.5, 0.5, 10 }));

	z = t->add(transFactory.getTranslation(0,0,1));
	z = z->add(transFactory.getScale1c(0.05));
	z->add(objFactory.createCone((ObjMaterial){Vector4D(0,0,1,0), 0.5, 0.5, 0.5, 10 }));

	r = t->add(transFactory.getRotationd('x', 90));

	y = r->add(transFactory.getScale3c(0.05,0.05,0.5));
	y = y->add(transFactory.getTranslation(0,0,1));
	y->add(objFactory.createCylinder((ObjMaterial){Vector4D(0,1,0,0), 0.5, 0.5, 0.5, 10 }));

	y = r->add(transFactory.getTranslation(0,0,1));
	y = y->add(transFactory.getScale1c(0.05));
	y->add(objFactory.createCone((ObjMaterial){Vector4D(0,1,0,0), 0.5, 0.5, 0.5, 10 }));

	r = t->add(transFactory.getRotationd('y', 90));

	x = r->add(transFactory.getScale3c(0.05,0.05,0.5));
	x = x->add(transFactory.getTranslation(0,0,1));
	x->add(objFactory.createCylinder((ObjMaterial){Vector4D(1,0,0,0), 0.5, 0.5, 0.5, 10 }));

	x = r->add(transFactory.getTranslation(0,0,1));
	x = x->add(transFactory.getScale1c(0.05));
	x->add(objFactory.createCone((ObjMaterial){Vector4D(1,0,0,0), 0.5, 0.5, 0.5, 10 }));

	r = t->add(transFactory.getScale1c(0.05));
	r->add(objFactory.createSphere((ObjMaterial){Vector4D(1,1,1,0), 0.5, 0.5, 0.5, 10 }));

	return t;
}

void Interface::initDisplay() {
	/// CAMERA INITIALIZATION...
	// Cam manager
	camManager.createCamera(Vector3D(-3.0,2.0,10.0),-Vector3D(0.0,1.0,3.0));
	camManager.createCamera(Vector3D(4.0,10.,10.),-Vector3D(4.0,10.,10.0));
    //camManager.createCamera(Vector3D(4.0,-10.,-10.),-Vector3D(4.0,-10.,-10.0));
	Camera * cam = camManager.createCamera(Vector3D(0.0,0.0,-10.0), Vector3D(0.0,0.0,-10.0));
	Camera * cabine = camManager.createCamera(Vector3D(0.0,0.0,8.0), -Vector3D(0.0,0.0, 8.0));
	Camera * camAvion = camManager.createCamera(Vector3D(0.0,5.0,0.0), -Vector3D(0.0,5.0,0.0));

	currentCam = camManager.getCurentFPSCamera();

	/// SHADER INITIALIZATION...
	idShader = ShaderManager::loadShader("./shaders/flat_shading.vert","./shaders/flat_shading.frag",true);

	/// INITIALISATION DES VBOS ...
	repere = STP3D::createRepere(1.0);
	repere->createVAO();
	repere->releaseCPUMemory();

	/// Light creation
    Vector3D power(1,1,1); power *= 1;
    lights.createPonctualLight(power,power,power, Vector3D(20,20,10));
    //lights.createPonctualLight(Vector3D(1,1,1), Vector3D(1,1,1), Vector3D(1,1,1), Vector3D(-10,-10,10));
    //lights.createSpotLight(spotColor,spotColor,spotColor, Vector3D(1,0,0), Vector3D(0,0,0), 0.01);

	//Animation variable
	transFactory.setControlVariable((char*)"null", 0, 0, 0, TransfoFactory::NO);

	transFactory.setControlVariable((char*)"a", 0, 360, 2, TransfoFactory::YES);
	transFactory.setControlVariable((char*)"b", 0, 360, 2, TransfoFactory::YES);

	transFactory.setControlVariable((char*)"i", 0, 360, -0.01, TransfoFactory::YES);
	transFactory.setControlVariable((char*)"j", -6, 6, 0.05, TransfoFactory::REVERSE);


	//Transfo avec offset pour l'anim de départ
	transFactory.setControlVariable((char*)"c", 0, 5, 0.1, TransfoFactory::NO);
	transFactory.setControlVariable((char*)"d", 0, 5, 0.1, TransfoFactory::NO, 50);
	transFactory.setControlVariable((char*)"e",6.28,0, -0.01, TransfoFactory::NO, 100);
	transFactory.setControlVariable((char*)"f", 0, -5, -0.1, TransfoFactory::NO, 728);
	transFactory.setControlVariable((char*)"h", 0, -5, -0.1, TransfoFactory::NO, 778);

	//Tranfo avec bind pour controller l'helico au clavier
	transFactory.setControlVariable((char*)"turn", 0, 6.28,0.05, TransfoFactory::YES);
	transFactory.bindControlVariable((char*)"turn", (char*)"5");

	transFactory.setControlVariable((char*)"up/down", 0, 20,0.05, TransfoFactory::REVERSE);
	transFactory.bindControlVariable((char*)"up/down", (char*)"+");

	transFactory.setControlVariable((char*)"left/right", -20, 20,0.05, TransfoFactory::REVERSE,0,'+',true);
	transFactory.bindControlVariable((char*)"left/right", (char*)"4");

	transFactory.setControlVariable((char*)"front/back", -20, 20,0.05, TransfoFactory::REVERSE,0,'+',true);
	transFactory.bindControlVariable((char*)"front/back", (char*)"8");

	rep = create3DRepere();

	//// Tree building
	Tree * t;
	Tree * helicoBase = new Tree();
	Tree * helico = new Tree();
	helico = helico->add(transFactory.getTranslation(0.,0.,-5));
	helico->add(makeHelico());

	//Helico
	t = helicoBase->add(transFactory.getTranslation((char*)"null",(char*)"h",(char*)"null"));
	t = t->add(transFactory.getRotationd('y',(char*)"e"));
	//t = t->add(transFactory.getRotationd('y',"turn"));
 	t = t->add(transFactory.getTranslation((char*)"null",(char*)"c",(char*)"null"));
 	t = t->add(transFactory.getTranslation((char*)"d", (char*)"null",(char*)"null"));
 	t = t->add(transFactory.getTranslation((char*)"f", (char*)"null",(char*)"null"));
	t = t->add(transFactory.getTranslation((char*)"null",(char*)"up/down",(char*)"null"));
	t = t->add(transFactory.getTranslation((char*)"left/right", (char*)"null",(char*)"null"));
	t = t->add(transFactory.getTranslation((char*)(char*)"null",(char*)"null", (char*)"front/back"));
	t = t->add(transFactory.getRotationd('y',(char*)"turn"));
	t = t->add(helico);
    t = t->add(lights.createSpotLight(Vector3D(1,1,1),Vector3D(1,1,1),Vector3D(1,1,1), Vector3D(0,0,1), Vector3D(0,0,0), 0.3));
	t = t->add(cabine);
	t->add(cam);

	//Sol2
	t = helicoBase->add(transFactory.getTranslation(0,-6,0));
	t = t->add(transFactory.getScale3c(75,1,75));
	t = t->add(textureFactory.createTexture("ressources/skyBox/arr/dn.ppm"));
	t->add(objFactory.createCube((ObjMaterial){Vector4D(0,0,0,0), 0.5, 0.5, 0.5, 10 }));

	//Base
	t = helicoBase->add(transFactory.getTranslation(0,-5,0));
	t = t->add(transFactory.getScale3c(6,3,6));
	t = t->add(textureFactory.createTexture("ressources/masonry-wall-texture.ppm"));
	t->add(objFactory.createCube((ObjMaterial){Vector4D(0,0,0,0), 0.5, 0.5, 0.5, 10 }));

	//Heliport
	t = helicoBase->add(transFactory.getTranslation(0,-2.9,0));
	t = t->add(transFactory.getScale3c(4,1,4));
	t = t->add(textureFactory.createTexture("ressources/h.ppm"));
	t->add(objFactory.createCube((ObjMaterial){Vector4D(0,0,0,0), 0.5, 0.5, 0.5, 10 }));

	//Add helico + base (principale)
	sceneGraph.add(helicoBase);

	//Copy helico base
//	t = sceneGraph.add(transFactory.getTranslation(5,5,6));	t->add(helicoBase);
//	t = sceneGraph.add(transFactory.getTranslation(-5,5,6));	t->add(helicoBase);
//	t = sceneGraph.add(transFactory.getTranslation(5,-5,6));	t->add(helicoBase);
//	t = sceneGraph.add(transFactory.getTranslation(-5,-5,6));	t->add(helicoBase);

	Tree * avion = new Tree();
	Tree * a;

	/* Base avion == */
	a = avion->add(transFactory.getScale3c(1,0.4,0.4));
	a = a->add(transFactory.getRotationr('y', -M_PI/2.));
	a = a->add(transFactory.getScale3c(0.75,0.75,1));
	a->add(objFactory.createCylinder((ObjMaterial){Vector4D(1,1,1,0), 0.5, 0.5, 0.5, 10 }));

	/* Base Avion o= */
	a = avion->add(transFactory.getTranslation(-1,0,0));
	a = a->add(transFactory.getScale3c(0.75,0.299,0.299));
	a->add(objFactory.createSphere((ObjMaterial){Vector4D(1,1,1,0), 0.5, 0.5, 0.5, 10 }));

	/* Base Avion =o */
	a = avion->add(transFactory.getTranslation(1,0,0));
	a = a->add(transFactory.getScale3c(0.5,0.299,0.299));
	a->add(objFactory.createSphere((ObjMaterial){Vector4D(1,1,1,0), 0.5, 0.5, 0.5, 10 }));

	/* Base Avion =O */
	a = avion->add(transFactory.getTranslation(1.25,0.25,0));
	a = a->add(transFactory.getScale3c(0.3,0.5,0.1));
	a->add(objFactory.createSphere((ObjMaterial){Vector4D(0,0,1,0), 0.5, 0.5, 0.5, 10 }));

	/* Base Avion =0 */
	a = avion->add(transFactory.getTranslation(1,0,0));
	a = a->add(transFactory.getRotationr('y', -M_PI/2));
	a = a->add(transFactory.getScale3c(0.7,0.05,0.3));
	a->add(objFactory.createSphere((ObjMaterial){Vector4D(0,0,1,0), 0.5, 0.5, 0.5, 10 }));

	Tree * vitre = new Tree();
	Tree * v = vitre->add(transFactory.getScale1c(0.5));
	v = v->add(transFactory.getTranslation(0,0.3,0));

	/* vitre Avion /__ */
	a = v->add(transFactory.getTranslation(-1,0.2,0));
	a = a->add(transFactory.getRotationr('y', -M_PI/2));
	a = a->add(transFactory.getScale1c(0.3));
	a->add(objFactory.createCone((ObjMaterial){Vector4D(0.5,0.5,1,0), 0.5, 0.5, 0.5, 10 }));

	/* contoure vitre Avion /O */
	a = v->add(transFactory.getTranslation(-0.7,0.2,0.));
	a = a->add(transFactory.getRotationr('y',-M_PI/2));
	a = a->add(transFactory.getScale3c(0.20,0.20,0.1));
	a->add(objFactory.createTore((ObjMaterial){Vector4D(1,0,0,0), 0.5, 0.5, 0.5, 10 }));

	avion->add(vitre);

	/* siege Avion /O */
	a = avion->add(transFactory.getTranslation(0.4,0.2,0.));
	a = a->add(transFactory.getScale3c(0.05,0.3,0.2));
	a->add(objFactory.createSphere((ObjMaterial){Vector4D(1,0,0,0), 0.5, 0.5, 0.5, 10 }));

	/* siege Avion /O */
	a = avion->add(transFactory.getTranslation(0.1,0.2,0.));
	a = a->add(transFactory.getScale3c(0.05,0.3,0.2));
	a->add(objFactory.createSphere((ObjMaterial){Vector4D(1,0,0,0), 0.5, 0.5, 0.5, 10 }));

	Tree * a00;
	a00 = avion->add(transFactory.getTranslation(0.3,0,0));

	Tree * asr;
	asr = a00->add(transFactory.getTranslation(-1.2,-0.3,0));
	asr = asr->add(transFactory.getRotationr('y',M_PI/2));

	/* struct avant roue 1 */
	a = asr->add(transFactory.getTranslation(0.15,0,0));
	a = a->add(transFactory.getRotationr('x',-M_PI/5.5));
	a = a->add(transFactory.getScale3c(0.05,0.05,0.38));
	a->add(objFactory.createCylinder((ObjMaterial){Vector4D(1,0,0,0), 0.5, 0.5, 0.5, 10 }));

	/* struct avant roue 2 */
	a = asr->add(transFactory.getTranslation(-0.15,0,0));
	a = a->add(transFactory.getRotationr('x',-M_PI/5.5));
	a = a->add(transFactory.getScale3c(0.05,0.05,0.38));
	a->add(objFactory.createCylinder((ObjMaterial){Vector4D(1,0,0,0), 0.5, 0.5, 0.5, 10 }));

	/* noeud de déplacement struct arrière */
	Tree * asra;
	asra = a00->add(transFactory.getTranslation(-0.8,-0.3,0));
	asra = asra->add(transFactory.getRotationr('y',M_PI/2));

	/* struct arrière roue 1 */
	a = asra->add(transFactory.getTranslation(0.15,0,0));
	a = a->add(transFactory.getRotationr('x',M_PI/3));
	a = a->add(transFactory.getScale3c(0.05,0.05,0.2));
	a->add(objFactory.createCylinder((ObjMaterial){Vector4D(1,0,0,0), 0.5, 0.5, 0.5, 10 }));

	/* struct arrière roue 2 */
	a = asra->add(transFactory.getTranslation(-0.15,0,0));
	a = a->add(transFactory.getRotationr('x',M_PI/3));
	a = a->add(transFactory.getScale3c(0.05,0.05,0.2));
	a->add(objFactory.createCylinder((ObjMaterial){Vector4D(1,0,0,0), 0.5, 0.5, 0.5, 10 }));

	/* noeud de déplacement struct centrale */
	Tree * asrc;
	asrc = a00->add(transFactory.getTranslation(-0.9,-0.5,0.));

	/* struct arrière roue 1 */
	a = asrc->add(transFactory.getScale3c(0.1,0.1,0.2));
	a->add(objFactory.createCylinder((ObjMaterial){Vector4D(1,0,0,0), 0.5, 0.5, 0.5, 10 }));

	/* roue 1 */
	a = asrc->add(transFactory.getTranslation(0,0,0.2));
	a = a->add(transFactory.getScale1c(0.075));
	a->add(objFactory.createTore((ObjMaterial){Vector4D(0,0,0,0), 0.5, 0.5, 0.5, 10 }));

	/* roue 2 */
	a = asrc->add(transFactory.getTranslation(0,0,-0.2));
	a = a->add(transFactory.getScale1c(0.075));
	a->add(objFactory.createTore((ObjMaterial){Vector4D(0,0,0,0), 0.5, 0.5, 0.5, 10 }));

	/* noeud de placement struct ailes */
	Tree * aa;
	aa = avion->add(transFactory.getTranslation(-0.3,-0.25,0.));

	/* aile basse ___ */
	a = aa->add(transFactory.getScale3c(0.5,0.05,1.2));
	a->add(objFactory.createSphere((ObjMaterial){Vector4D(0,0,0.8,0), 0.5, 0.5, 0.5, 10 }));

	/* aile haute --- */
	a = aa->add(transFactory.getTranslation(0.,1,0));
	a = a->add(transFactory.getScale3c(0.5,0.05,1.2));
	a->add(objFactory.createSphere((ObjMaterial){Vector4D(0,0,0.8,0), 0.5, 0.5, 0.5, 10 }));

	/* struc aile gauche |_ */
	a = aa->add(transFactory.getTranslation(0.25,0.5,1.));
	a = a->add(transFactory.getRotationr('x',-M_PI/2));
	a = a->add(transFactory.getScale3c(0.05,0.05,0.5));
	a->add(objFactory.createCylinder((ObjMaterial){Vector4D(1,0,0,0), 0.5, 0.5, 0.5, 10 }));

	/* struc aile gauche _| */
	a = aa->add(transFactory.getTranslation(-0.25,0.5,1.));
	a = a->add(transFactory.getRotationr('x',-M_PI/2));
	a = a->add(transFactory.getScale3c(0.05,0.05,0.5));
	a->add(objFactory.createCylinder((ObjMaterial){Vector4D(1,0,0,0), 0.5, 0.5, 0.5, 10 }));

	/* struc aile gauche \ */
	a = aa->add(transFactory.getTranslation(0,0.5,1.));
	a = a->add(transFactory.getRotationr('x',-M_PI/2));
	a = a->add(transFactory.getRotationr('y', -M_PI/6));
	a = a->add(transFactory.getScale3c(0.05,0.05,0.55));
	a->add(objFactory.createCylinder((ObjMaterial){Vector4D(1,1,1,0), 0.5, 0.5, 0.5, 10 }));

	/* struc aile gauche / */
	a = aa->add(transFactory.getTranslation(0,0.5,1.));
	a = a->add(transFactory.getRotationr('x',-M_PI/2));
	a = a->add(transFactory.getRotationr('y', M_PI/6));
	a = a->add(transFactory.getScale3c(0.05,0.05,0.55));
	a->add(objFactory.createCylinder((ObjMaterial){Vector4D(1,1,1,0), 0.5, 0.5, 0.5, 10 }));

	/* struc aile droite |_ */
	a = aa->add(transFactory.getTranslation(0.25,0.5,-1.));
	a = a->add(transFactory.getRotationr('x',-M_PI/2));
	a = a->add(transFactory.getScale3c(0.05,0.05,0.5));
	a->add(objFactory.createCylinder((ObjMaterial){Vector4D(1,0,0,0), 0.5, 0.5, 0.5, 10 }));

	/* struc aile droite _| */
	a = aa->add(transFactory.getTranslation(-0.25,0.5,-1.));
	a = a->add(transFactory.getRotationr('x',-M_PI/2));
	a = a->add(transFactory.getScale3c(0.05,0.05,0.5));
	a->add(objFactory.createCylinder((ObjMaterial){Vector4D(1,0,0,0), 0.5, 0.5, 0.5, 10 }));

	/* struc aile droite \ */
	a = aa->add(transFactory.getTranslation(0,0.5,-1.));
	a = a->add(transFactory.getRotationr('x',-M_PI/2));
	a = a->add(transFactory.getRotationr('y', -M_PI/6));
	a = a->add(transFactory.getScale3c(0.05,0.05,0.55));
	a->add(objFactory.createCylinder((ObjMaterial){Vector4D(1,1,1,0), 0.5, 0.5, 0.5, 10 }));

	/* struc aile droite / */
	a = aa->add(transFactory.getTranslation(0,0.5,-1.));
	a = a->add(transFactory.getRotationr('x',-M_PI/2));
	a = a->add(transFactory.getRotationr('y', M_PI/6));
	a = a->add(transFactory.getScale3c(0.05,0.05,0.55));
	a->add(objFactory.createCylinder((ObjMaterial){Vector4D(1,1,1,0), 0.5, 0.5, 0.5, 10 }));

	/* noeud de déplacement base Helice */
	Tree *ah = avion->add(transFactory.getTranslation(-1.75,0,0));
	ah = ah->add(transFactory.getRotationd('x',(char*)"a"));

	/* helice */
	a = ah->add(transFactory.getTranslation(-0.05,0,0));
	a = a->add(transFactory.getRotationr('y', M_PI/2));
	a = a->add(transFactory.getScale3c(0.4,2,0.1));
	a = a->add(transFactory.getScale1c(0.15));
	a->add(objFactory.createMobius((ObjMaterial){Vector4D(0.8,0.8,0.8,0), 0.5, 0.5, 0.5, 10 }));

	t = sceneGraph.add(transFactory.getRotationd('y', (char*)"i"));
	t = t->add(transFactory.getRotationr('x',M_PI/6.));
	t = t->add(transFactory.getTranslation((char*)"null",(char*)"j",(char*)"null"));
	t = t->add(transFactory.getScale1c(2.5));
	t = t->add(transFactory.getTranslation(0,0,5));
	t->add(camAvion);
	t->add(avion);

	//sceneGraph.add(avion);
	sceneGraph.add(rep);

	t = sceneGraph.add(transFactory.getTranslation(0, 2, 0));
    t = t->add(transFactory.getScale1c(150));
	t->add(new SkyBox("ressources/skyBox/arr/lf.ppm", "ressources/skyBox/arr/rt.ppm", "ressources/skyBox/arr/up.ppm","ressources/skyBox/arr/dn.ppm", "ressources/skyBox/arr/ft.ppm",
            "ressources/skyBox/arr/bk.ppm", 40));
	
	

	/// INITIALISATION CLASSIQUE OPENGL ...
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel( GL_SMOOTH );
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_FOG);
	glCullFace(GL_BACK);
	glDisable( GL_CULL_FACE );
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glDisable(GL_LIGHTING);

	CHECK_GL;
	glUseProgram(0);
}

void makeRepere(float lg = 1.0) {
	/// Affichage du repere
	glBegin(GL_LINES);
	glColor3f(1.0,0.0,0.0);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(lg,0.0,0.0);
	glColor3f(0.0,1.0,0.0);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(0.0,lg,0.0);
	glColor3f(0.0,0.0,1.0);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(0.0,0.0,lg);
	glEnd();
}

double deg2rad(double deg) {
	return deg*M_PI/180.0;
}

void Interface::drawScene(void) {
	// ******************************* CLEARING ********************************
	glClearColor(0.2,0.2,0.2,0.0);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glUseProgram(idShader);
	{
		if (flagFilaire) glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		else glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

		//Light -> shader
		lights.sendLight(idShader);

		currentCam = camManager.getCurentFPSCamera();
		ShaderTools::sendProjectionMatrix(currentCam->getProjection());
		ShaderTools::sendViewMatrix(currentCam->getViewMatrix());
		ShaderTools::sendModelMatrix(Matrix4D::translation(0,0,0));

		sceneGraph.step();
		sceneGraph.draw(lights);
	}
	glUseProgram(0);
	CHECK_GL;
	glutSwapBuffers();
}
