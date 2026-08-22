#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    long long comparisons;
} SortStats;

void swap_int(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int compare_ints(int a, int b, SortStats *stats)
{
    stats->comparisons++;

    if (a < b)
        return -1;
    if (a > b)
        return 1;
    return 0;
}

void bubble_sort(int *a, int n, SortStats *stats)
{
    for (int end = n - 1; end > 0; end--) {
        int changed = 0;

        for (int i = 0; i < end; i++) {
            if (compare_ints(a[i], a[i + 1], stats) > 0) {
                swap_int(&a[i], &a[i + 1]);
                changed = 1;
            }
        }

        if (!changed)
            break;
    }
}

void merge_parts(int *a, int *temp, int left, int mid, int right,
                 SortStats *stats)
{
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        if (compare_ints(a[i], a[j], stats) <= 0)
            temp[k++] = a[i++];
        else
            temp[k++] = a[j++];
    }

    while (i <= mid)
        temp[k++] = a[i++];

    while (j <= right)
        temp[k++] = a[j++];

    for (i = left; i <= right; i++)
        a[i] = temp[i];
}

void merge_sort_rec(int *a, int *temp, int left, int right,
                    SortStats *stats)
{
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;

    merge_sort_rec(a, temp, left, mid, stats);
    merge_sort_rec(a, temp, mid + 1, right, stats);
    merge_parts(a, temp, left, mid, right, stats);
}

void merge_sort(int *a, int n, SortStats *stats)
{
    int *temp = malloc((size_t)n * sizeof(int));

    if (!temp)
        exit(1);

    merge_sort_rec(a, temp, 0, n - 1, stats);
    free(temp);
}

void partition_three_way(int *a, int left, int right, int pivot_value,
                         int *less_end, int *greater_start,
                         SortStats *stats)
{
    int low = left;
    int mid = left;
    int high = right;

    while (mid <= high) {
        int cmp = compare_ints(a[mid], pivot_value, stats);

        if (cmp < 0) {
            swap_int(&a[low], &a[mid]);
            low++;
            mid++;
        }
        else if (cmp > 0) {
            swap_int(&a[mid], &a[high]);
            high--;
        }
        else {
            mid++;
        }
    }

    *less_end = low - 1;
    *greater_start = high + 1;
}

void quick_sort_random_rec(int *a, int left, int right,
                           SortStats *stats)
{
    if (left >= right)
        return;

    int pivot_index = left + rand() % (right - left + 1);
    int less_end;
    int greater_start;

    partition_three_way(a, left, right, a[pivot_index],
                         &less_end, &greater_start, stats);

    quick_sort_random_rec(a, left, less_end, stats);
    quick_sort_random_rec(a, greater_start, right, stats);
}

void quick_sort_random(int *a, int n, SortStats *stats)
{
    if (n > 1)
        quick_sort_random_rec(a, 0, n - 1, stats);
}

int median_of_three(int *a, int first, int second, int third,
                    SortStats *stats)
{
    if (compare_ints(a[first], a[second], stats) > 0) {
        if (compare_ints(a[second], a[third], stats) > 0)
            return second;

        if (compare_ints(a[first], a[third], stats) > 0)
            return third;

        return first;
    }

    if (compare_ints(a[first], a[third], stats) > 0)
        return first;

    if (compare_ints(a[second], a[third], stats) > 0)
        return third;

    return second;
}

void get_three_random_indices(int left, int right,
                              int *first, int *second, int *third)
{
    int size = right - left + 1;

    *first = left + rand() % size;

    do {
        *second = left + rand() % size;
    } while (*second == *first);

    do {
        *third = left + rand() % size;
    } while (*third == *first || *third == *second);
}

void quick_sort_median3_rec(int *a, int left, int right,
                            SortStats *stats)
{
    if (left >= right)
        return;

    if (right - left + 1 < 3) {
        quick_sort_random_rec(a, left, right, stats);
        return;
    }

    int first;
    int second;
    int third;

    get_three_random_indices(left, right, &first, &second, &third);

    int pivot_index = median_of_three(a, first, second, third, stats);
    int less_end;
    int greater_start;

    partition_three_way(a, left, right, a[pivot_index],
                         &less_end, &greater_start, stats);

    quick_sort_median3_rec(a, left, less_end, stats);
    quick_sort_median3_rec(a, greater_start, right, stats);
}

void quick_sort_median3(int *a, int n, SortStats *stats)
{
    if (n > 1)
        quick_sort_median3_rec(a, 0, n - 1, stats);
}

void sort_small_group(int *a, int left, int right, SortStats *stats)
{
    for (int i = left + 1; i <= right; i++) {
        int value = a[i];
        int j = i - 1;

        while (j >= left && compare_ints(a[j], value, stats) > 0) {
            a[j + 1] = a[j];
            j--;
        }

        a[j + 1] = value;
    }
}

int select_median_index(int *a, int left, int right, int target,
                        SortStats *stats);

int median_of_medians_index(int *a, int left, int right,
                            SortStats *stats)
{
    int count = right - left + 1;

    if (count <= 5) {
        sort_small_group(a, left, right, stats);
        return left + count / 2;
    }

    int write = left;

    for (int group_left = left; group_left <= right; group_left += 5) {
        int group_right = group_left + 4;

        if (group_right > right)
            group_right = right;

        sort_small_group(a, group_left, group_right, stats);

        int middle = group_left +
                     (group_right - group_left) / 2;

        swap_int(&a[write], &a[middle]);
        write++;
    }

    int median_count = write - left;
    int target = left + median_count / 2;

    return select_median_index(a, left, write - 1,
                               target, stats);
}

int select_median_index(int *a, int left, int right, int target,
                        SortStats *stats)
{
    while (left < right) {
        int pivot_index =
            median_of_medians_index(a, left, right, stats);

        int pivot_value = a[pivot_index];
        int less_end;
        int greater_start;

        partition_three_way(a, left, right, pivot_value,
                             &less_end, &greater_start, stats);

        if (target <= less_end)
            right = less_end;
        else if (target >= greater_start)
            left = greater_start;
        else
            return target;
    }

    return left;
}

void quick_sort_median_rec(int *a, int left, int right,
                           SortStats *stats)
{
    if (left >= right)
        return;

    int target = left + (right - left) / 2;

    int pivot_index =
        select_median_index(a, left, right, target, stats);

    int pivot_value = a[pivot_index];
    int less_end;
    int greater_start;

    partition_three_way(a, left, right, pivot_value,
                         &less_end, &greater_start, stats);

    quick_sort_median_rec(a, left, less_end, stats);
    quick_sort_median_rec(a, greater_start, right, stats);
}

void quick_sort_median(int *a, int n, SortStats *stats)
{
    if (n > 1)
        quick_sort_median_rec(a, 0, n - 1, stats);
}

void generate_test_case(int n, const char *filename)
{
    FILE *file = fopen(filename, "w");

    if (!file)
        exit(1);

    for (int i = 0; i < n; i++) {
        fprintf(file, "%d%c",
                rand(),
                i + 1 == n ? '\n' : ' ');
    }

    fclose(file);
}

int read_input(const char *filename, int **values)
{
    FILE *file = fopen(filename, "r");

    if (!file)
        return 0;

    int capacity = 1024;
    int count = 0;

    int *data = malloc((size_t)capacity * sizeof(int));

    if (!data) {
        fclose(file);
        return 0;
    }

    while (fscanf(file, "%d", &data[count]) == 1) {
        count++;

        if (count == capacity) {
            capacity *= 2;

            int *next =
                realloc(data, (size_t)capacity * sizeof(int));

            if (!next) {
                free(data);
                fclose(file);
                return 0;
            }

            data = next;
        }
    }

    fclose(file);

    *values = data;
    return count;
}

double elapsed_seconds(struct timespec start, struct timespec end)
{
    return (double)(end.tv_sec - start.tv_sec)
         + (double)(end.tv_nsec - start.tv_nsec) / 1000000000.0;
}

void run_sort(const char *name, int *data, int n)
{
    int *copy = malloc((size_t)n * sizeof(int));

    if (!copy)
        exit(1);

    memcpy(copy, data, (size_t)n * sizeof(int));

    SortStats stats = {0};

    struct timespec start;
    struct timespec end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    if (strcmp(name, "bubble") == 0)
        bubble_sort(copy, n, &stats);
    else if (strcmp(name, "merge") == 0)
        merge_sort(copy, n, &stats);
    else if (strcmp(name, "quick1") == 0)
        quick_sort_random(copy, n, &stats);
    else if (strcmp(name, "quick3") == 0)
        quick_sort_median3(copy, n, &stats);
    else if (strcmp(name, "quickM") == 0)
        quick_sort_median(copy, n, &stats);
    else {
        free(copy);
        return;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    printf("%s %lld %.9f\n",
           name,
           stats.comparisons,
           elapsed_seconds(start, end));

    free(copy);
}

int main(int argc, char **argv)
{
    srand((unsigned)time(NULL));

    if (argc >= 2 && strcmp(argv[1], "--generate") == 0) {
        if (argc != 4)
            return 1;

        generate_test_case(atoi(argv[2]), argv[3]);
        return 0;
    }

    if (argc < 3)
        return 1;

    int *data;
    int n = read_input(argv[1], &data);

    if (n <= 0)
        return 1;

    for (int i = 2; i < argc; i++)
        run_sort(argv[i], data, n);

    free(data);
    return 0;
}
