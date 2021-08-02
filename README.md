# sc4cpp
sc4cpp is a shellcode framework based on C++

## Compiler
Clang for Windows

## Compiler options
```
/O2 /Os /MT /GS- /Gs1048576 -mno-sse
```

## Example
```cpp
SC_NOINLINE
SC_CODESEG_REORDERING
DWORD WINAPI WorkerThread(PVOID lpAnsiMsg) {
    SC_IMPORT_API_BATCH_BEGIN();
    SC_IMPORT_API_BATCH("User32.dll", MessageBoxA);
    SC_IMPORT_API_BATCH("User32.dll", MessageBoxW);
    SC_IMPORT_API_BATCH_END();

    MessageBoxA(NULL, (PCSTR)lpAnsiMsg, SC_PISTRINGA("Hello!"), MB_OK);
    MessageBoxW(NULL, SC_PISTRINGW(L"Hello Unicode!"), SC_PISTRINGW(L"Hello!"), MB_OK);

    return 0;
}

SC_MAIN_BEGIN()
{
    SC_IMPORT_API_BATCH_BEGIN();
    SC_IMPORT_API_BATCH("Kernel32.dll", CreateThread);
    SC_IMPORT_API_BATCH("Kernel32.dll", WaitForSingleObject);
    SC_IMPORT_API_BATCH("Kernel32.dll", ExitProcess);
    SC_IMPORT_API_BATCH_END();

    HANDLE hWorker =
        CreateThread(NULL, 0, SC_PIFUNCTION(WorkerThread), SC_PISTRINGA("Hello Ansi!"), 0, NULL);
    WaitForSingleObject(hWorker, INFINITE);

    ExitProcess(0);
}
SC_MAIN_END()
```

## Example assembly output
```asm
; Segment type: Pure code
seg000          segment byte public 'CODE' use32
                assume cs:seg000
                assume es:nothing, ss:nothing, ds:nothing, fs:nothing, gs:nothing
                call    $+5
                pop     eax
                lea     eax, [eax-5]
                lea     ecx, ds:405000h
                neg     ecx
                lea     eax, [eax+ecx+40501Ch]
                push    eax
                call    eax
                retn
; ---------------------------------------------------------------------------
                push    ebp
                push    ebx
                push    edi
                push    esi
                sub     esp, 38h
                mov     eax, dword ptr fs:loc_2F+1
                mov     eax, [eax+0Ch]
                mov     eax, [eax+14h]

loc_2F:                                 ; DATA XREF: seg000:00000023↑r
                                        ; seg000:loc_10D↓r ...
                mov     ecx, eax

loc_31:                                 ; CODE XREF: seg000:000000FE↓j
                mov     edi, [ecx+10h]
                mov     ecx, [ecx]
                test    edi, edi
                jz      loc_FC
                mov     edx, [edi+3Ch]
                mov     edx, [edi+edx+78h]
                test    edx, edx
                jz      loc_FC
                mov     esi, [edi+edx+0Ch]
                mov     bl, [edi+esi]
                test    bl, bl
                jz      loc_FC
                mov     [esp+4], edi
                lea     esi, [edi+esi+1]
                mov     edi, 811C9DC5h

loc_69:                                 ; CODE XREF: seg000:0000008D↓j
                mov     bh, bl
                movzx   ebp, bl
                add     bl, 20h ; ' '
                add     bh, 0BFh
                cmp     bh, 1Ah
                movzx   ebx, bl
                cmovnb  ebx, ebp
                movzx   ebx, bl
                xor     ebx, edi
                imul    edi, ebx, 1000193h
                mov     bl, [esi]
                inc     esi
                test    bl, bl
                jnz     short loc_69
                mov     esi, [esp+4]
                cmp     edi, 0A3E6F6C3h
                jnz     short loc_FC
                mov     ebx, [esi+edx+18h]
                test    ebx, ebx
                jz      short loc_FC
                mov     ebp, [esi+edx+20h]
                lea     edi, [esi+1]
                mov     [esp], ebx
                mov     [esp+0Ch], edi
                xor     edi, edi
                add     ebp, esi
                mov     [esp+8], ebp

loc_B9:                                 ; CODE XREF: seg000:000000FA↓j
                mov     ebp, [ebp+edi*4+0]
                mov     bl, [esi+ebp]
                test    bl, bl
                jz      short loc_EC
                add     ebp, [esp+0Ch]
                mov     esi, 811C9DC5h

loc_CD:                                 ; CODE XREF: seg000:000000DE↓j
                movzx   ebx, bl
                xor     ebx, esi
                imul    esi, ebx, 1000193h
                mov     bl, [ebp+0]
                inc     ebp
                test    bl, bl
                jnz     short loc_CD
                cmp     esi, 53B2070Fh
                jz      loc_2F9

loc_EC:                                 ; CODE XREF: seg000:000000C2↑j
                mov     ebx, [esp]
                mov     esi, [esp+4]
                mov     ebp, [esp+8]
                inc     edi
                cmp     edi, ebx
                jnz     short loc_B9

loc_FC:                                 ; CODE XREF: seg000:00000038↑j
                                        ; seg000:00000047↑j ...
                cmp     ecx, eax
                jnz     loc_31
                int     3               ; Trap to Debugger
                mov     dword ptr [esp+4], 0

loc_10D:                                ; CODE XREF: seg000:00000314↓j
                mov     eax, dword ptr fs:loc_2F+1
                mov     eax, [eax+0Ch]
                mov     eax, [eax+14h]
                mov     ecx, eax

loc_11B:                                ; CODE XREF: seg000:000001E7↓j
                mov     ebp, [ecx+10h]
                mov     ecx, [ecx]
                test    ebp, ebp
                jz      loc_1E5
                mov     edx, [ebp+3Ch]
                mov     edx, [ebp+edx+78h]
                test    edx, edx
                jz      loc_1E5
                mov     esi, [ebp+edx+0Ch]
                mov     bl, [ebp+esi+0]
                test    bl, bl
                jz      loc_1E5
                lea     esi, [ebp+esi+1]
                mov     edi, 811C9DC5h
                mov     [esp], edx

loc_153:                                ; CODE XREF: seg000:00000177↓j
                mov     bh, bl
                movzx   edx, bl
                add     bl, 20h ; ' '
                add     bh, 0BFh
                cmp     bh, 1Ah
                movzx   ebx, bl
                cmovnb  ebx, edx
                movzx   edx, bl
                mov     bl, [esi]
                inc     esi
                xor     edx, edi
                imul    edi, edx, 1000193h
                test    bl, bl
                jnz     short loc_153
                mov     edx, [esp]
                cmp     edi, 0A3E6F6C3h
                jnz     short loc_1E5
                mov     esi, [ebp+edx+18h]
                test    esi, esi
                jz      short loc_1E5
                mov     edx, [ebp+edx+20h]
                lea     edi, [ebp+1]
                mov     [esp+0Ch], edi
                xor     edi, edi
                add     edx, ebp
                mov     [esp+8], edx

loc_19F:                                ; CODE XREF: seg000:000001E3↓j
                mov     ebx, [edx+edi*4]
                mov     dl, [ebp+ebx+0]
                test    dl, dl
                jz      short loc_1DC
                add     ebx, [esp+0Ch]
                mov     [esp+10h], esi
                mov     esi, 811C9DC5h

loc_1B7:                                ; CODE XREF: seg000:000001C7↓j
                movzx   edx, dl
                xor     edx, esi
                imul    esi, edx, 1000193h
                mov     dl, [ebx]
                inc     ebx
                test    dl, dl
                jnz     short loc_1B7
                cmp     esi, 0F8F45725h
                mov     edx, [esp]
                mov     esi, [esp+10h]
                jz      loc_319

loc_1DC:                                ; CODE XREF: seg000:000001A8↑j
                mov     edx, [esp+8]
                inc     edi
                cmp     edi, esi
                jnz     short loc_19F

loc_1E5:                                ; CODE XREF: seg000:00000122↑j
                                        ; seg000:00000131↑j ...
                cmp     ecx, eax
                jnz     loc_11B
                int     3               ; Trap to Debugger
                xor     ebp, ebp

loc_1F0:                                ; CODE XREF: seg000:0000032C↓j
                lea     esi, [esp+14h]
                mov     ecx, 6E72654Bh
                lea     edi, [esp+24h]
                xor     eax, eax
                mov     edx, 6C6C642Eh
                mov     [esi], ecx
                mov     ecx, 32336C65h
                mov     dword ptr [edi], 61657243h
                mov     dword ptr [edi+4], 68546574h
                mov     dword ptr [edi+8], 64616572h
                mov     [edi+0Ch], al
                mov     [esi+4], ecx
                mov     [esi+8], edx
                mov     [esi+0Ch], al
                push    esi
                mov     ebx, [esp+8]
                call    ebx
                push    edi
                push    eax
                call    ebp
                mov     [esp], eax
                mov     eax, 6E72654Bh
                mov     dword ptr [edi], 74696157h
                mov     dword ptr [edi+4], 53726F46h
                mov     dword ptr [edi+8], 6C676E69h
                mov     dword ptr [edi+0Ch], 6A624F65h
                mov     dword ptr [edi+10h], 746365h
                mov     [esi], eax
                mov     eax, 32336C65h
                mov     [esi+4], eax
                mov     eax, 6C6C642Eh
                mov     [esi+8], eax
                xor     eax, eax
                mov     [esi+0Ch], al
                push    esi
                call    ebx
                push    edi
                push    eax
                call    ebp
                mov     [esp+8], eax
                mov     eax, 6E72654Bh
                mov     dword ptr [esi], 74697845h
                mov     dword ptr [esi+4], 636F7250h
                mov     dword ptr [esi+8], 737365h
                mov     [edi], eax
                mov     eax, 32336C65h
                mov     [edi+4], eax
                mov     eax, 6C6C642Eh
                mov     [edi+8], eax
                xor     eax, eax
                mov     [edi+0Ch], al
                push    edi
                call    ebx
                push    esi
                push    eax
                call    ebp
                mov     ebp, eax
                mov     eax, 40501Ch
                mov     ecx, 405331h
                mov     dword ptr [edi], 6C6C6548h
                mov     dword ptr [edi+4], 6E41206Fh
                mov     dword ptr [edi+8], 216973h
                sub     ecx, eax
                mov     eax, [esp+4Ch]
                add     eax, ecx
                xor     ecx, ecx
                push    ecx
                push    ecx
                push    edi
                push    eax
                push    ecx
                push    ecx
                xor     esi, esi
                call    dword ptr [esp+18h]
                push    0FFFFFFFFh
                push    eax
                call    dword ptr [esp+10h]
                push    esi
                call    ebp

loc_2F9:                                ; CODE XREF: seg000:000000E6↑j
                mov     esi, [esp+4]
                mov     eax, [esi+edx+24h]
                mov     ecx, [esi+edx+1Ch]
                add     eax, esi
                add     ecx, esi
                movzx   eax, word ptr [eax+edi*2]
                add     esi, [ecx+eax*4]
                mov     [esp+4], esi
                jmp     loc_10D
; ---------------------------------------------------------------------------

loc_319:                                ; CODE XREF: seg000:000001D6↑j
                mov     eax, [ebp+edx+24h]
                mov     ecx, [ebp+edx+1Ch]
                add     eax, ebp
                add     ecx, ebp
                movzx   eax, word ptr [eax+edi*2]
                add     ebp, [ecx+eax*4]
                jmp     loc_1F0
; ---------------------------------------------------------------------------
                push    ebp
                push    ebx
                push    edi
                push    esi
                sub     esp, 44h
                mov     eax, dword ptr fs:loc_2F+1
                mov     eax, [eax+0Ch]
                mov     ecx, [eax+14h]
                mov     [esp+4], ecx

loc_348:                                ; CODE XREF: seg000:00000412↓j
                mov     edx, [ecx+10h]
                mov     ecx, [ecx]
                test    edx, edx
                jz      loc_40E
                mov     eax, [edx+3Ch]
                mov     esi, [edx+eax+78h]
                test    esi, esi
                mov     eax, esi
                mov     [esp], esi
                jz      loc_40E
                mov     eax, [esp]
                mov     eax, [edx+eax+0Ch]
                mov     bl, [edx+eax]
                test    bl, bl
                jz      loc_40E
                lea     eax, [edx+eax+1]
                mov     edi, 811C9DC5h
                mov     [esp+8], edx

loc_388:                                ; CODE XREF: seg000:000003AC↓j
                mov     edx, ebx
                movzx   esi, bl
                add     bl, 20h ; ' '
                add     dl, 0BFh
                cmp     dl, 1Ah
                movzx   edx, bl
                mov     bl, [eax]
                cmovnb  edx, esi
                inc     eax
                movzx   edx, dl
                xor     edx, edi
                imul    edi, edx, 1000193h
                test    bl, bl
                jnz     short loc_388
                mov     esi, [esp+8]
                cmp     edi, 0A3E6F6C3h
                jnz     short loc_40E
                mov     eax, [esp]
                mov     edi, [esi+eax+18h]
                test    edi, edi
                jz      short loc_40E
                mov     eax, [esp]
                xor     ebx, ebx
                mov     ebp, [esi+eax+20h]
                lea     eax, [esi+1]
                mov     [esp+0Ch], eax
                add     ebp, esi

loc_3D7:                                ; CODE XREF: seg000:0000040C↓j
                mov     edx, [ebp+ebx*4+0]
                mov     al, [esi+edx]
                test    al, al
                jz      short loc_409
                add     edx, [esp+0Ch]
                mov     esi, 811C9DC5h

loc_3EB:                                ; CODE XREF: seg000:000003FB↓j
                movzx   eax, al
                xor     eax, esi
                imul    esi, eax, 1000193h
                mov     al, [edx]
                inc     edx
                test    al, al
                jnz     short loc_3EB
                cmp     esi, 53B2070Fh
                mov     esi, [esp+8]
                jz      short loc_41D

loc_409:                                ; CODE XREF: seg000:000003E0↑j
                inc     ebx
                cmp     ebx, edi
                jnz     short loc_3D7

loc_40E:                                ; CODE XREF: seg000:0000034F↑j
                                        ; seg000:00000363↑j ...
                cmp     ecx, [esp+4]
                jnz     loc_348
                int     3               ; Trap to Debugger
                xor     esi, esi
                jmp     short loc_433
; ---------------------------------------------------------------------------

loc_41D:                                ; CODE XREF: seg000:00000407↑j
                mov     ecx, [esp]
                mov     eax, [esi+ecx+24h]
                mov     ecx, [esi+ecx+1Ch]
                add     eax, esi
                add     ecx, esi
                movzx   eax, word ptr [eax+ebx*2]
                add     esi, [ecx+eax*4]

loc_433:                                ; CODE XREF: seg000:0000041B↑j
                mov     [esp+8], esi
                mov     eax, dword ptr fs:loc_2F+1
                mov     eax, [eax+0Ch]
                mov     eax, [eax+14h]
                mov     [esp+0Ch], eax

loc_447:                                ; CODE XREF: seg000:00000510↓j
                mov     ebp, [eax+10h]
                mov     eax, [eax]
                test    ebp, ebp
                mov     [esp], eax
                jz      loc_509
                mov     eax, [ebp+3Ch]
                mov     ecx, [ebp+eax+78h]
                test    ecx, ecx
                mov     eax, ecx
                mov     [esp+4], ecx
                jz      loc_509
                mov     eax, [esp+4]
                mov     eax, [ebp+eax+0Ch]
                mov     bl, [ebp+eax+0]
                test    bl, bl
                jz      loc_509
                lea     eax, [ebp+eax+1]
                mov     edx, 811C9DC5h

loc_489:                                ; CODE XREF: seg000:000004AD↓j
                mov     ecx, ebx
                movzx   esi, bl
                add     bl, 20h ; ' '
                add     cl, 0BFh
                cmp     cl, 1Ah
                movzx   ecx, bl
                mov     bl, [eax]
                cmovnb  ecx, esi
                inc     eax
                movzx   ecx, cl
                xor     ecx, edx
                imul    edx, ecx, 1000193h
                test    bl, bl
                jnz     short loc_489
                cmp     edx, 0A3E6F6C3h
                jnz     short loc_509
                mov     eax, [esp+4]
                mov     edi, [ebp+eax+18h]
                test    edi, edi
                jz      short loc_509
                mov     eax, [esp+4]
                lea     ecx, [ebp+1]
                xor     ebx, ebx
                mov     [esp+10h], ecx
                mov     eax, [ebp+eax+20h]
                add     eax, ebp

loc_4D6:                                ; CODE XREF: seg000:00000507↓j
                mov     edx, [eax+ebx*4]
                mov     cl, [ebp+edx+0]
                test    cl, cl
                jz      short loc_504
                add     edx, [esp+10h]
                mov     esi, 811C9DC5h

loc_4EA:                                ; CODE XREF: seg000:000004FA↓j
                movzx   ecx, cl
                xor     ecx, esi
                imul    esi, ecx, 1000193h
                mov     cl, [edx]
                inc     edx
                test    cl, cl
                jnz     short loc_4EA
                cmp     esi, 0F8F45725h
                jz      short loc_51B

loc_504:                                ; CODE XREF: seg000:000004DF↑j
                inc     ebx
                cmp     ebx, edi
                jnz     short loc_4D6

loc_509:                                ; CODE XREF: seg000:00000451↑j
                                        ; seg000:00000466↑j ...
                mov     eax, [esp]
                cmp     eax, [esp+0Ch]
                jnz     loc_447
                int     3               ; Trap to Debugger
                xor     ebp, ebp
                jmp     short loc_532
; ---------------------------------------------------------------------------

loc_51B:                                ; CODE XREF: seg000:00000502↑j
                mov     ecx, [esp+4]
                mov     eax, [ebp+ecx+24h]
                mov     ecx, [ebp+ecx+1Ch]
                add     eax, ebp
                add     ecx, ebp
                movzx   eax, word ptr [eax+ebx*2]
                add     ebp, [ecx+eax*4]

loc_532:                                ; CODE XREF: seg000:00000519↑j
                mov     eax, 7373654Dh
                lea     ebx, [esp+24h]
                lea     edi, [esp+14h]
                mov     [ebx], eax
                mov     eax, 42656761h
                mov     [ebx+4], eax
                mov     eax, 72657355h
                mov     dword ptr [ebx+8], 41786Fh
                mov     [edi], eax
                mov     eax, 642E3233h
                mov     [edi+4], eax
                mov     ax, 6C6Ch
                mov     [edi+8], ax
                xor     eax, eax
                mov     [edi+0Ah], al
                push    edi
                mov     esi, [esp+0Ch]
                call    esi
                push    ebx
                push    eax
                call    ebp
                mov     [esp], eax
                mov     eax, 7373654Dh
                mov     [ebx], eax
                mov     eax, 42656761h
                mov     [ebx+4], eax
                mov     eax, 72657355h
                mov     dword ptr [ebx+8], 57786Fh
                mov     [edi], eax
                mov     eax, 642E3233h
                mov     [edi+4], eax
                mov     ax, 6C6Ch
                mov     [edi+8], ax
                xor     eax, eax
                mov     [edi+0Ah], al
                push    edi
                call    esi
                push    ebx
                push    eax
                call    ebp
                mov     esi, eax
                xor     eax, eax
                mov     dword ptr [ebx], 6C6C6548h
                mov     word ptr [ebx+4], 216Fh
                mov     [ebx+6], al
                push    eax
                push    ebx
                push    dword ptr [esp+60h]
                push    eax
                xor     ebp, ebp
                call    dword ptr [esp+10h]
                mov     eax, 650048h
                mov     ecx, 6C006Ch
                mov     [edi], eax
                mov     [edi+4], ecx
                mov     dword ptr [edi+8], 21006Fh
                mov     [ebx], eax
                mov     [ebx+4], ecx
                mov     dword ptr [ebx+8], 20006Fh
                mov     dword ptr [ebx+0Ch], 6E0055h
                mov     dword ptr [ebx+10h], 630069h
                mov     dword ptr [ebx+14h], 64006Fh
                mov     dword ptr [ebx+18h], 210065h
                mov     [edi+0Ch], bp
                mov     [ebx+1Ch], bp
                push    ebp
                push    edi
                push    ebx
                push    ebp
                call    esi
                xor     eax, eax
                add     esp, 44h
                pop     esi
                pop     edi
                pop     ebx
                pop     ebp
                retn    4
seg000          ends
```
