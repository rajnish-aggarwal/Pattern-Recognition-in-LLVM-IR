; ModuleID = 'listing1-m2r.bc'
source_filename = "listing1.c"
target datalayout = "e-m:e-p:32:32-f64:32:64-f80:32-n8:16:32-S128"
target triple = "i686-pc-linux-gnu"

; Function Attrs: noinline nounwind
define i32 @main() #0 {
entry:
  %a = alloca [100 x [10 x i32]], align 4
  %b = alloca [10 x [100 x i32]], align 4
  %c = alloca [100 x [100 x i32]], align 4
  %0 = bitcast [100 x [100 x i32]]* %c to i8*
  call void @llvm.memset.p0i8.i32(i8* %0, i8 0, i32 40000, i32 4, i1 false)
  br label %for.cond

for.cond:                                         ; preds = %for.inc5, %entry
  %i.0 = phi i32 [ 0, %entry ], [ %inc6, %for.inc5 ]
  %cmp = icmp slt i32 %i.0, 100
  br i1 %cmp, label %for.body, label %for.end7

for.body:                                         ; preds = %for.cond
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %j.0 = phi i32 [ 0, %for.body ], [ %inc, %for.inc ]
  %cmp2 = icmp slt i32 %j.0, 10
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %mul = mul nsw i32 %i.0, 10
  %add = add nsw i32 %mul, %j.0
  %arrayidx = getelementptr inbounds [100 x [10 x i32]], [100 x [10 x i32]]* %a, i32 0, i32 %i.0
  %arrayidx4 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx, i32 0, i32 %j.0
  store i32 %add, i32* %arrayidx4, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %inc = add nsw i32 %j.0, 1
  br label %for.cond1

for.end:                                          ; preds = %for.cond1
  br label %for.inc5

for.inc5:                                         ; preds = %for.end
  %inc6 = add nsw i32 %i.0, 1
  br label %for.cond

for.end7:                                         ; preds = %for.cond
  br label %for.cond9

for.cond9:                                        ; preds = %for.inc23, %for.end7
  %i8.0 = phi i32 [ 0, %for.end7 ], [ %inc24, %for.inc23 ]
  %cmp10 = icmp slt i32 %i8.0, 10
  br i1 %cmp10, label %for.body11, label %for.end25

for.body11:                                       ; preds = %for.cond9
  br label %for.cond13

for.cond13:                                       ; preds = %for.inc20, %for.body11
  %j12.0 = phi i32 [ 0, %for.body11 ], [ %inc21, %for.inc20 ]
  %cmp14 = icmp slt i32 %j12.0, 100
  br i1 %cmp14, label %for.body15, label %for.end22

for.body15:                                       ; preds = %for.cond13
  %mul16 = mul nsw i32 %i8.0, 10
  %add17 = add nsw i32 %mul16, %j12.0
  %arrayidx18 = getelementptr inbounds [10 x [100 x i32]], [10 x [100 x i32]]* %b, i32 0, i32 %i8.0
  %arrayidx19 = getelementptr inbounds [100 x i32], [100 x i32]* %arrayidx18, i32 0, i32 %j12.0
  store i32 %add17, i32* %arrayidx19, align 4
  br label %for.inc20

for.inc20:                                        ; preds = %for.body15
  %inc21 = add nsw i32 %j12.0, 1
  br label %for.cond13

for.end22:                                        ; preds = %for.cond13
  br label %for.inc23

for.inc23:                                        ; preds = %for.end22
  %inc24 = add nsw i32 %i8.0, 1
  br label %for.cond9

for.end25:                                        ; preds = %for.cond9
  br label %for.cond27

for.cond27:                                       ; preds = %for.inc51, %for.end25
  %i26.0 = phi i32 [ 0, %for.end25 ], [ %inc52, %for.inc51 ]
  %cmp28 = icmp slt i32 %i26.0, 100
  br i1 %cmp28, label %for.body29, label %for.end53

for.body29:                                       ; preds = %for.cond27
  br label %for.cond31

for.cond31:                                       ; preds = %for.inc48, %for.body29
  %j30.0 = phi i32 [ 0, %for.body29 ], [ %inc49, %for.inc48 ]
  %cmp32 = icmp slt i32 %j30.0, 100
  br i1 %cmp32, label %for.body33, label %for.end50

for.body33:                                       ; preds = %for.cond31
  br label %for.cond34

for.cond34:                                       ; preds = %for.inc45, %for.body33
  %k.0 = phi i32 [ 0, %for.body33 ], [ %inc46, %for.inc45 ]
  %cmp35 = icmp slt i32 %k.0, 10
  br i1 %cmp35, label %for.body36, label %for.end47

for.body36:                                       ; preds = %for.cond34
  %arrayidx37 = getelementptr inbounds [100 x [10 x i32]], [100 x [10 x i32]]* %a, i32 0, i32 %i26.0
  %arrayidx38 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx37, i32 0, i32 %k.0
  %1 = load i32, i32* %arrayidx38, align 4
  %arrayidx39 = getelementptr inbounds [10 x [100 x i32]], [10 x [100 x i32]]* %b, i32 0, i32 %k.0
  %arrayidx40 = getelementptr inbounds [100 x i32], [100 x i32]* %arrayidx39, i32 0, i32 %j30.0
  %2 = load i32, i32* %arrayidx40, align 4
  %mul41 = mul nsw i32 %1, %2
  %arrayidx42 = getelementptr inbounds [100 x [100 x i32]], [100 x [100 x i32]]* %c, i32 0, i32 %i26.0
  %arrayidx43 = getelementptr inbounds [100 x i32], [100 x i32]* %arrayidx42, i32 0, i32 %j30.0
  %3 = load i32, i32* %arrayidx43, align 4
  %add44 = add nsw i32 %3, %mul41
  store i32 %add44, i32* %arrayidx43, align 4
  br label %for.inc45

for.inc45:                                        ; preds = %for.body36
  %inc46 = add nsw i32 %k.0, 1
  br label %for.cond34

for.end47:                                        ; preds = %for.cond34
  br label %for.inc48

for.inc48:                                        ; preds = %for.end47
  %inc49 = add nsw i32 %j30.0, 1
  br label %for.cond31

for.end50:                                        ; preds = %for.cond31
  br label %for.inc51

for.inc51:                                        ; preds = %for.end50
  %inc52 = add nsw i32 %i26.0, 1
  br label %for.cond27

for.end53:                                        ; preds = %for.cond27
  ret i32 0
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i32(i8* nocapture writeonly, i8, i32, i32, i1) #1

attributes #0 = { noinline nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="skylake" "target-features"="+aes,+avx,+avx2,+clflushopt,+cmov,+cx16,+fma,+fsgsbase,+fxsr,+lzcnt,+mmx,+movbe,+mpx,+pclmul,+popcnt,+prfchw,+rdrnd,+rdseed,+sse,+sse2,+sse3,+sse4.1,+sse4.2,+ssse3,+x87,+xsave,-adx,-avx512bw,-avx512cd,-avx512dq,-avx512er,-avx512f,-avx512ifma,-avx512pf,-avx512vbmi,-avx512vl,-avx512vpopcntdq,-bmi,-bmi2,-clwb,-clzero,-f16c,-fma4,-lwp,-mwaitx,-pku,-prefetchwt1,-rtm,-sgx,-sha,-sse4a,-tbm,-xop,-xsavec,-xsaveopt,-xsaves" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"NumRegisterParameters", i32 0}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{!"clang version 5.0.1 (tags/RELEASE_501/final)"}
