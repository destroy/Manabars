// mana.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include "mana.h"
//#include "LDE64.h"
//#pragma comment(lib,"LDE64.lib")
#pragma comment(linker,"/entry:DllMain")
#pragma comment(linker, "/MERGE:.rdata=.text") 
#pragma comment(linker, "/MERGE:.data=.text") 
#pragma comment(linker, "/MERGE:code=.text") 
#pragma comment(linker,"/SECTION:.text,RWE")
#pragma optimize("gsy", on)
#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        //HaveFun();
    }
    return TRUE; 
}

//////////////////////////////////////////////////////////////////////////
#define ADDRESS LPVOID  // data
#define GADDRESS LPVOID // game call
 
GADDRESS g16FF24;   // 27B950 6f27B950
GADDRESS g16FF68;   // 334C00 6f334C00
GADDRESS a16FF64;   // 606860 6f606860
GADDRESS a16FF5C;   // 606370 6f606370
GADDRESS a16FF58;   // 35A740 6f35A740
GADDRESS a16FF20; // 32D300   6f
ADDRESS a16F088; //storm 0x191 #401
GADDRESS a2C7F10;
// as data
ADDRESS a16F08C; 
ADDRESS a16F004;
ADDRESS a1698A0;
ADDRESS a16F090;
ADDRESS a16F06C;
ADDRESS a16F070;
ADDRESS a3000AC; // should always be 1
ADDRESS a3000B0;


BYTE a16F008[0x80];

char* a164684 = "scaleFactor";

double a164A18 = 72; // ds:[00164A18]=72.00000000000000
double a164A10 = 0.0005000000237487257; //ds:[00164A10]=0.0005000000237487257
double a164A08 = 0.006000000052154064; //ds:[00164A08]=0.006000000052154064
double a1649D4 = 0.03000000; //ds:[004149D4]=0.03000000
double a1649D0 = 0.004000000;
double a1649CC = 0.3000000;

void __declspec(naked) f00152750()
{
    __asm
    {
      push    ebx
    mov     ebx, a16FF64    // ds:[0016FF64]=6F606860 (Game.6F606860)
    push    edi
    mov     edi, a16FF5C   // ds:[0016FF5C]=6F606370 (Game.6F606370)
    push    0
    push    0
    push    0
    xor     edx, edx
    mov     ecx, esi
    call    a16FF58         // ds:[0016FF58]=6F35A740 (Game.6F35A740)
    fld     a1649D4         // ds:[001649D4]=0.03000000
    push    0
    fstp    dword ptr [esi+0x58]
    xor     edx, edx
    mov     ecx, esi
    call    edi
    fld     a1649D0         // ds:[001649D0]=0.004000000
    push    0
    fstp    dword ptr [esi+0x5C]
    xor     edx, edx
    mov     ecx, esi
    call    edi
    fld     a1649CC         // ds:[001649CC]=0.3000000
    push    1
    sub     esp, 0x8
    fst     dword ptr [esp+0x4]
    xor     edx, edx
    fstp    dword ptr [esp]
    push    1
    mov     ecx, esi
    call    ebx
    mov     eax, dword ptr [esi]
    mov     eax, dword ptr [eax+0x64]
    pop     edi
    xor     edx, edx
    mov     ecx, esi
    pop     ebx
    jmp     eax
    }

}

void __declspec(naked) f001527C0()
{
    __asm{
            pop     a16F08C
            pop     eax
            add     eax, eax
            push    eax
            call    a16F088
            pushad
            mov     a16F004, eax
            mov     esi, a16F004
            add     esi, 0x158
            call    f00152750
            popad
            push    a16F08C
        retn
    }
}

void __declspec(naked) f152950()
{
    __asm{

            mov     eax, a16F090
            mov     eax, dword ptr [eax+0x64]
            push    esi
            push    edi
            mov     esi, edx
            mov     edi, ecx
            call    eax
            mov     eax, a16F090
            mov     eax, dword ptr [eax+0x64]
            lea     ecx, dword ptr [edi+0x158]
            pop     edi
            mov     edx, esi
            pop     esi
            jmp     eax

    }
}
void __declspec(naked) f152980()
{
    __asm{
    mov     eax, a16F090
    mov     eax, dword ptr [eax+0x68]
    jmp     eax
    }
}

void __declspec(naked) f00152710()
{
    __asm{
        mov     a16F090, ecx
        xor     eax, eax
        jmp L004
        lea     ebx, dword ptr [ebx]
L004:
        mov     dl, byte ptr [eax+ecx]
        mov     byte ptr [eax+a16F008], dl
        inc     eax
        cmp     eax, 0x80
        jb L004                 // copy函数数组，下面替换其中的两个函数
        push eax
        push ebx
        lea     eax, a16F008
        add     eax, 0x64
        lea     ebx, f152950
        mov     dword ptr [eax], ebx
        lea     eax, a16F008
        add     eax, 0x68
        lea     ebx, f152980
        mov     dword ptr [eax], ebx
        //mov     dword ptr [a16F008+0X6c], f152950
        //mov     dword ptr [a16F008+0X70], f152980
        pop ebx
        pop eax
        retn
    }
}


void __declspec(naked) f001527F0()
{
    __asm{
        sub     esp, 0x10
        cmp     a3000AC, 0
        push    edi
        mov     edi, a16F004
        je L093
        mov     eax, dword ptr [edi+0x50]
        test    eax, eax
        je L093
        cmp     a3000B0, 0
        push    ebx
        mov     ebx, dword ptr [eax+0xC]
        push    esi
        lea     esi, dword ptr [ebx+0x158]
        jnz L017
        mov     ecx, dword ptr [ebx]
        call    f00152710
        mov     a3000B0, 1
L017:
        push    0
        lea     eax, dword ptr [esp+0x10]
        push    eax
        xor     edx, edx
        mov     ecx, edi
        push eax
        lea eax,a16F008
        mov     dword ptr [ebx], eax
        pop eax
        call    g16FF24     // ds:[0016FF24]=6F27B950 (Game.6F27B950)
        fldz
        fcomp   dword ptr [esp+0xC]
        fstsw   ax
        test    ah, 1
        je L091
        push    3
        lea     ecx, dword ptr [esp+0x10]
        push    ecx
        xor     edx, edx
        mov     ecx, edi
        call    g16FF24     // ds:[0016FF24]=6F27B950 (Game.6F27B950)
        fldz
        fcomp   dword ptr [esp+0xC]
        fstsw   ax
        test    ah, 5
        jpe L091
        mov     eax, dword ptr [esi]
        mov     eax, dword ptr [eax+0x74]
        push    ebp
        push    edi
        xor     edx, edx
        mov     ecx, esi
        call    eax
        mov     ebx, a16FF64    //ds:[0016FF64]=6F606860 (Game.6F606860)
        mov     ebp, a16FF5C    // ds:[0016FF5C]=6F606370 (Game.6F606370)
        lea     ecx, dword ptr [esp+0x1C]
        push    ecx
        lea     edx, dword ptr [esp+0x18]
        mov     ecx, edi
        call    g16FF68         // ds:[0016FF68]=6F334C00 (Game.6F334C00)
        mov     ecx, dword ptr [edi+0x30]
        mov     eax, a16FF20   // [0016FF20]=6F32D300
        lea     edx, a164684
        call    eax
        test    eax, eax
        jnz L062
        fld1
        jmp L063
L062:
        fld     dword ptr [eax+0x54]
L063:
        fstp    dword ptr [esp+0x10]
        push    0
        fld     dword ptr [esp+0x14]
        xor     edx, edx
        fmul    qword ptr [a164A18]
        mov     ecx, esi
        fmul    qword ptr [a164A10]
        fstp    dword ptr [esi+0x58]
        call    ebp
        fld     dword ptr [esp+0x18]
        push    1
        fsub    qword ptr [a164A08]
        sub     esp, 8
        xor     edx, edx
        mov     ecx, esi
        fstp    dword ptr [esp+0x24]
        fld     dword ptr [esp+0x24]
        fstp    dword ptr [esp+0x4]
        fld     dword ptr [esp+0x20]
        fstp    dword ptr [esp]
        push    1
        call    ebx
        mov     eax, dword ptr [esi]
        mov     eax, dword ptr [eax+0x68]
        xor     edx, edx
        mov     ecx, esi
        call    eax
        pop     ebp
L091:
        pop     esi
        pop     ebx
L093:
        pop     edi
        add     esp, 0x10
        retn
}
}
void __declspec(naked) f00152930()
{
    __asm{
        pushad
        mov     a16F004, ecx
        call    f001527F0
        popad
        //mov     eax, a1698A0    // game.dll base
        mov     eax, a2C7F10
        jmp     eax
    }
}


ADDRESS a6F37A563;
ADDRESS a6F37A968;
void Hook(ADDRESS lpBase)
{

    DWORD old;
    VirtualProtect(a6F37A563, 5, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect(a6F37A968, 5, PAGE_EXECUTE_READWRITE, &old);
    
    {
        unsigned char* p = reinterpret_cast<unsigned char*>(a6F37A563);
        *p = 0xe8;
        p += 5;
        int X = (int)f001527C0 - (int)p;
        p -= 5;
        *reinterpret_cast<DWORD*>(p+1) = X;
    }
    {
        unsigned char* p = reinterpret_cast<unsigned char*>(a6F37A968);
        *p = 0xe8;
        p += 5;
        int X = (int)f00152930 - (int)p;
        p -= 5;
        *reinterpret_cast<DWORD*>(p+1) = X;
    }

}

BOOL WINAPI HaveFun()
{
    *(int*)&a3000AC = 1;
    HMODULE hMod = LoadLibraryA("storm.dll");
    a16F088 = (ADDRESS)GetProcAddress(hMod, (LPCSTR)0x191);
    a1698A0 = LoadLibraryA("game.dll");
    ADDRESS veraddr;
    *(int*)&veraddr = (int)a1698A0 + 0x636F5D;
    
    unsigned char p120e[] = {0x55, 0x8B, 0xEC, 0x53};
    unsigned char p124c[] = {0x00, 0x00, 0x75, 0x19};
    unsigned char pvs124[] = {0x80, 0xBE, 0xA8, 0x01};
    unsigned char p124d[] = {0x7c, 0x1E, 0x8B, 0xC1};
    unsigned char p124e[] = {0x8B, 0x50, 0x3C, 0x3B};
	unsigned char p125b[] = {0x10, 0x75, 0x72, 0xC7};
	unsigned char p126b[] = {0x7c, 0x73, 0x63, 0x6f};
	
    if (0 == memcmp(p120e, (unsigned char*)veraddr, sizeof(p120e)))
    {
        *(int*)&g16FF24 = (int)a1698A0 + 0x1C0030/*0x27B950*/;  // 6f27B950
        *(int*)&g16FF68 = (int)a1698A0 + 0x1264D0/*0x334C00*/;  // 6f334C00
        *(int*)&a16FF64 = (int)a1698A0 + 0x013780/*0x606860*/;  // 6f606860
        *(int*)&a16FF5C = (int)a1698A0 + 0x013AB0/*0x606370*/;  // 6f606370
        *(int*)&a16FF58 = (int)a1698A0 + 0x1529A0/*0x35A740*/;  // 6f35A740
        *(int*)&a16FF20 = (int)a1698A0 + 0x111700/*0x32D300*/;  // 6f32D300
        *(int*)&a2C7F10 = (int)a1698A0 + 0x1D63D0;  

        *(int*)&a6F37A563 = (int)a1698A0 + 0x166A14/*0x37A563*/;
        *(int*)&a6F37A968 = (int)a1698A0 + 0x167639/*0x37A968*/;
    }
    else if (0 == memcmp(p124c, (unsigned char*)veraddr, sizeof(p124c)))
    {
        *(int*)&g16FF24 = (int)a1698A0 + 0x27B950;  // 6f27B950
        *(int*)&g16FF68 = (int)a1698A0 + 0x334C00;  // 6f334C00
        *(int*)&a16FF64 = (int)a1698A0 + 0x606860;  // 6f606860
        *(int*)&a16FF5C = (int)a1698A0 + 0x606370;  // 6f606370
        *(int*)&a16FF58 = (int)a1698A0 + 0x35A740;  // 6f35A740
        *(int*)&a16FF20 = (int)a1698A0 + 0x32D300;  // 6f32D300
        *(int*)&a2C7F10 = (int)a1698A0 + 0x2C7F10; 

        *(int*)&a6F37A563 = (int)a1698A0 + 0x37A563;
        *(int*)&a6F37A968 = (int)a1698A0 + 0x37A968;
    }
    else if (0 == memcmp(pvs124, (unsigned char*)veraddr, sizeof(pvs124)))
    {
        // vs 显示的是1.24.1什么版本,但是都是1.24所以偏移非常好找
        *(int*)&g16FF24 = (int)a1698A0 + 0x27B950 ;  // 6f27B950
        *(int*)&g16FF68 = (int)a1698A0 + 0x334C00 ;  // 6f334C00
        *(int*)&a16FF64 = (int)a1698A0 + 0x6068A0 ;  // 6f606860
        *(int*)&a16FF5C = (int)a1698A0 + 0x6063B0 ;  // 6f606370
        *(int*)&a16FF58 = (int)a1698A0 + 0x35A740 ;  // 6f35A740
        *(int*)&a16FF20 = (int)a1698A0 + 0x32D300;  // 6f32D300
        *(int*)&a2C7F10 = (int)a1698A0 + 0x2C7F10; 

        *(int*)&a6F37A563 = (int)a1698A0 + 0x37A563;
        *(int*)&a6F37A968 = (int)a1698A0 + 0x37A968;
    }
    else if (0 == memcmp(p124d, (unsigned char*)veraddr, sizeof(p124d)))
    {
        *(int*)&g16FF24 = (int)a1698A0 + 0x27B950 ;  // 6f27B950
        *(int*)&g16FF68 = (int)a1698A0 + 0x334C60  ;  // 6f334C00
        *(int*)&a16FF64 = (int)a1698A0 + 0x6068C0  ;  // 6f606860
        *(int*)&a16FF5C = (int)a1698A0 + 0x6063D0  ;  // 6f606370
        *(int*)&a16FF58 = (int)a1698A0 + 0x35A7A0  ;  // 6f35A740
        *(int*)&a16FF20 = (int)a1698A0 + 0x32D360 ;  // 6f32D300
        *(int*)&a2C7F10 = (int)a1698A0 + 0x2C7F70 ; 

        *(int*)&a6F37A563 = (int)a1698A0 + 0x37A5C3 ;
        *(int*)&a6F37A968 = (int)a1698A0 + 0x37A9C8 ;
    }
    else if (0 == memcmp(p124e, (unsigned char*)veraddr, sizeof(p124e)))
    {
        *(int*)&g16FF24 = (int)a1698A0 + 0x27B9B0  ;  // 6F27B9B0 
        *(int*)&g16FF68 = (int)a1698A0 + 0x334CC0   ;  // 0x6f334CC0
        *(int*)&a16FF64 = (int)a1698A0 + 0x606950   ;  // 0x6f606950
        *(int*)&a16FF5C = (int)a1698A0 + 0x606460   ;  // 0x6f606460
        *(int*)&a16FF58 = (int)a1698A0 + 0x35A800   ;  // 0x6f35A800
        *(int*)&a16FF20 = (int)a1698A0 + 0x32D3C0  ;  // 0x32D3C0
        *(int*)&a2C7F10 = (int)a1698A0 + 0x32C650  ;    // 0x6f2C7FD0

        *(int*)&a6F37A563 = (int)a1698A0 + 0x37A623  ;  // 0x6f37A623
        *(int*)&a6F37A968 = (int)a1698A0 + 0x37AA28   ;  // 0x6F37AA28
    }
	else if (0 == memcmp(p125b, (unsigned char*)veraddr, sizeof(p125b)))
    {
        *(int*)&g16FF24 = (int)a1698A0 + 0x27AC60  ;  // 6F27B9B0 
        *(int*)&g16FF68 = (int)a1698A0 + 0x47CE50   ;  // 0x6f334CC0
        *(int*)&a16FF64 = (int)a1698A0 + 0x605F80   ;  // 0x6f606950
        *(int*)&a16FF5C = (int)a1698A0 + 0x605A90   ;  // 0x6f606460
        *(int*)&a16FF58 = (int)a1698A0 + 0x35A800   ;  // 0x6f35A800
        *(int*)&a16FF20 = (int)a1698A0 + 0x32C650  ;  // 0x32D3C0
        *(int*)&a2C7F10 = (int)a1698A0 + 0x32BB40  ;    // 0x6f2C7FD0???

        *(int*)&a6F37A563 = (int)a1698A0 + 0x28D6A1  ;  // 0x6f37A623
        *(int*)&a6F37A968 = (int)a1698A0 + 0x814158   ;  // 0x6F37AA28
    }
	else if (0 == memcmp(p126b, (unsigned char*)veraddr, sizeof(p126b)))
    {
        *(int*)&g16FF24 = (int)a1698A0 + 0x27AE90  ;  // 6F27B9B0 
        *(int*)&g16FF68 = (int)a1698A0 + 0x334180   ;  // 0x6f334CC0
        *(int*)&a16FF64 = (int)a1698A0 + 0x6061B0   ;  // 0x6f606950
        *(int*)&a16FF5C = (int)a1698A0 + 0x605CC0   ;  // 0x6f606460
        *(int*)&a16FF58 = (int)a1698A0 + 0x359CC0   ;  // 0x6f35A800
        *(int*)&a16FF20 = (int)a1698A0 + 0x32C880  ;  // 0x32D3C0
        *(int*)&a2C7F10 = (int)a1698A0 + 0x2C74B0  ;    // 0x6f2C7FD0??

        *(int*)&a6F37A563 = (int)a1698A0 + 0x379AE3  ;  // 0x6f37A623
        *(int*)&a6F37A968 = (int)a1698A0 + 0x379EE8   ;  // 0x6F37AA28
    }
    else
    {
        MessageBoxA(NULL, "don't support the version of current warcraft!", "error", MB_OK);
        return FALSE;
    }

    Hook(a1698A0);
    return TRUE;
}

BOOL WINAPI DoJob()
{
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif


