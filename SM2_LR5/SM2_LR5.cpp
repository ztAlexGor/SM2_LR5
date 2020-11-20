// SM2_LR5.cpp : Определяет точку входа для приложения.
//

#include <windows.h>
#include <ctime>
#include "SM2_LR5.h"
#include "Game.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
HBITMAP hBackground;
BITMAP Background;

HBITMAP hImageAttack;
BITMAP ImageAttack;

HDC backDC;
HBITMAP hBackBMP;
RECT clientRect;
HWND hWnd;
Game game(&hImageAttack, &ImageAttack);
HDC hdc;
int Game::Player::NumberOfCards = 2;
bool isDrawing = true;


// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
bool makeTurn(int pl, LPARAM lParam);
void DrawBackground(HDC hdc);
bool BlitBitmap(HBITMAP hBitmap, BITMAP Bitmap, HDC hdc);
bool LoadGlobalBitmap(HINSTANCE hInst, HBITMAP& hBitmap, BITMAP& bitmap, INT bitmapID);
void DrawFrame();


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.
    srand(time(NULL));
    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SM2LR5, szWindowClass, MAX_LOADSTRING);
    LoadGlobalBitmap(hInstance, hBackground, Background, IDB_BACKGROUND);
    LoadGlobalBitmap(hInstance, hImageAttack, ImageAttack, IDB_ATTACKCARD);


    MyRegisterClass(hInstance);
    game.Initialize();


    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SM2LR5));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SM2LR5));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SM2LR5);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   
   
   int width = 1280, height = 720;
   int   x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
   int   y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

   RECT _windowRC{ x, y, x + width, y + height };//100x200
   AdjustWindowRect((LPRECT)&_windowRC, WS_OVERLAPPEDWINDOW, true);//121x234

   hWnd = CreateWindow(szWindowClass, L"Lab 5", (WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX),
       _windowRC.left,
       _windowRC.top,
       _windowRC.right - _windowRC.left, _windowRC.bottom - _windowRC.top, nullptr, nullptr, hInstance, nullptr);


   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&DrawFrame, NULL, NULL, NULL);
   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_LBUTTONDOWN:
        makeTurn(PLAYER1, lParam);
        break;
    case WM_RBUTTONDOWN:
        makeTurn(PLAYER2, lParam);
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
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
            BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, backDC, 0, 0, SRCCOPY);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        isDrawing = false;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

bool makeTurn(int pl, LPARAM lParam) {
    WORD x = LOWORD(lParam);
    WORD y = HIWORD(lParam);
    if (y <= 400) {
        return game.MakeMove(pl, 0);
    }
    else {
        return game.MakeMove(pl, 1);
    }
}

void DrawFrame() {
    
    GetClientRect(hWnd, &clientRect);
    hdc = GetDC(hWnd);
    backDC = CreateCompatibleDC(hdc);
    hBackBMP = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
    HGDIOBJ oldBMP = SelectObject(backDC, hBackBMP);
    ReleaseDC(hWnd, hdc);

    while (isDrawing) {
        BlitBitmap(hBackground, Background, backDC);
        game.Draw(backDC);
        //InvalidateRect(hWnd, 0, FALSE);
        RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
        Sleep(1000 / 60);
    }

    SelectObject(hdc, oldBMP);
    DeleteObject(hBackBMP);
    DeleteDC(backDC);
}


void DrawBackground(HDC hdc) {
    
}

bool LoadGlobalBitmap(HINSTANCE hInst, HBITMAP& hBitmap, BITMAP& bitmap, INT bitmapID)
{
    hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(bitmapID));
    //hBitmap = (HBITMAP)::LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
    if (hBitmap == NULL) {
        ::MessageBox(NULL, L"LoadImage Failed", L"Error", MB_OK);
        return false;
    }
    
    // Get the bitmap's parameters and verify the get
    int iReturn = GetObject(hBitmap, sizeof(BITMAP), &bitmap);
    if (!iReturn) {
        ::MessageBox(NULL, L"GetObject Failed", L"Error", MB_OK);
        return false;
    }
    return true;
}

bool BlitBitmap(HBITMAP hBitmap, BITMAP Bitmap, HDC hdc)
{
    HDC hLocalDC = CreateCompatibleDC(hdc);
    if (hLocalDC == NULL) {
        MessageBox(NULL, L"CreateCompatibleDC Failed", L"Error", MB_OK);
        return false;
    }

    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hLocalDC, hBitmap);
    if (hOldBitmap == NULL) {
        MessageBox(NULL, L"SelectObject Failed", L"Error", MB_OK);
        return false;
    }

    // Blit the dc which holds the bitmap onto the window's dc
    BOOL qRetBlit = BitBlt(hdc, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, hLocalDC, 0, 0, SRCCOPY);
    if (!qRetBlit) {
        MessageBox(NULL, L"Blit Failed", L"Error", MB_OK);
        return false;
    }

    // Unitialize and deallocate resources
    SelectObject(hLocalDC, hOldBitmap);
    DeleteDC(hLocalDC);
    DeleteObject(hOldBitmap);
    return true;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
