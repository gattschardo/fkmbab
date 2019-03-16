die() { echo "$*"; exit 1; }

test_correctness()
{
	for i in `seq -1 16`
	do
		./batch $i > stdout 2> stderr
		diff stdout expected/$i.stdout || die "fail $i stdout"
		diff stderr expected/$i.stderr || die "fail $i stderr"
		echo "$i ok"
	done
	rm stdout stderr 
}

test_safety()
{
	for i in `seq -2 9`
	do
		valgrind --leak-check=yes --error-exitcode=69 ./batch $i
		if [ $? -eq 69 ]
		then
			die "valgrind error on $i"
		fi
	done
}

test_correctness
test_safety
