#include "UI.h"
#include "Buttons.h"
#include <commctrl.h>
#include <iomanip>
#include <sstream>
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
                wchar_t buffer[128];
                GetWindowTextW(ui.hEdit1, buffer, 128);

                double result = RelativityCalculator::ParseAndCalculate(buffer);

                // 1. Clear the input box immediately
                SetWindowTextW(ui.hEdit1, L"");
                ShowWindow(ui.hEditResult, SW_SHOW); // Ensure it's visible

                // 2. Display the result in the new box
                if (result == -1.0) {
                    SetWindowTextW(ui.hEditResult, L"Error: v must be < c and >= 0");
                } else if (result == -2.0) {
                    SetWindowTextW(ui.hEditResult, L"Error: Invalid input");
                } else {
                    std::wstringstream ss;
                    ss << L"Gamma: " << std::fixed << std::setprecision(15) << result;

                    SetWindowTextW(ui.hEditResult, ss.str().c_str());
                }
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
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Uses the brush created in UI constructor
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