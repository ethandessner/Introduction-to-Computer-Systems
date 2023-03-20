# Ethan Dessner 117919219 edessner
fibonacci: 
   beq $a0, $0, zero # branch to zero if n = 0
   beq $a0, 1, one  # branch to one if n = 1
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