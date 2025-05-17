# インストラクションテーブル

##  オペコードのビット 27..20 による大分類

|      | + 0 | + 1 | + 2 | + 3 | + 4 | + 5 | + 6 | + 7 |
|-----:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 0x00 |(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|
| 0x08 |(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|
| 0x10 |(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|
| 0x18 |(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|
| 0x20 |(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|
| 0x28 |(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|
| 0x30 |(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|
| 0x38 |(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|(C-1)|
| 0x40 | STR | LDR | STR | LDR | STR | LDR | STR | LDR |
| 0x48 | STR | LDR | STR | LDR | STR | LDR | STR | LDR |
| 0x50 | STR | LDR | STR | LDR | STR | LDR | STR | LDR |
| 0x58 | STR | LDR | STR | LDR | STR | LDR | STR | LDR |
| 0x60 | STR | LDR | STR | LDR | STR | LDR | STR | LDR |
| 0x68 | STR | LDR | STR | LDR | STR | LDR | STR | LDR |
| 0x70 | STR | LDR | STR | LDR | STR | LDR | STR | LDR |
| 0x78 | STR | LDR | STR | LDR | STR | LDR | STR | LDR |
| 0x80 | STM | LDM | STM | LDM | STM | LDM | STM | LDM |
| 0x88 | STM | LDM | STM | LDM | STM | LDM | STM | LDM |
| 0x90 | STM | LDM | STM | LDM | STM | LDM | STM | LDM |
| 0x98 | STM | LDM | STM | LDM | STM | LDM | STM | LDM |
| 0xA0 | (B) | (B) | (B) | (B) | (B) | (B) | (B) | (B) |
| 0xA8 | (B) | (B) | (B) | (B) | (B) | (B) | (B) | (B) |
| 0xB0 | (B) | (B) | (B) | (B) | (B) | (B) | (B) | (B) |
| 0xB8 | (B) | (B) | (B) | (B) | (B) | (B) | (B) | (B) |
| 0xC0 |[und]|[und]|[und]|[und]|[und]|[und]|[und]|[und]|
| 0xC8 |[und]|[und]|[und]|[und]|[und]|[und]|[und]|[und]|
| 0xD0 |[und]|[und]|[und]|[und]|[und]|[und]|[und]|[und]|
| 0xD8 |[und]|[und]|[und]|[und]|[und]|[und]|[und]|[und]|
| 0xE0 |(COP)|(COP)|(COP)|(COP)|(COP)|(COP)|(COP)|(COP)|
| 0xE8 |(COP)|(COP)|(COP)|(COP)|(COP)|(COP)|(COP)|(COP)|
| 0xF0 | (B) | (B) | (B) | (B) | (B) | (B) | (B) | (B) |
| 0xF8 | (B) | (B) | (B) | (B) | (B) | (B) | (B) | (B) |

記号
- (C-1) : [ビット 07..04 による小分類](#C-1--ビット-0704-による小分類)
-  (B)  : [02. 分岐命令](#02-分岐命令)
- (COP) : [09. コプロセッサ命令](#09-コプロセッサ命令)
-  STR  : [04. ロード／ストア](#04-ロードストア) のストア (STR) 命令
-  LDR  : [04. ロード／ストア](#04-ロードストア) のロード (LDR) 命令
-  STM  : [06. スタック](#06-スタック) の STM 命令 (PUSH)
-  LDM  : [06. スタック](#06-スタック) の LDM 命令 (POP)
- [und] : 未定義命令

##  (C-1) : ビット 07..04 による小分類

- 範囲 0x00 - 0x3F

|       | + 0 | + 1 | + 2 | + 3 | + 4 | + 5 | + 6 | + 7 |
|------:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 0x000 | AND | AND | AND | AND | AND | AND | AND | AND |
| 0x008 | AND | MUL | AND |     | AND |     | AND |     |
| 0x010 | AND | AND | AND | AND | AND | AND | AND | AND |
| 0x018 | AND | MUL | AND |     | AND |     | AND |     |
| 0x020 | EOR | EOR | EOR | EOR | EOR | EOR | EOR | EOR |
| 0x028 | EOR | MLA | EOR |     | EOR |     | EOR |     |
| 0x030 | EOR | EOR | EOR | EOR | EOR | EOR | EOR | EOR |
| 0x038 | EOR | MLA | EOR |     | EOR |     | EOR |     |
| 0x040 | SUB | SUB | SUB | SUB | SUB | SUB | SUB | SUB |
| 0x048 | SUB |(MUL)| SUB |     | SUB |     | SUB |     |
| 0x050 | SUB | SUB | SUB | SUB | SUB | SUB | SUB | SUB |
| 0x058 | SUB |(MUL)| SUB |     | SUB |     | SUB |     |
| 0x060 | RSB | RSB | RSB | RSB | RSB | RSB | RSB | RSB |
| 0x068 | RSB |(MUL)| RSB |     | RSB |     | RSB |     |
| 0x070 | RSB | RSB | RSB | RSB | RSB | RSB | RSB | RSB |
| 0x078 | RSB |(MUL)| RSB |     | RSB |     | RSB |     |
| 0x080 | ADD | ADD | ADD | ADD | ADD | ADD | ADD | ADD |
| 0x088 | ADD |UMULL| ADD |     | ADD |     | ADD |     |
| 0x090 | ADD | ADD | ADD | ADD | ADD | ADD | ADD | ADD |
| 0x098 | ADD |UMULL| ADD |     | ADD |     | ADD |     |
| 0x0A0 | ADC | ADC | ADC | ADC | ADC | ADC | ADC | ADC |
| 0x0A8 | ADC |UMLAL| ADC |     | ADC |     | ADC |     |
| 0x0B0 | ADC | ADC | ADC | ADC | ADC | ADC | ADC | ADC |
| 0x0B8 | ADC |UMLAL| ADC |     | ADC |     | ADC |     |
| 0x0C0 | SBC | SBC | SBC | SBC | SBC | SBC | SBC | SBC |
| 0x0C8 | SBC |SMULL| SBC |     | SBC |     | SBC |     |
| 0x0D0 | SBC | SBC | SBC | SBC | SBC | SBC | SBC | SBC |
| 0x0D8 | SBC |SMULL| SBC |     | SBC |     | SBC |     |
| 0x0E0 | RSC | RSC | RSC | RSC | RSC | RSC | RSC | RSC |
| 0x0E8 | RSC |SMLAL| RSC |     | RSC |     | RSC |     |
| 0x0F0 | RSC | RSC | RSC | RSC | RSC | RSC | RSC | RSC |
| 0x0F8 | RSC |SMLAL| RSC |     | RSC |     | RSC |     |
| 0x100 | MRS |     |     |     |     |     |     |     |
| 0x108 |     | SWP |     |     |     |     |     |     |
| 0x110 | TST | TST | TST | TST | TST | TST | TST | TST |
| 0x118 | TST |     | TST |     | TST |     | TST |     |
| 0x120 | MSR | BX  |     |     |     |     |     |     |
| 0x128 |     |     |     |     |     |     |     |     |
| 0x130 | TEQ | TEQ | TEQ | TEQ | TEQ | TEQ | TEQ | TEQ |
| 0x138 | TEQ |     | TEQ |     | TEQ |     | TEQ |     |
| 0x140 | MRS |     |     |     |     |     |     |     |
| 0x148 |     | SWP |     |     |     |     |     |     |
| 0x150 | CMP | CMP | CMP | CMP | CMP | CMP | CMP | CMP |
| 0x158 | CMP |     | CMP |     | CMP |     | CMP |     |
| 0x160 | MSR |     |     |     |     |     |     |     |
| 0x168 |     |     |     |     |     |     |     |     |
| 0x170 | CMN | CMN | CMN | CMN | CMN | CMN | CMN | CMN |
| 0x178 | CMN |     | CMN |     | CMN |     | CMN |     |
| 0x180 | ORR | ORR | ORR | ORR | ORR | ORR | ORR | ORR |
| 0x188 | ORR |     | ORR |     | ORR |     | ORR |     |
| 0x190 | ORR | ORR | ORR | ORR | ORR | ORR | ORR | ORR |
| 0x198 | ORR |     | ORR |     | ORR |     | ORR |     |
| 0x1A0 | MOV | MOV | MOV | MOV | MOV | MOV | MOV | MOV |
| 0x1A8 | MOV |     | MOV |     | MOV |     | MOV |     |
| 0x1B0 | MOV | MOV | MOV | MOV | MOV | MOV | MOV | MOV |
| 0x1B8 | MOV |     | MOV |     | MOV |     | MOV |     |
| 0x1C0 | BIC | BIC | BIC | BIC | BIC | BIC | BIC | BIC |
| 0x1C8 | BIC |     | BIC |     | BIC |     | BIC |     |
| 0x1D0 | BIC | BIC | BIC | BIC | BIC | BIC | BIC | BIC |
| 0x1D8 | BIC |     | BIC |     | BIC |     | BIC |     |
| 0x1E0 | MVN | MVN | MVN | MVN | MVN | MVN | MVN | MVN |
| 0x1E8 | MVN |     | MVN |     | MVN |     | MVN |     |
| 0x1F0 | MVN | MVN | MVN | MVN | MVN | MVN | MVN | MVN |
| 0x1F8 | MVN |     | MVN |     | MVN |     | MVN |     |
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
| 0x320 | MSR | MSR | MSR | MSR | MSR | MSR | MSR | MSR |
| 0x328 | MSR | MSR | MSR | MSR | MSR | MSR | MSR | MSR |
| 0x330 | TEQ | TEQ | TEQ | TEQ | TEQ | TEQ | TEQ | TEQ |
| 0x338 | TEQ | TEQ | TEQ | TEQ | TEQ | TEQ | TEQ | TEQ |
| 0x340 |     |     |     |     |     |     |     |     |
| 0x348 |     |     |     |     |     |     |     |     |
| 0x350 | CMP | CMP | CMP | CMP | CMP | CMP | CMP | CMP |
| 0x358 | CMP | CMP | CMP | CMP | CMP | CMP | CMP | CMP |
| 0x360 | MSR | MSR | MSR | MSR | MSR | MSR | MSR | MSR |
| 0x368 | MSR | MSR | MSR | MSR | MSR | MSR | MSR | MSR |
| 0x370 | CMN | CMN | CMN | CMN | CMN | CMN | CMN | CMN |
| 0x378 | CMN | CMN | CMN | CMN | CMN | CMN | CMN | CMN |
| 0x380 | ORR | ORR | ORR | ORR | ORR | ORR | ORR | ORR |
| 0x388 | ORR | ORR | ORR | ORR | ORR | ORR | ORR | ORR |
| 0x390 | ORR | ORR | ORR | ORR | ORR | ORR | ORR | ORR |
| 0x398 | ORR | ORR | ORR | ORR | ORR | ORR | ORR | ORR |
| 0x3A0 | MOV | MOV | MOV | MOV | MOV | MOV | MOV | MOV |
| 0x3A8 | MOV | MOV | MOV | MOV | MOV | MOV | MOV | MOV |
| 0x3B0 | MOV | MOV | MOV | MOV | MOV | MOV | MOV | MOV |
| 0x3B8 | MOV | MOV | MOV | MOV | MOV | MOV | MOV | MOV |
| 0x3C0 | BIC | BIC | BIC | BIC | BIC | BIC | BIC | BIC |
| 0x3C8 | BIC | BIC | BIC | BIC | BIC | BIC | BIC | BIC |
| 0x3D0 | BIC | BIC | BIC | BIC | BIC | BIC | BIC | BIC |
| 0x3D8 | BIC | BIC | BIC | BIC | BIC | BIC | BIC | BIC |
| 0x3E0 | MVN | MVN | MVN | MVN | MVN | MVN | MVN | MVN |
| 0x3E8 | MVN | MVN | MVN | MVN | MVN | MVN | MVN | MVN |
| 0x3F0 | MVN | MVN | MVN | MVN | MVN | MVN | MVN | MVN |
| 0x3F8 | MVN | MVN | MVN | MVN | MVN | MVN | MVN | MVN |


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

- 000x xxx? ????  ???? ???? 1001 ???? (0x0000090 - 0x1FFFF9F) ワード
- 000x xxx0 ????  ???? ???? 1??0 ???? (0x0000090 - 0x1EFFFEF) ハーフワード

ビット 27..20 とビット 07..04 をつなげた組合せで取り得る数値
- 0x009, 0x019, 0x029, ... 0x1F9,
- 0x008, 0x00A, 0x00C, 0x00E, 0x028, 0x02A, 0x02C, 0x02E, ...,
  0x1E8, 0x1EA, 0x1EC, 0x1EE

ワード単位

| 演算 |  命令  | オペコード                         | 12ビット分類 |    備考    |
|:----:|:------:|:----------------------------------:|:-------------|:-----------|
| 0000 |  MUL   | 0000 000S rrrr rrrr rrrr 1001 rrrr | 0x009, 0x019 |            |
| 0001 |  MLA   | 0000 001S rrrr rrrr rrrr 1001 rrrr | 0x029, 0x039 |            |
| 0010 | UMAAL  | 0000 010S rrrr rrrr rrrr 1001 rrrr | 0x049, 0x059 | 未サポート |
| 0011 | 未定義 | 0000 011S rrrr rrrr rrrr 1001 rrrr | 0x069, 0x079 |            |
| 0100 | UMULL  | 0000 100S rrrr rrrr rrrr 1001 rrrr | 0x089, 0x099 |            |
| 0101 | UMLAL  | 0000 101S rrrr rrrr rrrr 1001 rrrr | 0x0A9, 0x0B9 |            |
| 0110 | SMULL  | 0000 110S rrrr rrrr rrrr 1001 rrrr | 0x0C9, 0x0D9 |            |
| 0111 | SMLAL  | 0000 111S rrrr rrrr rrrr 1001 rrrr | 0x0E9, 0x0F9 |            |

ハーフワード単位
- ただしいずれも未サポートの命令

| 演算 |  命令  | オペコード                         |  12 ビット分類  |
|:----:|:------:|:----------------------------------:|:----------------|
| 1000 | SMLA   | 0001 0000 ???? ???? ???? 1??0 0000 | 108 10A 10C 10E |
| 1001 | SMLAW  | 0001 0010 ???? ???? ???? 1??0 0000 | 128 12A 12C 12E |
| 1001 | SMULW  | 0001 0010 ???? ???? ???? 1??0 0000 | 128 12A 12C 12E |
| 1010 | SMLAL  | 0001 0100 ???? ???? ???? 1??0 0000 | 148 14A 14C 14E |
| 1011 | SMUL   | 0001 0110 ???? ???? ???? 1??0 0000 | 168 16A 16C 16E |
| 1100 | 未定義 | 0001 1000 ???? ???? ???? 1??0 0000 | 188 18A 18C 18E |
| 1101 | 未定義 | 0001 1010 ???? ???? ???? 1??0 0000 | 1A8 1AA 1AC 1AE |
| 1110 | 未定義 | 0001 1100 ???? ???? ???? 1??0 0000 | 1C8 1CA 1CC 1CE |
| 1111 | 未定義 | 0001 1110 ???? ???? ???? 1??0 0000 | 1E8 1EA 1EC 1EE |

###   04. ロード／ストア

- 010? ???0 ????  ???? ???? ???? ???? (0x4000000 - 0x5EFFFFF) STR
- 011? ???0 ????  ???? ???? ???? ???? (0x6000000 - 0x7EFFFFF) STR
- 010? ???1 ????  ???? ???? ???? ???? (0x4100000 - 0x5FFFFFF) LDR
- 011? ???1 ????  ???? ???? ???? ???? (0x6100000 - 0x7FFFFFF) LDR

除外
- ビット 25 (I=1) の時、ビット 4 は 0 でなければならない
    - 011? ???x ????  ???? ???? ???0 ???? は OK
    - 011? ???x ????  ???? ???? ???1 ???? は NG

ビット 27..20 とビット 07..04 をつなげた組合せで取り得る数値

|       | + 0 | + 1 | + 2 | + 3 | + 4 | + 5 | + 6 | + 7 |
|------:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 0x600 | STR | und | STR | und | STR | und | STR | und |
| 0x608 | STR | und | STR | und | STR | und | STR | und |
| 0x610 | LDR | und | LDR | und | LDR | und | LDR | und |
| 0x618 | LDR | und | LDR | und | LDR | und | LDR | und |
| 0x620 | STR | und | STR | und | STR | und | STR | und |
| 0x628 | STR | und | STR | und | STR | und | STR | und |
| 0x630 | LDR | und | LDR | und | LDR | und | LDR | und |
| 0x638 | LDR | und | LDR | und | LDR | und | LDR | und |
|  ...  | ... | ... | ... | ... | ... | ... | ... | ... |
| 0x7E0 | STR | und | STR | und | STR | und | STR | und |
| 0x7E8 | STR | und | STR | und | STR | und | STR | und |
| 0x7F0 | LDR | und | LDR | und | LDR | und | LDR | und |
| 0x7F8 | LDR | und | LDR | und | LDR | und | LDR | und |

###   05. ロード／ストア

- 000? ?0?x ????  ???? 0000 1xx1 ???? (0x0000090 - 0x1BFF0FF) : STR/LDR
- 000? ?1?x ????  ???? ???? 1xx1 ???? (0x0400090 - 0x1FFFFFF) : STR/LDR

除外
- ビット 06..05 の xx は 00 を除く（未使用）

ビット 27..20 とビット 07..04 をつなげた組合せで取り得る数値
- 0x009, 0x00B, 0x00D, 0x00F
- 0x019, 0x01B, 0x01D, 0x01F

| L | Op | 命令 |             オペコード             |  12 ビット分類  |
|:-:|:--:|:----:|:----------------------------------:|:----------------|
| 0 | 00 | ---- |
| 0 | 01 | STRH |
| 0 | 10 | LDRD |
| 0 | 11 | STRD |
| 1 | 00 | ---- |
| 1 | 01 | LDRH |
| 1 | 10 | LDRSB|
| 1 | 11 | LDRSH|

###   06. スタック

- 100? ???0 ????  ???? ???? ???? ???? STM
- 100? ???1 ????  ???? ???? ???? ???? LDM

###   07. PSR 操作

- 0001 0?00 1111  ???? 0000 0000 0000 (0x1?F0000 - 0x1?FF000 ?=0,4) : MRS
- 0001 0?10 ????  ???? 0000 0000 ???? (0x1?00000 - 0x1?FF00F ?=2,6) : MSR
- 0011 0?10 ????  ???? ???? ???? ???? (0x3?00000 - 0x3?FFFFF ?=2,6) : MSR

###   08. スワップ

- 0001 0?00 ????  ???? 0000 1001 ???? (0x1?00090 - 0x1?FF09F ?=0,4) : SWP

###   09. コプロセッサ命令

- 1110 xxx0 ????  ???? ???? ???1 ???? (0xE000010 - 0xEEFFFFF) : MCR
- 1110 xxx1 ????  ???? ???? ???1 ???? (0xE100010 - 0xEFFFFFF) : MRC
