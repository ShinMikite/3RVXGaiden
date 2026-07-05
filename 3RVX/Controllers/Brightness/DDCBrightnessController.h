// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#pragma comment(lib, "Dxva2.lib") 

#include <Windows.h>
#include <HighLevelMonitorConfigurationAPI.h>
#include "BrightnessController.h"

class Monitor;

class DDCBrightnessController : public BrightnessController{
public:
    DDCBrightnessController(HMONITOR monitor);
    DDCBrightnessController(Monitor &monitor);
    ~DDCBrightnessController();

    float Brightness() override;
    void Brightness(float level) override;

private:
    HANDLE _monitorHandle = NULL;
    DWORD _minBrightness = 0;
    DWORD _maxBrightness = 100;
    bool _useBrightnessAPI = false;

    bool SupportsBrightnessAPI(PHYSICAL_MONITOR &pm);
};
