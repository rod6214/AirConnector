#include "framework.h"
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>

int     quit = 0;
HANDLE  hSimConnect = NULL;

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

void DLLTEMPLATE_API SubscribePlaneEvent(pPlaneEventCallback callback);