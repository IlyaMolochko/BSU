#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *func(void *arg) {
    const char *c = (const char *) arg;
    for (int i = 0; i < 10000; ++i) {
//        printf("%s", c);
        write(STDOUT_FILENO, c, 1);
    }
    pthread_exit(0);
}

int main(int argc, char **argv) {
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;

    pthread_create(&thread1, NULL, func, (void *) "a");
    pthread_create(&thread2, NULL, func, (void *) "b");
    pthread_create(&thread3, NULL, func, (void *) "c");

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    return 0;
}