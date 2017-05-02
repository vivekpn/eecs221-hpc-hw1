#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <iostream>
#include "sort.hh"
#include <string.h>
#include <math.h>

using namespace std;

void print_keytype(keytype* values, int size);
keytype* merge(keytype* leftsorted, int left_size, keytype* rightsorted, int right_size);
int binary_insert(keytype* B, int size, keytype value);
keytype* pmergesort(int N, keytype* A);
keytype* mergesort(int N, keytype* A, int from, int to);

int depth = 5; // MAX_FUNCTION_CALL_DEPTH for parallelizing.

/*
void parallelfor(){
	int* a = new int[6];
	int i;
	omp_set_num_threads(2);
	#pragma omp parallel shared (a) private (i)
	{

		#pragma omp for
		for (i = 0; i < 6; ++i) {
			printf("%d ", i);
			a[i] += i;
		} // Implicit barrier/join
	} // Implicit barrier/join
}

void helloworld(){
	omp_set_num_threads(2);
	#pragma omp parallel
	{
		printf("hello, world!\n"); // Execute in parallel
	} // Implicit barrier/join
}
*/

void print_keytype(keytype* values, int size){
	for(int i = 0; i< size; i++){
		cout << values[i] << " ";
	}
	cout << endl;
}

keytype* merge(keytype* leftsorted, int left_size, keytype* rightsorted, int right_size){
	keytype* merged = new keytype[left_size + right_size];
	int ptr1 = 0;
	int ptr2 = 0;
	while(ptr1 < left_size && ptr2 < right_size){
		if(leftsorted[ptr1] < rightsorted[ptr2]){
			merged[ptr1 + ptr2] = leftsorted[ptr1];
			ptr1++;
		} else{
			merged[ptr1 + ptr2] = rightsorted[ptr2];
			ptr2++;
		}
	}

	while(ptr1 < left_size || ptr2 < right_size){
		if(ptr1 < left_size){
			merged[ptr1 + ptr2] = leftsorted[ptr1];
			ptr1++;
		} else{
			merged[ptr1 + ptr2] = rightsorted[ptr2];
			ptr2++;
		}
	}
	return merged;
}


int binary_insert(keytype* B, int size, keytype value){
	int left = 0;
	int right = size - 1;
	while(left <= right){
		unsigned int mid = left + (right - left) / 2;
		if(B[mid] == value) {
			return mid;
		}
		if(B[mid] < value) {
			left = mid + 1;
		} else {
			right = mid - 1;
		}
	}
	return left;
}


keytype* combine(keytype* c1, int size1,keytype v, keytype* c2, int size2){
        int i=0;
	    keytype* array = new keytype[size1+size2+1];
        for(i=0;i<size1;i++)
        array[i] = c1[i];

        array[i] = v;
        i++;

        for(int j=0;j<size2;j++)
        {
        array[i++] = c2[j];
        }
        return array;

}

keytype* smerge(keytype* A, int size1, keytype* B, int size2){
    if(size1 == 0)
    	return B;

	if (size2 ==0)
    	return A;

    int vindex = size1 / 2;
	keytype v = A[vindex];
	keytype* A2 = A + vindex + 1;
	int k = binary_insert(B, size2, v);
	keytype* B2 = B + k;
	keytype* c1 = smerge(A, vindex, B, k );
	int c1size = vindex + k;
	keytype* c2 = smerge(A2, size1 - vindex - 1, B2, size2 - k);
	int c2size = size1 - vindex - 1 + size2 - k;
	return combine(c1, c1size, v, c2, c2size);
}


keytype* pmerge(keytype* A, int size1, keytype* B, int size2, int level){
/*	if( level > depth){
		return smerge(A, size1, B, size2);
	} */
	
    if(size1 == 0)
    	return B;


	if (size2 ==0)
    	return A;

    int vindex = size1 / 2;
	keytype v = A[vindex];
	keytype* A2 = A + vindex + 1;
	int k = binary_insert(B, size2, v);
	keytype* B2 = B + k;
	keytype* c1;
//	#pragma omp task
	c1 = pmerge(A, vindex, B, k, level + 1);
	int c1size = vindex + k;
	keytype* c2 = pmerge(A2, size1 - vindex - 1, B2, size2 - k,  level + 1);
	
	int c2size = size1 - vindex - 1 + size2 - k;
//	#pragma omp taskwait
	return combine(c1, c1size, v, c2, c2size);
}


keytype* smergesort(int N, keytype* A){		
	if( N < 2){
		return A;
	}
	int mid = N / 2;
	keytype* leftsorted;
	leftsorted = smergesort(mid, A);
	keytype* rightsorted = smergesort(N-mid, A+mid);	
	keytype* returned = smerge(leftsorted, mid, rightsorted, N-mid);
	return returned;
}



keytype* pmergesort(int N, keytype* A, int level){		
/*	if( level > depth ){
		return smergesort(N, A);
	} */
	
	if( N < 2){
		return A;
	}
	int mid = N / 2;
	keytype* leftsorted;
	#pragma omp task
	{
		leftsorted = pmergesort(mid, A, level + 1);
	}
	keytype* rightsorted = pmergesort(N-mid, A+mid, level + 1);	
	
	#pragma omp taskwait

	cout << "left sorted" << endl;
	print_keytype(leftsorted, mid);
	cout << "right sorted" << endl;
	print_keytype(rightsorted, N-mid);
		
	keytype* returned = pmerge(leftsorted, mid, rightsorted, N-mid, level + 1);
	return returned;
}


keytype* mergesort(int N, keytype* A, int from, int to){
	if( N < 2){
		keytype* array = new keytype[N];
		for(int i = 0; i< N; i++){
			array[i] = A[from + i];
		}
		return array;
	}
	int mid = (from + to)/2;
	keytype* leftsorted = mergesort(mid-from + 1, A, from, mid);
	keytype* rightsorted = mergesort(to-mid, A, mid+1, to);	
	return merge(leftsorted, mid-from + 1, rightsorted, to-mid);
}

void parallelSort (int N, keytype* A){
	keytype* merged;
	#pragma omp parallel
	{ 
		#pragma omp master
		{
			int threads = omp_get_max_threads();
			cout << "Number of threads:" << threads << endl;
			depth = log2(threads);
			cout << "Maximum function call depth:" << depth << endl;			
		}
		#pragma omp single
		{
			merged = pmergesort(N, A, 0);
		}
//	 	#pragma omp for shared (A,N, merged) private (i)
	}
	
	for(int i=0;i<N;i++){
		A[i] = merged[i];
	}

}
