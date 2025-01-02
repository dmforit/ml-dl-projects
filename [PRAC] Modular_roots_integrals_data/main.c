#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>


typedef double (*func)(double x);


extern double f1(double x);
extern double f2(double x);
extern double f3(double x);
extern double der_f1(double x);
extern double der_f2(double x);
extern double der_f3(double x);

double g1(double x);
double g2(double x);
double der_g1(double x);
double der_g2(double x);


/*
Первое тестирование.


double g1(double x) {
	return ln((1 + x * x) / 5);
}


double g2(double x) {
	return 0.0;
}


double der_g1(double x) {
	return 2 * x / (1 + x * x);
}


double der_g2(double x) {
	return 0.0;
}
*/

/* 
Второе тестирование.

double g1(double x) {
	return sin(x);
}


double g2(double x) {
	return cos(x * x);
}


double der_g1(double x) {
	return cos(x);
}


double der_g2(double x) {
	return -sin(x * x) * 2 * x;
}
*/

/*
Третье тестирование.

double g1(double x) {
	return exp(x);
}


double g2(double x) {
	return 9 / (x * x * x);
}


double der_g1(double x) {
	return exp(x);
}


double der_g2(double x) {
	return - 27 / (x * x * x * x);
}
*/


/**
 * Prints help information
 */
void print_help() {
    printf("The program for calculating the square of limited area. All rights reserved.\n");
    printf("Usage: <file> [option(s)]\nOptions:\n");
    printf("  %-20s Display this information.\n", "--help");
    printf("  %-20s Print points of curves\' crossing. \n", "-p");
    printf("  %-20s Print the number of iterations while calculating the crossing points. \n", "-n");
    printf("  %-20s Test a function \n", "-t <number 0..1>");
    printf("  %-20s 0 - Root function.\n", " ");
    printf("  %-20s 1 - Integral function.\n", " ");
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
 * Calculating the root of the equation: f(x) = g(x)
 * @param f
 * @param g
 * @param a
 * @param b
 * @param e1
 * @param der_f
 * @param der_g
 * @param iterations
 * @return the root of the equation
 */
double root(func f, func g, double a, double b, double e1, func der_f, func der_g, long long* iterations) {
    double Fa, Fb;
    double der_Fa, der_Fb, der2_Fa, der2_Fb;
    double der_Fae, der_Fbe;
    *iterations = 0;
    while (fabs(a - b) > 2 * e1) {
        (*iterations)++;
        Fa = f(a) - g(a);
        Fb = f(b) - g(b);
        der_Fa = der_f(a) - der_g(a);
        der_Fb = der_f(b) - der_g(b);
        der_Fae = der_f(a + e1) - der_g(a + e1);
        der_Fbe = der_f(b + e1) - der_g(b + e1);
        der2_Fa = (der_Fae - der_Fa) / e1;
        der2_Fb = (der_Fbe - der_Fb) / e1;
        
        if (Fa * der2_Fa < 0) {
            a = a - (Fa * (a - b)) / (Fa - Fb); 
        }
        else {
            a = a - Fa / der_Fa;
        }

        if (Fb * der2_Fb < 0) {
            b = b - (Fb * (b - a)) / (Fb - Fa);
        }
        else {
            b = b - Fb / der_Fb;
        }
    }
    return (a + b) / 2;
}


/**
 * Exchange the values of two double numbers
 * @param a
 * @param b
 */
void swap(double *a, double *b) {
	double temp = *a;
	*a = *b;
	*b = temp;
}


/**
 * Calculates the following number: 1 divided by the root of the value
 * @param number
 * @return the calculated number
 */
float Q3_32(float number) {
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y = number;
    i = *(long*)&y; // evil bit point hack
    i = 0x5f3759df - (i >> 1); // what???
    y = *(float*)&i; // iterations
    y = y * (threehalfs - (x2 * y * y)); // it's a cool function

    return y;
}


/**
 * The function for calculation of the number of segments
 * @param a
 * @param b
 * @param e
 * @return the number of segments
 */
int calculate_n(double a, double b, double e) {
    return (int)((b - a) * (b - a) * Q3_32(24 * e));
}


/**
 * Calculating the integral of function f(x) - g(x) between a and b
 * @param f
 * @param g
 * @param a
 * @param b
 * @param e2
 * @return the integral
 */
double integral (func f, func g, double a, double b, double e2) {
    int n = calculate_n(a, b, e2);
    double res = 0;
    for (int k = 1; k <= 2 * n - 1; k += 2) {
        double x = a + (b - a) * k / (2 * n);
        res += f(x) - g(x);
    }
    res *= (b - a) / n;
    return res;
}


static func functions[3] = {f1, f2, f3};
static func der_functions[3] = {der_f1, der_f2, der_f3};


/**
 * Testing the root function
 */
void test_root() {
	char while_symbol, temp; //temp for one redundant symbol
	printf("Root testing started...\n");
	while (1) {
		printf("Type in the segment: ");
		double a, b;
		scanf("%lf %lf", &a, &b);
		if (a > b) {
			swap(&a, &b);
		}
		printf("Type in the precision: ");
		double precision;
		scanf("%lf", &precision);
		printf("Type in two integer sequence numbers of functions which are to be tested <0..2>: ");
		int f_one, f_two;
		scanf("%d %d", &f_one, &f_two);
		long long iterations = 0;
		double result = root(functions[f_one], functions[f_two], a, b, precision, der_functions[f_one], der_functions[f_two], &iterations);
		double y_result = functions[f_one](result);
		printf("The calculated point of root: (%.5f, %.5f)\n", result, y_result);
		printf("The number of iterations: %ld\n", iterations);
		while (1) {
			printf("Continue? [Y/N]: ");
			scanf("%c%c", &temp, &while_symbol);
			if (while_symbol == 'N' || while_symbol == 'Y' || while_symbol == 'n' || while_symbol == 'y')
				break;	
			else {
				printf("Wrong symbol. Try again :)\n");
			}
		}
		if (while_symbol == 'N' || while_symbol == 'n')
			break;
	}
	printf("Root testing ended.\n");
}


/**
 * Testing the integral function
 */
void test_integral() {
	char while_symbol, temp; //temp for one redundant symbol
	printf("Integral testing started...\n");
	while (1) {
		printf("Type in the segment: ");
		double a, b;
		scanf("%lf %lf", &a, &b);
		if (a > b) {
			swap(&a, &b);
		}
		printf("Type in the precision: ");
		double precision;
		scanf("%lf", &precision);
		printf("Type in two integer sequence numbers of functions which are to be tested <0..2>: ");
		int f_one, f_two;
		scanf("%d %d", &f_one, &f_two);
		double result = integral(functions[f_one], functions[f_two], a, b, precision);
		printf("The calculated integral: %.5f\n", result);
		while (1) {
			printf("Continue? [Y/N]: ");
			scanf("%c%c", &temp, &while_symbol);
			if (while_symbol == 'N' || while_symbol == 'Y' || while_symbol == 'n' || while_symbol == 'y') 
				break;	
			else {
				printf("Wrong symbol. Try again :)\n");
			}
		}
		if (while_symbol == 'N' || while_symbol == 'n')
			break;
	}
	printf("Integral testing ended.\n");
}


/**
 * Testing functions
 * @param testing_type
 */
void start_testing(int testing_type) {
	if (!testing_type) {
		test_root();
	}
	else {
		test_integral();
	}
}


/**
 * Main function
 * @param argc
 * @param args
 * @return 0 if everything is okay and 1 if there is an error
 */
int main(int argc, char **args) {
    int p = 0; // mode of printing the points
    int n = 0; // mode of printing number of the iterations
    int testing_type = -1;
	if (argc > 0) { // Arguments processing
        if (help_is_found(argc, args)) {
            print_help();
            return 0;
        }
        int res;
        while ((res = getopt(argc, args, "pnt:")) != -1) {
            switch(res) {
                case 'p': p = 1; break;
                case 'n': n = 1; break;
                case 't':
                    if (set_str_value(&testing_type, optarg) == -1) { // checking for errors
                        printf("%s: option requires an argument -- t\n", args[0]);
                        return 1;
                    }
                    else if (testing_type != 0 && testing_type != 1) { // checking for values
                        printf("%s: wrong value for -t\n", args[0]);
                        return 1;
                    } break;
                case '?': return 1;
            }
        }
    }
    printf("The program started...\n");
    
    if (testing_type != - 1) {
    	if (p == 1 || n == 1) {
    		printf("The program doesn't process other flags during tests\n");
    	}
 		printf("Testing mode is ON.\n");   		
    	start_testing(testing_type);
    	return 0;
    } 
    printf("Testing mode is OF.\n\n");
    long long first_iterations = 0;
    long long second_iterations = 0;
    long long third_iterations = 0;
    double x1 = root(f3, f1, -3.0, -2.0, 0.000001, der_f3, der_f1, &first_iterations);
    double x2 = root(f3, f2, -1.0, -0.25, 0.000001, der_f3, der_f2, &second_iterations);
    double x3 = root(f2, f1, 1.0, 2.0, 0.000001, der_f2, der_f1, &third_iterations);
    double y1 = f1(x1);
    double y2 = f2(x2);
    double y3 = f1(x3);
    double i1 = integral(f3, f1, x1, x2, 0.0001);
    double i2 = integral(f2, f1, x2, x3, 0.0001);
    double I = i1 + i2;
    
    if (p) { // Print crossing points
    	printf("%-12s%s\n", " ", "Crossing points");
        printf("%-7s%-14s%7s%-14s\n", " ", "Curves", " ", "Points");
    	printf("%-20s (%.5f, %.5f)\n", "One and three", x1, y1);
    	printf("%-20s (%.5f, %.5f)\n", "Two and three", x2, y2);
    	printf("%-20s (%.5f, %.5f)\n", "One and two", x3, y3);
    	printf("\n");
    }
    if (n) { // Print the number of iterations
    	printf("The number of iterations for each point:\n");
    	printf("For the first point: %lld iterations\n", first_iterations);
    	printf("For the second point: %lld iterations\n", second_iterations);
    	printf("For the third point: %lld iterations\n", third_iterations);
    	printf("\n");
    }
    printf("The calculated integral: %.4f\n", I);
    return 0;
}
