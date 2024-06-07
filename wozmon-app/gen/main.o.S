
main.o:     file format elf32-littlearm


Disassembly of section .text:

00000000 <fibbonacci>:
   0:	b570      	push	{r4, r5, r6, lr}
   2:	0004      	movs	r4, r0
   4:	2500      	movs	r5, #0
   6:	2c00      	cmp	r4, #0
   8:	d007      	beq.n	1a <fibbonacci+0x1a>
   a:	2c01      	cmp	r4, #1
   c:	d005      	beq.n	1a <fibbonacci+0x1a>
   e:	1e60      	subs	r0, r4, #1
  10:	f7ff fff6 	bl	0 <fibbonacci>
  14:	3c02      	subs	r4, #2
  16:	182d      	adds	r5, r5, r0
  18:	e7f5      	b.n	6 <fibbonacci+0x6>
  1a:	1960      	adds	r0, r4, r5
  1c:	bd70      	pop	{r4, r5, r6, pc}

0000001e <sqrt>:
  1e:	4340      	muls	r0, r0
  20:	4770      	bx	lr
