#include "framework.h"
#include "WindowsAPI.h"
#include <string>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // ID de programa (instancia)
WCHAR szTitle[MAX_LOADSTRING];                  // Titulo del programa
WCHAR szWindowClass[MAX_LOADSTRING];            // Nombre de la clase de la ventana principal
HWND hWndEdit;
HWND modal;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About1(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
std::wstring s2ws(const std::string& s);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Carga los titulos con los string en resource
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSAPI, szWindowClass, MAX_LOADSTRING);
    // Se asignan propiedades a la pantalla principal
    MyRegisterClass(hInstance);

    // Se despliega la ventana principal
    if (!InitInstance (hInstance, nCmdShow)){
        return FALSE;
    }

    // Se toma la lista de accesos directos creada en resource.h
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSAPI));
    // Variable para capturar mensajes
    MSG msg;

    // Bucle principal para leer mensajes de pantalla
    while (GetMessage(&msg, nullptr, 0, 0)){
        // Verifica si el mensaje es un acceso directo, si lo es lo traduce y ejecuta
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance){
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;  // Funcion de callback para la ventana
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSAPI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSAPI);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(
       szWindowClass, 
       szTitle, 
       WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT, // Position X
       0,             // Position Y
       CW_USEDEFAULT, 
       0, 
       nullptr, 
       nullptr, 
       hInstance, 
       nullptr);

   if (!hWnd) {
      return FALSE;
   }

   HWND hwndButton = CreateWindowW(
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"OK JEJE",      // Button text 
       WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
       10,         // x position 
       10,         // y position 
       100,        // Button width
       100,        // Button height
       hWnd,       // Parent window
       (HMENU)1002,       // No menu. NULL
       (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   // Creamos un textbox para capturar texto
   hWndEdit = CreateWindowEx(
       WS_EX_CLIENTEDGE, 
       TEXT("Edit"), 
       TEXT("test asdas as"),
       WS_CHILD | WS_VISIBLE, 
       200, 20, 
       255,
       20, hWnd, (HMENU)1003, NULL, NULL);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            std::string swmId = std::to_string(wmId);
            MessageBox(hWnd, s2ws(swmId).c_str(), TEXT(""), 0);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);

                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case 1002: {
                int len = GetWindowTextLengthW(GetDlgItem(hWnd, 1003)) + 1;
                wchar_t text[MAX_LOADSTRING];
                wchar_t text1[MAX_LOADSTRING];
                GetWindowTextW(GetDlgItem(hWnd, 1003), text, len);
                //MessageBoxW(NULL, text, L"MsgFromTextBox s", MB_OK);

                len = GetWindowTextLengthW(hWndEdit) + 1;
                GetWindowText(hWndEdit, &text1[0], len);
                //MessageBoxW(NULL, text1, L"MsgFromTextBox s", MB_OK);
                int msgboxID = MessageBox(NULL, L"DOCTOR WHO", L"BBC", MB_OKCANCEL | MB_DEFBUTTON2);
                swmId = std::to_string(msgboxID);
                //MessageBox(hWnd, s2ws(swmId).c_str(), TEXT(""), 0);
                modal = CreateDialogW(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, About1);
                //modal = CreateDialogW(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About1);
                ShowWindow(modal, SW_SHOW);
                UpdateWindow(modal);
            }
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
// Message handler for about box.
INT_PTR CALLBACK About1(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        int wmId = LOWORD(wParam);
        std::string swmId = std::to_string(wmId);
        MessageBox(hDlg, s2ws(swmId).c_str(), TEXT(""), 0);
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

std::wstring s2ws(const std::string& s){
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}