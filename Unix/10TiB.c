#include <stdio.h>
#include <sys/mman.h>
int main() {
    size_t n = (1UL << 40);
    void* addr = mmap(NULL, n * sizeof(int), PROT_READ | PROT_WRITE,
                      MAP_ANONYMOUS | MAP_PRIVATE | MAP_NORESERVE, -1, 0);
    if (addr == MAP_FAILED) {
        perror("mmap()");
        return 1;
    }
    char c;
    scanf("%c", &c);
    if (munmap(addr, n * sizeof(int)) == -1) {
        perror("munmap()");
        return 1;
    };
    return 0;
}