#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define SIZE 45
#define LOOP_INTERVAL 50000

int n_compares = 0;
int n_swaps = 0;
char *algorithm = "None";

void reset_stats() {
  n_compares = n_swaps = 0;
}

void print_data(int *values, int first, int second, int pivot, int color) {
  int i, j; 

  system("clear");
  printf("Algorithm: \"%s\"  Compares: %d  Swaps: %d\n\n", algorithm, n_compares, n_swaps);

  for(i=0; i<SIZE; i++) {
    if(i == first || i == second) {
      printf("\033[%dm%3d: \033[0m", color, i+1);
    } else {
      printf("%3d: ", i+1);
    }

    if(i == pivot) {
      printf("\033[35m");
    }

    for(j=0; j<values[i]; j++) {
      printf("-");
    }
    printf(">");
    if(i == pivot) {
      printf("\033[0m");
    }

    /*
    for( ; j<SIZE; j++) {
      printf(" ");
    }
    */

    printf("\n");
  }
}

void color_swap(int *values, int first, int second, int pivot) {
  int tmp;

  n_swaps++;

  tmp = values[first];
  values[first] = values[second];
  values[second] = tmp;

  print_data(values, first, second, pivot, 32);
  usleep(LOOP_INTERVAL);
}

void color_noswap(int *values, int first, int second, int pivot) {
  print_data(values, first, second, pivot, 31);
  usleep(LOOP_INTERVAL);
}

int color_compare(int *values, int first, int second, int pivot) {
  n_compares++;

  print_data(values, first, second, pivot, 33);
  usleep(LOOP_INTERVAL);
}

void bubble_sort(int *values) {
  int i, j;
  int newn = -1;

  reset_stats();
  algorithm = "Bubblesort";

  for(i=SIZE-1; i>0; i--) {
    for(j=0; j<i; j++) {
      color_compare(values, j, j+1, i+1);
      if( values[j+1] < values[j] ) {
        color_swap(values, j, j+1, i+1);
      } else {
        color_noswap(values, j, j+1, i+1);
      }
    }
  }
}

void quick_sort_r(int *values, int left, int right) {
  int i = left;
  int j = right;
  int tmp;
  int pivot = (left + right) / 2;
  int pivot_value = values[pivot];

  while( i <= j ) {
    
    while( values[i] < pivot_value ) {
      color_compare(values, i, pivot, pivot);
      i++;
    }

    while( values[j] > pivot_value ) {
      color_compare(values, j, pivot, pivot);
      j--;
    }

    color_compare(values, i, j, pivot);

    if( i <= j ) {
      color_swap(values, i, j, pivot);
      i++;
      j--;
    } else {
      color_noswap(values, i, j, pivot);
    }
  }

  if( left < j ) {
    quick_sort_r(values, left, j);
  }

  if( i < right ){
    quick_sort_r(values, i, right);
  }
}

void quick_sort(int *values) {
  reset_stats();
  algorithm = "Quicksort";

  quick_sort_r(values, 0, SIZE - 1);
}

void heap_sort_siftDown(int *values, int start, int end) {
  int root = start;
  int child = (root * 2) + 1;
  int swap = -1;

  while((root * 2) + 1 <= end) {
    child = (root * 2) + 1;
    swap = root;

    color_compare(values, swap, child, -1);
    if( values[swap] < values[child] ) {
      swap = child;
    }

    if( child + 1 <= end ) {
      color_compare(values, swap, child+1, -1);
      if( values[swap] < values[child+1] ) {
        swap = child + 1;
      }
    }

    if( swap != root ) {
      color_swap(values, root, swap, -1);
      root = swap;
    } else {
      color_noswap(values, root, swap, -1);
      return;
    }
  }
}

void heap_sort_heapify(int *values) {
  int count = SIZE;
  int start = (SIZE - 2) / 2;

  while( start >= 0 ) {
    heap_sort_siftDown(values, start, count-1);
    start -= 1;
  }
}

void heap_sort(int *values) {
  int end = SIZE - 1;

  reset_stats();
  algorithm = "Heapsort";

  heap_sort_heapify(values);

  while( end > 0 ) {
    color_swap(values, end, 0, -1);
    end--;
    heap_sort_siftDown(values, 0, end);
  }
}

void insertion_sort(int *values) {
  int i, j;

  reset_stats();
  algorithm = "Insertion Sort";

  for(i = 2; i < SIZE; i++) {
    for(j = i; j > 0; j--) {
      color_compare(values, j, j-1, -1);
      if(values[j] <= values[j-1]) {
        color_swap(values, j, j-1, -1);
      } else {
        color_noswap(values, j, j-1, -1);
        break;
      }
    }
  }
}

void generate_data(int *values) {
  int i;
  int swap_from, swap_to, tmp;

  for(i=0; i<SIZE; i++) values[i] = i;

  for(i=0; i<SIZE; i++) {
    swap_from = rand() % SIZE;
    swap_to = rand() % SIZE;

    tmp = values[swap_to];
    values[swap_to] = values[swap_from];
    values[swap_from] = tmp;
  }

  print_data(values, -1, -1, -1, -1);
}

int main() {
  int i, j;
  int values[SIZE];

  struct timeval tim;
  gettimeofday(&tim, NULL);
  srand(tim.tv_usec / 1.0);

  typedef void (*fn_ptr)( int* );
  fn_ptr fn_ptr_array[] = {
    &quick_sort,
    &heap_sort,
    &insertion_sort,
    &bubble_sort,
  };

  int array_size = sizeof(fn_ptr_array) / sizeof(fn_ptr);
  
  struct result {
    char *algorithm;
    int n_compares;
    int n_swaps;
    double n_seconds;
  };

  struct result results_list[array_size];
  double start_time, end_time;

  for(i = 0 ;  ; i = (i % array_size) ) {
    generate_data(values);

    gettimeofday(&tim, NULL);
    start_time = tim.tv_sec+(tim.tv_usec/1000000.0);

    (*fn_ptr_array[i])(values);

    gettimeofday(&tim, NULL);
    end_time = tim.tv_sec+(tim.tv_usec/1000000.0);
    
    struct result current_result;

    current_result.algorithm = algorithm;
    current_result.n_compares = n_compares;
    current_result.n_swaps = n_swaps;
    current_result.n_seconds = end_time - start_time;

    results_list[i] = current_result;

    sleep(5);

    i++;

    if(i == array_size) {
      system("clear");
      printf("Results:\n\n");

      for(j = 0; j < array_size; j++) {
        printf("Algorithm: \"%s\"\tCompares: %3d  Swaps: %3d  Seconds: %3.2f\n",
               results_list[j].algorithm,
               results_list[j].n_compares,
               results_list[j].n_swaps,
               results_list[j].n_seconds
              );
      }
      sleep(10);
    }
  }
}

