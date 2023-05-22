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
	printf("%p %p %p\n", pointer + i * 3, pointer + i * 3 + 1, pointer + i * 3 + 2);
}

void *fill_blocks(void *argc){
	int i = *((int *)argc) % (n / 3);
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
}

static pthread_mutex_t output_file;

void *output_info(void *argc) {
	int i = *((int *)argc) % (n / 3);
	pthread_mutex_lock(&output_file);
	FILE *file = fopen("log.txt", "a");
	fprintf(file, "%p %p %p\n", pointer + i * 3, pointer + i * 3 + 1, pointer + i * 3 + 2);
	free(pointer[i * 3]);
	free(pointer[i * 3 + 1]);
	free(pointer[i * 3 + 2]);
	pthread_mutex_unlock(&output_file);
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
		}else if(i < 2 * n / 3){
			pthread_create(threads + i, NULL, fill_blocks, number + i);
		}else{
			pthread_create(threads + i, NULL, output_info, number + i);
		}
	}
	for(i = 0; i < n; ++i){
		pthread_join(threads[i], NULL);
	}
	// for(i = 0; i < 9; ++i){
	// 	printf("%p\n", pointer[i]);
	// }
}