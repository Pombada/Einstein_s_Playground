#include "UI.h"
#include "Buttons.h"
#include <commctrl.h>
#include <iomanip>
#include <sstream>
#pragma comment(lib, "comctl32.lib")

UI ui;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            ui.Init(hwnd);
            break;
        case WM_COMMAND: {
            int id = LOWORD(wp);
            if (id == TIME_DILATION) ui.ShowCalculation(L"Enter velocity in m/s", L"Enter m/s...");
            if (id == ID_INFO)    ui.ShowHelp(ui.isInverseMode);
            if (id == ID_BACK)    ui.ShowMenu();
            if (id == ID_TOGGLE) {
                ui.isInverseMode = !ui.isInverseMode;

                if (ui.isInverseMode) {
                    SetWindowTextW(ui.hStaticTitle, L"Enter Gamma Factor");
                    SendMessage(ui.hEdit1, EM_SETCUEBANNER, FALSE, (LPARAM)L"e.g. 2.29");
                } else {
                    SetWindowTextW(ui.hStaticTitle, L"Enter velocity in m/s");
                    SendMessage(ui.hEdit1, EM_SETCUEBANNER, FALSE, (LPARAM)L"Enter m/s, %, or M...");
                }
                SetWindowTextW(ui.hEditResult, L"Result:");
            }
            if (id == ID_SUBMIT) {
                wchar_t buffer[128];
                GetWindowTextW(ui.hEdit1, buffer, 128);
                std::wstring input(buffer);
                std::wstringstream ss;

                if (ui.isInverseMode) {
                    // Mode 1: Gamma -> Speed
                    long double result = RelativityCalculator::ParseGammaInput(input);

                    if (result == -1.0L) {
                        SetWindowTextW(ui.hEditResult, L"Error: Gamma must be >= 1");
                    } else if (result == -2.0L) {
                        SetWindowTextW(ui.hEditResult, L"Error: Invalid Input");
                    } else {
                        ss << L"Speed: " << std::fixed << std::setprecision(2) << result << L" m/s";
                        SetWindowTextW(ui.hEditResult, ss.str().c_str());
                    }
                } else {
                    // Mode 2: Speed -> Gamma (Your existing code)
                    long double result = RelativityCalculator::ParseAndCalculate(input);
                    if (result == -1.0L) {
                        SetWindowTextW(ui.hEditResult, L"Error: v must be < c and >= 0");
                    } else if (result == -2.0L) {
                        SetWindowTextW(ui.hEditResult, L"Error: Invalid input");
                    } else {
                        ss << L"Gamma: " << std::fixed << std::setprecision(15) << result;
                        SetWindowTextW(ui.hEditResult, ss.str().c_str());
                    }
                }
                SetWindowTextW(ui.hEdit1, L""); // Clear input
            }
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
    int windowWidth = 800;
    int windowHeight = 600;
    // 2. Register the Window Class
    WNDCLASSW wc = {};
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInst;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Uses the brush created in UI constructor
    wc.lpszClassName = L"EinsteinPlaygroundClass";

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Calculate the center position
    int posX = (screenWidth - windowWidth) / 2;
    int posY = (screenHeight - windowHeight) / 2;
    RegisterClassW(&wc);

    // 3. Create the Actual Window
    HWND hwnd = CreateWindowExW(
        WS_EX_APPWINDOW,
        L"EinsteinPlaygroundClass",
        L"Einstein's Playground",
        WS_OVERLAPPEDWINDOW,
        posX, posY, windowWidth, windowHeight,
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