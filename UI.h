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
    ID_OPTION1 = 101, ID_OPTION2, ID_OPTION3, ID_OPTION4,
    ID_SUBMIT = 200, ID_BACK = 300, ID_INFO = 400  , ID_TOGGLE = 500
};

class UI {
public:
    HWND hwnd = nullptr, hEditResult = nullptr, hStaticTitle = nullptr, hEdit1 = nullptr, hBtnSubmit = nullptr, hBtnBack = nullptr, hBtnInfo = nullptr;
    std::vector<HWND> menuButtons;

    HFONT hFontUI = nullptr, hFontTitle = nullptr , hFontToggle = nullptr;
    HBRUSH hBackBrush = nullptr, hHeaderBrush = nullptr;
    bool isInverseMode = false;
    HWND hBtnToggle = nullptr;


    UI();
    ~UI();

    void Init(HWND parentHwnd);
    void UpdateLayout();

    // UI "Scenes"
    void ShowCalculation(const std::wstring& title, const std::wstring& cue);
    void ShowMenu();
    void ShowHelp(bool inverse); // The "i" button logic
};

#endif
#endif //EINSTEIN_S_PLAYGROUND_UI_H