#include "UI.h"
#include <commctrl.h>
UI::UI() {
    hBackBrush = CreateSolidBrush(RGB(250, 250, 252));
    hHeaderBrush = CreateSolidBrush(RGB(255, 255, 255));

}

UI::~UI() {
    if (hFontUI) DeleteObject(hFontUI);
    if (hFontTitle) DeleteObject(hFontTitle);
    if (hBackBrush) DeleteObject(hBackBrush);
    if (hHeaderBrush) DeleteObject(hHeaderBrush);
}

void UI::Init(HWND parentHwnd) {
    hwnd = parentHwnd;
    hStaticTitle = CreateWindowW(L"STATIC", L"Einstein's Playground", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 0, 0, 0, 0, hwnd, NULL, NULL, NULL);
    std::vector<std::wstring> labels = {  L"Time Dilation", L"Latest Logs", L"Settings" };
    for (int i = 0; i < 3; ++i) {
        menuButtons.push_back(CreateWindowW(L"BUTTON", labels[i].c_str(), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, hwnd, (HMENU)(INT_PTR)(TIME_DILATION + i), NULL, NULL));
    }
    std::vector<std::wstring> timeDilationButtonsLabels = {L"Gamma Factor Calculator", L"Back"};
    for (int i = 0; i < 2; ++i) {
        timeDilationButtons.push_back(CreateWindowW(L"BUTTON",timeDilationButtonsLabels[i].c_str(),WS_VISIBLE | WS_CHILD,0,0,0,0,hwnd,(HMENU)(INT_PTR)(TIME_DILATION+199*i),NULL,NULL));
    }
    hGammaCalculator = CreateWindowExW(WS_EX_APPWINDOW,L"BUTTON", L"Gamma Factor Calculator",WS_CHILD,0,0,0,0,hwnd,(HMENU)CALC_GAMMA,NULL,NULL);
}

void UI::ShowCalculation(const std::wstring& title, const std::wstring& cue) {
    SetWindowTextW(hStaticTitle, title.c_str());
    SendMessage(CalcButtons.hEditInput, EM_SETCUEBANNER, FALSE, (LPARAM)cue.c_str());
    SetWindowTextW(hEditResult, L"Result:");
    for (auto b : menuButtons) ShowWindow(b, SW_HIDE);
    ShowWindow(CalcButtons.hEditInput, SW_SHOW); ShowWindow(CalcButtons.hBtnSubmit, SW_SHOW); ShowWindow(CalcButtons.hBtnBack, SW_SHOW);
    ShowWindow(CalcButtons.hBtnInfo, SW_SHOW);ShowWindow(CalcButtons.hEditResult, SW_SHOW); ShowWindow(CalcButtons.hBtnToggle, SW_SHOW);
}

void UI::ShowMenu() {
    SetWindowTextW(hStaticTitle, L"Einstein's Playground");
    SendMessage(hEdit1, EM_SETREADONLY, FALSE, 0);
    SetWindowTextW(hEdit1, L"");
    SetWindowTextW(hEditResult, L"");
    for (auto b : menuButtons) ShowWindow(b, SW_SHOW);
    ShowWindow(hEdit1, SW_HIDE); ShowWindow(hBtnSubmit, SW_HIDE); ShowWindow(hBtnBack, SW_HIDE); ShowWindow(hGammaCalculator, SW_HIDE);
    ShowWindow(hEditResult, SW_HIDE); ShowWindow(hBtnToggle, SW_HIDE);
}

void UI::ShowHelp(bool inverse) {
    if (inverse) {
        MessageBoxW(hwnd, L"You are currently on Gamma translation to velocity mode! \n\nGamma can't be lower than 1.\n\nSimply enter Gamma in numbers and get your speed in M/S!" , L"Help", MB_OK | MB_ICONINFORMATION);
        return;
    }
    MessageBoxW(hwnd, L"Enter velocity in m/s. Must be 0 <= v < c.\nC = 299,792,458 m/s\nThere are 3 ways to enter velocity!\n\n1. You can enter percentage of C. \nFor example: 90% will translate to 269,813,212 m/s \n\n 2. "
                      "You can enter numbers with letter K, or M after them \n k translate for 1,000 and M to 1,000,000 so 8.2K = 8,200 m/s\n\n 3. You can literally type the speed In meters (just numbers)  ", L"Help", MB_OK | MB_ICONINFORMATION);
}
void UI::Font_Update() {
    RECT r;
    GetClientRect(hwnd, &r);
    int w = r.right;
    int h = r.bottom;

    // --- NEW: Optimization Check ---
    // If the window size hasn't changed, don't recreate everything!
    static int lastW = 0, lastH = 0;
    if (w == lastW && h == lastH) return;
    lastW = w; lastH = h;
    float sx = w / 500.0f;
    float sy = h / 400.0f;
    float s = (sx < sy) ? sx : sy;
    if (hFontUI) DeleteObject(hFontUI);
    if (hFontTitle) DeleteObject(hFontTitle);
    if (hFontToggle) DeleteObject(hFontToggle);

    hFontUI = CreateFontW((int)(18 * s), 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE,
                          DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, L"Segoe UI");
    hFontTitle = CreateFontW((int)(28 * s), 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                            DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, L"Segoe UI");
    hFontToggle = CreateFontW((int)(24 * s), 0, 0, 0, FW_HEAVY, FALSE, FALSE, FALSE,
                             DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, L"Segoe UI Symbol");
}
void UI::ApplyFonts(const std::vector<HWND>& controls, HFONT font) {
    for (auto h : controls) {
        if (h) SendMessage(h, WM_SETFONT, (WPARAM)font, TRUE);
    }
}
void UI::UpdateLayout() {
    // 1. Safety Check: If buttons aren't created yet, don't try to move them
    if (menuButtons.empty() || !hwnd) return;
    RECT r;
    GetClientRect(hwnd, &r);
    int w = r.right;
    int h = r.bottom;

    // --- NEW: Optimization Check ---
    // If the window size hasn't changed, don't recreate everything!
    static int lastW = 0, lastH = 0;
    if (w == lastW && h == lastH) return;
    lastW = w; lastH = h;
    // -------------------------------
    // 2. Scaling factors (500x400 is our base design size)
    float sx = w / 500.0f;
    float sy = h / 400.0f;

    // 4. Position: Header Area
    MoveWindow(hStaticTitle, 0, 0, w, (int)(80 * sy), TRUE);
    // 5. Position: Menu Buttons (Grid layout)
    int btnW = (int)(160 * sx), btnH = (int)(50 * sy);
    int centerX = w / 2;
    //Time dilation button
    MoveWindow(menuButtons[0], centerX - btnW/2, (int)(120 * sy), btnW, btnH, TRUE);
    //settings,latest logs buttons.
    MoveWindow(menuButtons[1], centerX + 10,        (int)(180 * sy), btnW, btnH, TRUE);
    MoveWindow(menuButtons[2], centerX - btnW - 10, (int)(180 * sy), btnW, btnH, TRUE);

    // 6. Position: Form Controls (Input mode)
    int editW = (int)(320 * sx);

    MoveWindow(hEdit1, centerX - ((editW-140) / 2), (int)(130 * sy), editW-160, (int)(35 * sy), TRUE);

    // The "i" button sits exactly to the right of the input box
    MoveWindow(hBtnBack, centerX + (int)(30 * sx), (int)(275 * sy), (int)(100 * sx), (int)(40 * sy), TRUE);
    MoveWindow(hBtnToggle, centerX + (editW / 2) + 5, (int)(180 * sy), (int)(40 * sx), (int)(35 * sy), TRUE);
    MoveWindow(hGammaCalculator,centerX - btnW + (int)(10 * sx),(int)(275 * sy),(int)(150 * sx),(int)(40 * sy), TRUE);

    Font_Update();
    ApplyFonts({hEdit1, hEditResult, hBtnSubmit, hBtnBack, hGammaCalculator}, hFontUI);
    ApplyFonts({hStaticTitle}, hFontTitle);
    ApplyFonts({hBtnToggle}, hFontToggle);
    ApplyFonts(menuButtons, hFontUI);

    // Force a repaint so the changes show up immediately
    InvalidateRect(hwnd, NULL, TRUE);
}
void UI::InitCalculatorWindow(HWND calcHwnd) {
    CalcButtons.hwnd = calcHwnd;
    CalcButtons.hEditInput = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | ES_AUTOHSCROLL, 20, 50, 300, 35, calcHwnd, NULL, NULL, NULL);
    CalcButtons.hBtnSubmit = CreateWindowW(L"BUTTON", L"Calculate", WS_CHILD | WS_VISIBLE, 20, 100, 100, 40, calcHwnd, (HMENU)ID_SUBMIT, NULL, NULL);
    CalcButtons.hBtnBack = CreateWindowW(L"BUTTON", L"Back", WS_CHILD, 0, 0, 0, 0, calcHwnd, (HMENU)ID_BACK, NULL, NULL);
    CalcButtons.hBtnInfo = CreateWindowW(L"BUTTON", L"i", WS_CHILD, 0, 0, 0, 0, calcHwnd, (HMENU)ID_INFO, NULL, NULL);
    CalcButtons.hBtnToggle = CreateWindowW(L"BUTTON", L"⇅", WS_CHILD | BS_PUSHBUTTON,
    0, 0, 0, 0, hwnd, (HMENU)ID_TOGGLE, NULL, NULL);
    hEditResult = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
    WS_CHILD | ES_READONLY | ES_CENTER, 20, 150, 300, 35, calcHwnd, NULL, NULL, NULL);
    // Set titles/cues immediately
}
void::UI::showTimeDilation(const std::wstring& title ) {
    SetWindowTextW(hStaticTitle, title.c_str());
    for (auto b : menuButtons) ShowWindow(b, SW_HIDE);
    ShowWindow(hBtnBack, SW_SHOW); ShowWindow(hGammaCalculator, SW_SHOW);
}
