#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

/* Number of elements */
#define ARRAY_SIZE 45

/* Length of time for each iteration, in milliseconds */
#define LOOP_INTERVAL 100000

/* Terminal Colors */
#define COLOR_CLEAR "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_MAGENTA "\033[35m"

int n_compares = 0;
int n_swaps = 0;

double start_time;
double tick_time;

char *algorithm = "---";
struct timeval tim;

void reset_stats() {
  n_compares = n_swaps = 0;
}

/* do_sleep()
 * Uses the tick_time variable to determine the amount of time elapsed
 * since the last sleep. Sleeps for LOOP_INTERVAL - elapsed ms.
 */
void do_sleep() {
  double elapsed;

  gettimeofday(&tim, NULL);
  elapsed = (tim.tv_sec + (tim.tv_usec/1000000.0)) - tick_time;

  usleep(LOOP_INTERVAL - elapsed);
  gettimeofday(&tim, NULL);
  tick_time = tim.tv_sec + (tim.tv_usec/1000000.0);
}

/* print_data()
 * Takes an array of integers, three indexes into the array, and a 
 * color control string. Prints a header with stats, a color key,
 * and then the values contained in the array. Highlights the indexes
 * that were given in first, second, and pivot.
 * 
 * values: An array of integers to display
 * first: The first index into the array to highlight (compare or swap)
 * second: The second index into the array to highlight
 * pivot: Index into the array to be highlighted purple
 * color: A string containing a pre-defined color code
 */
void print_data(int *values, int first, int second, int pivot, char *color) {
  int i, j; 

  double cur_time; 

  gettimeofday(&tim, NULL);
  cur_time = (tim.tv_sec + (tim.tv_usec/1000000.0)) - start_time;

  system("clear");
  printf("Algorithm: \"%s\"  Compares: %d  Swaps: %d  Time: %3.2fs\n", 
         algorithm, n_compares, n_swaps, cur_time);
  printf("Key:  %s->%s Compare  ", COLOR_YELLOW, COLOR_CLEAR);
  printf("%s->%s Swap  ", COLOR_GREEN, COLOR_CLEAR);
  printf("%s->%s No Swap  ", COLOR_RED, COLOR_CLEAR);
  printf("%s|--|%s Pivot/Iteration", COLOR_MAGENTA, COLOR_CLEAR);
  printf("\n\n");

  for(i=0; i<ARRAY_SIZE; i++) {
    if(i == first || i == second) {
      printf("%s%3d -> %s", color, i+1, COLOR_CLEAR);
    } else {
      printf("%3d    ", i+1);
    }

    if(i == pivot) printf("%s", COLOR_MAGENTA);

    printf("|");

    for(j=0; j<=values[i]; j++) printf("-");

    printf("|");

    if(i == pivot) printf("%s", COLOR_CLEAR); 

    printf("\n");
  }
  do_sleep();
}

/* color_swap()
 * Given an array of integers, two indexes and a pivot, this function
 * will swap the values in valuaes[first] and values[second], then 
 * print the current state of the array with first and second
 * highlighted with COLOR_GREEN.
 * 
 * values: Array of integers to be displayed
 * first: Index into the array, first value to swap
 * second: Index into the array, other value to swap
 * pivot: Index into the array, the current pivot point (-1 to ignore)
 */
void color_swap(int *values, int first, int second, int pivot) {
  int tmp;

  n_swaps++;

  tmp = values[first];
  values[first] = values[second];
  values[second] = tmp;

  print_data(values, first, second, pivot, COLOR_GREEN);
}

/* color_noswap()
 * Given an array of integers, two indexes and a pivot index, this 
 * function will not swap any values, and display the state of the 
 * array with first and second highlighted with COLOR_RED.
 * 
 * values: Array of integers to display
 * first: Index into the array, to be highlighted red
 * second: Index into the array, to be highlighted red
 * pivot: Index into the array, to be highlighted purple (-1 to ignore)
 */
void color_noswap(int *values, int first, int second, int pivot) {
  print_data(values, first, second, pivot, COLOR_RED);
}

/* color_compare()
 * Given an array of integers, two indexes and a pivot index, this
 * function will display the state of the array with the first
 * and second values highlighted with COLOR_YELLOW.
 * 
 * values: Array of integers to display
 * first: Index into the array, to be highlighted yellow
 * second: Index into the array, to be highlighted yellow
 * pivot: Index into the array, to be highlighted purple (-1 to ignore)
 */
void color_compare(int *values, int first, int second, int pivot) {
  n_compares++;

  print_data(values, first, second, pivot, COLOR_YELLOW);
}

/* bubble_sort()
 * Implementation of Bubble Sort. Takes an array of integers, calls 
 * color_compare(), color_swap() and color_noswap() to display the
 * progress of the sort.
 * 
 * values: Array of integers to be sorted
 */
void bubble_sort(int *values) {
  int i, j;

  algorithm = "Bubble Sort";

  for(i=ARRAY_SIZE-1; i>0; i--) {
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

/* quick_sort_r()
 * Implementation of Quick Sort. Takes an array of integers and two 
 * indexes into that array. Left and right define the area that
 * quicksort will operate on. This function calls itself recursively.
 * Calls color_compare(), color_swap() and color_noswap() to display
 * the progress.
 * 
 * values: Array of integers to be sorted
 * left: Index to the array to indicate the left boundary to be sorted
 * right: Index to the array to indicate the right boundary to be sorted
 */
void quick_sort_r(int *values, int left, int right) {
  int i = left;
  int j = right;
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

/* quick_sort()
 * Wrapper around quick_sort_r() which implements Quick Sort
 * 
 * values: Array of integers to be sorted
 */
void quick_sort(int *values) {
  algorithm = "Quick Sort";

  quick_sort_r(values, 0, ARRAY_SIZE - 1);
}

/* heap_sort_siftDown()
 * Implementation of SiftDown for Heap Sort. Sifts smaller values down
 * through the heap structure to ensure that the array is a valid 
 * heap when finished. Calls color_compare(), color_swap() and 
 * color_noswap() to display its progress.
 * 
 * values: Array of integers to be sorted (assumed to be a heap)
 * start: Index into the array, heap node to start sifting down from
 * end: Index into the array, end
 */ 
void heap_sort_siftDown(int *values, int start, int end) {
  int root = start;
  int child = (root * 2) + 1;
  int swap = -1;

  while((root * 2) + 1 <= end) {
    child = (root * 2) + 1;
    swap = root;

    color_compare(values, swap, child, end+1);
    if( values[swap] < values[child] ) {
      swap = child;
    }

    if( child + 1 <= end ) {
      color_compare(values, swap, child+1, end+1);
      if( values[swap] < values[child+1] ) {
        swap = child + 1;
      }
    }

    if( swap != root ) {
      color_swap(values, root, swap, end+1);
      root = swap;
    } else {
      color_noswap(values, child, child+1, end+1);
      return;
    }
  }
}

/* heap_sort_heapify()
 * The first step in Heap Sort, we take a list of integers and call
 * heap_sort_siftDown() until the array is a valid representation of 
 * a heap.
 * 
 * values: Array of integers to be sorted
 */ 
void heap_sort_heapify(int *values) {
  int count = ARRAY_SIZE;
  int start = (ARRAY_SIZE - 2) / 2;

  while( start >= 0 ) {
    heap_sort_siftDown(values, start, count-1);
    start -= 1;
  }
}

/* heap_sort()
 * Implementation of Heap Sort. First calls heap_sort_heapify() to
 * create a heap structure from the array, then repeatedly calls
 * heap_sort_siftDown() after swapping the max value to the end of the
 * array. When there are no more elements in the heap, the array is
 * sorted.
 * 
 * values: Array of integers to be sorted
 */ 
void heap_sort(int *values) {
  int end = ARRAY_SIZE - 1;

  algorithm = "Heap Sort";

  heap_sort_heapify(values);

  while( end > 0 ) {
    color_swap(values, end, 0, end);
    end--;
    heap_sort_siftDown(values, 0, end);
  }
}

/* insertion_sort()
 * Implementation of Insertion Sort. Calls color_compare(), color_swap()
 * and color_noswap() to display the progress.
 * 
 * values: Array of integers to be sorted
 */ 
void insertion_sort(int *values) {
  int i, j;

  algorithm = "Insertion Sort";

  for(i = 1; i < ARRAY_SIZE; i++) {
    for(j = i; j > 0; j--) {
      color_compare(values, j, j-1, i);
      if(values[j] <= values[j-1]) {
        color_swap(values, j, j-1, i);
      } else {
        color_noswap(values, j, j-1, i);
        break;
      }
    }
  }
}

/* selection_sort()
 * Implementation of Selection Sort. Calls color_compare(), color_swap()
 * and color_noswap() to display the progress.
 * 
 * values: Array of integers to be sorted
 */ 
void selection_sort(int *values) {
  int i, j;
  int iMin;

  algorithm = "Selection Sort";

  for(j = 0; j < ARRAY_SIZE-1; j++) {
    iMin = j;
    for(i = j+1; i < ARRAY_SIZE; i++) {
      color_compare(values, i, iMin, j-1);
      if(values[i] < values[iMin]) {
        iMin = i;
      }
    }
    if(iMin != j) {
      color_swap(values, j, iMin, j-1);
    } else {
      color_noswap(values, j, iMin, j-1);
    }
  }

}

/* shell_sort()
 * Implementation of Shell Sort. Between the gaps, this function 
 * implements Insertion Sort. Calls color_compare(), color_swap()
 * and color_noswap() to display the progress.
 * 
 * values: Array of integers to be sorted
 */ 
void shell_sort(int *values) {
  int gap, i, j, k;
  int n = ARRAY_SIZE;
  int gaps[] = {48, 21, 7, 5, 1};

  algorithm = "Shell Sort";

  for( k = 0; k < sizeof(gaps) / sizeof(int); k++ ) {
    gap = gaps[k];
    for( i = 0 ; i < n ; i++ ) {
      for( j = i; j >= gap; j -= gap ) {
        color_compare(values, j, j-gap, i);
        if(values[j] < values[j-gap]) {
          color_swap(values, j, j-gap, i);
        } else {
          color_noswap(values, j, j-gap, i);
          break;
        }
      }
    }
  }
}

/* generate_data()
 * Takes an array of integers and uses rand() to randomly swap elements
 * of the array to randomize it. Prints the randomized array.
 * 
 * values: Array of integers to be randomized
 */ 
void generate_data(int *values) {
  int i;
  int swap_from, swap_to, tmp;

  algorithm = "---";

  for(i=0; i<ARRAY_SIZE; i++) values[i] = i;

  for(i=0; i<ARRAY_SIZE; i++) {
    swap_from = rand() % ARRAY_SIZE;
    swap_to = rand() % ARRAY_SIZE;

    tmp = values[swap_to];
    values[swap_to] = values[swap_from];
    values[swap_from] = tmp;
  }

  print_data(values, -1, -1, -1, COLOR_CLEAR);
}

/* main()
 * Loops forever calling each sorting algorithm with a randomized
 * list of integers each time. Records the number of compares, the
 * number of swaps, and the time for each run of each algorithm.
 * After all sorts have run, it displays the results, then loops again.
 */ 
int main() {
  int i, j;
  int max_l, min_i, max_i;
  double min_t, max_t;
  int values[ARRAY_SIZE];
  char ctrl_str[80];

  typedef void (*fn_ptr)( int* );
  fn_ptr fn_ptr_array[] = {
    &heap_sort,
    &quick_sort,
    &selection_sort,
    &insertion_sort,
    &shell_sort,
    &bubble_sort,
  };

  gettimeofday(&tim, NULL);
  srand(tim.tv_usec / 1.0);

  start_time = tick_time = tim.tv_sec + (tim.tv_usec/1000000.0);

  int array_size = sizeof(fn_ptr_array) / sizeof(fn_ptr);
  
  struct result {
    char *algorithm;
    int n_compares;
    int n_swaps;
    double n_seconds;
  };

  struct result results_list[array_size];
  double end_time;

  for(i = 0 ;  ; i = (i % array_size) ) {
    reset_stats();
    generate_data(values);

    gettimeofday(&tim, NULL);
    start_time = tim.tv_sec + (tim.tv_usec/1000000.0);

    (*fn_ptr_array[i])(values);

    gettimeofday(&tim, NULL);
    end_time = tim.tv_sec + (tim.tv_usec/1000000.0);
    
    print_data(values, -1, -1, -1, COLOR_CLEAR);

    struct result current_result;

    current_result.algorithm = algorithm;
    current_result.n_compares = n_compares;
    current_result.n_swaps = n_swaps;
    current_result.n_seconds = end_time - start_time;

    results_list[i] = current_result;

    sleep(5);

    i++;

    if(i == array_size) {
      min_t = 9999;
      max_t = 0;
      min_i = -1;
      max_i = -1;
      for(j = 0; j < array_size; j++) {
        if( results_list[j].n_seconds < min_t ) {
          min_t = results_list[j].n_seconds;
          min_i = j;
        }
        if( results_list[j].n_seconds > max_t ) {
          max_t = results_list[j].n_seconds;
          max_i = j;
        }
      }

      max_l = 0;
      for(j=0; j < array_size; j++) {
        if( strlen(results_list[j].algorithm) > max_l) {
            max_l = strlen(results_list[j].algorithm);
        }
      }
      system("clear");
      printf("Results:\n\n");

      sprintf(ctrl_str, "Run  %%%ds    Compares  Swaps  Time\n", -max_l);
      printf(ctrl_str, "Algorithm");
      sprintf(ctrl_str, "%%s%%d    %%%ds    %%-4d      %%-3d    %%-3.2fs%s\n", -max_l, COLOR_CLEAR);

      for(j = 0; j < array_size; j++) {
        char *color = COLOR_CLEAR;

        if( array_size > 1) {
          if( j == max_i ) {
            color = COLOR_RED;
          } 
          if( j == min_i ) {
            color = COLOR_GREEN;
          }
        }

        printf(ctrl_str,
               color,
               j+1,
               results_list[j].algorithm,
               results_list[j].n_compares,
               results_list[j].n_swaps,
               results_list[j].n_seconds
              );
      }
      sleep(30);
    }
  }
}

