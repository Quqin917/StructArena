# Region-Based Arena Allocator

single-thereaded memory allocator written in C, designed to outperform standard heap allocator for batch allocation workloads.

By utilizing a region-based bump-pointer architecture, this allocator achieves up to a **6x speedup** over the standard 'malloc/free' while maintaining perfect memory safety (verified by Valgrind Memcheck).

## Architecture & Design Principles

Standard heap allocation (`malloc`) suffer from performance overhead, fragmentation, and increased complexity of manual allocation and deallocation. This project solves those bottlenecks using three core strategies:

### 1. Region-Based Contiguous Memory

Instead of asking OS for memory on every allocation, the Arena requests large, contiguous block (e.g., 2MB Regions). Internal allocation then handled by simply advancing a pointer ( a "bump pointer").
* **Benefit:** $O(1)$ allocation time and continuous memory layout. This improves **CPU L1/L2 cache hits** and leverages the hardware prefetcher by reducing pointer-chasing across fragmented pages.

### 2. Stable Pointer Growth via Linked Chunks

When a region reaches capacity, the Arena dynamically allocates a new region and links it to the previous one.
* **Benefit:** Unlinked `realloc`-based dynamic arrays, this approach guarantess that **existing pointer are never invalidated** when the Arena grows.

### 3. Hardware Word-Alignment

Every allocation is mathematically aligned to the CPU's native word size (`uintptr_t`, typically 8 bytes on 64-bit systems).
* **Benefit:** Prevents misaligned access penalties and bus errors, making the code portable and safe for strict-alignment architectures.

## Performance Benchmarks

Bechmarking performed by allocating 1,000,000 linked-list nodes, traversing them, and cleaning up
| Metric | `malloc/free` | `Arena Allocator` |
| :--- | :--- | :--- |
| **Execution Time (-O2)** | 0.021199 sec | 0.003261 sec |
| **Speedup** | Baseline | **6.50x** |
| **System Calls (malloc)** | 1,000,000 | 2 (Using 2MB Regions) |


## Memory Lifecycle & "Warm" Reusability

To support high-frequency processing loops (like game loops or rendering frames), the Arena distinguishes between **Resetting** and **Freeing**:
* `arenaReset(&arena)`: Resets the bump pointer to the beginning of the linked region without returning memory to the OS.
* `freeArena(&arena)`: Properly walks the linked list of regions and returns all memory to the OS.

### Valgrind Verification

The memory lifecycle has been strictly verified for leaks.
```text
==24297== HEAP SUMMARY:
==24297==     in use at exit: 0 bytes in 0 blocks
==24297==   total heap usage: 7,000,003 allocs, 7,000,003 frees, 128,778,304 bytes allocated
==24297== 
==24297== All heap blocks were freed -- no leaks are possible
```
