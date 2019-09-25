.data

lfsr:
	.align 4
	.word 0x1

.text

# Implements a 16-bit lfsr
#
# Arguments: None
lfsr_random:
	la t0 lfsr
	lhu a0 0(t0)
    
	# Your Code Here
    add t1, x0, x0
    addi t2, x0, 16
    add t3, a0, x0 # copy array

loop:
    srli t5, t3, 2   # place 2
    xor  t4, t3, t5
    srli t5, t3, 3   # place 3
    xor  t4, t4, t5
    srli t5, t3, 5   # place 5
    xor  t4, t4, t5
    andi t4, t4, 0x1 # final place
    slli t4, t4, 15
    srli t3, t3, 1
    or   t3, t3, t4
    addi t1, t1, 1
	blt  t1, t2, loop
    
	# print value
    # add a1, t3, x0
    # addi a0, x0, 1 # print int
    # ecall
    # addi a1, x0, '\n'
    # addi a0, x0, 11
    # ecall
	addi a0, t3, 0
	la t0 lfsr
	sh a0 0(t0)
	jr ra
