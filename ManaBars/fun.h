#pragma once
#include <exception>
#include <Psapi.h>
class CWar
{
public:
  
    CWar();
    ~CWar();
    BOOL Find() ; // �Դ���Ѱ��
    BOOL Create(LPCTSTR lpWarPath); // ����war���̣���ʱ�ò���

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
