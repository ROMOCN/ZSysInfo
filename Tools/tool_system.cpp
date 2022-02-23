#include "Tools/tool_system.h"

SystemInfo::SystemInfo()
{
    CPU = 0;
    RAM = 0;
    memoryAll = 0;
    memoryUse = 0;
//    NvAPI_QueryInterface()
//    NvAPI_QueryInterface = (NvAPI_QueryInterface_t)GetProcAddress(hModule, "nvapi_QueryInterface");
}

void SystemInfo::GetRAM()
{
#ifdef Q_OS_WIN
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof (statex);
    GlobalMemoryStatusEx(&statex);
    RAM = statex.dwMemoryLoad;
    memoryAll = statex.ullTotalPhys / MB;
    memoryFree = statex.ullAvailPhys / MB;
    memoryUse = memoryAll - memoryFree;

//    QString msg = QString("CPU : %1%  内存 : %2% ( 已用 %3 MB / 共 %4 MB )")
//                  .arg(cpuPercent).arg(RAM).arg(memoryUse).arg(memoryAll);
#endif

}


int SystemInfo::GetFPS()
{
    static int lastTime = GetTickCount(); // ms
    static int frameCount = 0;

    //NvAPI_GetInfoFrame
    ++frameCount;

    int curTime = GetTickCount();
    if (curTime - lastTime > 1000) // 取固定时间间隔为1秒
    {
        FPS = frameCount;
        frameCount = 0;
        lastTime = curTime;
    }
    return FPS;
}


int SystemInfo::GetCPU()
{
    BOOL res;
    // 上一秒cup自开机起总的空闲时间
    static FILETIME preidleTime;
    // 上一秒cup自开机起总的内核进程占用时间
    static FILETIME prekernelTime;
    // 上一秒cup自开机起总的用户进程占用时间
    static  FILETIME preuserTime;
    // 当前cup自开机起总的空闲时间
    FILETIME idleTime;
    // 当前cup自开机起总的内核进程占用时间
    FILETIME kernelTime;
    // 当前cup自开机起总的用户进程占用时间
    FILETIME userTime;
    // 获取一次cup占用时间，等一秒后再次获取新的时间
    //    res = GetSystemTimes( &preidleTime, &prekernelTime, &preuserTime);
    //    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //间隔一段时间 例如间隔1秒后
    res = GetSystemTimes( &idleTime, &kernelTime, &userTime );
    // 计算数据间隔

    LONGLONG idle =  (fileTimeToLongLong(idleTime) - fileTimeToLongLong(preidleTime));
    LONGLONG kernel =(fileTimeToLongLong(kernelTime) - fileTimeToLongLong(prekernelTime));
    LONGLONG user =  ( fileTimeToLongLong(userTime) - fileTimeToLongLong(preuserTime));
    int cpu = 0;
    LONGLONG runTime = (kernel - idle +user);
    LONGLONG preTime = (kernel+user);
    CPU = runTime * 100 /  preTime;
    cpu = CPU;
    //    QString msg = QString("CPU : %1%  内存 : %2% ( 已用 %3 MB / 共 %4 MB )")
    //            .arg(cpuPercent).arg(memoryPercent).arg(memoryUse).arg(memoryAll);
    //    qDebug()<< msg;

    preidleTime = idleTime;
    prekernelTime = kernelTime;
    preuserTime = userTime;
    //    qDebug()<< "cpu:" << cpu;
    return cpu;
}

byte SystemInfo::GetGPU() {
    nvmlReturn_t result;
    unsigned int device_count, i;
    // First initialize NVML library
    result = nvmlInit();


    result = nvmlDeviceGetCount(&device_count);
    if (NVML_SUCCESS != result)
    {
        qDebug()<< "Failed to query device count: " << nvmlErrorString(result);
    }
    //std::cout << "Found" << device_count << " device" << endl;
    nvmlDevice_t device;
    //std::cout << "Listing devices:";
    for (i = 0; i < device_count; i++)
    {

        char name[NVML_DEVICE_NAME_BUFFER_SIZE];
        nvmlPciInfo_t pci;
        result = nvmlDeviceGetHandleByIndex(i, &device);
        if (NVML_SUCCESS != result) {
            qDebug() << "get device failed " << endl;
        }
        result = nvmlDeviceGetName(device, name, NVML_DEVICE_NAME_BUFFER_SIZE);
//        if (NVML_SUCCESS == result) {
//            qDebug()<< "GPU name: " << name << endl;
//        }
        //使用率
        nvmlUtilization_t utilization;
        result = nvmlDeviceGetUtilizationRates(device, &utilization);
        GetGPUClock(device, result, i);
        if (NVML_SUCCESS == result)
        {
            if(utilization.gpu >= 0)
            {
//                qDebug()<< "GPU: " << utilization.gpu ;
                GPU =  utilization.gpu ;
//                qDebug()<< "GPURam: " << utilization.memory ;
            }

            return ((byte)utilization.gpu);
        }else
        {
            qDebug()<< "get GPU false ";
//            GPU = 0;
        }
    }




    return 0;
}

void SystemInfo::GetGPUClock(nvmlDevice_t device, nvmlReturn_t result, int i)
{
    qDebug()<<"------ Clocks ------- \n";
    unsigned int max_clock;
    result = nvmlDeviceGetMaxClockInfo(device, NVML_CLOCK_GRAPHICS, &max_clock);
    if (NVML_SUCCESS != result)
    {
        qDebug()<<QString("device %d   nvmlDeviceGetMaxClockInfo Failed : %s\n").arg(i).arg(nvmlErrorString(result));
    }

    unsigned int clock;
    result = nvmlDeviceGetClockInfo(device, NVML_CLOCK_GRAPHICS, &clock);
    if (NVML_SUCCESS != result)
    {
        qDebug()<<"Failed to get NVML_CLOCK_GRAPHICS info for device %i: %s\n", i, nvmlErrorString(result);
    }
    else
    {
        qDebug()<<QString("GRAPHICS： %d Mhz   max clock ：%d  \n").arg(clock).arg(max_clock);
    }

    result = nvmlDeviceGetMaxClockInfo(device, NVML_CLOCK_SM, &max_clock);
    if (NVML_SUCCESS != result)
    {
        qDebug()<<"Failed to get max NVML_CLOCK_SM for device %i: %s\n", i, nvmlErrorString(result);
    }

    result = nvmlDeviceGetClockInfo(device, NVML_CLOCK_SM, &clock);
    if (NVML_SUCCESS != result)
    {
        qDebug()<<"Failed to get current NVML_CLOCK_SM for device %i: %s\n", i, nvmlErrorString(result);
    }
    else
    {
        qDebug()<<QString("      SM： %d Mhz   max clock ：%d   \n").arg(clock).arg(max_clock);
    }

    result = nvmlDeviceGetMaxClockInfo(device, NVML_CLOCK_MEM, &max_clock);
    if (NVML_SUCCESS != result)
    {
        qDebug()<<"Failed to get max NVML_CLOCK_MEM for device %i: %s\n", i, nvmlErrorString(result);
    }

    result = nvmlDeviceGetClockInfo(device, NVML_CLOCK_MEM, &clock);
    if (NVML_SUCCESS != result)
    {
        qDebug()<<"Failed to get current NVML_CLOCK_MEM for device %i: %s\n", i, nvmlErrorString(result);
    }
    else
    {
        qDebug()<<QString("     MEM： %d Mhz   max clock ：%d   \n").arg(clock).arg(max_clock);
    }

    result = nvmlDeviceGetMaxClockInfo(device, NVML_CLOCK_VIDEO, &max_clock);
    if (NVML_SUCCESS != result)
    {
        qDebug()<<"Failed to get max NVML_CLOCK_VIDEO for device %i: %s\n", i, nvmlErrorString(result);
    }

    result = nvmlDeviceGetClockInfo(device, NVML_CLOCK_VIDEO, &clock);
    if (NVML_SUCCESS != result)
    {
        qDebug()<<"Failed to get current NVML_CLOCK_VIDEO for device %i: %s\n", i, nvmlErrorString(result);
    }
    else
    {
        qDebug()<<QString("   VIDEO： %1 Mhz   max clock ：%2   \n").arg(clock).arg(max_clock);
    }


}

std::pair<byte, ushort> SystemInfo::GetDiskRate_byte() {

    ULARGE_INTEGER FreeBytesAvailableC, TotalNumberOfBytesC, TotalNumberOfFreeBytesC;
    ULARGE_INTEGER FreeBytesAvailableD, TotalNumberOfBytesD, TotalNumberOfFreeBytesD;
    ULARGE_INTEGER FreeBytesAvailableE, TotalNumberOfBytesE, TotalNumberOfFreeBytesE;
    ULARGE_INTEGER FreeBytesAvailableF, TotalNumberOfBytesF, TotalNumberOfFreeBytesF;

    GetDiskFreeSpaceEx(_T("C:"), &FreeBytesAvailableC, &TotalNumberOfBytesC, &TotalNumberOfFreeBytesC);
    GetDiskFreeSpaceEx(_T("D:"), &FreeBytesAvailableD, &TotalNumberOfBytesD, &TotalNumberOfFreeBytesD);
    GetDiskFreeSpaceEx(_T("E:"), &FreeBytesAvailableE, &TotalNumberOfBytesE, &TotalNumberOfFreeBytesE);
    GetDiskFreeSpaceEx(_T("F:"), &FreeBytesAvailableF, &TotalNumberOfBytesF, &TotalNumberOfFreeBytesF);
//    float totalHardDisk = GB(TotalNumberOfBytesC) + GB(TotalNumberOfBytesD) + GB(TotalNumberOfBytesE) + GB(TotalNumberOfBytesF);
//    float freeHardDisk = GB(TotalNumberOfFreeBytesC) + GB(TotalNumberOfFreeBytesD) + GB(TotalNumberOfFreeBytesE) + GB(TotalNumberOfFreeBytesF);
    float totalHardDisk = GB(TotalNumberOfBytesD) + GB(TotalNumberOfBytesE) + GB(TotalNumberOfBytesF);
    float freeHardDisk = GB(TotalNumberOfFreeBytesD) + GB(TotalNumberOfFreeBytesE) + GB(TotalNumberOfFreeBytesF);

    double hardDiskUsage = (1 - freeHardDisk / totalHardDisk) * 100;
    auto res = std::make_pair((byte)hardDiskUsage, ushort(freeHardDisk));
    return res;
}
