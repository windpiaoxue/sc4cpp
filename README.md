# sc4cpp
sc4cpp is a shellcode framework based on C++

## Compiler
Clang for Windows

## Compiler options
```
/O2 /MT /GS- /Gs1048576 -mno-sse
```

## Example
```cpp
SC_MAIN_BEGIN()
{
    // *** Place function declaration here! ***
    UINT MyFunc(VOID);

    SC_IMPORT_API_BATCH_BEGIN();
    SC_IMPORT_API_BATCH("User32.dll", MessageBoxA);
    SC_IMPORT_API_BATCH("User32.dll", MessageBoxW);
    SC_IMPORT_API_BATCH("Kernel32.dll", ExitProcess);
    SC_IMPORT_API_BATCH_END();

    // *** Place code here! ***
    MessageBoxA(NULL, SC_PISTRINGA("Hello Ansi!"), SC_PISTRINGA("Hello!"), MB_OK);
    MessageBoxW(NULL, SC_PISTRINGW(L"Hello Unicode!"), SC_PISTRINGW(L"Hello!"), MB_OK);

    ExitProcess(MyFunc());
}

// *** Place function definition here! ***
SC_NOINLINE UINT MyFunc(VOID) { return 0; }

SC_MAIN_END()
```

## Example assembly output
```asm
sub_0           proc near

var_5C          = dword ptr -5Ch
var_58          = dword ptr -58h
var_54          = byte ptr -54h
var_48          = dword ptr -48h
var_44          = dword ptr -44h
var_40          = dword ptr -40h
var_3C          = dword ptr -3Ch
var_38          = dword ptr -38h
var_34          = byte ptr -34h
var_24          = dword ptr -24h
var_20          = dword ptr -20h
var_1C          = dword ptr -1Ch
var_18          = word ptr -18h
var_14          = dword ptr -14h

                push    ebp
                push    ebx
                push    edi
                push    esi
                sub     esp, 4Ch
                mov     eax, dword ptr fs:loc_2B+5
                mov     eax, [eax+0Ch]
                mov     eax, [eax+14h]
                mov     ecx, eax

loc_15:                                 ; CODE XREF: sub_0+112↓j
                mov     ebx, [ecx+10h]
                mov     ecx, [ecx]
                test    ebx, ebx
                jz      loc_110
                mov     edx, [ebx+3Ch]
                mov     edi, [ebx+edx+78h]
                test    edi, edi

loc_2B:                                 ; DATA XREF: sub_0+7↑r
                                        ; sub_0:loc_11B↓r
                jz      loc_110
                mov     esi, [ebx+edi+0Ch]
                mov     dl, [ebx+esi]
                test    dl, dl
                jz      loc_110
                mov     [esp+5Ch+var_58], edi
                lea     esi, [ebx+esi+1]
                mov     edi, 811C9DC5h

loc_4D:                                 ; CODE XREF: sub_0+72↓j
                mov     dh, dl
                movzx   ebp, dl
                add     dl, 20h ; ' '
                add     dh, 0BFh
                cmp     dh, 1Ah
                movzx   edx, dl
                cmovnb  edx, ebp
                movzx   edx, dl
                xor     edx, edi
                imul    edi, edx, 1000193h
                movzx   edx, byte ptr [esi]
                inc     esi
                test    dl, dl
                jnz     short loc_4D
                mov     edx, [esp+5Ch+var_58]
                cmp     edi, 0A3E6F6C3h
                jnz     loc_110
                mov     esi, [ebx+edx+18h]
                test    esi, esi
                jz      loc_110
                mov     ebp, [ebx+edx+20h]
                lea     edx, [ebx+1]
                mov     [esp+5Ch+var_44], eax
                mov     [esp+5Ch+var_38], edx
                xor     edx, edx
                add     ebp, ebx

loc_A3:                                 ; CODE XREF: sub_0+104↓j
                mov     edi, [ebp+edx*4+0]
                mov     al, [ebx+edi]
                mov     byte ptr [esp+5Ch+var_5C], al
                cmp     byte ptr [esp+5Ch+var_5C], 0
                jz      short loc_FD
                add     edi, [esp+5Ch+var_38]
                mov     [esp+5Ch+var_3C], esi
                mov     esi, 811C9DC5h
                mov     [esp+5Ch+var_14], edx
                mov     [esp+5Ch+var_40], ebp

loc_C8:                                 ; CODE XREF: sub_0+DF↓j
                movzx   ebp, byte ptr [esp+5Ch+var_5C]
                movzx   edx, byte ptr [edi]
                inc     edi
                xor     ebp, esi
                mov     byte ptr [esp+5Ch+var_5C], dl
                imul    esi, ebp, 1000193h
                cmp     byte ptr [esp+5Ch+var_5C], 0
                jnz     short loc_C8
                cmp     esi, 53B2070Fh
                mov     edi, [esp+5Ch+var_58]
                mov     esi, [esp+5Ch+var_3C]
                mov     ebp, [esp+5Ch+var_40]
                mov     edx, [esp+5Ch+var_14]
                jz      loc_380

loc_FD:                                 ; CODE XREF: sub_0+B1↑j
                mov     eax, [esp+5Ch+var_44]
                inc     edx
                cmp     edx, esi
                jnz     short loc_A3
                nop     word ptr cs:[eax+eax+00000000h]

loc_110:                                ; CODE XREF: sub_0+1C↑j
                                        ; sub_0:loc_2B↑j ...
                cmp     ecx, eax
                jnz     loc_15
                int     3               ; Trap to Debugger
                xor     ebx, ebx

loc_11B:                                ; CODE XREF: sub_0+393↓j
                mov     eax, dword ptr fs:loc_2B+5
                mov     eax, [eax+0Ch]
                mov     eax, [eax+14h]
                mov     ecx, eax

loc_129:                                ; CODE XREF: sub_0+212↓j
                mov     ebp, [ecx+10h]
                mov     ecx, [ecx]
                test    ebp, ebp
                jz      loc_210
                mov     edx, [ebp+3Ch]
                mov     edi, [ebp+edx+78h]
                test    edi, edi
                jz      loc_210
                mov     esi, [ebp+edi+0Ch]
                mov     dl, [ebp+esi+0]
                test    dl, dl
                jz      loc_210
                mov     [esp+5Ch+var_5C], edi
                lea     esi, [ebp+esi+1]
                mov     edi, 811C9DC5h
                mov     [esp+5Ch+var_58], eax

loc_165:                                ; CODE XREF: sub_0+18A↓j
                mov     dh, dl
                movzx   eax, dl
                add     dl, 20h ; ' '
                add     dh, 0BFh
                cmp     dh, 1Ah
                movzx   edx, dl
                cmovnb  edx, eax
                movzx   eax, dl
                movzx   edx, byte ptr [esi]
                inc     esi
                xor     eax, edi
                imul    edi, eax, 1000193h
                test    dl, dl
                jnz     short loc_165
                mov     eax, [esp+5Ch+var_58]
                mov     edx, [esp+5Ch+var_5C]
                cmp     edi, 0A3E6F6C3h
                jnz     short loc_210
                mov     esi, [ebp+edx+18h]
                test    esi, esi
                mov     [esp+5Ch+var_38], esi
                jz      short loc_210
                mov     edx, [ebp+edx+20h]
                lea     edi, [ebp+1]
                xor     esi, esi
                mov     [esp+5Ch+var_40], edi
                add     edx, ebp
                mov     [esp+5Ch+var_3C], edx

loc_1BA:                                ; CODE XREF: sub_0+209↓j
                mov     edi, [edx+esi*4]
                mov     dl, [ebp+edi+0]
                mov     byte ptr [esp+5Ch+var_44], dl
                cmp     byte ptr [esp+5Ch+var_44], 0
                jz      short loc_200
                add     edi, [esp+5Ch+var_40]
                mov     al, byte ptr [esp+5Ch+var_44]
                mov     edx, esi
                mov     esi, 811C9DC5h

loc_1DB:                                ; CODE XREF: sub_0+1EC↓j
                movzx   eax, al
                xor     eax, esi
                imul    esi, eax, 1000193h
                movzx   eax, byte ptr [edi]
                inc     edi
                test    al, al
                jnz     short loc_1DB
                mov     eax, [esp+5Ch+var_58]
                cmp     esi, 0F8F45725h
                mov     esi, edx
                jz      loc_398

loc_200:                                ; CODE XREF: sub_0+1CA↑j
                inc     esi
                mov     edx, [esp+5Ch+var_3C]
                cmp     esi, [esp+5Ch+var_38]
                jnz     short loc_1BA
                nop     dword ptr [eax+eax+00h]

loc_210:                                ; CODE XREF: sub_0+130↑j
                                        ; sub_0+13F↑j ...
                cmp     ecx, eax
                jnz     loc_129
                int     3               ; Trap to Debugger
                xor     ebp, ebp

loc_21B:                                ; CODE XREF: sub_0+3AE↓j
                lea     esi, [esp+5Ch+var_54]
                mov     dword ptr [esp+5Ch+var_34], 7373654Dh
                mov     dword ptr [esp+5Ch+var_34+4], 42656761h
                mov     dword ptr [esp+5Ch+var_54], 72657355h
                mov     dword ptr [esp+5Ch+var_54+4], 642E3233h
                mov     word ptr [esp+5Ch+var_54+8], 6C6Ch
                mov     dword ptr [esp+5Ch+var_34+8], 41786Fh
                mov     [esp+5Ch+var_54+0Ah], 0
                push    esi
                call    ebx
                lea     edi, [esp+5Ch+var_34]
                push    edi
                push    eax
                call    ebp
                mov     dword ptr [esp+5Ch+var_34], 7373654Dh
                mov     dword ptr [esp+5Ch+var_34+4], 42656761h
                mov     dword ptr [esp+5Ch+var_54], 72657355h
                mov     dword ptr [esp+5Ch+var_54+4], 642E3233h
                mov     word ptr [esp+5Ch+var_54+8], 6C6Ch
                mov     [esp+5Ch+var_5C], eax
                mov     dword ptr [esp+5Ch+var_34+8], 57786Fh
                mov     [esp+5Ch+var_54+0Ah], 0
                push    esi
                call    ebx
                push    edi
                push    eax
                call    ebp
                mov     dword ptr [esp+5Ch+var_54], 74697845h
                mov     dword ptr [esp+5Ch+var_54+4], 636F7250h
                mov     dword ptr [esp+5Ch+var_34], 6E72654Bh
                mov     dword ptr [esp+5Ch+var_34+4], 32336C65h
                mov     dword ptr [esp+5Ch+var_34+8], 6C6C642Eh
                mov     [esp+5Ch+var_58], eax
                mov     dword ptr [esp+5Ch+var_54+8], 737365h
                mov     [esp+5Ch+var_34+0Ch], 0
                push    edi
                call    ebx
                push    esi
                push    eax
                call    ebp
                mov     dword ptr [esp+5Ch+var_54], 6C6C6548h
                mov     word ptr [esp+5Ch+var_54+4], 216Fh
                mov     dword ptr [esp+5Ch+var_34], 6C6C6548h
                mov     dword ptr [esp+5Ch+var_34+4], 6E41206Fh
                mov     ebx, eax
                mov     [esp+5Ch+var_54+6], 0
                mov     dword ptr [esp+5Ch+var_34+8], 216973h
                push    0
                push    esi
                push    edi
                push    0
                call    [esp+6Ch+var_5C]
                mov     dword ptr [esp+5Ch+var_54], 650048h
                mov     dword ptr [esp+5Ch+var_54+4], 6C006Ch
                mov     dword ptr [esp+5Ch+var_54+8], 21006Fh
                mov     dword ptr [esp+5Ch+var_34], 650048h
                mov     dword ptr [esp+5Ch+var_34+4], 6C006Ch
                mov     dword ptr [esp+5Ch+var_34+8], 20006Fh
                mov     dword ptr [esp+5Ch+var_34+0Ch], 6E0055h
                mov     [esp+5Ch+var_24], 630069h
                mov     [esp+5Ch+var_20], 64006Fh
                mov     [esp+5Ch+var_1C], 210065h
                mov     word ptr [esp+5Ch+var_48], 0
                mov     [esp+5Ch+var_18], 0
                push    0
                push    esi
                push    edi
                push    0
                call    [esp+6Ch+var_58]
                push    0
                call    ebx

loc_380:                                ; CODE XREF: sub_0+F7↑j
                mov     eax, [ebx+edi+24h]
                mov     ecx, [ebx+edi+1Ch]
                add     eax, ebx
                add     ecx, ebx
                movzx   eax, word ptr [eax+edx*2]
                add     ebx, [ecx+eax*4]
                jmp     loc_11B
; ---------------------------------------------------------------------------

loc_398:                                ; CODE XREF: sub_0+1FA↑j
                mov     ecx, [esp+5Ch+var_5C]
                mov     eax, [ebp+ecx+24h]
                mov     ecx, [ebp+ecx+1Ch]
                add     eax, ebp
                add     ecx, ebp
                movzx   eax, word ptr [eax+esi*2]
                add     ebp, [ecx+eax*4]
                jmp     loc_21B
sub_0           endp
```
