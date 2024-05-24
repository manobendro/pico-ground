
main-avr.o:     file format elf32-avr


Disassembly of section .text:

00000000 <sum>:
   0:	cf 93       	push	r28
   2:	df 93       	push	r29
   4:	00 d0       	rcall	.+0      	; 0x6 <sum+0x6>
   6:	00 d0       	rcall	.+0      	; 0x8 <sum+0x8>
   8:	cd b7       	in	r28, 0x3d	; 61
   a:	de b7       	in	r29, 0x3e	; 62
   c:	9a 83       	std	Y+2, r25	; 0x02
   e:	89 83       	std	Y+1, r24	; 0x01
  10:	7c 83       	std	Y+4, r23	; 0x04
  12:	6b 83       	std	Y+3, r22	; 0x03
  14:	29 81       	ldd	r18, Y+1	; 0x01
  16:	3a 81       	ldd	r19, Y+2	; 0x02
  18:	8b 81       	ldd	r24, Y+3	; 0x03
  1a:	9c 81       	ldd	r25, Y+4	; 0x04
  1c:	82 0f       	add	r24, r18
  1e:	93 1f       	adc	r25, r19
  20:	0f 90       	pop	r0
  22:	0f 90       	pop	r0
  24:	0f 90       	pop	r0
  26:	0f 90       	pop	r0
  28:	df 91       	pop	r29
  2a:	cf 91       	pop	r28
  2c:	08 95       	ret

0000002e <sum1>:
  2e:	cf 93       	push	r28
  30:	df 93       	push	r29
  32:	00 d0       	rcall	.+0      	; 0x34 <sum1+0x6>
  34:	00 d0       	rcall	.+0      	; 0x36 <sum1+0x8>
  36:	00 d0       	rcall	.+0      	; 0x38 <sum1+0xa>
  38:	cd b7       	in	r28, 0x3d	; 61
  3a:	de b7       	in	r29, 0x3e	; 62
  3c:	9a 83       	std	Y+2, r25	; 0x02
  3e:	89 83       	std	Y+1, r24	; 0x01
  40:	7c 83       	std	Y+4, r23	; 0x04
  42:	6b 83       	std	Y+3, r22	; 0x03
  44:	5e 83       	std	Y+6, r21	; 0x06
  46:	4d 83       	std	Y+5, r20	; 0x05
  48:	29 81       	ldd	r18, Y+1	; 0x01
  4a:	3a 81       	ldd	r19, Y+2	; 0x02
  4c:	8b 81       	ldd	r24, Y+3	; 0x03
  4e:	9c 81       	ldd	r25, Y+4	; 0x04
  50:	28 0f       	add	r18, r24
  52:	39 1f       	adc	r19, r25
  54:	8d 81       	ldd	r24, Y+5	; 0x05
  56:	9e 81       	ldd	r25, Y+6	; 0x06
  58:	82 0f       	add	r24, r18
  5a:	93 1f       	adc	r25, r19
  5c:	26 96       	adiw	r28, 0x06	; 6
  5e:	0f b6       	in	r0, 0x3f	; 63
  60:	f8 94       	cli
  62:	de bf       	out	0x3e, r29	; 62
  64:	0f be       	out	0x3f, r0	; 63
  66:	cd bf       	out	0x3d, r28	; 61
  68:	df 91       	pop	r29
  6a:	cf 91       	pop	r28
  6c:	08 95       	ret

0000006e <sum2>:
  6e:	cf 93       	push	r28
  70:	df 93       	push	r29
  72:	cd b7       	in	r28, 0x3d	; 61
  74:	de b7       	in	r29, 0x3e	; 62
  76:	28 97       	sbiw	r28, 0x08	; 8
  78:	0f b6       	in	r0, 0x3f	; 63
  7a:	f8 94       	cli
  7c:	de bf       	out	0x3e, r29	; 62
  7e:	0f be       	out	0x3f, r0	; 63
  80:	cd bf       	out	0x3d, r28	; 61
  82:	9a 83       	std	Y+2, r25	; 0x02
  84:	89 83       	std	Y+1, r24	; 0x01
  86:	7c 83       	std	Y+4, r23	; 0x04
  88:	6b 83       	std	Y+3, r22	; 0x03
  8a:	5e 83       	std	Y+6, r21	; 0x06
  8c:	4d 83       	std	Y+5, r20	; 0x05
  8e:	38 87       	std	Y+8, r19	; 0x08
  90:	2f 83       	std	Y+7, r18	; 0x07
  92:	29 81       	ldd	r18, Y+1	; 0x01
  94:	3a 81       	ldd	r19, Y+2	; 0x02
  96:	8b 81       	ldd	r24, Y+3	; 0x03
  98:	9c 81       	ldd	r25, Y+4	; 0x04
  9a:	28 0f       	add	r18, r24
  9c:	39 1f       	adc	r19, r25
  9e:	8d 81       	ldd	r24, Y+5	; 0x05
  a0:	9e 81       	ldd	r25, Y+6	; 0x06
  a2:	28 0f       	add	r18, r24
  a4:	39 1f       	adc	r19, r25
  a6:	8f 81       	ldd	r24, Y+7	; 0x07
  a8:	98 85       	ldd	r25, Y+8	; 0x08
  aa:	82 0f       	add	r24, r18
  ac:	93 1f       	adc	r25, r19
  ae:	28 96       	adiw	r28, 0x08	; 8
  b0:	0f b6       	in	r0, 0x3f	; 63
  b2:	f8 94       	cli
  b4:	de bf       	out	0x3e, r29	; 62
  b6:	0f be       	out	0x3f, r0	; 63
  b8:	cd bf       	out	0x3d, r28	; 61
  ba:	df 91       	pop	r29
  bc:	cf 91       	pop	r28
  be:	08 95       	ret

000000c0 <do_sum>:
  c0:	cf 93       	push	r28
  c2:	df 93       	push	r29
  c4:	00 d0       	rcall	.+0      	; 0xc6 <do_sum+0x6>
  c6:	00 d0       	rcall	.+0      	; 0xc8 <do_sum+0x8>
  c8:	00 d0       	rcall	.+0      	; 0xca <do_sum+0xa>
  ca:	cd b7       	in	r28, 0x3d	; 61
  cc:	de b7       	in	r29, 0x3e	; 62
  ce:	9c 83       	std	Y+4, r25	; 0x04
  d0:	8b 83       	std	Y+3, r24	; 0x03
  d2:	7e 83       	std	Y+6, r23	; 0x06
  d4:	6d 83       	std	Y+5, r22	; 0x05
  d6:	8d 81       	ldd	r24, Y+5	; 0x05
  d8:	9e 81       	ldd	r25, Y+6	; 0x06
  da:	e8 2f       	mov	r30, r24
  dc:	f9 2f       	mov	r31, r25
  de:	20 81       	ld	r18, Z
  e0:	31 81       	ldd	r19, Z+1	; 0x01
  e2:	8b 81       	ldd	r24, Y+3	; 0x03
  e4:	9c 81       	ldd	r25, Y+4	; 0x04
  e6:	e8 2f       	mov	r30, r24
  e8:	f9 2f       	mov	r31, r25
  ea:	80 81       	ld	r24, Z
  ec:	91 81       	ldd	r25, Z+1	; 0x01
  ee:	62 2f       	mov	r22, r18
  f0:	73 2f       	mov	r23, r19
  f2:	00 d0       	rcall	.+0      	; 0xf4 <do_sum+0x34>
  f4:	9a 83       	std	Y+2, r25	; 0x02
  f6:	89 83       	std	Y+1, r24	; 0x01
  f8:	00 00       	nop
  fa:	26 96       	adiw	r28, 0x06	; 6
  fc:	0f b6       	in	r0, 0x3f	; 63
  fe:	f8 94       	cli
 100:	de bf       	out	0x3e, r29	; 62
 102:	0f be       	out	0x3f, r0	; 63
 104:	cd bf       	out	0x3d, r28	; 61
 106:	df 91       	pop	r29
 108:	cf 91       	pop	r28
 10a:	08 95       	ret
