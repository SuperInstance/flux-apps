# flux-apps — Maintenance Notes

## Purpose
Example FLUX VM applications demonstrating real-world usage patterns.

## Apps
- decision_tree.c: Weighted agent decision using RAND_WEIGHTED (0xE9). Shows how instincts (curiosity), trust (social), energy, and confidence map to action weights. The VM picks a random action proportional to weights — this is how agents make probabilistic decisions.

- prime_sieve.c: Sieve of Eratosthenes using memory-range operations. Shows how to use MEMFILL (mark all as primes), then LOOP+MUL+STORE (eliminate composites), then MEMSUM (count remaining). Pure computation with no agent features — demonstrates the VM is a general-purpose computer too.

## Build Instructions
gcc -std=c11 -Wall -I../flux-runtime-c/include -I../flux-runtime-c/src -o &lt;app&gt; &lt;app&gt;.c ../flux-runtime-c/src/flux_vm.c ../flux-runtime-c/src/memory.c -lm

## Related Crates
- flux-runtime-c: the VM
- cuda-flux-stdlib: standard library subroutines (more complex than these examples)
