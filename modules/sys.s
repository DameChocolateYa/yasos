# sys.s

# Input: rdi = status
.globl sys_exit
sys_exit:
  mov $60, %rax
  syscall

# Input: rdi = path, rsi = flags, rdx = mode
# Output: rax = fd
.globl sys_open
sys_open:
  mov   %rdi, %r10
  mov   %rsi, %r11
  mov   %rdx, %r12
  mov   $257, %rax
  mov   $-100, %rdi
  mov   %r10, %rsi
  mov   %r11, %rdx
  mov   %r12, %r10
  mov   %rax, %rax
  syscall
  ret

# Input: rdi = fd
# Output: rax = status
.globl sys_close
sys_close:
  mov   $3, %rax
  syscall
  ret

# Input: rdi = fd, rsi = buffer pointer, rdx = num of bytes
# Output: rax = written bytes
.globl sys_write
sys_write:
  mov $1, %rax
  syscall
  ret

# Input: rdi = fd, rsi = buffer pointer
.globl sys_putchar
sys_putchar:
  mov $1, %rax
  mov $1, %rdx
  syscall
  ret

# Input: rdi = fd, rsi = buffer, rdx = bytes to read
# Output: rax = readed bytes
.globl sys_read
sys_read:
  mov   $0, %rax
  syscall
  ret

# Input: rdi = fd, rsi = offset, rdx = whence
# Output: rax = new file position
.globl sys_seek
sys_seek:
  mov   $8, %rax
  syscall
  ret

# Input rdi = seconds
.globl sys_sleep
sys_sleep:
  sub     $16, %rsp
  movsd   %xmm0, (%rsp)

  # tv_sec = trunc(seconds)
  cvttsd2si   %xmm0, %rax         # entero truncado
  mov         %rax, timespec(%rip)

  # frac = seconds - (double)tv_sec
  cvtsi2sd    %rax, %xmm1         # xmm1 = (double)tv_sec
  subsd       %xmm1, %xmm0        # xmm0 = frac

  # frac * 1e9
  movsd       scale(%rip), %xmm1  # xmm1 = 1e9
  mulsd       %xmm1, %xmm0

  # convertir a entero nanosegundos
  cvttsd2si   %xmm0, %rax
  mov         %rax, 8+timespec(%rip)

  add         $16, %rsp

  # syscall nanosleep
  mov     $35, %rax
  lea     timespec(%rip), %rdi
  xor     %rsi, %rsi
  syscall

  xor     %eax, %eax
  ret

  .section .rodata
  .align 8
scale:
  .double 1000000000.0   # 1e9

  .section .bss
  .align 8
timespec:
  .skip 16