in=`basename $1 .b`
./bf2 $1
as -o ${in}.o ${in}.s
ld -o ${in} ${in}.o
