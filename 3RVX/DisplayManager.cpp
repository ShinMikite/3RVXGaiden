// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "DisplayManager.h"

#include <algorithm>
#include <d3d9.h>
#include <vector>
#pragma comment(lib, "d3d9.lib")
#include "Logger.h"

static std::unordered_map<std::wstring, Monitor> monitorMap;

static std::wstring BoundedDisplayString(const wchar_t *text, size_t maxLength) {
    return std::wstring(text, wcsnlen_s(text, maxLength));
}

static std::wstring TrimDisplayString(std::wstring text) {
    const wchar_t *trimChars = L" \t\r\n";
    size_t first = text.find_first_not_of(trimChars);
    if (first == std::wstring::npos) {
        return L"";
    }

    size_t last = text.find_last_not_of(trimChars);
    return text.substr(first, last - first + 1);
}

static std::wstring MonitorRegistryInstance(std::wstring deviceId) {
    std::wstring id = deviceId;

    size_t displayPath = id.find(L"DISPLAY#");
    if (displayPath != std::wstring::npos) {
        id = id.substr(displayPath + 8);
        size_t classGuid = id.find(L"#{");
        if (classGuid != std::wstring::npos) {
            id = id.substr(0, classGuid);
        }
        std::replace(id.begin(), id.end(), L'#', L'\\');
        return id;
    }

    displayPath = id.find(L"DISPLAY\\");
    if (displayPath != std::wstring::npos) {
        id = id.substr(displayPath + 8);
    }

    size_t suffix = id.rfind(L"_0");
    if (suffix != std::wstring::npos && suffix == id.length() - 2) {
        id = id.substr(0, suffix);
    }

    return id;
}

static std::wstring EdidMonitorName(const std::vector<BYTE> &edid) {
    if (edid.size() < 128) {
        return L"";
    }

    for (size_t offset = 54; offset + 18 <= edid.size(); offset += 18) {
        bool descriptor = edid[offset] == 0x00
            && edid[offset + 1] == 0x00
            && edid[offset + 2] == 0x00;
        BYTE tag = edid[offset + 3];
        if (!descriptor || (tag != 0xFC && tag != 0xFE)) {
            continue;
        }

        std::wstring name;
        for (size_t i = offset + 5; i < offset + 18; ++i) {
            BYTE ch = edid[i];
            if (ch == 0x00 || ch == 0x0A || ch == 0x0D) {
                break;
            }
            if (ch >= 0x20) {
                name.push_back((wchar_t) ch);
            }
        }

        name = TrimDisplayString(name);
        if (!name.empty()) {
            return name;
        }
    }

    return L"";
}

static std::wstring RegistryMonitorName(std::wstring deviceId) {
    std::wstring instance = MonitorRegistryInstance(deviceId);
    if (instance.empty()) {
        return L"";
    }

    std::wstring keyPath = L"SYSTEM\\CurrentControlSet\\Enum\\DISPLAY\\"
        + instance + L"\\Device Parameters";
    HKEY key = NULL;
    LONG result = RegOpenKeyEx(
        HKEY_LOCAL_MACHINE, keyPath.c_str(), 0, KEY_READ, &key);
    if (result != ERROR_SUCCESS) {
        return L"";
    }

    DWORD type = REG_BINARY;
    DWORD size = 0;
    result = RegQueryValueEx(
        key, L"EDID", NULL, &type, NULL, &size);
    if (result != ERROR_SUCCESS || type != REG_BINARY || size == 0) {
        RegCloseKey(key);
        return L"";
    }

    std::vector<BYTE> edid(size);
    result = RegQueryValueEx(
        key, L"EDID", NULL, &type, edid.data(), &size);
    RegCloseKey(key);
    if (result != ERROR_SUCCESS || type != REG_BINARY) {
        return L"";
    }

    return EdidMonitorName(edid);
}

std::unordered_map<std::wstring, Monitor> &DisplayManager::MonitorMap() {
    return monitorMap;
}

void DisplayManager::UpdateMonitorMap() {
    monitorMap.clear();
    EnumDisplayMonitors(NULL, NULL, &MonitorEnumProc, NULL);
}

Monitor DisplayManager::Primary() {
    /* The Primary or 'Main' monitor is at (0, 0). */
    const POINT p = { 0, 0 };
    HMONITOR monitor = MonitorFromPoint(p, MONITOR_DEFAULTTOPRIMARY);
    MONITORINFO mInfo = Info(monitor);
    return Monitor(monitor, L"Primary", mInfo.rcMonitor);
}

Monitor DisplayManager::MonitorAtPoint(POINT &pt, bool workingArea) {
    Monitor m;
    HMONITOR monitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONULL);
    if (monitor != NULL) {
        MONITORINFO mInfo = Info(monitor);
        if (workingArea) {
            return Monitor(monitor, L"Monitor@Point", mInfo.rcWork);
        } else {
            return Monitor(monitor, L"Monitor@Point", mInfo.rcMonitor);
        }
    }

    return m;
}

Monitor DisplayManager::MonitorAtWindow(HWND hWnd, bool workingArea) {
    Monitor m;
    HMONITOR monitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONULL);
    if (monitor != NULL) {
        MONITORINFO mInfo = Info(monitor);
        if (workingArea) {
            return Monitor(monitor, L"Monitor@Window", mInfo.rcWork);
        } else {
            return Monitor(monitor, L"Monitor@Window", mInfo.rcMonitor);
        }
    }

    return m;
}

const int DisplayManager::Width(HMONITOR monitor) {
    MONITORINFO mInfo = Info(monitor);
    RECT mDims = mInfo.rcMonitor;
    return mDims.right - mDims.left;
}

const int DisplayManager::Height(HMONITOR monitor) {
    MONITORINFO mInfo = Info(monitor);
    RECT mDims = mInfo.rcMonitor;
    return mDims.bottom - mDims.top;
}

RECT DisplayManager::Rect(HMONITOR monitor) {
    return Info(monitor).rcMonitor;
}

bool DisplayManager::IsFullscreen(HWND hWnd) {
    HWND fg = GetForegroundWindow();
    if (hWnd == NULL || fg == NULL) {
        return false;
    }

    HWND shell = GetShellWindow();
    if (fg == shell) {
        return false;
    }

    HWND dt = GetDesktopWindow();
    if (fg == dt) {
        return false;
    }

    RECT wndRect = { 0 };
    GetWindowRect(fg, &wndRect);
    Monitor wm = MonitorAtWindow(hWnd);
    if ((wndRect.bottom - wndRect.top) == wm.Height() &&
            (wndRect.right - wndRect.left) == wm.Width()) {
        return true;
    }
    return false;
}

bool DisplayManager::Direct3DOccluded(HWND hWnd) {
    IDirect3D9Ex *pDirect3DEx;
    LPDIRECT3DDEVICE9EX pDeviceEx;
    DWORD behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    D3DPRESENT_PARAMETERS d3dpp = { 0 };
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

    Direct3DCreate9Ex(D3D_SDK_VERSION, &pDirect3DEx);
    HRESULT hr = pDirect3DEx->CreateDeviceEx(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        behaviorFlags,
        &d3dpp,
        NULL,
        &pDeviceEx);

    hr = pDeviceEx->CheckDeviceState(NULL);
    bool occluded = (hr == S_PRESENT_OCCLUDED);

    pDeviceEx->Release();
    pDirect3DEx->Release();

    return occluded;
}

MONITORINFO DisplayManager::Info(HMONITOR monitor) {
    MONITORINFO mInfo = {};
    mInfo.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(monitor, &mInfo);
    return mInfo;
}

std::list<DISPLAY_DEVICE> DisplayManager::ListAllDevices() {
    std::list<DISPLAY_DEVICE> devs;
    DISPLAY_DEVICE dev = {};
    dev.cb = sizeof(DISPLAY_DEVICE);
    for (int i = 0; EnumDisplayDevices(NULL, i, &dev, NULL) != 0; ++i) {
        if (dev.StateFlags & DISPLAY_DEVICE_ACTIVE) {
            devs.push_back(dev);
        }
    }
    return devs;
}

BOOL CALLBACK DisplayManager::MonitorEnumProc(
    HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {

    MONITORINFOEX mInfo = {};
    mInfo.cbSize = sizeof(MONITORINFOEX);
    GetMonitorInfo(hMonitor, &mInfo);

    std::wstring deviceName = std::wstring(mInfo.szDevice);
    std::wstring monitorId = deviceName;
    std::wstring displayName = deviceName;

    DISPLAY_DEVICE monitorDevice = {};
    monitorDevice.cb = sizeof(DISPLAY_DEVICE);
    if (EnumDisplayDevices(
            mInfo.szDevice, 0, &monitorDevice, EDD_GET_DEVICE_INTERFACE_NAME)) {
        std::wstring deviceId = BoundedDisplayString(
            monitorDevice.DeviceID, ARRAYSIZE(monitorDevice.DeviceID));
        if (deviceId.length() > 0) {
            monitorId = deviceId;
        }

        std::wstring deviceString = BoundedDisplayString(
            monitorDevice.DeviceString, ARRAYSIZE(monitorDevice.DeviceString));
        if (deviceString.length() > 0) {
            displayName = deviceString;
        }

        std::wstring registryName = RegistryMonitorName(deviceId);
        if (registryName.length() > 0) {
            displayName = registryName;
        }
    }

    Monitor mon(hMonitor, monitorId, displayName, deviceName, mInfo.rcMonitor);
    monitorMap[monitorId] = mon;

    return TRUE;
}
