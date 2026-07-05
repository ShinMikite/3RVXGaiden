// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <Windows.h>
#include <list>
#include <string>
#include <unordered_map>

class Monitor {
public:
    Monitor() {

    }

    Monitor(HMONITOR handle, std::wstring name,
        int x, int y, int width, int height) :
    Monitor(handle, name, name, name, x, y, width, height) {

    }

    Monitor(HMONITOR handle, std::wstring name, std::wstring displayName,
        std::wstring deviceName, int x, int y, int width, int height) :
    _handle(handle),
    _name(name),
    _displayName(displayName),
    _deviceName(deviceName),
    _x(x),
    _y(y),
    _width(width),
    _height(height) {

    }

    Monitor(HMONITOR handle, std::wstring name, RECT rect) :
    Monitor(handle, name, name, name, rect) {

    }

    Monitor(HMONITOR handle, std::wstring name, std::wstring displayName,
        std::wstring deviceName, RECT rect) :
    _handle(handle),
    _name(name),
    _displayName(displayName),
    _deviceName(deviceName),
    _x(rect.left),
    _y(rect.top),
    _width(rect.right - rect.left),
    _height(rect.bottom - rect.top) {

    }

    int X() {
        return _x;
    }

    int Y() {
        return _y;
    }

    int Width() {
        return _width;
    }

    int Height() {
        return _height;
    }

    HMONITOR Handle() {
        return _handle;
    }

    std::wstring Name() {
        return _name;
    }

    std::wstring DisplayName() {
        return _displayName;
    }

    std::wstring DeviceName() {
        return _deviceName;
    }

private:
    HMONITOR _handle;
    std::wstring _name;
    std::wstring _displayName;
    std::wstring _deviceName;
    int _x;
    int _y;
    int _width;
    int _height;
};
