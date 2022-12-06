// Local includes
#include "Mouse.h"
#include "Context.h"
// GLUT includes
#include <GL/glut.h>

bool Mouse::mouseMovePressed = false;
bool Mouse::mouseRotatePressed = false;
bool Mouse::mouseZoomPressed = false;
int Mouse::lastX = 0;
int Mouse::lastY = 0;
int Mouse::lastZoom = 0;

void mouse(int button, int state, int x, int y) {
	if (state == GLUT_UP) {
		Mouse::mouseMovePressed = false;
		Mouse::mouseRotatePressed = false;
		Mouse::mouseZoomPressed = false;
	} else {
		if (button == GLUT_LEFT_BUTTON) {
			Context::camera.beginRotate(x, y);
			Mouse::mouseMovePressed = false;
			Mouse::mouseRotatePressed = true;
			Mouse::mouseZoomPressed = false;
			Context::refreshMatrices = true;
		} else if (button == GLUT_RIGHT_BUTTON) {
			Mouse::lastX = x;
			Mouse::lastY = y;
			Mouse::mouseMovePressed = true;
			Mouse::mouseRotatePressed = false;
			Mouse::mouseZoomPressed = false;
		} else if (button == GLUT_MIDDLE_BUTTON) {
			if (Mouse::mouseZoomPressed == false) {
				Mouse::lastZoom = y;
				Mouse::mouseMovePressed = false;
				Mouse::mouseRotatePressed = false;
				Mouse::mouseZoomPressed = true;
			}
		}
	}
	idle();
}

void motion(int x, int y) {
	if (Mouse::mouseRotatePressed == true) {
		Context::camera.rotate(x, y);
		Context::refreshMatrices = true;
	}
	else if (Mouse::mouseMovePressed == true) {
		Context::camera.move((x-Mouse::lastX)/static_cast<float>(SCREENWIDTH), (Mouse::lastY-y)/static_cast<float>(SCREENHEIGHT), 0.0);
		Mouse::lastX = x;
		Mouse::lastY = y;
		Context::refreshMatrices = true;
	}
	else if (Mouse::mouseZoomPressed == true) {
		Context::camera.move(0, 0, (y - Mouse::lastZoom)/static_cast<float>(SCREENHEIGHT));
		// Context::camera.zoom (float (y-Mouse::lastZoom)/SCREENHEIGHT);
		Mouse::lastZoom = y;
		Context::refreshMatrices = true;
	}
}
