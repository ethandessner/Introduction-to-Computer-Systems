# Ethan Dessner 117919219 edessner
reverse_prefix_sum:
    # PROLOGUE
    subu $sp, $sp, 8        # expand stack by 8 bytes
    sw   $ra, 8($sp)        # push $ra (ret addr, 4 bytes)
    sw   $fp, 4($sp)        # push $fp (4 bytes)
    addu $fp, $sp, 8        # set $fp to saved $ra

    lw $t0, ($a0)           # loading first element into $t0
    beq $t0, -1, ret_zero   # check if arr = -1
    subu $sp, $sp, 4        # allocating space
    sw $a0, 4($sp)          # storing address of first element
    addu $a0, $a0, 4        # incrementing address to second element of arr
    jal reverse_prefix_sum  # recursive call

    move $t0, $v0           # moving results from $v0 to $t0
    lw $t1, 4($sp)          # loading address into $t1
    lw $t2, ($t1)           # loading value of address of $t1 into $t2
    addu $t0, $t0, $t2      # adding to sum
    j ret

ret_zero:
    # EPILOGUE  
    li $v0, 0               # loads $v0 to 0 when value of arr = -1

    move $sp, $fp           # restore $sp
    lw   $ra, ($fp)         # restore saved $ra
    lw   $fp, -4($sp)       # restore saved $fp
    j    $ra                # return to kernel

ret:
    move $v0, $t0           # move final sum into $v0
    sw $v0, ($t1)           # *arr = r
    # EPILOGUE
    move $sp, $fp           # restore $sp
    lw   $ra, ($fp)         # restore saved $ra
    lw   $fp, -4($sp)       # restore saved $fp
    j    $ra                # return to kernel
