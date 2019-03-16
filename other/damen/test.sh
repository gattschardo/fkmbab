die() { echo "$*"; exit 1; }

for i in `seq -1 16`
do
	./batch $i > stdout 2> stderr
	diff stdout expected/$i.stdout || die "fail $i stdout"
	diff stderr expected/$i.stderr || die "fail $i stderr"
	echo "$i ok"
done

rm stdout stderr
