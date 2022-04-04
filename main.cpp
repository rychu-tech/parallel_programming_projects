//Time complexity for sequential - O(n^2)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

const long NUM = 1000000;
const int NUM_THREADS = 4; //MAX = 4



void generate_list(long long int num_elements, long long int list_of_elements[]) {
	long long int i;
	srand (time(NULL));
	for(i = 0; i < num_elements; i++) {
		list_of_elements[i] = rand()%100000;
	}
}

void swap(long long int *xp, long long int *yp) {
    long long int temp = *xp;
    *xp = *yp;
    *yp = temp;
}
 
void bubbleSort(long long int arr[], long long int n) {
   long long int i, j;
   for (i = 0; i < n-1; i++)     
       for (j = 0; j < n-i-1; j++)
           if (arr[j] > arr[j+1])
              swap(&arr[j], &arr[j+1]);
}

void print_array(long long int a[], long long int n) {
	long long int i;
	for (i = 0; i < n; i++) {
		printf("Array[%lld] : %lld\n", i, a[i]);
	}
}

void bubbleSort_parallel(long long int a[], long long int n) {
	omp_set_num_threads(NUM_THREADS);
	int i, j;
	for (i = 0; i < n - i - 1; i++) {
		#pragma omp parallel for
		for (j = 0; j < n - 1; j++) {
			if (a[j] > a[j+1]) {
				swap(&a[j], &a[j+1]);
			}
		}
	}
}

void copy_list(long long int n, long long int a[], long long int b[]) {
	int i;
	for (i = 0; i < n; i++) {
		b[i] = a[i];
	}
}

int main() {
	long long int *list_of_elements = malloc (NUM * sizeof *list_of_elements);
	long long int *list_of_elements_copy = malloc (NUM * sizeof *list_of_elements_copy);

	printf("\n------------------------------------------\n");
	printf("Please select from below:\n");
	printf("1. SEQUENTIAL Bubblesort\n");
	printf("2. PARALLEL Bubblesort\n");
	printf("3. Close the program\n");
	
	printf("\nGenerating %lld random integer elements...\n", NUM);
	
	generate_list(NUM, list_of_elements);
	
	printf("\nDone!\n");
	
	char used = 'f';
	
		
	char pick;
	int checker = 1;
	while (checker == 1) {
		if (used == 's') {
			copy_list(NUM, list_of_elements_copy, list_of_elements);
		}
		else {
			copy_list(NUM, list_of_elements, list_of_elements_copy);
		}
		printf("Your pick : ");
		scanf(" %c", &pick);
		while (pick != '1' && pick != '2' && pick != '3') {
			printf("Wrong choice! Try again : ");
			scanf(" %c", &pick);
		}
		
		switch(pick) {
			case '1':
				printf("\n----- Sequential Bubblesort Algorithm -----\n");
				clock_t t; 
				
				
				if (used == 'f') {
					t = clock();
					bubbleSort(list_of_elements, NUM);
					t = clock() - t;
					used = 's';
				} 
				else {
					t = clock();
					bubbleSort(list_of_elements_copy, NUM);
					t = clock() - t;
					used = 'f';
				}
				
				double time_taken = ((double)t)/CLOCKS_PER_SEC;
				
				printf("The sequential program took %f seconds to execute\n", time_taken);
				
				break;
			
			case '2':
				printf("\n------ Parallel Bubblesort Algorithm ------\n");
				double start, end;
				
				if (used == 'f') {
					start = omp_get_wtime();
					bubbleSort_parallel(list_of_elements, NUM);
					end = omp_get_wtime() - start;
					used = 's';
				}
				else 
				{
					start = omp_get_wtime();
					bubbleSort_parallel(list_of_elements_copy, NUM);
					end = omp_get_wtime() - start;
					used = 'f';
				}				

				printf("The parallel program took %f seconds to execute\n", end);
				
				break;
			
			case '3':
				printf("Closing the program...\n");
				checker = 0;
				printf("Closed\n");
				break;
				
			default:
				printf("\nPlease select '1', '2' or '3'!\n");
		}
	
	
	}
	
	
	return 0;
}
