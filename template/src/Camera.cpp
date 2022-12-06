// **************************************************
// GLRK : the OpenGL Refinement Kernel.
// Camera.
// This file is a part of the GLRK library.
// Author : Tamy Boubekeur (Tamy.Boubekeur@labri.fr).
// Copyright (C) 2006 Tamy Boubekeur.
// All rights reserved.
// **************************************************

#include "Camera.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>

#include <glm/gtc/quaternion.hpp>
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective

using namespace std;

static int _spinning, _moving;
static int _beginu, _beginv;
static float _curquat[4];

Camera::Camera () {

	fovAngle = 65.0;
	aspectRatio = 1.0;
	nearPlane = 0.001;
	farPlane = 10000.0;
	projection = getProjectionMatrix();

	position = glm::vec3(1, 0, 0);
	forward = glm::vec3(0) - position;
	forward = glm::normalize(forward);
	up = glm::vec3(0, -1, 0);
	view = getViewMatrix();
}


void Camera::resize (int _W, int _H) {
	H = _H;
	W = _W;
	aspectRatio = (float)W /(float)H;
	glViewport (0, 0, (GLint)W, (GLint)H);
	projection = getProjectionMatrix();
}

void Camera::move (float dx, float dy, float dz) {
	// We change it for a orbit camera (instead of freefly)
	// upgrade it with beginRotate for quaternion to avoid caveat of euler angles
	glm::vec3 right = glm::cross(forward, up);
	glm::vec3 tmpUp = glm::cross(forward, right);
	position += dy * tmpUp - right * dx + forward * dz;
	// we update forward with new position of camera
	forward = glm::normalize(target - position);
}


void Camera::beginRotate (int u, int v) {
	beginu = u;
	beginv = v;
	moving = 1;
	spinning = 0;
}


void Camera::rotate (int u, int v) {
	if (moving) {
		// trackball(lastquat,
		// 	(2.0 * beginu - W) / W,
		// 	(H - 2.0 * beginv) / H,
		// 	(2.0 * u - W) / W,
		// 	(H - 2.0 * v) / H);
		// 	beginu = u;
		// 	beginv = v;
		// 	spinning = 1;
		// 	add_quats (lastquat, curquat, curquat);
		}
}


void Camera::endRotate () {
	moving = false;
}

// convert lookAt into quaternion
// void Camera::lookAt(glm::vec3 target) {
// 	glm::mat4 lookMat = glm::lookAt(position, position + forward, up);
// 	current = glm::conjugate(glm::quat_cast(lookMat));
// }

glm::mat4 Camera::getViewMatrix() {
	// using quaternion current
	// view = glm::mat4_cast(current) * glm::translate(glm::mat4(1), position);
	view = glm::lookAt(position, position + forward, up);
	return view;
}

glm::mat4 Camera::getProjectionMatrix() {
	projection = glm::perspective(
		glm::radians(fovAngle),
		aspectRatio,
		nearPlane,
		farPlane
	);
	return projection;
}
