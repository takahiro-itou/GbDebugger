# インストラクションテーブル

##

- オペコードの上位１オクテットによる分類

|      | + 0 | + 1 | + 2 | + 3 | + 4 | + 5 | + 6 | + 7 |
|-----:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 0x00 | LSL | LSL | LSL | LSL | LSL | LSL | LSL | LSL |
| 0x08 | LSR | LSR | LSR | LSR | LSR | LSR | LSR | LSR |
| 0x10 | ASR | ASR | ASR | ASR | ASR | ASR | ASR | ASR |
| 0x18 | ADD | ADD | SUB | SUB | ADD | ADD | SUB | SUB |
| 0x20 | MOV | MOV | MOV | MOV | MOV | MOV | MOV | MOV |
| 0x28 | CMP | CMP | CMP | CMP | CMP | CMP | CMP | CMP |
| 0x30 | ADD | ADD | ADD | ADD | ADD | ADD | ADD | ADD |
| 0x38 | SUB | SUB | SUB | SUB | SUB | SUB | SUB | SUB |
| 0x40 | (4) | (4) | (4) | (4) | ADD | CMP | MOV | BX  |
| 0x48 | LDR | LDR | LDR | LDR | LDR | LDR | LDR | LDR |
| 0x50 | STR | STR | STRH| STRH| STRB| STRB| LDSB| LDSB|
| 0x58 | LDR | LDR | LDRH| LDRH| LDRB| LDRB| LDSH| LDSH|
| 0x60 | STR | STR | STR | STR | STR | STR | STR | STR |
| 0x68 | LDR | LDR | LDR | LDR | LDR | LDR | LDR | LDR |
| 0x70 | STRB| STRB| STRB| STRB| STRB| STRB| STRB| STRB|
| 0x78 | LDRB| LDRB| LDRB| LDRB| LDRB| LDRB| LDRB| LDRB|
| 0x80 | STRH| STRH| STRH| STRH| STRH| STRH| STRH| STRH|
| 0x88 | LDRH| LDRH| LDRH| LDRH| LDRH| LDRH| LDRH| LDRH|
| 0x90 | STR | STR | STR | STR | STR | STR | STR | STR |
| 0x98 | LDR | LDR | LDR | LDR | LDR | LDR | LDR | LDR |
| 0xA0 | ADD | ADD | ADD | ADD | ADD | ADD | ADD | ADD |
| 0xA8 | ADD | ADD | ADD | ADD | ADD | ADD | ADD | ADD |
| 0xB0 | ADD | und | und | und | PUSH| PUSH| und | und |
| 0xB8 | und | und | und | und | POP | POP | BKPT| und |
| 0xC0 |STMIA|STMIA|STMIA|STMIA|STMIA|STMIA|STMIA|STMIA|
| 0xC8 |LDMIA|LDMIA|LDMIA|LDMIA|LDMIA|LDMIA|LDMIA|LDMIA|
| 0xD0 | BEQ | BNE | BCS | BCC | BMI | BPL | BVS | BVC |
| 0xD8 | BHI | BLS | BGE | BLT | BGT | BLE | und | SWI |
| 0xE0 |  B  |  B  |  B  |  B  |  B  |  B  |  B  |  B  |
| 0xE8 | BL  | BL  | BL  | BL  | BL  | BL  | BL  | BL  |
| 0xF0 | BL  | BL  | BL  | BL  | BL  | BL  | BL  | BL  |
| 0xF8 | BL  | BL  | BL  | BL  | BL  | BL  | BL  | BL  |

|      | + 0 | + 1 | + 2 | + 3 | + 4 | + 5 | + 6 | + 7 |
|-----:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 0x00 | 01  | 01  | 01  | 01  | 01  | 01  | 01  | 01  |
| 0x08 | 01  | 01  | 01  | 01  | 01  | 01  | 01  | 01  |
| 0x10 | 01  | 01  | 01  | 01  | 01  | 01  | 01  | 01  |
| 0x18 | 02  | 02  | 02  | 02  | 02  | 02  | 02  | 02  |
| 0x20 | 03  | 03  | 03  | 03  | 03  | 03  | 03  | 03  |
| 0x28 | 03  | 03  | 03  | 03  | 03  | 03  | 03  | 03  |
| 0x30 | 03  | 03  | 03  | 03  | 03  | 03  | 03  | 03  |
| 0x38 | 03  | 03  | 03  | 03  | 03  | 03  | 03  | 03  |
| 0x40 | 04  | 04  | 04  | 04  | 05  | 05  | 05  | 05  |
| 0x48 | 06  | 06  | 06  | 06  | 06  | 06  | 06  | 06  |
| 0x50 | 07  | 07  | 08  | 08  | 07  | 07  | 08  | 08  |
| 0x58 | 07  | 07  | 08  | 08  | 07  | 07  | 08  | 08  |
| 0x60 | 09  | 09  | 09  | 09  | 09  | 09  | 09  | 09  |
| 0x68 | 09  | 09  | 09  | 09  | 09  | 09  | 09  | 09  |
| 0x70 | 09  | 09  | 09  | 09  | 09  | 09  | 09  | 09  |
| 0x78 | 09  | 09  | 09  | 09  | 09  | 09  | 09  | 09  |
| 0x80 | 10  | 10  | 10  | 10  | 10  | 10  | 10  | 10  |
| 0x88 | 10  | 10  | 10  | 10  | 10  | 10  | 10  | 10  |
| 0x90 | 11  | 11  | 11  | 11  | 11  | 11  | 11  | 11  |
| 0x98 | 11  | 11  | 11  | 11  | 11  | 11  | 11  | 11  |
| 0xA0 | 12  | 12  | 12  | 12  | 12  | 12  | 12  | 12  |
| 0xA8 | 12  | 12  | 12  | 12  | 12  | 12  | 12  | 12  |
| 0xB0 | 13  | und | und | und | 14  | 14  | und | und |
| 0xB8 | und | und | und | und | 14  | 14  | 17  | und |
| 0xC0 | 15  | 15  | 15  | 15  | 15  | 15  | 15  | 15  |
| 0xC8 | 15  | 15  | 15  | 15  | 15  | 15  | 15  | 15  |
| 0xD0 | 16  | 16  | 16  | 16  | 16  | 16  | 16  | 16  |
| 0xD8 | 16  | 16  | 16  | 16  | 16  | 16  | und | 17  |
| 0xE0 | 18  | 18  | 18  | 18  | 18  | 18  | 18  | 18  |
| 0xE8 | 19  | 19  | 19  | 19  | 19  | 19  | 19  | 19  |
| 0xF0 | 19  | 19  | 19  | 19  | 19  | 19  | 19  | 19  |
| 0xF8 | 19  | 19  | 19  | 19  | 19  | 19  | 19  | 19  |


##  詳細

01. ビットシフト

- 000x x??? ???? ???? (0x0000 - 0x17FF)
    - 0000 0??? ???? ???? (0x0000 - 0x07FF) : LSL Rd, Rs, #nn
    - 0000 1??? ???? ???? (0x0800 - 0x0FFF) : LSR Rd, Rs, #nn
    - 0001 0??? ???? ???? (0x1000 - 0x17FF) : ASR Rd, Rs, #nn

02. 加減算

- 0001 1xx? ???? ???? (0x1800 - 0x1FFF)
    - 0001 100? ???? ???? (0x1800 - 0x19FF) : ADD Rd, Rs, Rn
    - 0001 101? ???? ???? (0x1A00 - 0x1BFF) : SUB Rd, Rs, Rn
    - 0001 110? ???? ???? (0x1C00 - 0x1DFF) : ADD Rd, Rs, #nn
    - 0001 111? ???? ???? (0x1E00 - 0x1FFF) : SUB Rd, Rs, #nn


03. 即値の演算
- 001x x??? ???? ???? (0x2000 - 0x3FFF)
    - 0010 0??? ???? ???? (0x2000 - 0x27FF) : MOV Rd, #nn
    - 0010 1??? ???? ???? (0x2800 - 0x2FFF) : CMP Rd, #nn
    - 0011 0??? ???? ???? (0x3000 - 0x37FF) : ADD Rd, #nn
    - 0011 1??? ???? ???? (0x3800 - 0x37FF) : SUB Rd, #nn

04. 算術演算

- 0100 00xx xx?? ???? (0x4000 - 0x43FF)
    - 0100 0000 00?? ???? (0x4000 - 0x403F) : AND Rd, Rs
    - 0100 0000 01?? ???? (0x4040 - 0x407F) : EOR Rd, Rs
    - 0100 0000 10?? ???? (0x4080 - 0x40BF) : LSL Rd, Rs
    - 0100 0000 11?? ???? (0x40C0 - 0x40FF) : LSR Rd, Rs
    - 0100 0001 00?? ???? (0x4100 - 0x413F) : ASR Rd, Rs
    - 0100 0001 01?? ???? (0x4140 - 0x417F) : ADC Rd, Rs
    - 0100 0001 10?? ???? (0x4180 - 0x41BF) : SBC Rd, Rs
    - 0100 0001 11?? ???? (0x41C0 - 0x41FF) : ROR Rd, Rs
    - 0100 0010 00?? ???? (0x4200 - 0x423F) : TST Rd, Rs
    - 0100 0010 01?? ???? (0x4240 - 0x427F) : NEG Rd, Rs
    - 0100 0010 10?? ???? (0x4280 - 0x42BF) : CMP Rd, Rs
    - 0100 0010 11?? ???? (0x42C0 - 0x42FF) : CMN Rd, Rs
    - 0100 0011 00?? ???? (0x4300 - 0x433F) : ORR Rd, Rs
    - 0100 0011 01?? ???? (0x4340 - 0x437F) : MUL Rd, Rs
    - 0100 0011 10?? ???? (0x4380 - 0x43BF) : BIC Rd, Rs
    - 0100 0011 11?? ???? (0x43C0 - 0x43FF) : MVN Rd, Rs

05. ハイレジスタ操作

- 0100 01xx ???? ???? (0x4400 - 0x47FF)
    - 0100 0100 ???? ???? (0x4400 - 0x44FF) : ADD Rd, Rs
    - 0100 0101 ???? ???? (0x4500 - 0x45FF) : CMP Rd, Rs
    - 0100 0110 ???? ???? (0x4600 - 0x46FF) : MOV Rd, Rs
    - 0100 0111 ???? ???? (0x4700 - 0x47FF) : BX  Rs

06. ロード／ストア（PC相対）

- 0100 1??? ???? ???? (0x4800 - 0x4FFF) : LDR Rd, [PC, #nn]

07. ロード／ストア

- 0101 xx0? ???? ???? (0x5000 - 0x5FFF)
    - 0101 000? ???? ???? (0x5000 - 0x51FF) : STR  Rd, [Rb, Ro]
    - 0101 010? ???? ???? (0x5400 - 0x55FF) : STRB Rd, [Rb, Ro]
    - 0101 100? ???? ???? (0x5800 - 0x59FF) : LDR  Rd, [Rb, Ro]
    - 0101 110? ???? ???? (0x5C00 - 0x5DFF) : LDRB Rd, [Rb, Ro]

08. ロード／ストア（符号拡張／ハーフワード）

- 0101 xx1? ???? ???? (0x5000 - 0x5FFF)
    - 0101 001? ???? ???? (0x5200 - 0x53FF) : STRH Rd, [Rb, Ro]
    - 0101 011? ???? ???? (0x5600 - 0x57FF) : LDSB Rd, [Rb, Ro]
    - 0101 101? ???? ???? (0x5A00 - 0x5BFF) : LDRH Rd, [Rb, Ro]
    - 0101 111? ???? ???? (0x5E00 - 0x5FFF) : LDSH Rd, [Rb, Ro]

09. ロード／ストア

- 011x x??? ???? ???? (0x6000 - 0x7FFF)
    - 0110 0??? ???? ???? (0x6000 - 0x67FF) : STR  Rd, [Rb, #nn]
    - 0110 1??? ???? ???? (0x6800 - 0x6FFF) : LDR  Rd, [Rb, #nn]
    - 0111 0??? ???? ???? (0x7000 - 0x77FF) : STRB Rd, [Rb, #nn]
    - 0111 1??? ???? ???? (0x7800 - 0x7FFF) : LDRB Rd, [Rb, #nn]

10. ロード／ストア（ハーフワード）

- 1000 x??? ???? ???? (0x8000 - 0x8FFF)
    - 1000 0??? ???? ???? (0x8000 - 0x87FF) : STRH Rd, [Rb, #nn]
    - 1000 1??? ???? ???? (0x8800 - 0x8FFF) : LDRH Rd, [Rb, #nn]

11. ロード／ストア（SP相対）

- 1001 x??? ???? ???? (0x9000 - 0x9FFF)
    - 1001 0??? ???? ???? (0x9000 - 0x97FF) : STR  Rd, [SP, #nn]
    - 1001 1??? ???? ???? (0x9800 - 0x9FFF) : LDR  Rd, [SP, #nn]

12. アドレッシング

- 1010 x??? ???? ???? (0xA000 - 0xAFFF)
    - 1010 0??? ???? ???? (0xA000 - 0xA7FF) : ADD  Rd, PC, #nn
    - 1010 1??? ???? ???? (0xA800 - 0xAFFF) : ADD  Rd, SP, #nn

13. スタックポインタ操作

- 1011 0000 0??? ???? (0xB000 - 0xB07F) : ADD  SP, #nn
- 1011 0000 1??? ???? (0xB080 - 0xB0FF) : ADD  SP, #-nn

14. スタック

- 1011 x10? ???? ???? (0xB400 - 0xB5FF)
    - 1011 010? ???? ???? (0xB400 - 0xB5FF) : PUSH
    - 1011 110? ???? ???? (0xBC00 - 0xBDFF) : POP

15. ロード／ストア（レジスタリスト）

- 1100 x??? ???? ???? (0xC000 - 0xCFFF)
    - 1100 0??? ??? ??? (0xC000 - 0xC7FF) : STMIA Rb!, {Rlist}
    - 1100 1??? ??? ??? (0xC800 - 0xCFFF) : LDMIA Rb!, {Rlist}

16. 条件分岐

- 1101 xxxx ???? ???? (0xD000 -0xDFFF)
    - 1101 0000 ???? ???? (0xD000 - 0xD0FF) : BEQ
    - 1101 0001 ???? ???? (0xD100 - 0xD1FF) : BNE
    - 1101 0010 ???? ???? (0xD200 - 0xD2FF) : BCS
    - 1101 0011 ???? ???? (0xD300 - 0xD3FF) : BCC
    - 1101 0100 ???? ???? (0xD400 - 0xD4FF) : BMI
    - 1101 0101 ???? ???? (0xD500 - 0xD5FF) : BPL
    - 1101 0110 ???? ???? (0xD600 - 0xD6FF) : BVS
    - 1101 0111 ???? ???? (0xD700 - 0xD7FF) : BVC
    - 1101 1000 ???? ???? (0xD800 - 0xD8FF) : BHI
    - 1101 1001 ???? ???? (0xD900 - 0xD9FF) : BLS
    - 1101 1010 ???? ???? (0xDA00 - 0xDAFF) : BGE
    - 1101 1011 ???? ???? (0xDB00 - 0xDBFF) : BLT
    - 1101 1100 ???? ???? (0xDC00 - 0xDCFF) : BGT
    - 1101 1101 ???? ???? (0xDD00 - 0xDDFF) : BLE

17. 割り込み

- 1101 1111 ???? ???? (0xDF00 - 0xDFFF) : SWI  #nn
- 1011 1110 ???? ???? (0xBE00 - 0xBEFF) : BKPT #nn

18. 分岐（無条件）

- 1110 0??? ???? ???? (0xE000 - 0xE7FF) : B  label

19. サブルーチンコール

- 1111 0??? ???? ???? (0xF000 - 0xF7FF) : BLH
- 1111 1??? ???? ???? (0xF800 - 0xFFFF) : BL
- 1110 1??? ???? ???? (0xE800 - 0xEFFF) : BLX