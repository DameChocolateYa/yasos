; ModuleID = 'MainModule'
source_filename = "MainModule"

@str = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

declare i32 @exit(i32)

declare i32 @println(ptr, ...)

define i32 @panic(ptr %msg) {
panic:
  %msg_local = alloca ptr, align 8
  store ptr %msg, ptr %msg_local, align 8
  %msg1 = load ptr, ptr %msg_local, align 8
  %println_res = call i32 (ptr, ...) @println(ptr @str, ptr %msg1)
  %exit_res = call i32 @exit(i32 1)
  ret i32 0
}

define i32 @cuspanic(ptr %msg, i32 %status) {
cuspanic:
  %msg_local = alloca ptr, align 8
  store ptr %msg, ptr %msg_local, align 8
  %status_local = alloca i32, align 4
  store i32 %status, ptr %status_local, align 4
  %msg1 = load ptr, ptr %msg_local, align 8
  %println_res = call i32 (ptr, ...) @println(ptr @str.1, ptr %msg1)
  %status2 = load i32, ptr %status_local, align 4
  %exit_res = call i32 @exit(i32 %status2)
  ret i32 0
}
