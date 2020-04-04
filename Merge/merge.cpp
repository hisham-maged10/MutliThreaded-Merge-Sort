#include <iostream>
#include <pthread.h>
#include <fstream>
using namespace std;
void merge_sort(int*, int);
void* divide(void*);
void conquer(int*, int, int, int);
void getArray(int*, int);
void output(int*,int);
void doThreadedMerge();

ifstream mergeInput;
struct Arguments
{
    int* arr;
    int start;
    int end;
};


int main()
{
    doThreadedMerge(); // main method that is responsible for the operation of the program
    return 0;
}

void doThreadedMerge()
{
    mergeInput.open("input.txt"); // opens file stream with input file
    if(!mergeInput) // if not exists or cant open
    {
        cout << "Cant read from file, terminating" <<endl;                              
        exit(1);
    }
    int size;
    mergeInput >> size; // getting size of array
    int arr[size];
    getArray(arr,size); // reading array from file
    merge_sort(arr,size); // doing merge sort on array
    output(arr,size); // outputting array on stdout
    mergeInput.close();
}

// reads input array from file
void getArray(int* arr, int size)
{
    for(int i = 0 ; i < size ; ++i)
        mergeInput >> arr[i];
}
// sorts the array using threaded merge sort algorithm
void merge_sort(int* arr, int size)
{
    pthread_t mergeSortThread; // holds the thread that merge sort executes in
    Arguments mergeArg; // struct that holds the array pointer, start, end
    mergeArg.arr = arr; mergeArg.start = 0; mergeArg.end = size - 1; //initializing struct variable
    pthread_create(&mergeSortThread,NULL,divide,&mergeArg); // creating a thread to execute merge sort
    pthread_join(mergeSortThread,NULL); // waiting for the thread to finish
}
// outputs array on std out
void output(int* arr,int size)
{
    for(int i = 0 ; i < size ; ++i)
    {
        cout << arr[i] <<" ";
    }
    cout <<endl;
}
// divide part of merge sort
void* divide(void* arg)
{
    Arguments* args = (Arguments*) arg; // passing of arg of created thread
    int* arr = args->arr;
    int start = args->start;
    int end = args->end;
    if(start == end) // exiting thread using base case that start idx is equal to end idx
        pthread_exit(NULL);
    int mid = (end - start + 1)/2 + start; // getting middle index within range
    //creating two threads, each thread creates two thread recursively
    //making each thread for each division of array
    // then conquer them sorting them, after waiting for each thread to finish
    pthread_t half1, half2;
    Arguments arg1,arg2;
    arg1.arr = arr; arg1.start = start; arg1.end = mid - 1;
    pthread_create(&half1,NULL,divide,&arg1);
    arg2.arr = arr; arg2.start = mid; arg2.end = end;
    pthread_create(&half2,NULL,divide,&arg2);
    pthread_join(half1,NULL);
    pthread_join(half2,NULL);
    conquer(arr,start,mid,end);
}

// conquer part of merge sort where it sorts the array using two pointers on each half
void conquer(int* arr, int start, int mid, int end)
{
    int ptr1 = start, ptr2 = mid , end1 = mid -1, size = end - start + 1, sorted[size];

    for(int i = 0 ; i < size ; ++i)
    {
        if(ptr1 > end1)
            sorted[i] = arr[ptr2++];
        else if(ptr2 > end)
            sorted[i] = arr[ptr1++];
        else if(arr[ptr1] < arr[ptr2])
            sorted[i] = arr[ptr1++];
        else 
            sorted[i] = arr[ptr2++];  
    }

    for(int i = start, j = 0 ; i <= end ; ++i, ++j)
    {
        arr[i] = sorted[j];
    }
}

