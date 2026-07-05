// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include "../../3RVX/Controllers/Volume/VolumeController.h"
#include "SettingsTab.h"

class OSD : public SettingsTab {
public:
    OSD() :
    SettingsTab(NULL, MAKEINTRESOURCE(IDD_OSD), L"Volume") {

    }

    virtual void SaveSettings();

protected:
    virtual void Initialize();
    virtual void LoadSettings();

private:
    std::vector<VolumeController::DeviceInfo> _audioDevices;
    std::unordered_map<int, std::wstring> _taperLevels;

private:
    /* Controls: */
    GroupBox *_volumeGroup;
    Checkbox *_volumeEnabled;
    Checkbox *_volumeIcon;
    Checkbox *_subscribeVolEvents;
    Label *_audioDeviceLabel;
    ComboBox *_audioDevice;
    Label *_audioTaperLabel;
    EditBox *_audioTaperEdit;
    ComboBox *_audioTaper;
    Label *_limitLabel;
    Slider *_limitSlider;
    Label *_limitValue;
    Checkbox *_muteLock;
};
