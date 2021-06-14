// TestAirConnector.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <MariaDB.h>
//#include "TestMariaDB.h"

void testCallback(std::shared_ptr<PlaneData> ptr_planedata)
{
    auto planedata = ptr_planedata.get();
    auto airPlane = planedata->airPlane;

    printf("\nObjectID=%d  Title=\"%s\"\nLat=%f  Lon=%f  Alt=%f  Kohlsman=%.2f bank=%.2f pitch=%.2f heading=%.2f headindMagnetic=%.2f IsOnGround=%s Datetime=%s", 
        planedata->ObjectID,
        airPlane->title, airPlane->latitude, airPlane->longitude,
        airPlane->altitude, airPlane->kohlsmann, airPlane->bank,
        airPlane->pitch, airPlane->heading, airPlane->headingMagnetic,
        airPlane->isOnGround ? "true" : "false",
        planedata->ptr_datetime.get()->c_str());

    if (positionHasChanged(ptr_planedata))
    {
        insertDataPlane(ptr_planedata);
    }
}

int __cdecl _tmain(int argc, _TCHAR* argv[])
{
    SubscribePlaneEvent(testCallback, 10);
    
	return 0;
}
