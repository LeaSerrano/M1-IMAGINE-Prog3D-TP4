#ifndef CAMERA_H_DEFINED
#define CAMERA_H_DEFINED

#include "Trackball.h"

#include <glm/glm.hpp>

struct Camera {

	Camera ();
	virtual ~Camera () {}

	inline float getFovAngle () const { return fovAngle; }
	inline void setFovAngle (float newFovAngle) { fovAngle = newFovAngle; }
	inline float getAspectRatio () const { return aspectRatio; }
	inline float getNearPlane () const { return nearPlane; }
	inline void setNearPlane (float newNearPlane) { nearPlane = newNearPlane; }
	inline float getFarPlane () const { return farPlane; }
	inline void setFarPlane (float newFarPlane) { farPlane = newFarPlane; }
	inline unsigned int getScreenWidth () const { return W; }
	inline unsigned int getScreenHeight () const { return H; }

	void resize (int W, int H);
	void move (float dx, float dy, float dz);
	void beginRotate (int u, int v);
	void rotate (int u, int v);
	void endRotate ();

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();


	glm::mat4 view;
	glm::mat4 projection;
	// projection
	float fovAngle;
	float aspectRatio;
	float nearPlane;
	float farPlane;
	int H, W;
	// view
	// quaternion based
	// glm::quat current;
	// glm::quat last;
	// look At based view
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 forward;
	glm::vec3 up;

	// handling camera movements
	int spinning, moving;
	int beginu, beginv;
	// float _zoom;
};

#endif // CAMERA_H
