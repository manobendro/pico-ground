
main.o:     file format elf32-littlearm


Disassembly of section .text:

00000000 <sum>:
   0:	e52db004 	push	{fp}		; (str fp, [sp, #-4]!)
   4:	e28db000 	add	fp, sp, #0
   8:	e24dd00c 	sub	sp, sp, #12
   c:	e50b0008 	str	r0, [fp, #-8]
  10:	e50b100c 	str	r1, [fp, #-12]
  14:	e51b2008 	ldr	r2, [fp, #-8]
  18:	e51b300c 	ldr	r3, [fp, #-12]
  1c:	e0823003 	add	r3, r2, r3
  20:	e1a00003 	mov	r0, r3
  24:	e28bd000 	add	sp, fp, #0
  28:	e49db004 	pop	{fp}		; (ldr fp, [sp], #4)
  2c:	e12fff1e 	bx	lr

00000030 <sum1>:
  30:	e52db004 	push	{fp}		; (str fp, [sp, #-4]!)
  34:	e28db000 	add	fp, sp, #0
  38:	e24dd014 	sub	sp, sp, #20
  3c:	e50b0008 	str	r0, [fp, #-8]
  40:	e50b100c 	str	r1, [fp, #-12]
  44:	e50b2010 	str	r2, [fp, #-16]
  48:	e51b2008 	ldr	r2, [fp, #-8]
  4c:	e51b300c 	ldr	r3, [fp, #-12]
  50:	e0822003 	add	r2, r2, r3
  54:	e51b3010 	ldr	r3, [fp, #-16]
  58:	e0823003 	add	r3, r2, r3
  5c:	e1a00003 	mov	r0, r3
  60:	e28bd000 	add	sp, fp, #0
  64:	e49db004 	pop	{fp}		; (ldr fp, [sp], #4)
  68:	e12fff1e 	bx	lr

0000006c <sum2>:
  6c:	e52db004 	push	{fp}		; (str fp, [sp, #-4]!)
  70:	e28db000 	add	fp, sp, #0
  74:	e24dd014 	sub	sp, sp, #20
  78:	e50b0008 	str	r0, [fp, #-8]
  7c:	e50b100c 	str	r1, [fp, #-12]
  80:	e50b2010 	str	r2, [fp, #-16]
  84:	e50b3014 	str	r3, [fp, #-20]	; 0xffffffec
  88:	e51b2008 	ldr	r2, [fp, #-8]
  8c:	e51b300c 	ldr	r3, [fp, #-12]
  90:	e0822003 	add	r2, r2, r3
  94:	e51b3010 	ldr	r3, [fp, #-16]
  98:	e0822003 	add	r2, r2, r3
  9c:	e51b3014 	ldr	r3, [fp, #-20]	; 0xffffffec
  a0:	e0823003 	add	r3, r2, r3
  a4:	e1a00003 	mov	r0, r3
  a8:	e28bd000 	add	sp, fp, #0
  ac:	e49db004 	pop	{fp}		; (ldr fp, [sp], #4)
  b0:	e12fff1e 	bx	lr

000000b4 <do_sum>:
  b4:	e92d4800 	push	{fp, lr}
  b8:	e28db004 	add	fp, sp, #4
  bc:	e24dd010 	sub	sp, sp, #16
  c0:	e50b0010 	str	r0, [fp, #-16]
  c4:	e50b1014 	str	r1, [fp, #-20]	; 0xffffffec
  c8:	e51b3010 	ldr	r3, [fp, #-16]
  cc:	e5932000 	ldr	r2, [r3]
  d0:	e51b3014 	ldr	r3, [fp, #-20]	; 0xffffffec
  d4:	e5933000 	ldr	r3, [r3]
  d8:	e1a01003 	mov	r1, r3
  dc:	e1a00002 	mov	r0, r2
  e0:	ebfffffe 	bl	0 <sum>
  e4:	e50b0008 	str	r0, [fp, #-8]
  e8:	e1a00000 	nop			; (mov r0, r0)
  ec:	e24bd004 	sub	sp, fp, #4
  f0:	e8bd4800 	pop	{fp, lr}
  f4:	e12fff1e 	bx	lr

Disassembly of section .comment:

00000000 <.comment>:
   0:	43434700 	movtmi	r4, #14080	; 0x3700
   4:	3128203a 			; <UNDEFINED> instruction: 0x3128203a
   8:	2d393a35 	vldmdbcs	r9!, {s6-s58}
   c:	39313032 	ldmdbcc	r1!, {r1, r4, r5, ip, sp}
  10:	2d34712d 	ldfcss	f7, [r4, #-180]!	; 0xffffff4c
  14:	75627530 	strbvc	r7, [r2, #-1328]!	; 0xfffffad0
  18:	3175746e 	cmncc	r5, lr, ror #8
  1c:	2e392029 	cdpcs	0, 3, cr2, cr9, cr9, {1}
  20:	20312e32 	eorscs	r2, r1, r2, lsr lr
  24:	39313032 	ldmdbcc	r1!, {r1, r4, r5, ip, sp}
  28:	35323031 	ldrcc	r3, [r2, #-49]!	; 0xffffffcf
  2c:	65722820 	ldrbvs	r2, [r2, #-2080]!	; 0xfffff7e0
  30:	7361656c 	cmnvc	r1, #108, 10	; 0x1b000000
  34:	5b202965 	blpl	80a5d0 <do_sum+0x80a51c>
  38:	2f4d5241 	svccs	0x004d5241
  3c:	2d6d7261 	sfmcs	f7, 2, [sp, #-388]!	; 0xfffffe7c
  40:	72622d39 	rsbvc	r2, r2, #3648	; 0xe40
  44:	68636e61 	stmdavs	r3!, {r0, r5, r6, r9, sl, fp, sp, lr}^
  48:	76657220 	strbtvc	r7, [r5], -r0, lsr #4
  4c:	6f697369 	svcvs	0x00697369
  50:	3732206e 	ldrcc	r2, [r2, -lr, rrx]!
  54:	39393537 	ldmdbcc	r9!, {r0, r1, r2, r4, r5, r8, sl, ip, sp}
  58:	Address 0x0000000000000058 is out of bounds.


Disassembly of section .ARM.attributes:

00000000 <.ARM.attributes>:
   0:	00002941 	andeq	r2, r0, r1, asr #18
   4:	61656100 	cmnvs	r5, r0, lsl #2
   8:	01006962 	tsteq	r0, r2, ror #18
   c:	0000001f 	andeq	r0, r0, pc, lsl r0
  10:	00543405 	subseq	r3, r4, r5, lsl #8
  14:	01080206 	tsteq	r8, r6, lsl #4
  18:	04120109 	ldreq	r0, [r2], #-265	; 0xfffffef7
  1c:	01150114 	tsteq	r5, r4, lsl r1
  20:	01180317 	tsteq	r8, r7, lsl r3
  24:	011a0119 	tsteq	sl, r9, lsl r1
  28:	Address 0x0000000000000028 is out of bounds.

