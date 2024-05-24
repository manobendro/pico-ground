	.file	"main.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
.global	sum
	.type	sum, @function
sum:
	push r28
	push r29
	rcall .
	rcall .
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 4 */
/* stack size = 6 */
.L__stack_usage = 6
	std Y+2,r25
	std Y+1,r24
	std Y+4,r23
	std Y+3,r22
	ldd r18,Y+1
	ldd r19,Y+2
	ldd r24,Y+3
	ldd r25,Y+4
	add r24,r18
	adc r25,r19
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop r29
	pop r28
	ret
	.size	sum, .-sum
.global	sum1
	.type	sum1, @function
sum1:
	push r28
	push r29
	rcall .
	rcall .
	rcall .
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 6 */
/* stack size = 8 */
.L__stack_usage = 8
	std Y+2,r25
	std Y+1,r24
	std Y+4,r23
	std Y+3,r22
	std Y+6,r21
	std Y+5,r20
	ldd r18,Y+1
	ldd r19,Y+2
	ldd r24,Y+3
	ldd r25,Y+4
	add r18,r24
	adc r19,r25
	ldd r24,Y+5
	ldd r25,Y+6
	add r24,r18
	adc r25,r19
/* epilogue start */
	adiw r28,6
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
	pop r29
	pop r28
	ret
	.size	sum1, .-sum1
.global	sum2
	.type	sum2, @function
sum2:
	push r28
	push r29
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,8
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
/* prologue: function */
/* frame size = 8 */
/* stack size = 10 */
.L__stack_usage = 10
	std Y+2,r25
	std Y+1,r24
	std Y+4,r23
	std Y+3,r22
	std Y+6,r21
	std Y+5,r20
	std Y+8,r19
	std Y+7,r18
	ldd r18,Y+1
	ldd r19,Y+2
	ldd r24,Y+3
	ldd r25,Y+4
	add r18,r24
	adc r19,r25
	ldd r24,Y+5
	ldd r25,Y+6
	add r18,r24
	adc r19,r25
	ldd r24,Y+7
	ldd r25,Y+8
	add r24,r18
	adc r25,r19
/* epilogue start */
	adiw r28,8
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
	pop r29
	pop r28
	ret
	.size	sum2, .-sum2
.global	do_sum
	.type	do_sum, @function
do_sum:
	push r28
	push r29
	rcall .
	rcall .
	rcall .
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 6 */
/* stack size = 8 */
.L__stack_usage = 8
	std Y+4,r25
	std Y+3,r24
	std Y+6,r23
	std Y+5,r22
	ldd r24,Y+5
	ldd r25,Y+6
	mov r30,r24
	mov r31,r25
	ld r18,Z
	ldd r19,Z+1
	ldd r24,Y+3
	ldd r25,Y+4
	mov r30,r24
	mov r31,r25
	ld r24,Z
	ldd r25,Z+1
	mov r22,r18
	mov r23,r19
	rcall sum
	std Y+2,r25
	std Y+1,r24
	nop
/* epilogue start */
	adiw r28,6
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
	pop r29
	pop r28
	ret
	.size	do_sum, .-do_sum
	.ident	"GCC: (AVR_8_bit_GNU_Toolchain_3.7.0_1796) 7.3.0"
