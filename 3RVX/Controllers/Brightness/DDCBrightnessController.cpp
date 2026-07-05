// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "DDCBrightnessController.h"

#include "../../Monitor.h"
#include "../../Logger.h"

DDCBrightnessController::DDCBrightnessController(HMONITOR monitor) {
    BOOL result;
    DWORD numPhysicalMonitors = 0;
    result = GetNumberOfPhysicalMonitorsFromHMONITOR(
        monitor, &numPhysicalMonitors);

    if (result == FALSE || numPhysicalMonitors <= 0) {
        CLOG(L"Could not get physical monitors");
        return;
    }

    CLOG(L"Number of physical monitors detected: %d", numPhysicalMonitors);
    PHYSICAL_MONITOR *monitors = new PHYSICAL_MONITOR[numPhysicalMonitors];
    result = GetPhysicalMonitorsFromHMONITOR(
        monitor, numPhysicalMonitors, monitors);
    for (unsigned int i = 0; i < numPhysicalMonitors; ++i) {
        CLOG(L"Monitor: %s", monitors[i].szPhysicalMonitorDescription);
        bool supportsAPI = SupportsBrightnessAPI(monitors[i]);
        QCLOG(L"Supports *MonitorBrightness APIs: %s",
            supportsAPI ? L"YES" : L"NO");
        if (supportsAPI) {
            /* For now, we use the first compatible monitor found. */
            _monitorHandle = monitors[i].hPhysicalMonitor;
            _useBrightnessAPI = true;
            break;
        }
    }
    delete[] monitors;

    if (_monitorHandle == NULL) {
        CLOG(L"No compatible monitor brightness API found");
        return;
    }

    DWORD min, cur, max;
    result = GetMonitorBrightness(_monitorHandle, &min, &cur, &max);
    if (result == 0) {
        Logger::LogLastError();
        _useBrightnessAPI = false;
        return;
    }
    _minBrightness = min;
    _maxBrightness = max;
    CLOG(L"Got brightness: [%d, %d] %f", min, max, Brightness());
}

DDCBrightnessController::DDCBrightnessController(Monitor &monitor) :
DDCBrightnessController(monitor.Handle()) {

}

DDCBrightnessController::~DDCBrightnessController() {
    if (_monitorHandle != NULL) {
        DestroyPhysicalMonitor(_monitorHandle);
    }
}

float DDCBrightnessController::Brightness() {
    if (!_useBrightnessAPI || _monitorHandle == NULL) {
        return 0.0f;
    }

    DWORD min, cur, max;
    if (GetMonitorBrightness(_monitorHandle, &min, &cur, &max) == FALSE) {
        Logger::LogLastError();
        return 0.0f;
    }

    return (float) (cur - _minBrightness) / (_maxBrightness - _minBrightness);
}

void DDCBrightnessController::Brightness(float level) {
    if (!_useBrightnessAPI || _monitorHandle == NULL) {
        return;
    }

    DWORD setLevel = (DWORD) ((_maxBrightness - _minBrightness) * level);
    if (SetMonitorBrightness(_monitorHandle, setLevel) == FALSE) {
        Logger::LogLastError();
    }
}

bool DDCBrightnessController::SupportsBrightnessAPI(PHYSICAL_MONITOR &pm) {
    DWORD caps, color;
    BOOL result = GetMonitorCapabilities(pm.hPhysicalMonitor, &caps, &color);
    if (result == FALSE) {
        QCLOG(L"Monitor does not support DDC/CI");
        return false;
    }
    return ((caps & MC_CAPS_BRIGHTNESS) == MC_CAPS_BRIGHTNESS);
}
