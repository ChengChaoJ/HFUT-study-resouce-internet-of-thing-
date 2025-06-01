define dso_local i32 @main() {
bb0:
        %r0 = alloca i32, align 4
        store i32 5, i32* %r0
        %r1 = alloca i32, align 4
        store i32 10, i32* %r1
        store i32 2, i32* %r1
        %r2 = load i32, i32* %r1
        %r3 = icmp eq i32 %r2,2
        br i1 %r3,label %bb2,label %bb4
bb1:
        %r9 = phi i32 [ %r8,%bb5 ], [ 0,%bb6 ]
        ret i32 %r9
bb2:
        %r4 = load i32, i32* %r0
        %r5 = icmp eq i32 %r4,5
        br i1 %r5,label %bb3,label %bb4
bb3:
        store i32 3, i32* %r0
        br label %bb5
bb4:
        store i32 4, i32* %r0
        br label %bb5
bb5:
        %r6 = load i32, i32* %r0
        %r7 = load i32, i32* %r1
        %r8 = mul nsw i32 %r6, %r7
        br label %bb1
bb6:
        br label %bb1
}
