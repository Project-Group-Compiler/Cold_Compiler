; ModuleID = 'static.c'
source_filename = "static.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@increment.count = internal global i32 4, align 4
@count = internal global i32 5, align 4

; Function Attrs: noinline nounwind optnone
define dso_local void @increment() #0 {
entry:
  %0 = load i32, ptr @increment.count, align 4
  %add = add nsw i32 %0, 9
  store i32 %add, ptr @increment.count, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone
define dso_local i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  store i32 0, ptr %retval, align 4
  call void @increment()
  call void @increment()
  call void @increment()
  %0 = load i32, ptr @count, align 4
  %inc = add nsw i32 %0, 1
  store i32 %inc, ptr @count, align 4
  ret i32 0
}

attributes #0 = { noinline nounwind optnone "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+cx8,+mmx,+sse,+sse2,+x87" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"Ubuntu clang version 18.1.3 (1ubuntu1)"}
