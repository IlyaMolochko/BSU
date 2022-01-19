#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


struct vector{
    size_t size;
    size_t capacity;
    int *data;
};

void vector_init(struct vector *v){
    v->size = 0;
    v->capacity = 1;
    v->data = malloc(sizeof(int) * 1);
}

bool vector_push_back(struct vector *v, int x){
    if (v->size == v->capacity){
        if (2L * v->capacity < v->capacity){
            return false;
        }
        v->capacity *= 2;
        v->data = realloc(v->data, v->capacity * sizeof(int));
        if (v->data == NULL){
            return false;
        }
    }
    v->data[v->size] = x;
    ++v->size;
    return true;
}
const int* vector_at(const struct vector *v, size_t index){
    if (index < v->size){
        return v->data + index;
    }
    return NULL;
}

void vector_free(struct vector *v){
    free(v->data);
}

int main(){
    struct vector v;
    vector_init(&v);
    char command[10];
    int x;
    size_t index;
    const int *value;
    while (scanf("%s", command) != EOF){
        if (strcmp(command, "push_back") == 0){
            scanf("%d", &x);
            vector_push_back(&v, x);
        } else {
            scanf("%zu", &index);
            value = vector_at(&v, index);
            if (value != NULL){
                printf("%d\n", *value);
            } else {
                printf("out_of_range\n");
            }
        }
    }
    vector_free(&v);
}