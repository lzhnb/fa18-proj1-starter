.text

# Decodes a quadtree to the original matrix
#
# Arguments:
#     quadtree (qNode*)
#     matrix (void*)
#     matrix_width (int)
#
# Recall that quadtree representation uses the following format:
#     struct qNode {
#         bool leaf;
#         int size;
#         int x;
#         int y;
#         int gray_value;
#         qNode *child_NW, *child_NE, *child_SE, *child_SW;
#     }

quad2matrix:
	# Your code here
	# a0 means root
	# a1 point to matrix
	# a2 means map_width
helper:
    add t0 a0 x0  # t0 point to quadtree_root(begin)

find_child:
    addi sp sp -4 # use stack
    sw ra 0(sp)   # save ra point
	lw a3 0(t0)  # get leaf(1) or not(0)
    beq a3 x0 run_child # if not leaf
    
    # is leaf
    lw t1 4(t0)  # get section_width
    lw t2 16(t0) # get value
    # count a4, a5 for x&y
    lw a4 8(t0)  # get x
    lw a5 12(t0) # get y
    add t4 x0 x0 # count_x = 0
    add t5 x0 x0 # count_y = 0
    
	# x=a4, y=a5, section_width=t1, value=t2
loop_x:
	beq t4 t1 loop_y # if count_x == section_width: count_y ++
    add s1 t5 a5     # now_y = count_y + y
    mul t3 s1 a2     # t3 = now_y * map_width
    add s2 t4 a4     # now_x = count_x + x
    add t3 t3 s2     # t3 = now_y * map_width + now_x
    add s2 a1 t3     # a1 point to matrix head and s2 point to aim
    sb t2 0(s2)      # use 'sb' to confirm store one byte 'sw' will change 4 bytes
    addi t4 t4 1
    j loop_x
loop_y:
	add t4 x0 x0
	addi t5 t5 1   # count_y += 1
   	beq t5 t1 done # if count_y == section_width lood done
    j loop_x
    
run_child:
    addi sp sp -4
    sw t0 0(sp)
    lw t0 20(t0) # get NW
    jal find_child
    lw t0 0(sp)
    lw t0 24(t0) # get NE
    jal find_child
    lw t0 0(sp)
    lw t0 28(t0) # get SE
    jal find_child
    lw t0 0(sp)
    lw t0 32(t0) # get SW
    jal find_child
	lw t0 0(sp)
    addi sp sp 4
done:
	lw ra 0(sp)
    addi sp sp 4
    jr ra



