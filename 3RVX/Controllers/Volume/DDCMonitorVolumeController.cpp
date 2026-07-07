// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "DDCMonitorVolumeController.h"

#include <cmath>

#include "../../Logger.h"
#include "../../Monitor.h"
#include "VolumeTransformation.h"

DDCMonitorVolumeController::DDCMonitorVolumeController(HMONITOR monitor) {
    BOOL result;
    DWORD numPhysicalMonitors = 0;
    result = GetNumberOfPhysicalMonitorsFromHMONITOR(
        monitor, &numPhysicalMonitors);

    if (result == FALSE || numPhysicalMonitors <= 0) {
        CLOG(L"Could not get physical monitors for monitor volume");
        return;
    }

    CLOG(L"Physical monitors detected for volume: %d", numPhysicalMonitors);
    PHYSICAL_MONITOR *monitors = new PHYSICAL_MONITOR[numPhysicalMonitors];
    result = GetPhysicalMonitorsFromHMONITOR(
        monitor, numPhysicalMonitors, monitors);
    if (result == FALSE) {
        Logger::LogLastError();
        delete[] monitors;
        return;
    }

    int selectedMonitor = -1;
    for (unsigned int i = 0; i < numPhysicalMonitors; ++i) {
        DWORD current = 0;
        DWORD maximum = 0;
        CLOG(L"Monitor volume candidate: %s",
            monitors[i].szPhysicalMonitorDescription);
        if (SupportsVolumeVCP(monitors[i], &current, &maximum)) {
            selectedMonitor = (int) i;
            _monitorHandle = monitors[i].hPhysicalMonitor;
            _deviceName = monitors[i].szPhysicalMonitorDescription;
            _maxVolume = maximum > 0 ? maximum : 100;
            _deviceEnabled = true;
            CLOG(L"Using monitor volume: %s [%d/%d]",
                _deviceName.c_str(), current, _maxVolume);
            break;
        }
    }

    for (unsigned int i = 0; i < numPhysicalMonitors; ++i) {
        if ((int) i != selectedMonitor) {
            DestroyPhysicalMonitor(monitors[i].hPhysicalMonitor);
        }
    }
    delete[] monitors;

    if (_monitorHandle == NULL) {
        CLOG(L"No compatible monitor volume VCP control found");
    }
}

DDCMonitorVolumeController::DDCMonitorVolumeController(Monitor &monitor) :
DDCMonitorVolumeController(monitor.Handle()) {

}

DDCMonitorVolumeController::~DDCMonitorVolumeController() {
    if (_monitorHandle != NULL) {
        DestroyPhysicalMonitor(_monitorHandle);
    }
}

float DDCMonitorVolumeController::Volume() {
    if (!_deviceEnabled || _monitorHandle == NULL) {
        return 0.0f;
    }

    DWORD current = 0;
    DWORD maximum = 0;
    if (!TryReadVolume(_monitorHandle, &current, &maximum)) {
        return 0.0f;
    }

    _maxVolume = maximum > 0 ? maximum : _maxVolume;
    if (_maxVolume == 0) {
        return 0.0f;
    }

    return (float) current / (float) _maxVolume;
}

void DDCMonitorVolumeController::Volume(float vol) {
    if (!_deviceEnabled || _monitorHandle == NULL) {
        return;
    }

    if (vol < 0.0f) {
        vol = 0.0f;
    } else if (vol > 1.0f) {
        vol = 1.0f;
    }
    DWORD newValue = (DWORD) std::round(vol * _maxVolume);
    if (SetVCPFeature(_monitorHandle, VCP_AUDIO_VOLUME, newValue) == FALSE) {
        Logger::LogLastError();
    }
}

DWORD DDCMonitorVolumeController::VolumeValue() {
    if (!_deviceEnabled || _monitorHandle == NULL) {
        return 0;
    }

    DWORD current = 0;
    DWORD maximum = 0;
    if (!TryReadVolume(_monitorHandle, &current, &maximum)) {
        return 0;
    }

    _maxVolume = maximum > 0 ? maximum : _maxVolume;
    return current;
}

void DDCMonitorVolumeController::VolumeValue(int value) {
    if (!_deviceEnabled || _monitorHandle == NULL) {
        return;
    }

    if (value < 0) {
        value = 0;
    } else if ((DWORD) value > _maxVolume) {
        value = (int) _maxVolume;
    }

    if (SetVCPFeature(_monitorHandle, VCP_AUDIO_VOLUME, (DWORD) value) == FALSE) {
        Logger::LogLastError();
    }
}

DWORD DDCMonitorVolumeController::MaxVolume() {
    return _maxVolume;
}

bool DDCMonitorVolumeController::Muted() {
    return false;
}

void DDCMonitorVolumeController::Muted(bool mute) {

}

bool DDCMonitorVolumeController::DeviceEnabled() {
    return _deviceEnabled;
}

void DDCMonitorVolumeController::AddTransformation(
        VolumeTransformation *transform) {

}

void DDCMonitorVolumeController::RemoveTransformation(
        VolumeTransformation *transform) {

}

std::wstring DDCMonitorVolumeController::DeviceName() {
    return _deviceName;
}

bool DDCMonitorVolumeController::TryReadVolume(
        HANDLE monitorHandle, DWORD *current, DWORD *maximum) {
    MC_VCP_CODE_TYPE codeType;
    BOOL result = GetVCPFeatureAndVCPFeatureReply(
        monitorHandle,
        VCP_AUDIO_VOLUME,
        &codeType,
        current,
        maximum);

    if (result == FALSE) {
        Logger::LogLastError();
        return false;
    }

    return true;
}

bool DDCMonitorVolumeController::SupportsVolumeVCP(
        PHYSICAL_MONITOR &pm, DWORD *current, DWORD *maximum) {
    if (!TryReadVolume(pm.hPhysicalMonitor, current, maximum)) {
        QCLOG(L"Monitor does not expose audio volume VCP 0x62");
        return false;
    }

    if (*maximum == 0) {
        QCLOG(L"Monitor audio volume VCP 0x62 returned no maximum value");
        return false;
    }

    return true;
}
