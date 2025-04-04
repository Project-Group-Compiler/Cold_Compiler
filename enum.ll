

@x = global i32 0, align 4

; Function Attrs: noinline nounwind optnone
define dso_local i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %myColor1 = alloca i32, align 4
  %error1 = alloca i32, align 4
  %error2 = alloca i32, align 4
  store i32 0, ptr %retval, align 4
  store i32 1, ptr %myColor1, align 4
  store i32 205, ptr %error1, align 4
  store i32 101, ptr %error2, align 4
  %0 = load i32, ptr %myColor1, align 4
  switch i32 %0, label %sw.epilog [
    i32 0, label %sw.bb
    i32 1, label %sw.bb1
    i32 2, label %sw.bb2
  ]

sw.bb:                                            ; preds = %entry
  store i32 0, ptr @x, align 4
  br label %sw.epilog

sw.bb1:                                           ; preds = %entry
  store i32 1, ptr @x, align 4
  br label %sw.epilog

sw.bb2:                                           ; preds = %entry
  store i32 2, ptr @x, align 4
  br label %sw.epilog

sw.epilog:                                        ; preds = %entry, %sw.bb2, %sw.bb1, %sw.bb
  store i32 2, ptr @x, align 4
  %1 = load i32, ptr %error1, align 4
  %cmp = icmp eq i32 %1, 205
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %sw.epilog
  %2 = load i32, ptr @x, align 4
  %inc = add nsw i32 %2, 1
  store i32 %inc, ptr @x, align 4
  br label %if.end

if.else:                                          ; preds = %sw.epilog
  %3 = load i32, ptr @x, align 4
  %dec = add nsw i32 %3, -1
  store i32 %dec, ptr @x, align 4
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  ret i32 0
}

attributes #0 = { noinline nounwind optnone "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+cx8,+mmx,+sse,+sse2,+x87" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"Ubuntu clang version 18.1.3 (1ubuntu1)"}
