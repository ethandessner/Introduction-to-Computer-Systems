   .data
str1:
   .asciiz "abba"
str2:
   .asciiz "racecar"
str3:
   .asciiz "swap paws",
str4:
   .asciiz "not a palindrome"
str5:
   .asciiz "another non palindrome"
str6:
   .asciiz "almost but tsomla"

# array of char pointers = {&str1, &str2, ..., &str6}
ptr_arr:
   .word str1, str2, str3, str4, str5, str6, 0

yes_str:
   .asciiz " --> Y\n"
no_str:
   .asciiz " --> N\n"

   .text

# main(): ##################################################
#   char ** j = ptr_arr
#   while (*j != 0):
#     rval = is_palindrome(*j)
#     printf("%s --> %c\n", *j, rval ? yes_str: no_str)
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

   la   $s0, ptr_arr        # use s0 for j. init ptr_arr
main_while:
   lw   $s1, ($s0)         # use s1 for *j
   beqz $s1, main_end      # while (*j != 0):
   move $a0, $s1           #    print_str(*j)
   li   $v0, 4
   syscall
   move $a0, $s1           #    v0 = is_palindrome(*j)
   jal  is_palindrome
   beqz $v0, main_print_no #    if v0 != 0:
   la   $a0, yes_str       #       print_str(yes_str)
   b    main_print_resp
main_print_no:             #    else:
   la   $a0, no_str        #       print_str(no_str)
main_print_resp:
   li   $v0, 4
   syscall

   addu $s0, $s0, 4       #     j++
   b    main_while        # end while
main_end:

   # EPILOGUE
   move $sp, $fp           # restore $sp
   lw   $ra, ($fp)         # restore saved $ra
   lw   $fp, -4($sp)       # restore saved $fp
   j    $ra                # return to kernel
# end main ################################################

is_palindrome:
# first check the length of your argument
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
   lb $t1, 0($a0)
   beqz $t1, end
   addi $a0, $a0, 1
   addi $t0, $t0, 1
   j count

end:
   # EPILOGUE
   move $v0, $t0

   move $sp, $fp           # restore $sp
   lw   $ra, ($fp)         # restore saved $ra
   lw   $fp, -4($sp)       # restore saved $fp
   j    $ra                # return to kernel