# clang -emit-llvm -S main.c -o main.ll
cd /coursegrader/testdata/submit/src/ir/run
llvm-as main.ll -o main.bc
lli main.bc
echo $?
