// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "OSD.h"

#include "../../3RVX/Controllers/Volume/CoreAudio.h"
#include "../../3RVX/Controllers/Volume/VolumeController.h"
#include "../../3RVX/LanguageTranslator.h"
#include "../../3RVX/Logger.h"
#include "../../3RVX/Settings.h"
#include "../resource.h"

void OSD::Initialize() {
    _volumeGroup = new GroupBox(GRP_VOLUME, *this);
    _volumeEnabled = new Checkbox(CHK_ENABLEVOSD, *this);
    _volumeIcon = new Checkbox(CHK_VOLICON, *this);
    _subscribeVolEvents = new Checkbox(CHK_MONITORVOL, *this);
    _audioDeviceLabel = new Label(LBL_AUDIODEV, *this);
    _audioDevice = new ComboBox(CMB_AUDIODEV, *this);
    _audioTaperLabel = new Label(LBL_AUDIOTAPER, *this);
    _audioTaper = new ComboBox(CMB_AUDIOTAPER, *this);
    _audioTaperEdit = new EditBox(ED_AUDIOTAPER, *this);
    _audioTaper->OnSelectionChange = [this]() {
        _audioTaperEdit->Enabled(_volumeGroup->Enabled()
            && _audioTaper->SelectionIndex() == _audioTaper->Count() - 1);
        return true;
    };

    _limitLabel = new Label(LBL_LIMITER, *this);
    _limitSlider = new Slider(SLD_LIMITER, *this);
    _limitValue = new Label(LBL_LIMITVAL, *this);
    _limitSlider->Buddy(_limitValue);
    _limitSlider->OnSlide = [this]() {
        std::wstring value = std::to_wstring(_limitSlider->Position()) + L"%";
        _limitValue->Text(value);
        return true;
    };
    _muteLock = new Checkbox(CHK_MUTELOCK, *this);
    _volumeGroup->AddChildren({
        _volumeEnabled,
        _volumeIcon,
        _subscribeVolEvents,
        _audioDeviceLabel, _audioDevice,
        _audioTaperLabel, _audioTaper, _audioTaperEdit,
        _limitLabel, _limitSlider, _limitValue,
        _muteLock,
    });
}

void OSD::LoadSettings() {
    Settings *settings = Settings::Instance();
    LanguageTranslator *translator = settings->Translator();

    _audioDevice->AddItem(translator->Translate(L"Default"));
    _audioDevice->Select(0);
    std::wstring selectedId = settings->AudioDeviceID();
    CoreAudio *volumeCtrl = new CoreAudio(NULL);
    volumeCtrl->Init();
    _audioDevices = volumeCtrl->ListDevices();
    for (VolumeController::DeviceInfo dev : _audioDevices) {
        _audioDevice->AddItem(dev.name);
        if (dev.id == selectedId) {
            _audioDevice->Select(dev.name);
        }
    }
    volumeCtrl->Dispose();

    _taperLevels[0] = L"Disabled";
    _taperLevels[2] = L"Low";
    _taperLevels[4] = L"Medium";
    _taperLevels[6] = L"High";
    for (auto it = _taperLevels.begin(); it != _taperLevels.end(); ++it) {
        _audioTaper->AddItem(translator->Translate(it->second));
    }
    _audioTaper->AddItem(translator->Translate(L"Custom"));

    int level = settings->VolumeCurveAdjustment();
    if (_taperLevels.find(level) == _taperLevels.end()) {
        /* Select the last item (custom) */
        _audioTaper->Select((int) _taperLevels.size());
        _audioTaperEdit->Text(settings->VolumeCurveAdjustment());
    } else {
        _audioTaper->Select(translator->Translate(_taperLevels[level]));
    }
    _audioTaper->OnSelectionChange();

    _volumeEnabled->Checked(settings->VolumeOSDEnabled());
    _volumeIcon->Checked(settings->VolumeIconEnabled());
    _subscribeVolEvents->Checked(settings->SubscribeVolumeEvents());
    _limitSlider->Position((int) (settings->VolumeLimiter() * 100.0f));
    _muteLock->Checked(settings->MuteOnLock());
}

void OSD::SaveSettings() {
    CLOG(L"Saving: Volume");
    Settings *settings = Settings::Instance();
    
    settings->VolumeOSDEnabled(_volumeEnabled->Checked());
    settings->BrightnessOSDEnabled(false);
    settings->EjectOSDEnabled(false);
    settings->KeyboardOSDEnabled(false);

    settings->VolumeIconEnabled(_volumeIcon->Checked());
    settings->SubscribeVolumeEvents(_subscribeVolEvents->Checked());
    int selectedDevice = _audioDevice->SelectionIndex();
    if (selectedDevice == 0) {
        settings->AudioDeviceID(L"");
    } else {
        settings->AudioDeviceID(_audioDevices[selectedDevice - 1].id);
    }
    if (_audioTaper->SelectionIndex() == _audioTaper->Count() - 1) {
        /* Custom taper */
        settings->VolumeCurveAdjustment(_audioTaperEdit->TextAsInt());
    } else {
        /* NOTE: We multiply the taper index by two to get the actual taper
         * value. For instance, low = 2, medium = 4, etc. */
        settings->VolumeCurveAdjustment(_audioTaper->SelectionIndex() * 2);
    }
    settings->VolumeLimiter(((float) _limitSlider->Position()) / 100.0f);
    settings->MuteOnLock(_muteLock->Checked());
    settings->EjectIconEnabled(false);
    settings->SubscribeEjectEvents(false);
}
