
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <iostream>

using namespace std;

// Global variables
static wchar_t* dialog_text = NULL;
int len_dialog_text;
int state;

//The string that appears in the window
 TCHAR greeting[] = _T("A simple crackme :D");
// The main window class name.
static TCHAR szWindowClass[] = _T("DesktopApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Simple CrackMe");

HINSTANCE hInst;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    // Store instance handle in our global variable
    hInst = hInstance;

    // The parameters to CreateWindowEx explained:
    // WS_EX_OVERLAPPEDWINDOW : An optional extended window style.
    // szWindowClass: the name of the application
    // szTitle: the text that appears in the title bar
    // WS_OVERLAPPEDWINDOW: the type of window to create
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    // 500, 100: initial size (width, length)
    // NULL: the parent of this window
    // NULL: this application does not have a menu bar
    // hInstance: the first parameter from WinMain
    // NULL: not used in this application
    HWND hWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        400,200,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Simple Crack Me"),
            NULL);

        return 1;
    }


    // The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindow
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    static HWND hwndEdit;
    static HBRUSH g_hbrBackground = CreateSolidBrush(RGB(0, 0, 255));
    ;

    switch (message)
    {
    case WM_CREATE:
        CreateWindowA(TEXT("button"), TEXT("Enter"),
            WS_VISIBLE | WS_CHILD,
            20, 80, 80, 25,
            hwnd, (HMENU)1, NULL, NULL);

        CreateWindow(TEXT("button"), TEXT("Quit"),
            WS_VISIBLE | WS_CHILD,
            120, 80, 80, 25,
            hwnd, (HMENU)2, NULL, NULL);

        hwndEdit = CreateWindowW(L"Edit", NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            20, 50, 180, 20, hwnd, (HMENU)3,
            NULL, NULL);
        break;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);

        TextOut(hdc,
            5, 5,
            greeting, _tcslen(greeting));

        EndPaint(hwnd, &ps);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == 1) {
            len_dialog_text = GetWindowTextLengthW(hwndEdit) + sizeof(wchar_t);
            GetWindowTextW(hwndEdit, dialog_text, len_dialog_text);

            // Checks if string entered is the answer
            if (wcslen(dialog_text) == strlen("we did it")) {
                state = wcscmp(dialog_text, L"we did it");
            }
            else
            {
                state = -1;
            }

            if (state == 0) {
                SetWindowTextW(hwnd, L"Right!");
            }
            else
            {
                SetWindowTextW(hwnd, L"Wrong!");
            }
        }

        if (LOWORD(wParam) == 2) {
            PostQuitMessage(0);
        }

        if (LOWORD(wParam) == 3) {
            len_dialog_text = GetWindowTextLengthW(hwndEdit) + sizeof(wchar_t);
            if (dialog_text != NULL ) {
                dialog_text = NULL;
                free(dialog_text);
            }

            dialog_text = (wchar_t*)(malloc(len_dialog_text));

        }

        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
        break;
    }

    return 0;
}
