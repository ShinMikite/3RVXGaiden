// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include "../../3RVX/HotkeyInfo.h"
#include "../../3RVX/LanguageTranslator.h"
#include "SettingsTab.h"

class Hotkeys : public SettingsTab {
public:
    Hotkeys() :
    SettingsTab(NULL, MAKEINTRESOURCE(IDD_HOTKEYS), L"Hotkeys") {

    }

    virtual void SaveSettings();

protected:
    virtual void Initialize();
    virtual void LoadSettings();

protected:
    std::vector<HotkeyInfo> _keyInfo;
    std::vector<int> _actionIds;
    LanguageTranslator *_translator;

    HotkeyInfo *CurrentHotkeyInfo();

    void LoadSelection();
    void LoadSelection(int index);
    void LoadAction(int index, HotkeyInfo &selection);
    std::wstring ActionString(HotkeyInfo &selection);
    std::wstring VolumeActionString(HotkeyInfo &selection);
    int ActionComboIndex(int action);
    int SelectedAction();

    void DefaultArgControlStates();
    void VolumeArgControlStates(HotkeyInfo &selection);

private:
    /* Control Event Handlers */
    void OnKeyListItemChange(NMLISTVIEW *lv);

    bool OnAddButtonClick();
    bool OnRemoveButtonClick();

    bool OnKeysButtonClick();
    bool OnActionChange();

    bool OnArgButtonClick();
    bool OnArgComboChange();
    bool OnArgCheckChange();
    bool OnArgEditTextChange();

protected:
    /* Controls: */
    ListView *_keyList;
    Button *_add;
    Button *_remove;

    GroupBox *_editorGroup;
    Label *_keysLabel;
    Button *_keys;
    Label *_actionLabel;
    ComboBox *_action;

    Label *_argLabel;
    Checkbox *_argCheck;
    ComboBox *_argCombo;
    EditBox *_argEdit;
    Button *_argButton;

protected:
    /* Strings: */
    std::wstring _hotkeysColumnStr = L"Hotkeys";
    std::wstring _actionColumnStr = L"Action";
    std::wstring _amountVolArgStr = L"Amount:";
    std::wstring _unitsVolArgStr = L"Volume Units";
    std::wstring _percentVolArgStr = L"Percent";
};
