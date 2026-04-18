The program simulates a cache management system. When a cache miss occurs (a requested resource is not in the cache) and the cache is full, the algorithm must decide which element to evict.

The Greedy Strategy used here is to evict the element that will be requested farthest in the future. If an element in the cache will never be requested again, it is treated as having a request time of infinity and becomes a primary candidate for eviction.

Key Features
Implements the optimal eviction policy to minimize cache misses.

Handles string-based resource identifiers.

Provides step-by-step logs of hits, misses, and the current state of the cache.

Includes automated testing via assert statements.
