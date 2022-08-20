1
a. 0
b. still 0 (because the block size is 8 == stepsize, which will cause conflict miss)
c. make stepsize=1 (hitrate=0.5)

2
a. hit rate = 0.75 (since the width of byte offset is 3 and we add 8 each time, so it take 2 step to make index + 1, so it takes 4 access, and we will miss the first and hit the last three)
b. hit rate will be close to 1 since we will not miss any longer
c. cache blocking techniques, we add a loop stepsize = blocksize to move data into ceche and reuse them in the inner loop

3
a. 0-0.5 (best: just like LRU[like Scenario1], worst: 0)
b. set associativity to 1 (then it will become a direct map, there will be no randomness between ways)



ijk:    n = 1000, 2.323 Gflop/s
ikj:    n = 1000, 0.333 Gflop/s --worst
jik:    n = 1000, 2.372 Gflop/s
jki:    n = 1000, 15.282 Gflop/s --best
kij:    n = 1000, 0.334 Gflop/s  --worst
kji:    n = 1000, 14.541 Gflop/s

average step affect spatial locality



small enough to put in the cache block
much bigger than the cache block
