/**
 *  \file parallel-mergesort.cc
 *
 *  \brief Implement your parallel mergesort in this file.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <iostream>
using namespace std;
#include "sort.hh"
#include <string.h>

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
//	cout << left_size  << " " << right_size << endl;
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
//	cout << "merged" << endl;
//	print_keytype(merged, left_size + right_size);

	return merged;
}


int binary_search(keytype* B, int value){
	

}






keytype* pmerge(keytype* A, int size1, keytype* B, int size2){
	int vindex = size1 / 2;
	keytype v = A[vindex];
	keytype* A2 = A + vindex + 1;
	int k = binary_search(A+size1, v);
	keytype* B2 = B + k
	keytype* c1 = pmerge(A, vindex, B, k);
	int c1size = vindex + k;
	keytype* c2 = pmerge(A2, size1 - vindex - 1, B2, size2 - k);
	int c2size = size1 - vindex - 1 + size2 - k;
	return combine(c1, c1size, v, c2, c2size);
}




void pmergesort(int N, ketype* A){
	if( N < 2){
		return;
	}
	int mid = N / 2;
	keytype* leftsorted = pmergesort(mid, A);
	keytype* rightsorted = pmergesort(N-mid, A+mid);
	pmerge(A, mid, A+ mid, N-mid);
	return;
}



keytype* mergesort(int N, keytype* A, int from, int to){
	if( N < 2){
		keytype* array = new keytype[N];
		for(int i = 0; i< N; i++){
			array[i] = A[from + i];
		}
//		cout << "small input" << endl;
//		print_keytype(array, N);
		return array;
	}
	int mid = (from + to)/2;
	keytype* leftsorted = mergesort(mid-from + 1, A, from, mid);
	keytype* rightsorted = mergesort(to-mid, A, mid+1, to);
//	cout << "left sorted" << endl;
//	print_keytype(leftsorted, mid-from + 1);
//	cout << "right sorted" << endl;
//	print_keytype(rightsorted, to-mid);
	return merge(leftsorted, mid-from + 1, rightsorted, to-mid);
}


void parallelSort (int N, keytype* A){
//	helloworld();
//	parallelfor();
//	cout << "input" << endl;
//	print_keytype(A, N);
	merged = pmergesort(N, A);
//	cout << "sorted" << endl;
//	print_keytype(merged, N);
//        for(int i=0;i<N;i++){
//        	A[i] = merged[i];
//        }
}
