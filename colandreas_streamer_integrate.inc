#if defined _colandreas_auto_streamer_inc
	#endinput
#endif
#define _colandreas_auto_streamer_inc

#if !defined _streamer_included
    #endinput
#endif

#if !defined _colandreas_included
    #endinput
#endif


#include <YSI_Coding\y_hooks>

#define MAX_DYNAMIC_OBJECTS 1000000

static stock DynamicObjectIDToColID[MAX_DYNAMIC_OBJECTS] = {-1, ...};


stock CA_auto_CreateDynamicObject(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, worldid = -1, interiorid = -1, playerid = -1, Float:streamdistance = STREAMER_OBJECT_SD, Float:drawdistance = STREAMER_OBJECT_DD, STREAMER_TAG_AREA:areaid = STREAMER_TAG_AREA:-1, priority = 0)
{
    new dynamic_object_id = CreateDynamicObject(modelid, x, y, z, rx, ry, rz, worldid, interiorid, playerid, streamdistance, drawdistance, areaid, priority);
    if(dynamic_object_id < MAX_DYNAMIC_OBJECTS)
    {
        DynamicObjectIDToColID[dynamic_object_id] = CA_CreateObject(modelid, x, y, z, rx, ry, rz, true);
        if(DynamicObjectIDToColID[dynamic_object_id] == 65535)
        {
            DynamicObjectIDToColID[dynamic_object_id] = -1;
        }
    }
    else
    {
        printf("colandreas-auto-streamer.inc: WARNING - dynamic_object_id >= MAX_DYNAMIC_OBJECTS!");
    }
    
    return dynamic_object_id;
}
#if defined _ALS_CreateDynamicObject
    #undef CreateDynamicObject
#else
    #define _ALS_CreateDynamicObject
#endif
#define CreateDynamicObject CA_auto_CreateDynamicObject


stock CA_auto_DestroyDynamicObject(STREAMER_TAG_OBJECT:objectid)
{
    DestroyDynamicObject(objectid);
    if(DynamicObjectIDToColID[objectid] != -1)
    {
        CA_DestroyObject(DynamicObjectIDToColID[objectid]);
        DynamicObjectIDToColID[objectid] = -1;
    }
}

#if defined _ALS_DestroyDynamicObject
    #undef DestroyDynamicObject
#else
    #define _ALS_DestroyDynamicObject
#endif
#define DestroyDynamicObject CA_auto_DestroyDynamicObject


stock CA_auto_SetDynamicObjectPos(STREAMER_TAG_OBJECT:objectid, Float:x, Float:y, Float:z)
{
    SetDynamicObjectPos(objectid, x, y, z);
    if(DynamicObjectIDToColID[objectid] != -1)
    {
        CA_SetObjectPos(DynamicObjectIDToColID[objectid], x, y, z);
    }
}

#if defined _ALS_SetDynamicObjectPos
    #undef SetDynamicObjectPos
#else
    #define _ALS_SetDynamicObjectPos
#endif
#define SetDynamicObjectPos CA_auto_SetDynamicObjectPos


stock CA_auto_SetDynamicObjectRot(STREAMER_TAG_OBJECT:objectid, Float:rx, Float:ry, Float:rz)
{
    SetDynamicObjectRot(objectid, rx, ry, rz);
    if(DynamicObjectIDToColID[objectid] != -1)
    {
        CA_SetObjectRot(DynamicObjectIDToColID[objectid], rx, ry, rz);
    }
}

#if defined _ALS_SetDynamicObjectRot
    #undef SetDynamicObjectRot
#else
    #define _ALS_SetDynamicObjectRot
#endif
#define SetDynamicObjectRot CA_auto_SetDynamicObjectRot


hook OnDynamicObjectMoved(objectid)
{
    if(DynamicObjectIDToColID[objectid] != -1)
    {
        new Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz;
        GetDynamicObjectPos(objectid, x, y, z);
        GetDynamicObjectRot(objectid, rx, ry, rz);
        CA_SetObjectPos(DynamicObjectIDToColID[objectid], x, y, z);
        CA_SetObjectRot(DynamicObjectIDToColID[objectid], rx, ry, rz);
    }

    return 1;
}


#undef MAX_DYNAMIC_OBJECTS