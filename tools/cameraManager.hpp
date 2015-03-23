/*
 * cameraManager.hpp
 *
 *  Created on: 22 mai 2014
 *      Author: stokayo
 */

#ifndef CAMERAMANAGER_HPP_
#define CAMERAMANAGER_HPP_

#include "vector3d.hpp"
#include "camera.hpp"
#include <vector>

class Camera;

/*
 * This class allow to create cameras and move it in a scene graph
 */
class CameraManager {
public:
	/*
	 * Create an empty cameraManager with no cam
	 */
	CameraManager();

	/*
	 * Create a new Camera to put in the sceneGraph
	 */
	Camera * createCamera(STP3D::Vector3D pos_init, STP3D::Vector3D dir_init,
			STP3D::Vector3D top_init = STP3D::Vector3D(0.0,1.0,0.0),
			STP3D::Vector3D orient_init = STP3D::Vector3D());

	/*
	 * Switch to the next camera
	 */
	void nextCamera();

	/*
	 * Switch to the previous camera
	 */
	void previewCamera();

	/*
	 * Return the current selected Camera or NULL if there is no cam
	 */
	STP3D::FPSCamera * getCurentFPSCamera();

	/*
	 * Destructor
	 */
	virtual ~CameraManager();

private:
	std::vector<Camera*> cams;
	int current_index;
};

/*
 * The class is only a data structure to represent a camera in the scene graph
 */
class Camera {
public:
	Camera();
	Camera(STP3D::Vector3D init_position_cam, STP3D::Vector3D init_dir_cam,
	STP3D::Vector3D top_init, STP3D::Vector3D orient_init) :
		cam(init_position_cam, init_dir_cam, top_init, orient_init)
	{
		this->init_position_cam = init_position_cam;
		this->init_direction_cam = init_direction_cam;
	}

	/*
	 * Should be call in the scene graph to move camera
	 * mtx represent the current view matrix when you run through the sceneGraph
	 */
	void move(STP3D::Matrix4D mtx);

	STP3D::FPSCamera cam;
private :
	STP3D::Vector3D init_position_cam;
	STP3D::Vector3D init_direction_cam;
};

#endif /* CAMERAMANAGER_HPP_ */
