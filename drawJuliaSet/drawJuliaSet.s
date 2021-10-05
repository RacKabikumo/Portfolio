            .global	__aeabi_idiv
            .text
            .globl  drawJuliaSet
            .type	drawJuliaSet, %function

drawJuliaSet:   stmfd   sp!,{lr}
                @ sp + 4 is frame
                sub     sp, sp, #48
                @ sp + 52 is frame
                str     r0, [sp,#4]         @ cx
                str     r1, [sp,#8]         @ cy
                str     r2, [sp,#12]        @ width
                str     r3, [sp,#16]        @ height
                mov     r4, #255            @ max
                mov     r5, #0              @ x = 0
                b       for1
for1:           ldr     r2, [sp,#12]        @ ldr width in r2
                cmp     r5, r2              @ cmp if x < width
                bge     end
                @********************@
                movlt   r6, #0              @ y = 0
                @********************@
                b       for2
for2:           ldr     r3, [sp,#16]        @ ldr height in r3
                cmp     r6, r3              @ cmp if y < height
                @********************@
                addge   r5, r5, #1          @ if y >= height mean need break then x++
                @********************@
                bge     for1
                ldr     r2, [sp,#12]        @ ldr width in r2
                sub     ip, r5, r2, asr #1  @ x - width>>1 (right move 1)
                mov     r0, #1000           @ r0 = 1000
                add     r0, r0, #500        @ r0 = 1500
                mul     ip, r0, ip          @ 1500 * ( x-width>>1 )
                mov     r0, ip
                ldr     r2, [sp,#12]        @ ldr   width
                mov     r1, r2, asr #1      @ width >> 1
                bl      __aeabi_idiv        @ div
                str     r0, [sp,#20]        @ save zx
                ldr     r3, [sp,#16]        @ ldr height in r3
                sub     ip, r6, r3, asr #1  @ y - height>>1
                mov     r0, #1000           @ r0 = 1000
                mul     ip, r0, ip          @ 1000 * (y-height>>1)
                mov     r0, ip
                ldr     r3, [sp,#16]        @ ldr height
                mov     r1, r3, asr #1      @ height >> 1
                bl      __aeabi_idiv        @ div
                str     r0, [sp,#24]        @ save zy
                mov     r4, #255            @ i = max
                b       while
afterwhile:     strh    r4,[sp,#32]         @ save half word i
                ldrh    r4,[sp,#32]         @ ldr half word i
                and     r0, r4, #0xff       @ i&0xff
                orr     r0, r0, r0, asl #8  @ i&0xff | i&0xff
                mvn     r1, r0              @ ~color
                mov     r2, #255            @ r2 = 255
                add     r3, r2, #2          @ r3 = 257
                mul     r2, r2, r3          @ r2 = 65535(0xffff)
                and     r0, r1, r2          @ ~color&0xffff
                mov     r2, #640            @ max width
                mul     r1, r2, r6          @ y *640
                add     r1, r1, r5          @ [y][x]'s position
                mov     r2, #2
                mul     r1, r1, r2          @ 2byte(16bit)
                add     r2, sp, #52         @ r2 = frame start address?
                add     r2, r2, r1          @ r2 = now to save position
                strh    r0,[r2]             @ save color in frame[y][x]
                add     r6, r6, #1          @ y++
                b       for2

while:          cmp     r4, #0
                ble     afterwhile          @ if i <= 0 return
                ldrgt   ip,[sp,#20]         @ ldr zx in ip
                mulgt   ip, ip, ip          @ zx*zx
                ldrgt   r0,[sp,#24]         @ ldr zy in r0
                mulgt   r0, r0, r0          @ zy*zy
                addgt   ip, ip, r0          @ zx*zx+zy*zy
                movgt   r1, #256            @ r1 = 256
                movgt   r2, #125            @ r2 = 125
                mulgt   r2, r2, r2          @ r2 = 15625
                mulgt   r1, r1, r2          @ r1 = 4000000
                cmpgt   ip, r1
                bge     afterwhile          @ if ip >= r1 return
                sub     ip, ip, r0, asl #1  @ zx*zx+zy*zy  - zy*zy*2
                mov     r1, #1000           @ r1 = 1000
                mov     r0, ip
                bl      __aeabi_idiv        @ div
                ldr     r2,[sp,#4]          @ ldr cx in r2
                add     ip, r0, r2          @ ip = zx*zx-zy*zy/1000+cx
                str     ip,[sp,#28]         @ save tmp
                ldr     r0,[sp,#20]         @ ldr zx in r0
                ldr     r3,[sp,#24]         @ ldr zy in r3
                mul     ip, r0, r3          @ zx*zy
                mov     r0, #2
                mul     ip, ip, r0          @ zx*zy*2
                mov     r0, #0
                mov     r1, #1000           @ r1 = 1000
                mov     r0, ip
                bl      __aeabi_idiv        @ div
                ldr     r2,[sp,#8]          @ ldr cy in r2
                add     ip, r0, r2          @ 2*zx*zy/1000+cy
                str     ip,[sp,#24]         @ save zy
                ldr     ip,[sp,#28]         @ ldr tmp in ip
                str     ip,[sp,#20]         @ save zx
                sub     r4, r4, #1          @ i--
                b       while
end:            add     sp, sp, #48
                adds    r14, r0, r1
                ldmfd   sp!,{lr}
                mov     pc, lr
