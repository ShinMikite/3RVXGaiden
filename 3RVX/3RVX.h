// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <Windows.h>
#include <unordered_map>

#include "HotkeyInfo.h"
#include "Window.h"

class OSD;
class VolumeOSD;

class HotkeyManager;

class _3RVX : public Window {
public:
    _3RVX(HINSTANCE hInstance);

private:
    std::vector<OSD *> _osds;
    VolumeOSD *_vOSD;

    HotkeyManager *_hkManager;
    std::unordered_map<int, HotkeyInfo> _hotkeys;

    void Initialize();
    void ProcessHotkeys(HotkeyInfo &hki);
    void ToggleOSDs();
    virtual LRESULT WndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);

public:
    static HWND MasterHwnd() {
        return FindWindow(CLASS_3RVX, CLASS_3RVX);
    }

    static HWND MasterSettingsHwnd() {
        return FindWindow(CLASS_3RVX_SETTINGS, CLASS_3RVX_SETTINGS);
    }

    static void Message(WPARAM wParam, LPARAM lParam, bool post = false) {
        HWND masterWnd = MasterHwnd();
        if (masterWnd == NULL) {
            return;
        }

        if (post) {
            PostMessage(masterWnd, WM_3RVX_CTRL, wParam, lParam);
        } else {
            DWORD_PTR result = 0;
            SendMessageTimeout(masterWnd, WM_3RVX_CTRL, wParam, lParam,
                SMTO_ABORTIFHUNG, 5000, &result);
        }
    }

    static void SettingsMessage(
            WPARAM wParam, LPARAM lParam, bool post = false) {
        HWND masterWnd = MasterSettingsHwnd();
        if (masterWnd == NULL) {
            return;
        }

        if (post) {
            PostMessage(masterWnd, WM_3RVX_SETTINGSCTRL, wParam, lParam);
        } else {
            DWORD_PTR result = 0;
            SendMessageTimeout(masterWnd, WM_3RVX_SETTINGSCTRL, wParam, lParam,
                SMTO_ABORTIFHUNG, 5000, &result);
        }
    }

public:
    static constexpr const wchar_t *CLASS_3RVX = L"3RVXv3";
    static constexpr const wchar_t *CLASS_3RVX_SETTINGS = L"3RVXv3-Settings";

    static const UINT WM_3RVX_CTRL;
    static const UINT WM_3RVX_SETTINGSCTRL;

public:
    /* 3RVX Messages */
    static const int MSG_LOAD = WM_APP + 100;
    static const int MSG_SETTINGS = WM_APP + 101;
    static const int MSG_EXIT = WM_APP + 102;
    static const int MSG_HIDEOSD = WM_APP + 103;

    /* Settings App Messages */
    static const int MSG_ACTIVATE = WM_APP + 200;
    static const int MSG_MUSTRESTART = WM_APP + 201;
    static const int MSG_SAVESETTINGS = WM_APP + 203;
};
