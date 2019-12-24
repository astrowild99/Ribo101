.START     LDA .BASE
           STA .NUMERO
.LOOP      INT .STAMPA
           LDA .NUMERO
           ADD .NUMERO
           STA .NUMERO
           OUT
           JC .INCREMENT
           JNC .LOOP
.INCREMENT LDA .BASE
           ADN 1
           STA .BASE
           JMP .START
.BASE 2
.NUMERO 0
.STAMPA    LDA .COM0
           LCI 
           LDA .COM1
           LCI
           LDA .COM2
           LCI
           LDA .LENGTH
           ADN &LENGTH
           STA .LENGTH
.STAMPAST  LDA
.STAMPAIN  .STRINGA
           LCD
           LDA .STAMPAIN
           ADN 1
           STA .STAMPAIN
           CMP .LENGTH
           JZ .STAMPAEND
           JMP .STAPAST
.STAPAEND  ENT
.LENGTH    5
.STRINGA   "Bravo"
.COM0      0x38
.COM1      0x0f
.COM2      0x06
