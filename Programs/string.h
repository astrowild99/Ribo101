;string.h
;
;initialize the lcd without anything in a, b, d
.LCDINIT     LDA .LCDCOMMAND0
             LCI
             LDA .LCDCOMMAND1
             LCI
             LDA .LCDCOMMAND2
             LCI
             LDA .LCDCOMMAND3
             LCI
             ENT
             
;print the string with a= length, b= startOfString
.LCDPRINT    STA .STRDATA
             STB .STRLENGTH
.STRSTART    LDA .STRLENGTH
             SUN 1
             STA .STRLENGTH
             JZ  .STREND
             LDA .STRDATA
             LDX
             LCD
             LDA .STRDATA
             ADN 1
             STA .STRDATA
             JMP .STRSTART
.STREND      ENT

;move the cursor on the lines
.LCDRET1     LDA .LCDCOMMANDLINE1
             LCI
             ENT
.LCDRET2     LDA .LCDCOMMANDLINE2
             LCI
             ENT
             
;convert single number in a character and out to LCD
.INTTOCHAR   ADN 0x30
             LCD
             SUN 0x30
             ENT
             
;data:
.STRLENGTH   0
.STRDATA     0
.LCDCOMMAND0 0x38
.LCDCOMMAND1 0x06
.LCDCOMMAND2 0x01
.LCDCOMMAND3 0x0f
.LCDCOMMANDLINE1 0x80
.LCDCOMMANDLINE2 0xc0





