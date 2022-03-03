 .syntax unified
	.cpu cortex-m3
	.fpu softvfp
	.thumb

.global	__delay

//       Arg: Number of cycles (min 32)
__delay:
        LSRS    R1, R0, #3
        SUBS    R1, R1, #3        
L1:
        NOP
        NOP
        NOP
        SUBS    R1, R1, #1
        BNE     L1
        
        AND     R1, R0, #7
        TST     R1, #1
        BEQ     L2
        NOP
        B       L2
L2:
        TST     R1, #2
        BEQ     L3
        NOP
        NOP
        B       L3
L3:
        TST     R1, #4
        BEQ     L5
        NOP
        B       L4
L4:        
        B       L5
L5:
        NOP
        BX      LR
