#include <malloc.h>
#include <pthread.h>

const int n = 9;
char *arr[3];
void *pointer[9];

void *make_blocks(void *argc){
	int i = *((int *)argc);
	pointer[i * 3] = malloc(16);
	pointer[i * 3 + 1] = malloc(1024);
	pointer[i * 3 + 2] = malloc(4096);
}

void *fill_blocks(void *argc){
	int i = *((int *)argc);
	int j = 0;
	int **list = (int **)pointer;
	for(j = 0; j < 16 / sizeof(int); j += sizeof(int)){
		list[i * 3][j] = i;
	}
	for(j = 0; j < 1024 / sizeof(int); j += sizeof(int)){
		list[i * 3 + 1][j] = i;
	}
	for(j = 0; j < 4096 / sizeof(int); j += sizeof(int)){
		list[i * 3 + 2][j] = i;
	}
	free(list[i * 3]);
	free(list[i * 3 + 1]);
	free(list[i * 3 + 2]);

}

void *output_info(void *argc){

}

int main(){
	pthread_t threads[n];
	int i = 0;
	int number[n];
	for(i = 0; i < n; i++){
		number[i] = i % (n / 3);
	}
	for(i = 0; i < n; ++i){
		if(i < n / 3){
			pthread_create(threads + i, NULL, make_blocks, number + i);
		}
	}
	// int i =0;
	// pointer[0 * 3] = malloc(16);
	// pointer[0 * 3 + 1] = malloc(16);
	// pointer[0 * 3 + 2] = malloc(1);
	for(i = 0; i < 9; ++i){
		printf("%p\n", pointer[i]);
	}
}