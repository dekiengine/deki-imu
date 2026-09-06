#pragma once

#ifdef _WIN32
    #ifdef DEKI_IMU_EXPORTS
        #define DEKI_IMU_API __declspec(dllexport)
    #else
        #define DEKI_IMU_API __declspec(dllimport)
    #endif
#else
    #define DEKI_IMU_API __attribute__((visibility("default")))
#endif
