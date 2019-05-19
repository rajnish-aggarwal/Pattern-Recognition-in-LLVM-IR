; ModuleID = 'listing2-m2r.bc'
source_filename = "listing2.c"
target datalayout = "e-m:e-p:32:32-f64:32:64-f80:32-n8:16:32-S128"
target triple = "i686-pc-linux-gnu"

; Function Attrs: noinline nounwind
define i32 @main() #0 {
entry:
  %a = alloca i32*, align 4
  %b = alloca i32*, align 4
  %c = alloca i32*, align 4
  %0 = bitcast i32** %a to i8**
  %call = call i32 @posix_memalign(i8** %0, i32 32, i32 4000) #2
  %1 = bitcast i32** %b to i8**
  %call1 = call i32 @posix_memalign(i8** %1, i32 32, i32 4000) #2
  %2 = bitcast i32** %c to i8**
  %call2 = call i32 @posix_memalign(i8** %2, i32 32, i32 40000) #2
  br label %for.cond

for.cond:                                         ; preds = %for.inc8, %entry
  %i.0 = phi i32 [ 0, %entry ], [ %inc9, %for.inc8 ]
  %cmp = icmp slt i32 %i.0, 100
  br i1 %cmp, label %for.body, label %for.end10

for.body:                                         ; preds = %for.cond
  br label %for.cond3

for.cond3:                                        ; preds = %for.inc, %for.body
  %j.0 = phi i32 [ 0, %for.body ], [ %inc, %for.inc ]
  %cmp4 = icmp slt i32 %j.0, 10
  br i1 %cmp4, label %for.body5, label %for.end

for.body5:                                        ; preds = %for.cond3
  %mul = mul nsw i32 %i.0, 10
  %add = add nsw i32 %mul, %j.0
  %3 = load i32*, i32** %a, align 4
  %mul6 = mul nsw i32 %i.0, 10
  %add.ptr = getelementptr inbounds i32, i32* %3, i32 %mul6
  %add.ptr7 = getelementptr inbounds i32, i32* %add.ptr, i32 %j.0
  store i32 %add, i32* %add.ptr7, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body5
  %inc = add nsw i32 %j.0, 1
  br label %for.cond3

for.end:                                          ; preds = %for.cond3
  br label %for.inc8

for.inc8:                                         ; preds = %for.end
  %inc9 = add nsw i32 %i.0, 1
  br label %for.cond

for.end10:                                        ; preds = %for.cond
  br label %for.cond12

for.cond12:                                       ; preds = %for.inc27, %for.end10
  %i11.0 = phi i32 [ 0, %for.end10 ], [ %inc28, %for.inc27 ]
  %cmp13 = icmp slt i32 %i11.0, 10
  br i1 %cmp13, label %for.body14, label %for.end29

for.body14:                                       ; preds = %for.cond12
  br label %for.cond16

for.cond16:                                       ; preds = %for.inc24, %for.body14
  %j15.0 = phi i32 [ 0, %for.body14 ], [ %inc25, %for.inc24 ]
  %cmp17 = icmp slt i32 %j15.0, 100
  br i1 %cmp17, label %for.body18, label %for.end26

for.body18:                                       ; preds = %for.cond16
  %mul19 = mul nsw i32 %i11.0, 100
  %add20 = add nsw i32 %mul19, %j15.0
  %4 = load i32*, i32** %b, align 4
  %mul21 = mul nsw i32 %i11.0, 100
  %add.ptr22 = getelementptr inbounds i32, i32* %4, i32 %mul21
  %add.ptr23 = getelementptr inbounds i32, i32* %add.ptr22, i32 %j15.0
  store i32 %add20, i32* %add.ptr23, align 4
  br label %for.inc24

for.inc24:                                        ; preds = %for.body18
  %inc25 = add nsw i32 %j15.0, 1
  br label %for.cond16

for.end26:                                        ; preds = %for.cond16
  br label %for.inc27

for.inc27:                                        ; preds = %for.end26
  %inc28 = add nsw i32 %i11.0, 1
  br label %for.cond12

for.end29:                                        ; preds = %for.cond12
  br label %for.cond31

for.cond31:                                       ; preds = %for.inc58, %for.end29
  %i30.0 = phi i32 [ 0, %for.end29 ], [ %inc59, %for.inc58 ]
  %cmp32 = icmp slt i32 %i30.0, 100
  br i1 %cmp32, label %for.body33, label %for.end60

for.body33:                                       ; preds = %for.cond31
  br label %for.cond35

for.cond35:                                       ; preds = %for.inc55, %for.body33
  %j34.0 = phi i32 [ 0, %for.body33 ], [ %inc56, %for.inc55 ]
  %cmp36 = icmp slt i32 %j34.0, 100
  br i1 %cmp36, label %for.body37, label %for.end57

for.body37:                                       ; preds = %for.cond35
  br label %for.cond38

for.cond38:                                       ; preds = %for.inc52, %for.body37
  %k.0 = phi i32 [ 0, %for.body37 ], [ %inc53, %for.inc52 ]
  %cmp39 = icmp slt i32 %k.0, 10
  br i1 %cmp39, label %for.body40, label %for.end54

for.body40:                                       ; preds = %for.cond38
  %5 = load i32*, i32** %a, align 4
  %mul41 = mul nsw i32 %i30.0, 10
  %add.ptr42 = getelementptr inbounds i32, i32* %5, i32 %mul41
  %add.ptr43 = getelementptr inbounds i32, i32* %add.ptr42, i32 %k.0
  %6 = load i32, i32* %add.ptr43, align 4
  %7 = load i32*, i32** %b, align 4
  %mul44 = mul nsw i32 %k.0, 100
  %add.ptr45 = getelementptr inbounds i32, i32* %7, i32 %mul44
  %add.ptr46 = getelementptr inbounds i32, i32* %add.ptr45, i32 %j34.0
  %8 = load i32, i32* %add.ptr46, align 4
  %mul47 = mul nsw i32 %6, %8
  %9 = load i32*, i32** %c, align 4
  %mul48 = mul nsw i32 %i30.0, 100
  %add.ptr49 = getelementptr inbounds i32, i32* %9, i32 %mul48
  %add.ptr50 = getelementptr inbounds i32, i32* %add.ptr49, i32 %j34.0
  %10 = load i32, i32* %add.ptr50, align 4
  %add51 = add nsw i32 %10, %mul47
  store i32 %add51, i32* %add.ptr50, align 4
  br label %for.inc52

for.inc52:                                        ; preds = %for.body40
  %inc53 = add nsw i32 %k.0, 1
  br label %for.cond38

for.end54:                                        ; preds = %for.cond38
  br label %for.inc55

for.inc55:                                        ; preds = %for.end54
  %inc56 = add nsw i32 %j34.0, 1
  br label %for.cond35

for.end57:                                        ; preds = %for.cond35
  br label %for.inc58

for.inc58:                                        ; preds = %for.end57
  %inc59 = add nsw i32 %i30.0, 1
  br label %for.cond31

for.end60:                                        ; preds = %for.cond31
  ret i32 0
}

; Function Attrs: nounwind
declare i32 @posix_memalign(i8**, i32, i32) #1

attributes #0 = { noinline nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="skylake" "target-features"="+aes,+avx,+avx2,+clflushopt,+cmov,+cx16,+fma,+fsgsbase,+fxsr,+lzcnt,+mmx,+movbe,+mpx,+pclmul,+popcnt,+prfchw,+rdrnd,+rdseed,+sse,+sse2,+sse3,+sse4.1,+sse4.2,+ssse3,+x87,+xsave,-adx,-avx512bw,-avx512cd,-avx512dq,-avx512er,-avx512f,-avx512ifma,-avx512pf,-avx512vbmi,-avx512vl,-avx512vpopcntdq,-bmi,-bmi2,-clwb,-clzero,-f16c,-fma4,-lwp,-mwaitx,-pku,-prefetchwt1,-rtm,-sgx,-sha,-sse4a,-tbm,-xop,-xsavec,-xsaveopt,-xsaves" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="skylake" "target-features"="+aes,+avx,+avx2,+clflushopt,+cmov,+cx16,+fma,+fsgsbase,+fxsr,+lzcnt,+mmx,+movbe,+mpx,+pclmul,+popcnt,+prfchw,+rdrnd,+rdseed,+sse,+sse2,+sse3,+sse4.1,+sse4.2,+ssse3,+x87,+xsave,-adx,-avx512bw,-avx512cd,-avx512dq,-avx512er,-avx512f,-avx512ifma,-avx512pf,-avx512vbmi,-avx512vl,-avx512vpopcntdq,-bmi,-bmi2,-clwb,-clzero,-f16c,-fma4,-lwp,-mwaitx,-pku,-prefetchwt1,-rtm,-sgx,-sha,-sse4a,-tbm,-xop,-xsavec,-xsaveopt,-xsaves" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"NumRegisterParameters", i32 0}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{!"clang version 5.0.1 (tags/RELEASE_501/final)"}
