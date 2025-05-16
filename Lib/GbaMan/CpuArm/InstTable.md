# インストラクションテーブル

##  オペコードのビット 27..20 による大分類

|      | + 0 | + 1 | + 2 | + 3 | + 4 | + 5 | + 6 | + 7 |
|-----:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 0x00 |
| 0x08 |
| 0x10 |
| 0x18 |
| 0x20 |
| 0x28 |
| 0x30 |
| 0x38 |
| 0x40 |
| 0x48 |
| 0x50 |
| 0x58 |
| 0x60 |
| 0x68 |
| 0x70 |
| 0x78 |
| 0x80 |
| 0x88 |
| 0x90 |
| 0x98 |
| 0xA0 | (B) | (B) | (B) | (B) | (B) | (B) | (B) | (B) |
| 0xA8 | (B) | (B) | (B) | (B) | (B) | (B) | (B) | (B) |
| 0xB0 | (B) | (B) | (B) | (B) | (B) | (B) | (B) | (B) |
| 0xB8 | (B) | (B) | (B) | (B) | (B) | (B) | (B) | (B) |
| 0xC0 |
| 0xC8 |
| 0xD0 |
| 0xD8 |
| 0xE0 |(COP)|(COP)|(COP)|(COP)|(COP)|(COP)|(COP)|(COP)|
| 0xE8 |(COP)|(COP)|(COP)|(COP)|(COP)|(COP)|(COP)|(COP)|
| 0xF0 | (B) | (B) | (B) | (B) | (B) | (B) | (B) | (B) |
| 0xF8 | (B) | (B) | (B) | (B) | (B) | (B) | (B) | (B) |

記号
- (B)   : [02. 分岐命令](#02-分岐命令)
- (COP) : [09. コプロセッサ命令](#09-コプロセッサ命令)

##  さらにビット 07..04 も必要な分類

- 範囲 0x00 - 0x3F の場合

|       | + 0 | + 1 | + 2 | + 3 | + 4 | + 5 | + 6 | + 7 |
|------:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 0x000 | AND | AND | AND | AND | AND | AND | AND | AND |
| 0x008 | AND |     | AND |     | AND |     | AND |     |
| 0x010 | AND | AND | AND | AND | AND | AND | AND | AND |
| 0x018 | AND |     | AND |     | AND |     | AND |     |
| 0x020 | EOR | EOR | EOR | EOR | EOR | EOR | EOR | EOR |
| 0x028 | EOR |     | EOR |     | EOR |     | EOR |     |
| 0x030 | EOR | EOR | EOR | EOR | EOR | EOR | EOR | EOR |
| 0x038 | EOR |     | EOR |     | EOR |     | EOR |     |
| 0x040 | SUB | SUB | SUB | SUB | SUB | SUB | SUB | SUB |
| 0x048 | SUB |     | SUB |     | SUB |     | SUB |     |
| 0x050 | SUB | SUB | SUB | SUB | SUB | SUB | SUB | SUB |
| 0x058 | SUB |     | SUB |     | SUB |     | SUB |     |
| 0x060 | RSB | RSB | RSB | RSB | RSB | RSB | RSB | RSB |
| 0x068 | RSB |     | RSB |     | RSB |     | RSB |     |
| 0x070 | RSB | RSB | RSB | RSB | RSB | RSB | RSB | RSB |
| 0x078 | RSB |     | RSB |     | RSB |     | RSB |     |
| 0x080 | ADD | ADD | ADD | ADD | ADD | ADD | ADD | ADD |
| 0x088 | ADD |     | ADD |     | ADD |     | ADD |     |
| 0x090 | ADD | ADD | ADD | ADD | ADD | ADD | ADD | ADD |
| 0x098 | ADD |     | ADD |     | ADD |     | ADD |     |
| 0x0A0 | ADC | ADC | ADC | ADC | ADC | ADC | ADC | ADC |
| 0x0A8 | ADC |     | ADC |     | ADC |     | ADC |     |
| 0x0B0 | ADC | ADC | ADC | ADC | ADC | ADC | ADC | ADC |
| 0x0B8 | ADC |     | ADC |     | ADC |     | ADC |     |
| 0x0C0 | SBC | SBC | SBC | SBC | SBC | SBC | SBC | SBC |
| 0x0C8 | SBC |     | SBC |     | SBC |     | SBC |     |
| 0x0D0 | SBC | SBC | SBC | SBC | SBC | SBC | SBC | SBC |
| 0x0D8 | SBC |     | SBC |     | SBC |     | SBC |     |
| 0x0E0 | RSC | RSC | RSC | RSC | RSC | RSC | RSC | RSC |
| 0x0E8 | RSC |     | RSC |     | RSC |     | RSC |     |
| 0x0F0 | RSC | RSC | RSC | RSC | RSC | RSC | RSC | RSC |
| 0x0F8 | RSC |     | RSC |     | RSC |     | RSC |     |
| 0x100 |     |     |     |     |     |     |     |     |
| 0x108 |     |     |     |     |     |     |     |     |
| 0x110 | TST | TST | TST | TST | TST | TST | TST | TST |
| 0x118 | TST |     | TST |     | TST |     | TST |     |
| 0x120 |     |     |     |     |     |     |     |     |
| 0x128 |     |     |     |     |     |     |     |     |
| 0x130 | TEQ | TEQ | TEQ | TEQ | TEQ | TEQ | TEQ | TEQ |
| 0x138 | TEQ |     | TEQ |     | TEQ |     | TEQ |     |
| 0x140 |     |     |     |     |     |     |     |     |
| 0x148 |     |     |     |     |     |     |     |     |
| 0x150 | CMP | CMP | CMP | CMP | CMP | CMP | CMP | CMP |
| 0x158 | CMP |     | CMP |     | CMP |     | CMP |     |
| 0x160 |     |     |     |     |     |     |     |     |
| 0x168 |     |     |     |     |     |     |     |     |
| 0x170 | CMN | CMN | CMN | CMN | CMN | CMN | CMN | CMN |
| 0x178 | CMN |     | CMN |     | CMN |     | CMN |     |
| 0x180 | ALU | ALU | ALU | ALU | ALU | ALU | ALU | ALU |
| 0x188 | ALU |     | ALU |     | ALU |     | ALU |     |
| 0x190 | ALU | ALU | ALU | ALU | ALU | ALU | ALU | ALU |
| 0x198 | ALU |     | ALU |     | ALU |     | ALU |     |
| 0x1A0 | ALU | ALU | ALU | ALU | ALU | ALU | ALU | ALU |
| 0x1A8 | ALU |     | ALU |     | ALU |     | ALU |     |
| 0x1B0 | ALU | ALU | ALU | ALU | ALU | ALU | ALU | ALU |
| 0x1B8 | ALU |     | ALU |     | ALU |     | ALU |     |
| 0x1C0 | ALU | ALU | ALU | ALU | ALU | ALU | ALU | ALU |
| 0x1C8 | ALU |     | ALU |     | ALU |     | ALU |     |
| 0x1D0 | ALU | ALU | ALU | ALU | ALU | ALU | ALU | ALU |
| 0x1D8 | ALU |     | ALU |     | ALU |     | ALU |     |
| 0x1E0 | ALU | ALU | ALU | ALU | ALU | ALU | ALU | ALU |
| 0x1E8 | ALU |     | ALU |     | ALU |     | ALU |     |
| 0x1F0 | ALU | ALU | ALU | ALU | ALU | ALU | ALU | ALU |
| 0x1F8 | ALU |     | ALU |     | ALU |     | ALU |     |
| 0x200 | AND | AND | AND | AND | AND | AND | AND | AND |
| 0x208 | AND | AND | AND | AND | AND | AND | AND | AND |
| 0x210 | AND | AND | AND | AND | AND | AND | AND | AND |
| 0x218 | AND | AND | AND | AND | AND | AND | AND | AND |
| 0x220 | EOR | EOR | EOR | EOR | EOR | EOR | EOR | EOR |
| 0x228 | EOR | EOR | EOR | EOR | EOR | EOR | EOR | EOR |
| 0x230 | EOR | EOR | EOR | EOR | EOR | EOR | EOR | EOR |
| 0x238 | EOR | EOR | EOR | EOR | EOR | EOR | EOR | EOR |
| 0x240 | SUB | SUB | SUB | SUB | SUB | SUB | SUB | SUB |
| 0x248 | SUB | SUB | SUB | SUB | SUB | SUB | SUB | SUB |
| 0x250 | SUB | SUB | SUB | SUB | SUB | SUB | SUB | SUB |
| 0x258 | SUB | SUB | SUB | SUB | SUB | SUB | SUB | SUB |
| 0x260 | RSB | RSB | RSB | RSB | RSB | RSB | RSB | RSB |
| 0x268 | RSB | RSB | RSB | RSB | RSB | RSB | RSB | RSB |
| 0x270 | RSB | RSB | RSB | RSB | RSB | RSB | RSB | RSB |
| 0x278 | RSB | RSB | RSB | RSB | RSB | RSB | RSB | RSB |
| 0x280 | ADD | ADD | ADD | ADD | ADD | ADD | ADD | ADD |
| 0x288 | ADD | ADD | ADD | ADD | ADD | ADD | ADD | ADD |
| 0x290 | ADD | ADD | ADD | ADD | ADD | ADD | ADD | ADD |
| 0x298 | ADD | ADD | ADD | ADD | ADD | ADD | ADD | ADD |
| 0x2A0 | ADC | ADC | ADC | ADC | ADC | ADC | ADC | ADC |
| 0x2A8 | ADC | ADC | ADC | ADC | ADC | ADC | ADC | ADC |
| 0x2B0 | ADC | ADC | ADC | ADC | ADC | ADC | ADC | ADC |
| 0x2B8 | ADC | ADC | ADC | ADC | ADC | ADC | ADC | ADC |
| 0x2C0 | SBC | SBC | SBC | SBC | SBC | SBC | SBC | SBC |
| 0x2C8 | SBC | SBC | SBC | SBC | SBC | SBC | SBC | SBC |
| 0x2D0 | SBC | SBC | SBC | SBC | SBC | SBC | SBC | SBC |
| 0x2D8 | SBC | SBC | SBC | SBC | SBC | SBC | SBC | SBC |
| 0x2E0 | RSC | RSC | RSC | RSC | RSC | RSC | RSC | RSC |
| 0x2E8 | RSC | RSC | RSC | RSC | RSC | RSC | RSC | RSC |
| 0x2F0 | RSC | RSC | RSC | RSC | RSC | RSC | RSC | RSC |
| 0x2F8 | RSC | RSC | RSC | RSC | RSC | RSC | RSC | RSC |
| 0x300 |     |     |     |     |     |     |     |     |
| 0x308 |     |     |     |     |     |     |     |     |
| 0x310 | TST | TST | TST | TST | TST | TST | TST | TST |
| 0x318 | TST | TST | TST | TST | TST | TST | TST | TST |
| 0x320 |     |     |     |     |     |     |     |     |
| 0x328 |     |     |     |     |     |     |     |     |
| 0x330 | TEQ | TEQ | TEQ | TEQ | TEQ | TEQ | TEQ | TEQ |
| 0x338 | TEQ | TEQ | TEQ | TEQ | TEQ | TEQ | TEQ | TEQ |
| 0x340 |     |     |     |     |     |     |     |     |
| 0x348 |     |     |     |     |     |     |     |     |
| 0x350 | CMP | CMP | CMP | CMP | CMP | CMP | CMP | CMP |
| 0x358 | CMP | CMP | CMP | CMP | CMP | CMP | CMP | CMP |
| 0x360 |     |     |     |     |     |     |     |     |
| 0x368 |     |     |     |     |     |     |     |     |
| 0x370 | CMN | CMN | CMN | CMN | CMN | CMN | CMN | CMN |
| 0x378 | CMN | CMN | CMN | CMN | CMN | CMN | CMN | CMN |
| 0x380 | ALU | ALU | ALU | ALU | ALU | ALU | ALU | ALU |
| 0x388 | ALU | ALU | ALU | ALU | ALU | ALU | ALU | ALU |
| 0x390 | ALU | ALU | ALU | ALU | ALU | ALU | ALU | ALU |
| 0x398 | ALU | ALU | ALU | ALU | ALU | ALU | ALU | ALU |
| 0x3A0 | ALU | ALU | ALU | ALU | ALU | ALU | ALU | ALU |
| 0x3A8 | ALU | ALU | ALU | ALU | ALU | ALU | ALU | ALU |
| 0x3B0 | ALU | ALU | ALU | ALU | ALU | ALU | ALU | ALU |
| 0x3B8 | ALU | ALU | ALU | ALU | ALU | ALU | ALU | ALU |
| 0x3C0 | ALU | ALU | ALU | ALU | ALU | ALU | ALU | ALU |
| 0x3C8 | ALU | ALU | ALU | ALU | ALU | ALU | ALU | ALU |
| 0x3D0 | ALU | ALU | ALU | ALU | ALU | ALU | ALU | ALU |
| 0x3D8 | ALU | ALU | ALU | ALU | ALU | ALU | ALU | ALU |
| 0x3E0 | ALU | ALU | ALU | ALU | ALU | ALU | ALU | ALU |
| 0x3E8 | ALU | ALU | ALU | ALU | ALU | ALU | ALU | ALU |
| 0x3F0 | ALU | ALU | ALU | ALU | ALU | ALU | ALU | ALU |
| 0x3F8 | ALU | ALU | ALU | ALU | ALU | ALU | ALU | ALU |



##  詳細

- 全ての命令において上位４ビットは条件指定なので省略する

###   01. ALU

- 000x xxx? ????  ???? ???? ???? ???? (0x0000000 - 0x1FFFFFF) : ALU レジスタ
- 001x xxx? ????  ???? ???? ???? ???? (0x2000000 - 0x3FFFFFF) : ALU 即値

除外
- 000x xxx? ????  ???? ???? 1??1 ???? (R=1 の時 bit7 は必ずゼロ)
- 00?1 0000 ????  ???? ???? ???? ???? (TST 命令でフラグを更新しない時)
- 00?1 0010 ????  ???? ???? ???? ???? (TEQ 命令でフラグを更新しない時)
- 00?1 0100 ????  ???? ???? ???? ???? (CMP 命令でフラグを更新しない時)
- 00?1 0110 ????  ???? ???? ???? ???? (CMN 命令でフラグを更新しない時)

###   02. 分岐命令

- 101x ???? ????  ???? ???? ???? ???? (0xA000000 - 0xBFFFFFF)
    - 1010 ???? ????  ???? ???? ???? ???? (0xA000000 - 0xAFFFFFF) : B  label
    - 1011 ???? ????  ???? ???? ???? ???? (0xB000000 - 0xBFFFFFF) : BL label
- 0001 0010 1111  1111 1111 0001 ???? (0x12FFF10 - 0x12FFF1F) : BX
- 1111 ???? ????  ???? ???? ???? ???? (0xF000000 - 0xFFFFFFF) : SWI

###   03.  乗算

- 000x xxx? ????  ???? ???? 1001 ???? (0x0000090 - 0x1FFFF9F)
- 000x xxx0 ????  ???? ???? 1??0 ???? (0x0000090 - 0x1EFFFEF)

###   04. ロード／ストア

- 010? ???0 ????  ???? ???? ???? ???? (0x4000000 - 0x5EFFFFF) STR
- 011? ???0 ????  ???? ???? ???? ???? (0x6000000 - 0x7EFFFFF) STR
- 010? ???1 ????  ???? ???? ???? ???? (0x4100000 - 0x5FFFFFF) LDR
- 011? ???1 ????  ???? ???? ???? ???? (0x6100000 - 0x7FFFFFF) LDR

###   05. ロード／ストア

- 000? ?0?x ????  ???? 0000 1xx1 ????
- 000? ?1?x ????  ???? ???? 1xx1 ????

###   06. スタック

- 100? ???0 ????  ???? ???? ???? ???? STM
- 100? ???1 ????  ???? ???? ???? ???? LDM

###   07. PSR 操作

- 0001 0?00 1111  ???? 0000 0000 0000 MRS
- 0001 0?10 ????  ???? 0000 0000 ???? MSR
- 0011 0?10 ????  ???? ???? ???? ???? MSR

###   08. スワップ

- 0001 0?00 ????  ???? 0000 1001 ???? SWP

###   09. コプロセッサ命令

- 1110 xxx0 ????  ???? ???? ???1 ???? (0xE000010 - 0xEEFFFFF) : MCR
- 1110 xxx1 ????  ???? ???? ???1 ???? (0xE100010 - 0xEFFFFFF) : MRC
