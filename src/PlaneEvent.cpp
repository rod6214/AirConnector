//------------------------------------------------------------------------------
//
//  Author: Nelson Amador
//  
//	Description:
//------------------------------------------------------------------------------
#include <time.h>
#include <string>
#include <memory>
#include "PlaneEvent.h"
#include "SimConnect.h"

#include <iostream>

static int     quit = 0;
static HANDLE  hSimConnect = NULL;

static std::unique_ptr<std::string> getTime();

void CALLBACK MyDispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext)
{   
    switch(pData->dwID)
    {
        case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
        {
            SIMCONNECT_RECV_SIMOBJECT_DATA* pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;

            switch (pObjData->dwRequestID)
            {
            case REQUEST_1:
            {
                DWORD ObjectID = pObjData->dwObjectID;
                AirPlane* pS = (AirPlane*)&pObjData->dwData;
                if (SUCCEEDED(StringCbLengthA(&pS->title[0], sizeof(pS->title), NULL))) // security check
                {
                    pPlaneEventCallback callback = (pPlaneEventCallback)pContext;

                    PlaneData planedata;
                    planedata.ptr_datetime = getTime();
                    planedata.airPlane = pS;
                    planedata.ObjectID = ObjectID;

                    std::shared_ptr<PlaneData> ptr_planedata = std::make_shared<PlaneData>(planedata);

                    callback(ptr_planedata);
                }
                break;
            }

            default:
                break;
            }
            break;
        }


        case SIMCONNECT_RECV_ID_QUIT:
        {
            quit = 1;
            break;
        }

        default:
            printf("\nReceived:%d",pData->dwID);
            break;
    }
}

static std::unique_ptr<std::string> getTime()
{
    static struct tm newtime;
    static __time32_t aclock;
    static char buffer[32];

    _time32(&aclock);   // Get time in seconds.
    _localtime32_s(&newtime, &aclock);   // Convert time to struct tm form.

    // Print local time as a string.
    std::string* time = new std::string();
    
    auto ptr_time = std::make_unique<std::string>();

    size_t total = strftime(buffer, 32, "%Y-%m-%d %H:%M:%S", &newtime);

    if (!total)
    {
        printf("Error...");
    }
    else
    {
        ptr_time->assign(buffer, 32);
    }
 
    return ptr_time;
}

void DLLTEMPLATE_API SubscribePlaneEvent(pPlaneEventCallback callback, DWORD period)
{
    HRESULT hr;

    if (SUCCEEDED(SimConnect_Open(&hSimConnect, "Request Data", NULL, 0, 0, 0)))
    {
        printf("\nConnected to Flight Simulator!");
        // Set up the data definition, but do not yet do anything with it
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Title", NULL, SIMCONNECT_DATATYPE_STRING256);
        if (hr != S_OK) throw "It couldn't create the definition Title";

        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Kohlsman setting hg", "inHg");
        if (hr != S_OK) throw "It couldn't create the definition Kohlsman setting hg";

        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Plane Altitude", "feet");
        if (hr != S_OK) throw "It couldn't create the definition Plane Altitude";

        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Plane Latitude", "degrees");
        if (hr != S_OK) throw "It couldn't create the definition Plane Latitude";

        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Plane Longitude", "degrees");
        if (hr != S_OK) throw "It couldn't create the definition Plane Longitude";

        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Plane Pitch Degrees", "radians");
        if (hr != S_OK) throw "It couldn't create the definition Plane Pitch Degrees";

        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Plane Bank Degrees", "radians");
        if (hr != S_OK) throw "It couldn't create the definition Plane Bank Degrees";

        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Plane Heading Degrees", "radians");
        if (hr != S_OK) throw "It couldn't create the definition Plane Heading Degrees";

        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Plane Heading Degrees Magnetic", "radians");
        if (hr != S_OK) throw "It couldn't create the definition Plane Heading Degrees Magnetic";

        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Ground Altitude", "meters");
        if (hr != S_OK) throw "It couldn't create the definition Ground Altitude";

        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Sim On Ground", "bool");
        if (hr != S_OK) throw "It couldn't create the definition Sim On Ground";

        hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQUEST_1, DEFINITION_1, SIMCONNECT_OBJECT_ID_USER, 
            SIMCONNECT_PERIOD_SECOND, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED, 0, period);
        if (hr != S_OK) throw "It can't init the request data";

        while( 0 == quit )
        {
            SimConnect_CallDispatch(hSimConnect, MyDispatchProcRD, callback);
            Sleep(1);
        } 

        hr = SimConnect_Close(hSimConnect);
        if (hr != S_OK) throw "There was an error when the application tried to close SimConnect";
    }

    throw "It couldn't open connection with SimConnect";
}
