// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <vector>
#include <Windows.h>

#include "../Animation.h"

class SlideOut : public Animation {
public:
    SlideOut(int speed);

    virtual bool Animate(MeterWnd *meterWnd);
    virtual bool AnimateIn(MeterWnd *meterWnd);
    virtual void Reset(MeterWnd *meterWnd);
    virtual void PrepareShow(MeterWnd *meterWnd);
    virtual bool HasAnimateIn();

    virtual int UpdateInterval();

private:
    static const int SLIDE_MARGIN = 12;

    int _interval;
    int _elapsed;
    bool _animatingIn;
    bool _animatingOut;
    std::vector<POINT> _origin;

    int SlideDistance(MeterWnd *meterWnd);
    void CaptureOrigin(MeterWnd *meterWnd);
    void RestoreOrigin(MeterWnd *meterWnd);
    void MoveWindows(MeterWnd *meterWnd, int yOffset);
};
