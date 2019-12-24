;math.h
;
;moltiplication
.MULTIPLY        STA .MATHBASE
                 STA .MATHRES
                 STB .MATHCONT
.MULLOOP         LDA .MATHCONT
                 SUN 1
                 STA .MATHCONT
                 LDA .MATHRES
                 JZ .MULEND
                 ADD .MATHBASE
                 STA .MATHRES
                 OUT
                 JMP .MULLOOP
.MULEND          ENT
;power
.POWER           STA .POWRES
                 STB .POWCONT
.POWLOOP         LDA .POWCONT
                 SUN 1
                 STA .POWCONT
                 LDA .POWRES
                 JZ .POWEND
                 ADD .POWRES
                 STA .POWRES
                 JMP .POWLOOP
.POWEND          ENT

:division
;.DIVIDE          STA .MATHRES
;                 STB .MATHBASE
;                 LDN 0
;                 STA .MATHCONT
;.DIVLOOP         LDA .MATHRES
;                 SUB .MATHBASE
;                 STA .MATHRES
;                 JNC .DIVEND
;                 JZ .DIVEND
;                 LDA .MATHCONT
;                 ADN 1
;                 STA .MATHCONT
;                 JMP .DIVLOOP
;.DIVEND          MOV A,B
;                 LDA .MATHCONT
;                 ENT

;datas common to all the operations, to be updatede everytime
.MATHBASE 0
.MATHRES 0
.MATHCONT 0
.POWRES 0
-POWCONT 0
