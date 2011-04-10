#pragma once
#include <exception>
#include <Psapi.h>
class CWar
{
public:
  
    CWar();
    ~CWar();
    BOOL Find() ; // 以窗口寻找
    BOOL Create(LPCTSTR lpWarPath); // 创建war进程，暂时用不到

    BOOL DoWork();
    void MoniterAndDowork();
    operator HANDLE()
    {
        return m_handle;
    }
protected:
    BOOL EnalbePrivilege();
    BOOL FindGamedll(MODULEINFO* pInfo);
    LPVOID GetRelocBase(LPVOID lpModuleBaseAddr);
private:
    HANDLE m_handle; 
};
