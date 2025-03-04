! Template for instruction-level verification
! Author : Ashfaque Ahammed
! 16 August 2015
!
! NOTE : register g1 is reserved for storing the trap number.
! On normal exit a test should end in trap 0, so g1=0x80.
! Upon exit with an error condition during testing, g1=0xBAD


.section	".data"
.align 8
      ZERO: .double 0


.section	".text"
.global main
main:
_start:

	
	!clear all registers belonging to window 0
	mov 0x10E0, %l0	; wr %l0, %psr ;nop;nop;nop	
	clr %g0; clr %g1; clr %g2; clr %g3; clr %g4; clr %g5; clr %g6; clr %g7; 
	clr %l0; clr %l1; clr %l2; clr %l3; clr %l4; clr %l5; clr %l6; clr %l7; 
	clr %i0; clr %i1; clr %i2; clr %i3; clr %i4; clr %i5; clr %i6; clr %i7; 
	clr %o0; clr %o1; clr %o2; clr %o3; clr %o4; clr %o5; clr %o6; clr %o7;
	!clear all FPU registers
	set ZERO, %l0; ldd [%l0], %f0
	fmovs %f0, %f1  ; fmovs %f0, %f2  ; fmovs %f0, %f3  ; fmovs %f0, %f4  ;
	fmovs %f0, %f5  ; fmovs %f0, %f6  ; fmovs %f0, %f7  ; fmovs %f0, %f8  ;
	fmovs %f0, %f9  ; fmovs %f0, %f10 ; fmovs %f0, %f11 ; fmovs %f0, %f12 ;
	fmovs %f0, %f13 ; fmovs %f0, %f14 ; fmovs %f0, %f15 ; fmovs %f0, %f16 ;
	fmovs %f0, %f17 ; fmovs %f0, %f18 ; fmovs %f0, %f19 ; fmovs %f0, %f20 ;
	fmovs %f0, %f21 ; fmovs %f0, %f22 ; fmovs %f0, %f23 ; fmovs %f0, %f24 ;
	fmovs %f0, %f25 ; fmovs %f0, %f26 ; fmovs %f0, %f27 ; fmovs %f0, %f28 ;
	fmovs %f0, %f29 ; fmovs %f0, %f30 ; fmovs %f0, %f31 ; 

! Initialize PSR, enable traps.
	! set PSR with ET=1 PS=1 S=1, EF=1 all other fields=0
	set 0x10E0, %l0
	nop	! nops here since set
	nop	! may be delayed
	nop
	wr %l0, %psr
	

	nop	! insert nops here because
	nop	! writes to psr may be delayed 
	nop	
	
	
	!store base of trap table in TBR register
	set	trap_table_base, %l0
	wr	%l0, 0x0, %tbr
	nop	! insert nops here because
	nop	! writes to tbr may be delayed
	nop
	!Initialize g1. Upon a trap, g1 should be 
	!overwritten by the trap number
	mov 0xBAD, %g1


	!
	! initialize fpu-fsr so that bit 27 is 1 (NVM=1).
	!
	mov 0x1, %l1
	mov 0x1b, %l2
	sll %l1, %l2, %l2
	set mFSR, %l3
	set mFSRN, %l4
	st %l2, [%l3]
	ld [%l3], %fsr


	!======================================
	! Perform instruction test and store
	! results in the current window's registers
	!
	! instruction = fcmps

	
	set A, %l0		! move  address of 7.6 into l0
	set B, %l1		! move  address of 3.1 into l1
	ld [%l0], %f0		! loads 7.6 to f0
	ld [%l1], %f1		! loads 3.1 to f1
	fcmps %f0, %f1		! freg1 > freg2 => fcc=2
	set 0x00, %o4		! o4 = 0x0800
	st %fsr, [%o4]	 	! m[0x0] = 0x
	rd %psr, %o1		! Expected o3=0x10E0

	fcmpes %f1, %f0		! freg1 < freg2 => fcc=1
	st %fsr, [%o4 + 4]	! m[0x4] = 0x0400
	rd %psr, %o2		! Expected o3=0x10E0

	set C, %l2		! move  address of 7.6 into l2
	ld [%l2], %f2		! loads 7.6 to f2
	fcmpes %f2, %f0		! freg0 = freg2 => fcc=0
	st %fsr, [%o4 + 8]	! m[0x4] = 0x00
	rd %psr, %o3		! Expected o3=0x10E0

!======================================================
	! For testing IEEE_754 Excepting
	! Generate FP_exception when one of operand is a sNaN
	!

	set D, %l3		! move  address of NaN into l3
	ld [%l3], %f10		! loads NaN to f10
	fcmpes %f6, %f10		! 
	rd %psr, %o2		! Expected o3=0x10E0
	
	ta 0			! end of test
	nop
	nop


	
	!======================================
	!control should NOT reach here
not_reached:
	set 0xDEAD, %g1
	ta 0
	nop
	nop


	.align 4
        A: .single 0r7.6
	B: .single 0r3.1
	C: .single 0r7.6
	D: .word 0x7F900000 ! sNaN
 	mFSR: .word 0x0
 	mFSRN: .word 0x0

	!====================================
	! TRAP TABLE
	!====================================
	!NOTE: the trap table has to be aligned on 2^12 = 4096 bytes
	!because wr tbr instruction copies bits 
	!31:12 of the result only into tbr register
	
	.align 4096

trap_table_base:
	! The trap handler for each trap does the following:
	!	- store the trap number in g1
	!	- do a restore operation so that we  
	!	  go back to the original window 
	!	- ta 0 (cause a trap in trap to go into ERROR mode)

hardware_trap_table_base:

HW_trap_0x00: mov 0x00, %g1; restore; ta 0; nop
HW_trap_0x01: mov 0x01, %g1; restore; ta 0; nop
HW_trap_0x02: mov 0x02, %g1; restore; ta 0; nop
HW_trap_0x03: mov 0x03, %g1; restore; ta 0; nop
HW_trap_0x04: mov 0x04, %g1; restore; ta 0; nop
HW_trap_0x05: mov 0x05, %g1; restore; ta 0; nop
HW_trap_0x06: mov 0x06, %g1; restore; ta 0; nop
HW_trap_0x07: mov 0x07, %g1; restore; ta 0; nop
HW_trap_0x08: inc %g2; rett %r18; nop; nop      !FP exception
HW_trap_0x09: mov 0x09, %g1; restore; ta 0; nop
HW_trap_0x0a: mov 0x0a, %g1; restore; ta 0; nop
HW_trap_0x0b: mov 0x0b, %g1; restore; ta 0; nop
HW_trap_0x0c: mov 0x0c, %g1; restore; ta 0; nop
HW_trap_0x0d: mov 0x0d, %g1; restore; ta 0; nop
HW_trap_0x0e: mov 0x0e, %g1; restore; ta 0; nop
HW_trap_0x0f: mov 0x0f, %g1; restore; ta 0; nop
HW_trap_0x10: mov 0x10, %g1; restore; ta 0; nop
HW_trap_0x11: mov 0x11, %g1; restore; ta 0; nop
HW_trap_0x12: mov 0x12, %g1; restore; ta 0; nop
HW_trap_0x13: mov 0x13, %g1; restore; ta 0; nop
HW_trap_0x14: mov 0x14, %g1; restore; ta 0; nop
HW_trap_0x15: mov 0x15, %g1; restore; ta 0; nop
HW_trap_0x16: mov 0x16, %g1; restore; ta 0; nop
HW_trap_0x17: mov 0x17, %g1; restore; ta 0; nop
HW_trap_0x18: mov 0x18, %g1; restore; ta 0; nop
HW_trap_0x19: mov 0x19, %g1; restore; ta 0; nop
HW_trap_0x1a: mov 0x1a, %g1; restore; ta 0; nop
HW_trap_0x1b: mov 0x1b, %g1; restore; ta 0; nop
HW_trap_0x1c: mov 0x1c, %g1; restore; ta 0; nop
HW_trap_0x1d: mov 0x1d, %g1; restore; ta 0; nop
HW_trap_0x1e: mov 0x1e, %g1; restore; ta 0; nop
HW_trap_0x1f: mov 0x1f, %g1; restore; ta 0; nop
HW_trap_0x20: mov 0x20, %g1; restore; ta 0; nop
HW_trap_0x21: mov 0x21, %g1; restore; ta 0; nop
HW_trap_0x22: mov 0x22, %g1; restore; ta 0; nop
HW_trap_0x23: mov 0x23, %g1; restore; ta 0; nop
HW_trap_0x24: mov 0x24, %g1; restore; ta 0; nop
HW_trap_0x25: mov 0x25, %g1; restore; ta 0; nop
HW_trap_0x26: mov 0x26, %g1; restore; ta 0; nop
HW_trap_0x27: mov 0x27, %g1; restore; ta 0; nop
HW_trap_0x28: mov 0x28, %g1; restore; ta 0; nop
HW_trap_0x29: mov 0x29, %g1; restore; ta 0; nop
HW_trap_0x2a: mov 0x2a, %g1; restore; ta 0; nop
HW_trap_0x2b: mov 0x2b, %g1; restore; ta 0; nop
HW_trap_0x2c: mov 0x2c, %g1; restore; ta 0; nop
HW_trap_0x2d: mov 0x2d, %g1; restore; ta 0; nop
HW_trap_0x2e: mov 0x2e, %g1; restore; ta 0; nop
HW_trap_0x2f: mov 0x2f, %g1; restore; ta 0; nop
HW_trap_0x30: mov 0x30, %g1; restore; ta 0; nop
HW_trap_0x31: mov 0x31, %g1; restore; ta 0; nop
HW_trap_0x32: mov 0x32, %g1; restore; ta 0; nop
HW_trap_0x33: mov 0x33, %g1; restore; ta 0; nop
HW_trap_0x34: mov 0x34, %g1; restore; ta 0; nop
HW_trap_0x35: mov 0x35, %g1; restore; ta 0; nop
HW_trap_0x36: mov 0x36, %g1; restore; ta 0; nop
HW_trap_0x37: mov 0x37, %g1; restore; ta 0; nop
HW_trap_0x38: mov 0x38, %g1; restore; ta 0; nop
HW_trap_0x39: mov 0x39, %g1; restore; ta 0; nop
HW_trap_0x3a: mov 0x3a, %g1; restore; ta 0; nop
HW_trap_0x3b: mov 0x3b, %g1; restore; ta 0; nop
HW_trap_0x3c: mov 0x3c, %g1; restore; ta 0; nop
HW_trap_0x3d: mov 0x3d, %g1; restore; ta 0; nop
HW_trap_0x3e: mov 0x3e, %g1; restore; ta 0; nop
HW_trap_0x3f: mov 0x3f, %g1; restore; ta 0; nop
HW_trap_0x40: mov 0x40, %g1; restore; ta 0; nop
HW_trap_0x41: mov 0x41, %g1; restore; ta 0; nop
HW_trap_0x42: mov 0x42, %g1; restore; ta 0; nop
HW_trap_0x43: mov 0x43, %g1; restore; ta 0; nop
HW_trap_0x44: mov 0x44, %g1; restore; ta 0; nop
HW_trap_0x45: mov 0x45, %g1; restore; ta 0; nop
HW_trap_0x46: mov 0x46, %g1; restore; ta 0; nop
HW_trap_0x47: mov 0x47, %g1; restore; ta 0; nop
HW_trap_0x48: mov 0x48, %g1; restore; ta 0; nop
HW_trap_0x49: mov 0x49, %g1; restore; ta 0; nop
HW_trap_0x4a: mov 0x4a, %g1; restore; ta 0; nop
HW_trap_0x4b: mov 0x4b, %g1; restore; ta 0; nop
HW_trap_0x4c: mov 0x4c, %g1; restore; ta 0; nop
HW_trap_0x4d: mov 0x4d, %g1; restore; ta 0; nop
HW_trap_0x4e: mov 0x4e, %g1; restore; ta 0; nop
HW_trap_0x4f: mov 0x4f, %g1; restore; ta 0; nop
HW_trap_0x50: mov 0x50, %g1; restore; ta 0; nop
HW_trap_0x51: mov 0x51, %g1; restore; ta 0; nop
HW_trap_0x52: mov 0x52, %g1; restore; ta 0; nop
HW_trap_0x53: mov 0x53, %g1; restore; ta 0; nop
HW_trap_0x54: mov 0x54, %g1; restore; ta 0; nop
HW_trap_0x55: mov 0x55, %g1; restore; ta 0; nop
HW_trap_0x56: mov 0x56, %g1; restore; ta 0; nop
HW_trap_0x57: mov 0x57, %g1; restore; ta 0; nop
HW_trap_0x58: mov 0x58, %g1; restore; ta 0; nop
HW_trap_0x59: mov 0x59, %g1; restore; ta 0; nop
HW_trap_0x5a: mov 0x5a, %g1; restore; ta 0; nop
HW_trap_0x5b: mov 0x5b, %g1; restore; ta 0; nop
HW_trap_0x5c: mov 0x5c, %g1; restore; ta 0; nop
HW_trap_0x5d: mov 0x5d, %g1; restore; ta 0; nop
HW_trap_0x5e: mov 0x5e, %g1; restore; ta 0; nop
HW_trap_0x5f: mov 0x5f, %g1; restore; ta 0; nop
HW_trap_0x60: mov 0x60, %g1; restore; ta 0; nop
HW_trap_0x61: mov 0x61, %g1; restore; ta 0; nop
HW_trap_0x62: mov 0x62, %g1; restore; ta 0; nop
HW_trap_0x63: mov 0x63, %g1; restore; ta 0; nop
HW_trap_0x64: mov 0x64, %g1; restore; ta 0; nop
HW_trap_0x65: mov 0x65, %g1; restore; ta 0; nop
HW_trap_0x66: mov 0x66, %g1; restore; ta 0; nop
HW_trap_0x67: mov 0x67, %g1; restore; ta 0; nop
HW_trap_0x68: mov 0x68, %g1; restore; ta 0; nop
HW_trap_0x69: mov 0x69, %g1; restore; ta 0; nop
HW_trap_0x6a: mov 0x6a, %g1; restore; ta 0; nop
HW_trap_0x6b: mov 0x6b, %g1; restore; ta 0; nop
HW_trap_0x6c: mov 0x6c, %g1; restore; ta 0; nop
HW_trap_0x6d: mov 0x6d, %g1; restore; ta 0; nop
HW_trap_0x6e: mov 0x6e, %g1; restore; ta 0; nop
HW_trap_0x6f: mov 0x6f, %g1; restore; ta 0; nop
HW_trap_0x70: mov 0x70, %g1; restore; ta 0; nop
HW_trap_0x71: mov 0x71, %g1; restore; ta 0; nop
HW_trap_0x72: mov 0x72, %g1; restore; ta 0; nop
HW_trap_0x73: mov 0x73, %g1; restore; ta 0; nop
HW_trap_0x74: mov 0x74, %g1; restore; ta 0; nop
HW_trap_0x75: mov 0x75, %g1; restore; ta 0; nop
HW_trap_0x76: mov 0x76, %g1; restore; ta 0; nop
HW_trap_0x77: mov 0x77, %g1; restore; ta 0; nop
HW_trap_0x78: mov 0x78, %g1; restore; ta 0; nop
HW_trap_0x79: mov 0x79, %g1; restore; ta 0; nop
HW_trap_0x7a: mov 0x7a, %g1; restore; ta 0; nop
HW_trap_0x7b: mov 0x7b, %g1; restore; ta 0; nop
HW_trap_0x7c: mov 0x7c, %g1; restore; ta 0; nop
HW_trap_0x7d: mov 0x7d, %g1; restore; ta 0; nop
HW_trap_0x7e: mov 0x7e, %g1; restore; ta 0; nop
HW_trap_0x7f: mov 0x7f, %g1; restore; ta 0; nop
software_trap_table_base:
SW_trap_0x80: mov 0x80, %g1; restore; ta 0; nop
SW_trap_0x81: mov 0x81, %g1; restore; ta 0; nop
SW_trap_0x82: mov 0x82, %g1; restore; ta 0; nop
SW_trap_0x83: mov 0x83, %g1; restore; ta 0; nop
SW_trap_0x84: mov 0x84, %g1; restore; ta 0; nop
SW_trap_0x85: mov 0x85, %g1; restore; ta 0; nop
SW_trap_0x86: mov 0x86, %g1; restore; ta 0; nop
SW_trap_0x87: mov 0x87, %g1; restore; ta 0; nop
SW_trap_0x88: mov 0x88, %g1; restore; ta 0; nop
SW_trap_0x89: mov 0x89, %g1; restore; ta 0; nop
SW_trap_0x8a: mov 0x8a, %g1; restore; ta 0; nop
SW_trap_0x8b: mov 0x8b, %g1; restore; ta 0; nop
SW_trap_0x8c: mov 0x8c, %g1; restore; ta 0; nop
SW_trap_0x8d: mov 0x8d, %g1; restore; ta 0; nop
SW_trap_0x8e: mov 0x8e, %g1; restore; ta 0; nop
SW_trap_0x8f: mov 0x8f, %g1; restore; ta 0; nop
SW_trap_0x90: mov 0x90, %g1; restore; ta 0; nop
SW_trap_0x91: mov 0x91, %g1; restore; ta 0; nop
SW_trap_0x92: mov 0x92, %g1; restore; ta 0; nop
SW_trap_0x93: mov 0x93, %g1; restore; ta 0; nop
SW_trap_0x94: mov 0x94, %g1; restore; ta 0; nop
SW_trap_0x95: mov 0x95, %g1; restore; ta 0; nop
SW_trap_0x96: mov 0x96, %g1; restore; ta 0; nop
SW_trap_0x97: mov 0x97, %g1; restore; ta 0; nop
SW_trap_0x98: mov 0x98, %g1; restore; ta 0; nop
SW_trap_0x99: mov 0x99, %g1; restore; ta 0; nop
SW_trap_0x9a: mov 0x9a, %g1; restore; ta 0; nop
SW_trap_0x9b: mov 0x9b, %g1; restore; ta 0; nop
SW_trap_0x9c: mov 0x9c, %g1; restore; ta 0; nop
SW_trap_0x9d: mov 0x9d, %g1; restore; ta 0; nop
SW_trap_0x9e: mov 0x9e, %g1; restore; ta 0; nop
SW_trap_0x9f: mov 0x9f, %g1; restore; ta 0; nop
SW_trap_0xa0: mov 0xa0, %g1; restore; ta 0; nop
SW_trap_0xa1: mov 0xa1, %g1; restore; ta 0; nop
SW_trap_0xa2: mov 0xa2, %g1; restore; ta 0; nop
SW_trap_0xa3: mov 0xa3, %g1; restore; ta 0; nop
SW_trap_0xa4: mov 0xa4, %g1; restore; ta 0; nop
SW_trap_0xa5: mov 0xa5, %g1; restore; ta 0; nop
SW_trap_0xa6: mov 0xa6, %g1; restore; ta 0; nop
SW_trap_0xa7: mov 0xa7, %g1; restore; ta 0; nop
SW_trap_0xa8: mov 0xa8, %g1; restore; ta 0; nop
SW_trap_0xa9: mov 0xa9, %g1; restore; ta 0; nop
SW_trap_0xaa: mov 0xaa, %g1; restore; ta 0; nop
SW_trap_0xab: mov 0xab, %g1; restore; ta 0; nop
SW_trap_0xac: mov 0xac, %g1; restore; ta 0; nop
SW_trap_0xad: mov 0xad, %g1; restore; ta 0; nop
SW_trap_0xae: mov 0xae, %g1; restore; ta 0; nop
SW_trap_0xaf: mov 0xaf, %g1; restore; ta 0; nop
SW_trap_0xb0: mov 0xb0, %g1; restore; ta 0; nop
SW_trap_0xb1: mov 0xb1, %g1; restore; ta 0; nop
SW_trap_0xb2: mov 0xb2, %g1; restore; ta 0; nop
SW_trap_0xb3: mov 0xb3, %g1; restore; ta 0; nop
SW_trap_0xb4: mov 0xb4, %g1; restore; ta 0; nop
SW_trap_0xb5: mov 0xb5, %g1; restore; ta 0; nop
SW_trap_0xb6: mov 0xb6, %g1; restore; ta 0; nop
SW_trap_0xb7: mov 0xb7, %g1; restore; ta 0; nop
SW_trap_0xb8: mov 0xb8, %g1; restore; ta 0; nop
SW_trap_0xb9: mov 0xb9, %g1; restore; ta 0; nop
SW_trap_0xba: mov 0xba, %g1; restore; ta 0; nop
SW_trap_0xbb: mov 0xbb, %g1; restore; ta 0; nop
SW_trap_0xbc: mov 0xbc, %g1; restore; ta 0; nop
SW_trap_0xbd: mov 0xbd, %g1; restore; ta 0; nop
SW_trap_0xbe: mov 0xbe, %g1; restore; ta 0; nop
SW_trap_0xbf: mov 0xbf, %g1; restore; ta 0; nop
SW_trap_0xc0: mov 0xc0, %g1; restore; ta 0; nop
SW_trap_0xc1: mov 0xc1, %g1; restore; ta 0; nop
SW_trap_0xc2: mov 0xc2, %g1; restore; ta 0; nop
SW_trap_0xc3: mov 0xc3, %g1; restore; ta 0; nop
SW_trap_0xc4: mov 0xc4, %g1; restore; ta 0; nop
SW_trap_0xc5: mov 0xc5, %g1; restore; ta 0; nop
SW_trap_0xc6: mov 0xc6, %g1; restore; ta 0; nop
SW_trap_0xc7: mov 0xc7, %g1; restore; ta 0; nop
SW_trap_0xc8: mov 0xc8, %g1; restore; ta 0; nop
SW_trap_0xc9: mov 0xc9, %g1; restore; ta 0; nop
SW_trap_0xca: mov 0xca, %g1; restore; ta 0; nop
SW_trap_0xcb: mov 0xcb, %g1; restore; ta 0; nop
SW_trap_0xcc: mov 0xcc, %g1; restore; ta 0; nop
SW_trap_0xcd: mov 0xcd, %g1; restore; ta 0; nop
SW_trap_0xce: mov 0xce, %g1; restore; ta 0; nop
SW_trap_0xcf: mov 0xcf, %g1; restore; ta 0; nop
SW_trap_0xd0: mov 0xd0, %g1; restore; ta 0; nop
SW_trap_0xd1: mov 0xd1, %g1; restore; ta 0; nop
SW_trap_0xd2: mov 0xd2, %g1; restore; ta 0; nop
SW_trap_0xd3: mov 0xd3, %g1; restore; ta 0; nop
SW_trap_0xd4: mov 0xd4, %g1; restore; ta 0; nop
SW_trap_0xd5: mov 0xd5, %g1; restore; ta 0; nop
SW_trap_0xd6: mov 0xd6, %g1; restore; ta 0; nop
SW_trap_0xd7: mov 0xd7, %g1; restore; ta 0; nop
SW_trap_0xd8: mov 0xd8, %g1; restore; ta 0; nop
SW_trap_0xd9: mov 0xd9, %g1; restore; ta 0; nop
SW_trap_0xda: mov 0xda, %g1; restore; ta 0; nop
SW_trap_0xdb: mov 0xdb, %g1; restore; ta 0; nop
SW_trap_0xdc: mov 0xdc, %g1; restore; ta 0; nop
SW_trap_0xdd: mov 0xdd, %g1; restore; ta 0; nop
SW_trap_0xde: mov 0xde, %g1; restore; ta 0; nop
SW_trap_0xdf: mov 0xdf, %g1; restore; ta 0; nop
SW_trap_0xe0: mov 0xe0, %g1; restore; ta 0; nop
SW_trap_0xe1: mov 0xe1, %g1; restore; ta 0; nop
SW_trap_0xe2: mov 0xe2, %g1; restore; ta 0; nop
SW_trap_0xe3: mov 0xe3, %g1; restore; ta 0; nop
SW_trap_0xe4: mov 0xe4, %g1; restore; ta 0; nop
SW_trap_0xe5: mov 0xe5, %g1; restore; ta 0; nop
SW_trap_0xe6: mov 0xe6, %g1; restore; ta 0; nop
SW_trap_0xe7: mov 0xe7, %g1; restore; ta 0; nop
SW_trap_0xe8: mov 0xe8, %g1; restore; ta 0; nop
SW_trap_0xe9: mov 0xe9, %g1; restore; ta 0; nop
SW_trap_0xea: mov 0xea, %g1; restore; ta 0; nop
SW_trap_0xeb: mov 0xeb, %g1; restore; ta 0; nop
SW_trap_0xec: mov 0xec, %g1; restore; ta 0; nop
SW_trap_0xed: mov 0xed, %g1; restore; ta 0; nop
SW_trap_0xee: mov 0xee, %g1; restore; ta 0; nop
SW_trap_0xef: mov 0xef, %g1; restore; ta 0; nop
SW_trap_0xf0: mov 0xf0, %g1; restore; ta 0; nop
SW_trap_0xf1: mov 0xf1, %g1; restore; ta 0; nop
SW_trap_0xf2: mov 0xf2, %g1; restore; ta 0; nop
SW_trap_0xf3: mov 0xf3, %g1; restore; ta 0; nop
SW_trap_0xf4: mov 0xf4, %g1; restore; ta 0; nop
SW_trap_0xf5: mov 0xf5, %g1; restore; ta 0; nop
SW_trap_0xf6: mov 0xf6, %g1; restore; ta 0; nop
SW_trap_0xf7: mov 0xf7, %g1; restore; ta 0; nop
SW_trap_0xf8: mov 0xf8, %g1; restore; ta 0; nop
SW_trap_0xf9: mov 0xf9, %g1; restore; ta 0; nop
SW_trap_0xfa: mov 0xfa, %g1; restore; ta 0; nop
SW_trap_0xfb: mov 0xfb, %g1; restore; ta 0; nop
SW_trap_0xfc: mov 0xfc, %g1; restore; ta 0; nop
SW_trap_0xfd: mov 0xfd, %g1; restore; ta 0; nop
SW_trap_0xfe: mov 0xfe, %g1; restore; ta 0; nop

nop
nop

