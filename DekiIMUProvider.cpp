#include "DekiIMUProvider.h"

IDekiIMU* DekiIMUProvider::s_Current = nullptr;

void DekiIMUProvider::SetCurrent(IDekiIMU* imu)
{
    s_Current = imu;
}

IDekiIMU* DekiIMUProvider::GetCurrent()
{
    return s_Current;
}
