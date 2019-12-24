;this is the demo for the exam
;the main purpose is to show all the ability of the computer
;stay within the 256 bytes

;loading libs
#string.h
#math.h

;start of the program itself
.DEMOSTART       INT .LCDINIT
.GREETINGS       LDN .DEMOSTRING
                 LDB .DEMOSTRINGLENGTH
                 INT .LCDPRINT
                 
;moltiplication table
.TABELLINAINIT   INT .LCDINIT
                 LDN 1
                 STA .CURRENTNUM
                 LDN .TABELLINESTRING
                 LDB .TABELLINESTRINGLENGTH
                 INT .LCDPRINT
.TABELLINASTART  LDA .CURRENTNUM
                 ADN 1
                 STA .CURRENTNUM
                 INT .INTTOCHAR
                 CMP .MAXBASE
                 JZ  .POTENZEINIT
                 LDB .MAXNUM
                 INT .MULTIPLY
                 JMP .TABELLINASTART
                 
;various powers
.POTENZEINIT     LDN 1
                 STA .CURRENTNUM
.POTENZESTART    INT .LCDINIT
                 LDN .POTENZESTRING
                 LDB .POTENZESTRINGLENGTH
                 INT .LCDPRINT
                 LDA .CURRENTNUM
                 ADN 1
                 STA .CURRENTNUM
                 INT .INTTOCHAR
                 CMP .MAXBASE
                 JZ .DEMOEND
                 MOV A,B
                 LDN 2
                 INT .POWER
                 OUT
                 JMP .POTENZESTART
                 
;end of the demo
.DEMOEND         INT .LCDINIT
                 LDN .GRAZIESTRING
                 LDB .GRAZIESTRINGLENGTH
                 INT .LCDPRINT
                 JMP .DEMOSTART
;strings:
.DEMOSTRING "Salve"
.DEMOSTRINGLENGTH 6
.TABELLINESTRING "Tabellina:"
.TABELLINESTRINGLENGTH 11
.POTENZESTRING "Potenze:2^"
.POTENZESTRINGLENGTH 11
.GRAZIESTRING "Grazie"
.GRAZIESTRINGLENGTH 7

;data:
.MAXBASE 8
.MAXNUM 10
.CURRENTNUM 1
.MAXPOW 2
