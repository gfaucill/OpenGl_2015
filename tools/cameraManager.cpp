/*
 * cameraManager.cpp
 *
 *  Created on: 22 mai 2014
 *      Author: stokayo
 */

#include "cameraManager.hpp"

CameraManager::CameraManager() : current_index(-1) {
}

Camera* CameraManager::createCamera(STP3D::Vector3D pos_init,
		STP3D::Vector3D dir_init, STP3D::Vector3D top_init,
		STP3D::Vector3D orient_init) {

	/* put the current index on 0 for the first camera creation */
	if (current_index == -1)
		current_index = 0;

	Camera * c = new Camera(pos_init, dir_init, top_init, orient_init);
	cams.push_back(c);
	return c;
}

void CameraManager::nextCamera() {
	if (current_index == -1)
		return;
	current_index++;
	if(current_index == cams.size())
		current_index = 0;
}

void CameraManager::previewCamera() {
	if(current_index == -1)
		return;
	current_index--;
	if(current_index == -1)
		current_index = cams.size() - 1;
}

STP3D::FPSCamera* CameraManager::getCurentFPSCamera() {
	return &cams[current_index]->cam;
}


CameraManager::~CameraManager() {
	for ( std::vector<Camera*>::iterator it = cams.begin(); it != cams.end(); ++it) {
		delete *it;
	}
}

void Camera::move(STP3D::Matrix4D mtx) {

	STP3D::Vector4D newPos = mtx.xPoint(init_position_cam);
	STP3D::Vector3D pos = STP3D::Vector3D(newPos.x, newPos.y, newPos.z);

	STP3D::Vector4D newDir = mtx.xPoint(init_direction_cam);
	STP3D::Vector3D dir = STP3D::Vector3D(newDir.x, newDir.y, newDir.z);

	cam.reinitPosCam(pos,-pos,cam.top_init_cam, cam.orientat_cam);
}
