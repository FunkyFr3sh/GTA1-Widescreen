#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ddraw.h>
#include "patch.h"

static int g_width;
static int g_height;
static int g_width43;
static BOOL g_is_widescreen;

static void __declspec(naked) classic_check_chosen_res16()
{
    __asm {
        mov ecx, 0x13

        mov g_is_widescreen, 0

        cmp esi, 0x004B642C
        jnz patch_disabled

        mov g_is_widescreen, 1

        patch_disabled :

        retn
    }
}

static void __declspec(naked) classic_check_chosen_res32()
{
    __asm {
        mov ecx, 0x13

        mov g_is_widescreen, 0

        cmp esi, 0x004B5AAC
        jnz patch_disabled

        mov g_is_widescreen, 1

        patch_disabled :

        retn
    }
}

static void __declspec(naked) classic_fix_aspect_ratio()
{
    __asm {
        mov[ecx + 0x74F168], esi

        cmp g_is_widescreen, 1
        jnz no_widescreen

        mov esi, g_width43
        mov[ecx + 0x74F168], esi

        no_widescreen :

        retn
    }
}

static void __declspec(naked) classic_restore_width()
{
    __asm {
        cmp g_is_widescreen, 1
        jnz no_widescreen

        mov edx, g_width

        no_widescreen :

        mov ecx, 0x5C0C00
        mov[ecx], edx

        retn
    }
}

void classic_init(int a, int b)
{
    /* original function call replaced by the patch */
    ((void(*)(int, int))0x0048AFE0)(a, b);

    /* load custom resolution from .ini */
    g_width  = GetPrivateProfileIntA("GTA", "Width", 1068, ".\\GTA.ini");
    g_height = GetPrivateProfileIntA("GTA", "Height", 600, ".\\GTA.ini");

    /* add our custom resolution to the list of supported resolutions */
    LPDDENUMMODESCALLBACK lpEnumModesCallback = (void*)0x00491990;

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
    patch_setdword((DWORD*)0x004B642C + 0, g_width  - 1);
    patch_setdword((DWORD*)0x004B642C + 1, g_height - 1);

    /* overwrite 800x600x32 with our custom resolution */
    patch_setdword((DWORD*)0x004B5AAC + 0, g_width  - 1);
    patch_setdword((DWORD*)0x004B5AAC + 1, g_height - 1);

    /* check chosen res and make sure our patch is disabled for any res other than 800x600 */
    patch_call((void*)0x004904BF, (void*)classic_check_chosen_res16);
    patch_call((void*)0x004900B1, (void*)classic_check_chosen_res32);

    /* allow proper widescreen aspect ratio without stretching (Credits go to ThirteenAG for this hack!) */
    g_width43 = (int)(g_height * (4.0f / 3.0f));

    patch_clear((void*)0x0046453B, 0x90, (void*)0x00464541);
    patch_call((void*)0x0046453B, (void*)classic_fix_aspect_ratio);

    patch_clear((void*)0x0043B7CF, 0x90, (void*)0x0043B7D5);
    patch_call((void*)0x0043B7CF, (void*)classic_restore_width);
}
