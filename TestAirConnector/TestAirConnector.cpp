// TestAirConnector.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "PlaneEvent.h"
#include "TestMariaDB.h"

void testCallback(AirPlane* airPlane, DWORD ObjectID)
{
    printf("\nObjectID=%d  Title=\"%s\"\nLat=%f  Lon=%f  Alt=%f  Kohlsman=%.2f bank=%.2f pitch=%.2f heading=%.2f headindMagnetic=%.2f IsOnGround=%s", ObjectID,
        airPlane->title, airPlane->latitude, airPlane->longitude,
        airPlane->altitude, airPlane->kohlsmann, airPlane->bank,
        airPlane->pitch, airPlane->heading, airPlane->headingMagnetic,
        airPlane->isOnGround ? "true" : "false");
}

int __cdecl _tmain(int argc, _TCHAR* argv[])
{
    testConn();
    //SubscribePlaneEvent(testCallback);
    
	return 0;
}
