#include <windows.h>
#include <Xinput.h>
#pragma comment (lib, "xinput.lib")
#include <strsafe.h>
#include "resource.h"

#define MY_TIMER 1001

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
void DrawSprite(HDC destino, HDC origen, int posx, int posy, SpriteSheet &imagen);
void DrawBackground(HDC destino, HDC origen, int posx, int posy, SpriteBackground imagen);
void Render(HWND pantalla, HDC backbuffer);
void DeleteSpriteSheet(SpriteSheet imagenes);
void DeleteBackground(SpriteBackground imagenes);

HINSTANCE hInsta;



SpriteBackground bgMain;
SpriteSheet player;

HBITMAP myHSurface;
HBITMAP myHOldSurface;

TCHAR windowName[20] = TEXT("2D Test");

// 2 Handles a device contexts. El Handle al device context nos permite manipular el despliegue de informacion grafica
// orientado a un dispositivo (Contexto). Nos asegura el siempre disponer de un conjunto de funciones para el uso de imagenes.
HDC myHDC;
HDC backBufferDC = 0;


int planeX = 100, planeY = 400; //Posiciones de personaje

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
	SetTimer(hWnd, MY_TIMER, 1000/24, NULL);

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
		LoadSpriteSheet(hInsta, player, IDB_PLAYER, IDB_TYRMASK, 128, 128, 10, 0);


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

		if ((XINPUT_GAMEPAD_B & state.Gamepad.wButtons))
		{
			if (planeX < 700)
				planeX += 2;
		}

		if (XINPUT_GAMEPAD_X & state.Gamepad.wButtons)
		{
			if (planeX > 100)
				planeX -= 2;
		}

		if (XINPUT_GAMEPAD_Y & state.Gamepad.wButtons || GetKeyState(VK_UP))
		{
			if (planeY > 2)
				planeY -= 2;
		}

		if (XINPUT_GAMEPAD_A & state.Gamepad.wButtons)
		{
			if (planeY < 500)
				planeY += 2;
		}

		DrawBackground(myHDC, backBufferDC, 0, 0, bgMain);
		DrawSprite(myHDC, backBufferDC, planeX, planeY, player);

		Render(hWnd, myHDC);

	}
	break;
	case WM_DESTROY:
		DeleteBackground(bgMain);
		DeleteSpriteSheet(player);

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
void DrawSprite(HDC destino, HDC backBuffer, int posx, int posy, SpriteSheet &imagen)
{
	int framesX = imagen.ancho / imagen.anchoFrame;
	int framesY = imagen.alto / imagen.altoFrame;

	int columnaActual = (imagen.frameActual + imagen.frameInicial) % framesX;
	int filaActual = (imagen.frameActual + imagen.frameInicial) / framesY;

	int coordX = imagen.anchoFrame * columnaActual;
	int coordY = imagen.altoFrame * filaActual;

	//Le pasamos la imagen o sprite al DC backBuffer en memoria y obtenemos el vacio que creamos al inicio.
	HGDIOBJ oldObj = SelectObject(backBuffer, imagen.handlerMascara);

	//Ahora del backBuffer pasamos los datos al Device Context en memoria
	//el SRCAND es un and con el source del fondo que tiene el Device Context.
	BitBlt(destino, posx, posy, imagen.anchoFrame, imagen.altoFrame, backBuffer, coordX, coordY, SRCAND);
	//Aqui hacemos lo mismo que arriba pero ahora SRCPAINT, esto es equivalente al or.
	SelectObject(backBuffer, imagen.handlerImagen);
	BitBlt(destino, posx, posy, imagen.anchoFrame, imagen.altoFrame, backBuffer, coordX, coordY, SRCPAINT);

	// Restauramos al viejo bitmap asociado
	SelectObject(backBuffer, oldObj);
	imagen.frameActual++;
	if (imagen.frameActual >= imagen.framesAnimacion)
		imagen.frameActual = 0;
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