#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

typedef struct{
    size_t size;
    struct s_block *next;
    struct s_block *prev;
    int free;
    int padding;
}s_block;

s_block *first_block = NULL;
const size_t SIZE_BLOCK = sizeof(s_block);

s_block* find_block(size_t size){
    s_block *cur_block = first_block;
    s_block *ans = NULL;
    int max_dif = -1;
    int flag = 1;
    while(cur_block && (cur_block != first_block || flag)){
        if(cur_block->free){
            if(cur_block->size > size && max_dif == -1 || cur_block->size > size && cur_block->size - size < max_dif){
                ans = cur_block;
            }
        }
        flag = 0;
    }

    return ans;
}

s_block* extend_new_block(size_t size){
    s_block *new_block;
    new_block = sbrk(0);

    if(sbrk(sizeof(s_block) + size) == (void *)-1){
        return NULL;
    }

    new_block->size = size;
    new_block->free = 0;

    if(first_block){
        s_block *last_block = first_block->prev;
        last_block->next = new_block;
        new_block->prev = last_block;
        new_block->next = first_block;
        first_block->prev = new_block;
    }else{
        first_block = new_block;
        first_block->next = first_block;
        first_block->prev = first_block;
    }

    return new_block;
}

void split_block(s_block *block, size_t size){
    s_block *new_block;

    new_block = block + SIZE_BLOCK + size;

    new_block->size = block->size - size - SIZE_BLOCK;
    new_block->next = block->next;
    block->prev = new_block;
    block->prev = new_block;
    new_block->prev = block;

    block->size = size;
    new_block->free = 1;
}

size_t align8(size_t s) {
    if(s & 0x7 == 0){
       return s;
    }
 
    return ((s >> 3) + 1) << 3;
}

void *my_malloc(size_t size){
    s_block *block, last;
    size_t s = align8(size);

    if(first_block){
        block = find_block(s);
        if(block){
            if((block->size - s) >= (SIZE_BLOCK + 8)){
                split_block(block, s);
            }
            block->free = 0;
        }else{
           block = extend_new_block(s);
            if(!block){
                return NULL;
            } 
        }
    }else{
        block = extend_new_block(s);
        if(!block){
            return NULL;
        }
        first_block = block;
    }

    return ((void *)block) + SIZE_BLOCK;
}

void *calloc(size_t number, size_t size){
    size_t *new;
    size_t s8, i;
    new = my_malloc(number * size);
    if(new){
        s8 = align8(number * size) >> 3;

        for(i = 0; i < s8; i++){
            new[i] = 0;
        }
    }

    return new;
}

int valid_addres(void *p){
    if(first_block){
        if(p >= first_block && p < sbrk(0)){
            int flag = 1, valid_flag = 0;
            s_block cur_block = first_block;

            while(cur_block && (cur_block != first_block || flag)){
                if(cur_block == p - SIZE_BLOCK){
                    valid_flag = 1;
                    break;
                }
                flag = 0;
            }
            return valid_flag;
        }
    }
    return 0;
}


int main(){
    void *b = sbrk(0);
    void *a = my_malloc(5);
    void *c = my_malloc(15);
    printf("%p % p %p\n", b, a, c);
    printf("%d\n", sizeof(s_block));
}

