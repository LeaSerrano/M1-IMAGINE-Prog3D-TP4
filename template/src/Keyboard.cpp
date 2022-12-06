#include "Keyboard.h"
#include "Context.h"
// GLUT includes
#include <GL/glut.h>

void key (unsigned char keyPressed, int x, int y) {
	switch (keyPressed) {
		case 'f':
		if (Context::fullScreen == true) {
			glutReshapeWindow (SCREENWIDTH, SCREENHEIGHT);
			Context::fullScreen = false;
		} else {
			glutFullScreen ();
			Context::fullScreen = true;
		}
		break;


		case 'w':
		GLint polygonMode[2];
		glGetIntegerv(GL_POLYGON_MODE, polygonMode);
		if(polygonMode[0] != GL_FILL)
		glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
		else
		glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
		break;


		case 'n': //Press n key to display normals
		// display_normals = !display_normals;
		break;


		case '+': //Press + key to increase scale
		// scale += 0.005;
		break;

		case '-': //Press - key to decrease scale
		// scale -= 0.005;
		break;

		case 'd': //Press d key to translate on x positive
		// translate[0] += 0.005;
		break;

		case 'q': //Press q key to translate on q negative
		// translate[0] -= 0.005;
		break;

		case 'z': //Press z key to translate on z positive
		// translate[1] += 0.005;
		break;

		case 's': //Press s key to translate on y negative
		// translate[1] -= 0.005;
		break;

		case '1': //Toggle loaded mesh display
		// display_loaded_mesh = !display_loaded_mesh;
		break;

		default:
		break;
	}
	idle ();
}
