#pragma once

#ifndef PLANEEVENT_H
#define PLANEEVENT_H

#include "framework.h"
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>

struct AirPlane
{
    char    title[256];
    double  kohlsmann;
    double  altitude;
    double  latitude;
    double  longitude;
    double  pitch;
    double  bank;
    double  heading;
    double  headingMagnetic;
    unsigned long long isOnGround;
};

typedef void (*pPlaneEventCallback)(AirPlane* airPlane, DWORD ObjectID);

enum DATA_DEFINE_ID {
    DEFINITION_1,
};

enum DATA_REQUEST_ID {
    REQUEST_1,
};

void DLLTEMPLATE_API SubscribePlaneEvent(pPlaneEventCallback callback, DWORD period = 5);


#endif // PLANEEVENT_H
