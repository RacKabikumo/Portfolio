        .data
team:   .asciz  "Team 03\n"
name1:  .asciz  "Lin Cheng Yan\n"
name2:  .asciz  "Lin Cheng Yan\n"
name3:  .asciz  "Chang Chun Jung\n"
namemsg1:   .asciz  "*****Print Name*****\n"
namemsg2:   .asciz  "*****End Print*****\n"

        .text
        .globl  Name

Name:
        stmfd   sp!, {lr}
        ldr     r0, =namemsg1
        bl      printf

        ldr     r0, =team
        bl      printf      @ print team

        ldr     r0, =name1
        bl      printf      @ print member 1 name

        ldr     r0, =name2
        bl      printf      @print member 2 name

        ldr     r0, =name3
        bl      printf      @ print member 3 name

        ldr     r3, =namemsg2
        mov     r4, #-1      @ because c = 0 but sbc will more -1
        sbcs    r0, r3, r4 @ r0 = r3(address) - r4(-1) + c - 1 > 0

        blpl    printf      @ pl == (N = 0) means r0 >= 0 print end

        sbc     r1, r3, #-1
        subs    r1, r1, r0      @ if r1 = 0 stand do not print end
        moveq   r0, r1			@ if z set do this
        movne   r0, #0			@ if z clear do this
        ldmfd   sp!,{lr}
        mov     pc, lr

        .globl  Getname1
        .globl  Getname2
        .globl  Getname3
        .globl  Getteam
Getname1:
        ldr     r0, =name1
        mov     pc, lr

Getname2:
        ldr     r0, =name2
        mov     pc, lr

Getname3:
        ldr     r0, =name3
        mov     pc, lr

Getteam:
        ldr     r0, =team
        mov     pc, lr
