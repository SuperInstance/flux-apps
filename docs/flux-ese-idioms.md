

**FLUX‑ese Idioms – common bytecode patterns (5)**  

| # | Idiom | Typical register‑pseudocode (example) | Agent behavior it enables | Real‑world use‑case example |
|---|-------|----------------------------------------|----------------------------|--------------------------------|
| 1 | **HALT** | `R0 = 0x00` | Stops the processor (halts execution) | Used when an unrecoverable error is detected – the CPU enters a halted state until reset. |
| 2 | **NOP** | `R1 = 0xFF` | Performs a *no‑operation* – effectively a delay cycle | Inserts a fixed‑latency stall in timing‑critical loops or to align pipeline stages. |
| 3 | **INC** | `R2 = R2 + 1` | Increases the contents of a register by 1 | Implements a loop counter (e.g., counting iterations in a `for`‑loop). |
| 4 | **DEC** | `R3 = R3 – 1` | Decreases the contents of a register by 1 | Implements a down‑counting loop counter (e.g., counting down in a `while` loop). |
| 5 | **PUSH** | `R4 = R4 + 1` | Pushes a word (typically 16‑bit) onto the hardware stack | Used for subroutine calls – saves return address and registers before jumping to a function. |

*Notes*  

* The register‑pseudocode column shows a **representative** encoding; actual FLUX‑ese machines may map the idiom to any register, but the pattern (e.g., “ + 1”, “ – 1”, “ = 0”) stays the same.  
* The *agent behavior* describes what the CPU does when the opcode is fetched.  
* The *real‑world use‑case* gives a concrete scenario where a programmer would rely on that idiom.  

---

**FLUX‑ese Standard Library Concepts – 3**  

| # | Concept | Purpose (what the library provides) | Register convention (which registers the library expects to use) |
|---|---------|------------------------------------|---------------------------------------------------------------|
| 1 | **MEM** | Dynamic memory‑management routines (alloc, free, memcpy, memset, etc.) | Uses **R0–R1** as a base pointer for the heap; the library treats the pair as a *heap base* and assumes the stack pointer lives in **R2**. |
| 2 | **IO** | Polled‑I/O framework (device registers, interrupt‑driven reads/writes, DMA helpers) | Expects **R2–R3** to hold the *device‑base* address; the library treats **R2** as the *port selector* and **R3** as the *data buffer* pointer. |
| 3 | **SYS** | System‑service primitives (exception handling, timer/tick, cache‑coherency, power‑management) | Allocates **R4–R5** for *system vectors*; the library uses **R4** as the *interrupt‑vector* base and **R5** as the *exception‑frame* pointer. |

*Notes*  

* Each standard‑library concept is deliberately tied to a **pair of consecutive registers** (e.g., R0‑R1, R2‑R3, R4‑R5) so that callers can rely on a known register layout without needing to save/restore them manually.  
* The *purpose* column summarizes the high‑level service the library offers; the *register convention* tells the programmer which registers the library will clobber or expect to be preset, enabling re‑entrant code.  

---  

*Total word count ≈ 340 words – well under the 800‑word limit.*