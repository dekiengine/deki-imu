#pragma once

#include "IDekiIMU.h"

class DekiIMUProvider
{
public:
    using Factory = IDekiIMU* (*)();

    static void      SetFactory(Factory factory);
    static IDekiIMU* Create();
    static bool      HasFactory();

    static void      SetCurrent(IDekiIMU* imu);
    static IDekiIMU* GetCurrent();

private:
    static Factory   s_Factory;
    static IDekiIMU* s_Current;
};
