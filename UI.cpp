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
    hEditResult = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
    WS_CHILD | ES_READONLY | ES_CENTER, 0, 0, 0, 0, hwnd, NULL, NULL, NULL);
    std::vector<std::wstring> labels = {  L"Time Dilation", L"Latest Logs", L"Settings" };
    for (int i = 0; i < 3; ++i) {
        menuButtons.push_back(CreateWindowW(L"BUTTON", labels[i].c_str(), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, hwnd, (HMENU)(INT_PTR)(TIME_DILATION + i), NULL, NULL));
    }


    hEdit1 = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | ES_AUTOHSCROLL, 0, 0, 0, 0, hwnd, NULL, NULL, NULL);
    hBtnSubmit = CreateWindowW(L"BUTTON", L"Calculate", WS_CHILD, 0, 0, 0, 0, hwnd, (HMENU)ID_SUBMIT, NULL, NULL);
    hBtnBack = CreateWindowW(L"BUTTON", L"Back", WS_CHILD, 0, 0, 0, 0, hwnd, (HMENU)ID_BACK, NULL, NULL);
    hBtnInfo = CreateWindowW(L"BUTTON", L"i", WS_CHILD, 0, 0, 0, 0, hwnd, (HMENU)ID_INFO, NULL, NULL);
    hBtnToggle = CreateWindowW(L"BUTTON", L"⇅", WS_CHILD | BS_PUSHBUTTON,
    0, 0, 0, 0, hwnd, (HMENU)ID_TOGGLE, NULL, NULL);

}

void UI::ShowCalculation(const std::wstring& title, const std::wstring& cue) {
    SetWindowTextW(hStaticTitle, title.c_str());
    SendMessage(hEdit1, EM_SETCUEBANNER, FALSE, (LPARAM)cue.c_str());
    SetWindowTextW(hEditResult, L"Result:");
    for (auto b : menuButtons) ShowWindow(b, SW_HIDE);
    ShowWindow(hEdit1, SW_SHOW); ShowWindow(hBtnSubmit, SW_SHOW); ShowWindow(hBtnBack, SW_SHOW); ShowWindow(hBtnInfo, SW_SHOW); ShowWindow(hEditResult, SW_SHOW); ShowWindow(hBtnToggle, SW_SHOW);
}

void UI::ShowMenu() {
    SetWindowTextW(hStaticTitle, L"Einstein's Playground");
    SendMessage(hEdit1, EM_SETREADONLY, FALSE, 0);
    SetWindowTextW(hEdit1, L"");
    SetWindowTextW(hEditResult, L"");
    for (auto b : menuButtons) ShowWindow(b, SW_SHOW);
    ShowWindow(hEdit1, SW_HIDE); ShowWindow(hBtnSubmit, SW_HIDE); ShowWindow(hBtnBack, SW_HIDE); ShowWindow(hBtnInfo, SW_HIDE); ShowWindow(hEditResult, SW_HIDE); ShowWindow(hBtnToggle, SW_HIDE);;
}

void UI::ShowHelp(bool inverse) {
    if (inverse) {
        MessageBoxW(hwnd, L"You are currently on Gamma translation to velocity mode! \n\nGamma can't be lower than 1.\n\nSimply enter Gamma in numbers and get your speed in M/S!" , L"Help", MB_OK | MB_ICONINFORMATION);
        return;
    }
    MessageBoxW(hwnd, L"Enter velocity in m/s. Must be 0 <= v < c.\nC = 299,792,458 m/s\nThere are 3 ways to enter velocity!\n\n1. You can enter percentage of C. \nFor example: 90% will translate to 269,813,212 m/s \n\n 2. "
                      "You can enter numbers with letter K, or M after them \n k translate for 1,000 and M to 1,000,000 so 8.2K = 8,200 m/s\n\n 3. You can literally type the speed In meters (just numbers)  ", L"Help", MB_OK | MB_ICONINFORMATION);
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
    float s = (sx < sy) ? sx : sy; // Use the smaller ratio for fonts to prevent overflow

    // 3. Recreate Fonts based on new scale
    if (hFontUI) DeleteObject(hFontUI);
    if (hFontTitle) DeleteObject(hFontTitle);
    if (hFontToggle) DeleteObject(hFontToggle);

    hFontUI = CreateFontW((int)(18 * s), 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE,
                          DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, L"Segoe UI");
    hFontTitle = CreateFontW((int)(28 * s), 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                            DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, L"Segoe UI");
    hFontToggle = CreateFontW((int)(24 * s), 0, 0, 0, FW_HEAVY, FALSE, FALSE, FALSE,
                             DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, L"Segoe UI Symbol");

    // 4. Position: Header Area
    MoveWindow(hStaticTitle, 0, 0, w, (int)(80 * sy), TRUE);

    // 5. Position: Menu Buttons (Grid layout)
    int btnW = (int)(160 * sx), btnH = (int)(50 * sy);
    int centerX = w / 2;

    // Top row
    MoveWindow(menuButtons[0], centerX - btnW - 10, (int)(120 * sy), btnW, btnH, TRUE);
    MoveWindow(menuButtons[1], centerX + 10,        (int)(120 * sy), btnW, btnH, TRUE);
    // Bottom row
    MoveWindow(menuButtons[2], centerX - btnW - 10, (int)(180 * sy), btnW, btnH, TRUE);
    MoveWindow(menuButtons[3], centerX + 10,        (int)(180 * sy), btnW, btnH, TRUE);

    // 6. Position: Form Controls (Input mode)
    int editW = (int)(320 * sx);
    int infoSize = (int)(35 * sy); // Match height of edit box

    MoveWindow(hEdit1, centerX - ((editW-140) / 2), (int)(130 * sy), editW-160, (int)(35 * sy), TRUE);

    // The "i" button sits exactly to the right of the input box
    MoveWindow(hBtnInfo, centerX + ((editW-180) / 2) + 5, (int)(130 * sy), infoSize, (int)(35 * sy), TRUE);
    MoveWindow(hEditResult, centerX - (editW / 2), (int)(180 * sy), editW, (int)(35 * sy), TRUE);
    MoveWindow(hBtnSubmit, centerX - (int)(110 * sx), (int)(225 * sy), (int)(100 * sx), (int)(40 * sy), TRUE);
    MoveWindow(hBtnBack, centerX + (int)(10 * sx), (int)(225 * sy), (int)(100 * sx), (int)(40 * sy), TRUE);
    MoveWindow(hBtnToggle, centerX + (editW / 2) + 5, (int)(180 * sy), (int)(40 * sx), (int)(35 * sy), TRUE);

    // 7. Apply the new Fonts
    auto apply = [&](HWND h, HFONT f) {
        if(h) SendMessage(h, WM_SETFONT, (WPARAM)f, TRUE);
    };

    apply(hStaticTitle, hFontTitle);
    apply(hEdit1, hFontUI);
    apply(hEditResult, hFontUI);
    apply(hBtnSubmit, hFontUI);
    apply(hBtnBack, hFontUI);
    apply(hBtnInfo, hFontUI);
    apply(hBtnToggle, hFontToggle); // Use the heavy symbol font here

    for (auto b : menuButtons) apply(b, hFontUI);

    // Force a repaint so the changes show up immediately
    InvalidateRect(hwnd, NULL, TRUE);
}