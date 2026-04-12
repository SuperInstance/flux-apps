Below is the design of a FLUX bytecode program for a weighted agent decision tree with four actions: EXPLORE, SOCIALIZE, REST, and BUILD. The program reads inputs, computes weights, and uses a weighted random selection to choose an action.

### Register Allocation
- `R10`: Energy level
- `R5`: Confidence
- `R0`: Temporary register for calculations
- `R1`: Temporary register for calculations
- `R2`: Temporary register for calculations
- `R3`: Temporary register for calculations
- `R4`: Temporary register for calculations
- `R6`: Sum of trust scores
- `R7`: Weight for EXPLORE
- `R8`: Weight for SOCIALIZE
- `R9`: Weight for REST
- `R11`: Weight for BUILD
- `R12`: Base address for weights (memory[200])
- `R13`: Action result
- `R15`: Result storage

### Bytecode Program

```c
unsigned char flux_program[] = {
    // Compute EXPLORE weight (curiosity)
    0x10, 0x07, 0x01, // MOVI R7, 1 (curiosity)

    // Compute SOCIALIZE weight (sum of trust scores)
    0x40, 0x06, 0x00, 0x00, // MOVI16 R6, 0x0000 (initialize sum)
    0x20, 0x00, 0x06, 0x00, // ADD R0, R6, MEM[0] (add trust score 0)
    0x20, 0x06, 0x00, 0x01, // ADD R6, R6, MEM[1] (add trust score 1)
    0x20, 0x06, 0x00, 0x02, // ADD R6, R6, MEM[2] (add trust score 2)
    0x20, 0x06, 0x00, 0x03, // ADD R6, R6, MEM[3] (add trust score 3)
    0x30, 0x08, 0x12, 0x00, // STORE R8, R12, 0 (store SOCIALIZE weight)

    // Compute REST weight ((100-energy)*10/100)
    0x40, 0x00, 0x64, 0x00, // MOVI16 R0, 0x0064 (100)
    0x20, 0x00, 0x00, 0x0A, // SUB R0, R0, R10 (100 - energy)
    0x22, 0x09, 0x00, 0x0A, // MUL R9, R0, 0x0A (multiply by 10)
    0x40, 0x00, 0x64, 0x00, // MOVI16 R0, 0x0064 (100)
    0x22, 0x09, 0x09, 0x00, // DIV R9, R9, R0 (divide by 100)
    0x30, 0x09, 0x12, 0x01, // STORE R9, R12, 1 (store REST weight)

    // Compute BUILD weight (confidence*10)
    0x22, 0x0B, 0x05, 0x0A, // MUL R11, R5, 0x0A (confidence * 10)
    0x30, 0x0B, 0x12, 0x03, // STORE R11, R12, 3 (store BUILD weight)

    // Store EXPLORE weight
    0x30, 0x07, 0x12, 0x02, // STORE R7, R12, 2 (store EXPLORE weight)

    // Use RAND_WEIGHTED to pick action
    0xE9, 0x0D, 0x12, 0x04, // RAND_WEIGHTED R13, R12, 4

    // Store result in R15
    0x30, 0x0D, 0x0F, 0x00, // STORE R13, R15, 0

    // HALT
    0x00
};

```

### C Harness to Run the Program

```c
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define MEMORY_SIZE 256

uint8_t memory[MEMORY_SIZE];
uint8_t registers[16];

void execute_flux_program() {
    uint8_t *pc = flux_program;
    while (*pc != 0x00) { // HALT
        uint8_t opcode = *pc++;
        uint8_t rd, rs1, rs2, imm8, imm16_lo, imm16_hi;

        switch (opcode) {
            case 0xE8: // RAND_RANGE
                rd = *pc++;
                rs1 = *pc++;
                // RAND_RANGE implementation
                registers[rd] = rand() % 256;
                break;

            case 0xE9: // RAND_WEIGHTED
                rd = *pc++;
                rs1 = *pc++;
                rs2 = *pc++;
                // RAND_WEIGHTED implementation
                uint8_t weights[4];
                for (int i = 0; i < 4; i++) {
                    weights[i] = memory[rs1 + i];
                }
                uint8_t total = 0;
                for (int i = 0; i < 4; i++) {
                    total += weights[i];
                }
                uint8_t r = rand() % total;
                for (int i = 0; i < 4; i++) {
                    if (r < weights[i]) {
                        registers[rd] = i;
                        break;
                    }
                    r -= weights[i];
                }
                break;

            case 0x20: // ADD
                rd = *pc++;
                rs1 = *pc++;
                rs2 = *pc++;
                registers[rd] = registers[rs1] + registers[rs2];
                break;

            case 0x22: // MUL
                rd = *pc++;
                rs1 = *pc++;
                rs2 = *pc++;
                registers[rd] = registers[rs1] * registers[rs2];
                break;

            case 0x10: // MOVI
                rd = *pc++;
                imm8 = *pc++;
                registers[rd] = imm8;
                break;

            case 0x40: // MOVI16
                rd = *pc++;
                imm16_lo = *pc++;
                imm16_hi = *pc++;
                registers[rd] = (imm16_hi << 8) | imm16_lo;
                break;

            case 0x30: // STORE
                rd = *pc++;
                rs1 = *pc++;
                uint8_t offset = *pc++;
                memory[registers[rs1] + offset] = registers[rd];
                break;

            default:
                printf("Unknown opcode: %02x\n", opcode);
                exit(1);
        }
    }
}

int main() {
    srand(time(NULL));

    // Initialize memory and registers
    memory[0] = 5; // Trust score 0
    memory[1] = 3; // Trust score 1
    memory[2] = 4; // Trust score 2
    memory[3] = 2; // Trust score 3
    registers[10] = 50; // Energy level
    registers[5] = 7; // Confidence

    execute_flux_program();

    printf("Action chosen: %d\n", registers[15]);
    return 0;
}
```

This C program initializes the memory and registers, executes the bytecode, and prints the chosen action.