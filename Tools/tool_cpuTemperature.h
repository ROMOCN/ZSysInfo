#ifndef TOOL_CPUTEMPERATURE_H
#define TOOL_CPUTEMPERATURE_H
#include "windows.h"
#include <tchar.h>
#include <iostream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define  MYWINRIN0 _T("MyWinRing0")
#define OLS_DRIVER_ID							_T("WinRing0_1_2_0")
#define OLS_TYPE 40000
#define IOCTL_OLS_READ_MSR \
    CTL_CODE(OLS_TYPE, 0x821, METHOD_BUFFERED, FILE_ANY_ACCESS)
class Tool_CpuTemperature
{
    BOOL gIsMsr = TRUE;
    TCHAR gDriverPath[MAX_PATH];
    HANDLE gHandle = INVALID_HANDLE_VALUE;
public:
    Tool_CpuTemperature()
    {
        Remove();
        Initialize();
    }
    BOOL Initialize()
    {
        TCHAR dir[MAX_PATH];
        TCHAR *ptr;

        GetModuleFileName(NULL, dir, MAX_PATH);
        if((ptr = _tcsrchr(dir, '\\')) != NULL)
        {
            *ptr = '\0';
        }
        wsprintf(gDriverPath, _T("%s\\%s"), dir, _T("WinRing0.sys"));
        if(IsFileExist(gDriverPath) == FALSE)
        {
            return FALSE;
        }
        if(OpenDriver())
        {
            return TRUE;
        }
        else
        {
            LoadDriver();
            OpenDriver();
        }
        return TRUE;

    }
    BOOL IsFileExist(LPCTSTR fileName)
    {
        WIN32_FIND_DATA	findData;

        HANDLE hFile = FindFirstFile(fileName, &findData);
        if(hFile != INVALID_HANDLE_VALUE)
        {
            FindClose( hFile );
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    //加载驱动
    int LoadDriver()
    {
        SC_HANDLE	hSCManager = NULL;
        SC_HANDLE	hService = NULL;
        SC_HANDLE	hService1 = NULL;
        hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
        hService = CreateService(hSCManager,
            MYWINRIN0,
            MYWINRIN0,
            SERVICE_ALL_ACCESS,
            SERVICE_KERNEL_DRIVER,
            SERVICE_DEMAND_START,
            SERVICE_ERROR_NORMAL,
            gDriverPath,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL
            );
        CloseServiceHandle(hService);
        hService1 = OpenService(hSCManager, MYWINRIN0, SERVICE_ALL_ACCESS);
        int result = StartService(hService1, 0, NULL);
        CloseServiceHandle(hSCManager);
        CloseServiceHandle(hService1);
        return result;
    }

    //打开驱动
    BOOL OpenDriver()
    {
        char message[256];
        TCHAR *str=_T("\\\\.\\") OLS_DRIVER_ID;
        gHandle = CreateFile(
            _T("\\\\.\\") OLS_DRIVER_ID,
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
            );
        int code=GetLastError();
        if(gHandle == INVALID_HANDLE_VALUE)
        {
            return FALSE;
        }
        return TRUE;
    }
    BOOL  StopDriver(SC_HANDLE hSCManager,LPCTSTR DriverId)
    {
        SC_HANDLE		hService = NULL;
        BOOL			rCode = FALSE;
        SERVICE_STATUS	serviceStatus;
        DWORD		error = NO_ERROR;

        hService = OpenService(hSCManager, DriverId, SERVICE_ALL_ACCESS);

        if(hService != NULL)
        {
            rCode = ControlService(hService, SERVICE_CONTROL_STOP, &serviceStatus);
            error = GetLastError();
            CloseServiceHandle(hService);
        }

        return rCode;
    }
    void Remove()
    {
        SC_HANDLE	hSCManager = NULL;
        hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
        StopDriver(hSCManager,MYWINRIN0);
        RemoveDriver(hSCManager,MYWINRIN0);
        CloseServiceHandle(hSCManager);
    }
    BOOL RemoveDriver(SC_HANDLE hSCManager, LPCTSTR DriverId)
    {
        SC_HANDLE   hService = NULL;
        BOOL        rCode = FALSE;

        hService = OpenService(hSCManager, DriverId, SERVICE_ALL_ACCESS);
        if(hService == NULL)
        {
            rCode = TRUE;
        }
        else
        {
            rCode = DeleteService(hService);
            CloseServiceHandle(hService);
        }

        return rCode;
    }
    void OnTimer()
    {
         DWORD   eax = 0 , edx = 0 ;
         ULONG   result ;
         char   s [ 20 ];
         result = SetThreadAffinityMask ( GetCurrentThread (), 1 );
         Rdmsr ( 0x19c ,& eax ,& edx ); //read Temperature
         SetThreadAffinityMask ( GetCurrentThread (), result );
         sprintf ( s , "%d" , 100 -(( eax & 0x007f0000 )>> 16 ));
         result = SetThreadAffinityMask ( GetCurrentThread (), 2 );
         Rdmsr ( 0x19c ,& eax ,& edx ); //read Temperature
         SetThreadAffinityMask ( GetCurrentThread (), result );
         sprintf ( s , "%d" , 100 -(( eax & 0x007f0000 )>> 16 ));

    }
    BOOL WINAPI Rdmsr(DWORD index, PDWORD eax, PDWORD edx)
    {
        if(gHandle == INVALID_HANDLE_VALUE)
        {
            return FALSE;
        }
        if(eax == NULL || edx == NULL || gIsMsr == FALSE)
        {
            return FALSE;
        }
        DWORD   returnedLength = 0;
        BOOL    result = FALSE;
        BYTE    outBuf[8] = {0};

        result = DeviceIoControl(
            gHandle,
            IOCTL_OLS_READ_MSR,
            &index,
            sizeof(index),
            &outBuf,
            sizeof(outBuf),
            &returnedLength,
            NULL
            );
        if(result)
        {
            memcpy(eax, outBuf, 4);
            memcpy(edx, outBuf + 4, 4);
        }
        if(result)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
};

#endif // TOOL_CPUTEMPERATURE_H
