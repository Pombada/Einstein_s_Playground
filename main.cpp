#include "UI.h"
#include "Buttons.h"
#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")

UI ui; // Short and sweet

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            ui.Init(hwnd);
            break;
        case WM_COMMAND: {
            int id = LOWORD(wp);
            if (id == ID_OPTION1) ui.ShowCalculation(L"Enter velocity in m/s", L"Enter m/s...");
            if (id == ID_INFO)    ui.ShowHelp();
            if (id == ID_BACK)    ui.ShowMenu();
            if (id == ID_SUBMIT) {
                // Logic for calculation using Buttons.h
            }
            break;
        }
        case WM_SIZE:
            ui.UpdateLayout();
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int nShow) {
    // 1. Initialize Common Controls (for modern button styles)
    InitCommonControls();

    // 2. Register the Window Class
    WNDCLASSW wc = {};
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInst;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = ui.hBackBrush; // Uses the brush created in UI constructor
    wc.lpszClassName = L"EinsteinPlaygroundClass";

    RegisterClassW(&wc);

    // 3. Create the Actual Window
    HWND hwnd = CreateWindowExW(
        WS_EX_APPWINDOW,
        L"EinsteinPlaygroundClass",
        L"Einstein's Playground",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 400,
        NULL, NULL, hInst, NULL
    );

    if (!hwnd) return 0;

    // 4. Show the Window
    ShowWindow(hwnd, nShow);

    // 5. The Message Loop (The heart of the app)
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}