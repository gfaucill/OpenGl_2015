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

#include <string>
#include <iostream>
#include <cstring>
#include <GL/glew.h>
#include <GL/glut.h>
#include "interface.hpp"

#define width_screen 600
#define height_screen 600

using namespace STP3D;


////////////////////////////           GLOBALS      ////////////////////////////////
Interface* hmi;

////////////////////////////      FUNCTION WRAPPERS      ///////////////////////////
void drawSceneWrapper() {
	 hmi->drawScene();
}

void reshapeWrapper(int w,int h) {
	hmi->reshape(w,h);
}

void mouseWrapper(int button,int state,int x,int y) {
	hmi->mouseFunc(button,state,x,y);
}

void keyboardWrapper(unsigned char c,int x,int y) {
	hmi->kbdFunc(c,x,y);
}

void keyboardSpecialWrapper(int c,int x,int y) {
	hmi->kbdSpFunc(c,x,y);
}

void idleWrapper() {
	hmi->idleFunc();
}


////////////////////////////      MAIN      ///////////////////////////
int main(int argc, char** argv)
{
  // Parse the command line arguments
	for (int i=1;i<argc;i++) {
		if (strcmp(argv[i],"-h") == 0) {
			//help(argv[0]);
			exit(1);
		}
		/*
		if (strcmp(argv[i],"-f") == 0)
			fullscreen = true;
		if (strcmp(argv[i],"-s") == 0)
			solide = true;
		*/
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_STENCIL);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width_screen, height_screen);
	if (glutCreateWindow("STP3D") == GL_FALSE) return 1;
	//if (fullscreen) glutFullScreen();
	glewInit();


	std::cerr<<"Initialisation"<<std::endl;
	hmi = new Interface(width_screen,height_screen);
	hmi->initEngine();
	std::cerr<<"Fin initialisation"<<std::endl;
	
	/* callback for window reshape */
	glutReshapeFunc(reshapeWrapper);
	/* callback for window display */
	glutDisplayFunc(drawSceneWrapper);
	/* callback for mouse clic */
	glutMouseFunc(mouseWrapper);
	/* callback for keyboard event */
	glutKeyboardFunc(keyboardWrapper);
	/* callback for special keyboard event */
	glutSpecialFunc(keyboardSpecialWrapper);
	/* callback when no event */
	glutIdleFunc(idleWrapper);
	/* callback for mouse drag */
	//glutMotionFunc(motionFunc);

	/* Ready to go! */
	glutMainLoop();

	return 0;
}
