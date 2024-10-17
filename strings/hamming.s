#315112870 Mor Hanania 206945107 Yuval Arbel

.intel_syntax noprefix


.section .text
.global hamming_dist
hamming_dist:
    push rbp                    # Save base pointer
    mov rbp, rsp                # Set new base pointer

    xor rax, rax                # Clear eax register (result)

    # Save the str and str2 pointers.
    mov r8, rdi                 # Backup str1 pointer in r8
    mov r9, rsi                 # Backup str2 pointer in r9

    # Calculate length of str1 (rdi already points to str1)
    call strlen1
    mov rbx, rax                # Save length of str1 in rbx

    # Calculate length of str2 (rsi already points to str2)
    mov rdi, rsi
    call strlen1
    mov rcx, rax                # Save length of str2 in rcx

    # Restore pointers
    mov rdi, r8                 # Restore rdi to str1 pointer
    mov rsi, r9                 # Restore rsi to str2 pointer

    # Determine shorter string length for full register processing
    mov rdx, rbx                # Copy length of str1 to rdx
    cmp rbx, rcx
    cmovg rdx, rcx              # If rbx > rcx, move rcx to rdx

    # Calculate the number of full AVX-512 registers needed
    mov r8, rdx                # Set r8 to shorter length
    shr r8, 6                  # Divide by 64

    # Main loop for processing full 64-byte chunks
    xor rax, rax                # Clear rax (result accumulator)
    xor r10, r10                # Clear r10 (offset counter)
    .loop_full_registers:
        test r8, r8
        jz .handle_remaining

        vmovdqu64 zmm0, [rdi + r10]  # Load from str1
        vmovdqu64 zmm1, [rsi + r10]  # Load from str2
        vpcmpeqb k1, zmm0, zmm1
        kmovq r9, k1

        popcnt r9, r9               # Count the number of set bits (matches)
        mov r11, 64                 # Total number of bytes processed
        sub r11, r9                 # Subtract matches from total to get mismatches
        add rax, r11                # Accumulate mismatches in rax

        add r10, 64                # Increment offset counter
        dec r8                     # Decrement loop counter
        jmp .loop_full_registers

    # Handle remaining characters
    .handle_remaining:
        cmp rbx, rcx               # Compare lengths of str1 and str2
        jg .str2_is_shorter        # Jump if str1 is longer than str2

        # Process remaining elements for str1 (shorter string)
        mov rdx, rbx               # Use length of str1
        jmp .calc_remaining        # Jump to calculate remaining

    .str2_is_shorter:
        # Process remaining elements for str2 (shorter string)
        mov rdx, rcx               # Use length of str2

    .calc_remaining:
        mov r11, rcx               # Preserve original value of rcx in r11
        and rdx, 63                # Remaining elements in shorter string
        test rdx, rdx              # Check if there are remaining elements
        jz .done                   # Jump to done if no remaining elements

        # Create mask for remaining elements
        mov rcx, rdx               # Move remaining elements count to rcx
        mov r9, 1
        shl r9, cl                 # Shift left to create mask
        dec r9                     # Subtract 1 to finalize mask
        kmovq k2, r9               # Move mask to k2

        vmovdqu64 zmm0{k2}, [rdi + r10]  # Load remaining from str1
        vmovdqu64 zmm1{k2}, [rsi + r10]  # Load remaining from str2
        vpcmpeqb k1 {k2}, zmm0, zmm1     # Compare remaining bytes
        kmovq r9, k1
        popcnt r9, r9
        sub rdx, r9                # Calculate mismatches
        add rax, rdx               # Add mismatches to total distanc

     .done:
        # Calculate length difference
        mov rcx, r11               # Restore original value of rcx from r11
        mov rdx, rbx             # Move length of str1 to rdx
        sub rdx, rcx             # Subtract length of str2 from rdx (rdx = length difference)
        jns .length_diff_done    # If rdx is non-negative, jump to length_diff_done

        neg rdx                  # If rdx is negative, make it positive

      .length_diff_done:
        add rax, rdx             # Add length difference to total Hamming distance

        mov rsp, rbp             # Restore previous stack frame
        pop rbp                  # Restore previous base pointer
        ret                      # Return from function

strlen1:
    push rbp                    # Save base pointer
    mov rbp, rsp                # Set new base pointer

    xor rax, rax                 # Clear rax (result)
 .loop:
     vmovdqu64 zmm0, [rdi + rax]  # Load 64 bytes from the string
     vptestnmb k1, zmm0, zmm0     # Test each byte for null character
     kmovq rcx, k1                # Move mask to rcx
     tzcnt rcx, rcx               # Count trailing zeros in rcx (find first zero bit)
     jnc .found_null              # Jump if a zero bit is found (null byte)

     add rax, 64                  # Increment position for next 64-byte block
     jmp .loop

 .found_null:
     add rax, rcx                 # Add position of first null
     mov rsp, rbp             # Restore previous stack frame
     pop rbp                  # Restore previous base pointer
     ret                      # Return from function
