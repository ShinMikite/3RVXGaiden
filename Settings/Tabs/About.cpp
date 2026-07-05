// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "About.h"

#include "../resource.h"

void About::Initialize() {
    _title = new Label(LBL_TITLE, *this, false);
    _title->Text(L"3RVXGaiden v0.3.0-preview");
}

void About::LoadSettings() {

}

void About::SaveSettings() {

}
