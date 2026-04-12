/* decision_tree.c - FLUX VM weighted agent decision
 * Build: gcc -std=c11 -Wall -I../flux-runtime-c/include -I../flux-runtime-c/src -o dt decision_tree.c ../flux-runtime-c/src/flux_vm.c ../flux-runtime-c/src/memory.c -lm
 */
#include <stdio.h>
#include <string.h>
#include "flux_vm.h"

int main(void) {
    FluxVM vm; flux_vm_init(&vm);
    vm.gp[10] = 75;  /* energy */
    vm.gp[5] = 60;   /* confidence */
    vm.gp[6] = 30;   /* curiosity instinct */
    vm.memory[0]=80; vm.memory[1]=40; vm.memory[2]=60; vm.memory[3]=20; /* trust in 4 agents */
    vm.gp[20] = 200; /* weight array base addr */
    vm.gp[16] = 4;   /* number of actions */

    printf("=== FLUX Agent Decision Tree ===
");
    printf("Environment: Energy=%d Conf=%d Curiosity=%d
", vm.gp[10], vm.gp[5], vm.gp[6]);
    printf("Trust: [%d, %d, %d, %d]

", vm.memory[0], vm.memory[1], vm.memory[2], vm.memory[3]);

    /* Compute weights for 4 actions */
    int explore = vm.gp[6];  /* curiosity */
    int social = vm.memory[0]+vm.memory[1]+vm.memory[2]+vm.memory[3]; /* trust sum */
    int rest = (100 - vm.gp[10]); /* inversely proportional to energy */
    int build = vm.gp[5];  /* confidence */
    vm.memory[200] = explore;
    vm.memory[201] = social;
    vm.memory[202] = rest;
    vm.memory[203] = build;

    printf("Weights: EXPLORE=%d SOCIAL=%d REST=%d BUILD=%d
", explore, social, rest, build);

    /* Execute RAND_WEIGHTED (0xE9): R15 = weighted pick from mem[R20], count=R16 */
    uint8_t bc[] = {0xE9, 0x15, 0x14, 0x10};
    flux_vm_execute(&vm, bc, 4);

    const char* actions[] = {"EXPLORE", "SOCIALIZE", "REST", "BUILD"};
    int choice = vm.gp[15];
    printf("
Chosen action: %s (index %d)
",
           choice < 4 ? actions[choice] : "UNKNOWN", choice);
    printf("FLUX cycles: %lld
", (long long)vm.cycles);
    return 0;
}
