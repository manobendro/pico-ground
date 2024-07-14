
echo.elf:     file format elf32-littlearm


Disassembly of section .text.startup:

00000000 <_start>:
   0:	a006      	add	r0, pc, #24	; (adr r0, 1c <UART0_BASE>)
   2:	6800      	ldr	r0, [r0, #0]
   4:	a106      	add	r1, pc, #24	; (adr r1, 20 <TEXT>)

00000006 <WRITE_LOOP>:
   6:	780a      	ldrb	r2, [r1, #0]
   8:	3101      	adds	r1, #1
   a:	2a00      	cmp	r2, #0
   c:	d005      	beq.n	1a <DONE>

0000000e <WAIT_TXFE>:
   e:	6983      	ldr	r3, [r0, #24]
  10:	2480      	movs	r4, #128	; 0x80
  12:	4223      	tst	r3, r4
  14:	d0fb      	beq.n	e <WAIT_TXFE>
  16:	7002      	strb	r2, [r0, #0]
  18:	e7f5      	b.n	6 <WRITE_LOOP>

0000001a <DONE>:
  1a:	e7fe      	b.n	1a <DONE>

0000001c <UART0_BASE>:
  1c:	40034000 	.word	0x40034000

00000020 <TEXT>:
  20:	6c6c6548 	.word	0x6c6c6548
  24:	57202c6f 	.word	0x57202c6f
  28:	646c726f 	.word	0x646c726f
  2c:	000a      	.short	0x000a
