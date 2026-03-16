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
            if (id == TIME_DILATION) ui.showTimeDilation(L"Welcome to Time Dilation!");
            if (id == CALC_GAMMA) {
                POINT mid = ui.Get_Mid_coordinates(hwnd);
                HWND hCalc = CreateWindowExW(
    0,
    L"GammaCalculatorClass",
    L"Gamma Calculator",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
    mid.x, mid.y, 600, 500,
    NULL,   // ❗ NOT hwnd
    NULL,
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
    hwnd    // pass main hwnd via lpParam
);
            }
            if (id == ID_BACK)    ui.ShowMenu();
        }
            break;

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
LRESULT CALLBACK GammaWindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    static UI gammaUI;

    switch (msg) {

        case WM_CREATE:
        {
            gammaUI.InitCalculatorWindow(hwnd);
            gammaUI.ShowCalculation(L"Enter velocity in m/s",L"Enter m/s...");
            gammaUI.UpdateLayoutCalculator();
            break;
        }

        case WM_COMMAND:
        {
            int id = LOWORD(wp);

            if (id == ID_TOGGLE) {
                gammaUI.isInverseMode = !gammaUI.isInverseMode;

                if (gammaUI.isInverseMode) {
                    SetWindowTextW(gammaUI.CalcButtons.hStaticTitle, L"Enter Gamma Factor");
                    SendMessage(gammaUI.CalcButtons.hEditInput, EM_SETCUEBANNER, FALSE, (LPARAM)L"e.g. 2.29");
                } else {
                    SetWindowTextW(gammaUI.CalcButtons.hStaticTitle, L"Enter velocity in m/s");
                    SendMessage(gammaUI.CalcButtons.hEditInput, EM_SETCUEBANNER, FALSE, (LPARAM)L"Enter m/s...");
                }

                SetWindowTextW(gammaUI.CalcButtons.hEditResult, L"Result:");
            }
            if (id == ID_INFO) gammaUI.ShowHelp(gammaUI.isInverseMode);

            if (id == ID_SUBMIT) {
                wchar_t buffer[128];
                GetWindowTextW(gammaUI.CalcButtons.hEditInput, buffer, 128);
                std::wstring input(buffer);
                std::wstringstream ss;

                if (gammaUI.isInverseMode) {
                    long double result = RelativityCalculator::ParseGammaInput(input);

                    if (result == -1.0L)
                        SetWindowTextW(gammaUI.CalcButtons.hEditResult, L"Error: Gamma must be >= 1");
                    else if (result == -2.0L)
                        SetWindowTextW(gammaUI.CalcButtons.hEditResult, L"Error: Invalid Input");
                    else {
                        ss << L"Speed: " << std::fixed << std::setprecision(2) << result << L" m/s";
                        SetWindowTextW(gammaUI.CalcButtons.hEditResult, ss.str().c_str());
                    }
                }
                else {
                    long double result = RelativityCalculator::ParseAndCalculate(input);

                    if (result == -1.0L)
                        SetWindowTextW(gammaUI.CalcButtons.hEditResult, L"Error: v must be < c");
                    else if (result == -2.0L)
                        SetWindowTextW(gammaUI.CalcButtons.hEditResult, L"Error: Invalid Input");
                    else {
                        ss << L"Gamma: " << std::fixed << std::setprecision(15) << result;
                        SetWindowTextW(gammaUI.CalcButtons.hEditResult, ss.str().c_str());
                    }
                }

                SetWindowTextW(gammaUI.CalcButtons.hEditInput, L"");
            }

            if (id == ID_BACK)
                DestroyWindow(hwnd);

            break;
        }

        case WM_SIZE:
            gammaUI.UpdateLayoutCalculator();
            break;

        case WM_CLOSE:
            DestroyWindow(hwnd);
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
    WNDCLASSW gwc = {};
    gwc.lpfnWndProc   = GammaWindowProc;
    gwc.hInstance     = hInst;
    gwc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    gwc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    gwc.lpszClassName = L"GammaCalculatorClass";
    RegisterClassW(&gwc);

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