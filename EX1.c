
#define _CRT_SECURE_NO_WARNINGS

/* Libraries */

#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>


/* Constant definitions */

#define N 3
#define ROWS 4
#define COLS 5

/* Type definitions */

typedef struct number
{
	unsigned long long num;
	int sum;
} Number;

typedef struct triad
{
	int i, j, value;
} Triad;

typedef struct item
{
	Triad data;
	struct item* next;
} Item;

/* Function declarations */

void Ex1();
void Ex2();
void Ex3();

Number* primeSums(unsigned long long n1, unsigned long long n2, int* p_size);
int** matrixMaxNeighbor(int A[][COLS], int rows, int cols);
void createThreeLists(int** A, int rows, int cols, Item** pL1, Item** pL2);

/* Declarations of auxiliary functions */

int isPrime(int num);
int digitSum(unsigned long long num);

int neighborMax(int A[][COLS], int rows, int cols, int i, int j);
int** allocMatrix(int rows, int cols);

Triad createThree(int i, int j, int value);
Item* createNewNode(Triad input);
void insert(Item** lst, Triad input);


void printArray(Number* arr, int size);
void inputMatrix(int A[][COLS], int rows, int cols);
void printMatrix(int A[][COLS], int rows, int cols);
void printDynamicMatrix(int** A, int rows, int cols);
void freeMatrix(void** A, int rows);
void inputDynamicMatrix(int** A, int rows, int cols);
void printList(Item* lst);
void freeList(Item* lst);

/* ------------------------------- */

int main()
{
	int select = 0, i, all_Ex_in_loop = 0;
	printf("Run menu once or cyclically?\n(Once - enter 0, cyclically - enter other number) ");
	if (scanf("%d", &all_Ex_in_loop) == 1)
		do
		{
			for (i = 1; i <= N; i++)
				printf("Ex%d--->%d\n", i, i);
			printf("EXIT-->0\n");
			do {
				select = 0;
				printf("please select 0-%d : ", N);
				scanf_s("%d", &select);
			} while ((select < 0) || (select > N));
			switch (select)
			{
			case 1: Ex1(); break;
			case 2: Ex2(); break;
			case 3: Ex3(); break;
			}
		} while (all_Ex_in_loop && select);
		return 0;
}


/* Function definitions */

void Ex1()
{
	Number* resArr = NULL;
	unsigned long long n1, n2;
	int size = 0;

	// Getting user input of number 1 and number 2.
	printf("\n\nPlease enter input number 1\n"); 
	scanf_s("%llu", &n1);
	printf("Please enter input number 2\n");
	scanf_s("%llu", &n2);

	resArr = primeSums(n1, n2, &size);
	printArray(resArr, size);
	// Freeing memory allocation.
	free(resArr), resArr = NULL; 

}

void Ex2()
{
	int mat[ROWS][COLS], ** newMat = NULL;

	// Dynamic matrix user input and print.
	printf("\n\nPlease enter values for the matrix:\n"); 
	inputMatrix(mat, ROWS, COLS);
	printf("The matrix you have input is:");
	printMatrix(mat, ROWS, COLS);

	// Print of the new neighbor matrix.
	printf("\nNeighbor matrix is:"); 
	newMat = matrixMaxNeighbor(mat, ROWS, COLS);
	printDynamicMatrix(newMat, ROWS, COLS);

	// Free the memory used for the dynamic allocated matrix.
	freeMatrix(newMat, ROWS); 
	newMat = NULL;
	
}

void Ex3()
{
	int rows, cols, ** mat = NULL;
	Item* L1 = NULL, * L2 = NULL;

	// User input of rows and columns.
	printf("\n\nEnter number of rows\n");
	scanf_s("%d", &rows);
	printf("Enter number of columns\n");
	scanf_s("%d", &cols);

	// Creating a matrix, getting user input and printing the matrix.
	mat = allocMatrix(rows, cols);
	inputDynamicMatrix(mat, rows, cols);
	printf("The matrix you have entered is:\n\n");
	printDynamicMatrix(mat, rows, cols);

	// Creating the lists from the matrix the user input.
	createThreeLists(mat, rows, cols, &L1, &L2);
	printf("First list (L1):\n");
	printList(L1);
	printf("\n\n");
	printf("Second list (L2):\n");
	printList(L2);
	printf("\n\n");

	// Freeing memory allocations - 2 lists and the matrix.
	freeList(L1), freeList(L2);
	freeMatrix(mat, rows);
	L1 = L2 = NULL;
	mat = NULL;
}

Number* primeSums(unsigned long long n1, unsigned long long n2, int* p_size)
{
	// Decalring the variables and dynamically allocating the array.
	int res = 0, i = 0, size = 2;
	Number* arr = NULL, * tempArr = NULL;
	arr = tempArr = (Number*)calloc(size, sizeof(Number));   
	assert(arr), assert(tempArr);
	*p_size = 0;

	while (n1 <= n2) {

		res = digitSum(n1);

		// Entering the condition if the sum of digits is prime.
		if (isPrime(res)) {   
			arr = tempArr;
			arr[i].num = n1;
			arr[i++].sum = res;

			// Checking if we need to realloc the array to store the next values.
			if (i == size) {   
				size *= 2;   // Doubling the size each time to prevent unecessary Reallocs.
				tempArr = (Number*)realloc(arr, size * sizeof(Number));
				assert(tempArr);
				arr = tempArr; // Setting 'arr' to be same as the after realloc array.
			}
		}
		n1++;
	}
	// Returning the size of an array by reference.
	*p_size = i; 
	return i == 0 ? NULL : arr; // Returning NULL if the size of the array is 0 - There are no elements.
}

int** matrixMaxNeighbor(int A[][COLS], int rows, int cols)
{
	int** newMat = NULL, i, j;
	newMat = allocMatrix(rows, cols); // Allocating new matrix.

	// Iterating on each element and checking if the condition is true.
	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {

			newMat[i][j] = neighborMax(A, rows, cols, i, j); 
		}
	}
	return newMat;
}
void createThreeLists(int** A, int rows, int cols, Item** pL1, Item** pL2)
{
	int i, j;

	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {

			// Checking if the element is equal to the coordinates sum.
			if (i + j == A[i][j]) {
				insert(pL1, createThree(i, j, A[i][j]));
			}

			// Checking if the indexes and the matrix element are arithmetic sequence.
			if (j - i == A[i][j] - j) {
				insert(pL2, createThree(i, j, A[i][j]));
			}
		}
	}
}

/* Definitions of auxiliary functions */


int isPrime(int num) {

	int i;

	// First conditions check wether if the function can return an answer immediatly without the need of a loop.
	if (num <= 1) return 0; 
	if (num <= 3) return 1;
	if (num % 2 == 0 || num % 3 == 0) return 0;

	// Each iteration we check the i^2 is smaller than the number, in order to get complexity of square root of N.
	for (i = 5; i * i <= num; i += 2) {
		if (num % i == 0) {
			return 0;
		}
	}
	return 1;
}
int digitSum(unsigned long long num) {

	int sum = 0;
	while (num != 0) {

		// Isolating each digit in the loop and sums it until the number is zero.
		sum += num % 10; 
		num /= 10;
	}
	return sum;
}

int neighborMax(int A[][COLS], int rows, int cols, int i, int j) {

	int currMax = 0;

	// The conditions handles both finding the max element and dealing with negative elements.

	// Checking top element.
	if (i > 0) {
		if (currMax < A[i - 1][j] || currMax == 0) {
			currMax = A[i - 1][j];
		}
	}

	// Checking below element.
	if (i < rows - 1) {
		if (currMax < A[i + 1][j] || currMax == 0) {
			currMax = A[i + 1][j];
		}
	}

	// Checking left element.
	if (j > 0) {
		if (currMax < A[i][j - 1] || currMax == 0) {
			currMax = A[i][j - 1];
		}
	}

	// Checking right element.
	if (j < cols - 1) {
		if (currMax < A[i][j + 1] || currMax == 0) {
			currMax = A[i][j + 1];
		}
	}
	return currMax;
}

int** allocMatrix(int rows, int cols) {

	int** mat = NULL, i;
	// Allocating an array with ROWS number of pointer elements.
	mat = (int**)calloc(rows, sizeof(int*)); 
	assert(mat);

	for (i = 0; i < rows; ++i) {
		// Allocating an array with COLS number of elements for each row in the matrix.
		mat[i] = (int*)calloc(cols, sizeof(int)); 
		assert(mat[i]);
	}
	return mat;

}

Triad createThree(int i, int j, int value) {

	Triad temp = { 0,0,0 };

	// Setting the values for the temp variable and returning it.
	temp.i = i;
	temp.j = j;
	temp.value = value;

	return temp;
}

Item* createNewNode(Triad input) {

	// Creating a single node and returning it.
	Item* newNode = (Item*)malloc(sizeof(Item));
	if (newNode) {
		newNode->data = input;
		newNode->next = NULL;
	}

	return newNode;
}
void insert(Item** lst, Triad input) {

	Item* currPtr = (*lst);

	// Checking if the list is empty.
	if (*lst == NULL) {
		*lst = createNewNode(input);
	}

	// If the list is not empty, iterating the list until the end to add one more element.
	else {
		while (currPtr->next != NULL) {
			currPtr = currPtr->next;
		}
		currPtr->next = createNewNode(input);
	}
}

void printArray(Number* arr, int size) {

	int i,j;

	// If the array is NULL, print NULL.
	if (arr == NULL) printf("\nNULL Array, size is 0\n\n"); 

	else {
		printf("\nArray size: %d\n\n", size);
		printf("Result array:\n");
		printf("Num:\t");
		for (i = 0; i < size; ++i) {
			printf("%llu\t", arr[i].num);
		}
		printf("\nSum:\t");
		for (j = 0; j < size; ++j) {
			printf("%d\t", arr[j].sum);
		}
		printf("\n\n\n");
	}
}

void inputMatrix(int A[][COLS], int rows, int cols) {

	int i, j;
	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {
			printf("[%d][%d] = ", i, j);
			scanf_s("%d", &A[i][j]);
		}
		printf("\n");
	}
}

void printMatrix(int A[][COLS], int rows, int cols) {

	int i, j;
	printf("\n\n");
	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {
			printf("%4d", A[i][j]);
		}
		printf("\n");
	}
}

void printDynamicMatrix(int** A, int rows, int cols) {

	int i, j;
	printf("\n\n");
	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {
			printf("%4d", A[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
}

void freeMatrix(void** A, int rows) {

	int i;
	for (i = 0; i < rows; ++i) {

		// Freeing the memory allocation for each row in the matrix.
		free(A[i]); 
	}
	free(A), A = NULL;
}

void inputDynamicMatrix(int** A, int rows, int cols) {

	int i, j;

	printf("\nEnter values for the matrix:\n");
	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {
			printf("[%d][%d] = ", i, j);
			scanf_s("%d", &A[i][j]);
		}
		printf("\n");
	}

}

void printList(Item* lst) {

	printf("\n");
	while (lst != NULL) {
		printf("[%d,%d,%d]--> ", lst->data.i, lst->data.j, lst->data.value);
		lst = lst->next;
	}
	printf("NULL\n");

}

void freeList(Item* lst) {

	Item* toDelete = lst;

	// Iterating the list until we get to the last element.
	while (toDelete != NULL)
	{
		// Iterating one node further and then releasing the previous node.
		lst = lst->next;
		free(toDelete);
		toDelete = NULL;
		toDelete = lst;
	}

}

/* ------------------- */