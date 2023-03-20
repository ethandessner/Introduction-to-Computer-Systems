   .data

# array terminated by 0 (which is not part of the array)
xarr:
   .word 1
   .word 2
   .word 3
   .word 4
   .word 10
   .word 11
   .word 12
   .word 13
   .word 14
   .word 15
   .word 16
   .word 17
   .word 18
   .word 19
   .word 20
   .word 21
   .word 22
   .word 23
   .word 24
   .word 0

   .text

# main(): ##################################################
#   uint* j = xarr
#   while (*j != 0):
#     printf(" %d\n", fibonacci(*j))
#     j++
#
main:
   li   $sp, 0x7ffffffc    # initialize $sp

   # PROLOGUE
   subu $sp, $sp, 8        # expand stack by 8 bytes
   sw   $ra, 8($sp)        # push $ra (ret addr, 4 bytes)
   sw   $fp, 4($sp)        # push $fp (4 bytes)
   addu $fp, $sp, 8        # set $fp to saved $ra

   subu $sp, $sp, 8        # save s0, s1 on stack before using them
   sw   $s0, 8($sp)        # push $s0
   sw   $s1, 4($sp)        # push $s1

   la   $s0, xarr          # use s0 for j. init to xarr
main_while:
   lw   $s1, ($s0)         # use s1 for *j
   beqz $s1, main_end      # if *j == 0 go to main_end
   move $a0, $s1
   jal  fibonacci          # result = fibonacci(*j)
   move $a0, $v0           # print_int(result)
   li   $v0, 1
   syscall
   li   $a0, 10            # print_char('\n')
   li   $v0, 11
   syscall
   addu $s0, $s0, 4        # j++
   b    main_while
main_end:
   lw   $s0, -8($fp)       # restore s0
   lw   $s1, -12($fp)      # restore s1

   # EPILOGUE
   move $sp, $fp           # restore $sp
   lw   $ra, ($fp)         # restore saved $ra
   lw   $fp, -4($sp)       # restore saved $fp
   j    $ra                # return to kernel
## end main #################################################

fibonacci: 
   li $t0, 1   # set $t0 to 1
   beq $a0, $0, zero # branch to zero if n = 0
   beq $a0, $t0, one  # branch to one if n = 1
   jr fib_recurse

zero:
   li $v0, 0   # return 0 if n == 0
   jr $ra

one:
   li $v0, 1   # return 1 if n == 1
   jr $ra

fib_recurse:
   # PROLOGUE
   subu $sp, $sp, 16  # Make room for 4 elements in stack
                       # $ra and n stored now, 
                       # sums will be stored later
   sw $ra, 0($sp)  # Save return address
   sw $a0, 4($sp)  # Save n

   # BODY
   # Calculate fib(n-1)
   addi $a0, $a0, -1  # n -1
   jal fibonacci   # recurse for fib(n-1)
   sw $v0, 8($sp)  # Save value of fib(n-1)

   # Calculate fib(n-2)
   lw $a0, 4($sp)  # restore value of i from stack
   addi $a0, $a0, -2  # n - 2
   jal fibonacci   # recurse for fib(n-2)
   sw $v0, 12($sp)  # save result of fib(n-2)

   # EPILOGUE
   # Restore from stack and sum
   lw $ra, 0($sp)  # Load return address
   lw $t0, 8($sp)  # load fib(n-1)
   lw $t1, 12($sp)  # load fib(n-2)
   addi $sp, $sp, 16  # free up 4 elements on stack
   add $v0, $t0, $t1  # Sum fib(n-1) + fib(n-2)
   jr $ra