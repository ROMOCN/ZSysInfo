#ifndef TOOL_SYSTEM_H
#define TOOL_SYSTEM_H
#include <QObject>
#include <QDebug>
#include <tchar.h>
#include <stdio.h>
#ifdef Q_OS_WIN
#include "windows.h"
#endif
#define MB (1024 * 1024)
#define KB (1024)
//-----GPU
#include <include/nvml.h>
typedef unsigned char byte;
//-----Disk
#define GB(x) (((x).HighPart << 2) + ((DWORD)(x).LowPart) / 1024.0 / 1024.0 / 1024.0)

class SystemInfo : public QObject
{
    Q_OBJECT
public:
    int CPU;
    int RAM;
    int memoryAll;
    int memoryUse;
    int memoryFree;
    int FPS;
    int GPU;
    SystemInfo();
    int GetCPU();
    void GetRAM();
    int GetFPS();

    byte GetGPU();
    void GetGPUClock(nvmlDevice_t device, nvmlReturn_t result, int i);
    std::pair<byte, ushort> GetDiskRate_byte();

    inline long long fileTimeToLongLong(FILETIME time1)
    {
        long long a = time1.dwHighDateTime << 32 | time1.dwLowDateTime ;
        return a;
    }
//    NVAPI_INTERFACE NvAPI_GetInfoFrame(NvDisplayHandle  hNvDisplay,
//      NvU32  displayId,
//      NV_INFOFRAME_TYPE  type,
//      NV_INFOFRAME *  pInfoFrame) {}



private:


};

#endif // TOOL_SYSTEM_H
