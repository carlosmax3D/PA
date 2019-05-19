#include <windows.h>
#include <Xinput.h>
#pragma comment (lib, "xinput.lib")
#include <strsafe.h>
#include "resource.h"

#define MY_TIMER 1001

struct CoordCollider {
	int x;
	int y;
};

struct BoxCollider {
	int ancho;
	int alto;
	CoordCollider leftTop;
	CoordCollider leftBottom;
	CoordCollider rightTop;
	CoordCollider rightBottom;
};

//Estructura para el manejo del sprite.
struct SpriteSheet {
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

struct Position {
	int X;
	int Y;
};

struct Objec2D
{
	SpriteSheet sprite;
	Position position;
	BoxCollider collider;
};


struct SpriteBackground {
	HBITMAP handlerImagen;
	HBITMAP handlerMascara;
	BITMAP bitmapImagen;
	BITMAP bitmapMascara;
	int ancho;
	int alto;
};

//SPRITE: Bitmap grafico orientado a ser parte de una escena mas grande.


void LoadSpriteSheet(HINSTANCE, SpriteSheet &, int, int, int, int, int, int);
void LoadSpriteBackground(HINSTANCE, SpriteBackground &, int, int);
void LoadCollider(Objec2D &, int, int);
void DrawSprite(HDC destino, HDC origen, Objec2D &obj2D);
void DrawBackground(HDC destino, HDC origen, int posx, int posy, SpriteBackground imagen);
void Render(HWND pantalla, HDC backbuffer);
void DeleteSpriteSheet(SpriteSheet imagenes);
void DeleteBackground(SpriteBackground imagenes);
void DetectCollision(Objec2D &, Objec2D &);

HINSTANCE hInsta;



SpriteBackground bgMain;
Objec2D player01, player02;

HBITMAP myHSurface;
HBITMAP myHOldSurface;

TCHAR windowName[20] = TEXT("2D Test");

// 2 Handles a device contexts. El Handle al device context nos permite manipular el despliegue de informacion grafica
// orientado a un dispositivo (Contexto). Nos asegura el siempre disponer de un conjunto de funciones para el uso de imagenes.
HDC myHDC;
HDC backBufferDC = 0;

//Tipo de dato propio de la libreria de Xinput para obtener datos de entrada del control de xbox.
XINPUT_STATE state;

//Funcion callback de la ventrana principal.
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void GamePadStatus(HWND, XINPUT_STATE &);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	hInsta = hInstance;
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_CROSS);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = windowName;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, "Fallo al registrar la ventana!", "ERROR", NULL);
		return 1;
	}

	HWND hWnd = CreateWindow(
		windowName,
		windowName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		800, 600,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hWnd)
	{
		MessageBox(NULL, "Fallo la creacion de la ventana!", "ERROR", NULL);
		return 1;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd); // Envia el mensaje WM_PAINT inmediatamente a la ventana y no se espera a refrescar.

						//Asignamos el ciclo que tendra nuestro timer.
	SetTimer(hWnd, MY_TIMER, 1000 / 24, NULL);

	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	//Delete del timer.
	KillTimer(hWnd, MY_TIMER);
	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//Estructura para pintar en un area de cliente siendo la ventana.
	PAINTSTRUCT ps;

	//HANDLE a nuestro DeviceContext
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
	{

		//Carga de Sprites
		LoadSpriteBackground(hInsta, bgMain, IDB_BGMAIN, IDB_BGMAIN);
		LoadSpriteSheet(hInsta, player01.sprite, IDB_PLAYER, IDB_PLAYER, 128, 128, 10, 0);
		LoadSpriteSheet(hInsta, player02.sprite, IDB_PLAYER, IDB_PLAYER, 128, 128, 6, 10);
		player01.position.X = 100;
		player01.position.Y = 400;
		player02.position.X = 400;
		player02.position.Y = 400;
		//Carga de colliders
		LoadCollider(player01, 128, 128);
		LoadCollider(player02, 128, 128);
		//Device context que esta vacio, para que soporte los sprites que vamos a dibujar.
		backBufferDC = CreateCompatibleDC(NULL);

		//Se crea un handler de device context temporal para, a traves de el, obtener el contexto de la ventana creada.
		HDC tempDC = GetDC(hWnd);

		/*Como no se puede crear un device context sin un device, en esta seccion creamos un
		contexto nuevo en memoria basado en el device temporal (tempDC)*/
		myHDC = CreateCompatibleDC(tempDC);

		// Creamos un bitmap en la memoria compatible con el contexto del dispositivo que se obtuvo previamente.
		myHSurface = CreateCompatibleBitmap(tempDC, bgMain.ancho, bgMain.alto);

		// Desocupamos el DC de la ventana y nos quedamos con el que se hizo en la memoria.
		ReleaseDC(hWnd, tempDC);

		/*Con la funcion SelectObject asignamos el bitmap de (myHSurface) al Device Context, pero
		como nada mas puede haber un bitmap o un objeto por device context, se pierde lo que habia antes.
		A menos que lo obtengamos del resultado de la funcion. Por eso lo igualamos a (myHOldSurface).*/
		myHOldSurface = (HBITMAP)SelectObject(myHDC, myHSurface);

		// Seleccionamos una brocha blanca para borrar.
		HBRUSH white = (HBRUSH)GetStockObject(WHITE_BRUSH);
		//Asignamos nuestra brocha blanca al Device Context (myHDC). Y asi como con el Surface, ahora obtenemos la vieja
		//brocha.
		//NOTA: Pueden existir multiples objetos en el device context con la regla de que solo pueden convivir si son
		//de diferente tipo.
		HBRUSH oldBrush = (HBRUSH)SelectObject(myHDC, white);

		//Se crea un rectangulo blanco desde el (0,0) al (Fondo.ancho, Fondo.alto). Todo esto hacia nuestro Device Context.
		Rectangle(myHDC, 0, 0, bgMain.ancho, bgMain.alto);

		// Regresamos la brocha original al DC.
		SelectObject(myHDC, oldBrush);
	}
	break;
	case WM_PAINT:
	{
		BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_TIMER:
	{

		GamePadStatus(hWnd, state);

		if ((XINPUT_GAMEPAD_B & state.Gamepad.wButtons) || GetKeyState(VK_RIGHT))
		{
			if (player01.position.X < 700)
				player01.position.X += 2;
		}

		if (XINPUT_GAMEPAD_X & state.Gamepad.wButtons || GetKeyState(VK_LEFT))
		{
			if (player01.position.X > 100)
				player01.position.X -= 2;
		}

		if (XINPUT_GAMEPAD_Y & state.Gamepad.wButtons || GetKeyState(VK_UP))
		{
			if (player01.position.Y > 2)
				player01.position.Y -= 2;
		}

		if (XINPUT_GAMEPAD_A & state.Gamepad.wButtons || GetKeyState(VK_DOWN))
		{
			if (player01.position.Y < 500)
				player01.position.Y += 2;
		}

		DrawBackground(myHDC, backBufferDC, 0, 0, bgMain);
		DrawSprite(myHDC, backBufferDC, player01);
		DrawSprite(myHDC, backBufferDC, player02);

		DetectCollision(player01, player02);

		Render(hWnd, myHDC);

	}
	break;
	case WM_DESTROY:
		DeleteBackground(bgMain);
		DeleteSpriteSheet(player01.sprite);
		DeleteSpriteSheet(player02.sprite);

		DeleteObject(myHSurface);
		DeleteDC(myHDC);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

void GamePadStatus(HWND hWnd, XINPUT_STATE &stata)
{

	DWORD dwResult;


	ZeroMemory(&stata, sizeof(XINPUT_STATE));

	// Simply get the state of the controller from XInput.
	dwResult = XInputGetState(0, &stata);

	if (dwResult == ERROR_SUCCESS)
	{
		//MessageBox(hWnd, "Controlador conectado", "Funciono", 0);
	}
	else
	{
		//MessageBox(hWnd, "Controlador no conectado", "No Funciono", 0);
	}

}

void Render(HWND ventana, HDC myDC)
{
	//Obtenemos el device Context de la ventana mostrada.
	HDC hWndDC = GetDC(ventana);

	//Pasamos de nuestro device Context en memoria hacia el device Context habilitado en la ventana.
	BitBlt(hWndDC, 0, 0, 800, 600, myDC, 0, 0, SRCCOPY);

	// Siempre hay que liberar el device context que no nos pertenece.
	ReleaseDC(ventana, hWndDC);
}
void DeleteSpriteSheet(SpriteSheet imagenes)
{
	DeleteObject(imagenes.handlerImagen);
	DeleteObject(imagenes.handlerMascara);
}
void DeleteBackground(SpriteBackground imagenes)
{
	DeleteObject(imagenes.handlerImagen);
	DeleteObject(imagenes.handlerMascara);
}
void DrawSprite(HDC destino, HDC backBuffer, Objec2D &obj2D)
{
	int framesX = obj2D.sprite.ancho / obj2D.sprite.anchoFrame;
	int framesY = obj2D.sprite.alto / obj2D.sprite.altoFrame;

	int columnaActual = (obj2D.sprite.frameActual + obj2D.sprite.frameInicial) % framesX;
	int filaActual = (obj2D.sprite.frameActual + obj2D.sprite.frameInicial) / framesY;

	int coordX = obj2D.sprite.anchoFrame * columnaActual;
	int coordY = obj2D.sprite.altoFrame * filaActual;

	//Le pasamos la imagen o sprite al DC backBuffer en memoria y obtenemos el vacio que creamos al inicio.
	HGDIOBJ oldObj = SelectObject(backBuffer, obj2D.sprite.handlerMascara);

	//Ahora del backBuffer pasamos los datos al Device Context en memoria
	//el SRCAND es un and con el source del fondo que tiene el Device Context.
	BitBlt(destino, obj2D.position.X, obj2D.position.Y, obj2D.sprite.anchoFrame, obj2D.sprite.altoFrame, backBuffer, coordX, coordY, SRCAND);
	//Aqui hacemos lo mismo que arriba pero ahora SRCPAINT, esto es equivalente al or.
	SelectObject(backBuffer, obj2D.sprite.handlerImagen);
	BitBlt(destino, obj2D.position.X, obj2D.position.Y, obj2D.sprite.anchoFrame, obj2D.sprite.altoFrame, backBuffer, coordX, coordY, SRCPAINT);

	// Restauramos al viejo bitmap asociado
	SelectObject(backBuffer, oldObj);
	obj2D.sprite.frameActual++;
	if (obj2D.sprite.frameActual >= obj2D.sprite.framesAnimacion)
		obj2D.sprite.frameActual = 0;
}

void DrawBackground(HDC destino, HDC backBuffer, int posx, int posy, SpriteBackground imagen)
{
	//Le pasamos la imagen o sprite al DC backBuffer en memoria y obtenemos el vacio que creamos al inicio.
	HGDIOBJ oldObj = SelectObject(backBuffer, imagen.handlerMascara);

	//Ahora del backBuffer pasamos los datos al Device Context en memoria
	//el SRCAND es un and con el source del fondo que tiene el Device Context.
	BitBlt(destino, posx, posy, imagen.ancho, imagen.alto, backBuffer, 0, 0, SRCAND);
	//Aqui hacemos lo mismo que arriba pero ahora SRCPAINT, esto es equivalente al or.
	SelectObject(backBuffer, imagen.handlerImagen);
	BitBlt(destino, posx, posy, imagen.ancho, imagen.alto, backBuffer, 0, 0, SRCPAINT);

	// Restauramos al viejo bitmap asociado
	SelectObject(backBuffer, oldObj);
}

//Guarda en memoria las imagenes a cargar. Se utiliza punteros para el paso por referencia.
void LoadSpriteSheet(HINSTANCE hInstancia, SpriteSheet &imagenes, int imagenId, int mascaraiD, int hF, int wF, int framesAnim, int frameInit)
{
	imagenes.framesAnimacion = framesAnim;
	imagenes.altoFrame = hF;
	imagenes.anchoFrame = wF;
	imagenes.frameInicial = frameInit;
	// Obtiene los HBITMAP. Los parametros son el puntero de la aplicacion y el MAKEINTRESOURCE Con el ID de dicha imagen.
	imagenes.handlerImagen = LoadBitmap(hInstancia, MAKEINTRESOURCE(imagenId));
	imagenes.handlerMascara = LoadBitmap(hInstancia, MAKEINTRESOURCE(mascaraiD));

	//Por medio del HBTIMAP carga las imagenes a un BITMAP.
	GetObject(imagenes.handlerImagen, sizeof(BITMAP), &imagenes.bitmapImagen);
	GetObject(imagenes.handlerMascara, sizeof(BITMAP), &imagenes.bitmapMascara);

	//Gracias a todo este proceso obtenemos los tamaños para asignarlos a nuestra estructura y que sea mas entendible.
	imagenes.ancho = imagenes.bitmapImagen.bmWidth;
	imagenes.alto = imagenes.bitmapImagen.bmHeight;
}

void LoadSpriteBackground(HINSTANCE hInstancia, SpriteBackground &imagenes, int imagenId, int mascaraiD)
{
	// Obtiene los HBITMAP. Los parametros son el puntero de la aplicacion y el MAKEINTRESOURCE Con el ID de dicha imagen.
	imagenes.handlerImagen = LoadBitmap(hInstancia, MAKEINTRESOURCE(imagenId));
	imagenes.handlerMascara = LoadBitmap(hInstancia, MAKEINTRESOURCE(mascaraiD));

	//Por medio del HBTIMAP carga las imagenes a un BITMAP.
	GetObject(imagenes.handlerImagen, sizeof(BITMAP), &imagenes.bitmapImagen);
	GetObject(imagenes.handlerMascara, sizeof(BITMAP), &imagenes.bitmapMascara);

	//Gracias a todo este proceso obtenemos los tamaños para asignarlos a nuestra estructura y que sea mas entendible.
	imagenes.ancho = imagenes.bitmapImagen.bmWidth;
	imagenes.alto = imagenes.bitmapImagen.bmHeight;
}

void LoadCollider(Objec2D &obj, int ancho, int alto)
{
	obj.collider.alto = alto;
	obj.collider.ancho = ancho;
}
int flagCollider = 0;
void DetectCollision(Objec2D &obj1, Objec2D &obj2)
{
	/*
	//Collider 1
	obj1.collider.leftTop.x = obj1.position.X;
	obj1.collider.leftTop.y = obj1.position.Y;

	obj1.collider.rightTop.x = obj1.position.X + obj1.collider.ancho;
	obj1.collider.rightTop.y = obj1.position.Y;

	obj1.collider.leftBottom.x = obj1.position.X;
	obj1.collider.leftBottom.y = obj1.position.Y + obj1.collider.alto;

	obj1.collider.rightBottom.x = obj1.position.X + obj1.collider.ancho;
	obj1.collider.rightBottom.y = obj1.position.Y + obj1.collider.alto;
	//Collider 2
	obj2.collider.leftTop.x = obj2.position.X;
	obj2.collider.leftTop.y = obj2.position.Y;

	obj2.collider.rightTop.x = obj2.position.X + obj2.collider.ancho;
	obj2.collider.rightTop.y = obj2.position.Y;

	obj2.collider.leftBottom.x = obj2.position.X;
	obj2.collider.leftBottom.y = obj2.position.Y + obj2.collider.alto;

	obj2.collider.rightBottom.x = obj2.position.X + obj2.collider.ancho;
	obj2.collider.rightBottom.y = obj2.position.Y + obj2.collider.alto;

	if ((obj1.collider.leftTop.x < obj2.collider.leftTop.x &&
		obj1.collider.rightBottom.x < obj2.collider.rightBottom.x) &&
		(obj1.collider.leftTop.y < obj2.collider.leftTop.y &&
			obj1.collider.rightBottom.y < obj2.collider.rightBottom.y))
	{
		//flagCollider = 1;
	}
	*/
	//Metodo AABB
	if (obj1.position.X < obj2.position.X + obj2.collider.ancho &&
		obj1.position.X + obj1.collider.ancho > obj2.position.X &&
		obj1.position.Y < obj2.position.Y + obj2.collider.alto &&
		obj1.position.Y + obj1.collider.alto > obj2.position.Y
		)
	{
		//Agregar a vinculador
		//A entrada
		//winmm.lib
		obj1.position.X -= 10;
		flagCollider = 1;
		//PlaySound(TEXT("SystemStart"), NULL, SND_ALIAS);
		//PlaySound(NULL, 0, 0);
		//flagCollider = 1;
	}

	if (flagCollider == 1)
	{
		flagCollider = 0;
		//PlaySound(TEXT("sound.wav"), NULL, SND_FILENAME);
		//MessageBox(NULL, "Mandado a llamar", "Collider", NULL);
	}
}