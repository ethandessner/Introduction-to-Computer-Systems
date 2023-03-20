# Ethan Dessner 117919219 edessner
is_palindrome:
   # PROLOGUE
   subu $sp, $sp, 8        # expand stack by 8 bytes
   sw   $ra, 8($sp)        # push $ra (ret addr, 4 bytes)
   sw   $fp, 4($sp)        # push $fp (4 bytes)
   addu $fp, $sp, 8        # set $fp to saved $ra

   # BODY
   subu $sp, $sp, 4
   sw $a0, 4($sp)    # store the string into memory

   jal strlen
   move $t0, $v0     # moving the length of the string into $t0
   div $t0, $t0, 2   # divide the length of the string by 2
   li $t1, 0         # i == 0
   lw $t2, 4($sp)    # starting at the beginning of the string
   subu $a0, $a0, 1  # starting at the end of the string
   li $v0, 1         # if the string is a palindrome, $v0 will never be set to 0

loop:
   lb $t3, 0($t2)    # loading byte from the starting pointer
   lb $t4, 0($a0)    # loading byte from the ending pointer
   beq $t0, $t1, ret # check if the current counter and the length of the 
                     # string are the same
   bne $t3, $t4, not_pal # if the two chars being compared are not equal, the
                         # the string is not a palindrome
   addu $t2, $t2, 1  # increment starting pointer
   subu $a0, $a0, 1  # decrement ending pointer
   add $t1, $t1, 1   # i++
   
   j loop

not_pal:
   li $v0, 0         # result is set to 0 if the string is not a palindrome
   j ret

ret:
   move $sp, $fp           # restore $sp
   lw   $ra, ($fp)         # restore saved $ra
   lw   $fp, -4($sp)       # restore saved $fp
   j    $ra                # return to kernel

strlen:
   # PROLOGUE
   subu $sp, $sp, 8        # expand stack by 8 bytes
   sw   $ra, 8($sp)        # push $ra (ret addr, 4 bytes)
   sw   $fp, 4($sp)        # push $fp (4 bytes)
   addu $fp, $sp, 8        # set $fp to saved $ra
   # BODY
   li $t0, 0
count:
   lb $t1, 0($a0)          # load first byte into $t1
   beqz $t1, end           # checks if $t1 is at the end of the string
   addi $a0, $a0, 1        # increment string pointer
   addi $t0, $t0, 1        # increment counter
   j count

end:
   # EPILOGUE
   move $v0, $t0           # move strlen into $v0

   move $sp, $fp           # restore $sp
   lw   $ra, ($fp)         # restore saved $ra
   lw   $fp, -4($sp)       # restore saved $fp
   j    $ra                # return to kernel
