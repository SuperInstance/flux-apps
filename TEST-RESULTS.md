# FLUX Application Test Results

## Iteration 4 (2026-04-11) — Jetson Orin Nano 8GB, Computed Goto VM

| App | Result | Expected | Status |
|-----|--------|----------|--------|
| fibonacci (15 iters) | R1=610, R2=987 | F(15)=610, F(16)=987 | PASS |
| sum (1..1000) | R2=500500 | 500500 | PASS |
| factorial (10!) | R2=3628800 | 3628800 | PASS |

## Lessons Learned
1. LOOP opcode already decrements the register — do NOT emit separate DEC before LOOP
2. LOOP offset is relative to PC AFTER the instruction (pos+4 from opcode start)
3. MOVR opcode is Format E (4 bytes): opcode rd rs padding
4. FluxBuilder label fixup needs to account for instruction size

## Process Notes
- First attempt: all apps gave wrong results due to double-decrement bug
- Root cause: misread LOOP semantics — it decrements AND jumps, not just jumps
- Fix: removed explicit DEC from app builders
- Then: fixup offset was wrong (target-inst_start instead of target-inst_start-4)
- Fix: corrected offset to account for VM PC position after instruction read
