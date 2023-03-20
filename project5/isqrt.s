# Ethan Dessner 117919219 edessner
isqrt:
   # PROLOGUE
   subu $sp, $sp, 8        # expand stack by 8 bytes
   sw   $ra, 8($sp)        # push $ra (ret addr, 4 bytes)
   sw   $fp, 4($sp)        # push $fp (4 bytes)
   addu $fp, $sp, 8        # set $fp to saved $ra

   # BODY
   move $t0, $a0           # move current value into $t0
   blt $t0, 2, less_two    # check if value is less than 2
   subu $sp, $sp, 8        # allocate space for n and n >> 2
   sw $t0, 8($sp)          # storing n
   srl $t0, $t0, 2         # shifting n 2 to the right
   sw $t0, 4($sp)          # storing n >> 2
   move $a0, $t0           # moving n >> 2 into $a0
   jal isqrt               # recursive call

   move $t0, $v0          # move results from recursive call into $t0
   lw $t2, 8($sp)         # load n into $t2        
   sll $t0, $t0, 1        # shift small left 1
   move $t1, $t0          # move small into $t1
   addu $t1, $t1, 1       # large = small + 1
   move $t3, $t1          # move large into $t3
   mul $t3, $t3, $t3      # multiply large by itself
   bgt $t3, $t2, small    # compare (large)^2 to n - branch to small if greater

   move $v0, $t1          # move large into $v0 if (large)^2 <= n
   # EPILOGUE
   move $sp, $fp           # restore $sp
   lw   $ra, ($fp)         # restore saved $ra
   lw   $fp, -4($sp)       # restore saved $fp
   j    $ra                # return to kernel

small:
    move $v0, $t0         # move large into $v0 if (large)^2 > n
    # EPILOGUE
    move $sp, $fp           # restore $sp
    lw   $ra, ($fp)         # restore saved $ra
    lw   $fp, -4($sp)       # restore saved $fp
    j    $ra                # return to kernel

less_two:
    move $v0, $t0          # move $t0 into $v0 if less than 2
    # EPILOGUE
    move $sp, $fp           # restore $sp
    lw   $ra, ($fp)         # restore saved $ra
    lw   $fp, -4($sp)       # restore saved $fp
    j    $ra                # return to kernel