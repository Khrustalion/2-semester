addi    t0, zero, 64 // M
addi    t1, zero, 60 // N
addi    t2, zero, 32 // K
addi    s1, zero, 0x100 // a[M][K]
mul a0, t0, t2 // M * K
add a0, a0, s1 // b[K][N]
mul a1, t2, t1 // K*N
add a1, a1, a1 // 2*K*N
add a1, a1, a0 // c[M][N]
addi    a2, s1, 0 // pa
addi    a3, a1, 0 // pc
addi    a4, zero, 0 // y, for y
addi    a5, zero, 0 // x, for x
addi    a6, a0, 0 // pb
addi    a7, zero, 0 // s
addi    s2, zero, 0 // k, for k
add s3, a2, s2 // pa + k
lb  s3, 0, s3 // pa[k]
add s4, a5, a5 // 2*x
add s4, a6, s4 // pb + 2*x
lh  s4, 0, s4 // pb[x]
mul s3, s3, s4 // pa[k] * pb[x]
add a7, a7, s3// s += pa[k] * pb[x]
add a6, a6, t1 
add a6, a6, t1 // pb += 2*N
addi    s2, s2, 1 // ++k
blt s2, t2, -40 // jump to for k
add s4, a5, a5 
add s4, s4, a5
add s4, s4, a5
add s4, a3, s4 // pc + 4*x
sw  a7, 0, s4  // pc[x] = s
addi    a5, a5, 1 // ++x
blt a5, t1,  -80 // jump to for x
add a2, a2, t2 // pa += K
add a3, a3, t1
add a3, a3, t1
add a3, a3, t1
add a3, a3, t1 // pc += 4*N
addi    a4, a4, 1 // ++y
blt a4, t0, -112// jump to for y
jalr    zero, ra, 0
