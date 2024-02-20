#include "FrameWork.h"

char *(*gSciMseHandler)() = NULL; //xxx
int gSciUnk04 = 10;
int (*gScpOpcodeTable[ MAX_OPCODES ])( Intp_t * );
int gSciUnk02;
int gSciUnk01;
int gSciUnk03;

/**************************************************/

void SciUnk01()
{
    int i;

    ExportFlushVars();
    DialogFree();
    for( i = 0; i < 32; i++ ){
        if( gScpIfcSounds[ i ] ) ScpIfcSoundDelete( i | 0xA0000000 );
    }
    NevsClose();
    if( gIntpTasks ){
        dbg_free( gIntpTasks );
        gIntpTasks = NULL;
        gIntpTaskListCnt = 0;
    }
}

void SciUnk02( int a1 )
{
    gScpUnk01 = a1;
    if( a1 ){
        SciUnk25();
    } else if( ++gSciUnk01 == 1 ){
        gSciUnk02 = gIntpGetTime();
    }
}

void SciOpcodeExec( Intp_t *Scp, int Steps )
{
    Intp_t *ScrSave;
    char errstr[256], stmp[276];
//SCP_DBG_EN;
    ScrSave = gIntpCurScript;
    if( !gScpUnk01 || gSciUnk03 || Scp->i35 || ( Scp->Flags & 0x120 ) ) return;
    if( Scp->TimeAtExec == -1 ) Scp->TimeAtExec = 1000 * (unsigned int)gIntpGetTime() / gIntpTimeDiv;
    gIntpCurScript = Scp;    
    if( setjmp( Scp->EnvSave ) ){ // jumping target point, error handler
        gIntpCurScript = ScrSave;
        Scp->Flags |= ( SCR_FERROR | SCR_FEXIT );
DD
exit(0);
        return;
    }
    if( (Scp->Flags & SCR_FCRITICAL ) && (Steps < 3) ) Steps = 3; // minimum 3 steps
    while( ((Scp->Flags & SCR_FCRITICAL) || (--Steps != -1)) && !(Scp->Flags & 0x16D ) && !Scp->i35 ){
        if( Scp->Flags & SCR_FEXEC ){
    	    gSciUnk03 = 1;
    	    if( Scp->Func && Scp->Func( Scp ) ) {
        	gSciUnk03 = 0;
        	continue;
    	    }
    	    Scp->Func = NULL;
    	    gSciUnk03 = 0;
    	    Scp->Flags &= ~SCR_FEXEC;
        }
        Scp->Opcode = IntpReadBew( Scp->Code, Scp->CodePC );
        Scp->CodePC += 2;
        if( (Scp->Opcode & 0x8000) == 0 ){
            sprintf( stmp, "Bad opcode %x %c %d.", Scp->Opcode, Scp->Opcode, Scp->Opcode );
            IntpError( stmp ); // jump
        }
        if( gScpOpcodeTable[ Scp->Opcode & 0x3FF ] == 0 ){
            sprintf( errstr, "Undefined opcode %x.", Scp->Opcode );
            IntpError( errstr ); // jump
        }
        gScpOpcodeTable[ Scp->Opcode & 0x3FF ]( Scp );  //  exec        
    }
    if( Scp->Flags & SCR_FEXIT ){
        if( Scp->Parent ){
            if( Scp->Parent->Flags & SCR_FPROC_RUN ){
                Scp->Parent->Flags &= ~SCR_FPROC_RUN;
                Scp->Parent->ChildProcess = 0;
                Scp->Parent = NULL;
            }
        }
    }
    Scp->Flags &= ~0x40;
    gIntpCurScript = ScrSave;
    IntpMergeString( Scp );
}

void SciUnk03( Intp_t *scr, int Idx, int Arg )
{
    IntpPushIntStack( scr->StackB, &scr->StackIdxB, scr->CodePC ); 	IntpPushwB( scr, SCR_INT );
    IntpPushIntStack( scr->StackB, &scr->StackIdxB, Arg ); 		IntpPushwB( scr, SCR_INT );
    IntpPushIntStack( scr->StackA, &scr->StackApos, scr->Flags ); 	IntpPushwA( scr, SCR_INT );
    IntpPushPtrStack( scr->StackA, &scr->StackApos, scr->Func );	IntpPushwA( scr, SCR_PTR );
    IntpPushIntStack( scr->StackA, &scr->StackApos, scr->i34 ); 	IntpPushwA( scr, SCR_INT );
    scr->Flags = 0;
    scr->CodePC = Idx;
}

void SciUnk04( Intp_t *scr, int Pos, int Arg )
{
    SciUnk03( scr, Pos, Arg );
    IntpPushIntStack( scr->StackA, &scr->StackApos, 0 );
    IntpPushwA( scr, SCR_INT );
}

void SciUnk05( Intp_t *Scr1, Intp_t *Scr2, int Pos, int Arg )
{
DD
    IntpPushIntStack( Scr2->StackB, &Scr2->StackIdxB, Scr2->CodePC ); IntpPushwB( Scr2, SCR_INT );
    IntpPushIntStack( Scr2->StackB, &Scr2->StackIdxB, Scr1->Flags );   	IntpPushwB( Scr2, SCR_INT );
    IntpPushPtrStack( Scr2->StackB, &Scr2->StackIdxB, Scr1->Func );    	IntpPushwB( Scr2, SCR_PTR );
    IntpPushIntStack( Scr2->StackB, &Scr2->StackIdxB, Scr1->i34 );    	IntpPushwB( Scr2, SCR_INT );
    IntpPushIntStack( Scr2->StackB, &Scr2->StackIdxB, Arg );    	IntpPushwB( Scr2, SCR_INT );
    IntpPushIntStack( Scr2->StackA, &Scr2->StackApos, Scr2->Flags );   	IntpPushwA( Scr2, SCR_INT );
    IntpPushPtrStack( Scr2->StackA, &Scr2->StackApos, Scr2->Func );    	IntpPushwA( Scr2, SCR_PTR );
    IntpPushIntStack( Scr2->StackA, &Scr2->StackApos, Scr2->i34 );    	IntpPushwA( Scr2, SCR_INT );
    Scr2->Flags = 0;
    Scr2->CodePC = Pos;
    Scr2->i34 = Scr1->i34;
    Scr1->Flags |= 0x20;
}

void SciUnk06( Intp_t *scr1, Intp_t *scr2, int Pos, int Arg )
{
    SciUnk05( scr1, scr2, Pos, Arg);
    IntpPushIntStack( scr2->StackA, &scr2->StackApos, 0 ); IntpPushwA( scr2, SCR_INT );
}

void SciUnk07( Intp_t *a1, int a2, char *a3 )
{
    char *v5; 
    char *v6;

    if( !a2 ) return;        
    v6 = &a3[2 * a2];
    for( v5 = a3; v5 < v6; v5 += 8 ){
        if( *v5 >= 2 ){
    	    if( *v5 <= 2 ){
    		IntpPushIntStack(a1->StackA, &a1->StackApos, v5[1] );
    		IntpPushwA( a1, 0xA001 );
    	    } else {
    		if( *v5 == 3 ){
    		    IntpPushIntStack( a1->StackA, &a1->StackApos, v5[1] );
    		    IntpPushwA( a1, 0xC001 );
    		}
    	    }
        } else {
    	    if( *v5 == 1 ){
DD
//        	IntpPushIntStack( a1->StackA, &a1->StackApos, IntrpretUnk02( a1, v5[1], v6 ) );
        	IntpPushwA( a1, 0x9801 );
    	    }
        }
        
    }    
}

void SciUnk08( Intp_t *scr, int a2, int *a3, char *a4, int a5 )
{
    char *v9;
    jmp_buf p;
    short v15, v28;
    Intp_t *Scp;    
    short v320;
    char *v321;
    int Bei,EnvSave,v14,v18,v20,v21,v29,*v30,*pIdx,v34,*v35,*p_StackApos;    
    char *v22, *v23;

    v35 = a3;
    if( (IntpReadBei( (char *)&scr->ProcTable[ a2 ].NameOfst, 4) & 4) ){
        Bei = IntpReadBei( (char *)&scr->ProcTable[ a2 ].NameOfst, 0 );
        v9 = scr->ProcVarNames + Bei;
        Scp = ExportGetProcedure( v9, &v321, &v320 );
        if( !Scp ){ IntpLog( "External procedure %s not found\n", v9 ); return; }
        if( v320 != a5 ){ IntpLog( "Wrong number of arguments given to %s\n", v9 ); return; }
        SciUnk05(scr, Scp, v321, 40);
        SciUnk07(Scp, a5, a4);
        pIdx = &Scp->StackApos;
        IntpPushIntStack(Scp->StackA, &Scp->StackApos, a5);
        IntpPushwA( Scp, 0xC001 );
        memcpy( p, Scp->EnvSave, sizeof( jmp_buf ) );
        SciOpcodeExec(Scp, -1);
        memcpy( Scp->EnvSave, p, sizeof( jmp_buf ) );
        EnvSave = IntpPopShortStack(Scp->StackA, pIdx);
        v14 = IntpPopIntStack(Scp->StackA, pIdx);
        v34 = v14;
        if( EnvSave == 0x9801 );// IntpErrUnk01( Scp, 0x9801, v14 );
    } else {
	if( IntpReadBei( (char *)&scr->ProcTable[a2].NameOfst, 20) != a5 ){
    	    v18 = IntpReadBei( (char *)&scr->ProcTable[a2].NameOfst, 0 );
    	    eprintf( "Wrong number of args to procedure %s\n", scr->ProcVarNames + v18 );
    	    IntpLog( "Wrong number of args to procedure %s\n", scr ? scr->ProcVarNames + v18 : 0 );
    	    return;
	}
	v20 = IntpReadBei( (char *)&scr->ProcTable[ a2 ].NameOfst, 16 );
	v21 = 38;
        SciUnk03( scr, v20, 38 );
	v23 = &a4[ 8 * a5 ];
	p_StackApos = &scr->StackApos;
    	for( v22 = a4; v22 < v23; v22 += 8 ){
    	    if( v22[ 0 ] >= 2 ){
    		if( v22[ 0 ] <= 2 ){
        	    IntpPushIntStack( scr->StackA, p_StackApos, v21 = v22[ 4 ] );
        	    IntpPushwA( scr, 0xA001 );
    		} else {
        	    if( v22[ 0 ] == 3 ){
        		IntpPushIntStack( scr->StackA, p_StackApos, v21 = v22[ 4 ]);
        		IntpPushwA( scr, 0xC001 );
        	    }
    		}    		
    	    } else {
        	if( v22[ 0 ] == 1 ){
v21 = 0;
DD
//        	    v21 = IntrpretUnk02( scr, v22[ 4 ], v21 );
            	    IntpPushIntStack( scr->StackA, p_StackApos, v21 );
            	    IntpPushwA( scr, 0x9801 );
        	}
    	    }        	
    	}        
	IntpPushIntStack( scr->StackA, p_StackApos, a5 );
	IntpPushwA( scr, 0xC001 );
	memcpy( p, scr->EnvSave, sizeof( jmp_buf ) );
	SciOpcodeExec(scr, -1);
	memcpy( scr->EnvSave, p, sizeof( jmp_buf ) );
	v28 = IntpPopShortStack( scr->StackA, p_StackApos );
	v29 = IntpPopIntStack( scr->StackA, p_StackApos );
	v34 = v29;
	if( v28 == 0x9801 );// IntpErrUnk01( scr, 0x9801, v29 );	
    }
    if( !v35 ) return;
    if( v15 >= 0xA001 ){
        if( v15 <= 0xA001 ){
            v30 = v35;
            v35[0] = 2;
            v30[1] = v34;
        } else {
            if( v15 != 0xC001 ) return;
            v30 = v35;
            v35[0] = 3;
            v30[1] = v34;
        }        
        return;
    }
    if( v15 == 0x9001 ){
        v35[0] = 1;
DD
//        v35[1] = (v34 + scr->p12 + 4);
    }
}

/* not used function 
void SciUnk09( Intp_t *scr, int a2, int a3, char *ebx0 )
{
    int k; // ebx
    int v7; // eax
    const char *v8; // ecx
    Intp_t *scp; // edi
    int v10; // ebx
    unsigned int v11; // esi
    int *v12; // ecx
    int v13; // eax
    int *v14; // ecx
    short v15; // dx
    int Bei; // eax
    char *String; // eax
    int v18; // eax
    int v19; // ebx
    unsigned int v20; // edi
    int *p_StackApos; // ecx
    int v22; // eax
    int *v23; // ecx
    short v24; // dx
    char stmp[256]; // [esp+0h] [ebp-320h] BYREF
    char a1[256]; // [esp+100h] [ebp-220h] BYREF
    char v27[256]; // [esp+200h] [ebp-120h] BYREF
    int nn; // [esp+300h] [ebp-20h] BYREF
    int a4; // [esp+304h] [ebp-1Ch] BYREF
    unsigned int v31; // [esp+30Ch] [ebp-14h]
    unsigned int v32; // [esp+310h] [ebp-10h]

    k = 24 * a2;
    if( (IntpReadBei(&scr->ProcTable->NameOfst + 6 * a2, 4) & 4) == 0 ){
        if( (char *)IntpReadBei((int *)((char *)&scr->ProcTable->NameOfst + k), 20) != ebx0 ){
            Bei = IntpReadBei((int *)((char *)&scr->ProcTable->NameOfst + k), 0);
            String = IntpGetString(scr, Bei);
            sprintf(stmp, "Wrong number of args to procedure %s\n", String);
            eprintf(stmp);
            IntpLog(stmp);
            return;
        }
        v18 = IntpReadBei((int *)((char *)&scr->ProcTable->NameOfst + k), 16);
        v19 = 20;
        SciUnk03(scr, v18, 20);        
        p_StackApos = &scr->StackApos;
        v32 = 8 * (int)ebx0 + a3;
        for( v20 = a3; v20 < v32; v20 += 8 ){
            v22 = *(int *)v20;
            if( *(int *)v20 >= 2u ){
    		if( *(int *)v20 <= 2u ){
        	    v19 = *(int *)(v20 + 4);
        	    IntpPushIntStack((char *)scr->StackA, p_StackApos, v19);
        	    IntpPushwA(scr, 0xA001);
    		} else {
        	    if( v22 == 3 ){
        		v19 = *(int *)(v20 + 4);
        		IntpPushIntStack((char *)scr->StackA, p_StackApos, v19);
        		IntpPushwA(scr, 0xC001);
        	    }
    		}
            } else {
        	if( v22 == 1 ){
            	    v19 = IntpAddString(scr, *(char **)(v20 + 4), v19);
            	    IntpPushIntStack((char *)scr->StackA, v23, v19);
            	    IntpPushwA(scr, 0x9801);
        	}
            }            
        }
        IntpPushIntStack((char *)scr->StackA, p_StackApos, (int)ebx0);
        IntpPushwA(scr, 0xC001);
        SciOpcodeExec(scr, 0);
        return;
    }
    v7 = IntpReadBei((int *)((char *)&scr->ProcTable->NameOfst + k), 0);
    scp = ExportGetProcedure((char *)scr->ProcVarNames + v7, (char **)&a4, (short *)&nn);
    if( !scp ){ sprintf(a1, "External procedure %s not found\n", v8);IntpLog(a1); return;}
    if( (char *)nn != ebx0 ){ sprintf( v27, "Wrong number of arguments given to %s\n", v8); IntpLog(v27); return; }
    v10 = a4;
    SciUnk05(scr, scp, a4, 28);
    v11 = a3;
    v12 = &scp->StackApos;
    v31 = 8 * (int)ebx0 + a3;
    if( a3 < v31 ){
        for( ;v11 < v31; v11 += 8 ){
            v13 = *(int *)v11;
            if( *(int *)v11 >= 2u ){
    		if( *(int *)v11 <= 2u ){
        	    v10 = *(int *)(v11 + 4);
        	    IntpPushIntStack((char *)scp->StackA, v12, v10);
        	    IntpPushwA(scp, 0xA001);
    		} else {
        	    if( v13 == 3 ){
        		v10 = *(int *)(v11 + 4);
        		IntpPushIntStack((char *)scp->StackA, v12, v10);
        		IntpPushwA(scp, 0xC001);
        	    }
    		}    		
            } else {
        	if( v13 == 1 ){
            	    v10 = IntpAddString(scp, *(char **)(v11 + 4), v10);
            	    IntpPushIntStack((char *)scp->StackA, v14, v10);
            	    IntpPushwA(scp, 0x9801);
        	}
            }            
        }
    }
    IntpPushIntStack((char *)scp->StackA, v12, (int)ebx0);
    IntpPushwA(scp, 0xC001);
    SciOpcodeExec(scp, 0);
}
*/

/* not used function 
void SciUnk10( Intp_t *scr, int a2, int a3, unsigned int ecx0 )
{
    int v5; // ebx
    int Bei; // eax
    const char *v7; // ecx
    Intp_t *Procedure; // edi
    Intp_t *v9; // esi
    unsigned int result; // eax
    int v11; // ebx
    unsigned int v12; // ecx
    int *p_StackApos; // edi
    int v14; // eax
    int v15; // eax
    short v16; // dx
    Intp_t *v17; // eax
    int v18; // eax
    char *String; // eax
    int v20; // eax
    int v21; // ebx
    unsigned int v22; // esi
    int *v23; // ecx
    int v24; // eax
    int *v25; // ecx
    short v26; // dx
    unsigned int v27; // ecx
    unsigned int v28; // ecx
    char v29[256]; // [esp+0h] [ebp-35Ch] BYREF
    char fmt[256]; // [esp+100h] [ebp-25Ch] BYREF
    char a1[256]; // [esp+200h] [ebp-15Ch] BYREF
    jmp_buf v32; // [esp+300h] [ebp-5Ch] BYREF
    short nn; // [esp+334h] [ebp-28h] BYREF
    char *a4; // [esp+338h] [ebp-24h] BYREF
    char *StackA; // [esp+33Ch] [ebp-20h]
    unsigned int v38; // [esp+348h] [ebp-14h]
    unsigned int v39; // [esp+34Ch] [ebp-10h]

    if( IntpReadBei( &scr->ProcTable[ a2 ].NameOfst, 4) & 0x04 ){
        Bei = IntpReadBei( &scr->ProcTable[ a2 ].NameOfst, 0 );
        Procedure = ExportGetProcedure( (char *)scr->ProcVarNames + Bei, &a4, &nn );
        if( !Procedure ){ sprintf( a1, "External procedure %s not found\n", v7 ); return IntpLog( a1 ); }
        if( nn != ecx0 ){ sprintf( v29, "Wrong number of arguments given to %s\n", v7 ); return IntpLog( v29 ); }
        v11 = a4;
        SciUnk05( scr, Procedure, a4, 32 );        
        p_StackApos = &Procedure->StackApos;
        v38 = a3 + 8 * ecx0;
        for( v12 = a3; v12 < v38; v12 += 8 ){
            v14 = *(int *)v12;
            if( *(int *)v12 >= 2 ){
        	if( *(int *)v12 <= 2 ){
        	    v11 = *(int *)(v12 + 4);
        	    IntpPushIntStack( (char *)Procedure->StackA, p_StackApos, v11 );
        	    IntpPushwA( Procedure, 0xA001 );
        	} else {
        	    if( v14 == 3 ){
        		v11 = *(int *)(v12 + 4);
        		IntpPushIntStack( (char *)Procedure->StackA, p_StackApos, v11 );
        		IntpPushwA( Procedure, 0xC001 );
        	    }
        	}        	    
            } else {
        	if( v14 == 1 ){
            	    v15 = IntpAddString(Procedure, *(char **)(v12 + 4), v11);
        	    StackA = Procedure->StackA;
            	    v11 = v15;
        	    IntpPushIntStack(StackA, p_StackApos, v15);
            	    IntpPushwA( Procedure, 0x9801 );
        	}
            }                
        }
        IntpPushIntStack( (char *)Procedure->StackA, p_StackApos, ecx0 );
        IntpPushwA( Procedure, 0xC001 );
    } else {
	if( IntpReadBei( &scr->ProcTable[ a2 ].NameOfst, 20) != ecx0 ){
    	    String = IntpGetString( scr, IntpReadBei( &scr->ProcTable[ a2 ].NameOfst, 0 ) );
    	    sprintf( fmt, "Wrong number of args to procedure %s\n", String );
    	    eprintf( fmt );
    	    return IntpLog( fmt );
	}
	v21 = 24;
	SciUnk03( scr, IntpReadBei((int *)((char *)&scr->ProcTable[ a2 ].NameOfst), 16), 24 );
	v23 = &scr->StackApos;
	v39 = a3 + 8 * ecx0;
	if( a3 < v39 ){
    	    for( v22 = a3; v22 < v39; v22 += 8 ){
        	if( *(int *)v22 >= 0x02 ){
    		    if( *(int *)v22 <= 2 ){
    			v21 = *(int *)(v22 + 4);
        		IntpPushIntStack((char *)scr->StackA, v23, v21 );
        		IntpPushwA( scr, 0xA001 );
    		    } else {
        		if( *(int *)v22 == 3 ){
        		    v21 = *(int *)(v22 + 4);
        		    IntpPushIntStack((char *)scr->StackA, v23, v21 );
        		    IntpPushwA( scr, 0xC001 );
        		}
    		    }                
        	} else {
        	    if( *(int *)v22 == 1 ){
            		v21 = IntpAddString(scr, *(char **)(v22 + 4), v21 );
            		IntpPushIntStack((char *)scr->StackA, v25, v21 );
            		IntpPushwA( scr, 0x9801 );
        	    }
        	}            
    	    }
	}
	IntpPushIntStack( scr->StackA, v23, ecx0 );
	IntpPushwA( scr, 0xC001 );
    }    
    memcpy( v32, scr->EnvSave, sizeof( jmp_buf ) );
    SciOpcodeExec( scr, -1 );
    memcpy( scr->EnvSave, v32, sizeof( jmp_buf ) );
}
*/

void SciUnk11( Intp_t *scr, int a2 )
{
    Intp_t *Procedure;
    short nn;
    int Bei, v5;
    char v11, a1[256], *v7, *a4;

    if( ( ( v11 = IntpReadBei( (char *)&scr->ProcTable[ a2 ].NameOfst, 4 ) ) & 0x04 ) == 0 ){ // index 24* vs 28*
        Bei = IntpReadBei( (char *)&scr->ProcTable[ a2 ].NameOfst, 16 );
        SciUnk03( scr, Bei, 20 );
        IntpPushiA( scr, 0 );
        IntpPushwA( scr, 0xC001 );
        if( (v11 & 0x10) == 0 ) return;
        scr->Flags |= 0x80;
        SciOpcodeExec( scr, 0 );
        return;
    }
    v5 = IntpReadBei( (char *)&scr->ProcTable[ a2 ].NameOfst, 0 );
    v7 = scr->ProcVarNames + v5;
    Procedure = ExportGetProcedure( v7, &a4, &nn );
    if( !Procedure ){ sprintf( a1, "External procedure %s not found\n", v7 ); IntpLog( a1 ); return; }
    if( nn ){ IntpLog( "External procedure cannot take arguments in interrupt context" ); return; }
    SciUnk05( scr, Procedure, a4, 28 );
    IntpPushiA( Procedure, 0 );
    IntpPushwA( Procedure, 0xC001 );
    if( (IntpReadBei( (char *)&Procedure->ProcTable[ a2 ].NameOfst, 4 ) & 0x10 ) ){
        Procedure->Flags |= 0x80;
        SciOpcodeExec( Procedure, 0 );
    }
}

int SciLookupProcedure( Intp_t *itp, const char *ProcName )
{
    int i, cnt, idx;

    cnt = IntpReadBei( (char *)itp->ProcTable, 0 );
    for( i = 0; i < cnt; i++ ){
        idx = IntpReadBei( (char *)itp->ProcTable + i*24 + 4, 0 );
        if( !strcasecmp( ProcName, itp ? &itp->ProcVarNames[ idx ] : "" ) ) return i;
    }
    return -1;
}

void SciRunProcedure( Intp_t *scr, int ProcIdx )
{
    SCP_DBG_VAR;
    char stmp[256], *a4, *procname;
    int ofs, tmp[ 13 ];
    short IntCtx;
    Intp_t *p;

    SCP_DBGP( "Run script procedure: %x\n", ProcIdx );
    SCP_DBGP( "Procedure name: '%s'\n", gScptP_proc[ ProcIdx ] );
    if( IntpReadBei( (char *)&scr->ProcTable->Flags + 24 * ProcIdx, 0 ) & INTP_P_IMPORT ){ // Imported procedure
        ofs = IntpReadBei( (char *)&scr->ProcTable->NameOfst + 24 * ProcIdx, 0 );
        procname = scr->ProcVarNames + ofs;
printf("import procedure '%s'\n", procname );
        p = ExportGetProcedure( procname, &a4, &IntCtx );
        if( !p ){ sprintf( stmp, "External procedure %s not found\n", procname ); IntpLog( stmp ); return; }
        if( IntCtx ){ IntpLog( "External procedure cannot take arguments in interrupt context" ); return; }
        SciUnk05( scr, p, a4, 32 );
        IntpPushiA( p, 0 );
        IntpPushwA( p, SCR_INT );
    } else { // 
        SciUnk03( scr, IntpReadBei( (char *)&scr->ProcTable->BodyOfst + 24 * ProcIdx, 0 ), 24 );
        IntpPushiA( scr, 0 );
        IntpPushwA( scr, SCR_INT );
        p = scr;
    }
    memcpy( tmp, p->EnvSave, sizeof( tmp ) );
    SciOpcodeExec( p, -1 );
    memcpy( scr->EnvSave, tmp, sizeof( tmp ) );
}

void SciUnk14()
{
    unsigned int time;
    IntpList_t *p;
    Intp_t *Itp;
    short v8;
    char v2;
    jmp_buf tmp;
    int *v1,InstrPtr,v9,SaveFlags,i,cnt;

    if( gSciUnk01 ) return;
    time = 1000 * (unsigned int)gIntpGetTime() / gIntpTimeDiv;
    for( p = gIntpQe; p; p = p->Prev ){
        v1 = &p->Itp->ProcTable->Count + 1;
        cnt = IntpReadBei( (char *)p->Itp->ProcTable, 0 );
        for( i = 0; i < cnt; v1 += 6, i++ ){
            v2 = IntpReadBei( (char *)v1, 4 );
            if( v2 & 0x02 ){
                Itp = p->Itp;
                SaveFlags = p->Itp->Flags;
                InstrPtr = Itp->CodePC;
                memcpy( tmp, p->Itp->EnvSave, sizeof( jmp_buf ) );
                p->Itp->Flags = 0;
                p->Itp->CodePC = IntpReadBei( (char *)v1, 12 );
                SciOpcodeExec( p->Itp, -1 );
                if( !(p->Itp->Flags & 0x04) ){
                    v8 = IntpPopShortStack( p->Itp->StackA, &p->Itp->StackApos );
                    v9 = IntpPopIntStack( p->Itp->StackA, &p->Itp->StackApos );
                    if( v8 == 0x9801 ) IntpStringDeRef( p->Itp, 0x9801, v9 );
                    p->Itp->Flags = SaveFlags;
                    p->Itp->CodePC = InstrPtr;
                    if( v9 ){
                        IntpWriteBew( 0, (char *)v1, 4 );
                        IntpWriteBew( 0, (char *)v1, 6 );
                        SciUnk11( p->Itp, i );
                    }
                }
                memcpy( p->Itp->EnvSave, tmp, sizeof( jmp_buf ) );
            } else if( v2 & 0x01 ){
                if( IntpReadBei( (char *)v1, 8 ) < time ){
                    IntpWriteBew( 0, (char *)v1, 4 );
                    IntpWriteBew( 0, (char *)v1, 6 );
                    SciUnk11( p->Itp, i );
                }
            }                    
        }
    }
}

void SciUnk15( IntpList_t *a1 )
{
    IntpList_t *Prev, *Next;

    if( (Prev = a1->Prev) ) Prev->Next = a1->Next;    
    if( (Next = a1->Next) )
	Next->Prev = a1->Prev;
    else
        gIntpQe = a1->Prev;
    IntpUnLoad( a1->Itp );
    dbg_free( a1 );
}

void SciItpEqAdd( Intp_t *itp )
{
    IntpList_t *p;

    itp->Flags |= 0x02;
    p = dbg_malloc( sizeof( IntpList_t ) );
    p->Next = NULL;
    p->Itp = itp;
    p->Prev = gIntpQe;
    if( gIntpQe ) gIntpQe->Next = p;
    gIntpQe = p;
}

void SciUnk17()
{
    Intp_t *itp;
    
    if( !(itp = IntpLoad( gSciMseHandler() )) ) return;
    SciItpEqAdd( itp );
    SciOpcodeExec( itp, 24 );
}

void SciUnk18( int val )
{
    if( val < 1 ) val = 1;
    gSciUnk04 = val;
}

void SciUnk19()
{
    IntpList_t *Prev, *p;
    
    for( p = gIntpQe; p; p = Prev ){
        Prev = p->Prev;
        if( p->Itp ) SciOpcodeExec( p->Itp, gSciUnk04 );
        if( p->Itp->i35 ) SciUnk15( p );            
    }
    SciUnk14( Prev );
    ScpIfcNevsUpdate();
}

void SciUnk20()
{
    IntpList_t *p, *prev;
    
    for( p = gIntpQe; p; p = prev ){
    	prev = p->Prev;
        SciUnk15( p );        
    }        
}

void SciAddOpcode( short Opcode, void (*Func)( Intp_t * ) )
{
    Opcode &= 0x03ff;
    if( Opcode >= MAX_OPCODES ){ printf( "Too many opcodes!\n" ); exit( 1 ); }
    gScpOpcodeTable[ Opcode ] = (void *)Func;
}

void SciSetMseHandler( char *(*Cb)(void) )
{
    gSciMseHandler = Cb;
}

void SciTimeUpdate()
{
    if( ++gSciUnk01 == 1 ) gSciUnk02 = (gIntpGetTime)();
}

void SciUnk25()
{
    int i, cnt, *addr, dat;
    IntpList_t *p;
    unsigned int time;

    if( gSciUnk01 == 0 ) return;
    gSciUnk01--;
    if( gSciUnk01 != 0 ) return;
    time = 1000 * ((unsigned int)gIntpGetTime() - gSciUnk02) / gIntpTimeDiv;
    addr = (int *)gIntpQe->Itp->ProcTable;
    for( p = gIntpQe; p; p = p->Prev ){                
        cnt = IntpReadBei( (char *)p->Itp->ProcTable, 0 );
        addr++;
        for( i = 0; i < cnt; i++, addr += 6 ){
            if( !(IntpReadBei( (char *)addr, 4 ) & 0x01 ) ) continue; // flags
            dat = time + IntpReadBei( (char *)addr,  8 ); // delay
            IntpWriteBew( dat >> 16, (char *)addr, 8 ); 
            IntpWriteBew( dat & 0xffff, (char *)addr, 10 );
        }
    }
}

int SciUnk26()
{
    return 0;
}

void SciUnk27( char *fpath, Intp_t *a2, int cnt )
{
/*
    FILE *fh;
    char *v8, *v6;
    int Val;
    unsigned short Type;

    fh = fopen( fpath, "w" );
    v6 = &a2->StackA[ a2->StackApos - 6 ];
    if( !fh ) return;    
    v8 = &v6[ 1 ];
    while( --cnt != -1 ){
        Val = IntpReadBei( v6, 0 );
        v8 -= 6;
        Type = (v8[0] << 8) | v8[1];
	switch( Type ){
	    case SCR_STRING: fprintf( fh, "%s\n", &a2->StringsConst[ Val + 4 ] ); break;
	    case SCR_FSTRING: 
        	    if( Type & 0x800 )
            		fprintf( fh, "%s\n", &a2->Strings->Data[ Val ] );
        	    else
            		fprintf( fh, "%s\n", &a2->StringsConst[ Val + 4 ] );
            	    break;	    
	    case SCR_FLOAT: fprintf( fh, "%f\n", FLOAT( Val ) ); break;
	    case SCR_INT: fprintf( fh, "%d\n", Val ); break;
	    default: fprintf( fh, "Unknown\n" );
	}
    }
    fclose( fh );
*/
}

void SciUnk28()
{
/*
    IntpList_t *p;
    IntpOp_t *Strings;
    IntpString_t *str;
    int heap;

    for( p = gIntpQe; p; p = p->Prev ){
        if( !p->Itp ) continue;
    	Strings = p->Itp->Strings;
    	if( !Strings ){
            eprintf( "No string heap for program %s\n", p->Itp->FileName );
    	} else {    	        	    
    	    eprintf( "Program %s\n", p->Itp->FileName );
    	    for( str = (IntpString_t *)Strings->Data, heap = 0; str->Size != 0x8000; str += str->Size + sizeof( IntpString_t ), heap += str->Size + sizeof( IntpString_t ) ){
        	if( str->Size < 0 )
        	    eprintf( "Free space, length %d\n", str->Size );        	
        	else
        	    eprintf( "Size: %d, ref: %d, string %s\n", str->Size, str->Ref, str->String );
    	    }
    	    eprintf( "Total length of heap %d, stored length %d\n", heap, p->Itp->Strings->w01 );
    	}
    }    
*/
}


