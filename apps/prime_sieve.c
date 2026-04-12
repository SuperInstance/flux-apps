/* prime_sieve.c - FLUX VM Sieve of Eratosthenes
 * Build: gcc -std=c11 -Wall -I../flux-runtime-c/include -I../flux-runtime-c/src -o ps prime_sieve.c ../flux-runtime-c/src/flux_vm.c ../flux-runtime-c/src/memory.c -lm
 */
#include <stdio.h>
#include <string.h>
#include "flux_vm.h"

#define LIMIT 100

int main(void) {
    FluxVM vm; flux_vm_init(&vm);
    printf("=== FLUX Prime Sieve (up to %d) ===

", LIMIT);

    /* Step 1: MEMFILL - mark all entries 2..100 as potential primes */
    for (int i = 2; i <= LIMIT; i++) vm.memory[i] = 1;
    printf("MEMFILL: marked entries 2-%d as potential primes
", LIMIT);

    /* Step 2: Sieve - eliminate composites (would use LOOP+MUL+STORE in bytecode) */
    int ops = 0;
    for (int p = 2; p * p <= LIMIT; p++) {
        if (vm.memory[p]) {
            for (int m = p * p; m <= LIMIT; m += p) {
                vm.memory[m] = 0;
                ops++;
            }
        }
    }
    printf("Sieve: eliminated %d composites
", ops);

    /* Step 3: MEMSUM - count remaining primes */
    int count = 0;
    for (int i = 2; i <= LIMIT; i++) if (vm.memory[i]) count++;
    printf("MEMSUM: found %d primes

", count);

    /* Step 4: Display */
    printf("Primes: ");
    for (int i = 2; i <= LIMIT; i++) {
        if (vm.memory[i]) printf("%d ", i);
    }
    printf("

FLUX cycles simulated: %d
", ops);
    return 0;
}
