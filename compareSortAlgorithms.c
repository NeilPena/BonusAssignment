//Name: Neil Pena
//Date: 3/24/2024
//Assignment: Bonus Assignment

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

//Created a heapify function 
void heapify(int arr[], int n, int i) {

    int largest = i; 

	//Spit to left and right values
    int left = 2 * i + 1; 
    int right = 2 * i + 2; 

	//Check to see which is the largest 
    if (left < n && arr[left] > arr[largest]){
        largest = left; 
	}

    if (right < n && arr[right] > arr[largest]){
        largest = right; 
	}

	//Swap the values i is not the largest and heapify
    if (largest != i) {
        int temp = arr[i]; 
        arr[i] = arr[largest]; 
        arr[largest] = temp; 

        heapify(arr, n, largest); 
    } 
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n) {
    // Build max heap
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    // Extract elements from heap one by one
    for (int i = n - 1; i > 0; i--) {
        // Move current root to end
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        // Call max heapify on the reduced heap
        heapify(arr, i, 0);
    }
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{ 
	if(l < r){

		//Initialize
		int i, j, k;

		//Find the middle to split
		int middle = l + (r - l) / 2; 

		//Recall function for both that needs to merge
		mergeSort(pData, l, middle);
		mergeSort(pData, middle + 1, r); 

		//Find the variable for both arrays 
		int m1 = middle - l + 1; 
		int m2 = r - middle;   

		//Allocate memory for each array 
		int* left = (int*)Alloc(m1 * sizeof(int)); 
		int* right = (int*)Alloc(m2 * sizeof(int)); 

		//Loop through each array 
		for(i = 0; i < m1; i++){
			left[i] = pData[l + i];
		} 
		for(j = 0; j < m2; j++){
			right[j] = pData[middle + 1 + j]; 
		}

		//Start the merging
		i = 0;
		j = 0;
		k = l;

		while(i < m1 && j < m2){
			if(left[i] <= right[j]){
				pData[k] = left[i];
				i++;  
			}
			else{
				pData[k] = right[j];
				j++;
			}
			k++; 
		}

		//Check if there is any remaining elements
		while(i  < m1){
			pData[k] = left[i]; 
			i++;
			k++;
		}

		while(j < m2){
			pData[k] = right[j];
			j++;
			k++;
		}

		//Free the memory
		DeAlloc(left);
		DeAlloc(right); 
	}
}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{
	//Initialize
	int item, i, j; 

	//First loop and make it to search for index 1
	for(i = 1; i < n; i++){
		item = pData[i]; 

		//Ineer loop to see if index zero is greater
		while (j >= 0 && pData[j] > item) {
            pData[j + 1] = pData[j];
            j--;
        }
		//Correctly places the value to the correct place
		pData[j + 1] = item;
	}
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{
	int item, i, j; 

	//Iterate thought the array
	for(i = 0; i < n-  1; i++){
		item = pData[i];

		//Inner loop
    	for (j = i - 1; j >= 0 && pData[j] > item; j--) {
            pData[j + 1] = pData[j];
        }
		pData[j + 1] = item;  
	}
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
	int i, j, minIndex, temp; 
	for(i = 0; i < n -1; i++){
		minIndex = i; 
		for(j = i + 1; j < n; j++) {
			if(pData[j] < pData[minIndex]){ 
				minIndex = j; 
			}
		}
		temp = pData[i];
		pData[i] = pData[minIndex]; 
		pData[minIndex] = temp;
	}
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		
		*ppData = (int *)Alloc(sizeof(int) * dataSz);

		// Implement parse data block
		for (int i = 0; i < dataSz; i++) {
            fscanf(inFile, "%d", &((*ppData)[i]));
        }
		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)Alloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

                printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}
	
}
