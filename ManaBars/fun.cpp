#include "stdafx.h"
#include "fun.h"
#include <psapi.h>
#include "..\mana\mana.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\mana.lib")
#else
#pragma comment(lib, "..\\Release\\mana.lib")
#endif


#pragma comment(lib,"psapi.lib")

BOOL CWar::Find()
{
    HWND hWnd = ::FindWindow(_T("Warcraft III"), _T("Warcraft III"));
    if (!hWnd)
    {
        throw std::exception("没有找到魔兽窗口!请运行魔兽后再执行操作");
    }
    DWORD dwPid;
    GetWindowThreadProcessId(hWnd, &dwPid);
    m_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    if (!m_handle)
    {
        if(!EnalbePrivilege())
            throw std::exception("进程提权失败！无法打开魔兽进程");
        m_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
        if (!m_handle)
        {
            throw std::exception("无法打开魔兽进程，请尝试以管理员权限加载本程序");
        }
    }
    return TRUE;
}

BOOL CWar::EnalbePrivilege()
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tp = {0};
    HANDLE hProcess = GetCurrentProcess();
    if (!OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
        &hToken))
        return FALSE;
    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid))
    {
        CloseHandle(hToken);
        return FALSE;
    }
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES),
        NULL, NULL);
    CloseHandle(hToken);
    return TRUE;
}

CWar::CWar()
: m_handle(0)
{
    DoJob();
}

CWar::~CWar()
{
    if (m_handle)
    {
        CloseHandle(m_handle);
        m_handle = 0;
    }
}
LPVOID CWar::GetRelocBase(LPVOID lpModuleBaseAddr)
{
    PIMAGE_DOS_HEADER pImg_DOS_Header = (PIMAGE_DOS_HEADER)lpModuleBaseAddr;
    PIMAGE_NT_HEADERS pImg_NT_Header = (PIMAGE_NT_HEADERS)((ULONG)pImg_DOS_Header + pImg_DOS_Header->e_lfanew);

    _IMAGE_FILE_HEADER FileHeader = pImg_NT_Header->FileHeader;
    WORD NumberOfSections = FileHeader.NumberOfSections;
    IMAGE_SECTION_HEADER* pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pImg_NT_Header + sizeof(IMAGE_NT_HEADERS));
    for (WORD i=0; i<NumberOfSections; i++)
    {
        if (!lstrcmpiA((CHAR*)(pIMAGE_SECTION_HEADER->Name), ".reloc"))
        {
            return (LPVOID)((DWORD)lpModuleBaseAddr + pIMAGE_SECTION_HEADER->VirtualAddress);
        }
        pIMAGE_SECTION_HEADER += 1; //VC是智能的，它自己知道加多长的字节！这里弄了好久
    }
    return 0;
}
BOOL CWar::DoWork()
{
    //MODULEINFO info;
    //if (!FindGamedll(&info))
    //{
    //    throw std::exception("获取game.dll模块失败！");
    //}
    //
    HMODULE hMod = GetModuleHandleA("mana.dll");
    MODULEINFO info;
    GetModuleInformation(GetCurrentProcess(), hMod, &info, sizeof(info));
    LPVOID lpAlloc = VirtualAlloc(NULL, info.SizeOfImage, MEM_COMMIT, PAGE_READWRITE);
    memcpy(lpAlloc, info.lpBaseOfDll, info.SizeOfImage);
    LPVOID lpReloc = GetRelocBase(lpAlloc);
    
    LPVOID lpTargAddr = VirtualAllocEx(m_handle, NULL, info.SizeOfImage, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    DWORD dwX = (DWORD)lpTargAddr - (DWORD)info.lpBaseOfDll; // 差值
    char* pcurReloc = (char*)lpReloc;
    DWORD pBlockRVA;
    DWORD dwBlockSize, dwCurSize;
    WORD itemRVA;
    do
    {
        pBlockRVA = *(DWORD *)pcurReloc;
        dwBlockSize = *(DWORD *)(pcurReloc + 4);
        dwCurSize = 8;
        pcurReloc += 8;
        if ( dwBlockSize > 8 )
        {
            do
            {
                itemRVA = *(WORD *)pcurReloc;
                pcurReloc += 2;
                dwCurSize += 2;
                if ( itemRVA >> 12 )
                {
                    if ( itemRVA >> 12 != 3 )
                        return 4;
                    *(DWORD *)((char *)lpAlloc + pBlockRVA + (itemRVA & 0xFFF)) += dwX;
                }
            }
            while ( dwCurSize < dwBlockSize );
        }
    }
    while ( dwBlockSize );

    DWORD dwTid;
    WriteProcessMemory(m_handle, lpTargAddr, lpAlloc, info.SizeOfImage, &dwTid);
    HANDLE hThread = CreateRemoteThread(m_handle, NULL, 0, LPTHREAD_START_ROUTINE((char*)HaveFun - (char*)info.lpBaseOfDll + (char*)lpTargAddr), 0, CREATE_DEFAULT_ERROR_MODE, &dwTid);
    //WaitForSingleObject(hThread, 10*1000);
    VirtualFree(lpAlloc, 0, MEM_RELEASE);
    //VirtualFreeEx(m_handle, lpTargAddr, 0, MEM_RELEASE);
    return TRUE;
}

void CWar::MoniterAndDowork()
{
    while (TRUE)
    {
        try
        {
           Find();
           Sleep(3000);
           DoWork();
           WaitForSingleObject(m_handle, INFINITE);
           CloseHandle(m_handle);
        }
        catch (std::exception& )
        {
            Sleep(3000);
        }
    }
}

//BOOL CWar::FindGamedll(MODULEINFO* pInfo)
//{
//    if (!pInfo)
//    {
//        return FALSE;
//    }
//    HMODULE hMods[1024];
//    DWORD cbNeeded;
//    unsigned int i;
// 
//    if( EnumProcessModules( m_handle, hMods, sizeof(hMods), &cbNeeded))
//    {
//        for ( i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
//        {
//            char szModName[MAX_PATH];
//            if ( GetModuleBaseNameA( m_handle, hMods[i], szModName,
//                sizeof(szModName)))
//            {
//                if(0 == lstrcmpiA(szModName, "game.dll"))
//                {
//                    GetModuleInformation(m_handle, hMods[i], pInfo, sizeof(MODULEINFO));
//                    return TRUE;
//                }
//                //printf("\t%s (0x%08X)\n", szModName, hMods[i] );
//            }
//        }
//    }
//    return FALSE;
//}
