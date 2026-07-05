#include "AccentColor.h"

#include <dwmapi.h>
#include <VersionHelpers.h>

#include "../Logger.h"

AccentColor *AccentColor::instance = nullptr;

AccentColor::AccentColor() :
_dwmLib(nullptr) {
    Refresh();
}

AccentColor::~AccentColor() {
    FreeModule(_dwmLib);
}

AccentColor *AccentColor::Instance() {
    if (instance == nullptr) {
        instance = new AccentColor();
    }

    return instance;
}

UINT32 AccentColor::Color() const {
    return _color;
}

void AccentColor::Color(UINT32 color) {
    _color = color;
    _override = true;
}

void AccentColor::UseSystemColor(bool enable) {
    _override = !enable;
    Refresh();
}

void AccentColor::Refresh() {
    if (_override == true) {
        return;
    }

    INT64 registryColor = RegistryAccentColor();
    if (registryColor >= 0) {
        _color = (UINT32) registryColor;
        return;
    }

    if (IsWindows7OrGreater()) {
        if (_useUndocumented == true) {
            INT64 color = ColorizationParamColor();
            if (color >= 0) {
                _color = (UINT32) color;
                return;
            }
            /* If an error occurs (-1), fall through to the next technique. */
        }

        DWORD color;
        BOOL opaque;
        HRESULT hr = DwmGetColorizationColor(&color, &opaque);
        if (SUCCEEDED(hr)) {
            _color = color;
            return;
        }
    }

    /* Our last hope if both methods above failed: */
    DWORD caption = GetSysColor(COLOR_ACTIVECAPTION);
    _color = ColorRefToArgb(caption);
}

INT64 AccentColor::RegistryAccentColor() {
    HKEY key = NULL;
    LONG result = RegOpenKeyEx(
        HKEY_CURRENT_USER,
        L"Software\\Microsoft\\Windows\\DWM",
        0,
        KEY_READ,
        &key);
    if (result != ERROR_SUCCESS) {
        return -1;
    }

    DWORD color = 0;
    DWORD type = REG_DWORD;
    DWORD size = sizeof(color);
    result = RegQueryValueEx(
        key,
        L"AccentColor",
        NULL,
        &type,
        reinterpret_cast<LPBYTE>(&color),
        &size);
    RegCloseKey(key);

    if (result != ERROR_SUCCESS || type != REG_DWORD) {
        return -1;
    }

    return (INT64) AbgrToArgb(color);
}

UINT32 AccentColor::AbgrToArgb(DWORD color) {
    UINT32 red = color & 0x000000FF;
    UINT32 green = (color & 0x0000FF00);
    UINT32 blue = (color & 0x00FF0000) >> 16;
    return 0xFF000000 | (red << 16) | green | blue;
}

UINT32 AccentColor::ColorRefToArgb(DWORD color) {
    UINT32 red = color & 0x000000FF;
    UINT32 green = (color & 0x0000FF00);
    UINT32 blue = (color & 0x00FF0000) >> 16;
    return 0xFF000000 | (red << 16) | green | blue;
}

/* WARNING: This method uses an undocumented API. May crash in the future. */
INT64 AccentColor::ColorizationParamColor() {
    struct DwmColorizationParams {
        UINT ColorizationColor;
        UINT ColorizationAfterglow;
        UINT ColorizationColorBalance;
        UINT ColorizationAfterglowBalance;
        UINT ColorizationBlurBalance;
        UINT ColorizationGlassReflectionIntensity;
        UINT ColorizationOpaqueBlend;
    };

    if (_dwmLib == NULL) {
        /* May be the first time executing this method */
        _dwmLib = LoadLibrary(L"dwmapi.dll");
        if (_dwmLib == NULL) {
            CLOG(L"Could not load DWM library");
            return -1;
        }
    }

    HRESULT(WINAPI *DwmGetColorizationParameters)
        (DwmColorizationParams *colorParams);
    const int ColorizationParamOrd = 127;

    *(FARPROC *) &DwmGetColorizationParameters
        = GetProcAddress(_dwmLib, (LPCSTR) ColorizationParamOrd);
    if (DwmGetColorizationParameters == nullptr) {
        CLOG(L"Could not retrieve DwmGetColorizationParameters address");
        return -1;
    }

    DwmColorizationParams params = { 0 };
    HRESULT hr = DwmGetColorizationParameters(&params);
    if (FAILED(hr)) {
        CLOG(L"Could not retrieve DWM colorization parameters");
        return -1;
    }

    return (INT64) params.ColorizationColor;
}
