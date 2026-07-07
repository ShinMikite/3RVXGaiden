// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "SettingsTab.h"

#include "../../3RVX/LanguageTranslator.h"
#include "../../3RVX/Settings.h"

SettingsTab::SettingsTab(
        HINSTANCE hInstance, LPCWSTR tabTemplate, LPCWSTR title) :
TabPage(hInstance, tabTemplate,
    Settings::Instance()->Translator()->Translate(title).c_str()) {

}

SettingsTab::~SettingsTab() {

}

INT_PTR SettingsTab::DialogProc(
        HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    switch (uMsg) {
    case WM_INITDIALOG:
        _loading = true;
        Initialize();
        LoadSettings();
        PropSheet_UnChanged(GetParent(hwndDlg), hwndDlg);
        _loading = false;
        return FALSE;

    case WM_COMMAND:
        if (_loading == false) {
            PropSheet_Changed(GetParent(hwndDlg), NULL);
        }
        break;
    }

    return Dialog::DialogProc(hwndDlg, uMsg, wParam, lParam);
}
