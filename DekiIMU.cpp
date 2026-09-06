#include "DekiIMU.h"

IDekiIMU* DekiIMU::s_Current = nullptr;

void DekiIMU::SetCurrent(IDekiIMU* imu)
{
    s_Current = imu;
}

IDekiIMU* DekiIMU::GetCurrent()
{
    return s_Current;
}
