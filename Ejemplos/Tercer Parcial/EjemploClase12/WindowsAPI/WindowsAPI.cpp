#include "framework.h"
#include "WindowsAPI.h"
#include <string>
#include <oleauto.h>
#include <commdlg.h>
#include <commctrl.h>
#include <iostream>
#include <sstream>

#define MAX_LOADSTRING 100
#if defined _WIN32 || defined __CYGWIN__
#define PATH_SEPARATOR '\\'
#else
#define PATH_SEPARATOR "/"
#endif
// Global Variables:
HINSTANCE hInst;                                // ID de programa (instancia)
WCHAR szTitle[MAX_LOADSTRING];                  // Titulo del programa
WCHAR szWindowClass[MAX_LOADSTRING];            // Nombre de la clase de la ventana principal
HWND hWndEdit;
HWND modal;
HWND ventanaPrincipal;
#define IDT_TIMER1 2021
std::wstring valor1(L"");
std::wstring valor2(L"");
std::wstring operando(L"");

//Declaramos la estructura de nuestros nodos
struct NODO {
    std::string nombre;
    int edad;
    float peso;
    double fechaNacimiento;
    struct NODO* siguiente; // un apuntador del tipo NODO para ligar la lista
}*HEAD;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    callbackIDD_DIALOG1(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK    callbackIDD_DIALOG2(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK    callbackIDD_DIALOG3(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK    callbackIDD_DIALOG4(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK    callbackIDD_DIALOG5(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
std::wstring s2ws(const std::string& s);
void removeTimeFromSystemTime(SYSTEMTIME* date);
std::wstring valoresCalculadora(HWND hDlg, std::wstring number);
int onEditChange(HWND hDlg, int ctrlID, int type);
bool isFloat(std::wstring str);
bool isInt(std::wstring str);
SYSTEMTIME convertStringToSystemTime(char* dateTimeString);
// Definicion de funciones para hacer operaciones con la lista
struct NODO* nuevoNodo(std::string nombre, int edad, float peso, double fechaNacimiento);
struct NODO* agregarEnMedioDeLista(struct NODO** HEAD, std::string nombre, int edad, float peso, double fechaNacimiento, std::string buscarNombre);
struct NODO* agregarAlFinalDeLista(struct NODO** HEAD, std::string nombre, int edad, float peso, double fechaNacimiento);
struct NODO* agregarAElInicioDeLista(struct NODO** HEAD, std::string nombre, int edad, float peso, double fechaNacimiento);
void imprimirLista(struct NODO* lista);
struct NODO* modificarNodoDeLista(struct NODO** HEAD, std::string nombre, int edad, float peso, double fechaNacimiento, std::string buscarNombre);
void eliminarNodoMedioDeLista(struct NODO** HEAD, std::string buscarNombre);
void eliminarNodoInicioDeLista(struct NODO** HEAD);
void eliminarNodoAlFinalDeLista(struct NODO** HEAD);
struct NODO* ordenarPersonasPorFechaLista(struct NODO** head, int orden);
void abrirArchivo(struct NODO** lista);
void guardarArchivo(struct NODO* lista);
void leerArchivoTellg(struct NODO** lista);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
    HEAD = NULL;
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

   ventanaPrincipal = CreateWindowW(
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

   if (!ventanaPrincipal) {
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
       ventanaPrincipal,       // Parent window
       (HMENU)1002,       // No menu. NULL
       (HINSTANCE)GetWindowLongPtr(ventanaPrincipal, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   HWND hwndButton2 = CreateWindowW(
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"Nuevo Dialog",      // Button text 
       WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
       150,         // x position 
       150,         // y position 
       100,        // Button width
       100,        // Button height
       ventanaPrincipal,       // Parent window
       (HMENU)1004,       // No menu. NULL
       (HINSTANCE)GetWindowLongPtr(ventanaPrincipal, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   HWND hwndButton3 = CreateWindowW(
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"Calculadora",      // Button text 
       WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
       300,         // x position 
       300,         // y position 
       100,        // Button width
       50,        // Button height
       ventanaPrincipal,       // Parent window
       (HMENU)1005,       // No menu. NULL
       (HINSTANCE)GetWindowLongPtr(ventanaPrincipal, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.
   HWND hwndButton4 = CreateWindowW(
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"UnionAvances",      // Button text 
       WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
       500,         // x position 
       300,         // y position 
       100,        // Button width
       50,        // Button height
       ventanaPrincipal,       // Parent window
       (HMENU)1006,       // No menu. NULL
       (HINSTANCE)GetWindowLongPtr(ventanaPrincipal, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.
   HWND hwndButton5 = CreateWindowW(
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"Mostrar Nodos",      // Button text 
       WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
       500,         // x position 
       150,         // y position 
       100,        // Button width
       50,        // Button height
       ventanaPrincipal,       // Parent window
       (HMENU)1007,       // No menu. NULL
       (HINSTANCE)GetWindowLongPtr(ventanaPrincipal, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   // Creamos un textbox para capturar texto
   hWndEdit = CreateWindowEx(
       WS_EX_CLIENTEDGE, 
       TEXT("Edit"), 
       TEXT("test asdas as"),
       WS_CHILD | WS_VISIBLE, 
       200, 20, 
       255,
       20, ventanaPrincipal, (HMENU)1003, NULL, NULL);
   ShowWindow(ventanaPrincipal, nCmdShow);
   UpdateWindow(ventanaPrincipal);

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
    case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            //std::string swmId = std::to_string(wmId);
            //MessageBox(hWnd, s2ws(swmId).c_str(), TEXT(""), 0);
            // Parse the menu selections:
            switch (wmId) {
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
                SetWindowTextW(GetDlgItem(hWnd, 1003), L"ESTE ES UN CAMBIO");
                //MessageBoxW(NULL, text, L"MsgFromTextBox s", MB_OK);
                modal = CreateDialogW(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, callbackIDD_DIALOG1);
                ShowWindow(modal, SW_SHOW);
                UpdateWindow(modal);
                break;
            }
            case 1004:
                //MessageBoxW(NULL, text, L"MsgFromTextBox s", MB_OK);
                modal = CreateDialogW(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, callbackIDD_DIALOG2);
                SetTimer(modal, IDT_TIMER1, 1000, (TIMERPROC)callbackIDD_DIALOG2);
                ShowWindow(modal, SW_SHOW);
                UpdateWindow(modal);
                break;
            case 1005:
                //MessageBoxW(NULL, text, L"MsgFromTextBox s", MB_OK);
                modal = CreateDialogW(hInst, MAKEINTRESOURCE(IDD_DIALOG3), hWnd, callbackIDD_DIALOG3);
                ShowWindow(modal, SW_SHOW);
                UpdateWindow(modal);
                break;
            case 1006:
                //MessageBoxW(NULL, text, L"MsgFromTextBox s", MB_OK);
                modal = CreateDialogW(hInst, MAKEINTRESOURCE(IDD_DIALOG4), hWnd, callbackIDD_DIALOG4);
                ShowWindow(modal, SW_SHOW);
                UpdateWindow(modal);
                break;
            case 1007:
                //MessageBoxW(NULL, text, L"MsgFromTextBox s", MB_OK);
                modal = CreateDialogW(hInst, MAKEINTRESOURCE(IDD_DIALOG5), hWnd, callbackIDD_DIALOG5);
                ShowWindow(modal, SW_SHOW);
                UpdateWindow(modal);
                break;
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
// Message handler for IDD_DIALOG1 box.
INT_PTR CALLBACK callbackIDD_DIALOG1(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        int wmId = LOWORD(wParam);
        std::string swmId = std::to_string(wmId);
        //MessageBox(hDlg, s2ws(swmId).c_str(), TEXT(""), 0);
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        SYSTEMTIME t;
        TCHAR buf[200];
        switch (LOWORD(wParam)) {
        case IDC_BUTTON1:
            GetWindowTextW(GetDlgItem(hDlg, IDC_EDIT1), buf, 200);
            SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_INSERTSTRING, 0, (LPARAM)buf);
            break;
        case IDC_BUTTON2:
            DateTime_GetSystemtime(GetDlgItem(hDlg, IDC_DATETIMEPICKER1), &t);
            GetDateFormat(LOCALE_USER_DEFAULT, DATE_LONGDATE, &t, NULL, buf, 200);
            MessageBox(hDlg, buf, TEXT(""), 0);
            t = convertStringToSystemTime((char*)std::string("2020-04-05 19:30").c_str());
            DateTime_SetSystemtime(GetDlgItem(hDlg, IDC_DATETIMEPICKER1), GDT_VALID, &t);
            break;
        case IDC_BUTTON3: {
            int i = SendMessage(GetDlgItem(hDlg, IDC_LIST1), LB_GETCURSEL, 0, 0);
            if (i >= 0) {
                MessageBox(hDlg, s2ws(std::to_string(i)).c_str(), TEXT(""), 0);
                SendMessage(GetDlgItem(hDlg, IDC_LIST1), LB_GETTEXT, i, (LPARAM)buf);
                MessageBox(hDlg, buf, TEXT(""), 0);
                SetWindowTextW(GetDlgItem(hDlg, IDC_EDIT1), buf);
            }
            break;
        }
        case IDC_COMBO1:
            switch (HIWORD(wParam)) {
            case CBN_SELENDOK:
                int i = SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_GETCURSEL, 0, 0);
                if (i >= 0) {
                    SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_GETLBTEXT, i, (LPARAM)buf);
                    SendMessage(GetDlgItem(hDlg, IDC_LIST1), LB_INSERTSTRING, 0, (LPARAM)buf);
                    SetWindowTextW(GetDlgItem(ventanaPrincipal, IDC_EDIT1), buf);
                }
                break;
            }
        case IDC_LIST1:
            switch (HIWORD(wParam)) {
            case LBN_SELCHANGE:
                MessageBox(hDlg, L"Seleccionaste una opcion", TEXT(""), 0);
                int i = SendMessage(GetDlgItem(hDlg, IDC_LIST1), LB_GETCURSEL, 0, 0);
                if (i >= 0) {
                    MessageBox(hDlg, s2ws(std::to_string(i)).c_str(), TEXT(""), 0);
                    SendMessage(GetDlgItem(hDlg, IDC_LIST1), LB_GETTEXT, i, (LPARAM)buf);
                    MessageBox(hDlg, buf, TEXT(""), 0);
                    SetWindowTextW(GetDlgItem(ventanaPrincipal, 1003), buf);
                }
                break;
            }
            break;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
// Message handler for IDD_DIALOG1 box.
INT_PTR CALLBACK callbackIDD_DIALOG2(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    SYSTEMTIME t;
    TCHAR buf[200];
    switch (message) {
    case WM_INITDIALOG: {
        GetLocalTime(&t);
        GetTimeFormat(LOCALE_USER_DEFAULT, TIME_FORCE24HOURFORMAT, &t, NULL, buf, 200);
        SetWindowTextW(GetDlgItem(hDlg, IDC_EDIT1), buf);
        CheckDlgButton(hDlg, IDC_CHECK1, BST_CHECKED);
        //        if (IsDlgButtonChecked(hDlg, IDC_CHECK2) == BST_UNCHECKED)
        //            MessageBox(hDlg, L"Estoy seleccionado!", L"", 0);
        OPENFILENAME ofn;       // common dialog box structure
        TCHAR szFile[260] = { 0 };       // if using TCHAR macros
        // Initialize OPENFILENAME
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hDlg;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = _T(".bmp\0*.BMP\0.jpg\0*.JPG\0ALL\0*.*\0");
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
        if (GetOpenFileName(&ofn) == TRUE)
        {// use ofn.lpstrFile
        }
        std::wstring stt = ofn.lpstrFile;
        if (!stt.empty()) {
            int iu = stt.find_last_of(TEXT(PATH_SEPARATOR));
            std::wstring t = stt.substr(iu);
            stt = stt.substr(0, iu);
            stt = stt.substr(stt.find_last_of(TEXT(PATH_SEPARATOR)) + 1) + t;
        }
        HBITMAP imagen;
        imagen = (HBITMAP)LoadImage(hInst, stt.c_str(), IMAGE_BITMAP, 200, 200, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
        if (imagen == NULL) {
            MessageBox(hDlg, L"NO SE PUDO ABRIR LA IMAGEN", L"", 0);
            return (INT_PTR)TRUE;
        }
        SendDlgItemMessage(hDlg, IDC_IMAGEN, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imagen);
        HBITMAP imagen2;
        imagen2 = (HBITMAP)LoadImage(hInst, L"Images/OIP.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
        if (imagen2 == NULL) {
            MessageBox(hDlg, L"NO SE PUDO ABRIR LA IMAGEN", L"", 0);
            return (INT_PTR)TRUE;
        }
        //        SendDlgItemMessage(hDlg, IDC_IMAGEN2, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imagen2);
        SendDlgItemMessage(hDlg, IDC_BUTTON1, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imagen2);
    }
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    case WM_TIMER:
        switch (wParam) {
        case IDT_TIMER1:
            GetLocalTime(&t);
            GetTimeFormat(LOCALE_USER_DEFAULT, TIME_FORCE24HOURFORMAT, &t, NULL, buf, 200);
            SetWindowTextW(GetDlgItem(hDlg, IDC_EDIT1), buf);
            break;
        }
    }
    return (INT_PTR)FALSE;
}
// Message handler for IDD_DIALOG1 box.
INT_PTR CALLBACK callbackIDD_DIALOG3(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    SYSTEMTIME t;
    TCHAR buf[200];
    switch (message) {
    case WM_INITDIALOG:
        valor1.clear();
        valor2.clear();
        operando.clear();
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        switch (LOWORD(wParam)) {
        case IDC_BUTTON1: valoresCalculadora(hDlg, L"1");
            break;
        case IDC_BUTTON2: valoresCalculadora(hDlg, L"2");
            break;
        case IDC_BUTTON3: valoresCalculadora(hDlg, L"3");
            break;
        case IDC_BUTTON4: valoresCalculadora(hDlg, L"4");
            break;
        case IDC_BUTTON5: valoresCalculadora(hDlg, L"5");
            break;
        case IDC_BUTTON6: valoresCalculadora(hDlg, L"6");
            break;
        case IDC_BUTTON7: valoresCalculadora(hDlg, L"7");
            break;
        case IDC_BUTTON8: valoresCalculadora(hDlg, L"8");
            break;
        case IDC_BUTTON9: valoresCalculadora(hDlg, L"9");
            break;
        case IDC_BUTTON10: valoresCalculadora(hDlg, L",");
            break;
        case IDC_BUTTON11: valoresCalculadora(hDlg, L"0");
            break;
        case IDC_BUTTON12: valoresCalculadora(hDlg, L".");
            break;
        case IDC_BUTTON13: valoresCalculadora(hDlg, L"+");
            break;
        case IDC_BUTTON14: valoresCalculadora(hDlg, L"-");
            break;
        case IDC_BUTTON15: valoresCalculadora(hDlg, L"*");
            break;
        case IDC_BUTTON16: valoresCalculadora(hDlg, L"/");
            break;
        case IDC_BUTTON17: valoresCalculadora(hDlg, L"=");
            break;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK    callbackIDD_DIALOG4(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    SYSTEMTIME t;
    TCHAR buf[200];
    switch (message) {
    case WM_INITDIALOG:
        // Deshabilitamos el boton de agregar
        EnableWindow(GetDlgItem(hDlg, IDC_BTNAGREGAR), false);
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        switch (LOWORD(wParam)) {
            case IDC_BTNAGREGAR: {
                DateTime_GetSystemtime(GetDlgItem(hDlg, IDC_DATETIMEPICKER1), &t);
                removeTimeFromSystemTime(&t);
                GetDateFormat(LOCALE_USER_DEFAULT, DATE_LONGDATE, &t, NULL, buf, 200);
                GetWindowTextW(GetDlgItem(hDlg, IDC_NOMBRE), buf, 200);
                std::wstring winapiNombre(buf);
                std::string nombre(winapiNombre.begin(), winapiNombre.end());
                GetWindowTextW(GetDlgItem(hDlg, IDC_PESO), buf, 200);
                std::wstring winapiPeso(buf);
                std::string peso(winapiPeso.begin(), winapiPeso.end());
                GetWindowTextW(GetDlgItem(hDlg, IDC_EDAD), buf, 200);
                std::wstring winapiEdad(buf);
                std::string edad(winapiEdad.begin(), winapiEdad.end());
                double fecha;
                if (!SystemTimeToVariantTime(&t, &fecha)) { // Convierto la fecha2 a double si falla la fecha es 0
                    fecha = 0;
                }
                agregarAlFinalDeLista(&HEAD, nombre, atoi(edad.c_str()), atof(peso.c_str()), fecha);
                std::string temp;
                temp.append("");
            }break;
            case IDC_EDAD:
                if (HIWORD(wParam) == EN_CHANGE) {
                    int result = onEditChange(hDlg, IDC_EDAD, 1);
                    if (result == -1) {
                        EnableWindow(GetDlgItem(hDlg, IDC_BTNAGREGAR), false);
                        MessageBox(hDlg, L"Ingresa solo numeros decimales", L"Dato invalido", 0);
                    }
                    else {
                        if (result > 0)
                            EnableWindow(GetDlgItem(hDlg, IDC_BTNAGREGAR), true);
                    }
                }break;
            case IDC_PESO:
                if (HIWORD(wParam) == EN_CHANGE) {
                    int result = onEditChange(hDlg, IDC_PESO, 1);
                    if (result == -1) {
                        EnableWindow(GetDlgItem(hDlg, IDC_BTNAGREGAR), false);
                        MessageBox(hDlg, L"Ingresa solo numeros decimales", L"Dato invalido", 0);
                    }
                    else {
                        if (result > 0)
                            EnableWindow(GetDlgItem(hDlg, IDC_BTNAGREGAR), true);
                    }
                }break;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK    callbackIDD_DIALOG5(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    SYSTEMTIME t;
    TCHAR buf[200];
    switch (message) {
    case WM_INITDIALOG: {
        if (HEAD != NULL) {
            NODO* inicio = HEAD;
            while (inicio != NULL) {
                std::wstring wnombre = s2ws(inicio->nombre);
                if (!VariantTimeToSystemTime(inicio->fechaNacimiento, &t)) { // Convierto la fecha2 a double si falla la fecha es 0
                    GetLocalTime(&t);
                }
                GetDateFormat(LOCALE_USER_DEFAULT, DATE_LONGDATE, &t, NULL, buf, 200);
                wnombre.append(L" ").append(buf);
                const TCHAR* tnombre = wnombre.c_str();
                int pos = SendMessage(GetDlgItem(hDlg, IDC_LIST1), LB_INSERTSTRING, 0, (LPARAM)tnombre);
                SendMessage(GetDlgItem(hDlg, IDC_LIST1), LB_SETITEMDATA, pos, (LPARAM)inicio);
                inicio = inicio->siguiente;
            }
        }
        return (INT_PTR)TRUE;
    }break;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        switch (LOWORD(wParam)) {
            case IDC_BUTTON1: {
                SendMessage(GetDlgItem(hDlg, IDC_LIST1), LB_RESETCONTENT, 0, 0);
                HEAD = ordenarPersonasPorFechaLista(&HEAD, 1);
                NODO* inicio = HEAD;
                while (inicio != NULL) {
                    std::wstring wnombre = s2ws(inicio->nombre);
                    if (!VariantTimeToSystemTime(inicio->fechaNacimiento, &t)) { // Convierto la fecha2 a double si falla la fecha es 0
                        GetLocalTime(&t);
                    }
                    GetDateFormat(LOCALE_USER_DEFAULT, DATE_LONGDATE, &t, NULL, buf, 200);
                    wnombre.append(L" ").append(buf);
                    const TCHAR* tnombre = wnombre.c_str();
                    int pos = SendMessage(GetDlgItem(hDlg, IDC_LIST1), LB_INSERTSTRING, 0, (LPARAM)tnombre);
                    SendMessage(GetDlgItem(hDlg, IDC_LIST1), LB_SETITEMDATA, pos, (LPARAM)inicio);
                    inicio = inicio->siguiente;
                }
            }break;
            case IDC_BUTTON2: {
                SendMessage(GetDlgItem(hDlg, IDC_LIST1), LB_RESETCONTENT, 0, 0);
                HEAD = ordenarPersonasPorFechaLista(&HEAD, 2);
                NODO* inicio = HEAD;
                while (inicio != NULL) {
                    std::wstring wnombre = s2ws(inicio->nombre);
                    if (!VariantTimeToSystemTime(inicio->fechaNacimiento, &t)) { // Convierto la fecha2 a double si falla la fecha es 0
                        GetLocalTime(&t);
                    }
                    GetDateFormat(LOCALE_USER_DEFAULT, DATE_LONGDATE, &t, NULL, buf, 200);
                    wnombre.append(L" ").append(buf);
                    const TCHAR* tnombre = wnombre.c_str();
                    int pos = SendMessage(GetDlgItem(hDlg, IDC_LIST1), LB_INSERTSTRING, 0, (LPARAM)tnombre);
                    SendMessage(GetDlgItem(hDlg, IDC_LIST1), LB_SETITEMDATA, pos, (LPARAM)inicio);
                    inicio = inicio->siguiente;
                }
            }break;
            case IDC_BUTTON3: {
                SendMessage(GetDlgItem(hDlg, IDC_LIST1), LB_RESETCONTENT, 0, 0);
                DateTime_GetSystemtime(GetDlgItem(hDlg, IDC_DATETIMEPICKER1), &t);
                //removeTimeFromSystemTime(&t);
                NODO* inicio = HEAD;
                while (inicio != NULL) {
                    double fecha1 = inicio->fechaNacimiento, fecha2 = 0;
                    if (!SystemTimeToVariantTime(&t, &fecha2)) {
                        fecha2 = 0;
                    }
                    if ((int)fecha1 == (int)fecha2) {
                        std::wstring wnombre = s2ws(inicio->nombre);
                        SYSTEMTIME fecha;
                        if (!VariantTimeToSystemTime(inicio->fechaNacimiento, &fecha)) {
                            fecha = t;
                        }
                        GetDateFormat(LOCALE_USER_DEFAULT, DATE_LONGDATE, &fecha, NULL, buf, 200);
                        wnombre.append(L" ").append(buf);
                        const TCHAR* tnombre = wnombre.c_str();
                        int pos = SendMessage(GetDlgItem(hDlg, IDC_LIST1), LB_INSERTSTRING, 0, (LPARAM)tnombre);
                        SendMessage(GetDlgItem(hDlg, IDC_LIST1), LB_SETITEMDATA, pos, (LPARAM)inicio);
                    }
                    inicio = inicio->siguiente;
                }
            }break;
            case IDC_LIST1:
                switch (HIWORD(wParam)) {
                case LBN_SELCHANGE:
                    int i = SendMessage(GetDlgItem(hDlg, IDC_LIST1), LB_GETCURSEL, 0, 0);
                    if (i >= 0) {
                        NODO* data = (NODO *)SendMessage(GetDlgItem(hDlg, IDC_LIST1), LB_GETITEMDATA, i, 0);
                        std::string nNombre = data->nombre;
                        nNombre.append(" ").append(std::to_string(data->edad));
                        std::wstring b = s2ws(nNombre);
                        MessageBox(hDlg, b.c_str(), TEXT(""), 0);
                        SetWindowTextW(GetDlgItem(ventanaPrincipal, 1003), b.c_str());
                    }
                    break;
                }
                break;
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

std::wstring valoresCalculadora(HWND hDlg, std::wstring number) {
    TCHAR buf[200];
    std::wstring temporal(L"");
    temporal.clear();
    if (number.compare(L"+") == 0 ||
        number.compare(L"-") == 0 ||
        number.compare(L"*") == 0 ||
        number.compare(L"/") == 0 ||
        number.compare(L"=") == 0) {
        if (operando.compare(L"") == 0)
            operando.append(number);
        else
            if (operando.compare(L"+") == 0) {
                std::wstring tmp = valor1;
                valor1.clear();
                valor1.append(s2ws(std::to_string(wcstold(tmp.c_str(), NULL) + wcstold(valor2.c_str(), NULL))));
            } else if (operando.compare(L"-") == 0) {

            } else  if (operando.compare(L"*") == 0) {

            } else if (operando.compare(L"/") == 0) {

            } else if (operando.compare(L"=") == 0) {
            
            }
            valor2.clear();
            operando.clear();
            operando.append(number);
    }
    else {
        if (operando.compare(L"") != 0) {
            if (number.compare(L".") == 0) {
                if (valor2.find(L".") < 1 == std::wstring::npos)
                    valor2.append(number);
            }
            else if (number.compare(L",") == 0) {
                if (valor2.find(L",") < 1 == std::wstring::npos)
                    valor2.append(number);
            }
            else valor2.append(number);
        }
        else {
            if (number.compare(L".") == 0) {
                if (valor1.find(L".") == std::wstring::npos)
                    valor1.append(number);
            }
            else if (number.compare(L",") == 0) {
                if (valor1.find(L",") == std::wstring::npos)
                    valor1.append(number);
            }
            else valor1.append(number);
        }
    }
    if (operando.compare(L"=") == 0) {
        temporal.append(valor1);
        operando.clear();
        valor1.clear();
        valor2.clear();
    } else 
        temporal.append(valor1).append(L" ").append(operando).append(L" ").append(valor2);
    SetWindowTextW(GetDlgItem(hDlg, IDC_EDIT1), temporal.c_str());
    return temporal;
}
SYSTEMTIME convertStringToSystemTime(char* dateTimeString) {
    SYSTEMTIME systime;

    memset(&systime, 0, sizeof(systime));
    // Date string should be "yyyy-MM-dd hh:mm"
    sscanf_s(dateTimeString, "%d-%d-%d%d:%d:%d",
        &systime.wYear,
        &systime.wMonth,
        &systime.wDay,
        &systime.wHour,
        &systime.wMinute);
    return systime;
}

void imprimirLista(struct NODO* lista) {
    struct NODO* indice = lista;
    while (indice != NULL) {
        indice = indice->siguiente;
    }
}

void eliminarNodoAlFinalDeLista(struct NODO** HEAD) {
    if (*HEAD == NULL) { // Si la lista esta vacia, no se hace nada
        return;
    }
    else {
        struct NODO* indice = *HEAD, * anterior = NULL; //inicio de la lista
        while (indice->siguiente != NULL) { // Buscamos el final de la lista, osea apuntador siguiente == NULL
            anterior = indice;
            indice = indice->siguiente;
        }
        anterior->siguiente = NULL;
        delete indice;
    }
}

void eliminarNodoInicioDeLista(struct NODO** HEAD) {
    if (*HEAD == NULL) {// Si la lista esta vacia, no se hace nada
        return;
    }
    else {
        struct NODO* nodo = *HEAD; //inicio de la lista
        *HEAD = nodo->siguiente; // reemplazamos por nodo siguiente de la lista
        delete nodo;
    }
}

void eliminarNodoMedioDeLista(struct NODO** HEAD, std::string buscarNombre) {
    if (*HEAD == NULL) {// Si la lista esta vacia, no se hace nada
        return;
    }
    else {
        struct NODO* indice = *HEAD, * anterior = NULL;
        bool loEncontre = false;
        while (indice->siguiente != NULL) {// Buscamos el final de la lista, osea apuntador siguiente == NULL
            if (indice->nombre.compare(buscarNombre) == 0) {
                loEncontre = true;
                break;
            }
            anterior = indice;
            indice = indice->siguiente;
        }
        if (loEncontre == true || indice->nombre.compare(buscarNombre) == 0) {
            anterior->siguiente = indice->siguiente;
            delete indice;
        }
    }
}

struct NODO* modificarNodoDeLista(struct NODO** HEAD, std::string nombre, int edad, float peso, SYSTEMTIME fechaNacimiento, std::string buscarNombre) {
    struct NODO* nodo = NULL;
    if (*HEAD == NULL) {// Si la lista esta vacia, no se hace nada
        return NULL;
    }
    else {
        struct NODO* indice = *HEAD, * anterior = NULL;
        bool loEncontre = false;
        while (indice->siguiente != NULL) { // Buscamos el final de la lista, osea apuntador siguiente == NULL
            if (indice->nombre.compare(buscarNombre) == 0) {
                loEncontre = true;
                break;
            }
            anterior = indice;
            indice = indice->siguiente;
        }
        if (loEncontre == true || indice->nombre.compare(buscarNombre) == 0) {
            nodo = indice;
            nodo->nombre = nombre;
            nodo->peso = peso;
            nodo->edad = edad;
        }
    }
    return nodo;
}

struct NODO* agregarAlFinalDeLista(struct NODO** HEAD, std::string nombre, int edad, float peso, double fechaNacimiento) {
    struct NODO* nuevo = NULL;
    if (*HEAD == NULL) {// Si la lista esta vacia, no se hace nada
        nuevo = nuevoNodo(nombre, edad, peso, fechaNacimiento);
        *HEAD = nuevo;
    }
    else {
        struct NODO* indice = *HEAD;
        bool loEncontre = false;
        while (indice->siguiente != NULL) { // Buscamos el final de la lista, osea apuntador siguiente == NULL
            indice = indice->siguiente;
        }
        nuevo = nuevoNodo(nombre, edad, peso, fechaNacimiento);
        indice->siguiente = nuevo;
        nuevo->siguiente = NULL;
    }
    return nuevo;
}

struct NODO* agregarEnMedioDeLista(struct NODO** HEAD, std::string nombre, int edad, float peso, double fechaNacimiento, std::string buscarNombre) {
    struct NODO* nuevo = NULL;
    if (*HEAD == NULL) {// Si la lista esta vacia, no se hace nada
        nuevo = nuevoNodo(nombre, edad, peso, fechaNacimiento);
        *HEAD = nuevo;
    }
    else {
        struct NODO* indice = *HEAD, * anterior = NULL;
        bool loEncontre = false;
        while (indice->siguiente != NULL) { // Buscamos el final de la lista, osea apuntador siguiente == NULL
            if (indice->nombre.compare(buscarNombre) == 0) {
                loEncontre = true;
                break;
            }
            anterior = indice;
            indice = indice->siguiente;
        }
        if (loEncontre == true || indice->nombre.compare(buscarNombre) == 0) {
            if (anterior == NULL) {
                agregarAElInicioDeLista(HEAD, nombre, edad, peso, fechaNacimiento);
            }
            else {
                nuevo = nuevoNodo(nombre, edad, peso, fechaNacimiento);
                anterior->siguiente = nuevo;
                nuevo->siguiente = indice;

            }
        }
    }
    return nuevo;
}

struct NODO* agregarAElInicioDeLista(struct NODO** HEAD, std::string nombre, int edad, float peso, double fechaNacimiento) {
    struct NODO* nuevo = NULL;
    if (*HEAD == NULL) { // Si la lista esta vacia, no se hace nada
        nuevo = nuevoNodo(nombre, edad, peso, fechaNacimiento);
        *HEAD = nuevo;
    }
    else {
        nuevo = nuevoNodo(nombre, edad, peso, fechaNacimiento);
        nuevo->siguiente = *HEAD;
        *HEAD = nuevo;
    }
    return nuevo;
}

struct NODO* nuevoNodo(std::string nombre, int edad, float peso, double fechaNacimiento) {
    struct NODO* nuevo = new struct NODO; // Se le pide al sistema operativo memoria en el heap para nuestra estructura
    nuevo->nombre = nombre;
    nuevo->edad = edad;
    nuevo->fechaNacimiento = fechaNacimiento;
    nuevo->peso = peso;
    nuevo->siguiente = NULL;
    return nuevo;
}

void removeTimeFromSystemTime(SYSTEMTIME* date) {
    date->wHour = 0;
    date->wMinute = 0;
    date->wSecond = 0;
    date->wMilliseconds = 0;
}

struct NODO* ordenarPersonasPorFechaLista(struct NODO** head, int orden) {
    struct NODO* indice = *head, *primero = NULL;
    // Si la lista esta vacia, retorno el apuntador de entrada
    if (indice == NULL) return *head;
    // Si la lista solo tiene un elemento, retorno el apuntador de entrada
    if (indice->siguiente == NULL) return *head;
    // Variable para resguardar el nodo anterior al nodo en curso
    struct NODO* anterior = NULL;
    while (indice != NULL) {
        // Variable que empezara en el elemento siguiente al elemento actual i + 1
        struct NODO* indice2 = indice->siguiente, 
                    *anterior1 = indice; // Variable que contiene el nodo i
        // Recorremos la lista apartir del nodo i + 1
        while (indice2 != NULL) {
            double fecha1 = indice->fechaNacimiento, fecha2 = indice2->fechaNacimiento;
            bool condicionOrden = orden == 1? 
                                    fecha1 < fecha2 : // Condicion de ORDEN ASCENDENTE
                                    fecha1 > fecha2;  // Condicion de ORDEN DESCENDENTE
            if (condicionOrden) { // Si es ascendente o descendente, hago el intercambio de los nodos
                // Si el nodo anterior a la variable indice es diferente de NULL, 
                // cambiamos el apuntador siguiente al nuevo orden del nodo
                if (anterior != NULL) {
                    anterior->siguiente = indice2;
                }
                // Si el nodo anterior a la variable indice2 es diferente de NULL,
                // cambiamos el apuntador siguiente al nuevo orden del nodo
                if (anterior1 != NULL) {
                    anterior1->siguiente = indice;
                }
                // Guardamos el nodo siguiente de nuestro nodo indice
                struct NODO *tmp = indice->siguiente;
                // Reemplazamos los apuntadores para apuntar al elemento siguiente respecto
                // a su nueva posicion en la lista
                indice->siguiente = indice2->siguiente;
                indice2->siguiente = tmp;
                // Guardamos el nodo que vamos a intercambiar con el indice
                tmp = indice2;
                // Intercambiamos los nodos ya reordenados en la lista
                indice2 = indice;
                indice = tmp;
            }
            // Guardamos el nodo anterior al nodo que esta recorriendo la lista para comparar
            anterior1 = indice2;
            // Avanzamos en la lista
            indice2 = indice2->siguiente;
        }
        // Si el primer elemento de la lista ya se ordeno, lo guardamos para no perder
        // el nuevo origen de la lista
        if (primero == NULL)
            primero = indice;
        // Rresguardamos el elemento anterior al nodo pivote de ordenacion
        anterior = indice;
        // Movemos el pivote al siguiente elemento de la lista
        indice = indice->siguiente;
    }
    // Asignamos el nuevo origen de la lista al apuntador de entrada
    *head = primero;
    // Retornamos el apuntador con la lista ordenada
    return *head;
}

bool isFloat(std::wstring str) {
    std::wistringstream iss(str);
    float f;
    wchar_t wc;
    if (!(iss >> f) || iss.get(wc))
        return false;
    return true;
}

bool isInt(std::wstring str) {
    std::wistringstream iss(str);
    int f;
    wchar_t wc;
    if (!(iss >> f) || iss.get(wc))
        return false;
    return true;
}

int onEditChange(HWND hDlg, int ctrlID, int type) {
    HWND hEdit = GetDlgItem(hDlg, ctrlID);
    size_t len = GetWindowTextLength(hEdit) + 1;
    wchar_t* cstr = new wchar_t[len];
    GetWindowText(hEdit, cstr, len);

    std::wstring wstr(cstr);
    bool condicion = type == 0 ? isInt(wstr) : isFloat(wstr);
    int result = 0;
    if (!(condicion)) {
        result = -1;
    }
    result = result == -1 ? -1 : wstr.length();
    delete[] cstr;
    return result;
}