#include <pthread.h>
#include <stdio.h>

#define MAX 1000

struct TContext {
    const char* Name;
    int* Counter;
    int Mod;
};

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* ThreadFunc(void* arg) {
    struct TContext* ctxt = arg;
    int* counter = ctxt->Counter;
    fprintf(stderr, "This is %s thread\n", ctxt->Name);
    pthread_mutex_lock(&m);
    while (*counter < MAX) {
        if (*counter % 2 == ctxt->Mod) {
            printf("%d ", (*counter)++);
            pthread_cond_signal(&cond);
        } else {
            pthread_cond_wait(&cond, &m);
        }
    }
    pthread_mutex_unlock(&m);
    pthread_exit(0);
}

int main()
{
    pthread_t t1;
    pthread_t t2;

    int counter = 0;
    struct TContext ctxt1 = {"even", &counter, 0};
    struct TContext ctxt2 = {"odd", &counter, 1};
    pthread_create(&t1, 0, ThreadFunc, &ctxt1);
    pthread_create(&t2, 0, ThreadFunc, &ctxt2);

    pthread_join(t1, 0);
    pthread_join(t2, 0);
    printf("\n");
    return 0;
}