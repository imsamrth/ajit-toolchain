
.text
.global main

main:
start: 
        save %sp,-80,%sp    !save memory to store input values
        set 0xfffffffc,%g3   !double-word aligned for std
        and %fp, %g3,%fp

	set 0x3c66, %l0     !set value in a register  (value is 1.1)
	st %l0, [%fp + -4]  !put in memory
        ld [%fp + -4],%f0   !first input in f0 from memory
	fhtos %f0,%f1   !result should be 1.1
	
	restore
 
end:	ta 0
