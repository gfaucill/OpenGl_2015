/*
 * SkyBox.hpp
 */

#ifndef SKYBOX_HPP_
#define SKYBOX_HPP_

#include "cubemaptexture.h"
#include "../tools/indexed_mesh.hpp"
#include "../tools/basic_mesh.hpp"


#define CUBE_MAP_TEXTURE_UNIFORM "cubeMapTexture"
#define CUBE_MAP_ACTIVE_UNIFORM "skyboxActive"

class SkyBox {
public:
	SkyBox(const char * left, const char * right, const char * up, const char * down, const char * front,
			const char * back, float size = 100.);

	void load();
	void unload();
	void draw();

	virtual ~SkyBox();

private:
	CubemapTexture cubeMap;
    STP3D::IndexedMesh * cube;
};

#endif /* SKYBOX_HPP_ */
