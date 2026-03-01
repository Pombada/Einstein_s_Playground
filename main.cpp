#ifndef UNICODE
#define UNICODE
#endif

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "comctl32.lib")

#include <windows.h>
#include <commctrl.h>
#include <vector>
#include <string>

// Enable Modern Visual Styles
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

enum { ID_OPTION1 = 101, ID_OPTION2, ID_OPTION3, ID_OPTION4, ID_SUBMIT = 200, ID_BACK = 300 };

class ModernApp {
public:
    HWND hwnd = nullptr;
    HFONT hFontUI = nullptr;
    HFONT hFontTitle = nullptr;

    // Modern Colors
    HBRUSH hBackBrush = CreateSolidBrush(RGB(250, 250, 252));
    HBRUSH hHeaderBrush = CreateSolidBrush(RGB(255, 255, 255));

    std::vector<HWND> menuButtons;
    HWND hEdit1, hEdit2, hStaticTitle, hBtnSubmit, hBtnBack;

    void UpdateLayout() {
        RECT r;
        GetClientRect(hwnd, &r);
        int w = r.right;
        int h = r.bottom;

        // Scaling factors
        float sx = w / 500.0f;
        float sy = h / 400.0f;
        float s = (sx < sy) ? sx : sy;

        // Update Fonts (C++20 styles)
        if (hFontUI) DeleteObject(hFontUI);
        if (hFontTitle) DeleteObject(hFontTitle);
        hFontUI = CreateFontW((int)(18 * s), 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, L"Segoe UI");
        hFontTitle = CreateFontW((int)(28 * s), 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, L"Segoe UI");

        // Layout: Header Area
        MoveWindow(hStaticTitle, 0, 0, w, (int)(80 * sy), TRUE);

        // Layout: Menu Buttons (Grid)
        int btnW = (int)(160 * sx), btnH = (int)(50 * sy);
        int centerX = w / 2;
        MoveWindow(menuButtons[0], centerX - btnW - 10, (int)(120 * sy), btnW, btnH, TRUE);
        MoveWindow(menuButtons[1], centerX + 10,        (int)(120 * sy), btnW, btnH, TRUE);
        MoveWindow(menuButtons[2], centerX - btnW - 10, (int)(180 * sy), btnW, btnH, TRUE);
        MoveWindow(menuButtons[3], centerX + 10,        (int)(180 * sy), btnW, btnH, TRUE);

        // Layout: Form
        int editW = (int)(320 * sx);
        MoveWindow(hEdit1, centerX - (editW / 2), (int)(130 * sy), editW, (int)(35 * sy), TRUE);
        MoveWindow(hBtnSubmit, centerX - (int)(110 * sx), (int)(200 * sy), (int)(100 * sx), (int)(40 * sy), TRUE);
        MoveWindow(hBtnBack, centerX + (int)(10 * sx), (int)(200 * sy), (int)(100 * sx), (int)(40 * sy), TRUE);

        // Apply Fonts
        auto apply = [&](HWND h, HFONT f) { if(h) SendMessage(h, WM_SETFONT, (WPARAM)f, TRUE); };
        apply(hStaticTitle, hFontTitle);
        for (auto b : menuButtons) apply(b, hFontUI);
        apply(hEdit1, hFontUI);
        apply(hBtnSubmit, hFontUI);
        apply(hBtnBack, hFontUI);
    }
};

ModernApp app;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_CREATE: {
        app.hwnd = hwnd;
        app.hStaticTitle = CreateWindowW(L"STATIC", L"Main Menu", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 0, 0, 0, 0, hwnd, NULL, NULL, NULL);
        // 1. Define your unique names in a list
        std::vector<std::wstring> buttonLabels = {
            L"Calculate Gamma Factor",
            L"Calculate Time Difference",
            L"Latest Calculations",
            L"Settings"
        };

        // 2. Loop through the names to create buttons
        for (size_t i = 0; i < buttonLabels.size(); ++i) {
            app.menuButtons.push_back(CreateWindowExW(
                0,
                L"BUTTON",
                buttonLabels[i].c_str(), // Use the specific name from the list
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                0, 0, 0, 0,
                hwnd,
                (HMENU)(INT_PTR)(ID_OPTION1 + i),
                NULL,
                NULL
            ));
        }

        app.hEdit1 = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | ES_AUTOHSCROLL, 0, 0, 0, 0, hwnd, NULL, NULL, NULL);
        app.hBtnSubmit = CreateWindowW(L"BUTTON", L"Execute", WS_CHILD, 0, 0, 0, 0, hwnd, (HMENU)ID_SUBMIT, NULL, NULL);
        app.hBtnBack = CreateWindowW(L"BUTTON", L"Go Back", WS_CHILD, 0, 0, 0, 0, hwnd, (HMENU)ID_BACK, NULL, NULL);
        return 0;
    }
    case WM_CTLCOLORSTATIC: {
        HDC hdc = (HDC)wp;
        SetTextColor(hdc, RGB(40, 40, 40));
        SetBkMode(hdc, TRANSPARENT);
        return (LRESULT)app.hHeaderBrush; // Gives the title a clean white background
    }
    case WM_SIZE:
        app.UpdateLayout();
        break;
    case WM_COMMAND: {
        int id = LOWORD(wp);
        if (id >= ID_OPTION1 && id <= ID_OPTION4) {
            SetWindowTextW(app.hStaticTitle, L"Enter Parameters");
            for (auto b : app.menuButtons) ShowWindow(b, SW_HIDE);
            ShowWindow(app.hEdit1, SW_SHOW); ShowWindow(app.hBtnSubmit, SW_SHOW); ShowWindow(app.hBtnBack, SW_SHOW);
        }
        if (id == ID_BACK) {
            SetWindowTextW(app.hStaticTitle, L"Control Panel");
            for (auto b : app.menuButtons) ShowWindow(b, SW_SHOW);
            ShowWindow(app.hEdit1, SW_HIDE); ShowWindow(app.hBtnSubmit, SW_HIDE); ShowWindow(app.hBtnBack, SW_HIDE);
        }
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nShow) {
    InitCommonControls();

    // C++20 Designated Initializers
    WNDCLASSW wc = {
        .lpfnWndProc = WindowProc,
        .hInstance = hInst,
        .hCursor = LoadCursor(NULL, IDC_ARROW),
        .hbrBackground = app.hBackBrush,
        .lpszClassName = L"ModernX64App"
    };
    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(WS_EX_APPWINDOW, L"ModernX64App", L"Einstein's Playground",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 400, NULL, NULL, hInst, NULL);

    ShowWindow(hwnd, nShow);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}