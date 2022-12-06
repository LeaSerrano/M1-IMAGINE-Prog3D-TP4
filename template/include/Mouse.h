#ifndef MOUSE_H_DEFINED
#define MOUSE_H_DEFINED


struct Mouse {
	static bool mouseMovePressed;
	static bool mouseRotatePressed;
	static bool mouseZoomPressed;
	static int lastX;
	static int lastY;
	static int lastZoom;
};

void motion (int x, int y);

void mouse (int button, int state, int x, int y);

#endif
