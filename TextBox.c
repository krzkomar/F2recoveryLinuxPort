/* Not done */
#include "FrameWork.h"

#define TEXTBOX_MARGIN_H 2
#define TEXTBOX_MARGIN_V 8

typedef struct 
{
  int i01;
  int i02;
  int i03;
  int i04;
  int i05;
  int i06;
  int i07;
  int i08;
  int i09;
  int i10;
  char *i11;
} TextBox_t;


int TextBoxFileSelect( char *Title, char **List, int Win, int cnt, int w, int h, int flags )
{
    return TextBoxSelect( Title, List, Win, cnt, w, h, flags, 0 );
}

int TextBoxSelect( char *msg, char **FileList, int n, int ebx0, int a0,int a1,int a7,int a8 )
{
/*
    int v8; // edi
    unsigned int v10; // esi
    signed int v11; // eax
    int v12; // edx
    unsigned int v13; // edx
    signed int v14; // esi
    int i; // ebp
    char *Surface; // edx
    int v17; // eax
    int v18; // eax
    int v19; // eax
    int v20; // eax
    int v21; // ebp
    int v22; // ebp
    const char **v23; // edx
    int v24; // ebx
    int v25; // eax
    int v26; // eax
    int v27; // edx
    int v28; // ecx
    int v29; // eax
    int v30; // edx
    int v31; // eax
    int v32; // ecx
    int v33; // eax
    int v34; // eax
    int v35; // edx
    int v36; // eax
    int v37; // edx
    int v38; // eax
    int v39; // edx
    int v40; // eax
    int v41; // ecx
    int v42; // eax
    int v43; // ebx
    int v44; // ecx
    int v45; // ecx
    int v46; // eax
    const char **v47; // edx
    int v48; // ebx
    int v49; // eax
    int v50; // eax
    int v51; // ecx
    int v52; // edx
    unsigned int v53; // ebx
    int v54; // eax
    int v55; // edx
    int v56; // ebx
    int v57; // eax
    int v58; // ecx
    int v59; // eax
    unsigned int v60; // ebx
    int v61; // eax
    const char *v62; // edx
    int v63; // eax
    int v64; // edx
    int v65; // ebx
    int v66; // eax
    int v67; // [esp-20h] [ebp-DCh]
    int v68; // [esp-4h] [ebp-C0h]
    int v69; // [esp-4h] [ebp-C0h]
    int v70; // [esp+0h] [ebp-BCh]
    int v71; // [esp+0h] [ebp-BCh]
    int v72; // [esp+0h] [ebp-BCh]
    int v73; // [esp+0h] [ebp-BCh]
    int v74; // [esp+0h] [ebp-BCh]
    int v75; // [esp+0h] [ebp-BCh]
    VidRect_t v76; // [esp+4h] [ebp-B8h] BYREF
    int bottom; // [esp+14h] [ebp-A8h] BYREF
    int right; // [esp+18h] [ebp-A4h] BYREF
    char **v79; // [esp+1Ch] [ebp-A0h]
    char *pal; // [esp+20h] [ebp-9Ch]
    int y1; // [esp+24h] [ebp-98h]
    int a3; // [esp+28h] [ebp-94h]
    int v83; // [esp+2Ch] [ebp-90h]
    int a5; // [esp+30h] [ebp-8Ch]
    int v85; // [esp+34h] [ebp-88h]
    int v86; // [esp+38h] [ebp-84h]
    int v87; // [esp+3Ch] [ebp-80h]
    int x0; // [esp+40h] [ebp-7Ch]
    int posX; // [esp+44h] [ebp-78h]
    int xx; // [esp+48h] [ebp-74h]
    char *pDst; // [esp+4Ch] [ebp-70h]
    unsigned int v92; // [esp+50h] [ebp-6Ch]
    int Color; // [esp+54h] [ebp-68h]
    unsigned int v94; // [esp+58h] [ebp-64h]
    int Height; // [esp+5Ch] [ebp-60h]
    unsigned int a4; // [esp+60h] [ebp-5Ch]
    int v97; // [esp+64h] [ebp-58h]
    int g0; // [esp+68h] [ebp-54h]
    int a6; // [esp+6Ch] [ebp-50h]
    int v100; // [esp+70h] [ebp-4Ch]
    VidRect_t *p_Geometry; // [esp+74h] [ebp-48h]
    int v102; // [esp+78h] [ebp-44h]
    int Ypos; // [esp+7Ch] [ebp-40h]
    Window01_t *v104; // [esp+80h] [ebp-3Ch]
    Window01_t *win; // [esp+84h] [ebp-38h]
    Window_t *Window; // [esp+88h] [ebp-34h]
    unsigned int Width; // [esp+8Ch] [ebp-30h]
    char **v108; // [esp+90h] [ebp-2Ch]
    int arg4; // [esp+94h] [ebp-28h]
    int posY; // [esp+98h] [ebp-24h]
    unsigned int v111; // [esp+9Ch] [ebp-20h]
    unsigned int v112; // [esp+A0h] [ebp-1Ch]
    unsigned int a2; // [esp+A4h] [ebp-18h]
    int Pitch; // [esp+A8h] [ebp-14h]
    int ebx0a; // [esp+ACh] [ebp-10h]

    v8 = a8;
    y1 = msg;
    v104 = FileList;
    Pitch = n;
    Height = ebx0;
    v111 = -1;
    v102 = -1;
    if ( !gWinSys.Init )
        return -1;
    a2 = TextBoxListWidth(v104, n);
    v10 = a2 + 16;
    v11 = gFont.LineWidth(y1);
    if ( v11 > (a2 + 16) )
    {
        v13 = v11 - v10 + v12;
        v10 = v11;
        a2 = v13;
    }
    v14 = v10 + 20;
    ebx0a = 10;
    for ( i = 13;
          i > 8;
          --i )
    {
        v97 = i * gFont.ChrHeight() + 22;
        win = WinCreateWindow(a0, a1, v14, v97, 256, 20);
        if ( win != -1 )
            break;
        --ebx0a;
    }
    if ( win == -1 )
        return -1;
    Window = WinGetWindow(win);
    Surface = Window->Surface;
    p_Geometry = &Window->Geometry;
    pDst = Surface;
    ScrRectangle(Surface, v14, 0, 0, v14 - 1, v97 - 1, gPalColorCubeRGB[0][0][0]);
    ScrShadowRectangle(pDst, v14, 1, 1, v14 - 2, v97 - 2, gPalColorCubeRGB[0][0][gWinSys.Colors[1]], gPalColorCubeRGB[0][0][gWinSys.Colors[2]]);
    v79 = (v14 / 2 + 8 * v14 - gFont.LineWidth(y1) / 2);
    v70 = gPalColorCubeRGB[0][0][gWinSys.Colors[0]];
    v17 = gFont.ChrHeight();
    ScrFillSolid(&pDst[5 * v14 + 5], v14 - 11, v17 + 3, v14, v70);
    gFont.Print(&pDst[v79], y1, v14, v14, gPalColorCubeRGB[0][0][gWinSys.Colors[3]] | 0x10000);
    v71 = gPalColorCubeRGB[0][0][gWinSys.Colors[1]];
    v68 = gPalColorCubeRGB[0][0][gWinSys.Colors[2]];
    v18 = gFont.ChrHeight();
    ScrShadowRectangle(pDst, v14, 5, 5, v14 - 6, v18 + 8, v68, v71);
    arg4 = gFont.ChrHeight() + 16;
    v19 = gFont.ChrHeight();
    a3 = ebx0a * v19 + arg4;
    v108 = &pDst[v14 * arg4 + 8];
    Ypos = 8;
    ScrFillSolid(v108 - 2 * v14 - 3, a2 + 6, ebx0a * v19 + 2, v14, gPalColorCubeRGB[0][0][gWinSys.Colors[0]]);
    if ( a8 < 0 || a8 >= Pitch )
        v8 = 0;
    if ( Pitch - v8 >= ebx0a )
    {
        v22 = 0;
    }
    else
    {
        v20 = Pitch - ebx0a;
        if ( Pitch - ebx0a < 0 )
            v20 = 0;
        v21 = v8;
        v8 = v20;
        v22 = v21 - v20;
    }
    v23 = (&v104->Id + v8);
    if ( Pitch >= ebx0a )
        WinDrawTextList(win, v23, ebx0a, a2, Ypos, arg4, a7 | 0x2000000);
    else
        WinDrawTextList(win, v23, Pitch, a2, Ypos, arg4, a7 | 0x2000000);
    v24 = gFont.ChrHeight();
    v25 = gFont.ChrHeight();
    ScrEnlight(v108 + v14 * v22 * v25, a2, v24, v14);
    ScrShadowRectangle(pDst, v14, 5, arg4 - 3, a2 + 10, a3, gPalColorCubeRGB[0][0][gWinSys.Colors[2]], gPalColorCubeRGB[0][0][gWinSys.Colors[1]]);
    v79 = (v14 - 25);
    WinAddButton(win, v14 - 25, arg4 - 3, -1, -1, 328, -1, byte_50FA48, 0);
    v26 = gFont.ChrHeight();
    WinAddButton(win, v27, a3 - v26 - 5, v28, -1, 336, -1, byte_50FA4C, 0);
    v29 = gFont.ChrHeight();
    WinAddButton(win, v14 / 2 - 32, v30 - v29 - 6, -1, -1, -1, 27, "Done", 0);
    xx = v14 - 21;
    v31 = gFont.ChrHeight();
    posY = v32 + v31 + 7;
    Width = 14;
    v72 = gPalColorCubeRGB[0][0][gWinSys.Colors[0]];
    v79 = (a3 - posY);
    v33 = gFont.ChrHeight();
    pal = &pDst[v14 - 21 + v14 * posY];
    ScrFillSolid(pal, 0xFu, a3 - posY - v33 - 8, v14, v72);
    v34 = gFont.ChrHeight();
    WinCreateButton(win, v14 - 21, posY, 15, v35 - v34 - 8, -1, -1, 2048, -1, 0, 0, 0, 0);
    v73 = gPalColorCubeRGB[0][0][gWinSys.Colors[1]];
    v69 = gPalColorCubeRGB[0][0][gWinSys.Colors[2]];
    v36 = gFont.ChrHeight();
    ScrShadowRectangle(pDst, v14, v14 - 22, posY - 1, v14 - 21 + 15, v37 - v36 - 9, v69, v73);
    ScrShadowRectangle(pDst, v14, xx, posY, xx + 14, posY + 14, gPalColorCubeRGB[0][0][gWinSys.Colors[1]], gPalColorCubeRGB[0][0][gWinSys.Colors[2]]);
    ScrEnlight(pal, 0xEu, 14, v14);
    v100 = 0;
    if ( ebx0a > 0 )
    {
        a6 = 1024;
        g0 = 512;
        do
        {
            v67 = gFont.ChrHeight();
            v38 = gFont.ChrHeight();
            WinCreateButton(win, Ypos, v39 * v38 + arg4, a2, v67, g0++, -1, a6++, -1, 0, 0, 0, 0);
            ++v100;
        }
        while ( v100 < ebx0a );
    }
    v40 = gFont.ChrHeight();
    WinCreateButton(win, 0, 0, v41, v40 + 8, -1, -1, -1, -1, 0, 0, 0, 16);
    WinUpdate(win);
    v94 = Width + 1;
    v92 = &pDst[xx];
    a4 = a3 - arg4;
    v112 = Pitch - ebx0a;
    a5 = Width + xx;
    Color = a7 | 0x2000000;
    v85 = 4 * a7;
    HIWORD(v42) = HIWORD(a7);
    LOWORD(v42) = 0;
    v83 = v42;
    x0 = Pitch - 1;
    posX = ebx0a - 1;
    v87 = ebx0a + 512;
    v86 = ebx0a + 1024;
    while ( 1 )
    {
        while ( 1 )
        {
            while ( 1 )
            {
                v43 = InpUpdate();
                MseGetCursorPosition(&right, &bottom);
                if ( v43 != 13 && (v43 < 1024 || v43 >= v86) )
                    break;
                if ( v22 != -1 )
                {
                    v102 = v8 + v22;
                    if ( v8 + v22 < Pitch )
                    {
                        if ( !Height )
                            goto LABEL_90;
                        (Height)();
                    }
                    v102 = -1;
                }
            }
            if ( v44 != 2048 )
                break;
            if ( Window->Geometry.tp + posY > bottom )
            {
                v44 = 329;
                break;
            }
            if ( (Window->Geometry.tp + Width + posY) < bottom )
            {
                v44 = 337;
                break;
            }
        }
        if ( v44 == 27 )
            break;
        if ( v44 < 512 || v44 >= v87 )
        {
            switch ( v44 )
            {
                case 327:
                    if ( v8 > 0 )
                    {
                        v44 = -4;
                        v8 = 0;
                    }
                    break;
                case 328:
                    if ( v22 <= 0 )
                    {
                        if ( v8 > 0 )
                        {
                            v44 = -4;
                            --v8;
                        }
                    }
                    else
                    {
                        v44 = -3;
                        v111 = v22--;
                    }
                    break;
                case 329:
                    if ( v8 > 0 )
                    {
                        v8 -= ebx0a;
                        if ( v8 < 0 )
                            v8 = 0;
                        v44 = -4;
                    }
                    break;
                case 335:
                    if ( v8 < v112 )
                    {
                        v44 = -4;
                        v8 = v112;
                    }
                    break;
                case 336:
                    if ( v22 >= posX || v22 >= x0 )
                    {
                        if ( v8 + ebx0a < Pitch )
                        {
                            v44 = -4;
                            ++v8;
                        }
                    }
                    else
                    {
                        v44 = -3;
                        v111 = v22++;
                    }
                    break;
                case 337:
                    if ( v8 < v112 )
                    {
                        v8 += ebx0a;
                        if ( v8 > v112 )
                            v8 = v112;
                        v44 = -4;
                    }
                    break;
                default:
                    if ( Pitch > ebx0a && (v44 >= 97 && v44 <= 122 || v44 >= 65 && v44 <= 90) )
                    {
                        v46 = TextBoxSearchLine(v44, v104, Pitch);
                        v44 = v46;
                        if ( v46 != -1 )
                        {
                            v8 = v46;
                            if ( v46 > v112 )
                                v8 = v112;
                            v44 = -4;
                            v22 = v46 - v8;
                        }
                    }
                    break;
            }
LABEL_75:
            if ( v44 == -4 )
            {
                ScrFillSolid(v108, a2, a4, v14, gPalColorCubeRGB[0][0][gWinSys.Colors[0]]);
                v47 = (&v104->Id + v8);
                if ( Pitch >= ebx0a )
                    WinDrawTextList(win, v47, ebx0a, a2, Ypos, arg4, Color);
                else
                    WinDrawTextList(win, v47, Pitch, a2, Ypos, arg4, Color);
                v48 = gFont.ChrHeight();
                v49 = gFont.ChrHeight();
                ScrEnlight(v108 + v14 * v22 * v49, a2, v48, v14);
                if ( Pitch > ebx0a )
                {
                    ScrFillSolid((v92 + v14 * posY), v94, v94, v14, gPalColorCubeRGB[0][0][gWinSys.Colors[0]]);
                    gFont.ChrHeight();
                    v50 = gFont.ChrHeight();
                    posY = v52 + v51 + v50 + 7;
                    ScrShadowRectangle(pDst, v14, xx, posY, a5, Width + posY, gPalColorCubeRGB[0][0][gWinSys.Colors[1]], gPalColorCubeRGB[0][0][gWinSys.Colors[2]]);
                    ScrEnlight((v92 + v14 * posY), Width, Width, v14);
                }
                WinRedraw(Window, p_Geometry, 0);
            }
            else if ( v44 == -3 )
            {
                v76.lt = Ypos + p_Geometry->lt;
                v53 = v111;
                v76.rt = a2 + v76.lt;
                if ( v111 != -1 )
                {
                    v54 = gFont.ChrHeight();
                    v76.tp = v53 * v54 + arg4 + v55;
                    v76.bm = gFont.ChrHeight() + v76.tp;
                    v74 = gPalColorCubeRGB[0][0][gWinSys.Colors[0]];
                    v56 = gFont.ChrHeight();
                    v57 = gFont.ChrHeight();
                    ScrFillSolid(v108 + v14 * v58 * v57, a2, v56, v14, v74);
                    if ( BYTE1(a7) )
                        v59 = v83 | gPalColorCubeRGB[0][0][*(&gPalBase + v85 + 172)];
                    else
                        v59 = a7;
                    v75 = v59;
                    v60 = v111;
                    v61 = gFont.ChrHeight();
                    gFont.Print(v108 + v14 * v60 * v61, v62, v14, v14, v75);
                    WinRedraw(Window, &v76, 0);
                }
                if ( v22 != -1 )
                {
                    v63 = gFont.ChrHeight();
                    v76.tp = v22 * v63 + v64;
                    v76.bm = gFont.ChrHeight() + v76.tp;
                    v65 = gFont.ChrHeight();
                    v66 = gFont.ChrHeight();
                    ScrEnlight(v108 + v14 * v22 * v66, a2, v65, v14);
                    WinRedraw(Window, &v76, 0);
                }
            }
        }
        else
        {
            v45 = v44 - 512;
            if ( v45 != v22 && v45 < Pitch )
            {
                v111 = v22;
                v22 = v45;
                v44 = -3;
                goto LABEL_75;
            }
        }
    }
LABEL_90:
    WinClose(win);
    return v102;
*/
}


int TextBoxDialogEdit1( char *text, int MaxChars, char *Label, int Xpos, int Ypos )
{
    int win, w, a, h;
    char *Surface;

    if( !gWinSys.Init ) return -1;
    w = gFont.LineWidth( Label ) + 12;
    a = MaxChars + 1;
    if( w < a * gFont.MaxWidth() ) w = gFont.MaxWidth() * a;

    w += 16;
    if( w < 160 ) w = 160;

    h = 5 * gFont.ChrHeight() + 16;
    if( ( win = WinCreateWindow( Xpos, Ypos, w, h, 256, 20 ) ) == -1 ) return -1;

    WinDrawFrame( win );
    Surface = WinGetWindow( win )->Surface;
    ScrFillSolid( WIN_XY( 14, gFont.ChrHeight() + 14, w, Surface ), w - 2*14, gFont.ChrHeight() + 2, w, WIN_PALCOLOR_ALPHA );
    gFont.Print( WIN_XY( 8, 8, w, Surface ), Label, w, w, WIN_FRCOL_D );
    ScrShadowRectangle( Surface, w, 14, gFont.ChrHeight() + 14, w - 14, 2 * gFont.ChrHeight() + 16, WIN_FRCOL_B, WIN_FRCOL_A );
    WinAddButton( win, (w / 2) - 72, h - 8 - gFont.ChrHeight() - 6, -1, -1, -1, 13, "Done",  0 );
    WinAddButton( win, (w / 2) + 8,  h - 8 - gFont.ChrHeight() - 6, -1, -1, -1, 27, "Cancel", 0 );
    WinUpdate( win );
//    TextBoxDialogRun( win, text, MaxChars, 16, gFont.ChrHeight() + 16, WIN_FRCOL_C, WIN_PALCOLOR_ALPHA );
    WinClose( win );
    return 0;
}


void TextBoxUnk12( char *a1, int a2, int a3, int a4 )
{
/*
    int result; // eax
    int v7; // edx
    int v8; // esi
    int v9; // esi
    int v10; // edi
    int v11; // edx
    char *Surface; // edx
    int v13; // ebx
    int v14; // eax
    int v15; // eax
    int v16; // edx
    int Height; // [esp+4h] [ebp-14h]

    if ( !gWinSys.Init )
        return -1;
    Height = 3 * gFont.ChrHeight() + 16;
    v8 = gFont.LineWidth(a1) + 16;
    if ( v8 < 80 )
        v8 = 80;
    v9 = v8 + 16;
    result = WinCreateWindow(a2, v7, v9, Height, 256, 20);
    v10 = result;
    if ( result != -1 )
    {
        WinDrawFrame(result);
        Surface = WinGetWindow(v11)->Surface;
        if ( BYTE1(a4) )
        {
            HIWORD(v13) = HIWORD(a4);
            LOWORD(v13) = 0;
            v14 = v13 | gPalColorCubeRGB[0][0][*(&gPalBase + a4 + 43)];
        }
        else
        {
            v14 = a4;
        }
        gFont.Print(&Surface[8 * v9 + 16], a1, v9, v9, v14);
        v15 = gFont.ChrHeight();
        WinAddButton(v10, v9 / 2 - 32, v16 - v15 - 6, -1, -1, -1, 27, "Done", 0);
        WinUpdate(v10);
        while ( InpUpdate() != 27 )
            ;
        WinClose(v10);
        return 0;
    }
    return result;
*/
}





int TextBoxCreate( char **TextList, int Lines, int Xpos, int Ypos, int FrameColor, int BgColor, VidRect_t *Area )
{
    int WinId, w, h;

    h = Lines * gFont.ChrHeight() + 2*TEXTBOX_MARGIN_V;
    w = TextBoxListWidth( TextList, Lines ) + 2*TEXTBOX_MARGIN_H;
    if( h < 2 || w < 2 ) return -1;
    
    if( (WinId = WinCreateWindow( Xpos, Ypos, w, h, BgColor, 0x14 )) == -1 ) return -1;
    WinDrawTextList( WinId, TextList, Lines, w - 4, TEXTBOX_MARGIN_H, TEXTBOX_MARGIN_V, FrameColor );
    WinDrawRectangle( WinId, 0, 0, w - 1, h - 1, WIN_FRCOL_ALPHA );
    WinDrawRectangle( WinId, 1, 1, w - 2, h - 2, FrameColor );
    WinUpdate( WinId );
    WinGetRect( WinId, Area );
    
    return WinId;
}



int TextBoxUnk03( char *a1 )
{
/*
    int result; // eax
    int v2; // edi
    char *Surface; // ebp
    int v4; // eax
    int v5; // edx
    int v6; // eax
    int v7; // edx
    int v8; // eax
    int v9; // eax
    _BYTE *v10; // edx
    Window_t *Window; // eax
    int v13; // [esp+4h] [ebp-24h]
    char text[4]; // [esp+Ch] [ebp-1Ch] BYREF

    if ( !gWinSys.Init )
        return -1;
    v2 = gFont.ChrHeight();
    if ( WidgetId == (Window01_t *)-1 )
    {
        result = WinCreateWindow(80, 80, 300, 192, 256, 4);
        WidgetId = (Window01_t *)result;
        if ( result == -1 )
            return result;
        WinDrawFrame(result);
        Surface = WinGetWindow((int)WidgetId)->Surface;
        WinDrawFilledRect((int)WidgetId, 8, 8, 284, v2, 257);
        v4 = gFont.LineWidth(::text);
        WinDrawText((int)WidgetId, ::text, 0, (v5 - v4) / 2, 8, 0x2000104);
        ScrShadowRectangle(Surface, 300, 8, 8, 291, v2 + 8, (unsigned __int8)gPalColorCubeRGB[0][0][gWinSys.Colors[2]], (unsigned __int8)gPalColorCubeRGB[0][0][gWinSys.Colors[1]]);
        WinDrawFilledRect((int)WidgetId, 9, 26, 282, 135, 257);
        ScrShadowRectangle(Surface, 300, 8, 25, 291, v2 + 145, (unsigned __int8)gPalColorCubeRGB[0][0][gWinSys.Colors[2]], (unsigned __int8)gPalColorCubeRGB[0][0][gWinSys.Colors[1]]);
        Xpos = 9;
        statUnk01 = 26;
        v6 = gFont.LineWidth("Close");
        v8 = WinAddButton((int)WidgetId, (v7 - v6) / 2, 184 - v2 - 6, -1, -1, -1, -1, "Close", 0);
        WinSetButtonHandler(v8, 0, 0, 0, TextBoxUnk04);
        WinCreateButton((int)WidgetId, 8, 8, 284, v2, -1, -1, -1, -1, 0, 0, 0, 16);
    }
    text[1] = 0;
    if ( *a1 )
    {
        do
        {
            v9 = gFont.ChrWidth(*a1);
            v13 = v9;
            if ( *v10 == 10 || Xpos + v9 > 291 )
            {
                Xpos = 9;
                statUnk01 += v2;
            }
            while ( 160 - statUnk01 < v2 )
            {
                Window = WinGetWindow((int)WidgetId);
                ScrCopy(&Window->Surface[300 * v2 + 7809], 282, 134 - v2 - 1, 300, Window->Surface + 7809, 300);
                statUnk01 -= v2;
                WinDrawFilledRect((int)WidgetId, 9, statUnk01, 282, v2, 257);
            }
            if ( *a1 != 10 )
            {
                text[0] = *a1;
                WinDrawText((int)WidgetId, text, 0, Xpos, statUnk01, 0x2000104);
                Xpos += v13 + gFont.Distance();
            }
        }
        while ( *++a1 );
    }
    WinUpdate((int)WidgetId);
*/
    return 0;
}


int TextBoxSearchLine( int chr, char **TextList, int Lines )
{
    int i, s;

    if( chr >= 'A' && chr <= 'Z' ) chr +=' ';
    i = 0;
    for( i = 0; i< Lines; i++, TextList++ ){
        s = **TextList;
        if( s == chr || s == chr - ' ' ) return i;
    }
    return -1;        
}

int TextBoxListWidth( char **TextList, int Lines )
{
    int max, w, i;

    max = 0;
    for( i = 0; i < Lines; i++, TextList++ ){
        w = gFont.LineWidth( *TextList );
        if( w > max ) max = w;        
    }
    return max;
}
