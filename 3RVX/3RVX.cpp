// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "3RVX.h"
#include "HideWin10VolumeOSD.h"

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "Wtsapi32.lib")

#include <ctime>
#include <gdiplus.h>
#include <iostream>
#include <Wtsapi32.h>

#include "DisplayManager.h"
#include "HotkeyManager.h"
#include "Logger.h"
#include "OSD/OSD.h"
#include "OSD/VolumeOSD.h"
#include "Settings.h"
#include "Skin/AccentColor.h"
#include "Skin/SkinManager.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {

    Logger::Start();

    QCLOG(L"  _____ ______     ____  _______ ");
    QCLOG(L" |___ /|  _ \\ \\   / /\\ \\/ /___ / ");
    QCLOG(L"   |_ \\| |_) \\ \\ / /  \\  /  |_ \\ ");
    QCLOG(L"  ___) |  _ < \\ V /   /  \\ ___) |");
    QCLOG(L" |____/|_| \\_\\ \\_/   /_/\\_\\____/ ");
    QCLOG(L"");

    HANDLE mutex;
    mutex = CreateMutex(NULL, FALSE, L"Local\\3RVX");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        if (mutex) {
            ReleaseMutex(mutex);
        }

        CLOG(L"A previous instance of the program is already running.");

#if defined(ENABLE_3RVX_LOG) && (defined(ENABLE_3RVX_LOGTOFILE) == FALSE)
        CLOG(L"Press [enter] to terminate");
        std::cin.get();
#endif

        return EXIT_SUCCESS;
    }

    CLOG(L"App directory: %s", Settings::AppDir().c_str());

    using namespace Gdiplus;
    ULONG_PTR gdiplusToken;
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    _3RVX mainWnd(hInstance);

    HRESULT hr = CoInitializeEx(NULL,
        COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (hr != S_OK) {
        CLOG(L"Failed to initialize the COM library.");
        return EXIT_FAILURE;
    }

    /* Tell the program to initialize */
    PostMessage(mainWnd.Handle(), _3RVX::WM_3RVX_CTRL, _3RVX::MSG_LOAD, NULL);

    /* Register for session change notifications */
    WTSRegisterSessionNotification(mainWnd.Handle(), NOTIFY_FOR_THIS_SESSION);

    /* Start the event loop */
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    CoUninitialize();

    Logger::Stop();

    return (int) msg.wParam;
}

_3RVX::_3RVX(HINSTANCE hInstance) :
Window(_3RVX::CLASS_3RVX, _3RVX::CLASS_3RVX, hInstance),
_vOSD(nullptr),
_hkManager(nullptr) {

}

void _3RVX::Initialize() {
    CLOG(L"Initializing...");

    delete _vOSD;
    _vOSD = nullptr;

    Settings *settings = Settings::Instance();
    settings->Load();

    SkinManager::Instance()->LoadSkin(settings->SkinXML());

    DisplayManager::UpdateMonitorMap();

    /* OSDs */
    _vOSD = new VolumeOSD();
    _vOSD->Enabled(settings->VolumeOSDEnabled());

    _osds.clear();
    _osds.push_back(_vOSD);

    /* Hotkey setup */
    if (_hkManager != NULL) {
        _hkManager->Shutdown();
    }
    _hkManager = HotkeyManager::Instance(Handle());

    _hotkeys = Settings::Instance()->Hotkeys();
    for (auto it = _hotkeys.begin(); it != _hotkeys.end(); ++it) {
        /* Enable arg caching */
        it->second.EnableArgCache();

        int combination = it->first;
        _hkManager->Register(combination);
    }
}

void _3RVX::ProcessHotkeys(HotkeyInfo &hki) {
    switch (hki.action) {
    case HotkeyInfo::IncreaseVolume:
    case HotkeyInfo::DecreaseVolume:
    case HotkeyInfo::SetVolume:
    case HotkeyInfo::Mute:
    case HotkeyInfo::VolumeSlider:
    case HotkeyInfo::IncreaseMonitorVolume:
    case HotkeyInfo::DecreaseMonitorVolume:
        if (_vOSD) {
            _vOSD->ProcessHotkeys(hki);
        }
        break;

    case HotkeyInfo::DisableOSD:
        ToggleOSDs();
        break;

    case HotkeyInfo::Settings:
        ShellExecute(NULL, L"open", Settings::SettingsApp().c_str(),
            NULL, NULL, SW_SHOWNORMAL);
        break;

    case HotkeyInfo::Exit:
        SendMessage(Handle(), WM_CLOSE, NULL, NULL);
        break;
    }
}

void _3RVX::ToggleOSDs() {
    for (OSD *osd : _osds) {
        osd->Enabled(!(osd->Enabled()));
    }
}

LRESULT _3RVX::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_HOTKEY: {
        CLOG(L"Hotkey: %d", (int) wParam);
        auto hotkey = _hotkeys.find((int) wParam);
        if (hotkey == _hotkeys.end()) {
            CLOG(L"Ignoring unknown hotkey id: %d", (int) wParam);
            break;
        }

        HotkeyInfo hki = hotkey->second;
        ProcessHotkeys(hki);
        break;
    }

    case WM_DISPLAYCHANGE: {
        CLOG(L"Detected display change");
        DisplayManager::UpdateMonitorMap();
        for (OSD *osd : _osds) {
            osd->OnDisplayChange();
        }
        break;
    }

    case WM_DWMCOLORIZATIONCOLORCHANGED: {
        /* While this message contains the colorization color, we need to
         * refresh here to handle situations where the user has requested a
         * different color (undocumented colorization color, for example). */
        CLOG(L"Received DWM colorization change notification: %x",
            (unsigned int) wParam);
        AccentColor::Instance()->Refresh();
    }

    case WM_WTSSESSION_CHANGE: {
        CLOG(L"Detected session change");
        break;
    }

    case WM_CLOSE: {
        CLOG(L"Shutting down");
        HideWin10VolumeOSD::ShowOSD();
        HotkeyManager::Instance()->Shutdown();
        if (_vOSD) {
            _vOSD->HideIcon();
        }
        break;
    }

    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }

    }

    if (message == _3RVX::WM_3RVX_CTRL) {
        switch (wParam) {
        case _3RVX::MSG_LOAD:
            Initialize();
            HideWin10VolumeOSD::Init();
            break;

        case _3RVX::MSG_SETTINGS:
            Settings::LaunchSettingsApp();
            break;

        case _3RVX::MSG_HIDEOSD:
            int except = (OSDType) lParam;
            switch (except) {
            case Volume:
                break;
            }

            break;
        }
    }

    return Window::WndProc(hWnd, message, wParam, lParam);
}
