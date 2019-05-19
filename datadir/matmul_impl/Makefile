gcc_mat_mul:
	gcc -o gcc_mat_mul.x mat_mul.c  -fopt-info-vec-optimized -fprofile-use -O3
	./gcc_mat_mul.x

polly_mat_mul:
	clang -O3 -mllvm -polly -mllvm -polly-vectorizer=stripmine mat_mul.c  -o polly_mat_mul.x
	./polly_mat_mul.x

gcc_reg_mat_mul:
	gcc -o gcc_reg_mat_mul.x reg_mat_mul.c  -fopt-info-vec-optimized -fprofile-use -O3
	./gcc_reg_mat_mul.x

polly_reg_mat_mul:
	clang -O3 -mllvm -polly -mllvm -polly-vectorizer=stripmine reg_mat_mul.c  -o polly_reg_mat_mul.x
	./polly_reg_mat_mul.x

gcc_rev_mat_mul:
	gcc -o gcc_rev_mat_mul.x loop_reversed.c  -fopt-info-vec-optimized -fprofile-use -O3
	./gcc_rev_mat_mul.x

polly_rev_mat_mul:
	clang -O3 -mllvm -polly -mllvm -polly-vectorizer=stripmine loop_reversed.c  -o polly_rev_mat_mul.x
	./polly_rev_mat_mul.x

gcc_reg_rev_mat_mul:
	gcc -o gcc_reg_rev_mat_mul.x reg_loop_reversed.c  -fopt-info-vec-optimized -fprofile-use -O3
	./gcc_reg_rev_mat_mul.x

polly_reg_rev_mat_mul:
	clang -O3 -mllvm -polly -mllvm -polly-vectorizer=stripmine reg_loop_reversed.c  -o polly_reg_rev_mat_mul.x
	./polly_reg_rev_mat_mul.x


gcc_blocked_mat_mul:
	gcc -o gcc_blocked_mat_mul.x blocked_mat_mul.c  -fopt-info-vec-optimized -fprofile-use -O3
	./gcc_blocked_mat_mul.x

polly_blocked_mat_mul:
	clang -O3 -mllvm -polly -mllvm -polly-vectorizer=stripmine blocked_mat_mul.c  -o polly_blocked_mat_mul.x
	./polly_blocked_mat_mul.x

clean:
	rm -rf *.x op *.txt
