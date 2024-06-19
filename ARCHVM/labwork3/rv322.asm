addi   	s11, zero, 64 // M
addi   	s10, zero, 60 // N
addi   	s9, zero, 32 // K
addi   	t0, zero, 0x100 // a start == pa
addi   	a7, zero, 1152 //
add    	a7, a7, a7 // b start
mul    	a6, s11, s10 // K*N
add    	a6, a6, a6 // 2*K*N
add    	t2, a7, a6 // c start == pc
addi   	a0, zero, 0 // for 0 <= y < M
addi   	a1, zero, 0 // for 0 <= x < N
add    	t1, zero, a7 // b start == pb
addi   	t6, zero, 0 // s = 0
addi   	a3, zero, 0 // for 0 <= k < K
add    	a2, t0, a3 // pa[k]
add    	a4, t1, a1 //
add    	a4, a4, a1 // pb[x]
lb     	s2, 0, a2 // load pa[k]
lh     	s3, 0, a4 // load pb[x]
mul    	s4, s2, s3 // pa[k] * pb[x]
add    	t6, t6, s4 // s += pa[k] * pb[x]
add    	t1, t1, s10 // pb += 2 * N
add    	t1, t1, s10 //
addi   	a3, a3, 1 // k++
blt    	a3, s9, -40 // k <= K
add    	t5, t2, a1 //
add    	t5, t5, a1 //
add    	t5, t5, a1 //
add    	t5, t5, a1 // pc[x]
sw     	t5, 0, t6 // pc[x] = s
addi   	a1, a1, 1 // x++
blt    	a1, s10, -80 //
add    	t0, t0, s9 // pa += K
add    	t2, t2, s10 //
add    	t2, t2, s10 //
add    	t2, t2, s10 //
add    	t2, t2, s10 // pc += N
addi   	a0, a0, 1 // y++
blt    	a0, s11, -112 //
jalr   	zero, ra, 0 //
