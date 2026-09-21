; ModuleID = 'vector.c'
source_filename = "vector.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.Vec = type { ptr, i32, i32, i32, i8 }
%struct.__va_list_tag = type { i32, i32, ptr, ptr }
%struct.String = type { ptr, i32, i8 }

@stderr = external global ptr, align 8
@.str = private unnamed_addr constant [46 x i8] c"Vec::new: could not allocate memory for data\0A\00", align 1
@.str.1 = private unnamed_addr constant [45 x i8] c"cVec_push: Could not expand memory for push\0A\00", align 1
@.str.2 = private unnamed_addr constant [32 x i8] c"cVec_erase: index out of range\0A\00", align 1
@.str.3 = private unnamed_addr constant [40 x i8] c"cVec_edit(generic): index out of range\0A\00", align 1
@.str.4 = private unnamed_addr constant [32 x i8] c"get$MODVec: index out of range\0A\00", align 1
@.str.5 = private unnamed_addr constant [60 x i8] c"set$MODVecim: index out of range - omiting current element\0A\00", align 1
@.str.6 = private unnamed_addr constant [65 x i8] c"set$MODVecstringm: index out of range - omiting current element\0A\00", align 1
@.str.7 = private unnamed_addr constant [60 x i8] c"set$MODVecdm: index out of range - omiting current element\0A\00", align 1
@.str.8 = private unnamed_addr constant [62 x i8] c"set$MODVecvecm: index out of range - omiting current element\0A\00", align 1
@.str.9 = private unnamed_addr constant [34 x i8] c"Vec_cpy: error allocating memory\0A\00", align 1
@.str.10 = private unnamed_addr constant [51 x i8] c"Vec_cpy_and_push: vectors elem size are not equal\0A\00", align 1
@.str.11 = private unnamed_addr constant [38 x i8] c"Vec_cpy_and_push: index out of range\0A\00", align 1
@.str.12 = private unnamed_addr constant [52 x i8] c"Vec_cpy_and_pushm: vectors elem size are not equal\0A\00", align 1
@.str.13 = private unnamed_addr constant [65 x i8] c"Vec_cpy_and_pushm: index out of range - omiting current element\0A\00", align 1
@.str.14 = private unnamed_addr constant [45 x i8] c"Vec_cpy_in: vectors elem size are not equal\0A\00", align 1
@.str.15 = private unnamed_addr constant [32 x i8] c"Vec_cpy_in: index out of range\0A\00", align 1
@.str.16 = private unnamed_addr constant [62 x i8] c"Vec::clone: Could not clone Vector (error allocating memory)\0A\00", align 1

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"new$MODVec"(ptr dead_on_unwind noalias writable sret(%struct.Vec) align 8 %0, i32 noundef %1) #0 {
  %3 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  call void @llvm.memset.p0.i64(ptr align 8 %0, i8 0, i64 24, i1 false)
  %4 = load i32, ptr %3, align 4
  %5 = mul nsw i32 %4, 2
  %6 = call ptr @"alloc$MODmem"(i32 noundef %5)
  %7 = getelementptr inbounds nuw %struct.Vec, ptr %0, i32 0, i32 0
  store ptr %6, ptr %7, align 8
  %8 = getelementptr inbounds nuw %struct.Vec, ptr %0, i32 0, i32 0
  %9 = load ptr, ptr %8, align 8
  %10 = icmp ne ptr %9, null
  br i1 %10, label %14, label %11

11:                                               ; preds = %2
  %12 = load ptr, ptr @stderr, align 8
  %13 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %12, ptr noundef @.str) #6
  br label %20

14:                                               ; preds = %2
  %15 = getelementptr inbounds nuw %struct.Vec, ptr %0, i32 0, i32 1
  store i32 0, ptr %15, align 8
  %16 = getelementptr inbounds nuw %struct.Vec, ptr %0, i32 0, i32 2
  store i32 2, ptr %16, align 4
  %17 = getelementptr inbounds nuw %struct.Vec, ptr %0, i32 0, i32 4
  store i8 1, ptr %17, align 4
  %18 = load i32, ptr %3, align 4
  %19 = getelementptr inbounds nuw %struct.Vec, ptr %0, i32 0, i32 3
  store i32 %18, ptr %19, align 8
  br label %20

20:                                               ; preds = %14, %11
  ret void
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr writeonly captures(none), i8, i64, i1 immarg) #1

declare ptr @"alloc$MODmem"(i32 noundef) #2

; Function Attrs: nounwind
declare i32 @fprintf(ptr noundef, ptr noundef, ...) #3

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"push$MODVec"(ptr noundef %0, ptr noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
  store ptr %1, ptr %4, align 8
  %5 = load ptr, ptr %3, align 8
  %6 = getelementptr inbounds nuw %struct.Vec, ptr %5, i32 0, i32 0
  %7 = load ptr, ptr %6, align 8
  %8 = icmp ne ptr %7, null
  br i1 %8, label %9, label %17

9:                                                ; preds = %2
  %10 = load ptr, ptr %3, align 8
  %11 = getelementptr inbounds nuw %struct.Vec, ptr %10, i32 0, i32 4
  %12 = load i8, ptr %11, align 4
  %13 = trunc i8 %12 to i1
  br i1 %13, label %14, label %17

14:                                               ; preds = %9
  %15 = load ptr, ptr %4, align 8
  %16 = icmp ne ptr %15, null
  br i1 %16, label %18, label %17

17:                                               ; preds = %14, %9, %2
  br label %75

18:                                               ; preds = %14
  %19 = load ptr, ptr %3, align 8
  %20 = getelementptr inbounds nuw %struct.Vec, ptr %19, i32 0, i32 1
  %21 = load i32, ptr %20, align 8
  %22 = add nsw i32 %21, 1
  %23 = load ptr, ptr %3, align 8
  %24 = getelementptr inbounds nuw %struct.Vec, ptr %23, i32 0, i32 2
  %25 = load i32, ptr %24, align 4
  %26 = icmp sge i32 %22, %25
  br i1 %26, label %27, label %53

27:                                               ; preds = %18
  %28 = load ptr, ptr %3, align 8
  %29 = getelementptr inbounds nuw %struct.Vec, ptr %28, i32 0, i32 2
  %30 = load i32, ptr %29, align 4
  %31 = mul nsw i32 %30, 2
  store i32 %31, ptr %29, align 4
  %32 = load ptr, ptr %3, align 8
  %33 = getelementptr inbounds nuw %struct.Vec, ptr %32, i32 0, i32 0
  %34 = load ptr, ptr %33, align 8
  %35 = load ptr, ptr %3, align 8
  %36 = getelementptr inbounds nuw %struct.Vec, ptr %35, i32 0, i32 2
  %37 = load i32, ptr %36, align 4
  %38 = load ptr, ptr %3, align 8
  %39 = getelementptr inbounds nuw %struct.Vec, ptr %38, i32 0, i32 3
  %40 = load i32, ptr %39, align 8
  %41 = mul nsw i32 %37, %40
  %42 = call ptr @"realloc$MODmem"(ptr noundef %34, i32 noundef %41)
  %43 = load ptr, ptr %3, align 8
  %44 = getelementptr inbounds nuw %struct.Vec, ptr %43, i32 0, i32 0
  store ptr %42, ptr %44, align 8
  %45 = load ptr, ptr %3, align 8
  %46 = getelementptr inbounds nuw %struct.Vec, ptr %45, i32 0, i32 0
  %47 = load ptr, ptr %46, align 8
  %48 = icmp ne ptr %47, null
  br i1 %48, label %52, label %49

49:                                               ; preds = %27
  %50 = load ptr, ptr @stderr, align 8
  %51 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %50, ptr noundef @.str.1) #6
  br label %75

52:                                               ; preds = %27
  br label %53

53:                                               ; preds = %52, %18
  %54 = load ptr, ptr %3, align 8
  %55 = getelementptr inbounds nuw %struct.Vec, ptr %54, i32 0, i32 0
  %56 = load ptr, ptr %55, align 8
  %57 = load ptr, ptr %3, align 8
  %58 = getelementptr inbounds nuw %struct.Vec, ptr %57, i32 0, i32 1
  %59 = load i32, ptr %58, align 8
  %60 = load ptr, ptr %3, align 8
  %61 = getelementptr inbounds nuw %struct.Vec, ptr %60, i32 0, i32 3
  %62 = load i32, ptr %61, align 8
  %63 = mul nsw i32 %59, %62
  %64 = sext i32 %63 to i64
  %65 = getelementptr inbounds i8, ptr %56, i64 %64
  %66 = load ptr, ptr %4, align 8
  %67 = load ptr, ptr %3, align 8
  %68 = getelementptr inbounds nuw %struct.Vec, ptr %67, i32 0, i32 3
  %69 = load i32, ptr %68, align 8
  %70 = sext i32 %69 to i64
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %65, ptr align 1 %66, i64 %70, i1 false)
  %71 = load ptr, ptr %3, align 8
  %72 = getelementptr inbounds nuw %struct.Vec, ptr %71, i32 0, i32 1
  %73 = load i32, ptr %72, align 8
  %74 = add nsw i32 %73, 1
  store i32 %74, ptr %72, align 8
  br label %75

75:                                               ; preds = %53, %49, %17
  ret void
}

declare ptr @"realloc$MODmem"(ptr noundef, i32 noundef) #2

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #4

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"new_ptr$MODVec"(ptr dead_on_unwind noalias writable sret(%struct.Vec) align 8 %0, i32 noundef %1, ...) #0 {
  %3 = alloca i32, align 4
  %4 = alloca [1 x %struct.__va_list_tag], align 16
  %5 = alloca i32, align 4
  %6 = alloca ptr, align 8
  store i32 %1, ptr %3, align 4
  %7 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %4, i64 0, i64 0
  call void @llvm.va_start.p0(ptr %7)
  call void @"new$MODVec"(ptr dead_on_unwind writable sret(%struct.Vec) align 8 %0, i32 noundef 8)
  store i32 0, ptr %5, align 4
  br label %8

8:                                                ; preds = %29, %2
  %9 = load i32, ptr %5, align 4
  %10 = load i32, ptr %3, align 4
  %11 = icmp slt i32 %9, %10
  br i1 %11, label %12, label %32

12:                                               ; preds = %8
  %13 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %4, i64 0, i64 0
  %14 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %13, i32 0, i32 0
  %15 = load i32, ptr %14, align 16
  %16 = icmp ule i32 %15, 40
  br i1 %16, label %17, label %22

17:                                               ; preds = %12
  %18 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %13, i32 0, i32 3
  %19 = load ptr, ptr %18, align 16
  %20 = getelementptr i8, ptr %19, i32 %15
  %21 = add i32 %15, 8
  store i32 %21, ptr %14, align 16
  br label %26

22:                                               ; preds = %12
  %23 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %13, i32 0, i32 2
  %24 = load ptr, ptr %23, align 8
  %25 = getelementptr i8, ptr %24, i32 8
  store ptr %25, ptr %23, align 8
  br label %26

26:                                               ; preds = %22, %17
  %27 = phi ptr [ %20, %17 ], [ %24, %22 ]
  %28 = load ptr, ptr %27, align 8
  store ptr %28, ptr %6, align 8
  call void @"push$MODVec"(ptr noundef %0, ptr noundef %6)
  br label %29

29:                                               ; preds = %26
  %30 = load i32, ptr %5, align 4
  %31 = add nsw i32 %30, 1
  store i32 %31, ptr %5, align 4
  br label %8, !llvm.loop !6

32:                                               ; preds = %8
  %33 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %4, i64 0, i64 0
  call void @llvm.va_end.p0(ptr %33)
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.va_start.p0(ptr) #5

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.va_end.p0(ptr) #5

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"new_int$MODVec"(ptr dead_on_unwind noalias writable sret(%struct.Vec) align 8 %0, i32 noundef %1, ...) #0 {
  %3 = alloca i32, align 4
  %4 = alloca [1 x %struct.__va_list_tag], align 16
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  store i32 %1, ptr %3, align 4
  %7 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %4, i64 0, i64 0
  call void @llvm.va_start.p0(ptr %7)
  call void @"new$MODVec"(ptr dead_on_unwind writable sret(%struct.Vec) align 8 %0, i32 noundef 4)
  store i32 0, ptr %5, align 4
  br label %8

8:                                                ; preds = %29, %2
  %9 = load i32, ptr %5, align 4
  %10 = load i32, ptr %3, align 4
  %11 = icmp slt i32 %9, %10
  br i1 %11, label %12, label %32

12:                                               ; preds = %8
  %13 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %4, i64 0, i64 0
  %14 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %13, i32 0, i32 0
  %15 = load i32, ptr %14, align 16
  %16 = icmp ule i32 %15, 40
  br i1 %16, label %17, label %22

17:                                               ; preds = %12
  %18 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %13, i32 0, i32 3
  %19 = load ptr, ptr %18, align 16
  %20 = getelementptr i8, ptr %19, i32 %15
  %21 = add i32 %15, 8
  store i32 %21, ptr %14, align 16
  br label %26

22:                                               ; preds = %12
  %23 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %13, i32 0, i32 2
  %24 = load ptr, ptr %23, align 8
  %25 = getelementptr i8, ptr %24, i32 8
  store ptr %25, ptr %23, align 8
  br label %26

26:                                               ; preds = %22, %17
  %27 = phi ptr [ %20, %17 ], [ %24, %22 ]
  %28 = load i32, ptr %27, align 4
  store i32 %28, ptr %6, align 4
  call void @"push$MODVec"(ptr noundef %0, ptr noundef %6)
  br label %29

29:                                               ; preds = %26
  %30 = load i32, ptr %5, align 4
  %31 = add nsw i32 %30, 1
  store i32 %31, ptr %5, align 4
  br label %8, !llvm.loop !8

32:                                               ; preds = %8
  %33 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %4, i64 0, i64 0
  call void @llvm.va_end.p0(ptr %33)
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"new_string$MODVec"(ptr dead_on_unwind noalias writable sret(%struct.Vec) align 8 %0, i32 noundef %1, ...) #0 {
  %3 = alloca i32, align 4
  %4 = alloca [1 x %struct.__va_list_tag], align 16
  %5 = alloca i32, align 4
  %6 = alloca ptr, align 8
  store i32 %1, ptr %3, align 4
  %7 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %4, i64 0, i64 0
  call void @llvm.va_start.p0(ptr %7)
  call void @"new$MODVec"(ptr dead_on_unwind writable sret(%struct.Vec) align 8 %0, i32 noundef 8)
  store i32 0, ptr %5, align 4
  br label %8

8:                                                ; preds = %29, %2
  %9 = load i32, ptr %5, align 4
  %10 = load i32, ptr %3, align 4
  %11 = icmp slt i32 %9, %10
  br i1 %11, label %12, label %32

12:                                               ; preds = %8
  %13 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %4, i64 0, i64 0
  %14 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %13, i32 0, i32 0
  %15 = load i32, ptr %14, align 16
  %16 = icmp ule i32 %15, 40
  br i1 %16, label %17, label %22

17:                                               ; preds = %12
  %18 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %13, i32 0, i32 3
  %19 = load ptr, ptr %18, align 16
  %20 = getelementptr i8, ptr %19, i32 %15
  %21 = add i32 %15, 8
  store i32 %21, ptr %14, align 16
  br label %26

22:                                               ; preds = %12
  %23 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %13, i32 0, i32 2
  %24 = load ptr, ptr %23, align 8
  %25 = getelementptr i8, ptr %24, i32 8
  store ptr %25, ptr %23, align 8
  br label %26

26:                                               ; preds = %22, %17
  %27 = phi ptr [ %20, %17 ], [ %24, %22 ]
  %28 = load ptr, ptr %27, align 8
  store ptr %28, ptr %6, align 8
  call void @"push$MODVec"(ptr noundef %0, ptr noundef %6)
  br label %29

29:                                               ; preds = %26
  %30 = load i32, ptr %5, align 4
  %31 = add nsw i32 %30, 1
  store i32 %31, ptr %5, align 4
  br label %8, !llvm.loop !9

32:                                               ; preds = %8
  %33 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %4, i64 0, i64 0
  call void @llvm.va_end.p0(ptr %33)
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"new_double$MODVec"(ptr dead_on_unwind noalias writable sret(%struct.Vec) align 8 %0, i32 noundef %1, ...) #0 {
  %3 = alloca i32, align 4
  %4 = alloca [1 x %struct.__va_list_tag], align 16
  %5 = alloca i32, align 4
  %6 = alloca double, align 8
  store i32 %1, ptr %3, align 4
  %7 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %4, i64 0, i64 0
  call void @llvm.va_start.p0(ptr %7)
  call void @"new$MODVec"(ptr dead_on_unwind writable sret(%struct.Vec) align 8 %0, i32 noundef 8)
  store i32 0, ptr %5, align 4
  br label %8

8:                                                ; preds = %29, %2
  %9 = load i32, ptr %5, align 4
  %10 = load i32, ptr %3, align 4
  %11 = icmp slt i32 %9, %10
  br i1 %11, label %12, label %32

12:                                               ; preds = %8
  %13 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %4, i64 0, i64 0
  %14 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %13, i32 0, i32 1
  %15 = load i32, ptr %14, align 4
  %16 = icmp ule i32 %15, 160
  br i1 %16, label %17, label %22

17:                                               ; preds = %12
  %18 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %13, i32 0, i32 3
  %19 = load ptr, ptr %18, align 16
  %20 = getelementptr i8, ptr %19, i32 %15
  %21 = add i32 %15, 16
  store i32 %21, ptr %14, align 4
  br label %26

22:                                               ; preds = %12
  %23 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %13, i32 0, i32 2
  %24 = load ptr, ptr %23, align 8
  %25 = getelementptr i8, ptr %24, i32 8
  store ptr %25, ptr %23, align 8
  br label %26

26:                                               ; preds = %22, %17
  %27 = phi ptr [ %20, %17 ], [ %24, %22 ]
  %28 = load double, ptr %27, align 8
  store double %28, ptr %6, align 8
  call void @"push$MODVec"(ptr noundef %0, ptr noundef %6)
  br label %29

29:                                               ; preds = %26
  %30 = load i32, ptr %5, align 4
  %31 = add nsw i32 %30, 1
  store i32 %31, ptr %5, align 4
  br label %8, !llvm.loop !10

32:                                               ; preds = %8
  %33 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %4, i64 0, i64 0
  call void @llvm.va_end.p0(ptr %33)
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"new_char$MODVec"(ptr dead_on_unwind noalias writable sret(%struct.Vec) align 8 %0, i32 noundef %1, ...) #0 {
  %3 = alloca i32, align 4
  %4 = alloca [1 x %struct.__va_list_tag], align 16
  %5 = alloca i32, align 4
  %6 = alloca i8, align 1
  store i32 %1, ptr %3, align 4
  %7 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %4, i64 0, i64 0
  call void @llvm.va_start.p0(ptr %7)
  call void @"new$MODVec"(ptr dead_on_unwind writable sret(%struct.Vec) align 8 %0, i32 noundef 1)
  store i32 0, ptr %5, align 4
  br label %8

8:                                                ; preds = %30, %2
  %9 = load i32, ptr %5, align 4
  %10 = load i32, ptr %3, align 4
  %11 = icmp slt i32 %9, %10
  br i1 %11, label %12, label %33

12:                                               ; preds = %8
  %13 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %4, i64 0, i64 0
  %14 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %13, i32 0, i32 0
  %15 = load i32, ptr %14, align 16
  %16 = icmp ule i32 %15, 40
  br i1 %16, label %17, label %22

17:                                               ; preds = %12
  %18 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %13, i32 0, i32 3
  %19 = load ptr, ptr %18, align 16
  %20 = getelementptr i8, ptr %19, i32 %15
  %21 = add i32 %15, 8
  store i32 %21, ptr %14, align 16
  br label %26

22:                                               ; preds = %12
  %23 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %13, i32 0, i32 2
  %24 = load ptr, ptr %23, align 8
  %25 = getelementptr i8, ptr %24, i32 8
  store ptr %25, ptr %23, align 8
  br label %26

26:                                               ; preds = %22, %17
  %27 = phi ptr [ %20, %17 ], [ %24, %22 ]
  %28 = load i32, ptr %27, align 4
  %29 = trunc i32 %28 to i8
  store i8 %29, ptr %6, align 1
  call void @"push$MODVec"(ptr noundef %0, ptr noundef %6)
  br label %30

30:                                               ; preds = %26
  %31 = load i32, ptr %5, align 4
  %32 = add nsw i32 %31, 1
  store i32 %32, ptr %5, align 4
  br label %8, !llvm.loop !11

33:                                               ; preds = %8
  %34 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %4, i64 0, i64 0
  call void @llvm.va_end.p0(ptr %34)
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"new_vec$MODVec"(ptr dead_on_unwind noalias writable sret(%struct.Vec) align 8 %0, i32 noundef %1, ...) #0 {
  %3 = alloca i32, align 4
  %4 = alloca [1 x %struct.__va_list_tag], align 16
  %5 = alloca i32, align 4
  %6 = alloca ptr, align 8
  store i32 %1, ptr %3, align 4
  %7 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %4, i64 0, i64 0
  call void @llvm.va_start.p0(ptr %7)
  call void @"new$MODVec"(ptr dead_on_unwind writable sret(%struct.Vec) align 8 %0, i32 noundef 8)
  store i32 0, ptr %5, align 4
  br label %8

8:                                                ; preds = %29, %2
  %9 = load i32, ptr %5, align 4
  %10 = load i32, ptr %3, align 4
  %11 = icmp slt i32 %9, %10
  br i1 %11, label %12, label %32

12:                                               ; preds = %8
  %13 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %4, i64 0, i64 0
  %14 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %13, i32 0, i32 0
  %15 = load i32, ptr %14, align 16
  %16 = icmp ule i32 %15, 40
  br i1 %16, label %17, label %22

17:                                               ; preds = %12
  %18 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %13, i32 0, i32 3
  %19 = load ptr, ptr %18, align 16
  %20 = getelementptr i8, ptr %19, i32 %15
  %21 = add i32 %15, 8
  store i32 %21, ptr %14, align 16
  br label %26

22:                                               ; preds = %12
  %23 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %13, i32 0, i32 2
  %24 = load ptr, ptr %23, align 8
  %25 = getelementptr i8, ptr %24, i32 8
  store ptr %25, ptr %23, align 8
  br label %26

26:                                               ; preds = %22, %17
  %27 = phi ptr [ %20, %17 ], [ %24, %22 ]
  %28 = load ptr, ptr %27, align 8
  store ptr %28, ptr %6, align 8
  call void @"push$MODVec"(ptr noundef %0, ptr noundef %6)
  br label %29

29:                                               ; preds = %26
  %30 = load i32, ptr %5, align 4
  %31 = add nsw i32 %30, 1
  store i32 %31, ptr %5, align 4
  br label %8, !llvm.loop !12

32:                                               ; preds = %8
  %33 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %4, i64 0, i64 0
  call void @llvm.va_end.p0(ptr %33)
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"pop$MODVec"(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  %3 = load ptr, ptr %2, align 8
  %4 = getelementptr inbounds nuw %struct.Vec, ptr %3, i32 0, i32 0
  %5 = load ptr, ptr %4, align 8
  %6 = icmp ne ptr %5, null
  br i1 %6, label %7, label %17

7:                                                ; preds = %1
  %8 = load ptr, ptr %2, align 8
  %9 = getelementptr inbounds nuw %struct.Vec, ptr %8, i32 0, i32 4
  %10 = load i8, ptr %9, align 4
  %11 = trunc i8 %10 to i1
  br i1 %11, label %12, label %17

12:                                               ; preds = %7
  %13 = load ptr, ptr %2, align 8
  %14 = getelementptr inbounds nuw %struct.Vec, ptr %13, i32 0, i32 1
  %15 = load i32, ptr %14, align 8
  %16 = icmp sle i32 %15, 0
  br i1 %16, label %17, label %18

17:                                               ; preds = %12, %7, %1
  br label %39

18:                                               ; preds = %12
  %19 = load ptr, ptr %2, align 8
  %20 = getelementptr inbounds nuw %struct.Vec, ptr %19, i32 0, i32 1
  %21 = load i32, ptr %20, align 8
  %22 = sub nsw i32 %21, 1
  store i32 %22, ptr %20, align 8
  %23 = load ptr, ptr %2, align 8
  %24 = getelementptr inbounds nuw %struct.Vec, ptr %23, i32 0, i32 0
  %25 = load ptr, ptr %24, align 8
  %26 = load ptr, ptr %2, align 8
  %27 = getelementptr inbounds nuw %struct.Vec, ptr %26, i32 0, i32 1
  %28 = load i32, ptr %27, align 8
  %29 = load ptr, ptr %2, align 8
  %30 = getelementptr inbounds nuw %struct.Vec, ptr %29, i32 0, i32 3
  %31 = load i32, ptr %30, align 8
  %32 = mul nsw i32 %28, %31
  %33 = sext i32 %32 to i64
  %34 = getelementptr inbounds i8, ptr %25, i64 %33
  %35 = load ptr, ptr %2, align 8
  %36 = getelementptr inbounds nuw %struct.Vec, ptr %35, i32 0, i32 3
  %37 = load i32, ptr %36, align 8
  %38 = sext i32 %37 to i64
  call void @llvm.memset.p0.i64(ptr align 1 %34, i8 -35, i64 %38, i1 false)
  br label %39

39:                                               ; preds = %18, %17
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"erase_at$MODVec"(ptr noundef %0, i32 noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  store ptr %0, ptr %3, align 8
  store i32 %1, ptr %4, align 4
  %5 = load ptr, ptr %3, align 8
  %6 = getelementptr inbounds nuw %struct.Vec, ptr %5, i32 0, i32 0
  %7 = load ptr, ptr %6, align 8
  %8 = icmp ne ptr %7, null
  br i1 %8, label %9, label %14

9:                                                ; preds = %2
  %10 = load ptr, ptr %3, align 8
  %11 = getelementptr inbounds nuw %struct.Vec, ptr %10, i32 0, i32 4
  %12 = load i8, ptr %11, align 4
  %13 = trunc i8 %12 to i1
  br i1 %13, label %15, label %14

14:                                               ; preds = %9, %2
  br label %64

15:                                               ; preds = %9
  %16 = load i32, ptr %4, align 4
  %17 = icmp slt i32 %16, 0
  br i1 %17, label %24, label %18

18:                                               ; preds = %15
  %19 = load i32, ptr %4, align 4
  %20 = load ptr, ptr %3, align 8
  %21 = getelementptr inbounds nuw %struct.Vec, ptr %20, i32 0, i32 1
  %22 = load i32, ptr %21, align 8
  %23 = icmp sge i32 %19, %22
  br i1 %23, label %24, label %27

24:                                               ; preds = %18, %15
  %25 = load ptr, ptr @stderr, align 8
  %26 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %25, ptr noundef @.str.2) #6
  br label %64

27:                                               ; preds = %18
  %28 = load ptr, ptr %3, align 8
  %29 = getelementptr inbounds nuw %struct.Vec, ptr %28, i32 0, i32 0
  %30 = load ptr, ptr %29, align 8
  %31 = load i32, ptr %4, align 4
  %32 = load ptr, ptr %3, align 8
  %33 = getelementptr inbounds nuw %struct.Vec, ptr %32, i32 0, i32 3
  %34 = load i32, ptr %33, align 8
  %35 = mul nsw i32 %31, %34
  %36 = sext i32 %35 to i64
  %37 = getelementptr inbounds i8, ptr %30, i64 %36
  %38 = load ptr, ptr %3, align 8
  %39 = getelementptr inbounds nuw %struct.Vec, ptr %38, i32 0, i32 0
  %40 = load ptr, ptr %39, align 8
  %41 = load i32, ptr %4, align 4
  %42 = add nsw i32 %41, 1
  %43 = load ptr, ptr %3, align 8
  %44 = getelementptr inbounds nuw %struct.Vec, ptr %43, i32 0, i32 3
  %45 = load i32, ptr %44, align 8
  %46 = mul nsw i32 %42, %45
  %47 = sext i32 %46 to i64
  %48 = getelementptr inbounds i8, ptr %40, i64 %47
  %49 = load ptr, ptr %3, align 8
  %50 = getelementptr inbounds nuw %struct.Vec, ptr %49, i32 0, i32 1
  %51 = load i32, ptr %50, align 8
  %52 = load i32, ptr %4, align 4
  %53 = sub nsw i32 %51, %52
  %54 = sub nsw i32 %53, 1
  %55 = load ptr, ptr %3, align 8
  %56 = getelementptr inbounds nuw %struct.Vec, ptr %55, i32 0, i32 3
  %57 = load i32, ptr %56, align 8
  %58 = mul nsw i32 %54, %57
  %59 = sext i32 %58 to i64
  call void @llvm.memmove.p0.p0.i64(ptr align 1 %37, ptr align 1 %48, i64 %59, i1 false)
  %60 = load ptr, ptr %3, align 8
  %61 = getelementptr inbounds nuw %struct.Vec, ptr %60, i32 0, i32 1
  %62 = load i32, ptr %61, align 8
  %63 = add nsw i32 %62, -1
  store i32 %63, ptr %61, align 8
  br label %64

64:                                               ; preds = %27, %24, %14
  ret void
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memmove.p0.p0.i64(ptr writeonly captures(none), ptr readonly captures(none), i64, i1 immarg) #4

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"erase_all$MODVec"(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  %3 = load ptr, ptr %2, align 8
  %4 = getelementptr inbounds nuw %struct.Vec, ptr %3, i32 0, i32 1
  store i32 0, ptr %4, align 8
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"set$MODVec"(ptr noundef %0, i32 noundef %1, ptr noundef %2) #0 {
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  %6 = alloca ptr, align 8
  store ptr %0, ptr %4, align 8
  store i32 %1, ptr %5, align 4
  store ptr %2, ptr %6, align 8
  %7 = load ptr, ptr %4, align 8
  %8 = getelementptr inbounds nuw %struct.Vec, ptr %7, i32 0, i32 0
  %9 = load ptr, ptr %8, align 8
  %10 = icmp ne ptr %9, null
  br i1 %10, label %11, label %16

11:                                               ; preds = %3
  %12 = load ptr, ptr %4, align 8
  %13 = getelementptr inbounds nuw %struct.Vec, ptr %12, i32 0, i32 4
  %14 = load i8, ptr %13, align 4
  %15 = trunc i8 %14 to i1
  br i1 %15, label %17, label %16

16:                                               ; preds = %11, %3
  br label %45

17:                                               ; preds = %11
  %18 = load i32, ptr %5, align 4
  %19 = icmp slt i32 %18, 0
  br i1 %19, label %26, label %20

20:                                               ; preds = %17
  %21 = load i32, ptr %5, align 4
  %22 = load ptr, ptr %4, align 8
  %23 = getelementptr inbounds nuw %struct.Vec, ptr %22, i32 0, i32 1
  %24 = load i32, ptr %23, align 8
  %25 = icmp sge i32 %21, %24
  br i1 %25, label %26, label %29

26:                                               ; preds = %20, %17
  %27 = load ptr, ptr @stderr, align 8
  %28 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %27, ptr noundef @.str.3) #6
  br label %45

29:                                               ; preds = %20
  %30 = load ptr, ptr %4, align 8
  %31 = getelementptr inbounds nuw %struct.Vec, ptr %30, i32 0, i32 0
  %32 = load ptr, ptr %31, align 8
  %33 = load i32, ptr %5, align 4
  %34 = load ptr, ptr %4, align 8
  %35 = getelementptr inbounds nuw %struct.Vec, ptr %34, i32 0, i32 3
  %36 = load i32, ptr %35, align 8
  %37 = mul nsw i32 %33, %36
  %38 = sext i32 %37 to i64
  %39 = getelementptr inbounds i8, ptr %32, i64 %38
  %40 = load ptr, ptr %6, align 8
  %41 = load ptr, ptr %4, align 8
  %42 = getelementptr inbounds nuw %struct.Vec, ptr %41, i32 0, i32 3
  %43 = load i32, ptr %42, align 8
  %44 = sext i32 %43 to i64
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %39, ptr align 1 %40, i64 %44, i1 false)
  br label %45

45:                                               ; preds = %29, %26, %16
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"push_ptr$MODVec"(ptr noundef %0, ptr noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
  store ptr %1, ptr %4, align 8
  %5 = load ptr, ptr %3, align 8
  call void @"push$MODVec"(ptr noundef %5, ptr noundef %4)
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"push_int$MODVec"(ptr noundef %0, i32 noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  store ptr %0, ptr %3, align 8
  store i32 %1, ptr %4, align 4
  %5 = load ptr, ptr %3, align 8
  call void @"push$MODVec"(ptr noundef %5, ptr noundef %4)
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"push_string$MODVec"(ptr noundef %0, ptr %1, i64 %2) #0 {
  %4 = alloca %struct.String, align 8
  %5 = alloca ptr, align 8
  %6 = alloca %struct.String, align 8
  %7 = getelementptr inbounds nuw { ptr, i64 }, ptr %4, i32 0, i32 0
  store ptr %1, ptr %7, align 8
  %8 = getelementptr inbounds nuw { ptr, i64 }, ptr %4, i32 0, i32 1
  store i64 %2, ptr %8, align 8
  store ptr %0, ptr %5, align 8
  %9 = call { ptr, i64 } @"clone$MODString"(ptr noundef %4)
  %10 = getelementptr inbounds nuw { ptr, i64 }, ptr %6, i32 0, i32 0
  %11 = extractvalue { ptr, i64 } %9, 0
  store ptr %11, ptr %10, align 8
  %12 = getelementptr inbounds nuw { ptr, i64 }, ptr %6, i32 0, i32 1
  %13 = extractvalue { ptr, i64 } %9, 1
  store i64 %13, ptr %12, align 8
  %14 = load ptr, ptr %5, align 8
  call void @"push$MODVec"(ptr noundef %14, ptr noundef %6)
  ret void
}

declare { ptr, i64 } @"clone$MODString"(ptr noundef) #2

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"push_double$MODVec"(ptr noundef %0, double noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca double, align 8
  store ptr %0, ptr %3, align 8
  store double %1, ptr %4, align 8
  %5 = load ptr, ptr %3, align 8
  call void @"push$MODVec"(ptr noundef %5, ptr noundef %4)
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"push_char$MODVec"(ptr noundef %0, i8 noundef signext %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca i8, align 1
  store ptr %0, ptr %3, align 8
  store i8 %1, ptr %4, align 1
  %5 = load ptr, ptr %3, align 8
  call void @"push$MODVec"(ptr noundef %5, ptr noundef %4)
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"push_vec$MODVec"(ptr noundef %0, ptr noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
  store ptr %1, ptr %4, align 8
  %5 = load ptr, ptr %3, align 8
  %6 = load ptr, ptr %4, align 8
  call void @"push_ptr$MODVec"(ptr noundef %5, ptr noundef %6)
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"pushm$MODVec"(ptr noundef %0, i32 noundef %1, ...) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  %5 = alloca [1 x %struct.__va_list_tag], align 16
  %6 = alloca i32, align 4
  %7 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
  store i32 %1, ptr %4, align 4
  %8 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  call void @llvm.va_start.p0(ptr %8)
  store i32 0, ptr %6, align 4
  br label %9

9:                                                ; preds = %32, %2
  %10 = load i32, ptr %6, align 4
  %11 = load i32, ptr %4, align 4
  %12 = icmp slt i32 %10, %11
  br i1 %12, label %13, label %35

13:                                               ; preds = %9
  %14 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  %15 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %14, i32 0, i32 0
  %16 = load i32, ptr %15, align 16
  %17 = icmp ule i32 %16, 40
  br i1 %17, label %18, label %23

18:                                               ; preds = %13
  %19 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %14, i32 0, i32 3
  %20 = load ptr, ptr %19, align 16
  %21 = getelementptr i8, ptr %20, i32 %16
  %22 = add i32 %16, 8
  store i32 %22, ptr %15, align 16
  br label %27

23:                                               ; preds = %13
  %24 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %14, i32 0, i32 2
  %25 = load ptr, ptr %24, align 8
  %26 = getelementptr i8, ptr %25, i32 8
  store ptr %26, ptr %24, align 8
  br label %27

27:                                               ; preds = %23, %18
  %28 = phi ptr [ %21, %18 ], [ %25, %23 ]
  %29 = load ptr, ptr %28, align 8
  store ptr %29, ptr %7, align 8
  %30 = load ptr, ptr %3, align 8
  %31 = load ptr, ptr %7, align 8
  call void @"push$MODVec"(ptr noundef %30, ptr noundef %31)
  br label %32

32:                                               ; preds = %27
  %33 = load i32, ptr %6, align 4
  %34 = add nsw i32 %33, 1
  store i32 %34, ptr %6, align 4
  br label %9, !llvm.loop !13

35:                                               ; preds = %9
  %36 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  call void @llvm.va_end.p0(ptr %36)
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"pushm_int$MODVec"(ptr noundef %0, i32 noundef %1, ...) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  %5 = alloca [1 x %struct.__va_list_tag], align 16
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  store ptr %0, ptr %3, align 8
  store i32 %1, ptr %4, align 4
  %8 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  call void @llvm.va_start.p0(ptr %8)
  store i32 0, ptr %6, align 4
  br label %9

9:                                                ; preds = %32, %2
  %10 = load i32, ptr %6, align 4
  %11 = load i32, ptr %4, align 4
  %12 = icmp slt i32 %10, %11
  br i1 %12, label %13, label %35

13:                                               ; preds = %9
  %14 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  %15 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %14, i32 0, i32 0
  %16 = load i32, ptr %15, align 16
  %17 = icmp ule i32 %16, 40
  br i1 %17, label %18, label %23

18:                                               ; preds = %13
  %19 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %14, i32 0, i32 3
  %20 = load ptr, ptr %19, align 16
  %21 = getelementptr i8, ptr %20, i32 %16
  %22 = add i32 %16, 8
  store i32 %22, ptr %15, align 16
  br label %27

23:                                               ; preds = %13
  %24 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %14, i32 0, i32 2
  %25 = load ptr, ptr %24, align 8
  %26 = getelementptr i8, ptr %25, i32 8
  store ptr %26, ptr %24, align 8
  br label %27

27:                                               ; preds = %23, %18
  %28 = phi ptr [ %21, %18 ], [ %25, %23 ]
  %29 = load i32, ptr %28, align 4
  store i32 %29, ptr %7, align 4
  %30 = load ptr, ptr %3, align 8
  %31 = load i32, ptr %7, align 4
  call void @"push_int$MODVec"(ptr noundef %30, i32 noundef %31)
  br label %32

32:                                               ; preds = %27
  %33 = load i32, ptr %6, align 4
  %34 = add nsw i32 %33, 1
  store i32 %34, ptr %6, align 4
  br label %9, !llvm.loop !14

35:                                               ; preds = %9
  %36 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  call void @llvm.va_end.p0(ptr %36)
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"pushm_string$MODVec"(ptr noundef %0, i32 noundef %1, ...) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  %5 = alloca [1 x %struct.__va_list_tag], align 16
  %6 = alloca i32, align 4
  %7 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
  store i32 %1, ptr %4, align 4
  %8 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  call void @llvm.va_start.p0(ptr %8)
  store i32 0, ptr %6, align 4
  br label %9

9:                                                ; preds = %36, %2
  %10 = load i32, ptr %6, align 4
  %11 = load i32, ptr %4, align 4
  %12 = icmp slt i32 %10, %11
  br i1 %12, label %13, label %39

13:                                               ; preds = %9
  %14 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  %15 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %14, i32 0, i32 0
  %16 = load i32, ptr %15, align 16
  %17 = icmp ule i32 %16, 40
  br i1 %17, label %18, label %23

18:                                               ; preds = %13
  %19 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %14, i32 0, i32 3
  %20 = load ptr, ptr %19, align 16
  %21 = getelementptr i8, ptr %20, i32 %16
  %22 = add i32 %16, 8
  store i32 %22, ptr %15, align 16
  br label %27

23:                                               ; preds = %13
  %24 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %14, i32 0, i32 2
  %25 = load ptr, ptr %24, align 8
  %26 = getelementptr i8, ptr %25, i32 8
  store ptr %26, ptr %24, align 8
  br label %27

27:                                               ; preds = %23, %18
  %28 = phi ptr [ %21, %18 ], [ %25, %23 ]
  %29 = load ptr, ptr %28, align 8
  store ptr %29, ptr %7, align 8
  %30 = load ptr, ptr %3, align 8
  %31 = load ptr, ptr %7, align 8
  %32 = getelementptr inbounds nuw { ptr, i64 }, ptr %31, i32 0, i32 0
  %33 = load ptr, ptr %32, align 8
  %34 = getelementptr inbounds nuw { ptr, i64 }, ptr %31, i32 0, i32 1
  %35 = load i64, ptr %34, align 8
  call void @"push_string$MODVec"(ptr noundef %30, ptr %33, i64 %35)
  br label %36

36:                                               ; preds = %27
  %37 = load i32, ptr %6, align 4
  %38 = add nsw i32 %37, 1
  store i32 %38, ptr %6, align 4
  br label %9, !llvm.loop !15

39:                                               ; preds = %9
  %40 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  call void @llvm.va_end.p0(ptr %40)
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"pushm_double$MODVec"(ptr noundef %0, i32 noundef %1, ...) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  %5 = alloca [1 x %struct.__va_list_tag], align 16
  %6 = alloca i32, align 4
  %7 = alloca double, align 8
  store ptr %0, ptr %3, align 8
  store i32 %1, ptr %4, align 4
  %8 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  call void @llvm.va_start.p0(ptr %8)
  store i32 0, ptr %6, align 4
  br label %9

9:                                                ; preds = %32, %2
  %10 = load i32, ptr %6, align 4
  %11 = load i32, ptr %4, align 4
  %12 = icmp slt i32 %10, %11
  br i1 %12, label %13, label %35

13:                                               ; preds = %9
  %14 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  %15 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %14, i32 0, i32 1
  %16 = load i32, ptr %15, align 4
  %17 = icmp ule i32 %16, 160
  br i1 %17, label %18, label %23

18:                                               ; preds = %13
  %19 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %14, i32 0, i32 3
  %20 = load ptr, ptr %19, align 16
  %21 = getelementptr i8, ptr %20, i32 %16
  %22 = add i32 %16, 16
  store i32 %22, ptr %15, align 4
  br label %27

23:                                               ; preds = %13
  %24 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %14, i32 0, i32 2
  %25 = load ptr, ptr %24, align 8
  %26 = getelementptr i8, ptr %25, i32 8
  store ptr %26, ptr %24, align 8
  br label %27

27:                                               ; preds = %23, %18
  %28 = phi ptr [ %21, %18 ], [ %25, %23 ]
  %29 = load double, ptr %28, align 8
  store double %29, ptr %7, align 8
  %30 = load ptr, ptr %3, align 8
  %31 = load double, ptr %7, align 8
  call void @"push_double$MODVec"(ptr noundef %30, double noundef %31)
  br label %32

32:                                               ; preds = %27
  %33 = load i32, ptr %6, align 4
  %34 = add nsw i32 %33, 1
  store i32 %34, ptr %6, align 4
  br label %9, !llvm.loop !16

35:                                               ; preds = %9
  %36 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  call void @llvm.va_end.p0(ptr %36)
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"pushm_vec$MODVec"(ptr noundef %0, i32 noundef %1, ...) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  %5 = alloca [1 x %struct.__va_list_tag], align 16
  %6 = alloca i32, align 4
  %7 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
  store i32 %1, ptr %4, align 4
  %8 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  call void @llvm.va_start.p0(ptr %8)
  store i32 0, ptr %6, align 4
  br label %9

9:                                                ; preds = %32, %2
  %10 = load i32, ptr %6, align 4
  %11 = load i32, ptr %4, align 4
  %12 = icmp slt i32 %10, %11
  br i1 %12, label %13, label %35

13:                                               ; preds = %9
  %14 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  %15 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %14, i32 0, i32 0
  %16 = load i32, ptr %15, align 16
  %17 = icmp ule i32 %16, 40
  br i1 %17, label %18, label %23

18:                                               ; preds = %13
  %19 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %14, i32 0, i32 3
  %20 = load ptr, ptr %19, align 16
  %21 = getelementptr i8, ptr %20, i32 %16
  %22 = add i32 %16, 8
  store i32 %22, ptr %15, align 16
  br label %27

23:                                               ; preds = %13
  %24 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %14, i32 0, i32 2
  %25 = load ptr, ptr %24, align 8
  %26 = getelementptr i8, ptr %25, i32 8
  store ptr %26, ptr %24, align 8
  br label %27

27:                                               ; preds = %23, %18
  %28 = phi ptr [ %21, %18 ], [ %25, %23 ]
  %29 = load ptr, ptr %28, align 8
  store ptr %29, ptr %7, align 8
  %30 = load ptr, ptr %3, align 8
  %31 = load ptr, ptr %7, align 8
  call void @"push$MODVec"(ptr noundef %30, ptr noundef %31)
  br label %32

32:                                               ; preds = %27
  %33 = load i32, ptr %6, align 4
  %34 = add nsw i32 %33, 1
  store i32 %34, ptr %6, align 4
  br label %9, !llvm.loop !17

35:                                               ; preds = %9
  %36 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  call void @llvm.va_end.p0(ptr %36)
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"pushm_char$MODVec"(ptr noundef %0, i32 noundef %1, ...) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  %5 = alloca [1 x %struct.__va_list_tag], align 16
  %6 = alloca i32, align 4
  %7 = alloca i8, align 1
  store ptr %0, ptr %3, align 8
  store i32 %1, ptr %4, align 4
  %8 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  call void @llvm.va_start.p0(ptr %8)
  store i32 0, ptr %6, align 4
  br label %9

9:                                                ; preds = %33, %2
  %10 = load i32, ptr %6, align 4
  %11 = load i32, ptr %4, align 4
  %12 = icmp slt i32 %10, %11
  br i1 %12, label %13, label %36

13:                                               ; preds = %9
  %14 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  %15 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %14, i32 0, i32 0
  %16 = load i32, ptr %15, align 16
  %17 = icmp ule i32 %16, 40
  br i1 %17, label %18, label %23

18:                                               ; preds = %13
  %19 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %14, i32 0, i32 3
  %20 = load ptr, ptr %19, align 16
  %21 = getelementptr i8, ptr %20, i32 %16
  %22 = add i32 %16, 8
  store i32 %22, ptr %15, align 16
  br label %27

23:                                               ; preds = %13
  %24 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %14, i32 0, i32 2
  %25 = load ptr, ptr %24, align 8
  %26 = getelementptr i8, ptr %25, i32 8
  store ptr %26, ptr %24, align 8
  br label %27

27:                                               ; preds = %23, %18
  %28 = phi ptr [ %21, %18 ], [ %25, %23 ]
  %29 = load i32, ptr %28, align 4
  %30 = trunc i32 %29 to i8
  store i8 %30, ptr %7, align 1
  %31 = load ptr, ptr %3, align 8
  %32 = load i8, ptr %7, align 1
  call void @"push_char$MODVec"(ptr noundef %31, i8 noundef signext %32)
  br label %33

33:                                               ; preds = %27
  %34 = load i32, ptr %6, align 4
  %35 = add nsw i32 %34, 1
  store i32 %35, ptr %6, align 4
  br label %9, !llvm.loop !18

36:                                               ; preds = %9
  %37 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  call void @llvm.va_end.p0(ptr %37)
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local ptr @"get$MODVec"(ptr noundef %0, i32 noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  store ptr %0, ptr %4, align 8
  store i32 %1, ptr %5, align 4
  %6 = load ptr, ptr %4, align 8
  %7 = getelementptr inbounds nuw %struct.Vec, ptr %6, i32 0, i32 0
  %8 = load ptr, ptr %7, align 8
  %9 = icmp ne ptr %8, null
  br i1 %9, label %11, label %10

10:                                               ; preds = %2
  store ptr null, ptr %3, align 8
  br label %34

11:                                               ; preds = %2
  %12 = load i32, ptr %5, align 4
  %13 = icmp slt i32 %12, 0
  br i1 %13, label %20, label %14

14:                                               ; preds = %11
  %15 = load i32, ptr %5, align 4
  %16 = load ptr, ptr %4, align 8
  %17 = getelementptr inbounds nuw %struct.Vec, ptr %16, i32 0, i32 1
  %18 = load i32, ptr %17, align 8
  %19 = icmp sge i32 %15, %18
  br i1 %19, label %20, label %23

20:                                               ; preds = %14, %11
  %21 = load ptr, ptr @stderr, align 8
  %22 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %21, ptr noundef @.str.4) #6
  store ptr null, ptr %3, align 8
  br label %34

23:                                               ; preds = %14
  %24 = load ptr, ptr %4, align 8
  %25 = getelementptr inbounds nuw %struct.Vec, ptr %24, i32 0, i32 0
  %26 = load ptr, ptr %25, align 8
  %27 = load i32, ptr %5, align 4
  %28 = load ptr, ptr %4, align 8
  %29 = getelementptr inbounds nuw %struct.Vec, ptr %28, i32 0, i32 3
  %30 = load i32, ptr %29, align 8
  %31 = mul nsw i32 %27, %30
  %32 = sext i32 %31 to i64
  %33 = getelementptr inbounds i8, ptr %26, i64 %32
  store ptr %33, ptr %3, align 8
  br label %34

34:                                               ; preds = %23, %20, %10
  %35 = load ptr, ptr %3, align 8
  ret ptr %35
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local ptr @"get_ptr$MODVec"(ptr noundef %0, i32 noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  %5 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
  store i32 %1, ptr %4, align 4
  %6 = load ptr, ptr %3, align 8
  %7 = load i32, ptr %4, align 4
  %8 = call ptr @"get$MODVec"(ptr noundef %6, i32 noundef %7)
  store ptr %8, ptr %5, align 8
  %9 = load ptr, ptr %5, align 8
  %10 = icmp ne ptr %9, null
  br i1 %10, label %11, label %14

11:                                               ; preds = %2
  %12 = load ptr, ptr %5, align 8
  %13 = load ptr, ptr %12, align 8
  br label %15

14:                                               ; preds = %2
  br label %15

15:                                               ; preds = %14, %11
  %16 = phi ptr [ %13, %11 ], [ null, %14 ]
  ret ptr %16
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @"get_int$MODVec"(ptr noundef %0, i32 noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  %5 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
  store i32 %1, ptr %4, align 4
  %6 = load ptr, ptr %3, align 8
  %7 = load i32, ptr %4, align 4
  %8 = call ptr @"get$MODVec"(ptr noundef %6, i32 noundef %7)
  store ptr %8, ptr %5, align 8
  %9 = load ptr, ptr %5, align 8
  %10 = icmp ne ptr %9, null
  br i1 %10, label %11, label %14

11:                                               ; preds = %2
  %12 = load ptr, ptr %5, align 8
  %13 = load i32, ptr %12, align 4
  br label %15

14:                                               ; preds = %2
  br label %15

15:                                               ; preds = %14, %11
  %16 = phi i32 [ %13, %11 ], [ 0, %14 ]
  ret i32 %16
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local { ptr, i64 } @"get_string$MODVec"(ptr noundef %0, i32 noundef %1) #0 {
  %3 = alloca %struct.String, align 8
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  store ptr %0, ptr %4, align 8
  store i32 %1, ptr %5, align 4
  %6 = load ptr, ptr %4, align 8
  %7 = load i32, ptr %5, align 4
  %8 = call ptr @"get$MODVec"(ptr noundef %6, i32 noundef %7)
  call void @llvm.memcpy.p0.p0.i64(ptr align 8 %3, ptr align 8 %8, i64 16, i1 false)
  %9 = load { ptr, i64 }, ptr %3, align 8
  ret { ptr, i64 } %9
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local double @"get_double$MODVec"(ptr noundef %0, i32 noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  %5 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
  store i32 %1, ptr %4, align 4
  %6 = load ptr, ptr %3, align 8
  %7 = load i32, ptr %4, align 4
  %8 = call ptr @"get$MODVec"(ptr noundef %6, i32 noundef %7)
  store ptr %8, ptr %5, align 8
  %9 = load ptr, ptr %5, align 8
  %10 = icmp ne ptr %9, null
  br i1 %10, label %11, label %14

11:                                               ; preds = %2
  %12 = load ptr, ptr %5, align 8
  %13 = load double, ptr %12, align 8
  br label %15

14:                                               ; preds = %2
  br label %15

15:                                               ; preds = %14, %11
  %16 = phi double [ %13, %11 ], [ 0.000000e+00, %14 ]
  ret double %16
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"get_vec$MODVec"(ptr dead_on_unwind noalias writable sret(%struct.Vec) align 8 %0, ptr noundef %1, i32 noundef %2) #0 {
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  %6 = alloca ptr, align 8
  store ptr %1, ptr %4, align 8
  store i32 %2, ptr %5, align 4
  %7 = load ptr, ptr %4, align 8
  %8 = load i32, ptr %5, align 4
  %9 = call ptr @"get$MODVec"(ptr noundef %7, i32 noundef %8)
  store ptr %9, ptr %6, align 8
  %10 = load ptr, ptr %6, align 8
  %11 = icmp ne ptr %10, null
  br i1 %11, label %12, label %14

12:                                               ; preds = %3
  %13 = load ptr, ptr %6, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr align 8 %0, ptr align 8 %13, i64 24, i1 false)
  br label %20

14:                                               ; preds = %3
  call void @llvm.memset.p0.i64(ptr align 8 %0, i8 0, i64 24, i1 false)
  %15 = getelementptr inbounds nuw %struct.Vec, ptr %0, i32 0, i32 0
  %16 = getelementptr inbounds nuw %struct.Vec, ptr %0, i32 0, i32 1
  %17 = getelementptr inbounds nuw %struct.Vec, ptr %0, i32 0, i32 2
  %18 = getelementptr inbounds nuw %struct.Vec, ptr %0, i32 0, i32 3
  %19 = getelementptr inbounds nuw %struct.Vec, ptr %0, i32 0, i32 4
  br label %20

20:                                               ; preds = %14, %12
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local signext i8 @"get_char$MODVec"(ptr noundef %0, i32 noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  %5 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
  store i32 %1, ptr %4, align 4
  %6 = load ptr, ptr %3, align 8
  %7 = load i32, ptr %4, align 4
  %8 = call ptr @"get$MODVec"(ptr noundef %6, i32 noundef %7)
  store ptr %8, ptr %5, align 8
  %9 = load ptr, ptr %5, align 8
  %10 = icmp ne ptr %9, null
  br i1 %10, label %11, label %15

11:                                               ; preds = %2
  %12 = load ptr, ptr %5, align 8
  %13 = load i8, ptr %12, align 1
  %14 = sext i8 %13 to i32
  br label %16

15:                                               ; preds = %2
  br label %16

16:                                               ; preds = %15, %11
  %17 = phi i32 [ %14, %11 ], [ 0, %15 ]
  %18 = trunc i32 %17 to i8
  ret i8 %18
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"set_int$MODVec"(ptr noundef %0, i32 noundef %1, i32 noundef %2) #0 {
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  store ptr %0, ptr %4, align 8
  store i32 %1, ptr %5, align 4
  store i32 %2, ptr %6, align 4
  %7 = load ptr, ptr %4, align 8
  %8 = load i32, ptr %5, align 4
  call void @"set$MODVec"(ptr noundef %7, i32 noundef %8, ptr noundef %6)
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"set_string$MODVec"(ptr noundef %0, i32 noundef %1, ptr noundef %2) #0 {
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  %6 = alloca ptr, align 8
  store ptr %0, ptr %4, align 8
  store i32 %1, ptr %5, align 4
  store ptr %2, ptr %6, align 8
  %7 = load ptr, ptr %4, align 8
  %8 = load i32, ptr %5, align 4
  call void @"set$MODVec"(ptr noundef %7, i32 noundef %8, ptr noundef %6)
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"set_double$MODVec"(ptr noundef %0, i32 noundef %1, double noundef %2) #0 {
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  %6 = alloca double, align 8
  store ptr %0, ptr %4, align 8
  store i32 %1, ptr %5, align 4
  store double %2, ptr %6, align 8
  %7 = load ptr, ptr %4, align 8
  %8 = load i32, ptr %5, align 4
  call void @"set$MODVec"(ptr noundef %7, i32 noundef %8, ptr noundef %6)
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"set_char$MODVec"(ptr noundef %0, i32 noundef %1, i8 noundef signext %2) #0 {
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  %6 = alloca i8, align 1
  store ptr %0, ptr %4, align 8
  store i32 %1, ptr %5, align 4
  store i8 %2, ptr %6, align 1
  %7 = load ptr, ptr %4, align 8
  %8 = load i32, ptr %5, align 4
  call void @"set$MODVec"(ptr noundef %7, i32 noundef %8, ptr noundef %6)
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"set_vec$MODVec"(ptr noundef %0, i32 noundef %1, ptr noundef %2) #0 {
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  %6 = alloca ptr, align 8
  store ptr %0, ptr %4, align 8
  store i32 %1, ptr %5, align 4
  store ptr %2, ptr %6, align 8
  %7 = load ptr, ptr %4, align 8
  %8 = load i32, ptr %5, align 4
  call void @"set$MODVec"(ptr noundef %7, i32 noundef %8, ptr noundef %6)
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"setm_int$MODVec"(ptr noundef %0, i32 noundef %1, ...) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  %5 = alloca [1 x %struct.__va_list_tag], align 16
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  store ptr %0, ptr %3, align 8
  store i32 %1, ptr %4, align 4
  %9 = load ptr, ptr %3, align 8
  %10 = getelementptr inbounds nuw %struct.Vec, ptr %9, i32 0, i32 0
  %11 = load ptr, ptr %10, align 8
  %12 = icmp ne ptr %11, null
  br i1 %12, label %13, label %18

13:                                               ; preds = %2
  %14 = load ptr, ptr %3, align 8
  %15 = getelementptr inbounds nuw %struct.Vec, ptr %14, i32 0, i32 4
  %16 = load i8, ptr %15, align 4
  %17 = trunc i8 %16 to i1
  br i1 %17, label %19, label %18

18:                                               ; preds = %13, %2
  br label %78

19:                                               ; preds = %13
  %20 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  call void @llvm.va_start.p0(ptr %20)
  store i32 0, ptr %6, align 4
  br label %21

21:                                               ; preds = %73, %19
  %22 = load i32, ptr %6, align 4
  %23 = load i32, ptr %4, align 4
  %24 = icmp slt i32 %22, %23
  br i1 %24, label %25, label %76

25:                                               ; preds = %21
  %26 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  %27 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %26, i32 0, i32 0
  %28 = load i32, ptr %27, align 16
  %29 = icmp ule i32 %28, 40
  br i1 %29, label %30, label %35

30:                                               ; preds = %25
  %31 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %26, i32 0, i32 3
  %32 = load ptr, ptr %31, align 16
  %33 = getelementptr i8, ptr %32, i32 %28
  %34 = add i32 %28, 8
  store i32 %34, ptr %27, align 16
  br label %39

35:                                               ; preds = %25
  %36 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %26, i32 0, i32 2
  %37 = load ptr, ptr %36, align 8
  %38 = getelementptr i8, ptr %37, i32 8
  store ptr %38, ptr %36, align 8
  br label %39

39:                                               ; preds = %35, %30
  %40 = phi ptr [ %33, %30 ], [ %37, %35 ]
  %41 = load i32, ptr %40, align 4
  store i32 %41, ptr %7, align 4
  %42 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  %43 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %42, i32 0, i32 0
  %44 = load i32, ptr %43, align 16
  %45 = icmp ule i32 %44, 40
  br i1 %45, label %46, label %51

46:                                               ; preds = %39
  %47 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %42, i32 0, i32 3
  %48 = load ptr, ptr %47, align 16
  %49 = getelementptr i8, ptr %48, i32 %44
  %50 = add i32 %44, 8
  store i32 %50, ptr %43, align 16
  br label %55

51:                                               ; preds = %39
  %52 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %42, i32 0, i32 2
  %53 = load ptr, ptr %52, align 8
  %54 = getelementptr i8, ptr %53, i32 8
  store ptr %54, ptr %52, align 8
  br label %55

55:                                               ; preds = %51, %46
  %56 = phi ptr [ %49, %46 ], [ %53, %51 ]
  %57 = load i32, ptr %56, align 4
  store i32 %57, ptr %8, align 4
  %58 = load i32, ptr %7, align 4
  %59 = icmp slt i32 %58, 0
  br i1 %59, label %66, label %60

60:                                               ; preds = %55
  %61 = load i32, ptr %7, align 4
  %62 = load ptr, ptr %3, align 8
  %63 = getelementptr inbounds nuw %struct.Vec, ptr %62, i32 0, i32 1
  %64 = load i32, ptr %63, align 8
  %65 = icmp sge i32 %61, %64
  br i1 %65, label %66, label %69

66:                                               ; preds = %60, %55
  %67 = load ptr, ptr @stderr, align 8
  %68 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %67, ptr noundef @.str.5) #6
  br label %73

69:                                               ; preds = %60
  %70 = load ptr, ptr %3, align 8
  %71 = load i32, ptr %7, align 4
  %72 = load i32, ptr %8, align 4
  call void @"set_int$MODVec"(ptr noundef %70, i32 noundef %71, i32 noundef %72)
  br label %73

73:                                               ; preds = %69, %66
  %74 = load i32, ptr %6, align 4
  %75 = add nsw i32 %74, 1
  store i32 %75, ptr %6, align 4
  br label %21, !llvm.loop !19

76:                                               ; preds = %21
  %77 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  call void @llvm.va_end.p0(ptr %77)
  br label %78

78:                                               ; preds = %76, %18
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"setm_string$MODVec"(ptr noundef %0, i32 noundef %1, ...) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  %5 = alloca [1 x %struct.__va_list_tag], align 16
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
  store i32 %1, ptr %4, align 4
  %9 = load ptr, ptr %3, align 8
  %10 = getelementptr inbounds nuw %struct.Vec, ptr %9, i32 0, i32 0
  %11 = load ptr, ptr %10, align 8
  %12 = icmp ne ptr %11, null
  br i1 %12, label %13, label %18

13:                                               ; preds = %2
  %14 = load ptr, ptr %3, align 8
  %15 = getelementptr inbounds nuw %struct.Vec, ptr %14, i32 0, i32 4
  %16 = load i8, ptr %15, align 4
  %17 = trunc i8 %16 to i1
  br i1 %17, label %19, label %18

18:                                               ; preds = %13, %2
  br label %78

19:                                               ; preds = %13
  %20 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  call void @llvm.va_start.p0(ptr %20)
  store i32 0, ptr %6, align 4
  br label %21

21:                                               ; preds = %73, %19
  %22 = load i32, ptr %6, align 4
  %23 = load i32, ptr %4, align 4
  %24 = icmp slt i32 %22, %23
  br i1 %24, label %25, label %76

25:                                               ; preds = %21
  %26 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  %27 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %26, i32 0, i32 0
  %28 = load i32, ptr %27, align 16
  %29 = icmp ule i32 %28, 40
  br i1 %29, label %30, label %35

30:                                               ; preds = %25
  %31 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %26, i32 0, i32 3
  %32 = load ptr, ptr %31, align 16
  %33 = getelementptr i8, ptr %32, i32 %28
  %34 = add i32 %28, 8
  store i32 %34, ptr %27, align 16
  br label %39

35:                                               ; preds = %25
  %36 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %26, i32 0, i32 2
  %37 = load ptr, ptr %36, align 8
  %38 = getelementptr i8, ptr %37, i32 8
  store ptr %38, ptr %36, align 8
  br label %39

39:                                               ; preds = %35, %30
  %40 = phi ptr [ %33, %30 ], [ %37, %35 ]
  %41 = load i32, ptr %40, align 4
  store i32 %41, ptr %7, align 4
  %42 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  %43 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %42, i32 0, i32 0
  %44 = load i32, ptr %43, align 16
  %45 = icmp ule i32 %44, 40
  br i1 %45, label %46, label %51

46:                                               ; preds = %39
  %47 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %42, i32 0, i32 3
  %48 = load ptr, ptr %47, align 16
  %49 = getelementptr i8, ptr %48, i32 %44
  %50 = add i32 %44, 8
  store i32 %50, ptr %43, align 16
  br label %55

51:                                               ; preds = %39
  %52 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %42, i32 0, i32 2
  %53 = load ptr, ptr %52, align 8
  %54 = getelementptr i8, ptr %53, i32 8
  store ptr %54, ptr %52, align 8
  br label %55

55:                                               ; preds = %51, %46
  %56 = phi ptr [ %49, %46 ], [ %53, %51 ]
  %57 = load ptr, ptr %56, align 8
  store ptr %57, ptr %8, align 8
  %58 = load i32, ptr %7, align 4
  %59 = icmp slt i32 %58, 0
  br i1 %59, label %66, label %60

60:                                               ; preds = %55
  %61 = load i32, ptr %7, align 4
  %62 = load ptr, ptr %3, align 8
  %63 = getelementptr inbounds nuw %struct.Vec, ptr %62, i32 0, i32 1
  %64 = load i32, ptr %63, align 8
  %65 = icmp sge i32 %61, %64
  br i1 %65, label %66, label %69

66:                                               ; preds = %60, %55
  %67 = load ptr, ptr @stderr, align 8
  %68 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %67, ptr noundef @.str.6) #6
  br label %73

69:                                               ; preds = %60
  %70 = load ptr, ptr %3, align 8
  %71 = load i32, ptr %7, align 4
  %72 = load ptr, ptr %8, align 8
  call void @"set_string$MODVec"(ptr noundef %70, i32 noundef %71, ptr noundef %72)
  br label %73

73:                                               ; preds = %69, %66
  %74 = load i32, ptr %6, align 4
  %75 = add nsw i32 %74, 1
  store i32 %75, ptr %6, align 4
  br label %21, !llvm.loop !20

76:                                               ; preds = %21
  %77 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  call void @llvm.va_end.p0(ptr %77)
  br label %78

78:                                               ; preds = %76, %18
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"setm_double$MODVec"(ptr noundef %0, i32 noundef %1, ...) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  %5 = alloca [1 x %struct.__va_list_tag], align 16
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca double, align 8
  store ptr %0, ptr %3, align 8
  store i32 %1, ptr %4, align 4
  %9 = load ptr, ptr %3, align 8
  %10 = getelementptr inbounds nuw %struct.Vec, ptr %9, i32 0, i32 0
  %11 = load ptr, ptr %10, align 8
  %12 = icmp ne ptr %11, null
  br i1 %12, label %13, label %18

13:                                               ; preds = %2
  %14 = load ptr, ptr %3, align 8
  %15 = getelementptr inbounds nuw %struct.Vec, ptr %14, i32 0, i32 4
  %16 = load i8, ptr %15, align 4
  %17 = trunc i8 %16 to i1
  br i1 %17, label %19, label %18

18:                                               ; preds = %13, %2
  br label %78

19:                                               ; preds = %13
  %20 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  call void @llvm.va_start.p0(ptr %20)
  store i32 0, ptr %6, align 4
  br label %21

21:                                               ; preds = %73, %19
  %22 = load i32, ptr %6, align 4
  %23 = load i32, ptr %4, align 4
  %24 = icmp slt i32 %22, %23
  br i1 %24, label %25, label %76

25:                                               ; preds = %21
  %26 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  %27 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %26, i32 0, i32 0
  %28 = load i32, ptr %27, align 16
  %29 = icmp ule i32 %28, 40
  br i1 %29, label %30, label %35

30:                                               ; preds = %25
  %31 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %26, i32 0, i32 3
  %32 = load ptr, ptr %31, align 16
  %33 = getelementptr i8, ptr %32, i32 %28
  %34 = add i32 %28, 8
  store i32 %34, ptr %27, align 16
  br label %39

35:                                               ; preds = %25
  %36 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %26, i32 0, i32 2
  %37 = load ptr, ptr %36, align 8
  %38 = getelementptr i8, ptr %37, i32 8
  store ptr %38, ptr %36, align 8
  br label %39

39:                                               ; preds = %35, %30
  %40 = phi ptr [ %33, %30 ], [ %37, %35 ]
  %41 = load i32, ptr %40, align 4
  store i32 %41, ptr %7, align 4
  %42 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  %43 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %42, i32 0, i32 1
  %44 = load i32, ptr %43, align 4
  %45 = icmp ule i32 %44, 160
  br i1 %45, label %46, label %51

46:                                               ; preds = %39
  %47 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %42, i32 0, i32 3
  %48 = load ptr, ptr %47, align 16
  %49 = getelementptr i8, ptr %48, i32 %44
  %50 = add i32 %44, 16
  store i32 %50, ptr %43, align 4
  br label %55

51:                                               ; preds = %39
  %52 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %42, i32 0, i32 2
  %53 = load ptr, ptr %52, align 8
  %54 = getelementptr i8, ptr %53, i32 8
  store ptr %54, ptr %52, align 8
  br label %55

55:                                               ; preds = %51, %46
  %56 = phi ptr [ %49, %46 ], [ %53, %51 ]
  %57 = load double, ptr %56, align 8
  store double %57, ptr %8, align 8
  %58 = load i32, ptr %7, align 4
  %59 = icmp slt i32 %58, 0
  br i1 %59, label %66, label %60

60:                                               ; preds = %55
  %61 = load i32, ptr %7, align 4
  %62 = load ptr, ptr %3, align 8
  %63 = getelementptr inbounds nuw %struct.Vec, ptr %62, i32 0, i32 1
  %64 = load i32, ptr %63, align 8
  %65 = icmp sge i32 %61, %64
  br i1 %65, label %66, label %69

66:                                               ; preds = %60, %55
  %67 = load ptr, ptr @stderr, align 8
  %68 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %67, ptr noundef @.str.7) #6
  br label %73

69:                                               ; preds = %60
  %70 = load ptr, ptr %3, align 8
  %71 = load i32, ptr %7, align 4
  %72 = load double, ptr %8, align 8
  call void @"set_double$MODVec"(ptr noundef %70, i32 noundef %71, double noundef %72)
  br label %73

73:                                               ; preds = %69, %66
  %74 = load i32, ptr %6, align 4
  %75 = add nsw i32 %74, 1
  store i32 %75, ptr %6, align 4
  br label %21, !llvm.loop !21

76:                                               ; preds = %21
  %77 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  call void @llvm.va_end.p0(ptr %77)
  br label %78

78:                                               ; preds = %76, %18
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"setm_char$MODVec"(ptr noundef %0, i32 noundef %1, ...) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  %5 = alloca [1 x %struct.__va_list_tag], align 16
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i8, align 1
  store ptr %0, ptr %3, align 8
  store i32 %1, ptr %4, align 4
  %9 = load ptr, ptr %3, align 8
  %10 = getelementptr inbounds nuw %struct.Vec, ptr %9, i32 0, i32 0
  %11 = load ptr, ptr %10, align 8
  %12 = icmp ne ptr %11, null
  br i1 %12, label %13, label %18

13:                                               ; preds = %2
  %14 = load ptr, ptr %3, align 8
  %15 = getelementptr inbounds nuw %struct.Vec, ptr %14, i32 0, i32 4
  %16 = load i8, ptr %15, align 4
  %17 = trunc i8 %16 to i1
  br i1 %17, label %19, label %18

18:                                               ; preds = %13, %2
  br label %79

19:                                               ; preds = %13
  %20 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  call void @llvm.va_start.p0(ptr %20)
  store i32 0, ptr %6, align 4
  br label %21

21:                                               ; preds = %74, %19
  %22 = load i32, ptr %6, align 4
  %23 = load i32, ptr %4, align 4
  %24 = icmp slt i32 %22, %23
  br i1 %24, label %25, label %77

25:                                               ; preds = %21
  %26 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  %27 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %26, i32 0, i32 0
  %28 = load i32, ptr %27, align 16
  %29 = icmp ule i32 %28, 40
  br i1 %29, label %30, label %35

30:                                               ; preds = %25
  %31 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %26, i32 0, i32 3
  %32 = load ptr, ptr %31, align 16
  %33 = getelementptr i8, ptr %32, i32 %28
  %34 = add i32 %28, 8
  store i32 %34, ptr %27, align 16
  br label %39

35:                                               ; preds = %25
  %36 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %26, i32 0, i32 2
  %37 = load ptr, ptr %36, align 8
  %38 = getelementptr i8, ptr %37, i32 8
  store ptr %38, ptr %36, align 8
  br label %39

39:                                               ; preds = %35, %30
  %40 = phi ptr [ %33, %30 ], [ %37, %35 ]
  %41 = load i32, ptr %40, align 4
  store i32 %41, ptr %7, align 4
  %42 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  %43 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %42, i32 0, i32 0
  %44 = load i32, ptr %43, align 16
  %45 = icmp ule i32 %44, 40
  br i1 %45, label %46, label %51

46:                                               ; preds = %39
  %47 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %42, i32 0, i32 3
  %48 = load ptr, ptr %47, align 16
  %49 = getelementptr i8, ptr %48, i32 %44
  %50 = add i32 %44, 8
  store i32 %50, ptr %43, align 16
  br label %55

51:                                               ; preds = %39
  %52 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %42, i32 0, i32 2
  %53 = load ptr, ptr %52, align 8
  %54 = getelementptr i8, ptr %53, i32 8
  store ptr %54, ptr %52, align 8
  br label %55

55:                                               ; preds = %51, %46
  %56 = phi ptr [ %49, %46 ], [ %53, %51 ]
  %57 = load i32, ptr %56, align 4
  %58 = trunc i32 %57 to i8
  store i8 %58, ptr %8, align 1
  %59 = load i32, ptr %7, align 4
  %60 = icmp slt i32 %59, 0
  br i1 %60, label %67, label %61

61:                                               ; preds = %55
  %62 = load i32, ptr %7, align 4
  %63 = load ptr, ptr %3, align 8
  %64 = getelementptr inbounds nuw %struct.Vec, ptr %63, i32 0, i32 1
  %65 = load i32, ptr %64, align 8
  %66 = icmp sge i32 %62, %65
  br i1 %66, label %67, label %70

67:                                               ; preds = %61, %55
  %68 = load ptr, ptr @stderr, align 8
  %69 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %68, ptr noundef @.str.7) #6
  br label %74

70:                                               ; preds = %61
  %71 = load ptr, ptr %3, align 8
  %72 = load i32, ptr %7, align 4
  %73 = load i8, ptr %8, align 1
  call void @"set_char$MODVec"(ptr noundef %71, i32 noundef %72, i8 noundef signext %73)
  br label %74

74:                                               ; preds = %70, %67
  %75 = load i32, ptr %6, align 4
  %76 = add nsw i32 %75, 1
  store i32 %76, ptr %6, align 4
  br label %21, !llvm.loop !22

77:                                               ; preds = %21
  %78 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  call void @llvm.va_end.p0(ptr %78)
  br label %79

79:                                               ; preds = %77, %18
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"setm_vec$MODVec"(ptr noundef %0, i32 noundef %1, ...) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  %5 = alloca [1 x %struct.__va_list_tag], align 16
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
  store i32 %1, ptr %4, align 4
  %9 = load ptr, ptr %3, align 8
  %10 = getelementptr inbounds nuw %struct.Vec, ptr %9, i32 0, i32 0
  %11 = load ptr, ptr %10, align 8
  %12 = icmp ne ptr %11, null
  br i1 %12, label %13, label %18

13:                                               ; preds = %2
  %14 = load ptr, ptr %3, align 8
  %15 = getelementptr inbounds nuw %struct.Vec, ptr %14, i32 0, i32 4
  %16 = load i8, ptr %15, align 4
  %17 = trunc i8 %16 to i1
  br i1 %17, label %19, label %18

18:                                               ; preds = %13, %2
  br label %78

19:                                               ; preds = %13
  %20 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  call void @llvm.va_start.p0(ptr %20)
  store i32 0, ptr %6, align 4
  br label %21

21:                                               ; preds = %73, %19
  %22 = load i32, ptr %6, align 4
  %23 = load i32, ptr %4, align 4
  %24 = icmp slt i32 %22, %23
  br i1 %24, label %25, label %76

25:                                               ; preds = %21
  %26 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  %27 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %26, i32 0, i32 0
  %28 = load i32, ptr %27, align 16
  %29 = icmp ule i32 %28, 40
  br i1 %29, label %30, label %35

30:                                               ; preds = %25
  %31 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %26, i32 0, i32 3
  %32 = load ptr, ptr %31, align 16
  %33 = getelementptr i8, ptr %32, i32 %28
  %34 = add i32 %28, 8
  store i32 %34, ptr %27, align 16
  br label %39

35:                                               ; preds = %25
  %36 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %26, i32 0, i32 2
  %37 = load ptr, ptr %36, align 8
  %38 = getelementptr i8, ptr %37, i32 8
  store ptr %38, ptr %36, align 8
  br label %39

39:                                               ; preds = %35, %30
  %40 = phi ptr [ %33, %30 ], [ %37, %35 ]
  %41 = load i32, ptr %40, align 4
  store i32 %41, ptr %7, align 4
  %42 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  %43 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %42, i32 0, i32 0
  %44 = load i32, ptr %43, align 16
  %45 = icmp ule i32 %44, 40
  br i1 %45, label %46, label %51

46:                                               ; preds = %39
  %47 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %42, i32 0, i32 3
  %48 = load ptr, ptr %47, align 16
  %49 = getelementptr i8, ptr %48, i32 %44
  %50 = add i32 %44, 8
  store i32 %50, ptr %43, align 16
  br label %55

51:                                               ; preds = %39
  %52 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %42, i32 0, i32 2
  %53 = load ptr, ptr %52, align 8
  %54 = getelementptr i8, ptr %53, i32 8
  store ptr %54, ptr %52, align 8
  br label %55

55:                                               ; preds = %51, %46
  %56 = phi ptr [ %49, %46 ], [ %53, %51 ]
  %57 = load ptr, ptr %56, align 8
  store ptr %57, ptr %8, align 8
  %58 = load i32, ptr %7, align 4
  %59 = icmp slt i32 %58, 0
  br i1 %59, label %66, label %60

60:                                               ; preds = %55
  %61 = load i32, ptr %7, align 4
  %62 = load ptr, ptr %3, align 8
  %63 = getelementptr inbounds nuw %struct.Vec, ptr %62, i32 0, i32 1
  %64 = load i32, ptr %63, align 8
  %65 = icmp sge i32 %61, %64
  br i1 %65, label %66, label %69

66:                                               ; preds = %60, %55
  %67 = load ptr, ptr @stderr, align 8
  %68 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %67, ptr noundef @.str.8) #6
  br label %73

69:                                               ; preds = %60
  %70 = load ptr, ptr %3, align 8
  %71 = load i32, ptr %7, align 4
  %72 = load ptr, ptr %8, align 8
  call void @"set_vec$MODVec"(ptr noundef %70, i32 noundef %71, ptr noundef %72)
  br label %73

73:                                               ; preds = %69, %66
  %74 = load i32, ptr %6, align 4
  %75 = add nsw i32 %74, 1
  store i32 %75, ptr %6, align 4
  br label %21, !llvm.loop !23

76:                                               ; preds = %21
  %77 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %5, i64 0, i64 0
  call void @llvm.va_end.p0(ptr %77)
  br label %78

78:                                               ; preds = %76, %18
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"destroy$MODVec"(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  %3 = load ptr, ptr %2, align 8
  %4 = getelementptr inbounds nuw %struct.Vec, ptr %3, i32 0, i32 0
  %5 = load ptr, ptr %4, align 8
  %6 = icmp ne ptr %5, null
  br i1 %6, label %7, label %12

7:                                                ; preds = %1
  %8 = load ptr, ptr %2, align 8
  %9 = getelementptr inbounds nuw %struct.Vec, ptr %8, i32 0, i32 4
  %10 = load i8, ptr %9, align 4
  %11 = trunc i8 %10 to i1
  br i1 %11, label %13, label %12

12:                                               ; preds = %7, %1
  br label %25

13:                                               ; preds = %7
  %14 = load ptr, ptr %2, align 8
  %15 = getelementptr inbounds nuw %struct.Vec, ptr %14, i32 0, i32 0
  %16 = load ptr, ptr %15, align 8
  call void @"free$MODmem"(ptr noundef %16)
  %17 = load ptr, ptr %2, align 8
  %18 = getelementptr inbounds nuw %struct.Vec, ptr %17, i32 0, i32 2
  store i32 0, ptr %18, align 4
  %19 = load ptr, ptr %2, align 8
  %20 = getelementptr inbounds nuw %struct.Vec, ptr %19, i32 0, i32 1
  store i32 0, ptr %20, align 8
  %21 = load ptr, ptr %2, align 8
  %22 = getelementptr inbounds nuw %struct.Vec, ptr %21, i32 0, i32 3
  store i32 0, ptr %22, align 8
  %23 = load ptr, ptr %2, align 8
  %24 = getelementptr inbounds nuw %struct.Vec, ptr %23, i32 0, i32 4
  store i8 0, ptr %24, align 4
  br label %25

25:                                               ; preds = %13, %12
  ret void
}

declare void @"free$MODmem"(ptr noundef) #2

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"destroym$MODVec"(i32 noundef %0, ...) #0 {
  %2 = alloca i32, align 4
  %3 = alloca [1 x %struct.__va_list_tag], align 16
  %4 = alloca i32, align 4
  %5 = alloca ptr, align 8
  store i32 %0, ptr %2, align 4
  %6 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %3, i64 0, i64 0
  call void @llvm.va_start.p0(ptr %6)
  store i32 0, ptr %4, align 4
  br label %7

7:                                                ; preds = %29, %1
  %8 = load i32, ptr %4, align 4
  %9 = load i32, ptr %2, align 4
  %10 = icmp slt i32 %8, %9
  br i1 %10, label %11, label %32

11:                                               ; preds = %7
  %12 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %3, i64 0, i64 0
  %13 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %12, i32 0, i32 0
  %14 = load i32, ptr %13, align 16
  %15 = icmp ule i32 %14, 40
  br i1 %15, label %16, label %21

16:                                               ; preds = %11
  %17 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %12, i32 0, i32 3
  %18 = load ptr, ptr %17, align 16
  %19 = getelementptr i8, ptr %18, i32 %14
  %20 = add i32 %14, 8
  store i32 %20, ptr %13, align 16
  br label %25

21:                                               ; preds = %11
  %22 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %12, i32 0, i32 2
  %23 = load ptr, ptr %22, align 8
  %24 = getelementptr i8, ptr %23, i32 8
  store ptr %24, ptr %22, align 8
  br label %25

25:                                               ; preds = %21, %16
  %26 = phi ptr [ %19, %16 ], [ %23, %21 ]
  %27 = load ptr, ptr %26, align 8
  store ptr %27, ptr %5, align 8
  %28 = load ptr, ptr %5, align 8
  call void @"destroy$MODVec"(ptr noundef %28)
  br label %29

29:                                               ; preds = %25
  %30 = load i32, ptr %4, align 4
  %31 = add nsw i32 %30, 1
  store i32 %31, ptr %4, align 4
  br label %7, !llvm.loop !24

32:                                               ; preds = %7
  %33 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %3, i64 0, i64 0
  call void @llvm.va_end.p0(ptr %33)
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"disable$MODVec"(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  %3 = load ptr, ptr %2, align 8
  %4 = getelementptr inbounds nuw %struct.Vec, ptr %3, i32 0, i32 4
  store i8 0, ptr %4, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"enable$MODVec"(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  %3 = load ptr, ptr %2, align 8
  %4 = getelementptr inbounds nuw %struct.Vec, ptr %3, i32 0, i32 0
  %5 = load ptr, ptr %4, align 8
  %6 = icmp ne ptr %5, null
  br i1 %6, label %8, label %7

7:                                                ; preds = %1
  br label %11

8:                                                ; preds = %1
  %9 = load ptr, ptr %2, align 8
  %10 = getelementptr inbounds nuw %struct.Vec, ptr %9, i32 0, i32 4
  store i8 1, ptr %10, align 4
  br label %11

11:                                               ; preds = %8, %7
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"cpy$MODVec"(ptr noundef %0, ptr noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
  store ptr %1, ptr %4, align 8
  %5 = load ptr, ptr %4, align 8
  %6 = getelementptr inbounds nuw %struct.Vec, ptr %5, i32 0, i32 0
  %7 = load ptr, ptr %6, align 8
  %8 = icmp ne ptr %7, null
  br i1 %8, label %29, label %9

9:                                                ; preds = %2
  %10 = load ptr, ptr %4, align 8
  %11 = getelementptr inbounds nuw %struct.Vec, ptr %10, i32 0, i32 4
  %12 = load i8, ptr %11, align 4
  %13 = trunc i8 %12 to i1
  br i1 %13, label %14, label %29

14:                                               ; preds = %9
  %15 = load ptr, ptr %4, align 8
  %16 = getelementptr inbounds nuw %struct.Vec, ptr %15, i32 0, i32 1
  %17 = load i32, ptr %16, align 8
  %18 = icmp sle i32 %17, 0
  br i1 %18, label %29, label %19

19:                                               ; preds = %14
  %20 = load ptr, ptr %4, align 8
  %21 = getelementptr inbounds nuw %struct.Vec, ptr %20, i32 0, i32 3
  %22 = load i32, ptr %21, align 8
  %23 = icmp sle i32 %22, 0
  br i1 %23, label %29, label %24

24:                                               ; preds = %19
  %25 = load ptr, ptr %4, align 8
  %26 = getelementptr inbounds nuw %struct.Vec, ptr %25, i32 0, i32 2
  %27 = load i32, ptr %26, align 4
  %28 = icmp eq i32 %27, 0
  br i1 %28, label %29, label %30

29:                                               ; preds = %24, %19, %14, %9, %2
  br label %70

30:                                               ; preds = %24
  %31 = load ptr, ptr %4, align 8
  %32 = getelementptr inbounds nuw %struct.Vec, ptr %31, i32 0, i32 1
  %33 = load i32, ptr %32, align 8
  %34 = add nsw i32 %33, 1
  %35 = call ptr @"alloc$MODmem"(i32 noundef %34)
  %36 = load ptr, ptr %3, align 8
  %37 = getelementptr inbounds nuw %struct.Vec, ptr %36, i32 0, i32 0
  store ptr %35, ptr %37, align 8
  %38 = load ptr, ptr %3, align 8
  %39 = getelementptr inbounds nuw %struct.Vec, ptr %38, i32 0, i32 0
  %40 = load ptr, ptr %39, align 8
  %41 = icmp ne ptr %40, null
  br i1 %41, label %45, label %42

42:                                               ; preds = %30
  %43 = load ptr, ptr @stderr, align 8
  %44 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %43, ptr noundef @.str.9) #6
  br label %70

45:                                               ; preds = %30
  %46 = load ptr, ptr %3, align 8
  %47 = getelementptr inbounds nuw %struct.Vec, ptr %46, i32 0, i32 0
  %48 = load ptr, ptr %47, align 8
  %49 = load ptr, ptr %4, align 8
  %50 = getelementptr inbounds nuw %struct.Vec, ptr %49, i32 0, i32 0
  %51 = load ptr, ptr %50, align 8
  %52 = call ptr @strcpy(ptr noundef %48, ptr noundef %51) #6
  %53 = load ptr, ptr %4, align 8
  %54 = getelementptr inbounds nuw %struct.Vec, ptr %53, i32 0, i32 1
  %55 = load i32, ptr %54, align 8
  %56 = load ptr, ptr %3, align 8
  %57 = getelementptr inbounds nuw %struct.Vec, ptr %56, i32 0, i32 1
  store i32 %55, ptr %57, align 8
  %58 = load ptr, ptr %4, align 8
  %59 = getelementptr inbounds nuw %struct.Vec, ptr %58, i32 0, i32 2
  %60 = load i32, ptr %59, align 4
  %61 = load ptr, ptr %3, align 8
  %62 = getelementptr inbounds nuw %struct.Vec, ptr %61, i32 0, i32 2
  store i32 %60, ptr %62, align 4
  %63 = load ptr, ptr %3, align 8
  %64 = getelementptr inbounds nuw %struct.Vec, ptr %63, i32 0, i32 3
  %65 = load i32, ptr %64, align 8
  %66 = load ptr, ptr %3, align 8
  %67 = getelementptr inbounds nuw %struct.Vec, ptr %66, i32 0, i32 3
  store i32 %65, ptr %67, align 8
  %68 = load ptr, ptr %3, align 8
  %69 = getelementptr inbounds nuw %struct.Vec, ptr %68, i32 0, i32 4
  store i8 1, ptr %69, align 4
  br label %70

70:                                               ; preds = %45, %42, %29
  ret void
}

; Function Attrs: nounwind
declare ptr @strcpy(ptr noundef, ptr noundef) #3

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"cpy_and_push$MODVec"(ptr noundef %0, ptr noundef %1, i32 noundef %2) #0 {
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca i32, align 4
  %7 = alloca ptr, align 8
  store ptr %0, ptr %4, align 8
  store ptr %1, ptr %5, align 8
  store i32 %2, ptr %6, align 4
  %8 = load ptr, ptr %4, align 8
  %9 = getelementptr inbounds nuw %struct.Vec, ptr %8, i32 0, i32 3
  %10 = load i32, ptr %9, align 8
  %11 = load ptr, ptr %5, align 8
  %12 = getelementptr inbounds nuw %struct.Vec, ptr %11, i32 0, i32 3
  %13 = load i32, ptr %12, align 8
  %14 = icmp ne i32 %10, %13
  br i1 %14, label %15, label %18

15:                                               ; preds = %3
  %16 = load ptr, ptr @stderr, align 8
  %17 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %16, ptr noundef @.str.10) #6
  br label %18

18:                                               ; preds = %15, %3
  %19 = load ptr, ptr %5, align 8
  %20 = getelementptr inbounds nuw %struct.Vec, ptr %19, i32 0, i32 0
  %21 = load ptr, ptr %20, align 8
  %22 = icmp ne ptr %21, null
  br i1 %22, label %23, label %38

23:                                               ; preds = %18
  %24 = load ptr, ptr %4, align 8
  %25 = getelementptr inbounds nuw %struct.Vec, ptr %24, i32 0, i32 0
  %26 = load ptr, ptr %25, align 8
  %27 = icmp ne ptr %26, null
  br i1 %27, label %28, label %38

28:                                               ; preds = %23
  %29 = load ptr, ptr %5, align 8
  %30 = getelementptr inbounds nuw %struct.Vec, ptr %29, i32 0, i32 4
  %31 = load i8, ptr %30, align 4
  %32 = trunc i8 %31 to i1
  br i1 %32, label %33, label %38

33:                                               ; preds = %28
  %34 = load ptr, ptr %4, align 8
  %35 = getelementptr inbounds nuw %struct.Vec, ptr %34, i32 0, i32 4
  %36 = load i8, ptr %35, align 4
  %37 = trunc i8 %36 to i1
  br i1 %37, label %39, label %38

38:                                               ; preds = %33, %28, %23, %18
  br label %57

39:                                               ; preds = %33
  %40 = load i32, ptr %6, align 4
  %41 = icmp slt i32 %40, 0
  br i1 %41, label %48, label %42

42:                                               ; preds = %39
  %43 = load i32, ptr %6, align 4
  %44 = load ptr, ptr %5, align 8
  %45 = getelementptr inbounds nuw %struct.Vec, ptr %44, i32 0, i32 1
  %46 = load i32, ptr %45, align 8
  %47 = icmp sge i32 %43, %46
  br i1 %47, label %48, label %51

48:                                               ; preds = %42, %39
  %49 = load ptr, ptr @stderr, align 8
  %50 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %49, ptr noundef @.str.11) #6
  br label %57

51:                                               ; preds = %42
  %52 = load ptr, ptr %5, align 8
  %53 = load i32, ptr %6, align 4
  %54 = call ptr @"get$MODVec"(ptr noundef %52, i32 noundef %53)
  store ptr %54, ptr %7, align 8
  %55 = load ptr, ptr %4, align 8
  %56 = load ptr, ptr %7, align 8
  call void @"push$MODVec"(ptr noundef %55, ptr noundef %56)
  br label %57

57:                                               ; preds = %51, %48, %38
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"cpy_and_pushm$MODVec"(ptr noundef %0, ptr noundef %1, i32 noundef %2, ...) #0 {
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca i32, align 4
  %7 = alloca [1 x %struct.__va_list_tag], align 16
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca ptr, align 8
  store ptr %0, ptr %4, align 8
  store ptr %1, ptr %5, align 8
  store i32 %2, ptr %6, align 4
  %11 = load ptr, ptr %4, align 8
  %12 = getelementptr inbounds nuw %struct.Vec, ptr %11, i32 0, i32 3
  %13 = load i32, ptr %12, align 8
  %14 = load ptr, ptr %5, align 8
  %15 = getelementptr inbounds nuw %struct.Vec, ptr %14, i32 0, i32 3
  %16 = load i32, ptr %15, align 8
  %17 = icmp ne i32 %13, %16
  br i1 %17, label %18, label %21

18:                                               ; preds = %3
  %19 = load ptr, ptr @stderr, align 8
  %20 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %19, ptr noundef @.str.12) #6
  br label %21

21:                                               ; preds = %18, %3
  %22 = load ptr, ptr %5, align 8
  %23 = getelementptr inbounds nuw %struct.Vec, ptr %22, i32 0, i32 0
  %24 = load ptr, ptr %23, align 8
  %25 = icmp ne ptr %24, null
  br i1 %25, label %26, label %41

26:                                               ; preds = %21
  %27 = load ptr, ptr %4, align 8
  %28 = getelementptr inbounds nuw %struct.Vec, ptr %27, i32 0, i32 0
  %29 = load ptr, ptr %28, align 8
  %30 = icmp ne ptr %29, null
  br i1 %30, label %31, label %41

31:                                               ; preds = %26
  %32 = load ptr, ptr %5, align 8
  %33 = getelementptr inbounds nuw %struct.Vec, ptr %32, i32 0, i32 4
  %34 = load i8, ptr %33, align 4
  %35 = trunc i8 %34 to i1
  br i1 %35, label %36, label %41

36:                                               ; preds = %31
  %37 = load ptr, ptr %4, align 8
  %38 = getelementptr inbounds nuw %struct.Vec, ptr %37, i32 0, i32 4
  %39 = load i8, ptr %38, align 4
  %40 = trunc i8 %39 to i1
  br i1 %40, label %42, label %41

41:                                               ; preds = %36, %31, %26, %21
  br label %87

42:                                               ; preds = %36
  %43 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %7, i64 0, i64 0
  call void @llvm.va_start.p0(ptr %43)
  store i32 0, ptr %8, align 4
  br label %44

44:                                               ; preds = %82, %42
  %45 = load i32, ptr %8, align 4
  %46 = load i32, ptr %6, align 4
  %47 = icmp slt i32 %45, %46
  br i1 %47, label %48, label %85

48:                                               ; preds = %44
  %49 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %7, i64 0, i64 0
  %50 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %49, i32 0, i32 0
  %51 = load i32, ptr %50, align 16
  %52 = icmp ule i32 %51, 40
  br i1 %52, label %53, label %58

53:                                               ; preds = %48
  %54 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %49, i32 0, i32 3
  %55 = load ptr, ptr %54, align 16
  %56 = getelementptr i8, ptr %55, i32 %51
  %57 = add i32 %51, 8
  store i32 %57, ptr %50, align 16
  br label %62

58:                                               ; preds = %48
  %59 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %49, i32 0, i32 2
  %60 = load ptr, ptr %59, align 8
  %61 = getelementptr i8, ptr %60, i32 8
  store ptr %61, ptr %59, align 8
  br label %62

62:                                               ; preds = %58, %53
  %63 = phi ptr [ %56, %53 ], [ %60, %58 ]
  %64 = load i32, ptr %63, align 4
  store i32 %64, ptr %9, align 4
  %65 = load i32, ptr %9, align 4
  %66 = icmp slt i32 %65, 0
  br i1 %66, label %73, label %67

67:                                               ; preds = %62
  %68 = load i32, ptr %9, align 4
  %69 = load ptr, ptr %5, align 8
  %70 = getelementptr inbounds nuw %struct.Vec, ptr %69, i32 0, i32 1
  %71 = load i32, ptr %70, align 8
  %72 = icmp sge i32 %68, %71
  br i1 %72, label %73, label %76

73:                                               ; preds = %67, %62
  %74 = load ptr, ptr @stderr, align 8
  %75 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %74, ptr noundef @.str.13) #6
  br label %82

76:                                               ; preds = %67
  %77 = load ptr, ptr %5, align 8
  %78 = load i32, ptr %9, align 4
  %79 = call ptr @"get$MODVec"(ptr noundef %77, i32 noundef %78)
  store ptr %79, ptr %10, align 8
  %80 = load ptr, ptr %4, align 8
  %81 = load ptr, ptr %10, align 8
  call void @"push$MODVec"(ptr noundef %80, ptr noundef %81)
  br label %82

82:                                               ; preds = %76, %73
  %83 = load i32, ptr %8, align 4
  %84 = add nsw i32 %83, 1
  store i32 %84, ptr %8, align 4
  br label %44, !llvm.loop !25

85:                                               ; preds = %44
  %86 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %7, i64 0, i64 0
  call void @llvm.va_end.p0(ptr %86)
  br label %87

87:                                               ; preds = %85, %41
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"cpy_in$MODVec"(ptr noundef %0, i32 noundef %1, ptr noundef %2, i32 noundef %3) #0 {
  %5 = alloca ptr, align 8
  %6 = alloca i32, align 4
  %7 = alloca ptr, align 8
  %8 = alloca i32, align 4
  %9 = alloca ptr, align 8
  store ptr %0, ptr %5, align 8
  store i32 %1, ptr %6, align 4
  store ptr %2, ptr %7, align 8
  store i32 %3, ptr %8, align 4
  %10 = load ptr, ptr %5, align 8
  %11 = getelementptr inbounds nuw %struct.Vec, ptr %10, i32 0, i32 3
  %12 = load i32, ptr %11, align 8
  %13 = load ptr, ptr %7, align 8
  %14 = getelementptr inbounds nuw %struct.Vec, ptr %13, i32 0, i32 3
  %15 = load i32, ptr %14, align 8
  %16 = icmp ne i32 %12, %15
  br i1 %16, label %17, label %20

17:                                               ; preds = %4
  %18 = load ptr, ptr @stderr, align 8
  %19 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %18, ptr noundef @.str.14) #6
  br label %20

20:                                               ; preds = %17, %4
  %21 = load ptr, ptr %7, align 8
  %22 = getelementptr inbounds nuw %struct.Vec, ptr %21, i32 0, i32 0
  %23 = load ptr, ptr %22, align 8
  %24 = icmp ne ptr %23, null
  br i1 %24, label %25, label %40

25:                                               ; preds = %20
  %26 = load ptr, ptr %5, align 8
  %27 = getelementptr inbounds nuw %struct.Vec, ptr %26, i32 0, i32 0
  %28 = load ptr, ptr %27, align 8
  %29 = icmp ne ptr %28, null
  br i1 %29, label %30, label %40

30:                                               ; preds = %25
  %31 = load ptr, ptr %7, align 8
  %32 = getelementptr inbounds nuw %struct.Vec, ptr %31, i32 0, i32 4
  %33 = load i8, ptr %32, align 4
  %34 = trunc i8 %33 to i1
  br i1 %34, label %35, label %40

35:                                               ; preds = %30
  %36 = load ptr, ptr %5, align 8
  %37 = getelementptr inbounds nuw %struct.Vec, ptr %36, i32 0, i32 4
  %38 = load i8, ptr %37, align 4
  %39 = trunc i8 %38 to i1
  br i1 %39, label %41, label %40

40:                                               ; preds = %35, %30, %25, %20
  br label %84

41:                                               ; preds = %35
  %42 = load i32, ptr %8, align 4
  %43 = icmp slt i32 %42, 0
  br i1 %43, label %53, label %44

44:                                               ; preds = %41
  %45 = load i32, ptr %8, align 4
  %46 = load ptr, ptr %7, align 8
  %47 = getelementptr inbounds nuw %struct.Vec, ptr %46, i32 0, i32 1
  %48 = load i32, ptr %47, align 8
  %49 = icmp sge i32 %45, %48
  br i1 %49, label %53, label %50

50:                                               ; preds = %44
  %51 = load i32, ptr %6, align 4
  %52 = icmp slt i32 %51, 0
  br i1 %52, label %53, label %56

53:                                               ; preds = %50, %44, %41
  %54 = load ptr, ptr @stderr, align 8
  %55 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %54, ptr noundef @.str.15) #6
  br label %84

56:                                               ; preds = %50
  %57 = load ptr, ptr %7, align 8
  %58 = load i32, ptr %8, align 4
  %59 = call ptr @"get$MODVec"(ptr noundef %57, i32 noundef %58)
  store ptr %59, ptr %9, align 8
  %60 = load i32, ptr %6, align 4
  %61 = load ptr, ptr %5, align 8
  %62 = getelementptr inbounds nuw %struct.Vec, ptr %61, i32 0, i32 1
  %63 = load i32, ptr %62, align 8
  %64 = icmp sge i32 %60, %63
  br i1 %64, label %65, label %68

65:                                               ; preds = %56
  %66 = load ptr, ptr %5, align 8
  %67 = load ptr, ptr %9, align 8
  call void @"push$MODVec"(ptr noundef %66, ptr noundef %67)
  br label %84

68:                                               ; preds = %56
  %69 = load ptr, ptr %5, align 8
  %70 = getelementptr inbounds nuw %struct.Vec, ptr %69, i32 0, i32 0
  %71 = load ptr, ptr %70, align 8
  %72 = load i32, ptr %6, align 4
  %73 = load ptr, ptr %5, align 8
  %74 = getelementptr inbounds nuw %struct.Vec, ptr %73, i32 0, i32 3
  %75 = load i32, ptr %74, align 8
  %76 = mul nsw i32 %72, %75
  %77 = sext i32 %76 to i64
  %78 = getelementptr inbounds i8, ptr %71, i64 %77
  %79 = load ptr, ptr %9, align 8
  %80 = load ptr, ptr %5, align 8
  %81 = getelementptr inbounds nuw %struct.Vec, ptr %80, i32 0, i32 3
  %82 = load i32, ptr %81, align 8
  %83 = sext i32 %82 to i64
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %78, ptr align 1 %79, i64 %83, i1 false)
  br label %84

84:                                               ; preds = %68, %65, %53, %40
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"cpy_xelem_in$MODVec"(ptr noundef %0, ptr noundef %1, i32 noundef %2, ...) #0 {
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca i32, align 4
  %7 = alloca [1 x %struct.__va_list_tag], align 16
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca ptr, align 8
  store ptr %0, ptr %4, align 8
  store ptr %1, ptr %5, align 8
  store i32 %2, ptr %6, align 4
  %12 = load ptr, ptr %4, align 8
  %13 = getelementptr inbounds nuw %struct.Vec, ptr %12, i32 0, i32 3
  %14 = load i32, ptr %13, align 8
  %15 = load ptr, ptr %5, align 8
  %16 = getelementptr inbounds nuw %struct.Vec, ptr %15, i32 0, i32 3
  %17 = load i32, ptr %16, align 8
  %18 = icmp ne i32 %14, %17
  br i1 %18, label %19, label %22

19:                                               ; preds = %3
  %20 = load ptr, ptr @stderr, align 8
  %21 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %20, ptr noundef @.str.14) #6
  br label %22

22:                                               ; preds = %19, %3
  %23 = load ptr, ptr %5, align 8
  %24 = getelementptr inbounds nuw %struct.Vec, ptr %23, i32 0, i32 0
  %25 = load ptr, ptr %24, align 8
  %26 = icmp ne ptr %25, null
  br i1 %26, label %27, label %42

27:                                               ; preds = %22
  %28 = load ptr, ptr %4, align 8
  %29 = getelementptr inbounds nuw %struct.Vec, ptr %28, i32 0, i32 0
  %30 = load ptr, ptr %29, align 8
  %31 = icmp ne ptr %30, null
  br i1 %31, label %32, label %42

32:                                               ; preds = %27
  %33 = load ptr, ptr %5, align 8
  %34 = getelementptr inbounds nuw %struct.Vec, ptr %33, i32 0, i32 4
  %35 = load i8, ptr %34, align 4
  %36 = trunc i8 %35 to i1
  br i1 %36, label %37, label %42

37:                                               ; preds = %32
  %38 = load ptr, ptr %4, align 8
  %39 = getelementptr inbounds nuw %struct.Vec, ptr %38, i32 0, i32 4
  %40 = load i8, ptr %39, align 4
  %41 = trunc i8 %40 to i1
  br i1 %41, label %43, label %42

42:                                               ; preds = %37, %32, %27, %22
  br label %127

43:                                               ; preds = %37
  %44 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %7, i64 0, i64 0
  call void @llvm.va_start.p0(ptr %44)
  store i32 0, ptr %8, align 4
  br label %45

45:                                               ; preds = %124, %43
  %46 = load i32, ptr %8, align 4
  %47 = load i32, ptr %6, align 4
  %48 = icmp slt i32 %46, %47
  br i1 %48, label %49, label %127

49:                                               ; preds = %45
  %50 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %7, i64 0, i64 0
  %51 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %50, i32 0, i32 0
  %52 = load i32, ptr %51, align 16
  %53 = icmp ule i32 %52, 40
  br i1 %53, label %54, label %59

54:                                               ; preds = %49
  %55 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %50, i32 0, i32 3
  %56 = load ptr, ptr %55, align 16
  %57 = getelementptr i8, ptr %56, i32 %52
  %58 = add i32 %52, 8
  store i32 %58, ptr %51, align 16
  br label %63

59:                                               ; preds = %49
  %60 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %50, i32 0, i32 2
  %61 = load ptr, ptr %60, align 8
  %62 = getelementptr i8, ptr %61, i32 8
  store ptr %62, ptr %60, align 8
  br label %63

63:                                               ; preds = %59, %54
  %64 = phi ptr [ %57, %54 ], [ %61, %59 ]
  %65 = load i32, ptr %64, align 4
  store i32 %65, ptr %9, align 4
  %66 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %7, i64 0, i64 0
  %67 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %66, i32 0, i32 0
  %68 = load i32, ptr %67, align 16
  %69 = icmp ule i32 %68, 40
  br i1 %69, label %70, label %75

70:                                               ; preds = %63
  %71 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %66, i32 0, i32 3
  %72 = load ptr, ptr %71, align 16
  %73 = getelementptr i8, ptr %72, i32 %68
  %74 = add i32 %68, 8
  store i32 %74, ptr %67, align 16
  br label %79

75:                                               ; preds = %63
  %76 = getelementptr inbounds nuw %struct.__va_list_tag, ptr %66, i32 0, i32 2
  %77 = load ptr, ptr %76, align 8
  %78 = getelementptr i8, ptr %77, i32 8
  store ptr %78, ptr %76, align 8
  br label %79

79:                                               ; preds = %75, %70
  %80 = phi ptr [ %73, %70 ], [ %77, %75 ]
  %81 = load i32, ptr %80, align 4
  store i32 %81, ptr %10, align 4
  %82 = load i32, ptr %9, align 4
  %83 = icmp slt i32 %82, 0
  br i1 %83, label %93, label %84

84:                                               ; preds = %79
  %85 = load i32, ptr %9, align 4
  %86 = load ptr, ptr %5, align 8
  %87 = getelementptr inbounds nuw %struct.Vec, ptr %86, i32 0, i32 1
  %88 = load i32, ptr %87, align 8
  %89 = icmp sge i32 %85, %88
  br i1 %89, label %93, label %90

90:                                               ; preds = %84
  %91 = load i32, ptr %10, align 4
  %92 = icmp slt i32 %91, 0
  br i1 %92, label %93, label %96

93:                                               ; preds = %90, %84, %79
  %94 = load ptr, ptr @stderr, align 8
  %95 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %94, ptr noundef @.str.15) #6
  br label %127

96:                                               ; preds = %90
  %97 = load ptr, ptr %5, align 8
  %98 = load i32, ptr %9, align 4
  %99 = call ptr @"get$MODVec"(ptr noundef %97, i32 noundef %98)
  store ptr %99, ptr %11, align 8
  %100 = load i32, ptr %10, align 4
  %101 = load ptr, ptr %4, align 8
  %102 = getelementptr inbounds nuw %struct.Vec, ptr %101, i32 0, i32 1
  %103 = load i32, ptr %102, align 8
  %104 = icmp sge i32 %100, %103
  br i1 %104, label %105, label %108

105:                                              ; preds = %96
  %106 = load ptr, ptr %4, align 8
  %107 = load ptr, ptr %11, align 8
  call void @"push$MODVec"(ptr noundef %106, ptr noundef %107)
  br label %127

108:                                              ; preds = %96
  %109 = load ptr, ptr %4, align 8
  %110 = getelementptr inbounds nuw %struct.Vec, ptr %109, i32 0, i32 0
  %111 = load ptr, ptr %110, align 8
  %112 = load i32, ptr %10, align 4
  %113 = load ptr, ptr %4, align 8
  %114 = getelementptr inbounds nuw %struct.Vec, ptr %113, i32 0, i32 3
  %115 = load i32, ptr %114, align 8
  %116 = mul nsw i32 %112, %115
  %117 = sext i32 %116 to i64
  %118 = getelementptr inbounds i8, ptr %111, i64 %117
  %119 = load ptr, ptr %11, align 8
  %120 = load ptr, ptr %4, align 8
  %121 = getelementptr inbounds nuw %struct.Vec, ptr %120, i32 0, i32 3
  %122 = load i32, ptr %121, align 8
  %123 = sext i32 %122 to i64
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %118, ptr align 1 %119, i64 %123, i1 false)
  br label %124

124:                                              ; preds = %108
  %125 = load i32, ptr %8, align 4
  %126 = add nsw i32 %125, 1
  store i32 %126, ptr %8, align 4
  br label %45, !llvm.loop !26

127:                                              ; preds = %42, %93, %105, %45
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"clone$MODVec"(ptr dead_on_unwind noalias writable sret(%struct.Vec) align 8 %0, ptr noundef %1) #0 {
  %3 = alloca ptr, align 8
  store ptr %1, ptr %3, align 8
  %4 = load ptr, ptr %3, align 8
  %5 = getelementptr inbounds nuw %struct.Vec, ptr %4, i32 0, i32 3
  %6 = load i32, ptr %5, align 8
  %7 = load ptr, ptr %3, align 8
  %8 = getelementptr inbounds nuw %struct.Vec, ptr %7, i32 0, i32 2
  %9 = load i32, ptr %8, align 4
  %10 = mul nsw i32 %6, %9
  %11 = call ptr @"alloc$MODmem"(i32 noundef %10)
  %12 = getelementptr inbounds nuw %struct.Vec, ptr %0, i32 0, i32 0
  store ptr %11, ptr %12, align 8
  %13 = getelementptr inbounds nuw %struct.Vec, ptr %0, i32 0, i32 0
  %14 = load ptr, ptr %13, align 8
  %15 = icmp ne ptr %14, null
  br i1 %15, label %19, label %16

16:                                               ; preds = %2
  %17 = load ptr, ptr @stderr, align 8
  %18 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %17, ptr noundef @.str.16) #6
  br label %46

19:                                               ; preds = %2
  %20 = getelementptr inbounds nuw %struct.Vec, ptr %0, i32 0, i32 0
  %21 = load ptr, ptr %20, align 8
  %22 = load ptr, ptr %3, align 8
  %23 = getelementptr inbounds nuw %struct.Vec, ptr %22, i32 0, i32 0
  %24 = load ptr, ptr %23, align 8
  %25 = load ptr, ptr %3, align 8
  %26 = getelementptr inbounds nuw %struct.Vec, ptr %25, i32 0, i32 2
  %27 = load i32, ptr %26, align 4
  %28 = load ptr, ptr %3, align 8
  %29 = getelementptr inbounds nuw %struct.Vec, ptr %28, i32 0, i32 3
  %30 = load i32, ptr %29, align 8
  %31 = mul nsw i32 %27, %30
  %32 = sext i32 %31 to i64
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %21, ptr align 1 %24, i64 %32, i1 false)
  %33 = load ptr, ptr %3, align 8
  %34 = getelementptr inbounds nuw %struct.Vec, ptr %33, i32 0, i32 2
  %35 = load i32, ptr %34, align 4
  %36 = getelementptr inbounds nuw %struct.Vec, ptr %0, i32 0, i32 2
  store i32 %35, ptr %36, align 4
  %37 = load ptr, ptr %3, align 8
  %38 = getelementptr inbounds nuw %struct.Vec, ptr %37, i32 0, i32 1
  %39 = load i32, ptr %38, align 8
  %40 = getelementptr inbounds nuw %struct.Vec, ptr %0, i32 0, i32 1
  store i32 %39, ptr %40, align 8
  %41 = load ptr, ptr %3, align 8
  %42 = getelementptr inbounds nuw %struct.Vec, ptr %41, i32 0, i32 3
  %43 = load i32, ptr %42, align 8
  %44 = getelementptr inbounds nuw %struct.Vec, ptr %0, i32 0, i32 3
  store i32 %43, ptr %44, align 8
  %45 = getelementptr inbounds nuw %struct.Vec, ptr %0, i32 0, i32 4
  store i8 1, ptr %45, align 4
  br label %46

46:                                               ; preds = %19, %16
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local ptr @"iterate$MODVec"(ptr noundef %0, ptr noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  store ptr %0, ptr %4, align 8
  store ptr %1, ptr %5, align 8
  %6 = load ptr, ptr %5, align 8
  %7 = load i32, ptr %6, align 4
  %8 = load ptr, ptr %4, align 8
  %9 = getelementptr inbounds nuw %struct.Vec, ptr %8, i32 0, i32 1
  %10 = load i32, ptr %9, align 8
  %11 = icmp sge i32 %7, %10
  br i1 %11, label %12, label %14

12:                                               ; preds = %2
  %13 = load ptr, ptr %5, align 8
  store i32 -1, ptr %13, align 4
  store ptr null, ptr %3, align 8
  br label %19

14:                                               ; preds = %2
  %15 = load ptr, ptr %4, align 8
  %16 = load ptr, ptr %5, align 8
  %17 = load i32, ptr %16, align 4
  %18 = call ptr @"get$MODVec"(ptr noundef %15, i32 noundef %17)
  store ptr %18, ptr %3, align 8
  br label %19

19:                                               ; preds = %14, %12
  %20 = load ptr, ptr %3, align 8
  ret ptr %20
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @"size$MODVec"(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  %3 = load ptr, ptr %2, align 8
  %4 = getelementptr inbounds nuw %struct.Vec, ptr %3, i32 0, i32 1
  %5 = load i32, ptr %4, align 8
  ret i32 %5
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @"ssize$MODVec"() #0 {
  ret i32 24
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"abc$MODVec"(ptr dead_on_unwind noalias writable sret(%struct.Vec) align 8 %0) #0 {
  call void (ptr, i32, ...) @"new_char$MODVec"(ptr dead_on_unwind writable sret(%struct.Vec) align 8 %0, i32 noundef 26, i32 noundef 97, i32 noundef 98, i32 noundef 99, i32 noundef 100, i32 noundef 101, i32 noundef 102, i32 noundef 103, i32 noundef 104, i32 noundef 105, i32 noundef 106, i32 noundef 107, i32 noundef 108, i32 noundef 109, i32 noundef 110, i32 noundef 111, i32 noundef 112, i32 noundef 113, i32 noundef 114, i32 noundef 115, i32 noundef 116, i32 noundef 117, i32 noundef 118, i32 noundef 119, i32 noundef 120, i32 noundef 121, i32 noundef 122)
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"ABC$MODVec"(ptr dead_on_unwind noalias writable sret(%struct.Vec) align 8 %0) #0 {
  call void (ptr, i32, ...) @"new_char$MODVec"(ptr dead_on_unwind writable sret(%struct.Vec) align 8 %0, i32 noundef 26, i32 noundef 65, i32 noundef 66, i32 noundef 67, i32 noundef 68, i32 noundef 69, i32 noundef 70, i32 noundef 71, i32 noundef 72, i32 noundef 73, i32 noundef 74, i32 noundef 75, i32 noundef 76, i32 noundef 77, i32 noundef 78, i32 noundef 79, i32 noundef 80, i32 noundef 81, i32 noundef 82, i32 noundef 83, i32 noundef 84, i32 noundef 85, i32 noundef 86, i32 noundef 87, i32 noundef 88, i32 noundef 89, i32 noundef 90)
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"abcABC$MODVec"(ptr dead_on_unwind noalias writable sret(%struct.Vec) align 8 %0) #0 {
  call void (ptr, i32, ...) @"new_char$MODVec"(ptr dead_on_unwind writable sret(%struct.Vec) align 8 %0, i32 noundef 52, i32 noundef 97, i32 noundef 98, i32 noundef 99, i32 noundef 100, i32 noundef 101, i32 noundef 102, i32 noundef 103, i32 noundef 104, i32 noundef 105, i32 noundef 106, i32 noundef 107, i32 noundef 108, i32 noundef 109, i32 noundef 110, i32 noundef 111, i32 noundef 112, i32 noundef 113, i32 noundef 114, i32 noundef 115, i32 noundef 116, i32 noundef 117, i32 noundef 118, i32 noundef 119, i32 noundef 120, i32 noundef 121, i32 noundef 122, i32 noundef 65, i32 noundef 66, i32 noundef 67, i32 noundef 68, i32 noundef 69, i32 noundef 70, i32 noundef 71, i32 noundef 72, i32 noundef 73, i32 noundef 74, i32 noundef 75, i32 noundef 76, i32 noundef 77, i32 noundef 78, i32 noundef 79, i32 noundef 80, i32 noundef 81, i32 noundef 82, i32 noundef 83, i32 noundef 84, i32 noundef 85, i32 noundef 86, i32 noundef 87, i32 noundef 88, i32 noundef 89, i32 noundef 90)
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"ABCabc$MODVec"(ptr dead_on_unwind noalias writable sret(%struct.Vec) align 8 %0) #0 {
  call void (ptr, i32, ...) @"new_char$MODVec"(ptr dead_on_unwind writable sret(%struct.Vec) align 8 %0, i32 noundef 52, i32 noundef 65, i32 noundef 66, i32 noundef 67, i32 noundef 68, i32 noundef 69, i32 noundef 70, i32 noundef 71, i32 noundef 72, i32 noundef 73, i32 noundef 74, i32 noundef 75, i32 noundef 76, i32 noundef 77, i32 noundef 78, i32 noundef 79, i32 noundef 80, i32 noundef 81, i32 noundef 82, i32 noundef 83, i32 noundef 84, i32 noundef 85, i32 noundef 86, i32 noundef 87, i32 noundef 88, i32 noundef 89, i32 noundef 90, i32 noundef 97, i32 noundef 98, i32 noundef 99, i32 noundef 100, i32 noundef 101, i32 noundef 102, i32 noundef 103, i32 noundef 104, i32 noundef 105, i32 noundef 106, i32 noundef 107, i32 noundef 108, i32 noundef 109, i32 noundef 110, i32 noundef 111, i32 noundef 112, i32 noundef 113, i32 noundef 114, i32 noundef 115, i32 noundef 116, i32 noundef 117, i32 noundef 118, i32 noundef 119, i32 noundef 120, i32 noundef 121, i32 noundef 122)
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"AaBbCc$MODVec"(ptr dead_on_unwind noalias writable sret(%struct.Vec) align 8 %0) #0 {
  call void (ptr, i32, ...) @"new_char$MODVec"(ptr dead_on_unwind writable sret(%struct.Vec) align 8 %0, i32 noundef 52, i32 noundef 65, i32 noundef 97, i32 noundef 66, i32 noundef 98, i32 noundef 67, i32 noundef 99, i32 noundef 68, i32 noundef 100, i32 noundef 69, i32 noundef 101, i32 noundef 70, i32 noundef 102, i32 noundef 71, i32 noundef 103, i32 noundef 72, i32 noundef 104, i32 noundef 73, i32 noundef 105, i32 noundef 74, i32 noundef 106, i32 noundef 75, i32 noundef 107, i32 noundef 76, i32 noundef 108, i32 noundef 77, i32 noundef 109, i32 noundef 78, i32 noundef 110, i32 noundef 79, i32 noundef 111, i32 noundef 80, i32 noundef 112, i32 noundef 81, i32 noundef 113, i32 noundef 82, i32 noundef 114, i32 noundef 83, i32 noundef 115, i32 noundef 84, i32 noundef 116, i32 noundef 85, i32 noundef 117, i32 noundef 86, i32 noundef 118, i32 noundef 87, i32 noundef 119, i32 noundef 88, i32 noundef 120, i32 noundef 89, i32 noundef 121, i32 noundef 90, i32 noundef 122)
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @"aAbBcC$MODVec"(ptr dead_on_unwind noalias writable sret(%struct.Vec) align 8 %0) #0 {
  call void (ptr, i32, ...) @"new_char$MODVec"(ptr dead_on_unwind writable sret(%struct.Vec) align 8 %0, i32 noundef 52, i32 noundef 97, i32 noundef 65, i32 noundef 98, i32 noundef 66, i32 noundef 99, i32 noundef 67, i32 noundef 100, i32 noundef 68, i32 noundef 101, i32 noundef 69, i32 noundef 102, i32 noundef 70, i32 noundef 103, i32 noundef 71, i32 noundef 104, i32 noundef 72, i32 noundef 105, i32 noundef 73, i32 noundef 106, i32 noundef 74, i32 noundef 107, i32 noundef 75, i32 noundef 108, i32 noundef 76, i32 noundef 109, i32 noundef 77, i32 noundef 110, i32 noundef 78, i32 noundef 111, i32 noundef 79, i32 noundef 112, i32 noundef 80, i32 noundef 113, i32 noundef 81, i32 noundef 114, i32 noundef 82, i32 noundef 115, i32 noundef 83, i32 noundef 116, i32 noundef 84, i32 noundef 117, i32 noundef 85, i32 noundef 118, i32 noundef 86, i32 noundef 119, i32 noundef 87, i32 noundef 120, i32 noundef 88, i32 noundef 121, i32 noundef 89, i32 noundef 122, i32 noundef 90)
  ret void
}

attributes #0 = { noinline nounwind optnone sspstrong uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #2 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #5 = { nocallback nofree nosync nounwind willreturn }
attributes #6 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 22.1.8"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
!11 = distinct !{!11, !7}
!12 = distinct !{!12, !7}
!13 = distinct !{!13, !7}
!14 = distinct !{!14, !7}
!15 = distinct !{!15, !7}
!16 = distinct !{!16, !7}
!17 = distinct !{!17, !7}
!18 = distinct !{!18, !7}
!19 = distinct !{!19, !7}
!20 = distinct !{!20, !7}
!21 = distinct !{!21, !7}
!22 = distinct !{!22, !7}
!23 = distinct !{!23, !7}
!24 = distinct !{!24, !7}
!25 = distinct !{!25, !7}
!26 = distinct !{!26, !7}
