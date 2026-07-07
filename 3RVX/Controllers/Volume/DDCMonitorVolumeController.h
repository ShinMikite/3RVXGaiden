// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#pragma comment(lib, "Dxva2.lib")

#include <Windows.h>
#include <LowLevelMonitorConfigurationAPI.h>
#include <PhysicalMonitorEnumerationAPI.h>
#include <string>

#include "VolumeController.h"

class Monitor;
class VolumeTransformation;

class DDCMonitorVolumeController : public VolumeController {
public:
    DDCMonitorVolumeController(HMONITOR monitor);
    DDCMonitorVolumeController(Monitor &monitor);
    ~DDCMonitorVolumeController();

    float Volume() override;
    void Volume(float vol) override;
    DWORD VolumeValue();
    void VolumeValue(int value);
    DWORD MaxVolume();

    bool Muted() override;
    void Muted(bool mute) override;
    bool DeviceEnabled() override;

    void AddTransformation(VolumeTransformation *transform) override;
    void RemoveTransformation(VolumeTransformation *transform) override;

    std::wstring DeviceName();

private:
    static const BYTE VCP_AUDIO_VOLUME = 0x62;

    HANDLE _monitorHandle = NULL;
    DWORD _maxVolume = 100;
    bool _deviceEnabled = false;
    std::wstring _deviceName;

    bool TryReadVolume(HANDLE monitorHandle, DWORD *current, DWORD *maximum);
    bool SupportsVolumeVCP(PHYSICAL_MONITOR &pm, DWORD *current, DWORD *maximum);
};
