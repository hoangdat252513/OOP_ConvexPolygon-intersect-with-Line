// Final_Project.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Final_Project.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
 
    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FINALPROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FINALPROJECT));

    MSG msg;

    // Main message loop:
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



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FINALPROJECT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_FINALPROJECT);
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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

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
static int i = 0;
static bool check = false;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    double temp[] = { 200, 200, 600, 200, 900, 500, 700, 700, 300, 700, 100, 400 };
    static Point* p[] = { new Square(0, 0, 200),
                         new RecTangle(200, 200, 200, 100),
                         new Elip(300, 300, 200, 100),
                         new Triangle(200, 300, 600, 600, 100, 500),
                         new Circle(300, 300, 100), 
                         new Crescent(200, 200, 100), 
                         new ConvexPolygon(temp, sizeof(temp)/sizeof(temp[0]))
    };
    WCHAR msg[][50] = { L"Khong cat nhau", L"Cat nhau"};
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
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
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_RIGHT:
            p[i]->Move(10, 0);
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_LEFT:
            p[i]->Move(-1, 0);
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_UP:
            p[i]->Move(0, -10);
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_DOWN:
            p[i]->Move(0, 10);
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_TAB:
            if (i >=7) {
                i = 0;
            }
            else {
                i++;
            }
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        break;
    case WM_CHAR:
        switch (TCHAR(wParam))
        {
        case '+':
            p[i]->Scale(1.2);
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case '-':
            p[i]->Scale(1 / 1.2);
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...

            HGDIOBJ original = NULL;
            //    Saving the original object
            original = SelectObject(hdc, GetStockObject(DC_PEN));
            RECT r;
            GetClientRect(hWnd, &r);
            
            if (i == 7) {
                for (int j = 0; j < 7; j++) {
                    p[j]->draw(hdc, original);
                }
                int Count = Count_Intersect(p, 7, hdc, Point(300, 100), Point(500, 300));
                string str = "Co: " + to_string(Count) + " hinh giao voi duong thang.";
                std::wstring widestr = std::wstring(str.begin(), str.end());
                const wchar_t* widecst = widestr.c_str();
               
                DrawText(hdc, widecst, lstrlen(widecst), &r, DT_CENTER | DT_BOTTOM);
            }
            else {
                int check = p[i]->isIntersecting(hdc, Point(300, 100), Point(500, 300));
                p[i]->draw(hdc, original);
                DrawText(hdc, msg[check], lstrlen(msg[check]), &r, DT_CENTER | DT_BOTTOM);
            }
            
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
