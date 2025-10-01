use std::{malloc, free};
use string::{len};
use byte::{bytostr};

struct File {
  fd: int; #- Descriptor
  buffer: &str;
  buf_size: int;
  pos: int;
  flags: int;
  is_open: int;
};

extern fun sys_open(path: str, flags: int, mode: int) -> int;
extern fun sys_close(fd: int) -> none;
extern fun sys_write(fd: int, data: str, bytes: int) -> int;
extern fun sys_read(fd: int, buf: &str, bytes: int) -> int;
extern fun sys_seek(fd: int, offset: int, whence: int) -> int;

fun file_open(path: str, flags: int) -> File {
  var f: File;
  f.fd = sys_open(path, flags, 420);
  if f.fd < 0 {
    "ERROR: could not open file\n";
    ret f;
  }
  f.buf_size = 4096;
  f.buffer = malloc(f.buf_size);
  if f.buffer == nullptr {
    "ERROR: could not allocate file buffer\n";
    ret f;
  }
  var s: &str = f.buffer;
  deref s = "";
  f.buffer = s;

  f.pos = 0;
  f.flags = flags;
  f.is_open = true;
  ret f;
} pub;

fun file_close(f: &File) -> none {
  var local_f: File;
  local_f = *f;

  if local_f.fd >= 0 {
    sys_close(local_f.fd);
  }

  if local_f.buffer != nullptr {
    free(local_f.buffer);
  }
  local_f.buffer = nullptr;
  local_f.is_open = false;

  *f = local_f;
} pub;

fun file_isopen(f: File) -> int { #- No needed more since is_open field exists
  if f.buffer == nullptr {ret false;}
  ret true;
} pub;

fun file_write(f: &File, data: str) -> int {
  var local_f: File = *f;
  if local_f.fd < 0 {
    "ERROR: file not open\n";
    ret -1;
  }
  var bytes: int = len(data);
  var bytes_written: int = sys_write(local_f.fd, data, bytes);
  local_f.pos = bytes_written;

  deref f = local_f;
  ret bytes_written;
} pub;

fun file_read(f: &File) -> str {
  var local_f: File = deref f;
  if local_f.fd < 0 {
    "ERROR: file not open\n";
    ret "(null)";
  }
  if local_f.buffer == nullptr {
    "ERROR: file with a NULL buffer\n";
    ret "(null)";
  }
  var local_buf: &str = local_f.buffer;
  var bytes_read: int = sys_read(local_f.fd, local_buf, local_f.buf_size);
  if bytes_read <= 0 {
    ret "";
  } 
  var s: str = bytostr(local_buf, bytes_read - 1); #- I dunno know why can't be a deref

  deref f = local_f;

  ret s;
} pub;

fun file_seek(f: &File, pos: int, whence: int) -> none {
  var local_f: File = deref f;
  if local_f.fd < 0 {
    "ERROR: file not open\n";
    leave;
  }
  
  var new_pos: int = sys_seek(local_f.fd, pos, whence);
  if new_pos < 0 {
    "ERROR: seek failed";
    leave;
  }
  local_f.pos = new_pos;
  deref f = local_f;
} pub;
