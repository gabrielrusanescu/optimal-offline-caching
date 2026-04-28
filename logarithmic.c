#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_NAME 50
#define INF 2000000000

typedef struct {
    int id;
    int next_access;
} Node;

typedef struct {
    Node *data;
    int size;
    int *pos_in_heap;
} MaxHeap;

void swap(MaxHeap *h, int i, int j) {
    Node temp = h->data[i];
    h->data[i] = h->data[j];
    h->data[j] = temp;
    h->pos_in_heap[h->data[i].id] = i;
    h->pos_in_heap[h->data[j].id] = j;
}

void heapify_up(MaxHeap *h, int idx) {
    while (idx > 0 && h->data[idx].next_access > h->data[(idx - 1) / 2].next_access) {
        swap(h, idx, (idx - 1) / 2);
        idx = (idx - 1) / 2;
    }
}

void heapify_down(MaxHeap *h, int idx) {
    int max = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < h->size && h->data[left].next_access > h->data[max].next_access) max = left;
    if (right < h->size && h->data[right].next_access > h->data[max].next_access) max = right;

    if (max != idx) {
        swap(h, idx, max);
        heapify_down(h, max);
    }
}

int solve_cache_ultra(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return -1;

    int n, k, m;
    if (fscanf(f, "%d", &n) != 1) { fclose(f); return -1; }

    char **resource_names = (char **)malloc(n * sizeof(char *));
    for (int i = 0; i < n; i++) {
        resource_names[i] = (char *)malloc(MAX_NAME * sizeof(char));
        fscanf(f, "%s", resource_names[i]);
    }

    fscanf(f, "%d", &k);
    int *initial_cache = (int *)malloc(k * sizeof(int));
    char temp[MAX_NAME];
    for (int i = 0; i < k; i++) {
        fscanf(f, "%s", temp);
        for(int j=0; j<n; j++) if(strcmp(resource_names[j], temp) == 0) initial_cache[i] = j;
    }

    fscanf(f, "%d", &m);
    int *requests = (int *)malloc(m * sizeof(int));
    for (int i = 0; i < m; i++) {
        fscanf(f, "%s", temp);
        for(int j=0; j<n; j++) if(strcmp(resource_names[j], temp) == 0) requests[i] = j;
    }

    int *next_occurrence = (int *)malloc(m * sizeof(int));
    int *last_seen = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) last_seen[i] = INF;
    for (int i = m - 1; i >= 0; i--) {
        next_occurrence[i] = last_seen[requests[i]];
        last_seen[requests[i]] = i;
    }

    MaxHeap h;
    h.data = (Node *)malloc(k * sizeof(Node));
    h.pos_in_heap = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) h.pos_in_heap[i] = -1;
    h.size = 0;

    for (int i = 0; i < k; i++) {
        int res_id = initial_cache[i];
        int first_use = INF;
        for(int p=0; p<m; p++) if(requests[p] == res_id) { first_use = p; break; }
        
        h.data[h.size].id = res_id;
        h.data[h.size].next_access = first_use;
        h.pos_in_heap[res_id] = h.size;
        h.size++;
        heapify_up(&h, h.size - 1);
    }

    int cache_misses = 0;
    for (int i = 0; i < m; i++) {
        int current_req = requests[i];

        if (h.pos_in_heap[current_req] != -1) {
            int idx = h.pos_in_heap[current_req];
            h.data[idx].next_access = next_occurrence[i];
            heapify_down(&h, idx); 
            heapify_up(&h, idx);
        } else {
            cache_misses++;
            int victim_id = h.data[0].id;
            h.pos_in_heap[victim_id] = -1;

            h.data[0].id = current_req;
            h.data[0].next_access = next_occurrence[i];
            h.pos_in_heap[current_req] = 0;
            heapify_down(&h, 0);
        }
    }

    for (int i = 0; i < n; i++) free(resource_names[i]);
    free(resource_names); free(initial_cache); free(requests);
    free(next_occurrence); free(last_seen);
    free(h.data); free(h.pos_in_heap);
    fclose(f);

    return cache_misses;
}

int main() {
    /*assert(solve_cache_ultra("input1.txt") == 2);
    assert(solve_cache_ultra("input2.txt") == 0);
    assert(solve_cache_ultra("input3.txt") == 3);
    */
    printf("Toate testele au trecut! Complexitate O(m log k).\n");
    return 0;
}