// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "SlideOut.h"

#include <cmath>

#include "..\LayeredWnd.h"
#include "..\MeterWnd.h"

SlideOut::SlideOut(int speed) :
Animation(speed > 0 ? speed : 1),
_interval(10),
_elapsed(0),
_animatingIn(false),
_animatingOut(false) {

}

bool SlideOut::Animate(MeterWnd *meterWnd) {
    _animatingOut = true;
    _elapsed += _interval;

    float progress = (float) _elapsed / (float) _speed;
    if (progress > 1.0f) {
        progress = 1.0f;
    }
    float eased = progress * progress;
    int offset = (int) std::round(SlideDistance(meterWnd) * eased);

    MoveWindows(meterWnd, offset);

    if (progress >= 1.0f) {
        RestoreOrigin(meterWnd);
        _animatingOut = false;
        return true;
    }

    return false;
}

bool SlideOut::AnimateIn(MeterWnd *meterWnd) {
    _animatingIn = true;
    _elapsed += _interval;

    float progress = (float) _elapsed / (float) _speed;
    if (progress > 1.0f) {
        progress = 1.0f;
    }

    float eased = 1.0f - ((1.0f - progress) * (1.0f - progress));
    int offset = (int) std::round(SlideDistance(meterWnd) * (1.0f - eased));

    MoveWindows(meterWnd, offset);

    if (progress >= 1.0f) {
        RestoreOrigin(meterWnd);
        _animatingIn = false;
        return true;
    }

    return false;
}

void SlideOut::Reset(MeterWnd *meterWnd) {
    if (_animatingIn || _animatingOut) {
        RestoreOrigin(meterWnd);
    }

    CaptureOrigin(meterWnd);
    _elapsed = 0;
    _animatingIn = false;
    _animatingOut = false;
    meterWnd->Transparency(255);
}

void SlideOut::PrepareShow(MeterWnd *meterWnd) {
    Reset(meterWnd);
    MoveWindows(meterWnd, SlideDistance(meterWnd));
}

bool SlideOut::HasAnimateIn() {
    return true;
}

int SlideOut::UpdateInterval() {
    return _interval;
}

int SlideOut::SlideDistance(MeterWnd *meterWnd) {
    return meterWnd->Height() + SLIDE_MARGIN;
}

void SlideOut::CaptureOrigin(MeterWnd *meterWnd) {
    _origin.clear();
    _origin.push_back(meterWnd->Position());

    for (LayeredWnd *clone : meterWnd->Clones()) {
        _origin.push_back(clone->Position());
    }
}

void SlideOut::RestoreOrigin(MeterWnd *meterWnd) {
    MoveWindows(meterWnd, 0);
}

void SlideOut::MoveWindows(MeterWnd *meterWnd, int yOffset) {
    if (_origin.size() == 0) {
        CaptureOrigin(meterWnd);
    }

    meterWnd->Position(_origin[0].x, _origin[0].y + yOffset);

    std::vector<LayeredWnd *> clones = meterWnd->Clones();
    for (size_t i = 0; i < clones.size(); ++i) {
        size_t originIdx = i + 1;
        if (originIdx >= _origin.size()) {
            break;
        }
        clones[i]->Position(_origin[originIdx].x, _origin[originIdx].y + yOffset);
    }
}
