
more-avr.o:     file format elf32-avr


Disassembly of section .text:

00000000 <add>:
   0:	ef 92       	push	r14
   2:	ff 92       	push	r15
   4:	0f 93       	push	r16
   6:	1f 93       	push	r17
   8:	cf 93       	push	r28
   a:	df 93       	push	r29
   c:	cd b7       	in	r28, 0x3d	; 61
   e:	de b7       	in	r29, 0x3e	; 62
  10:	2c 97       	sbiw	r28, 0x0c	; 12
  12:	0f b6       	in	r0, 0x3f	; 63
  14:	f8 94       	cli
  16:	de bf       	out	0x3e, r29	; 62
  18:	0f be       	out	0x3f, r0	; 63
  1a:	cd bf       	out	0x3d, r28	; 61
  1c:	9a 83       	std	Y+2, r25	; 0x02
  1e:	89 83       	std	Y+1, r24	; 0x01
  20:	7c 83       	std	Y+4, r23	; 0x04
  22:	6b 83       	std	Y+3, r22	; 0x03
  24:	5e 83       	std	Y+6, r21	; 0x06
  26:	4d 83       	std	Y+5, r20	; 0x05
  28:	38 87       	std	Y+8, r19	; 0x08
  2a:	2f 83       	std	Y+7, r18	; 0x07
  2c:	1a 87       	std	Y+10, r17	; 0x0a
  2e:	09 87       	std	Y+9, r16	; 0x09
  30:	fc 86       	std	Y+12, r15	; 0x0c
  32:	eb 86       	std	Y+11, r14	; 0x0b
  34:	29 81       	ldd	r18, Y+1	; 0x01
  36:	3a 81       	ldd	r19, Y+2	; 0x02
  38:	8b 81       	ldd	r24, Y+3	; 0x03
  3a:	9c 81       	ldd	r25, Y+4	; 0x04
  3c:	28 0f       	add	r18, r24
  3e:	39 1f       	adc	r19, r25
  40:	8d 81       	ldd	r24, Y+5	; 0x05
  42:	9e 81       	ldd	r25, Y+6	; 0x06
  44:	28 0f       	add	r18, r24
  46:	39 1f       	adc	r19, r25
  48:	8f 81       	ldd	r24, Y+7	; 0x07
  4a:	98 85       	ldd	r25, Y+8	; 0x08
  4c:	28 0f       	add	r18, r24
  4e:	39 1f       	adc	r19, r25
  50:	89 85       	ldd	r24, Y+9	; 0x09
  52:	9a 85       	ldd	r25, Y+10	; 0x0a
  54:	28 0f       	add	r18, r24
  56:	39 1f       	adc	r19, r25
  58:	8b 85       	ldd	r24, Y+11	; 0x0b
  5a:	9c 85       	ldd	r25, Y+12	; 0x0c
  5c:	82 0f       	add	r24, r18
  5e:	93 1f       	adc	r25, r19
  60:	2c 96       	adiw	r28, 0x0c	; 12
  62:	0f b6       	in	r0, 0x3f	; 63
  64:	f8 94       	cli
  66:	de bf       	out	0x3e, r29	; 62
  68:	0f be       	out	0x3f, r0	; 63
  6a:	cd bf       	out	0x3d, r28	; 61
  6c:	df 91       	pop	r29
  6e:	cf 91       	pop	r28
  70:	1f 91       	pop	r17
  72:	0f 91       	pop	r16
  74:	ff 90       	pop	r15
  76:	ef 90       	pop	r14
  78:	08 95       	ret
