#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "patch.h"

void retail_init(int a, int b);
void london_init(int a, int b);
void london61_init(int a, int b);

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        HMODULE game_exe = GetModuleHandle(NULL);
        
        if (game_exe && memcmp((char*)game_exe + 0x00010000, "\x05\x00\x83\xC4\x04\x8B\x08\xC1\xF9\x07", 10) == 0)
        {
            patch_call((void*)0x00430194, (void*)retail_init);
        } /* GTA retail */
        
        else if (game_exe && memcmp((char*)game_exe + 0x00010000, "\x0F\xB6\x40\x07\x50\xE8\x8D\x2D\xFF\xFF", 10) == 0)
        {
            patch_call((void*)0x004BF808, (void*)london_init);
        } /* GTA london */
        
        else if (game_exe && memcmp((char*)game_exe + 0x00010000, "\x90\xA4\x00\x00\x00\x66\x89\x90\xD3\x00", 10) == 0)
        {
            patch_call((void*)0x00420227, (void*)london61_init);
        } /* GTA london 61 */
    }
    
    return TRUE;
}

#pragma comment(linker, "/export:_SmackBufferString@8=smackw32x._SmackBufferString@8,@1")
#pragma comment(linker, "/export:_SmackBufferOpen@24=smackw32x._SmackBufferOpen@24,@2")
#pragma comment(linker, "/export:_SmackBufferBlit@32=smackw32x._SmackBufferBlit@32,@3")
#pragma comment(linker, "/export:_SmackBufferFocused@4=smackw32x._SmackBufferFocused@4,@4")
#pragma comment(linker, "/export:_SmackBufferNewPalette@12=smackw32x._SmackBufferNewPalette@12,@5")
#pragma comment(linker, "/export:_SmackBufferClose@4=smackw32x._SmackBufferClose@4,@6")
#pragma comment(linker, "/export:_SmackBufferSetPalette@4=smackw32x._SmackBufferSetPalette@4,@7")
#pragma comment(linker, "/export:_SmackBufferClear@8=smackw32x._SmackBufferClear@8,@8")
#pragma comment(linker, "/export:_SmackSetSystemRes@4=smackw32x._SmackSetSystemRes@4,@9")
#pragma comment(linker, "/export:_SmackBufferToBuffer@32=smackw32x._SmackBufferToBuffer@32,@10")
#pragma comment(linker, "/export:_SmackBufferToBufferTrans@36=smackw32x._SmackBufferToBufferTrans@36,@11")
#pragma comment(linker, "/export:_SmackBufferFromScreen@16=smackw32x._SmackBufferFromScreen@16,@12")
#pragma comment(linker, "/export:_SmackBufferCopyPalette@12=smackw32x._SmackBufferCopyPalette@12,@13")
#pragma comment(linker, "/export:_SmackOpen@12=smackw32x._SmackOpen@12,@14")
#pragma comment(linker, "/export:_SmackSimulate@4=smackw32x._SmackSimulate@4,@15")
#pragma comment(linker, "/export:_SmackFrameRate@4=smackw32x._SmackFrameRate@4,@16")
#pragma comment(linker, "/export:_SmackSoundOnOff@8=smackw32x._SmackSoundOnOff@8,@17")
#pragma comment(linker, "/export:_SmackClose@4=smackw32x._SmackClose@4,@18")
#pragma comment(linker, "/export:_SmackDoFrame@4=smackw32x._SmackDoFrame@4,@19")
#pragma comment(linker, "/export:_SmackSummary@8=smackw32x._SmackSummary@8,@20")
#pragma comment(linker, "/export:_SmackNextFrame@4=smackw32x._SmackNextFrame@4,@21")
#pragma comment(linker, "/export:_SmackToScreen@24=smackw32x._SmackToScreen@24,@22")
#pragma comment(linker, "/export:_SmackToBuffer@28=smackw32x._SmackToBuffer@28,@23")
#pragma comment(linker, "/export:_SmackColorTrans@8=smackw32x._SmackColorTrans@8,@24")
#pragma comment(linker, "/export:_SmackColorRemap@16=smackw32x._SmackColorRemap@16,@25")
#pragma comment(linker, "/export:_SmackGetTrackData@12=smackw32x._SmackGetTrackData@12,@26")
#pragma comment(linker, "/export:_SmackGoto@8=smackw32x._SmackGoto@8,@27")
#pragma comment(linker, "/export:_SmackToBufferRect@8=smackw32x._SmackToBufferRect@8,@28")
#pragma comment(linker, "/export:_SmackSoundInTrack@8=smackw32x._SmackSoundInTrack@8,@29")
#pragma comment(linker, "/export:_SmackVolumePan@16=smackw32x._SmackVolumePan@16,@30")
#pragma comment(linker, "/export:_SmackSoundCheck@0=smackw32x._SmackSoundCheck@0,@31")
#pragma comment(linker, "/export:_SmackWait@4=smackw32x._SmackWait@4,@32")
#pragma comment(linker, "/export:_SmackSoundUseMSS@4=smackw32x._SmackSoundUseMSS@4,@33")
#pragma comment(linker, "/export:_SmackSoundUseDW@12=smackw32x._SmackSoundUseDW@12,@34")
#pragma comment(linker, "/export:_donemarker@20=smackw32x._donemarker@20,@35")
#pragma comment(linker, "/export:_TimerFunc@20=smackw32x._TimerFunc@20,@36")
#pragma comment(linker, "/export:_SetDirectSoundHWND@4=smackw32x._SetDirectSoundHWND@4,@37")
#pragma comment(linker, "/export:_SmackSoundUseDirectSound@4=smackw32x._SmackSoundUseDirectSound@4,@38")
#pragma comment(linker, "/export:_SmackSoundUseWin@0=smackw32x._SmackSoundUseWin@0,@39")
