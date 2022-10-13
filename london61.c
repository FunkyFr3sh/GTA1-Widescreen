#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ddraw.h>
#include "patch.h"

static int g_width;
static int g_height;
static int g_width43;
static BOOL g_is_widescreen;

static void __declspec(naked) london61_check_chosen_res16()
{
    __asm {
        mov ecx, 0x13

        mov g_is_widescreen, 0

        cmp esi, 0x005065bc
        jnz patch_disabled

        mov g_is_widescreen, 1

        patch_disabled :

        retn
    }
}

static void __declspec(naked) london61_check_chosen_res32()
{
    __asm {
        mov ecx, 0x13

        mov g_is_widescreen, 0

        cmp esi, 0x00505c3c
        jnz patch_disabled

        mov g_is_widescreen, 1

        patch_disabled :

        retn
    }
}

static void __declspec(naked) london61_fix_aspect_ratio()
{
    __asm {
        mov eax, [ecx + 0x80EB50]

        cmp g_is_widescreen, 1
        jnz no_widescreen

        mov esi, g_width43
        mov[ecx + 0x80EB50], esi

        no_widescreen :

        retn
    }
}

static void __declspec(naked) london61_restore_width()
{
    __asm {
        cmp g_is_widescreen, 1
        jnz no_widescreen

        mov edx, g_width

        no_widescreen :

        mov ecx, 0x628240
        mov[ecx], edx

        retn
    }
}

void london61_init(int a, int b)
{
    /* original function call replaced by the patch */
    ((void(*)(int, int))0x004BC5D0)(a, b);

    /* load custom resolution from .ini */
    g_width  = GetPrivateProfileIntA("GTA", "Width", 1068, ".\\GTA.ini");
    g_height = GetPrivateProfileIntA("GTA", "Height", 600, ".\\GTA.ini");

    /* add our custom resolution to the list of supported resolutions */
    LPDDENUMMODESCALLBACK lpEnumModesCallback = (void*)0x004C4A30;

    DDSURFACEDESC2 s = {0};
    s.dwSize = sizeof(DDSURFACEDESC);
    s.dwFlags = DDSD_HEIGHT | DDSD_REFRESHRATE | DDSD_WIDTH | DDSD_PITCH | DDSD_PIXELFORMAT;
    s.dwRefreshRate = 60;
    s.dwHeight = g_height;
    s.dwWidth = g_width;

    s.ddpfPixelFormat.dwFlags = DDPF_RGB;
    s.ddpfPixelFormat.dwRGBBitCount = 16;
    s.ddpfPixelFormat.dwRBitMask = 0xF800;
    s.ddpfPixelFormat.dwGBitMask = 0x07E0;
    s.ddpfPixelFormat.dwBBitMask = 0x001F;
    s.lPitch = ((s.dwWidth * s.ddpfPixelFormat.dwRGBBitCount + 31) & ~31) >> 3;

    lpEnumModesCallback((LPDDSURFACEDESC)&s, 0);

    s.ddpfPixelFormat.dwFlags = DDPF_RGB;
    s.ddpfPixelFormat.dwRGBBitCount = 32;
    s.ddpfPixelFormat.dwRBitMask = 0xFF0000;
    s.ddpfPixelFormat.dwGBitMask = 0x00FF00;
    s.ddpfPixelFormat.dwBBitMask = 0x0000FF;
    s.lPitch = ((s.dwWidth * s.ddpfPixelFormat.dwRGBBitCount + 31) & ~31) >> 3;

    lpEnumModesCallback((LPDDSURFACEDESC)&s, 0);

    /* overwrite 800x600x16 with our custom resolution */
    patch_setdword((DWORD*)0x005065bc + 0, g_width  - 1);
    patch_setdword((DWORD*)0x005065bc + 1, g_height - 1);

    /* overwrite 800x600x32 with our custom resolution */
    patch_setdword((DWORD*)0x00505c3c + 0, g_width  - 1);
    patch_setdword((DWORD*)0x00505c3c + 1, g_height - 1);

    /* check chosen res and make sure our patch is disabled for any res other than 800x600 */
    patch_call((void*)0x004C319F, (void*)london61_check_chosen_res16);
    patch_call((void*)0x004C2D91, (void*)london61_check_chosen_res32);

    /* allow proper widescreen aspect ratio without stretching (Credits go to ThirteenAG for this hack!) */
    g_width43 = (int)(g_height * (4.0f / 3.0f));

    patch_clear((void*)0x004854C3, 0x90, (void*)0x004854C9);
    patch_call((void*)0x004854C3, (void*)london61_fix_aspect_ratio);

    patch_clear((void*)0x00450BEF, 0x90, (void*)0x00450BF5);
    patch_call((void*)0x00450BEF, (void*)london61_restore_width);
}
