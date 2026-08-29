; ModuleID = 'myio.c'
source_filename = "myio.c"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.IOContext = type { ptr, i32, i32, ptr, i64, i64, i32 }

@.str = private unnamed_addr constant [4 x i8] c"w+b\00", align 1
@.str.1 = private unnamed_addr constant [3 x i8] c"w+\00", align 1
@.str.2 = private unnamed_addr constant [3 x i8] c"rb\00", align 1
@.str.3 = private unnamed_addr constant [2 x i8] c"r\00", align 1
@.str.4 = private unnamed_addr constant [3 x i8] c"wb\00", align 1
@.str.5 = private unnamed_addr constant [2 x i8] c"w\00", align 1
@.str.6 = private unnamed_addr constant [3 x i8] c"ab\00", align 1
@.str.7 = private unnamed_addr constant [2 x i8] c"a\00", align 1
@.str.8 = private unnamed_addr constant [14 x i8] c"Unknown error\00", align 1
@error_messages = internal global [7 x ptr] [ptr @.str.9, ptr @.str.10, ptr @.str.11, ptr @.str.12, ptr @.str.13, ptr @.str.14, ptr @.str.15], align 16
@.str.9 = private unnamed_addr constant [8 x i8] c"Success\00", align 1
@.str.10 = private unnamed_addr constant [20 x i8] c"Failed to open file\00", align 1
@.str.11 = private unnamed_addr constant [25 x i8] c"Failed to read from file\00", align 1
@.str.12 = private unnamed_addr constant [24 x i8] c"Failed to write to file\00", align 1
@.str.13 = private unnamed_addr constant [21 x i8] c"Failed to close file\00", align 1
@.str.14 = private unnamed_addr constant [23 x i8] c"Failed to seek in file\00", align 1
@.str.15 = private unnamed_addr constant [25 x i8] c"Memory allocation failed\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @io_init() #0 {
  ret i32 0
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @io_cleanup() #0 {
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @io_open(ptr noundef %0, i32 noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  %6 = alloca ptr, align 8
  %7 = alloca ptr, align 8
  store ptr %0, ptr %4, align 8
  store i32 %1, ptr %5, align 4
  store ptr null, ptr %6, align 8
  store ptr null, ptr %7, align 8
  %8 = call ptr @calloc(i64 noundef 1, i64 noundef 48) #5
  store ptr %8, ptr %6, align 8
  %9 = load ptr, ptr %6, align 8
  %10 = icmp ne ptr %9, null
  br i1 %10, label %12, label %11

11:                                               ; preds = %2
  store ptr null, ptr %3, align 8
  br label %82

12:                                               ; preds = %2
  %13 = load i32, ptr %5, align 4
  %14 = and i32 %13, 1
  %15 = icmp ne i32 %14, 0
  br i1 %15, label %16, label %26

16:                                               ; preds = %12
  %17 = load i32, ptr %5, align 4
  %18 = and i32 %17, 2
  %19 = icmp ne i32 %18, 0
  br i1 %19, label %20, label %26

20:                                               ; preds = %16
  %21 = load i32, ptr %5, align 4
  %22 = and i32 %21, 8
  %23 = icmp ne i32 %22, 0
  %24 = zext i1 %23 to i64
  %25 = select i1 %23, ptr @.str, ptr @.str.1
  store ptr %25, ptr %7, align 8
  br label %61

26:                                               ; preds = %16, %12
  %27 = load i32, ptr %5, align 4
  %28 = and i32 %27, 1
  %29 = icmp ne i32 %28, 0
  br i1 %29, label %30, label %36

30:                                               ; preds = %26
  %31 = load i32, ptr %5, align 4
  %32 = and i32 %31, 8
  %33 = icmp ne i32 %32, 0
  %34 = zext i1 %33 to i64
  %35 = select i1 %33, ptr @.str.2, ptr @.str.3
  store ptr %35, ptr %7, align 8
  br label %60

36:                                               ; preds = %26
  %37 = load i32, ptr %5, align 4
  %38 = and i32 %37, 2
  %39 = icmp ne i32 %38, 0
  br i1 %39, label %40, label %46

40:                                               ; preds = %36
  %41 = load i32, ptr %5, align 4
  %42 = and i32 %41, 8
  %43 = icmp ne i32 %42, 0
  %44 = zext i1 %43 to i64
  %45 = select i1 %43, ptr @.str.4, ptr @.str.5
  store ptr %45, ptr %7, align 8
  br label %59

46:                                               ; preds = %36
  %47 = load i32, ptr %5, align 4
  %48 = and i32 %47, 4
  %49 = icmp ne i32 %48, 0
  br i1 %49, label %50, label %56

50:                                               ; preds = %46
  %51 = load i32, ptr %5, align 4
  %52 = and i32 %51, 8
  %53 = icmp ne i32 %52, 0
  %54 = zext i1 %53 to i64
  %55 = select i1 %53, ptr @.str.6, ptr @.str.7
  store ptr %55, ptr %7, align 8
  br label %58

56:                                               ; preds = %46
  %57 = load ptr, ptr %6, align 8
  call void @free(ptr noundef %57)
  store ptr null, ptr %3, align 8
  br label %82

58:                                               ; preds = %50
  br label %59

59:                                               ; preds = %58, %40
  br label %60

60:                                               ; preds = %59, %30
  br label %61

61:                                               ; preds = %60, %20
  %62 = load ptr, ptr %4, align 8
  %63 = load ptr, ptr %7, align 8
  %64 = call ptr @fopen(ptr noundef %62, ptr noundef %63)
  %65 = load ptr, ptr %6, align 8
  %66 = getelementptr inbounds nuw %struct.IOContext, ptr %65, i32 0, i32 0
  store ptr %64, ptr %66, align 8
  %67 = load ptr, ptr %6, align 8
  %68 = getelementptr inbounds nuw %struct.IOContext, ptr %67, i32 0, i32 0
  %69 = load ptr, ptr %68, align 8
  %70 = icmp ne ptr %69, null
  br i1 %70, label %75, label %71

71:                                               ; preds = %61
  %72 = load ptr, ptr %6, align 8
  %73 = getelementptr inbounds nuw %struct.IOContext, ptr %72, i32 0, i32 1
  store i32 1, ptr %73, align 8
  %74 = load ptr, ptr %6, align 8
  call void @free(ptr noundef %74)
  store ptr null, ptr %3, align 8
  br label %82

75:                                               ; preds = %61
  %76 = load i32, ptr %5, align 4
  %77 = load ptr, ptr %6, align 8
  %78 = getelementptr inbounds nuw %struct.IOContext, ptr %77, i32 0, i32 2
  store i32 %76, ptr %78, align 4
  %79 = load ptr, ptr %6, align 8
  %80 = getelementptr inbounds nuw %struct.IOContext, ptr %79, i32 0, i32 6
  store i32 0, ptr %80, align 8
  %81 = load ptr, ptr %6, align 8
  store ptr %81, ptr %3, align 8
  br label %82

82:                                               ; preds = %75, %71, %56, %11
  %83 = load ptr, ptr %3, align 8
  ret ptr %83
}

; Function Attrs: allocsize(0,1)
declare dso_local ptr @calloc(i64 noundef, i64 noundef) #1

declare dso_local void @free(ptr noundef) #2

declare dso_local ptr @fopen(ptr noundef, ptr noundef) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @io_close(ptr noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
  %4 = load ptr, ptr %3, align 8
  %5 = icmp ne ptr %4, null
  br i1 %5, label %7, label %6

6:                                                ; preds = %1
  store i32 4, ptr %2, align 4
  br label %33

7:                                                ; preds = %1
  %8 = load ptr, ptr %3, align 8
  %9 = getelementptr inbounds nuw %struct.IOContext, ptr %8, i32 0, i32 0
  %10 = load ptr, ptr %9, align 8
  %11 = icmp ne ptr %10, null
  br i1 %11, label %12, label %22

12:                                               ; preds = %7
  %13 = load ptr, ptr %3, align 8
  %14 = getelementptr inbounds nuw %struct.IOContext, ptr %13, i32 0, i32 0
  %15 = load ptr, ptr %14, align 8
  %16 = call i32 @fclose(ptr noundef %15)
  %17 = icmp ne i32 %16, 0
  br i1 %17, label %18, label %21

18:                                               ; preds = %12
  %19 = load ptr, ptr %3, align 8
  %20 = getelementptr inbounds nuw %struct.IOContext, ptr %19, i32 0, i32 1
  store i32 4, ptr %20, align 8
  store i32 4, ptr %2, align 4
  br label %33

21:                                               ; preds = %12
  br label %22

22:                                               ; preds = %21, %7
  %23 = load ptr, ptr %3, align 8
  %24 = getelementptr inbounds nuw %struct.IOContext, ptr %23, i32 0, i32 3
  %25 = load ptr, ptr %24, align 8
  %26 = icmp ne ptr %25, null
  br i1 %26, label %27, label %31

27:                                               ; preds = %22
  %28 = load ptr, ptr %3, align 8
  %29 = getelementptr inbounds nuw %struct.IOContext, ptr %28, i32 0, i32 3
  %30 = load ptr, ptr %29, align 8
  call void @free(ptr noundef %30)
  br label %31

31:                                               ; preds = %27, %22
  %32 = load ptr, ptr %3, align 8
  call void @free(ptr noundef %32)
  store i32 0, ptr %2, align 4
  br label %33

33:                                               ; preds = %31, %18, %6
  %34 = load i32, ptr %2, align 4
  ret i32 %34
}

declare dso_local i32 @fclose(ptr noundef) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i64 @io_read(ptr noundef %0, ptr noundef %1, i64 noundef %2) #0 {
  %4 = alloca i64, align 8
  %5 = alloca ptr, align 8
  %6 = alloca ptr, align 8
  %7 = alloca i64, align 8
  %8 = alloca i64, align 8
  store ptr %0, ptr %5, align 8
  store ptr %1, ptr %6, align 8
  store i64 %2, ptr %7, align 8
  %9 = load ptr, ptr %5, align 8
  %10 = icmp ne ptr %9, null
  br i1 %10, label %11, label %19

11:                                               ; preds = %3
  %12 = load ptr, ptr %5, align 8
  %13 = getelementptr inbounds nuw %struct.IOContext, ptr %12, i32 0, i32 0
  %14 = load ptr, ptr %13, align 8
  %15 = icmp ne ptr %14, null
  br i1 %15, label %16, label %19

16:                                               ; preds = %11
  %17 = load ptr, ptr %6, align 8
  %18 = icmp ne ptr %17, null
  br i1 %18, label %26, label %19

19:                                               ; preds = %16, %11, %3
  %20 = load ptr, ptr %5, align 8
  %21 = icmp ne ptr %20, null
  br i1 %21, label %22, label %25

22:                                               ; preds = %19
  %23 = load ptr, ptr %5, align 8
  %24 = getelementptr inbounds nuw %struct.IOContext, ptr %23, i32 0, i32 1
  store i32 2, ptr %24, align 8
  br label %25

25:                                               ; preds = %22, %19
  store i64 0, ptr %4, align 8
  br label %56

26:                                               ; preds = %16
  %27 = load ptr, ptr %6, align 8
  %28 = load i64, ptr %7, align 8
  %29 = load ptr, ptr %5, align 8
  %30 = getelementptr inbounds nuw %struct.IOContext, ptr %29, i32 0, i32 0
  %31 = load ptr, ptr %30, align 8
  %32 = call i64 @fread(ptr noundef %27, i64 noundef 1, i64 noundef %28, ptr noundef %31)
  store i64 %32, ptr %8, align 8
  %33 = load i64, ptr %8, align 8
  %34 = load i64, ptr %7, align 8
  %35 = icmp ult i64 %33, %34
  br i1 %35, label %36, label %45

36:                                               ; preds = %26
  %37 = load ptr, ptr %5, align 8
  %38 = getelementptr inbounds nuw %struct.IOContext, ptr %37, i32 0, i32 0
  %39 = load ptr, ptr %38, align 8
  %40 = call i32 @ferror(ptr noundef %39)
  %41 = icmp ne i32 %40, 0
  br i1 %41, label %42, label %45

42:                                               ; preds = %36
  %43 = load ptr, ptr %5, align 8
  %44 = getelementptr inbounds nuw %struct.IOContext, ptr %43, i32 0, i32 1
  store i32 2, ptr %44, align 8
  br label %45

45:                                               ; preds = %42, %36, %26
  %46 = load ptr, ptr %5, align 8
  %47 = getelementptr inbounds nuw %struct.IOContext, ptr %46, i32 0, i32 0
  %48 = load ptr, ptr %47, align 8
  %49 = call i32 @feof(ptr noundef %48)
  %50 = icmp ne i32 %49, 0
  br i1 %50, label %51, label %54

51:                                               ; preds = %45
  %52 = load ptr, ptr %5, align 8
  %53 = getelementptr inbounds nuw %struct.IOContext, ptr %52, i32 0, i32 6
  store i32 1, ptr %53, align 8
  br label %54

54:                                               ; preds = %51, %45
  %55 = load i64, ptr %8, align 8
  store i64 %55, ptr %4, align 8
  br label %56

56:                                               ; preds = %54, %25
  %57 = load i64, ptr %4, align 8
  ret i64 %57
}

declare dso_local i64 @fread(ptr noundef, i64 noundef, i64 noundef, ptr noundef) #2

declare dso_local i32 @ferror(ptr noundef) #2

declare dso_local i32 @feof(ptr noundef) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i64 @io_write(ptr noundef %0, ptr noundef %1, i64 noundef %2) #0 {
  %4 = alloca i64, align 8
  %5 = alloca ptr, align 8
  %6 = alloca ptr, align 8
  %7 = alloca i64, align 8
  %8 = alloca i64, align 8
  store ptr %0, ptr %5, align 8
  store ptr %1, ptr %6, align 8
  store i64 %2, ptr %7, align 8
  %9 = load ptr, ptr %5, align 8
  %10 = icmp ne ptr %9, null
  br i1 %10, label %11, label %19

11:                                               ; preds = %3
  %12 = load ptr, ptr %5, align 8
  %13 = getelementptr inbounds nuw %struct.IOContext, ptr %12, i32 0, i32 0
  %14 = load ptr, ptr %13, align 8
  %15 = icmp ne ptr %14, null
  br i1 %15, label %16, label %19

16:                                               ; preds = %11
  %17 = load ptr, ptr %6, align 8
  %18 = icmp ne ptr %17, null
  br i1 %18, label %26, label %19

19:                                               ; preds = %16, %11, %3
  %20 = load ptr, ptr %5, align 8
  %21 = icmp ne ptr %20, null
  br i1 %21, label %22, label %25

22:                                               ; preds = %19
  %23 = load ptr, ptr %5, align 8
  %24 = getelementptr inbounds nuw %struct.IOContext, ptr %23, i32 0, i32 1
  store i32 3, ptr %24, align 8
  br label %25

25:                                               ; preds = %22, %19
  store i64 0, ptr %4, align 8
  br label %41

26:                                               ; preds = %16
  %27 = load ptr, ptr %6, align 8
  %28 = load i64, ptr %7, align 8
  %29 = load ptr, ptr %5, align 8
  %30 = getelementptr inbounds nuw %struct.IOContext, ptr %29, i32 0, i32 0
  %31 = load ptr, ptr %30, align 8
  %32 = call i64 @fwrite(ptr noundef %27, i64 noundef 1, i64 noundef %28, ptr noundef %31)
  store i64 %32, ptr %8, align 8
  %33 = load i64, ptr %8, align 8
  %34 = load i64, ptr %7, align 8
  %35 = icmp ult i64 %33, %34
  br i1 %35, label %36, label %39

36:                                               ; preds = %26
  %37 = load ptr, ptr %5, align 8
  %38 = getelementptr inbounds nuw %struct.IOContext, ptr %37, i32 0, i32 1
  store i32 3, ptr %38, align 8
  br label %39

39:                                               ; preds = %36, %26
  %40 = load i64, ptr %8, align 8
  store i64 %40, ptr %4, align 8
  br label %41

41:                                               ; preds = %39, %25
  %42 = load i64, ptr %4, align 8
  ret i64 %42
}

declare dso_local i64 @fwrite(ptr noundef, i64 noundef, i64 noundef, ptr noundef) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @io_printf(ptr noundef %0, ptr noundef %1, ...) #0 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca ptr, align 8
  %7 = alloca i32, align 4
  store ptr %0, ptr %4, align 8
  store ptr %1, ptr %5, align 8
  %8 = load ptr, ptr %4, align 8
  %9 = icmp ne ptr %8, null
  br i1 %9, label %10, label %18

10:                                               ; preds = %2
  %11 = load ptr, ptr %4, align 8
  %12 = getelementptr inbounds nuw %struct.IOContext, ptr %11, i32 0, i32 0
  %13 = load ptr, ptr %12, align 8
  %14 = icmp ne ptr %13, null
  br i1 %14, label %15, label %18

15:                                               ; preds = %10
  %16 = load ptr, ptr %5, align 8
  %17 = icmp ne ptr %16, null
  br i1 %17, label %25, label %18

18:                                               ; preds = %15, %10, %2
  %19 = load ptr, ptr %4, align 8
  %20 = icmp ne ptr %19, null
  br i1 %20, label %21, label %24

21:                                               ; preds = %18
  %22 = load ptr, ptr %4, align 8
  %23 = getelementptr inbounds nuw %struct.IOContext, ptr %22, i32 0, i32 1
  store i32 3, ptr %23, align 8
  br label %24

24:                                               ; preds = %21, %18
  store i32 -1, ptr %3, align 4
  br label %39

25:                                               ; preds = %15
  call void @llvm.va_start.p0(ptr %6)
  %26 = load ptr, ptr %4, align 8
  %27 = getelementptr inbounds nuw %struct.IOContext, ptr %26, i32 0, i32 0
  %28 = load ptr, ptr %27, align 8
  %29 = load ptr, ptr %5, align 8
  %30 = load ptr, ptr %6, align 8
  %31 = call i32 @vfprintf(ptr noundef %28, ptr noundef %29, ptr noundef %30) #6
  store i32 %31, ptr %7, align 4
  call void @llvm.va_end.p0(ptr %6)
  %32 = load i32, ptr %7, align 4
  %33 = icmp slt i32 %32, 0
  br i1 %33, label %34, label %37

34:                                               ; preds = %25
  %35 = load ptr, ptr %4, align 8
  %36 = getelementptr inbounds nuw %struct.IOContext, ptr %35, i32 0, i32 1
  store i32 3, ptr %36, align 8
  br label %37

37:                                               ; preds = %34, %25
  %38 = load i32, ptr %7, align 4
  store i32 %38, ptr %3, align 4
  br label %39

39:                                               ; preds = %37, %24
  %40 = load i32, ptr %3, align 4
  ret i32 %40
}

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.va_start.p0(ptr) #3

; Function Attrs: nounwind
declare dso_local i32 @vfprintf(ptr noundef, ptr noundef, ptr noundef) #4

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.va_end.p0(ptr) #3

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @io_scanf(ptr noundef %0, ptr noundef %1, ...) #0 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca ptr, align 8
  %7 = alloca i32, align 4
  store ptr %0, ptr %4, align 8
  store ptr %1, ptr %5, align 8
  %8 = load ptr, ptr %4, align 8
  %9 = icmp ne ptr %8, null
  br i1 %9, label %10, label %18

10:                                               ; preds = %2
  %11 = load ptr, ptr %4, align 8
  %12 = getelementptr inbounds nuw %struct.IOContext, ptr %11, i32 0, i32 0
  %13 = load ptr, ptr %12, align 8
  %14 = icmp ne ptr %13, null
  br i1 %14, label %15, label %18

15:                                               ; preds = %10
  %16 = load ptr, ptr %5, align 8
  %17 = icmp ne ptr %16, null
  br i1 %17, label %25, label %18

18:                                               ; preds = %15, %10, %2
  %19 = load ptr, ptr %4, align 8
  %20 = icmp ne ptr %19, null
  br i1 %20, label %21, label %24

21:                                               ; preds = %18
  %22 = load ptr, ptr %4, align 8
  %23 = getelementptr inbounds nuw %struct.IOContext, ptr %22, i32 0, i32 1
  store i32 2, ptr %23, align 8
  br label %24

24:                                               ; preds = %21, %18
  store i32 -1, ptr %3, align 4
  br label %48

25:                                               ; preds = %15
  call void @llvm.va_start.p0(ptr %6)
  %26 = load ptr, ptr %4, align 8
  %27 = getelementptr inbounds nuw %struct.IOContext, ptr %26, i32 0, i32 0
  %28 = load ptr, ptr %27, align 8
  %29 = load ptr, ptr %5, align 8
  %30 = load ptr, ptr %6, align 8
  %31 = call i32 @vfscanf(ptr noundef %28, ptr noundef %29, ptr noundef %30)
  store i32 %31, ptr %7, align 4
  call void @llvm.va_end.p0(ptr %6)
  %32 = load i32, ptr %7, align 4
  %33 = icmp eq i32 %32, -1
  br i1 %33, label %34, label %46

34:                                               ; preds = %25
  %35 = load ptr, ptr %4, align 8
  %36 = getelementptr inbounds nuw %struct.IOContext, ptr %35, i32 0, i32 6
  store i32 1, ptr %36, align 8
  %37 = load ptr, ptr %4, align 8
  %38 = getelementptr inbounds nuw %struct.IOContext, ptr %37, i32 0, i32 0
  %39 = load ptr, ptr %38, align 8
  %40 = call i32 @ferror(ptr noundef %39)
  %41 = icmp ne i32 %40, 0
  br i1 %41, label %42, label %45

42:                                               ; preds = %34
  %43 = load ptr, ptr %4, align 8
  %44 = getelementptr inbounds nuw %struct.IOContext, ptr %43, i32 0, i32 1
  store i32 2, ptr %44, align 8
  br label %45

45:                                               ; preds = %42, %34
  br label %46

46:                                               ; preds = %45, %25
  %47 = load i32, ptr %7, align 4
  store i32 %47, ptr %3, align 4
  br label %48

48:                                               ; preds = %46, %24
  %49 = load i32, ptr %3, align 4
  ret i32 %49
}

declare dso_local i32 @vfscanf(ptr noundef, ptr noundef, ptr noundef) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @io_seek(ptr noundef %0, i32 noundef %1, i32 noundef %2) #0 {
  %4 = alloca i32, align 4
  %5 = alloca ptr, align 8
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  store ptr %0, ptr %5, align 8
  store i32 %1, ptr %6, align 4
  store i32 %2, ptr %7, align 4
  %9 = load ptr, ptr %5, align 8
  %10 = icmp ne ptr %9, null
  br i1 %10, label %11, label %16

11:                                               ; preds = %3
  %12 = load ptr, ptr %5, align 8
  %13 = getelementptr inbounds nuw %struct.IOContext, ptr %12, i32 0, i32 0
  %14 = load ptr, ptr %13, align 8
  %15 = icmp ne ptr %14, null
  br i1 %15, label %17, label %16

16:                                               ; preds = %11, %3
  store i32 5, ptr %4, align 4
  br label %37

17:                                               ; preds = %11
  %18 = load i32, ptr %7, align 4
  switch i32 %18, label %22 [
    i32 0, label %19
    i32 1, label %20
    i32 2, label %21
  ]

19:                                               ; preds = %17
  store i32 0, ptr %8, align 4
  br label %23

20:                                               ; preds = %17
  store i32 1, ptr %8, align 4
  br label %23

21:                                               ; preds = %17
  store i32 2, ptr %8, align 4
  br label %23

22:                                               ; preds = %17
  store i32 5, ptr %4, align 4
  br label %37

23:                                               ; preds = %21, %20, %19
  %24 = load ptr, ptr %5, align 8
  %25 = getelementptr inbounds nuw %struct.IOContext, ptr %24, i32 0, i32 0
  %26 = load ptr, ptr %25, align 8
  %27 = load i32, ptr %6, align 4
  %28 = load i32, ptr %8, align 4
  %29 = call i32 @fseek(ptr noundef %26, i32 noundef %27, i32 noundef %28)
  %30 = icmp ne i32 %29, 0
  br i1 %30, label %31, label %34

31:                                               ; preds = %23
  %32 = load ptr, ptr %5, align 8
  %33 = getelementptr inbounds nuw %struct.IOContext, ptr %32, i32 0, i32 1
  store i32 5, ptr %33, align 8
  store i32 5, ptr %4, align 4
  br label %37

34:                                               ; preds = %23
  %35 = load ptr, ptr %5, align 8
  %36 = getelementptr inbounds nuw %struct.IOContext, ptr %35, i32 0, i32 6
  store i32 0, ptr %36, align 8
  store i32 0, ptr %4, align 4
  br label %37

37:                                               ; preds = %34, %31, %22, %16
  %38 = load i32, ptr %4, align 4
  ret i32 %38
}

declare dso_local i32 @fseek(ptr noundef, i32 noundef, i32 noundef) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @io_tell(ptr noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
  %4 = load ptr, ptr %3, align 8
  %5 = icmp ne ptr %4, null
  br i1 %5, label %6, label %11

6:                                                ; preds = %1
  %7 = load ptr, ptr %3, align 8
  %8 = getelementptr inbounds nuw %struct.IOContext, ptr %7, i32 0, i32 0
  %9 = load ptr, ptr %8, align 8
  %10 = icmp ne ptr %9, null
  br i1 %10, label %12, label %11

11:                                               ; preds = %6, %1
  store i32 -1, ptr %2, align 4
  br label %17

12:                                               ; preds = %6
  %13 = load ptr, ptr %3, align 8
  %14 = getelementptr inbounds nuw %struct.IOContext, ptr %13, i32 0, i32 0
  %15 = load ptr, ptr %14, align 8
  %16 = call i32 @ftell(ptr noundef %15)
  store i32 %16, ptr %2, align 4
  br label %17

17:                                               ; preds = %12, %11
  %18 = load i32, ptr %2, align 4
  ret i32 %18
}

declare dso_local i32 @ftell(ptr noundef) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @io_eof(ptr noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
  %4 = load ptr, ptr %3, align 8
  %5 = icmp ne ptr %4, null
  br i1 %5, label %7, label %6

6:                                                ; preds = %1
  store i32 1, ptr %2, align 4
  br label %21

7:                                                ; preds = %1
  %8 = load ptr, ptr %3, align 8
  %9 = getelementptr inbounds nuw %struct.IOContext, ptr %8, i32 0, i32 6
  %10 = load i32, ptr %9, align 8
  %11 = icmp ne i32 %10, 0
  br i1 %11, label %18, label %12

12:                                               ; preds = %7
  %13 = load ptr, ptr %3, align 8
  %14 = getelementptr inbounds nuw %struct.IOContext, ptr %13, i32 0, i32 0
  %15 = load ptr, ptr %14, align 8
  %16 = call i32 @feof(ptr noundef %15)
  %17 = icmp ne i32 %16, 0
  br label %18

18:                                               ; preds = %12, %7
  %19 = phi i1 [ true, %7 ], [ %17, %12 ]
  %20 = zext i1 %19 to i32
  store i32 %20, ptr %2, align 4
  br label %21

21:                                               ; preds = %18, %6
  %22 = load i32, ptr %2, align 4
  ret i32 %22
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @io_rewind(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  %3 = load ptr, ptr %2, align 8
  %4 = icmp ne ptr %3, null
  br i1 %4, label %5, label %16

5:                                                ; preds = %1
  %6 = load ptr, ptr %2, align 8
  %7 = getelementptr inbounds nuw %struct.IOContext, ptr %6, i32 0, i32 0
  %8 = load ptr, ptr %7, align 8
  %9 = icmp ne ptr %8, null
  br i1 %9, label %10, label %16

10:                                               ; preds = %5
  %11 = load ptr, ptr %2, align 8
  %12 = getelementptr inbounds nuw %struct.IOContext, ptr %11, i32 0, i32 0
  %13 = load ptr, ptr %12, align 8
  call void @rewind(ptr noundef %13)
  %14 = load ptr, ptr %2, align 8
  %15 = getelementptr inbounds nuw %struct.IOContext, ptr %14, i32 0, i32 6
  store i32 0, ptr %15, align 8
  br label %16

16:                                               ; preds = %10, %5, %1
  ret void
}

declare dso_local void @rewind(ptr noundef) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @io_error(ptr noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
  %4 = load ptr, ptr %3, align 8
  %5 = icmp ne ptr %4, null
  br i1 %5, label %7, label %6

6:                                                ; preds = %1
  store i32 -1, ptr %2, align 4
  br label %11

7:                                                ; preds = %1
  %8 = load ptr, ptr %3, align 8
  %9 = getelementptr inbounds nuw %struct.IOContext, ptr %8, i32 0, i32 1
  %10 = load i32, ptr %9, align 8
  store i32 %10, ptr %2, align 4
  br label %11

11:                                               ; preds = %7, %6
  %12 = load i32, ptr %2, align 4
  ret i32 %12
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @io_error_string(i32 noundef %0) #0 {
  %2 = alloca ptr, align 8
  %3 = alloca i32, align 4
  store i32 %0, ptr %3, align 4
  %4 = load i32, ptr %3, align 4
  %5 = icmp slt i32 %4, 0
  br i1 %5, label %10, label %6

6:                                                ; preds = %1
  %7 = load i32, ptr %3, align 4
  %8 = sext i32 %7 to i64
  %9 = icmp uge i64 %8, 7
  br i1 %9, label %10, label %11

10:                                               ; preds = %6, %1
  store ptr @.str.8, ptr %2, align 8
  br label %16

11:                                               ; preds = %6
  %12 = load i32, ptr %3, align 4
  %13 = sext i32 %12 to i64
  %14 = getelementptr inbounds [7 x ptr], ptr @error_messages, i64 0, i64 %13
  %15 = load ptr, ptr %14, align 8
  store ptr %15, ptr %2, align 8
  br label %16

16:                                               ; preds = %11, %10
  %17 = load ptr, ptr %2, align 8
  ret ptr %17
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @io_clear_error(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  %3 = load ptr, ptr %2, align 8
  %4 = icmp ne ptr %3, null
  br i1 %4, label %5, label %11

5:                                                ; preds = %1
  %6 = load ptr, ptr %2, align 8
  %7 = getelementptr inbounds nuw %struct.IOContext, ptr %6, i32 0, i32 1
  store i32 0, ptr %7, align 8
  %8 = load ptr, ptr %2, align 8
  %9 = getelementptr inbounds nuw %struct.IOContext, ptr %8, i32 0, i32 0
  %10 = load ptr, ptr %9, align 8
  call void @clearerr(ptr noundef %10)
  br label %11

11:                                               ; preds = %5, %1
  ret void
}

declare dso_local void @clearerr(ptr noundef) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i64 @io_get_file_size(ptr noundef %0) #0 {
  %2 = alloca i64, align 8
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store ptr %0, ptr %3, align 8
  %6 = load ptr, ptr %3, align 8
  %7 = icmp ne ptr %6, null
  br i1 %7, label %8, label %13

8:                                                ; preds = %1
  %9 = load ptr, ptr %3, align 8
  %10 = getelementptr inbounds nuw %struct.IOContext, ptr %9, i32 0, i32 0
  %11 = load ptr, ptr %10, align 8
  %12 = icmp ne ptr %11, null
  br i1 %12, label %14, label %13

13:                                               ; preds = %8, %1
  store i64 0, ptr %2, align 8
  br label %50

14:                                               ; preds = %8
  %15 = load ptr, ptr %3, align 8
  %16 = getelementptr inbounds nuw %struct.IOContext, ptr %15, i32 0, i32 0
  %17 = load ptr, ptr %16, align 8
  %18 = call i32 @ftell(ptr noundef %17)
  store i32 %18, ptr %4, align 4
  %19 = load i32, ptr %4, align 4
  %20 = icmp slt i32 %19, 0
  br i1 %20, label %21, label %22

21:                                               ; preds = %14
  store i64 0, ptr %2, align 8
  br label %50

22:                                               ; preds = %14
  %23 = load ptr, ptr %3, align 8
  %24 = getelementptr inbounds nuw %struct.IOContext, ptr %23, i32 0, i32 0
  %25 = load ptr, ptr %24, align 8
  %26 = call i32 @fseek(ptr noundef %25, i32 noundef 0, i32 noundef 2)
  %27 = icmp ne i32 %26, 0
  br i1 %27, label %28, label %29

28:                                               ; preds = %22
  store i64 0, ptr %2, align 8
  br label %50

29:                                               ; preds = %22
  %30 = load ptr, ptr %3, align 8
  %31 = getelementptr inbounds nuw %struct.IOContext, ptr %30, i32 0, i32 0
  %32 = load ptr, ptr %31, align 8
  %33 = call i32 @ftell(ptr noundef %32)
  store i32 %33, ptr %5, align 4
  %34 = load i32, ptr %5, align 4
  %35 = icmp slt i32 %34, 0
  br i1 %35, label %36, label %42

36:                                               ; preds = %29
  %37 = load ptr, ptr %3, align 8
  %38 = getelementptr inbounds nuw %struct.IOContext, ptr %37, i32 0, i32 0
  %39 = load ptr, ptr %38, align 8
  %40 = load i32, ptr %4, align 4
  %41 = call i32 @fseek(ptr noundef %39, i32 noundef %40, i32 noundef 0)
  store i64 0, ptr %2, align 8
  br label %50

42:                                               ; preds = %29
  %43 = load ptr, ptr %3, align 8
  %44 = getelementptr inbounds nuw %struct.IOContext, ptr %43, i32 0, i32 0
  %45 = load ptr, ptr %44, align 8
  %46 = load i32, ptr %4, align 4
  %47 = call i32 @fseek(ptr noundef %45, i32 noundef %46, i32 noundef 0)
  %48 = load i32, ptr %5, align 4
  %49 = sext i32 %48 to i64
  store i64 %49, ptr %2, align 8
  br label %50

50:                                               ; preds = %42, %36, %28, %21, %13
  %51 = load i64, ptr %2, align 8
  ret i64 %51
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @io_copy_file(ptr noundef %0, ptr noundef %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca ptr, align 8
  %7 = alloca ptr, align 8
  %8 = alloca [4096 x i8], align 16
  %9 = alloca i64, align 8
  %10 = alloca i32, align 4
  store ptr %0, ptr %4, align 8
  store ptr %1, ptr %5, align 8
  store ptr null, ptr %6, align 8
  store ptr null, ptr %7, align 8
  store i32 0, ptr %10, align 4
  %11 = load ptr, ptr %4, align 8
  %12 = call ptr @io_open(ptr noundef %11, i32 noundef 9)
  store ptr %12, ptr %6, align 8
  %13 = load ptr, ptr %6, align 8
  %14 = icmp ne ptr %13, null
  br i1 %14, label %16, label %15

15:                                               ; preds = %2
  store i32 1, ptr %3, align 4
  br label %59

16:                                               ; preds = %2
  %17 = load ptr, ptr %5, align 8
  %18 = call ptr @io_open(ptr noundef %17, i32 noundef 10)
  store ptr %18, ptr %7, align 8
  %19 = load ptr, ptr %7, align 8
  %20 = icmp ne ptr %19, null
  br i1 %20, label %22, label %21

21:                                               ; preds = %16
  store i32 1, ptr %10, align 4
  br label %45

22:                                               ; preds = %16
  br label %23

23:                                               ; preds = %36, %22
  %24 = load ptr, ptr %6, align 8
  %25 = getelementptr inbounds [4096 x i8], ptr %8, i64 0, i64 0
  %26 = call i64 @io_read(ptr noundef %24, ptr noundef %25, i64 noundef 4096)
  store i64 %26, ptr %9, align 8
  %27 = icmp ugt i64 %26, 0
  br i1 %27, label %28, label %37

28:                                               ; preds = %23
  %29 = load ptr, ptr %7, align 8
  %30 = getelementptr inbounds [4096 x i8], ptr %8, i64 0, i64 0
  %31 = load i64, ptr %9, align 8
  %32 = call i64 @io_write(ptr noundef %29, ptr noundef %30, i64 noundef %31)
  %33 = load i64, ptr %9, align 8
  %34 = icmp ne i64 %32, %33
  br i1 %34, label %35, label %36

35:                                               ; preds = %28
  store i32 3, ptr %10, align 4
  br label %45

36:                                               ; preds = %28
  br label %23, !llvm.loop !8

37:                                               ; preds = %23
  %38 = load ptr, ptr %6, align 8
  %39 = call i32 @io_error(ptr noundef %38)
  %40 = icmp ne i32 %39, 0
  br i1 %40, label %41, label %44

41:                                               ; preds = %37
  %42 = load ptr, ptr %6, align 8
  %43 = call i32 @io_error(ptr noundef %42)
  store i32 %43, ptr %10, align 4
  br label %44

44:                                               ; preds = %41, %37
  br label %45

45:                                               ; preds = %44, %35, %21
  %46 = load ptr, ptr %6, align 8
  %47 = icmp ne ptr %46, null
  br i1 %47, label %48, label %51

48:                                               ; preds = %45
  %49 = load ptr, ptr %6, align 8
  %50 = call i32 @io_close(ptr noundef %49)
  br label %51

51:                                               ; preds = %48, %45
  %52 = load ptr, ptr %7, align 8
  %53 = icmp ne ptr %52, null
  br i1 %53, label %54, label %57

54:                                               ; preds = %51
  %55 = load ptr, ptr %7, align 8
  %56 = call i32 @io_close(ptr noundef %55)
  br label %57

57:                                               ; preds = %54, %51
  %58 = load i32, ptr %10, align 4
  store i32 %58, ptr %3, align 4
  br label %59

59:                                               ; preds = %57, %15
  %60 = load i32, ptr %3, align 4
  ret i32 %60
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @io_file_exists(ptr noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
  %5 = load ptr, ptr %3, align 8
  %6 = call ptr @fopen(ptr noundef %5, ptr noundef @.str.3)
  store ptr %6, ptr %4, align 8
  %7 = load ptr, ptr %4, align 8
  %8 = icmp ne ptr %7, null
  br i1 %8, label %9, label %12

9:                                                ; preds = %1
  %10 = load ptr, ptr %4, align 8
  %11 = call i32 @fclose(ptr noundef %10)
  store i32 1, ptr %2, align 4
  br label %13

12:                                               ; preds = %1
  store i32 0, ptr %2, align 4
  br label %13

13:                                               ; preds = %12, %9
  %14 = load i32, ptr %2, align 4
  ret i32 %14
}

attributes #0 = { noinline nounwind optnone uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { allocsize(0,1) "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nocallback nofree nosync nounwind willreturn }
attributes #4 = { nounwind "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { allocsize(0,1) }
attributes #6 = { nounwind }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3, !4, !5, !6}
!llvm.ident = !{!7}

!0 = distinct !DICompileUnit(language: DW_LANG_C11, file: !1, producer: "clang version 22.1.8 (https://github.com/msys2/MINGW-packages 6e4e79c2f86eeb534e324e583f2057dc9fd5ecab)", isOptimized: false, runtimeVersion: 0, emissionKind: NoDebug, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "myio.c", directory: "I:/msys64/home/Administrator/falloy/lib")
!2 = !{i32 2, !"Debug Info Version", i32 3}
!3 = !{i32 1, !"wchar_size", i32 2}
!4 = !{i32 8, !"PIC Level", i32 2}
!5 = !{i32 7, !"uwtable", i32 2}
!6 = !{i32 1, !"MaxTLSAlign", i32 65536}
!7 = !{!"clang version 22.1.8 (https://github.com/msys2/MINGW-packages 6e4e79c2f86eeb534e324e583f2057dc9fd5ecab)"}
!8 = distinct !{!8, !9}
!9 = !{!"llvm.loop.mustprogress"}
