/**
 * This project is created in order to compare two types of sorting.
 * Used: Bubble Sort, Shell Sort.
 * The code was written by Karatyshchev Dmitry.
 */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>


/**
 * Prints help documentation
 */
void print_help() {
    printf("The program for comparing two types of sorting. All rights reserved.\n");
    printf("Usage: <file> [option(s)]\nOptions:\n");
    printf("  %-20s Display this information.\n", "--help");
    printf("  %-20s Number of array elements.\n", "-n <number>");
    printf("  %-20s Type of arrangement of elements in the array.\n", "-o <number 1..4>");
    printf("  %-20s 1 - The elements are already ordered.\n", " ");
    printf("  %-20s 2 - The elements are in reverse order.\n", " ");
    printf("  %-20s 3,4 - Arrangement of elements is random.\n", " ");
    printf("  %-20s Type of sorting.\n", "-t <number 0..2>");
    printf("  %-20s 0 - Both types.\n", " ");
    printf("  %-20s 1 - Bubble Sort.\n", " ");
    printf("  %-20s 2 - Shell Sort.\n", " ");
}


/**
 * Exchanges the values of two variables
 * @param a
 * @param b
 */
void swap_elems(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}


/**
 * Checks if two strings are equal
 * @param string
 * @param sample
 * @return 0 if string is not equal to sample and 1 otherwise
 */
int is_equal(const char* string, const char* sample) {
    int i = 0;
    while (string[i] && sample[i]) {
        if (string[i] != sample[i])
            return 0;
        i++;
    }
    return (string[i] == '\0' && sample[i] == '\0');
}


/**
 * Checks if the string is the help flag
 * @param string flag argument
 * @return 1 if string is the help flag and 0 otherwise
 */
int is_help(const char *string) {
    return (is_equal(string, "-h") | is_equal(string, "--help"));
}


/**
 * Finds the help flag
 * @param argc
 * @param args
 * @return 1 if the help flag is found and 0 otherwise
 */
int help_is_found(int argc, char **args) {
    for (int i = 1; i < argc; i++) {
        if (args[i][0] == '-' && is_help(args[i])) {
            return 1;
        }
    }
    return 0;
}


/**
 * Converts a string into the integer type and sets the value
 * @param number
 * @param value
 * @return 1 if value is not a flag and 0 otherwise
 */
int set_str_value(int* number, char* value) {
    if (value[0] == '-') {
        optind--;
        return -1;
    }
    *number = atoi(value);
    return 1;
}


/**
 * Generates elements for the first mode
 * @param size
 * @param arr
 */
void gen_el_1(int size, int* arr) {
    int sign = rand() % 2 == 0? 1 : -1;
    arr[0] = rand() * sign % (RAND_MAX / 2);
    for (int i = 1; i < size; i++) {
        sign = rand() % 2 == 0? 1 : -1;
        arr[i] = sign * (abs(arr[i - 1]) + rand() % (RAND_MAX / 4));
        if (abs(arr[i]) < abs(arr[i - 1]))
            arr[i] = INT_MAX;
    }
}


/**
 * Generates elements for the second mode
 * @param size
 * @param arr
 */
void gen_el_2(int size, int* arr) {
    int sign = rand() % 2 == 0? 1 : -1;
    arr[0] = rand() * sign * (rand() / 100);
    for (int i = 1; i < size; i++) {
        sign = rand() % 2 == 0? 1 : -1;
        arr[i] = sign * (abs(arr[i - 1]) - rand() / 10);
        if (abs(arr[i]) > abs(arr[i - 1]))
            arr[i] = 0;

    }
}


/**
 * Generates elements for the third and fourth modes
 * @param size
 * @param arr
 */
void gen_el_34(int size, int* arr) {
    int sign = rand() % 2 == 0? 1 : -1;
    for (int i = 0; i < size; i++) {
        arr[i] = sign * rand() * rand() % (RAND_MAX * (rand() / 100 + 1));
        sign = rand() % 2 == 0? 1 : -1;
    }
}


/**
 * Generates elements according to the mode
 * @param size
 * @param arr
 * @param mode
 */
void generate_elements(int size, int* arr, int mode) {
    srand(time(NULL));
    switch(mode) {
        case 1: gen_el_1(size, arr); break;
        case 2: gen_el_2(size, arr); break;
        default: gen_el_34(size, arr); break;
    }
}


/**
 * Bubble Sort
 * @param size
 * @param arr
 * @param comparisons
 * @param moves
 */
void BubbleSort(int size, int* arr, int* comparisons, int* moves) {
    *comparisons = 0;
    *moves = 0;
    for (int i = 1; i < size; i++) {
        for (int j = 1; j <= size - i; j++) {
            (*comparisons)++;
            if (abs(arr[j]) < abs(arr[j - 1])) {
                swap_elems(&arr[j], &arr[j - 1]);
                (*moves)++;
            }
        }
    }
}


/**
 * Shell Sort
 * @param size
 * @param arr
 * @param comparisons
 * @param moves
 */
void ShellSort(int size, int* arr, int* comparisons, int* moves) {
    *comparisons = 0;
    *moves = 0;
    int step = size / 2;
    while (step) {
        for (int i = step; i < size; i++) {
            int j = i;
            while (j > 0 && abs(arr[j - 1]) > abs(arr[j])) {
                (*comparisons)++;
                swap_elems(&arr[j - 1], &arr[j]);
                (*moves)++;
                j -= step;
            }
            if (j > 0)
                (*comparisons)++;
        }
        step /= 2;
    }
}


/**
 * It's for copying one array to another
 * @param size
 * @param arr
 * @param clear_arr
 */
void copy_arr(int size, int* arr, int* clear_arr) {
    for (int i = 0; i < size; i++)
        clear_arr[i] = arr[i];
}


/**
 * The main function
 * @param argc
 * @param args
 * @return 1 if the program works correctly and 0 otherwise
 */
int main(int argc, char **args) {
    int n = 100; // array size
    int mode = 1; // type of the array elements arrangement
    int sort_type = 1;
    if (argc > 0) { // Arguments processing
        if (help_is_found(argc, args)) {
            print_help();
            return 0;
        }
        int res;
        while ((res = getopt(argc, args, "n:o:t:")) != -1) {
            switch(res) {
                case 'n':
                    if (set_str_value(&n, optarg) == -1) { // checking for errors
                        printf("%s: option requires an argument -- n\n", args[0]);
                        return 1;
                    } break;
                case 'o':
                    if (set_str_value(&mode, optarg) == -1) { // checking for errors
                        printf("%s: option requires an argument -- o\n", args[0]);
                        return 1;
                    }
                    else if (mode < 1 || mode > 4) { // checking for values
                        printf("%s: wrong value for -o\n", args[0]);
                        return 1;
                    } break;
                case 't':
                    if (set_str_value(&sort_type, optarg) == -1) { // checking for errors
                        printf("%s: option requires an argument -- t\n", args[0]);
                        return 1;
                    }
                    else if (sort_type != 1 && sort_type != 2 && sort_type != 0) { // checking for values
                        printf("%s: wrong value for -t\n", args[0]);
                        return 1;
                    } break;
                case '?': return 1;
            }
        }
    }
    // Memory allocation and generation of elements
    int* arr = malloc(n * sizeof(int));
    printf("Memory is allocated successfully.\n");
    generate_elements(n, arr, mode);
    printf("Generation of elements was successful.\n");

    int comps = 0, moves = 0;
    // Choosing a sorting type
    if (sort_type == 1) {
        printf("Bubble Sort is chosen\n");
        BubbleSort(n, arr, &comps, &moves);

        printf("Elements are sorted now.\n");
        printf("The number of comparisons: %d\nThe number of moves: %d\n", comps, moves);
    }
    else if (sort_type == 2) {
        printf("Shell Sort is chosen\n");
        ShellSort(n, arr, &comps, &moves);

        printf("Elements are sorted now.\n");
        printf("The number of comparisons: %d\nThe number of moves: %d\n", comps, moves);
    }
    else if (sort_type == 0) {
        printf("Bubble Sort and Shell Sort are chosen\n");
        int* arr_copy = malloc(n * sizeof(int));
        copy_arr(n, arr, arr_copy);

        BubbleSort(n, arr, &comps, &moves);
        printf("Elements are sorted by Bubble Sort.\n");
        printf("The number of comparisons: %d\nThe number of moves: %d\n", comps, moves);
        comps = 0, moves = 0;

        ShellSort(n, arr_copy, &comps, &moves);
        printf("Elements are sorted by Shell Sort.\n");
        printf("The number of comparisons: %d\nThe number of moves: %d\n", comps, moves);
        free(arr_copy);
    }
    free(arr);
    return 0;
}
