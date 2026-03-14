//
// Created by Dumbledore on 3/5/2026.
//

#ifndef EINSTEIN_S_PLAYGROUND_UI_H
#define EINSTEIN_S_PLAYGROUND_UI_H
#ifndef UI_H
#define UI_H

#include <windows.h>
#include <vector>
#include <string>

// Global IDs
enum {
    TIME_DILATION = 101, SETTINGS, LOGS,
    ID_SUBMIT = 200, ID_BACK = 300, ID_INFO = 400  , ID_TOGGLE = 500 , CALC_GAMMA = 600
};
struct CalculatorControls {
    HWND hEditResult = nullptr,hEditInput = nullptr, hBtnSubmit = nullptr, hBtnBack = nullptr, hBtnInfo = nullptr, hwnd = nullptr,hBtnToggle, hStaticTitle = nullptr;
};

class UI {
public:
    CalculatorControls CalcButtons;
    HWND hwnd = nullptr, hGammaCalculator = nullptr, hEditResult = nullptr, hStaticTitle = nullptr, hEdit1 = nullptr, hBtnSubmit = nullptr, hBtnBack = nullptr;
    std::vector<HWND> menuButtons;
    std::vector<HWND> timeDilationButtons;
    HFONT hFontUI = nullptr, hFontTitle = nullptr , hFontToggle = nullptr;
    HBRUSH hBackBrush = nullptr, hHeaderBrush = nullptr;
    bool isInverseMode = false;
    HWND hBtnToggle = nullptr;

    int winW, winH;          // current window size
    int btnW, btnH;          // button sizes
    int spacing;             // spacing between buttons
    int headerH;


    UI();
    ~UI();

    void Init(HWND parentHwnd);
    void InitCalculatorWindow(HWND calcHwnd);
    void UpdateLayout();
    void UpdateLayoutCalculator();
    void Font_Update();
    void ApplyFonts(const std::vector<HWND>& controls, HFONT font);

    // UI "Scenes"
    void ShowCalculation(const std::wstring& title, const std::wstring& cue);
    void showTimeDilation(const std::wstring& title);
    void ShowMenu();
    void ShowHelp(bool inverse); // The "i" button logic
};

#endif
#endif //EINSTEIN_S_PLAYGROUND_UI_H