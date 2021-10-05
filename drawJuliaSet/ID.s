        .data
id1:    .word   0
id2:    .word   0
id3:    .word   0
sum:    .word   0
p:      .word   'p'
command:    .word  0
line:   .asciz  "\n"
cha:    .asciz  "%s"
int:    .asciz  "%d"
idmsg0: .asciz  "*****Input ID*****\n"
idmsg1: .asciz  "** Please Enter Member 1 ID:**\n"
idmsg2: .asciz  "** Please Enter Member 2 ID:**\n"
idmsg3: .asciz  "** Please Enter Member 3 ID:**\n"
idmsg4: .asciz  "** Please Enter Command **\n"
idmsg5: .asciz  "*****Print Team Member ID and ID Summation*****\n"
idmsg6: .asciz  "ID Summation = %d\n"
idmsg7: .asciz  "*****End Print*****\n"
        .text
        .globl  ID

ID:   stmfd   sp!, {lr}
        ldr     r0, =idmsg0
        bl      printf      @ print Input word

        ldr     r0, =idmsg1
        bl      printf      @ print enter id1

        ldr     r0, =int
        ldr     r1, =id1
        bl      scanf       @ scanf id1

        ldr     r0, =idmsg2
        bl      printf      @ print enter id2

        ldr     r0, =int
        ldr     r1, =id2
        bl      scanf       @ scanf id2

        ldr     r0, =idmsg3
        bl      printf      @ print enter id3

        ldr     r0, =int
        ldr     r1, =id3
        bl      scanf       @ scanf id3

        ldr     r0, =idmsg4
        bl      printf      @ print enter command

        ldr     r0, =cha
        ldr     r1, =command
        bl      scanf       @ scanf command

        ldr     r1, =command
        ldr     r4, =p
        ldr     r1, [r1]    @ lord word
        ldr     r4, [r4]    @ lord word
        cmp     r1, r4      @ compare if command = p

        ldrpl   r0, =idmsg5
        bleq    printf      @ if z set print all id but if command != p not print

        ldr     r4, =id1    @ to add sum
        ldr     r0, [r4]    @ load id1 value
        ldr     r1, [r4, #4]!   @ load id2 value in r1 and update r2 to id3 address
        add     r0, r0, r1  @ r0 = id1 + id2
        ldr     r1, [r4, #4]@ load id3 in r1
        adds    r0, r0, r1  @ r0 = id1 + id2 + id3

        ldr     r1, =sum
        strpl   r0, [r1]    @ if sum >= 0 store summation in label sum

        ldr     r0, =int    @ if want to print int need %d
        ldr     r1, =id1
        ldr     r1, [r1]
        blpl    printf      @ if sum >= 0 print id1

        ldr     r0, =line
        blpl    printf      @ if sum >= 0 print \n

        ldr     r0, =int
        ldr     r1, =id2
        ldr     r1, [r1]
        blpl    printf      @ if sum >= 0 print id2

        ldr     r0, =line
        blpl    printf      @ if sum >= 0 print \n

        ldr     r0, =int
        ldr     r1, =id3
        ldr     r1, [r1]
        blpl    printf      @ if sum >= 0 print id3

        ldr     r0, =line
        blpl    printf      @ if sum >= 0 print \n

        ldr     r0, =line
        blpl    printf      @ if sum >= 0 print \n

        ldr     r0, =idmsg6
        ldr     r1, =sum
        ldr     r1, [r1]
        blpl    printf      @ if sum >= 0 print sum

        ldr     r0, =idmsg7
        blpl    printf      @ if sum >= 0 print end

        mov     r0, #0
        ldmfd   sp!,{lr}
        mov     pc, lr

        .globl  Getid1
        .globl  Getid2
        .globl  Getid3
        .globl  Getsum
Getid1:
        ldr     r0, =id1
        ldr     r0, [r0]
        mov     pc, lr
Getid2:
        ldr     r0, =id2
        ldr     r0, [r0]
        mov     pc, lr
Getid3:
        ldr     r0, =id3
        ldr     r0, [r0]
        mov     pc, lr
Getsum:
        ldr     r0, =sum
        ldr     r0, [r0]
        mov     pc, lr

