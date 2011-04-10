#pragma once
#ifdef MANA_EXPORTS

#define MYAPI __declspec(dllexport)
#else
#define MYAPI __declspec(dllimport)
#endif

 MYAPI BOOL WINAPI HaveFun();
 MYAPI BOOL WINAPI DoJob();