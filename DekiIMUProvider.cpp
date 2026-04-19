#include "DekiIMUProvider.h"
#include "DekiLogSystem.h"

DekiIMUProvider::Factory DekiIMUProvider::s_Factory = nullptr;
IDekiIMU*                DekiIMUProvider::s_Current = nullptr;

void DekiIMUProvider::SetFactory(Factory factory)
{
    s_Factory = factory;
    DEKI_LOG_INTERNAL("DekiIMUProvider: Factory registered");
}

IDekiIMU* DekiIMUProvider::Create()
{
    if (!s_Factory)
    {
        DEKI_LOG_ERROR("DekiIMUProvider: No factory registered - module must call SetFactory()");
        return nullptr;
    }
    return s_Factory();
}

bool DekiIMUProvider::HasFactory()
{
    return s_Factory != nullptr;
}

void DekiIMUProvider::SetCurrent(IDekiIMU* imu)
{
    s_Current = imu;
}

IDekiIMU* DekiIMUProvider::GetCurrent()
{
    return s_Current;
}
