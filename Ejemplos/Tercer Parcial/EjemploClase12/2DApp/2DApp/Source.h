#include <windows.h>
#include <Xinput.h>
#include <strsafe.h>
#include "resource.h"

extern struct Sprite {
	HBITMAP handlerImagen;
	HBITMAP handlerMascara;
	BITMAP bitmapImagen;
	BITMAP bitmapMascara;
	int ancho;
	int alto;
};

extern struct CoordCollider {
	int x;
	int y;
};

extern struct BoxCollider {
	int ancho;
	int alto;
	CoordCollider leftTop;
	CoordCollider leftBottom;
	CoordCollider rightTop;
	CoordCollider rightBottom;
};

//Estructura para el manejo del sprite.
extern struct SpriteSheet {
	HBITMAP handlerImagen;
	HBITMAP handlerMascara;
	BITMAP bitmapImagen;
	BITMAP bitmapMascara;
	int ancho;
	int alto;
	int anchoFrame;
	int altoFrame;
	int framesAnimacion;
	int frameInicial;
	int frameActual;
};

extern struct Position {
	int X;
	int Y;
};

extern struct Objec2D
{
	SpriteSheet sprite;
	Position position;
	BoxCollider collider;
};


extern struct SpriteBackground {
	HBITMAP handlerImagen;
	HBITMAP handlerMascara;
	BITMAP bitmapImagen;
	BITMAP bitmapMascara;
	int ancho;
	int alto;
};


extern SpriteBackground bgMain;
extern Objec2D player01, player02;
