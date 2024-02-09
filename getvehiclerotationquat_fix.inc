#if defined _getvehiclerotationquat_fix
	#endinput
#endif
#define _getvehiclerotationquat_fix

#tryinclude <amx_assembly\phys_memory>
#if !defined PHYS_MEMORY_INC
	#endinput
#endif

#tryinclude <amx_assembly\os>
#if !defined OS_INC
	#endinput
#endif


#define SAMP_037R3 1
#define SAMP_03DL 0
#define UNSUPPORTED_SAMP_VERSION -1
#define UNINITIALIZED_SAMP_VERSION -2
#define UNINITIALIZED_OS_VERSION OS:-2
#define UNINITIALIZED_VEHPOOL_ADDR -2


// Adres, pod którym przechowywany jest adres puli pojazdów
static const ADDR_SERVER_DATA_ADDR[2][OS] = {
    {0, 0x081D7940, 0x004FC438},
	{0, 0x081CB5BC, 0x004F5FB8}
};


static samp_version = UNINITIALIZED_SAMP_VERSION;
static OS:os_version = UNINITIALIZED_OS_VERSION;
static vehpool_addr = UNINITIALIZED_VEHPOOL_ADDR;


static stock GetSampVersion()
{
    new version_string[64], v;
    GetConsoleVarAsString("version", version_string, sizeof(version_string));
    if(strcmp(version_string, "0.3.DL-R1") == 0)
    {
        v = SAMP_03DL;
    }
    else if(strcmp(version_string, "0.3.7-R3") == 0)
    {
        v = SAMP_037R3;
    }
    else
    {
        v = UNSUPPORTED_SAMP_VERSION;
    }

    return v;
}


static stock VehicleMatricesFuncInit()
{
    if(samp_version == UNINITIALIZED_SAMP_VERSION)
    {
        samp_version = GetSampVersion();
    }

    if(os_version == UNINITIALIZED_OS_VERSION)
    {
        os_version = GetOS();
    }

    if(samp_version == UNSUPPORTED_SAMP_VERSION || os_version == OS_UNKNOWN)
    {
        return 0;
    }

    if(vehpool_addr == UNINITIALIZED_VEHPOOL_ADDR)
    {
        new addr_vehpool_addr = ReadPhysMemoryCell(ADDR_SERVER_DATA_ADDR[samp_version][os_version]); // Adres, pod którym zapisany jest adres puli pojazdów
        vehpool_addr = ReadPhysMemoryCell(addr_vehpool_addr + 0xC); // Adres puli pojazdów
    }

    return 1;
}

// Zapo¿yczenia z https://github.com/IS4Code/i_quat
static stock near_zero(Float:val)
{
    return (_:val & 0x7FFFFFFF) <= 0x1F800000;
}
static stock is_bad(Float:val)
{
    return (_:val & 0x7FFFFFFF) > 0x3F800000;
}


stock GetVehicleMatricesRot(vehicleid, Float:m1[], Float:m2[], Float:m3[])
{
    if(!VehicleMatricesFuncInit(m1, m2, m3))
    {
        return 0;
    }

    new veh_data_addr = ReadPhysMemoryCell(vehpool_addr + vehicleid * 4 + 0x3F54); // Adres pojazdu w puli
    ReadPhysMemory(veh_data_addr + 0xC, m1, 3); // m1 czyli kierunek osi X w postaci wektora (https://gtamods.com/wiki/Memory_Addresses_(SA))
    ReadPhysMemory(veh_data_addr + 0x1C, m2, 3); // m2 czyli kierunek osi Y w postaci wektora (https://gtamods.com/wiki/Memory_Addresses_(SA))
    ReadPhysMemory(veh_data_addr + 0x2C, m3, 3); // m3 czyli kierunek osi Z w postaci wektora (https://gtamods.com/wiki/Memory_Addresses_(SA))

    return 1;
}


stock SetVehicleMatricesRot(vehicleid, Float:m1[], Float:m2[], Float:m3[])
{
    if(!VehicleMatricesFuncInit(m1, m2, m3))
    {
        return 0;
    }

    new veh_data_addr = ReadPhysMemoryCell(vehpool_addr + vehicleid * 4 + 0x3F54);
    WritePhysMemory(veh_data_addr + 0xC, m1, 3);
    WritePhysMemory(veh_data_addr + 0x1C, m2, 3);
    WritePhysMemory(veh_data_addr + 0x2C, m3, 3);

    return 1;
}



stock Fixed_GetVehicleRotationQuat(vehicleid, &Float:w, &Float:x, &Float:y, &Float:z)
{
    if(samp_version != UNSUPPORTED_SAMP_VERSION && os_version != OS_UNKNOWN)
    {
        new Float:m1[3], Float:m2[3], Float:m3[3];
        if(GetVehicleMatricesRot(vehicleid, m1, m2 ,m3))
        {
            if((near_zero(m3[0]) && near_zero(m3[1]) && near_zero(m3[2])) || is_bad(m3[0]) || is_bad(m3[1]) || is_bad(m3[2]))
            {
                // m3 (oœ Z) musi byæ prostopad³y do m1 (oœ X) i m2 (oœ Y) - obliczamy iloczyn wektorowy m1 i m2
                m3[0] = m1[1] * m2[2] - m1[2] * m2[1];
                m3[1] = m1[2] * m2[0] - m1[0] * m2[2];
                m3[2] = m1[0] * m2[1] - m1[1] * m2[0];

                SetVehicleMatricesRot(vehicleid, m1, m2, m3);
            }
        }
    }

    return GetVehicleRotationQuat(vehicleid, w, x, y, z);
}

#if defined _ALS_GetVehicleRotationQuat
    #undef GetVehicleRotationQuat
#else
    #define _ALS_GetVehicleRotationQuat
#endif
#define GetVehicleRotationQuat Fixed_GetVehicleRotationQuat


#undef SAMP_037R3
#undef SAMP_03DL
#undef UNSUPPORTED_SAMP_VERSION
#undef UNINITIALIZED_SAMP_VERSION
#undef UNINITIALIZED_OS_VERSION
#undef UNINITIALIZED_VEHPOOL_ADDR