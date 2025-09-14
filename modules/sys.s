# sys.s

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
