
unsigned char *MovDecodeNibbleFrame16( unsigned char *pdst, unsigned char op, unsigned char *pData, short Width, short Height );

// map, pdata, xpos, ypos, width, height 
void MovDecodeFrame16bpp( char *pMap, char *pData, short Xpos, short Ypos, short Width, short Height )
{
/*
    int i;
    char *pdst, n;

    gMovMovieXpos = 16 * Xpos;
    gMovUnk02 = 16 * Width;
    gMovMovieYpos = gMovScrSize * 8 * Ypos;
    gMovUnk20 = gMovScrSize * 8 * Height;
    pdst = gMovScrPixelsA;
    if( Xpos || Ypos ) pdst = &gMovScrPixelsA[ gMovMovieXpos + gMovSurfWidth * gMovMovieYpos ];

//    pFrame1 = pdst;
    for( ;Height; Height-- ){
	for( i = Width / 2; --i >= 0; ){
//    	    pFrame1 += gMovScrBufSizeA - 16 * Width;
	    n = *pMap++;
    	    pData = MovDecodeNibbleFrame16( pdst, n & 0x0f, pData, Width, Height ); pdst += 8; n >>= 4;
    	    pData = MovDecodeNibbleFrame16( pdst, n & 0x0f, pData, Width, Height ); pdst += 8;
	}
    }
*/
}

unsigned char *MovDecodeNibbleFrame16( unsigned char *pdst, unsigned char op, unsigned char *pData, short Width, short Height )
{
/*
    MveXYofs_t xy_offs, tmp1;
    char *pdst,*pSrc,*pFrame1,*v109,*v110,*v111,*v97,*v98,*v99,*v100,*v101,*v102,*v103,*v131,*v132,*v133,*v152,*v153,*v154,*v155,*v156;
    char *v157,*v158,*v159,*v160,*v161,*v186,*v187,*v188,*v189,*v190,*v191,*v192,*v193,*v194,*v195,*v219,*v220,*v221,*v222,*v223,*v224;
    char *v225,*v236,*v237,*v238,*v227,*v228,*v229,*v230,*v231,*v232,*v233,*v242,*v243,*v244,*v247,*v248,*v249,*v250,*v251,*v252,*v253;
    short *Cstr, v41;
    unsigned int v27,v28,v29,v30,v31,v32,v33,v34,*v36,v37,v38,v39,v40,v42,v43,v44,v45,v46,v47,v48,v49,*v51,v55,v56,v57,v58,v59,v60;
    unsigned int v61,v62,v63,*v65,v67,v68,v69,v70,v71,v72,v73,v74,*v76,v78,v79,v80,v81,v82,v83,v84,v85,v86,v87,v88,v89,v90,v91,v92;
    unsigned int v93,v95,v104,v105,v106,v107,v112,v113,v114,v115,v116,v117,v118,v119,*v120,v121,v122,v123,v124,v125,v126,v127,v129;
    unsigned int v134,v135,v136,v137,v138,v139,v140,v141,v142,v143,v144,v145,v146,v147,v148,v167,v168,v169,v170,v171,v172,v173,v174;
    unsigned int v175,v176,v177,v178,v179,v180,v181,v182,v201,v202,v203,v204,v205,v206,v207,v208,v209,v210,v211,v212,v213,v214,v215;
    unsigned int v150,v184,v217;
    unsigned char *Bstr;
    int v108,IdxXY,PitchA,*pDst,v35,v50,v52,v53,v54,v64,v66,v75,v77,v94,v96,v128,v130,v149,v151,*v162,*v163,*v164,*v165,*v166,v183; 
    int v185,*v196,*v197,*v198,*v199,*v200,v216,v218,v226,v234,v235,v239,v240,v241,v245,v246;
*/

    switch( op ){
        case 0:
//            IdxXY = gMovScrPixelsB - gMovScrPixelsA;
//            goto CopyBlock;
        case 1:
        case 0xF:
            return NULL;
        case 2:
//            xy_offs = gMovEncXYoffst[*Bstr];
//            goto LABEL_11;
        case 3:
//            tmp1 = gMovEncXYoffst[*Bstr];
//            xy_offs.x = -tmp1.x;
//            xy_offs.y = -tmp1.y;
//            goto LABEL_11;
        case 4:
//            xy_offs = gMovEncXYOffstCur[*Bstr];
//            goto LABEL_15;
        case 5:
//            xy_offs = (MveXYofs_t)*Cstr;
//LABEL_15:
//            IdxXY = gMovScrPixelsB - gMovScrPixelsA + gMovEncPosY[(unsigned char)xy_offs.y] + 2 * xy_offs.x;
//            goto CopyBlock;
        case 6:
//            xy_offs = (MveXYofs_t)*Cstr;
//LABEL_11:
//            IdxXY = gMovEncPosY[(unsigned char)xy_offs.y] + 2 * xy_offs.x;
//CopyBlock:
/*
            PitchA = gMovSurfWidth; *(int *)pFrame1 = *(int *)&pFrame1[IdxXY]; *((int *)pFrame1 + 1) = *(int *)&pFrame1[IdxXY + 4]; *((int *)pFrame1 + 2) = *(int *)&pFrame1[IdxXY + 8]; *((int *)pFrame1 + 3) = *(int *)&pFrame1[IdxXY + 12]; pSrc = &pFrame1[IdxXY + PitchA];
            pDst = (int *)&pFrame1[PitchA]; pDst[0] = *(int *)pSrc; pDst[1] = *((int *)pSrc + 1); pDst[2] = *((int *)pSrc + 2); pDst[3] = *((int *)pSrc + 3); pSrc += PitchA;
            pDst = (int *)((char *)pDst + PitchA); pDst[0] = *(int *)pSrc; pDst[1] = *((int *)pSrc + 1); pDst[2] = *((int *)pSrc + 2); pDst[3] = *((int *)pSrc + 3); pSrc += PitchA;
            pDst = (int *)((char *)pDst + PitchA); pDst[0] = *(int *)pSrc; pDst[1] = *((int *)pSrc + 1); pDst[2] = *((int *)pSrc + 2); pDst[3] = *((int *)pSrc + 3); pSrc += PitchA;
            pDst = (int *)((char *)pDst + PitchA); pDst[0] = *(int *)pSrc; pDst[1] = *((int *)pSrc + 1); pDst[2] = *((int *)pSrc + 2); pDst[3] = *((int *)pSrc + 3); pSrc += PitchA;
            pDst = (int *)((char *)pDst + PitchA); pDst[0] = *(int *)pSrc; pDst[1] = *((int *)pSrc + 1); pDst[2] = *((int *)pSrc + 2); pDst[3] = *((int *)pSrc + 3); pSrc += PitchA;
            pDst = (int *)((char *)pDst + PitchA); pDst[0] = *(int *)pSrc; pDst[1] = *((int *)pSrc + 1); pDst[2] = *((int *)pSrc + 2); pDst[3] = *((int *)pSrc + 3); pSrc += PitchA;
            pDst = (int *)((char *)pDst + PitchA); pDst[0] = *(int *)pSrc; pDst[1] = *((int *)pSrc + 1); pDst[2] = *((int *)pSrc + 2); pDst[3] = *((int *)pSrc + 3);
*/
            break;
        case 7:
/*
            if( *Cstr < 0 ){
                v37 = gMovEnc01[pData[6] & 0xF]; *(&loc_4FA350 + 1) = v37; *((char *)&loc_4FA350 + 8) = BYTE1(v37); v37 >>= 16; *((char *)&loc_4FA350 + 17) = v37; *((char *)&loc_4FA350 + 26) = BYTE1(v37);
                v38 = gMovEnc01[(unsigned char)pData[6] >> 4]; *((char *)&loc_4FA350 + 38) = v38; *((char *)&loc_4FA350 + 45) = BYTE1(v38); v38 >>= 16; *((char *)&loc_4FA350 + 54) = v38; *((char *)&loc_4FA350 + 63) = BYTE1(v38);
                v39 = gMovEnc01[pData[7] & 0xF]; *((char *)&loc_4FA350 + 75) = v39; *((char *)&loc_4FA350 + 82) = BYTE1(v39); v39 >>= 16; *((char *)&loc_4FA350 + 91) = v39; *((char *)&loc_4FA350 + 100) = BYTE1(v39);
                v40 = gMovEnc01[(unsigned char)pData[7] >> 4]; *((char *)&loc_4FA350 + 112) = v40; *((char *)&loc_4FA350 + 119) = BYTE1(v40); v40 >>= 16; *((char *)&loc_4FA350 + 128) = v40; *((char *)&loc_4FA350 + 137) = BYTE1(v40);
                *(int *)&v41 = __ROR4__(*(int *)Cstr & 0x7FFF7FFF, 16);
                v41 = *Cstr & 0x7FFF;
                MovProtUnk25(v41, gMovSurfWidth, *(int *)&v41, pFrame1);
            } else {
                v27 = gMovUnk21[(unsigned char)pData[6]];  *(&loc_4FpData54 + 1) = v27; *((char *)&loc_4FpData54 + 4) = BYTE1(v27); v27 >>= 16; *((char *)&loc_4FpData54 + 7) = v27; *((char *)&loc_4FpData54 + 10) = BYTE1(v27);
                v28 = gMovUnk21[(unsigned char)pData[7]];  *((char *)&loc_4FpData54 + 15) = v28; *((char *)&loc_4FpData54 + 18) = BYTE1(v28); v28 >>= 16; *((char *)&loc_4FpData54 + 21) = v28; *((char *)&loc_4FpData54 + 24) = BYTE1(v28);
                v29 = gMovUnk21[(unsigned char)pData[8]];  *((char *)&loc_4FpData54 + 29) = v29; *((char *)&loc_4FpData54 + 32) = BYTE1(v29); v29 >>= 16; *((char *)&loc_4FpData54 + 35) = v29; *((char *)&loc_4FpData54 + 38) = BYTE1(v29);
                v30 = gMovUnk21[(unsigned char)pData[9]];  *((char *)&loc_4FpData54 + 43) = v30; *((char *)&loc_4FpData54 + 46) = BYTE1(v30); v30 >>= 16; *((char *)&loc_4FpData54 + 49) = v30; *((char *)&loc_4FpData54 + 52) = BYTE1(v30);
                v31 = gMovUnk21[(unsigned char)pData[10]]; *((char *)&loc_4FpData54 + 57) = v31; *((char *)&loc_4FpData54 + 60) = BYTE1(v31); v31 >>= 16; *((char *)&loc_4FpData54 + 63) = v31; *((char *)&loc_4FpData54 + 66) = BYTE1(v31);
                v32 = gMovUnk21[(unsigned char)pData[11]]; *((char *)&loc_4FpData54 + 71) = v32; *((char *)&loc_4FpData54 + 74) = BYTE1(v32); v32 >>= 16; *((char *)&loc_4FpData54 + 77) = v32; *((char *)&loc_4FpData54 + 80) = BYTE1(v32);
                v33 = gMovUnk21[(unsigned char)pData[12]]; *((char *)&loc_4FpData54 + 85) = v33; *((char *)&loc_4FpData54 + 88) = BYTE1(v33); v33 >>= 16; *((char *)&loc_4FpData54 + 91) = v33; *((char *)&loc_4FpData54 + 94) = BYTE1(v33);
                v34 = gMovUnk21[(unsigned char)pData[13]]; *((char *)&loc_4FpData54 + 99) = v34; *((char *)&loc_4FpData54 + 102) = BYTE1(v34); v34 >>= 16; *((char *)&loc_4FpData54 + 105) = v34; *((char *)&loc_4FpData54 + 108) = BYTE1(v34);
                v35 = __ROR4__(*(int *)Cstr, 16);
                HIWORD(v34) = HIWORD(v35); HIWORD(v36) = HIWORD(*(int *)Cstr); LOWORD(v34) = *(int *)Cstr; LOWORD(v36) = v35;
                v36[0] = v34; v36[1] = v34; v36[2] = v34; v36[3] = v34;
                v36[0] = v34; v36[1] = v34; v36[2] = v34; v36[3] = v34;
                v36[0] = v34; v36[1] = v34; v36[2] = v34; v36[3] = v34;
                v36[0] = v34; v36[1] = v34; v36[2] = v34; v36[3] = v34;
                v36[0] = v34; v36[1] = v34; v36[2] = v34; v36[3] = v34;
                v36[0] = v34; v36[1] = v34; v36[2] = v34; v36[3] = v34;
                v36[0] = v34; v36[1] = v34; v36[2] = v34; v36[3] = v34;
                v36[0] = v34; v36[1] = v34; v36[2] = v34; v36[3] = v34;
            }
*/
            break;
        case 8:
/*
            if( *Cstr < 0 ){
                v55 = gMovUnk21[(unsigned char)pData[6]];
                if ( *((short *)pData + 5) < 0 ){
                    *(&loc_4FA80C + 1) = gMovUnk21[(unsigned char)pData[6]]; *((char *)&loc_4FA80C + 4) = BYTE1(v55); v67 = HIWORD(v55); *((char *)&loc_4FA80C + 7) = v67; *((char *)&loc_4FA80C + 10) = BYTE1(v67);
                    v68 = gMovUnk21[(unsigned char)pData[7]];  *((char *)&loc_4FA80C + 15) = v68; *((char *)&loc_4FA80C + 18) = BYTE1(v68); v68 >>= 16; *((char *)&loc_4FA80C + 21) = v68; *((char *)&loc_4FA80C + 24) = BYTE1(v68);
                    v69 = gMovUnk21[(unsigned char)pData[8]];  *((char *)&loc_4FA80C + 29) = v69; *((char *)&loc_4FA80C + 32) = BYTE1(v69); v69 >>= 16; *((char *)&loc_4FA80C + 35) = v69; *((char *)&loc_4FA80C + 38) = BYTE1(v69);
                    v70 = gMovUnk21[(unsigned char)pData[9]];  *((char *)&loc_4FA80C + 43) = v70; *((char *)&loc_4FA80C + 46) = BYTE1(v70); v70 >>= 16; *((char *)&loc_4FA80C + 49) = v70; *((char *)&loc_4FA80C + 52) = BYTE1(v70);
                    v71 = gMovUnk21[(unsigned char)pData[14]]; *((char *)&loc_4FA80C + 84) = v71; *((char *)&loc_4FA80C + 87) = BYTE1(v71); v71 >>= 16; *((char *)&loc_4FA80C + 90) = v71; *((char *)&loc_4FA80C + 93) = BYTE1(v71);
                    v72 = gMovUnk21[(unsigned char)pData[15]]; *((char *)&loc_4FA80C + 98) = v72; *((char *)&loc_4FA80C + 101) = BYTE1(v72); v72 >>= 16; *((char *)&loc_4FA80C + 104) = v72; *((char *)&loc_4FA80C + 107) = BYTE1(v72);
                    v73 = gMovUnk21[(unsigned char)pData[16]]; *((char *)&loc_4FA80C + 112) = v73; *((char *)&loc_4FA80C + 115) = BYTE1(v73); v73 >>= 16; *((char *)&loc_4FA80C + 118) = v73; *((char *)&loc_4FA80C + 121) = BYTE1(v73);
                    v74 = gMovUnk21[(unsigned char)pData[17]]; *((char *)&loc_4FA80C + 126) = v74; *((char *)&loc_4FA80C + 129) = BYTE1(v74); v74 >>= 16; *((char *)&loc_4FA80C + 132) = v74; *((char *)&loc_4FA80C + 135) = BYTE1(v74);
                    v75 = __ROR4__(*(int *)Cstr & 0x7FFF7FFF, 16);
                    HIWORD(v74) = HIWORD(v75);
                    HIWORD(v76) = (*(int *)Cstr & 0x7FFF7FFFu) >> 16;
                    LOWORD(v74) = *Cstr & 0x7FFF;
                    LOWORD(v76) = v75;
                    v76[0] = v74; v76[1] = v74; v76[2] = v74; v76[3] = v74;
                    v76[0] = v74; v76[1] = v74; v76[2] = v74; v76[3] = v74;
                    v76[0] = v74; v76[1] = v74; v76[2] = v74; v76[3] = v74;
                    v76[0] = v74; v76[1] = v74; v76[2] = v74; v76[3] = v74;
                    v77 = __ROR4__(*(int *)(pData + 10) & 0x7FFF7FFF, 16);
                    HIWORD(v74) = HIWORD(v77); HIWORD(v76) = (*(int *)(pData + 10) & 0x7FFF7FFFu) >> 16; LOWORD(v74) = *((_WORD *)pData + 5) & 0x7FFF; LOWORD(v76) = v77;
                    v76[0] = v74; v76[1] = v74; v76[2] = v74; v76[3] = v74;
                    v76[0] = v74; v76[1] = v74; v76[2] = v74; v76[3] = v74;
                    v76[0] = v74; v76[1] = v74; v76[2] = v74; v76[3] = v74;
                    v76[0] = v74; v76[1] = v74; v76[2] = v74; v76[3] = v74;
                } else {
                    *(&loc_4FA688 + 1) = gMovUnk21[(unsigned char)pData[6]]; *((char *)&loc_4FA688 + 4) = BYTE1(v55);
                    v56 = HIWORD(v55); *((char *)&loc_4FA688 + 9) = v56; *((char *)&loc_4FA688 + 12) = BYTE1(v56);
                    v57 = gMovUnk21[(unsigned char)pData[7]]; *((char *)&loc_4FA688 + 17) = v57; *((char *)&loc_4FA688 + 20) = BYTE1(v57); v57 >>= 16; *((char *)&loc_4FA688 + 25) = v57; *((char *)&loc_4FA688 + 28) = BYTE1(v57);
                    v58 = gMovUnk21[(unsigned char)pData[8]]; *((char *)&loc_4FA688 + 33) = v58; *((char *)&loc_4FA688 + 36) = BYTE1(v58); v58 >>= 16; *((char *)&loc_4FA688 + 41) = v58; *((char *)&loc_4FA688 + 44) = BYTE1(v58);
                    v59 = gMovUnk21[(unsigned char)pData[9]]; *((char *)&loc_4FA688 + 49) = v59; *((char *)&loc_4FA688 + 52) = BYTE1(v59); v59 >>= 16; *((char *)&loc_4FA688 + 57) = v59; *((char *)&loc_4FA688 + 60) = BYTE1(v59);
                    v60 = gMovUnk21[(unsigned char)pData[14]]; *((char *)&loc_4FA688 + 95) = v60; *((char *)&loc_4FA688 + 98) = BYTE1(v60); v60 >>= 16; *((char *)&loc_4FA688 + 103) = v60;*((char *)&loc_4FA688 + 106) = BYTE1(v60);
                    v61 = gMovUnk21[(unsigned char)pData[15]]; *((char *)&loc_4FA688 + 111) = v61; *((char *)&loc_4FA688 + 114) = BYTE1(v61); v61 >>= 16; *((char *)&loc_4FA688 + 119) = v61; *((char *)&loc_4FA688 + 122) = BYTE1(v61);
                    v62 = gMovUnk21[(unsigned char)pData[16]]; *((char *)&loc_4FA688 + 127) = v62; *((char *)&loc_4FA688 + 130) = BYTE1(v62); v62 >>= 16; *((char *)&loc_4FA688 + 135) = v62; *((char *)&loc_4FA688 + 138) = BYTE1(v62);
                    v63 = gMovUnk21[(unsigned char)pData[17]]; *((char *)&loc_4FA688 + 143) = v63; *((char *)&loc_4FA688 + 146) = BYTE1(v63); v63 >>= 16; *((char *)&loc_4FA688 + 151) = v63; *((char *)&loc_4FA688 + 154) = BYTE1(v63);
                    v64 = __ROR4__(*(int *)Cstr & 0x7FFF7FFF, 16); HIWORD(v63) = HIWORD(v64); HIWORD(v65) = (*(int *)Cstr & 0x7FFF7FFFu) >> 16; LOWORD(v63) = *Cstr & 0x7FFF; LOWORD(v65) = v64;
                    *v65 = v63; v65[1] = v63; *v65 = v63; v65[1] = v63; *v65 = v63; v65[1] = v63; *v65 = v63; v65[1] = v63; *v65 = v63; v65[1] = v63; *v65 = v63; v65[1] = v63; *v65 = v63; v65[1] = v63; *v65 = v63; v65[1] = v63;
                    v66 = __ROR4__(*(int *)(pData + 10), 16); HIWORD(v63) = HIWORD(v66); HIWORD(v65) = HIWORD(*(int *)(pData + 10)); LOWORD(v63) = *(int *)(pData + 10); LOWORD(v65) = v66;
                    *v65 = v63; v65[1] = v63; *v65 = v63; v65[1] = v63; *v65 = v63; v65[1] = v63; *v65 = v63; v65[1] = v63; *v65 = v63; v65[1] = v63; *v65 = v63; v65[1] = v63; *v65 = v63; v65[1] = v63; *v65 = v63; v65[1] = v63;
                }
            } else {
                v42 = gMovUnk21[(unsigned char)pData[6]];  *(&loc_4FA4CC + 1) = v42; *((char *)&loc_4FA4CC + 4) = BYTE1(v42); v42 >>= 16; *((char *)&loc_4FA4CC + 9) = v42; *((char *)&loc_4FA4CC + 12) = BYTE1(v42);
                v43 = gMovUnk21[(unsigned char)pData[7]];  *((char *)&loc_4FA4CC + 17) = v43; *((char *)&loc_4FA4CC + 20) = BYTE1(v43); v43 >>= 16; *((char *)&loc_4FA4CC + 25) = v43; *((char *)&loc_4FA4CC + 28) = BYTE1(v43);
                v44 = gMovUnk21[(unsigned char)pData[12]]; *((char *)&loc_4FA4CC + 54) = v44; *((char *)&loc_4FA4CC + 57) = BYTE1(v44); v44 >>= 16; *((char *)&loc_4FA4CC + 62) = v44; *((char *)&loc_4FA4CC + 65) = BYTE1(v44);
                v45 = gMovUnk21[(unsigned char)pData[13]]; *((char *)&loc_4FA4CC + 70) = v45; *((char *)&loc_4FA4CC + 73) = BYTE1(v45); v45 >>= 16; *((char *)&loc_4FA4CC + 78) = v45; *((char *)&loc_4FA4CC + 81) = BYTE1(v45);
                v46 = gMovUnk21[(unsigned char)pData[18]]; *((char *)&loc_4FA4CC + 116) = v46; *((char *)&loc_4FA4CC + 119) = BYTE1(v46); v46 >>= 16; *((char *)&loc_4FA4CC + 124) = v46; *((char *)&loc_4FA4CC + 127) = BYTE1(v46);
                v47 = gMovUnk21[(unsigned char)pData[19]]; *((char *)&loc_4FA4CC + 132) = v47; *((char *)&loc_4FA4CC + 135) = BYTE1(v47); v47 >>= 16; *((char *)&loc_4FA4CC + 140) = v47; *((char *)&loc_4FA4CC + 143) = BYTE1(v47);
                v48 = gMovUnk21[(unsigned char)pData[24]]; *((char *)&loc_4FA4CC + 169) = v48; *((char *)&loc_4FA4CC + 172) = BYTE1(v48); v48 >>= 16; *((char *)&loc_4FA4CC + 177) = v48; *((char *)&loc_4FA4CC + 180) = BYTE1(v48);
                v49 = gMovUnk21[(unsigned char)pData[25]]; *((char *)&loc_4FA4CC + 185) = v49; *((char *)&loc_4FA4CC + 188) = BYTE1(v49);  v49 >>= 16; *((char *)&loc_4FA4CC + 193) = v49; *((char *)&loc_4FA4CC + 196) = BYTE1(v49);                
                v50 = __ROR4__(*(int *)Cstr, 16); HIWORD(v49) = HIWORD(v50); HIWORD(v51) = HIWORD(*(int *)Cstr); LOWORD(v49) = *(int *)Cstr; LOWORD(v51) = v50; *v51 = v49; v51[1] = v49; *v51 = v49; v51[1] = v49; *v51 = v49; v51[1] = v49; *v51 = v49; v51[1] = v49;
                v52 = __ROR4__(*((int *)pData + 2), 16); HIWORD(v49) = HIWORD(v52); HIWORD(v51) = HIWORD(*((int *)pData + 2)); LOWORD(v49) = *((int *)pData + 2); LOWORD(v51) = v52; *v51 = v49; v51[1] = v49; *v51 = v49; v51[1] = v49; *v51 = v49; v51[1] = v49; *v51 = v49; v51[1] = v49;
                v53 = __ROR4__(*(int *)(pData + 14), 16); HIWORD(v49) = HIWORD(v53); HIWORD(v51) = HIWORD(*(int *)(pData + 14)); LOWORD(v49) = *(int *)(pData + 14); LOWORD(v51) = v53; *v51 = v49; v51[1] = v49; *v51 = v49; v51[1] = v49; *v51 = v49; v51[1] = v49; *v51 = v49; v51[1] = v49;
                v54 = __ROR4__(*((int *)pData + 5), 16); HIWORD(v49) = HIWORD(v54); HIWORD(v51) = HIWORD(*((int *)pData + 5)); LOWORD(v49) = *((int *)pData + 5); LOWORD(v51) = v54; *v51 = v49; v51[1] = v49; *v51 = v49; v51[1] = v49; *v51 = v49; v51[1] = v49; *v51 = v49; v51[1] = v49;
            }
*/
            break;
        case 9:
/*
            if( *Cstr < 0 ){
                if( *((short *)pData + 3) < 0 ){
                    v121 = gMovUnk22[(unsigned char)pData[10]]; *(&loc_4FAFB0 + 2) = BYTE1(v121); *((char *)&loc_4FAFB0 + 8) = v121; v121 >>= 16; *((char *)&loc_4FAFB0 + 16) = BYTE1(v121); *((char *)&loc_4FAFB0 + 22) = v121;
                    v122 = gMovUnk22[(unsigned char)pData[11]]; *((char *)&loc_4FAFB0 + 32) = BYTE1(v122); *((char *)&loc_4FAFB0 + 38) = v122; v122 >>= 16; *((char *)&loc_4FAFB0 + 48) = BYTE1(v122); *((char *)&loc_4FAFB0 + 54) = v122;
                    v123 = gMovUnk22[(unsigned char)pData[12]]; *((char *)&loc_4FAFB0 + 67) = BYTE1(v123); *((char *)&loc_4FAFB0 + 73) = v123; v123 >>= 16; *((char *)&loc_4FAFB0 + 81) = BYTE1(v123); *((char *)&loc_4FAFB0 + 87) = v123;
                    v124 = gMovUnk22[(unsigned char)pData[13]]; *((char *)&loc_4FAFB0 + 97) = BYTE1(v124); *((char *)&loc_4FAFB0 + 103) = v124; v124 >>= 16; *((char *)&loc_4FAFB0 + 113) = BYTE1(v124); *((char *)&loc_4FAFB0 + 119) = v124;
                    v125 = gMovUnk22[(unsigned char)pData[14]]; *((char *)&loc_4FAFB0 + 132) = BYTE1(v125); *((char *)&loc_4FAFB0 + 138) = v125; v125 >>= 16; *((char *)&loc_4FAFB0 + 146) = BYTE1(v125); *((char *)&loc_4FAFB0 + 152) = v125;
                    v126 = gMovUnk22[(unsigned char)pData[15]]; *((char *)&loc_4FAFB0 + 162) = BYTE1(v126); *((char *)&loc_4FAFB0 + 168) = v126; v126 >>= 16; *((char *)&loc_4FAFB0 + 178) = BYTE1(v126); *((char *)&loc_4FAFB0 + 184) = v126;
                    v127 = gMovUnk22[(unsigned char)pData[16]]; *((char *)&loc_4FAFB0 + 197) = BYTE1(v127); *((char *)&loc_4FAFB0 + 203) = v127; v127 >>= 16; *((char *)&loc_4FAFB0 + 211) = BYTE1(v127); *((char *)&loc_4FAFB0 + 217) = v127;
                    v128 = (unsigned char)pData[17];
                    v129 = gMovUnk22[v128]; *((char *)&loc_4FAFB0 + 227) = BYTE1(v129); *((char *)&loc_4FAFB0 + 233) = v129; v129 >>= 16; *((char *)&loc_4FAFB0 + 243) = BYTE1(v129); *((char *)&loc_4FAFB0 + 249) = v129;
                    LOWORD(v129) = *Cstr & 0x7FFF;
                    v130 = gMovSurfWidth;
                    LOWORD(v128) = v129; v128 <<= 16; LOWORD(v128) = v129; *(int *)pFrame1 = v128; *(int *)&pFrame1[v130] = v128;
                    LOWORD(v128) = v129; v128 <<= 16; LOWORD(v128) = v129; *((int *)pFrame1 + 1) = v128; *(int *)&pFrame1[v130 + 4] = v128;
                    LOWORD(v128) = v129; v128 <<= 16; LOWORD(v128) = v129; *((int *)pFrame1 + 2) = v128; *(int *)&pFrame1[v130 + 8] = v128;
                    LOWORD(v128) = v129; v128 <<= 16; LOWORD(v128) = v129; *((int *)pFrame1 + 3) = v128; *(int *)&pFrame1[v130 + 12] = v128; v131 = &pFrame1[2 * v130];
                    LOWORD(v128) = v129; v128 <<= 16; LOWORD(v128) = v129; *(int *)v131 = v128; *(int *)&v131[v130] = v128;
                    LOWORD(v128) = v129; v128 <<= 16; LOWORD(v128) = v129; *((int *)v131 + 1) = v128; *(int *)&v131[v130 + 4] = v128;
                    LOWORD(v128) = v129; v128 <<= 16; LOWORD(v128) = v129; *((int *)v131 + 2) = v128; *(int *)&v131[v130 + 8] = v128;
                    LOWORD(v128) = v129; v128 <<= 16; LOWORD(v128) = v129; *((int *)v131 + 3) = v128; *(int *)&v131[v130 + 12] = v128; v132 = &v131[2 * v130];
                    LOWORD(v128) = v129; v128 <<= 16; LOWORD(v128) = v129; *(int *)v132 = v128; *(int *)&v132[v130] = v128;
                    LOWORD(v128) = v129; v128 <<= 16; LOWORD(v128) = v129; *((int *)v132 + 1) = v128; *(int *)&v132[v130 + 4] = v128;
                    LOWORD(v128) = v129; v128 <<= 16; LOWORD(v128) = v129; *((int *)v132 + 2) = v128; *(int *)&v132[v130 + 8] = v128;
                    LOWORD(v128) = v129; v128 <<= 16; LOWORD(v128) = v129; *((int *)v132 + 3) = v128; *(int *)&v132[v130 + 12] = v128; v133 = &v132[2 * v130];
                    LOWORD(v128) = v129; v128 <<= 16; LOWORD(v128) = v129; *(int *)v133 = v128; *(int *)&v133[v130] = v128;
                    LOWORD(v128) = v129; v128 <<= 16; LOWORD(v128) = v129; *((int *)v133 + 1) = v128; *(int *)&v133[v130 + 4] = v128;
                    LOWORD(v128) = v129; v128 <<= 16; LOWORD(v128) = v129; *((int *)v133 + 2) = v128; *(int *)&v133[v130 + 8] = v128;
                    LOWORD(v128) = v129; v128 <<= 16; LOWORD(v128) = v129; *((int *)v133 + 3) = v128; *(int *)&v133[v130 + 12] = v128;
                } else {
                    v112 = gMovUnk21[(unsigned char)pData[10]]; *(&loc_4FAE30 + 1) = v112; *((char *)&loc_4FAE30 + 4) = BYTE1(v112); v112 >>= 16; *((char *)&loc_4FAE30 + 7) = v112; *((char *)&loc_4FAE30 + 10) = BYTE1(v112);
                    v113 = gMovUnk21[(unsigned char)pData[11]]; *((char *)&loc_4FAE30 + 15) = v113; *((char *)&loc_4FAE30 + 18) = BYTE1(v113); v113 >>= 16; *((char *)&loc_4FAE30 + 21) = v113; *((char *)&loc_4FAE30 + 24) = BYTE1(v113);
                    v114 = gMovUnk21[(unsigned char)pData[12]]; *((char *)&loc_4FAE30 + 29) = v114; *((char *)&loc_4FAE30 + 32) = BYTE1(v114); v114 >>= 16; *((char *)&loc_4FAE30 + 35) = v114; *((char *)&loc_4FAE30 + 38) = BYTE1(v114);
                    v115 = gMovUnk21[(unsigned char)pData[13]]; *((char *)&loc_4FAE30 + 43) = v115; *((char *)&loc_4FAE30 + 46) = BYTE1(v115); v115 >>= 16; *((char *)&loc_4FAE30 + 49) = v115; *((char *)&loc_4FAE30 + 52) = BYTE1(v115);
                    v116 = gMovUnk21[(unsigned char)pData[14]]; *((char *)&loc_4FAE30 + 57) = v116; *((char *)&loc_4FAE30 + 60) = BYTE1(v116); v116 >>= 16; *((char *)&loc_4FAE30 + 63) = v116; *((char *)&loc_4FAE30 + 66) = BYTE1(v116);
                    v117 = gMovUnk21[(unsigned char)pData[15]]; *((char *)&loc_4FAE30 + 71) = v117; *((char *)&loc_4FAE30 + 74) = BYTE1(v117); v117 >>= 16; *((char *)&loc_4FAE30 + 77) = v117; *((char *)&loc_4FAE30 + 80) = BYTE1(v117);
                    v118 = gMovUnk21[(unsigned char)pData[16]]; *((char *)&loc_4FAE30 + 85) = v118; *((char *)&loc_4FAE30 + 88) = BYTE1(v118); v118 >>= 16; *((char *)&loc_4FAE30 + 91) = v118; *((char *)&loc_4FAE30 + 94) = BYTE1(v118);
                    v119 = gMovUnk21[(unsigned char)pData[17]]; *((char *)&loc_4FAE30 + 99) = v119; *((char *)&loc_4FAE30 + 102) = BYTE1(v119); v119 >>= 16; *((char *)&loc_4FAE30 + 105) = v119; *((char *)&loc_4FAE30 + 108) = BYTE1(v119);
                    HIWORD(v119) = *Cstr & 0x7FFF;
                    LOWORD(v119) = HIWORD(v119);
                    HIWORD(v120) = *((_WORD *)pData + 4);
                    LOWORD(v120) = HIWORD(v120);
                    v120[0] = v119; v120[1] = v119; v120[2] = v119; v120[3] = v119;
                    v120[0] = v119; v120[1] = v119; v120[2] = v119; v120[3] = v119;
                    v120[0] = v119; v120[1] = v119; v120[2] = v119; v120[3] = v119;
                    v120[0] = v119; v120[1] = v119; v120[2] = v119; v120[3] = v119;
                    v120[0] = v119; v120[1] = v119; v120[2] = v119; v120[3] = v119;
                    v120[0] = v119; v120[1] = v119; v120[2] = v119; v120[3] = v119;
                    v120[0] = v119; v120[1] = v119; v120[2] = v119; v120[3] = v119;
                    v120[0] = v119; v120[1] = v119; v120[2] = v119; v120[3] = v119;
                }
            } else {
                v78 = gMovUnk22[(unsigned char)pData[10]];
                if( *((short *)pData + 3) < 0 ){
                    *(&loc_4FAC98 + 1) = gMovUnk22[(unsigned char)pData[10]]; *((char *)&loc_4FAC98 + 8) = BYTE1(v78);
                    v104 = HIWORD(v78); *((char *)&loc_4FAC98 + 17) = v104; *((char *)&loc_4FAC98 + 26) = BYTE1(v104);
                    v105 = gMovUnk22[(unsigned char)pData[11]]; *((char *)&loc_4FAC98 + 38) = v105; *((char *)&loc_4FAC98 + 45) = BYTE1(v105); v105 >>= 16; *((char *)&loc_4FAC98 + 54) = v105; *((char *)&loc_4FAC98 + 63) = BYTE1(v105);
                    v106 = gMovUnk22[(unsigned char)pData[12]]; *((char *)&loc_4FAC98 + 75) = v106; *((char *)&loc_4FAC98 + 82) = BYTE1(v106); v106 >>= 16; *((char *)&loc_4FAC98 + 91) = v106; *((char *)&loc_4FAC98 + 100) = BYTE1(v106);
                    v107 = gMovUnk22[(unsigned char)pData[13]]; *((char *)&loc_4FAC98 + 112) = v107; *((char *)&loc_4FAC98 + 119) = BYTE1(v107); v107 >>= 16; *((char *)&loc_4FAC98 + 128) = v107; *((char *)&loc_4FAC98 + 137) = BYTE1(v107); HIWORD(v107) = *Cstr; LOWORD(v107) = *Cstr;
                    v108 = gMovSurfWidth; *(int *)pFrame1 = v107; *(int *)&pFrame1[v108] = v107; *((int *)pFrame1 + 1) = v107; *(int *)&pFrame1[v108 + 4] = v107; *((int *)pFrame1 + 2) = v107; *(int *)&pFrame1[v108 + 8] = v107; *((int *)pFrame1 + 3) = v107; *(int *)&pFrame1[v108 + 12] = v107;
                    v109 = &pFrame1[2 * v108]; *(int *)v109 = v107; *(int *)&v109[v108] = v107; *((int *)v109 + 1) = v107; *(int *)&v109[v108 + 4] = v107; *((int *)v109 + 2) = v107; *(int *)&v109[v108 + 8] = v107; *((int *)v109 + 3) = v107; *(int *)&v109[v108 + 12] = v107;
                    v110 = &v109[2 * v108]; *(int *)v110 = v107; *(int *)&v110[v108] = v107; *((int *)v110 + 1) = v107; *(int *)&v110[v108 + 4] = v107; *((int *)v110 + 2) = v107; *(int *)&v110[v108 + 8] = v107; *((int *)v110 + 3) = v107; *(int *)&v110[v108 + 12] = v107;
                    v111 = &v110[2 * v108]; *(int *)v111 = v107; *(int *)&v111[v108] = v107; *((int *)v111 + 1) = v107; *(int *)&v111[v108 + 4] = v107; *((int *)v111 + 2) = v107; *(int *)&v111[v108 + 8] = v107; *((int *)v111 + 3) = v107; *(int *)&v111[v108 + 12] = v107;
                } else {
                    *(&loc_4FAA6C + 2) = BYTE1(gMovUnk22[(unsigned char)pData[10]]); *((char *)&loc_4FAA6C + 8) = v78;
                    v79 = HIWORD(v78); *((char *)&loc_4FAA6C + 13) = BYTE1(v79); *((char *)&loc_4FAA6C + 19) = v79;
                    v80 = gMovUnk22[(unsigned char)pData[11]]; *((char *)&loc_4FAA6C + 25) = BYTE1(v80); *((char *)&loc_4FAA6C + 31) = v80; v80 >>= 16; *((char *)&loc_4FAA6C + 37) = BYTE1(v80); *((char *)&loc_4FAA6C + 43) = v80;
                    v81 = gMovUnk22[(unsigned char)pData[12]]; *((char *)&loc_4FAA6C + 51) = BYTE1(v81); *((char *)&loc_4FAA6C + 57) = v81; v81 >>= 16; *((char *)&loc_4FAA6C + 62) = BYTE1(v81); *((char *)&loc_4FAA6C + 68) = v81;
                    v82 = gMovUnk22[(unsigned char)pData[13]]; *((char *)&loc_4FAA6C + 74) = BYTE1(v82); *((char *)&loc_4FAA6C + 80) = v82; v82 >>= 16; *((char *)&loc_4FAA6C + 86) = BYTE1(v82); *((char *)&loc_4FAA6C + 92) = v82;
                    v83 = gMovUnk22[(unsigned char)pData[14]]; *((char *)&loc_4FAA6C + 100) = BYTE1(v83); *((char *)&loc_4FAA6C + 106) = v83; v83 >>= 16; *((char *)&loc_4FAA6C + 111) = BYTE1(v83); *((char *)&loc_4FAA6C + 117) = v83;
                    v84 = gMovUnk22[(unsigned char)pData[15]]; *((char *)&loc_4FAA6C + 123) = BYTE1(v84); *((char *)&loc_4FAA6C + 129) = v84; v84 >>= 16; *((char *)&loc_4FAA6C + 135) = BYTE1(v84); *((char *)&loc_4FAA6C + 141) = v84;
                    v85 = gMovUnk22[(unsigned char)pData[16]]; *((char *)&loc_4FAA6C + 149) = BYTE1(v85); *((char *)&loc_4FAA6C + 155) = v85; v85 >>= 16; *((char *)&loc_4FAA6C + 160) = BYTE1(v85); *((char *)&loc_4FAA6C + 166) = v85;
                    v86 = gMovUnk22[(unsigned char)pData[17]]; *((char *)&loc_4FAA6C + 172) = BYTE1(v86); *((char *)&loc_4FAA6C + 178) = v86; v86 >>= 16; *((char *)&loc_4FAA6C + 184) = BYTE1(v86); *((char *)&loc_4FAA6C + 190) = v86;
                    v87 = gMovUnk22[(unsigned char)pData[18]]; *((char *)&loc_4FAA6C + 198) = BYTE1(v87); *((char *)&loc_4FAA6C + 204) = v87; v87 >>= 16; *((char *)&loc_4FAA6C + 209) = BYTE1(v87); *((char *)&loc_4FAA6C + 215) = v87;
                    v88 = gMovUnk22[(unsigned char)pData[19]]; *((char *)&loc_4FAA6C + 221) = BYTE1(v88); *((char *)&loc_4FAA6C + 227) = v88; v88 >>= 16; *((char *)&loc_4FAA6C + 233) = BYTE1(v88); *((char *)&loc_4FAA6C + 239) = v88;
                    v89 = gMovUnk22[(unsigned char)pData[20]]; *((char *)&loc_4FAA6C + 247) = BYTE1(v89); *((char *)&loc_4FAA6C + 253) = v89; v89 >>= 16; *((char *)&loc_4FAA6C + 258) = BYTE1(v89); *((char *)&loc_4FAA6C + 264) = v89;
                    v90 = gMovUnk22[(unsigned char)pData[21]]; *((char *)&loc_4FAA6C + 270) = BYTE1(v90); *((char *)&loc_4FAA6C + 276) = v90; v90 >>= 16; *((char *)&loc_4FAA6C + 282) = BYTE1(v90); *((char *)&loc_4FAA6C + 288) = v90;
                    v91 = gMovUnk22[(unsigned char)pData[22]]; *((char *)&loc_4FAA6C + 296) = BYTE1(v91); *((char *)&loc_4FAA6C + 302) = v91; v91 >>= 16; *((char *)&loc_4FAA6C + 307) = BYTE1(v91); *((char *)&loc_4FAA6C + 313) = v91;
                    v92 = gMovUnk22[(unsigned char)pData[23]]; *((char *)&loc_4FAA6C + 319) = BYTE1(v92); *((char *)&loc_4FAA6C + 325) = v92; v92 >>= 16; *((char *)&loc_4FAA6C + 331) = BYTE1(v92); *((char *)&loc_4FAA6C + 337) = v92;
                    v93 = gMovUnk22[(unsigned char)pData[24]]; *((char *)&loc_4FAA6C + 345) = BYTE1(v93); *((char *)&loc_4FAA6C + 351) = v93; v93 >>= 16; *((char *)&loc_4FAA6C + 356) = BYTE1(v93); *((char *)&loc_4FAA6C + 362) = v93;
                    v94 = (unsigned char)pData[25]; v95 = gMovUnk22[v94]; *((char *)&loc_4FAA6C + 368) = BYTE1(v95); *((char *)&loc_4FAA6C + 374) = v95; v95 >>= 16; *((char *)&loc_4FAA6C + 380) = BYTE1(v95); *((char *)&loc_4FAA6C + 386) = v95;
                    LOWORD(v95) = *Cstr;
                    v96 = gMovSurfWidth;
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *(int *)pFrame1 = v94;
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *((int *)pFrame1 + 1) = v94;
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *((int *)pFrame1 + 2) = v94;
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *((int *)pFrame1 + 3) = v94; v97 = &pFrame1[v96];
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *(int *)v97 = v94;
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *((int *)v97 + 1) = v94;
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *((int *)v97 + 2) = v94;
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *((int *)v97 + 3) = v94; v98 = &v97[v96];
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *(int *)v98 = v94;
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *((int *)v98 + 1) = v94;
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *((int *)v98 + 2) = v94;
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *((int *)v98 + 3) = v94; v99 = &v98[v96];
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *(int *)v99 = v94;
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *((int *)v99 + 1) = v94;
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *((int *)v99 + 2) = v94;
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *((int *)v99 + 3) = v94; v100 = &v99[v96];
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *(int *)v100 = v94;
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *((int *)v100 + 1) = v94;
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *((int *)v100 + 2) = v94;
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *((int *)v100 + 3) = v94; v101 = &v100[v96];
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *(int *)v101 = v94;
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *((int *)v101 + 1) = v94;
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *((int *)v101 + 2) = v94;
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *((int *)v101 + 3) = v94; v102 = &v101[v96];
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *(int *)v102 = v94;
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *((int *)v102 + 1) = v94;
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *((int *)v102 + 2) = v94;
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *((int *)v102 + 3) = v94; v103 = &v102[v96];
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *(int *)v103 = v94;
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *((int *)v103 + 1) = v94;
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *((int *)v103 + 2) = v94;
                    LOWORD(v94) = v95; v94 <<= 16; LOWORD(v94) = v95; *((int *)v103 + 3) = v94;
                }
            }
*/
            break;
        case 0xA:
/*
            if( *Cstr < 0 ){
                v167 = gMovUnk22[(unsigned char)pData[10]];
                if ( *((short *)pData + 9) < 0 ){
                    *(&loc_4FB9B4 + 2) = BYTE1(gMovUnk22[(unsigned char)pData[10]]); *((char *)&loc_4FB9B4 + 8) = v167;
                    v201 = HIWORD(v167); *((char *)&loc_4FB9B4 + 13) = BYTE1(v201); *((char *)&loc_4FB9B4 + 19) = v201;
                    v202 = gMovUnk22[(unsigned char)pData[11]]; *((char *)&loc_4FB9B4 + 25) = BYTE1(v202); *((char *)&loc_4FB9B4 + 31) = v202; v202 >>= 16; *((char *)&loc_4FB9B4 + 37) = BYTE1(v202); *((char *)&loc_4FB9B4 + 43) = v202;
                    v203 = gMovUnk22[(unsigned char)pData[12]]; *((char *)&loc_4FB9B4 + 51) = BYTE1(v203); *((char *)&loc_4FB9B4 + 57) = v203; v203 >>= 16; *((char *)&loc_4FB9B4 + 62) = BYTE1(v203); *((char *)&loc_4FB9B4 + 68) = v203;
                    v204 = gMovUnk22[(unsigned char)pData[13]]; *((char *)&loc_4FB9B4 + 74) = BYTE1(v204); *((char *)&loc_4FB9B4 + 80) = v204; v204 >>= 16; *((char *)&loc_4FB9B4 + 86) = BYTE1(v204); *((char *)&loc_4FB9B4 + 92) = v204;
                    v205 = gMovUnk22[(unsigned char)pData[14]]; *((char *)&loc_4FB9B4 + 100) = BYTE1(v205); *((char *)&loc_4FB9B4 + 106) = v205; v205 >>= 16; *((char *)&loc_4FB9B4 + 111) = BYTE1(v205); *((char *)&loc_4FB9B4 + 117) = v205;
                    v206 = gMovUnk22[(unsigned char)pData[15]]; *((char *)&loc_4FB9B4 + 123) = BYTE1(v206); *((char *)&loc_4FB9B4 + 129) = v206; v206 >>= 16; *((char *)&loc_4FB9B4 + 135) = BYTE1(v206); *((char *)&loc_4FB9B4 + 141) = v206;
                    v207 = gMovUnk22[(unsigned char)pData[16]]; *((char *)&loc_4FB9B4 + 149) = BYTE1(v207); *((char *)&loc_4FB9B4 + 155) = v207; v207 >>= 16; *((char *)&loc_4FB9B4 + 160) = BYTE1(v207); *((char *)&loc_4FB9B4 + 166) = v207;
                    v208 = gMovUnk22[(unsigned char)pData[17]]; *((char *)&loc_4FB9B4 + 172) = BYTE1(v208); *((char *)&loc_4FB9B4 + 178) = v208; v208 >>= 16; *((char *)&loc_4FB9B4 + 184) = BYTE1(v208); *((char *)&loc_4FB9B4 + 190) = v208;
                    v209 = gMovUnk22[(unsigned char)pData[26]]; *((char *)&loc_4FB9B4 + 223) = BYTE1(v209); *((char *)&loc_4FB9B4 + 229) = v209; v209 >>= 16; *((char *)&loc_4FB9B4 + 234) = BYTE1(v209); *((char *)&loc_4FB9B4 + 240) = v209;
                    v210 = gMovUnk22[(unsigned char)pData[27]]; *((char *)&loc_4FB9B4 + 246) = BYTE1(v210); *((char *)&loc_4FB9B4 + 252) = v210; v210 >>= 16; *((char *)&loc_4FB9B4 + 258) = BYTE1(v210); *((char *)&loc_4FB9B4 + 264) = v210;
                    v211 = gMovUnk22[(unsigned char)pData[28]]; *((char *)&loc_4FB9B4 + 272) = BYTE1(v211); *((char *)&loc_4FB9B4 + 278) = v211; v211 >>= 16; *((char *)&loc_4FB9B4 + 283) = BYTE1(v211); *((char *)&loc_4FB9B4 + 289) = v211;
                    v212 = gMovUnk22[(unsigned char)pData[29]]; *((char *)&loc_4FB9B4 + 295) = BYTE1(v212); *((char *)&loc_4FB9B4 + 301) = v212; v212 >>= 16; *((char *)&loc_4FB9B4 + 307) = BYTE1(v212); *((char *)&loc_4FB9B4 + 313) = v212;
                    v213 = gMovUnk22[(unsigned char)pData[30]]; *((char *)&loc_4FB9B4 + 321) = BYTE1(v213); *((char *)&loc_4FB9B4 + 327) = v213; v213 >>= 16; *((char *)&loc_4FB9B4 + 332) = BYTE1(v213); *((char *)&loc_4FB9B4 + 338) = v213;
                    v214 = gMovUnk22[(unsigned char)pData[31]]; *((char *)&loc_4FB9B4 + 344) = BYTE1(v214); *((char *)&loc_4FB9B4 + 350) = v214; v214 >>= 16; *((char *)&loc_4FB9B4 + 356) = BYTE1(v214); *((char *)&loc_4FB9B4 + 362) = v214;
                    v215 = gMovUnk22[(unsigned char)pData[32]]; *((char *)&loc_4FB9B4 + 370) = BYTE1(v215); *((char *)&loc_4FB9B4 + 376) = v215; v215 >>= 16; *((char *)&loc_4FB9B4 + 381) = BYTE1(v215); *((char *)&loc_4FB9B4 + 387) = v215;
                    v216 = (unsigned char)pData[33];
                    v217 = gMovUnk22[v216];
                    *((char *)&loc_4FB9B4 + 393) = BYTE1(v217); *((char *)&loc_4FB9B4 + 399) = v217; v217 >>= 16; *((char *)&loc_4FB9B4 + 405) = BYTE1(v217); *((char *)&loc_4FB9B4 + 411) = v217;
                    LOWORD(v217) = *Cstr & 0x7FFF;
                    v218 = gMovSurfWidth;
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *(int *)pFrame1 = v216;
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *((int *)pFrame1 + 1) = v216;
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *((int *)pFrame1 + 2) = v216;
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *((int *)pFrame1 + 3) = v216; v219 = &pFrame1[v218];
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *(int *)v219 = v216;
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *((int *)v219 + 1) = v216;
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *((int *)v219 + 2) = v216;
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *((int *)v219 + 3) = v216; v220 = &v219[v218];
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *(int *)v220 = v216;
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *((int *)v220 + 1) = v216;
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *((int *)v220 + 2) = v216;
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *((int *)v220 + 3) = v216; v221 = &v220[v218];
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *(int *)v221 = v216;
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *((int *)v221 + 1) = v216;
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *((int *)v221 + 2) = v216;
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *((int *)v221 + 3) = v216; v222 = &v221[v218]; HIWORD(v216) = (unsigned int)(pData + 2) >> 16;
                    LOWORD(v217) = *((_WORD *)pData + 9) & 0x7FFF;
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *(int *)v222 = v216;
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *((int *)v222 + 1) = v216;
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *((int *)v222 + 2) = v216;
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *((int *)v222 + 3) = v216; v223 = &v222[v218];
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *(int *)v223 = v216;
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *((int *)v223 + 1) = v216;
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *((int *)v223 + 2) = v216;
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *((int *)v223 + 3) = v216; v224 = &v223[v218];
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *(int *)v224 = v216;
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *((int *)v224 + 1) = v216;
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *((int *)v224 + 2) = v216;
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *((int *)v224 + 3) = v216; v225 = &v224[v218]; 
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *(int *)v225 = v216;
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *((int *)v225 + 1) = v216;
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *((int *)v225 + 2) = v216;
                    LOWORD(v216) = v217; v216 <<= 16; LOWORD(v216) = v217; *((int *)v225 + 3) = v216;
                } else {
                    *(&loc_4FB640 + 2) = BYTE1(gMovUnk22[(unsigned char)pData[10]]);
                    *((char *)&loc_4FB640 + 8) = v167;
                    v168 = HIWORD(v167); *((char *)&loc_4FB640 + 13) = BYTE1(v168); *((char *)&loc_4FB640 + 19) = v168;
                    v169 = gMovUnk22[(unsigned char)pData[11]]; *((char *)&loc_4FB640 + 27) = BYTE1(v169); *((char *)&loc_4FB640 + 33) = v169; v169 >>= 16; *((char *)&loc_4FB640 + 38) = BYTE1(v169); *((char *)&loc_4FB640 + 44) = v169;
                    v170 = gMovUnk22[(unsigned char)pData[12]]; *((char *)&loc_4FB640 + 52) = BYTE1(v170); *((char *)&loc_4FB640 + 58) = v170; v170 >>= 16; *((char *)&loc_4FB640 + 63) = BYTE1(v170); *((char *)&loc_4FB640 + 69) = v170;
                    v171 = gMovUnk22[(unsigned char)pData[13]]; *((char *)&loc_4FB640 + 77) = BYTE1(v171); *((char *)&loc_4FB640 + 83) = v171; v171 >>= 16; *((char *)&loc_4FB640 + 88) = BYTE1(v171); *((char *)&loc_4FB640 + 94) = v171;
                    v172 = gMovUnk22[(unsigned char)pData[14]]; *((char *)&loc_4FB640 + 102) = BYTE1(v172); *((char *)&loc_4FB640 + 108) = v172; v172 >>= 16; *((char *)&loc_4FB640 + 113) = BYTE1(v172); *((char *)&loc_4FB640 + 119) = v172;
                    v173 = gMovUnk22[(unsigned char)pData[15]]; *((char *)&loc_4FB640 + 127) = BYTE1(v173); *((char *)&loc_4FB640 + 133) = v173; v173 >>= 16; *((char *)&loc_4FB640 + 138) = BYTE1(v173); *((char *)&loc_4FB640 + 144) = v173;
                    v174 = gMovUnk22[(unsigned char)pData[16]]; *((char *)&loc_4FB640 + 152) = BYTE1(v174); *((char *)&loc_4FB640 + 158) = v174; v174 >>= 16; *((char *)&loc_4FB640 + 163) = BYTE1(v174); *((char *)&loc_4FB640 + 169) = v174;
                    v175 = gMovUnk22[(unsigned char)pData[17]]; *((char *)&loc_4FB640 + 177) = BYTE1(v175); *((char *)&loc_4FB640 + 183) = v175; v175 >>= 16; *((char *)&loc_4FB640 + 188) = BYTE1(v175); *((char *)&loc_4FB640 + 194) = v175;
                    v176 = gMovUnk22[(unsigned char)pData[26]]; *((char *)&loc_4FB640 + 230) = BYTE1(v176); *((char *)&loc_4FB640 + 236) = v176; v176 >>= 16; *((char *)&loc_4FB640 + 241) = BYTE1(v176); *((char *)&loc_4FB640 + 247) = v176;
                    v177 = gMovUnk22[(unsigned char)pData[27]]; *((char *)&loc_4FB640 + 255) = BYTE1(v177); *((char *)&loc_4FB640 + 261) = v177; v177 >>= 16; *((char *)&loc_4FB640 + 266) = BYTE1(v177); *((char *)&loc_4FB640 + 272) = v177;
                    v178 = gMovUnk22[(unsigned char)pData[28]]; *((char *)&loc_4FB640 + 280) = BYTE1(v178); *((char *)&loc_4FB640 + 286) = v178; v178 >>= 16; *((char *)&loc_4FB640 + 291) = BYTE1(v178); *((char *)&loc_4FB640 + 297) = v178;
                    v179 = gMovUnk22[(unsigned char)pData[29]]; *((char *)&loc_4FB640 + 305) = BYTE1(v179); *((char *)&loc_4FB640 + 311) = v179; v179 >>= 16; *((char *)&loc_4FB640 + 316) = BYTE1(v179); *((char *)&loc_4FB640 + 322) = v179;
                    v180 = gMovUnk22[(unsigned char)pData[30]]; *((char *)&loc_4FB640 + 330) = BYTE1(v180); *((char *)&loc_4FB640 + 336) = v180; v180 >>= 16; *((char *)&loc_4FB640 + 341) = BYTE1(v180); *((char *)&loc_4FB640 + 347) = v180;
                    v181 = gMovUnk22[(unsigned char)pData[31]]; *((char *)&loc_4FB640 + 355) = BYTE1(v181); *((char *)&loc_4FB640 + 361) = v181; v181 >>= 16; *((char *)&loc_4FB640 + 366) = BYTE1(v181); *((char *)&loc_4FB640 + 372) = v181;
                    v182 = gMovUnk22[(unsigned char)pData[32]]; *((char *)&loc_4FB640 + 380) = BYTE1(v182); *((char *)&loc_4FB640 + 386) = v182; v182 >>= 16; *((char *)&loc_4FB640 + 391) = BYTE1(v182); *((char *)&loc_4FB640 + 397) = v182;
                    v183 = (unsigned char)pData[33]; v184 = gMovUnk22[v183]; *((char *)&loc_4FB640 + 405) = BYTE1(v184); *((char *)&loc_4FB640 + 411) = v184; v184 >>= 16; *((char *)&loc_4FB640 + 416) = BYTE1(v184); *((char *)&loc_4FB640 + 422) = v184;
                    LOWORD(v184) = *Cstr & 0x7FFF; 
                    v185 = gMovSurfWidth; LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184; *(int *)pFrame1 = v183; LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184;
                    *((int *)pFrame1 + 1) = v183; v186 = &pFrame1[v185]; LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184; *(int *)v186 = v183; LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184;
                    *((int *)v186 + 1) = v183; v187 = &v186[v185]; LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184; *(int *)v187 = v183; LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184;
                    *((int *)v187 + 1) = v183; v188 = &v187[v185]; LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184; *(int *)v188 = v183; LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184;
                    *((int *)v188 + 1) = v183; v189 = &v188[v185]; LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184; *(int *)v189 = v183; LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184;
                    *((int *)v189 + 1) = v183; v190 = &v189[v185]; LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184; *(int *)v190 = v183; LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184;
                    *((int *)v190 + 1) = v183; v191 = &v190[v185]; LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184; *(int *)v191 = v183; LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184;
                    *((int *)v191 + 1) = v183; v192 = &v191[v185]; LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184; *(int *)v192 = v183; LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184;
                    *((int *)v192 + 1) = v183; v193 = &v192[v185 + 8 + -8 * v185]; HIWORD(v183) = (unsigned int)(pData + 2) >> 16; LOWORD(v184) = *((_WORD *)pData + 9); LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184; *(int *)v193 = v183; LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184;
                    *((int *)v193 + 1) = v183; v194 = &v193[v185]; LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184; *(int *)v194 = v183; LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184;
                    *((int *)v194 + 1) = v183; v195 = &v194[v185]; LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184; *v195 = v183; LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184;
                    v195[1] = v183; v196 = (int *)((char *)v195 + v185); LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184; *v196 = v183; LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184;
                    v196[1] = v183; v197 = (int *)((char *)v196 + v185); LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184; *v197 = v183; LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184;
                    v197[1] = v183; v198 = (int *)((char *)v197 + v185); LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184; *v198 = v183; LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184;
                    v198[1] = v183; v199 = (int *)((char *)v198 + v185); LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184; *v199 = v183; LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184;
                    v199[1] = v183; v200 = (int *)((char *)v199 + v185); LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184; *v200 = v183; LOWORD(v183) = v184; v183 <<= 16; LOWORD(v183) = v184; v200[1] = v183;
                }
            } else {
                v134 = gMovUnk22[(unsigned char)pData[10]]; *(&loc_4FB294 + 2) = BYTE1(v134); *((char *)&loc_4FB294 + 8) = v134; v134 >>= 16; *((char *)&loc_4FB294 + 13) = BYTE1(v134); *((char *)&loc_4FB294 + 19) = v134;
                v135 = gMovUnk22[(unsigned char)pData[11]]; *((char *)&loc_4FB294 + 27) = BYTE1(v135); *((char *)&loc_4FB294 + 33) = v135; v135 >>= 16; *((char *)&loc_4FB294 + 38) = BYTE1(v135); *((char *)&loc_4FB294 + 44) = v135;
                v136 = gMovUnk22[(unsigned char)pData[12]]; *((char *)&loc_4FB294 + 52) = BYTE1(v136); *((char *)&loc_4FB294 + 58) = v136; v136 >>= 16; *((char *)&loc_4FB294 + 63) = BYTE1(v136); *((char *)&loc_4FB294 + 69) = v136;
                v137 = gMovUnk22[(unsigned char)pData[13]]; *((char *)&loc_4FB294 + 77) = BYTE1(v137); *((char *)&loc_4FB294 + 83) = v137; v137 >>= 16; *((char *)&loc_4FB294 + 88) = BYTE1(v137); *((char *)&loc_4FB294 + 94) = v137;
                v138 = gMovUnk22[(unsigned char)pData[22]]; *((char *)&loc_4FB294 + 121) = BYTE1(v138); *((char *)&loc_4FB294 + 127) = v138; v138 >>= 16; *((char *)&loc_4FB294 + 132) = BYTE1(v138); *((char *)&loc_4FB294 + 138) = v138;
                v139 = gMovUnk22[(unsigned char)pData[23]]; *((char *)&loc_4FB294 + 146) = BYTE1(v139); *((char *)&loc_4FB294 + 152) = v139; v139 >>= 16; *((char *)&loc_4FB294 + 157) = BYTE1(v139); *((char *)&loc_4FB294 + 163) = v139;
                v140 = gMovUnk22[(unsigned char)pData[24]]; *((char *)&loc_4FB294 + 171) = BYTE1(v140); *((char *)&loc_4FB294 + 177) = v140; v140 >>= 16; *((char *)&loc_4FB294 + 182) = BYTE1(v140); *((char *)&loc_4FB294 + 188) = v140;
                v141 = gMovUnk22[(unsigned char)pData[25]]; *((char *)&loc_4FB294 + 196) = BYTE1(v141); *((char *)&loc_4FB294 + 202) = v141; v141 >>= 16; *((char *)&loc_4FB294 + 207) = BYTE1(v141); *((char *)&loc_4FB294 + 213) = v141;
                v142 = gMovUnk22[(unsigned char)pData[34]]; *((char *)&loc_4FB294 + 249) = BYTE1(v142); *((char *)&loc_4FB294 + 255) = v142; v142 >>= 16; *((char *)&loc_4FB294 + 260) = BYTE1(v142); *((char *)&loc_4FB294 + 266) = v142;
                v143 = gMovUnk22[(unsigned char)pData[35]]; *((char *)&loc_4FB294 + 274) = BYTE1(v143); *((char *)&loc_4FB294 + 280) = v143; v143 >>= 16; *((char *)&loc_4FB294 + 285) = BYTE1(v143); *((char *)&loc_4FB294 + 291) = v143;
                v144 = gMovUnk22[(unsigned char)pData[36]]; *((char *)&loc_4FB294 + 299) = BYTE1(v144); *((char *)&loc_4FB294 + 305) = v144; v144 >>= 16; *((char *)&loc_4FB294 + 310) = BYTE1(v144); *((char *)&loc_4FB294 + 316) = v144;
                v145 = gMovUnk22[(unsigned char)pData[37]]; *((char *)&loc_4FB294 + 324) = BYTE1(v145); *((char *)&loc_4FB294 + 330) = v145; v145 >>= 16; *((char *)&loc_4FB294 + 335) = BYTE1(v145); *((char *)&loc_4FB294 + 341) = v145;
                v146 = gMovUnk22[(unsigned char)pData[46]]; *((char *)&loc_4FB294 + 368) = BYTE1(v146); *((char *)&loc_4FB294 + 374) = v146; v146 >>= 16; *((char *)&loc_4FB294 + 379) = BYTE1(v146); *((char *)&loc_4FB294 + 385) = v146;
                v147 = gMovUnk22[(unsigned char)pData[47]]; *((char *)&loc_4FB294 + 393) = BYTE1(v147); *((char *)&loc_4FB294 + 399) = v147; v147 >>= 16; *((char *)&loc_4FB294 + 404) = BYTE1(v147); *((char *)&loc_4FB294 + 410) = v147;
                v148 = gMovUnk22[(unsigned char)pData[48]]; *((char *)&loc_4FB294 + 418) = BYTE1(v148); *((char *)&loc_4FB294 + 424) = v148; v148 >>= 16; *((char *)&loc_4FB294 + 429) = BYTE1(v148); *((char *)&loc_4FB294 + 435) = v148;
                v149 = (unsigned char)pData[49]; 
                v150 = gMovUnk22[v149]; *((char *)&loc_4FB294 + 443) = BYTE1(v150); *((char *)&loc_4FB294 + 449) = v150; v150 >>= 16; *((char *)&loc_4FB294 + 454) = BYTE1(v150); *((char *)&loc_4FB294 + 460) = v150; LOWORD(v150) = *Cstr;
                v151 = gMovSurfWidth;  LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150; *(int *)pFrame1 = v149; LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150; *((int *)pFrame1 + 1) = v149;
                v152 = &pFrame1[v151]; LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150; *(int *)v152 = v149; LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150; *((int *)v152 + 1) = v149;
                v153 = &v152[v151]; LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150; *(int *)v153 = v149; LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150; *((int *)v153 + 1) = v149;
                v154 = &v153[v151]; LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150; *(int *)v154 = v149; LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150; *((int *)v154 + 1) = v149;
                v155 = &v154[v151]; HIWORD(v149) = (unsigned int)(pData + 2) >> 16; LOWORD(v150) = *((_WORD *)pData + 7); LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150;*(int *)v155 = v149;LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150; *((int *)v155 + 1) = v149;
                v156 = &v155[v151]; LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150; *(int *)v156 = v149; LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150; *((int *)v156 + 1) = v149;
                v157 = &v156[v151]; LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150; *(int *)v157 = v149; LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150; *((int *)v157 + 1) = v149;
                v158 = &v157[v151]; LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150; *(int *)v158 = v149; LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150; *((int *)v158 + 1) = v149;
                v159 = &v158[v151 + 8 + -8 * v151]; HIWORD(v149) = (unsigned int)(pData + 2) >> 16; LOWORD(v150) = *((_WORD *)pData + 13); LOWORD(v149) = v150;v149 <<= 16; LOWORD(v149) = v150; *(int *)v159 = v149;LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150; *((int *)v159 + 1) = v149;
                v160 = &v159[v151]; LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150; *(int *)v160 = v149; LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150; *((int *)v160 + 1) = v149;
                v161 = &v160[v151]; LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150; *v161 = v149; LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150; v161[1] = v149; 
                v162 = (int *)((char *)v161 + v151); LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150; *v162 = v149; LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150; v162[1] = v149;
                v163 = (int *)((char *)v162 + v151); HIWORD(v149) = (unsigned int)(pData + 2) >> 16; LOWORD(v150) = *((_WORD *)pData + 19); LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150; *v163 = v149; LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150; v163[1] = v149;
                v164 = (int *)((char *)v163 + v151); LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150; *v164 = v149; LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150; v164[1] = v149;
                v165 = (int *)((char *)v164 + v151); LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150; *v165 = v149; LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150; v165[1] = v149;
                v166 = (int *)((char *)v165 + v151); LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150; *v166 = v149; LOWORD(v149) = v150; v149 <<= 16; LOWORD(v149) = v150; v166[1] = v149;
            }
*/
            break;
        case 0xB:
/*
            v226 = gMovSurfWidth; *(int *)pFrame1 = *(int *)Cstr; *((int *)pFrame1 + 1) = *(int *)(pData + 6); *((int *)pFrame1 + 2) = *(int *)(pData + 10); *((int *)pFrame1 + 3) = *(int *)(pData + 14);
            v227 = &pFrame1[v226]; *(int *)v227 = *(int *)(pData + 18); *((int *)v227 + 1) = *(int *)(pData + 22); *((int *)v227 + 2) = *(int *)(pData + 26); *((int *)v227 + 3) = *(int *)(pData + 30);
            v228 = &v227[v226]; *(int *)v228 = *(int *)(pData + 34); *((int *)v228 + 1) = *(int *)(pData + 38); *((int *)v228 + 2) = *(int *)(pData + 42); *((int *)v228 + 3) = *(int *)(pData + 46);
            v229 = &v228[v226]; *(int *)v229 = *(int *)(pData + 50); *((int *)v229 + 1) = *(int *)(pData + 54); *((int *)v229 + 2) = *(int *)(pData + 58); *((int *)v229 + 3) = *(int *)(pData + 62);
            v230 = &v229[v226]; *(int *)v230 = *(int *)(pData + 66); *((int *)v230 + 1) = *(int *)(pData + 70); *((int *)v230 + 2) = *(int *)(pData + 74); *((int *)v230 + 3) = *(int *)(pData + 78);
            v231 = &v230[v226]; *(int *)v231 = *(int *)(pData + 82); *((int *)v231 + 1) = *(int *)(pData + 86); *((int *)v231 + 2) = *(int *)(pData + 90); *((int *)v231 + 3) = *(int *)(pData + 94);
            v232 = &v231[v226]; *(int *)v232 = *(int *)(pData + 98);  *((int *)v232 + 1) = *(int *)(pData + 102); *((int *)v232 + 2) = *(int *)(pData + 106); *((int *)v232 + 3) = *(int *)(pData + 110);
            v233 = &v232[v226]; *(int *)v233 = *(int *)(pData + 114); *((int *)v233 + 1) = *(int *)(pData + 118); *((int *)v233 + 2) = *(int *)(pData + 122); *((int *)v233 + 3) = *(int *)(pData + 126);
*/
            break;
        case 0xC:
/*
            v234 = gMovSurfWidth;
            HIWORD(v235) = *Cstr; LOWORD(v235) = *Cstr; *(int *)pFrame1 = v235; *(int *)&pFrame1[v234] = v235;
            HIWORD(v235) = *((_WORD *)pData + 2); LOWORD(v235) = HIWORD(v235); *((int *)pFrame1 + 1) = v235; *(int *)&pFrame1[v234 + 4] = v235;
            HIWORD(v235) = *((_WORD *)pData + 3); LOWORD(v235) = HIWORD(v235); *((int *)pFrame1 + 2) = v235; *(int *)&pFrame1[v234 + 8] = v235;
            HIWORD(v235) = *((_WORD *)pData + 4); LOWORD(v235) = HIWORD(v235); *((int *)pFrame1 + 3) = v235; *(int *)&pFrame1[v234 + 12] = v235;
            v236 = &pFrame1[2 * v234];
            HIWORD(v235) = *((_WORD *)pData + 5); LOWORD(v235) = HIWORD(v235); *(int *)v236 = v235; *(int *)&v236[v234] = v235;
            HIWORD(v235) = *((_WORD *)pData + 6); LOWORD(v235) = HIWORD(v235); *((int *)v236 + 1) = v235; *(int *)&v236[v234 + 4] = v235;
            HIWORD(v235) = *((_WORD *)pData + 7); LOWORD(v235) = HIWORD(v235); *((int *)v236 + 2) = v235; *(int *)&v236[v234 + 8] = v235;
            HIWORD(v235) = *((_WORD *)pData + 8); LOWORD(v235) = HIWORD(v235); *((int *)v236 + 3) = v235; *(int *)&v236[v234 + 12] = v235;
            v237 = &v236[2 * v234];
            HIWORD(v235) = *((_WORD *)pData + 9); LOWORD(v235) = HIWORD(v235); *(int *)v237 = v235; *(int *)&v237[v234] = v235;
            HIWORD(v235) = *((_WORD *)pData + 10); LOWORD(v235) = HIWORD(v235); *((int *)v237 + 1) = v235; *(int *)&v237[v234 + 4] = v235;
            HIWORD(v235) = *((_WORD *)pData + 11); LOWORD(v235) = HIWORD(v235); *((int *)v237 + 2) = v235; *(int *)&v237[v234 + 8] = v235;
            HIWORD(v235) = *((_WORD *)pData + 12); LOWORD(v235) = HIWORD(v235); *((int *)v237 + 3) = v235; *(int *)&v237[v234 + 12] = v235;
            v238 = &v237[2 * v234];
            HIWORD(v235) = *((_WORD *)pData + 13); LOWORD(v235) = HIWORD(v235); *(int *)v238 = v235; *(int *)&v238[v234] = v235;
            HIWORD(v235) = *((_WORD *)pData + 14); LOWORD(v235) = HIWORD(v235); *((int *)v238 + 1) = v235; *(int *)&v238[v234 + 4] = v235;
            HIWORD(v235) = *((_WORD *)pData + 15); LOWORD(v235) = HIWORD(v235); *((int *)v238 + 2) = v235; *(int *)&v238[v234 + 8] = v235;
            HIWORD(v235) = *((_WORD *)pData + 16); LOWORD(v235) = HIWORD(v235); *((int *)v238 + 3) = v235; *(int *)&v238[v234 + 12] = v235;
*/
            break;
        case 0xD:
/*
            v239 = gMovSurfWidth; HIWORD(v240) = *Cstr; LOWORD(v240) = *Cstr; HIWORD(v241) = *((_WORD *)pData + 2); LOWORD(v241) = HIWORD(v241);
            *(int *)pFrame1 = v240; *((int *)pFrame1 + 1) = v240; *((int *)pFrame1 + 2) = v241; *((int *)pFrame1 + 3) = v241; *(int *)&pFrame1[v239] = v240; *(int *)&pFrame1[v239 + 4] = v240; *(int *)&pFrame1[v239 + 8] = v241; *(int *)&pFrame1[v239 + 12] = v241;
            v242 = &pFrame1[2 * v239]; *(int *)v242 = v240; *((int *)v242 + 1) = v240; *((int *)v242 + 2) = v241; *((int *)v242 + 3) = v241; *(int *)&v242[v239] = v240; *(int *)&v242[v239 + 4] = v240; *(int *)&v242[v239 + 8] = v241; *(int *)&v242[v239 + 12] = v241;
            v243 = &v242[2 * v239]; HIWORD(v240) = *((_WORD *)pData + 3); LOWORD(v240) = HIWORD(v240); HIWORD(v241) = *((_WORD *)pData + 4); LOWORD(v241) = HIWORD(v241);
            *(int *)v243 = v240; *((int *)v243 + 1) = v240; *((int *)v243 + 2) = v241; *((int *)v243 + 3) = v241; *(int *)&v243[v239] = v240; *(int *)&v243[v239 + 4] = v240; *(int *)&v243[v239 + 8] = v241; *(int *)&v243[v239 + 12] = v241;
            v244 = &v243[2 * v239]; *(int *)v244 = v240; *((int *)v244 + 1) = v240; *((int *)v244 + 2) = v241; *((int *)v244 + 3) = v241; *(int *)&v244[v239] = v240; *(int *)&v244[v239 + 4] = v240; *(int *)&v244[v239 + 8] = v241; *(int *)&v244[v239 + 12] = v241;
*/
            break;
        case 0xE:
/*
            HIWORD(v245) = *Cstr;
            LOWORD(v245) = *Cstr;
            v246 = gMovSurfWidth; *(int *)pFrame1 = v245; *((int *)pFrame1 + 1) = v245; *((int *)pFrame1 + 2) = v245; *((int *)pFrame1 + 3) = v245;
            v247 = &pFrame1[v246]; *(int *)v247 = v245; *((int *)v247 + 1) = v245; *((int *)v247 + 2) = v245; *((int *)v247 + 3) = v245;
            v248 = &v247[v246]; *(int *)v248 = v245; *((int *)v248 + 1) = v245; *((int *)v248 + 2) = v245; *((int *)v248 + 3) = v245;
            v249 = &v248[v246]; *(int *)v249 = v245; *((int *)v249 + 1) = v245; *((int *)v249 + 2) = v245; *((int *)v249 + 3) = v245;
            v250 = &v249[v246]; *(int *)v250 = v245; *((int *)v250 + 1) = v245; *((int *)v250 + 2) = v245; *((int *)v250 + 3) = v245;
            v251 = &v250[v246]; *(int *)v251 = v245; *((int *)v251 + 1) = v245; *((int *)v251 + 2) = v245; *((int *)v251 + 3) = v245;
            v252 = &v251[v246]; *(int *)v252 = v245; *((int *)v252 + 1) = v245; *((int *)v252 + 2) = v245; *((int *)v252 + 3) = v245;
            v253 = &v252[v246]; *(int *)v253 = v245; *((int *)v253 + 1) = v245; *((int *)v253 + 2) = v245; *((int *)v253 + 3) = v245;
*/
            break;
    }
    return NULL;
}

