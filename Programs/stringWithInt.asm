;including string.h
#string.h
#math.h
;printing a string by using a int in string.h
.START      INT .LCDINIT
            INT .LCDRET1
            INT .LCDRET2
            LDN .DATA
            OUT
            LDB .LENGTH
            INT .LCDPRINT
            LDN 4
            INT .INTTOCHAR
            MOV A,B
            LDN 5
            INT .INTTOCHAR
            INT .MULTIPLY
            OUT
            HLT
.LENGTH     13
.DATA       "Frugis Merda"

;string library
            
