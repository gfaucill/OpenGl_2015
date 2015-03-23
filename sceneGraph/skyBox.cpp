/*
 * SkyBox.cpp
 */

#include "skyBox.hpp"

SkyBox::SkyBox(const char* left, const char* right, const char* up,
		const char* down, const char* front, const char* back, float size) :
		cubeMap(left,right,up,down,front,back)
{
	cubeMap.initTexture();
    cube = STP3D::meshCube(size,size);
	cube->createVAO();
	cube->releaseCPUMemory();
}

void SkyBox::load() {
	cubeMap.loadTexture();
	int id; glGetIntegerv(GL_CURRENT_PROGRAM, &id);
	glUniform1i(glGetUniformLocation(id, CUBE_MAP_TEXTURE_UNIFORM), 0);
	glUniform1i(glGetUniformLocation(id, CUBE_MAP_ACTIVE_UNIFORM), 1);
}

void SkyBox::unload() {
	cubeMap.unloadTexture();
	int id; glGetIntegerv(GL_CURRENT_PROGRAM, &id);
	glUniform1i(glGetUniformLocation(id, CUBE_MAP_ACTIVE_UNIFORM), 0);
}

void SkyBox::draw() {
	cube->draw();
}

SkyBox::~SkyBox() {
	delete cube;
}
