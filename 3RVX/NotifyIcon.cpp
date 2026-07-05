// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "NotifyIcon.h"

#include "Logger.h"

int NotifyIcon::ids = 0;

template <size_t N>
static void CopyShellString(wchar_t (&dest)[N], const std::wstring &source) {
    wcsncpy_s(dest, source.c_str(), _TRUNCATE);
}

NotifyIcon::NotifyIcon(HWND hWnd, std::wstring tip, HICON icon) :
_icon(icon),
_tip(tip) {

    _id = NotifyIcon::ids++;

    _nid = { 0 };
    _nid.cbSize = { sizeof(_nid) };
    _nid.hWnd = hWnd;
    _nid.uID = _id;
    _nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    _nid.uCallbackMessage = MSG_NOTIFYICON;
    _nid.hIcon = _icon;

    CopyShellString(_nid.szTip, tip);

    Shell_NotifyIcon(NIM_ADD, &_nid);

    _nii = { 0 };
    _nii.cbSize = { sizeof(_nii) };
    _nii.hWnd = hWnd;
    _nii.uID = _id;
}

void NotifyIcon::Balloon(std::wstring title, std::wstring text, HICON icon) {
    _nid.uFlags = _nid.uFlags | NIF_INFO;
    _nid.dwInfoFlags = NIIF_RESPECT_QUIET_TIME;
    if (icon != nullptr) {
        _nid.dwInfoFlags = _nid.dwInfoFlags | NIIF_USER | NIIF_LARGE_ICON;
        _nid.hBalloonIcon = icon;
    }

    CopyShellString(_nid.szInfoTitle, title);
    CopyShellString(_nid.szInfo, text);

    Shell_NotifyIcon(NIM_MODIFY, &_nid);
}

void NotifyIcon::UpdateIcon(HICON icon) {
    _nid.hIcon = icon;
    Shell_NotifyIcon(NIM_MODIFY, &_nid);
}

void NotifyIcon::UpdateToolTip(std::wstring newTip) {
    CopyShellString(_nid.szTip, newTip);
    Shell_NotifyIcon(NIM_MODIFY, &_nid);
}

NotifyIcon::~NotifyIcon() {
    Shell_NotifyIcon(NIM_DELETE, &_nid);
}

NOTIFYICONDATA NotifyIcon::IconData() {
    return _nid;
}

NOTIFYICONIDENTIFIER NotifyIcon::IconID() {
    return _nii;
}
