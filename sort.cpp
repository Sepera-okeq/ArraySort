// //////////////////////////////////////////////////////////
// sort.cpp
// Copyright (c) 2023 Sergey Leshkevich.
//

// g++ -O3 -std=c++11 sort.cpp -o sort

#include <cstdio>
#include <cstdlib>   // srand/rand
#include <cmath>     // fabs

#include <vector>
#include <list>
#include <algorithm> // std::sort, std::reverse

#include "sort.h"


// добавьте -DCHECKRESULT в командную строку GCC => если хотите, чтобы результаты будут проверены на правильность их сортировки

// тип данных, подлежащий сортировке
typedef int Number;
typedef std::vector<Number> Container;


// защита ОС от перегрузки:
// отключите несколько очень медленных тестов, когда сортируется более чем ограниченное количество элементов
const int RestrictedSort = 25000000;
// верхний предел, никаких сортировок сверх этого количества элементов
const int MaxSort        = 10000000;

#ifdef _MSC_VER
#define USE_WINDOWS_TIMER
#endif

#ifdef USE_WINDOWS_TIMER
#include <windows.h>
#include <ctime>
#else
#include <sys/time.h>
#endif


// Время, зависящее от конкретной операционной системы
static double seconds()
{
#ifdef USE_WINDOWS_TIMER
  LARGE_INTEGER frequency, now;
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter  (&now);
  return now.QuadPart / double(frequency.QuadPart);
#else
  timeval now;
  gettimeofday(&now, NULL);
  return now.tv_sec + now.tv_usec/1000000.0;
#endif
}

// Главная функция
int main()
{
    testSortData(50);
    testSortData(500);
    testSortData(5000);
    testSortData(50000);
    testSortData(500000);
}


void testSortData(int numElements)
{

  // только положительные числа!
  if (numElements == 0)
    numElements = 10000;
  if (numElements < 0)
    numElements = -numElements;
  // избегайте перегрузки ОС
  if (numElements > MaxSort)
    numElements = MaxSort;

#ifdef LESSTHAN
  printf("%d element%s\n", numElements, numElements == 1 ? "":"s");
#else
  printf("\n%d integer%s\t\t  ascending \t descending  \t     random\t   all time\n", numElements, numElements == 1 ? "":"s");
#endif

  // инициализировать контейнер на N элементов
  // 0,1,2,3,4,...
  Container ascending(numElements);
  for (size_t i = 0; i < numElements; i++)
    ascending[i] = Number(i);

  // ...,4,3,2,1,0
  Container descending(numElements);
  for (size_t i = 0; i < numElements; i++)
    descending[i] = Number((numElements - 1) - i);
  //std::reverse(descending.begin(), descending.end());

  // просто случайные числа
  Container random(numElements);
  srand(time(NULL));//0);
  for (int i = 0; i < numElements; i++)
    random[i] = Number(rand());

#ifdef CHECKRESULT
  Container sorted = ascending;
  Container sortedRandom = random;
  std::sort(sortedRandom.begin(), sortedRandom.end());
#endif // CHECKRESULT

  // используйте этот контейнер для входных данных
  Container data;

  double timeInverted = 0;
  double timeSorted   = 0;
  double timeRandom   = 0;

#ifndef FORWARDITERATOR
  // BubbleSort
  // sorted data
  data = ascending;
  timeSorted = seconds();
  bubbleSort(data.begin(), data.end());
  timeSorted = fabs(seconds() - timeSorted);

#ifdef CHECKRESULT
  if (data != sorted)
    printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

  if (numElements < RestrictedSort)
  {
    // inverted data
    data = descending;
    timeInverted = seconds();
    bubbleSort(data.begin(), data.end());
    timeInverted = fabs(seconds() - timeInverted);

#ifdef CHECKRESULT
    if (data != sorted)
      printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

    // random data
    data = random;
    timeRandom = seconds();
    bubbleSort(data.begin(), data.end());
    timeRandom = fabs(seconds() - timeRandom);

#ifdef CHECKRESULT
    if (data != sortedRandom)
      printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

    printf("Bubble Sort\t\t%8.3f ms\t%8.3f ms\t%8.3f ms\t%8.3f ms\n",
           1000*timeSorted, 1000*timeInverted, 1000*timeRandom, 1000*(timeSorted+timeInverted+timeRandom));
  }
  else
    // skip bubble sort in order to prevent server overload
    printf("Bubble Sort\t\t%8.3f ms\tn/a\tn/a\tn/a\n", 1000*timeSorted);
#endif // FORWARDITERATOR


  // SelectionSort
  if (numElements < RestrictedSort)
  {
    // inverted data
    data = descending;
    timeInverted = seconds();
    selectionSort(data.begin(), data.end());
    timeInverted = fabs(seconds() - timeInverted);

#ifdef CHECKRESULT
    if (data != sorted)
      printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

    // sorted data
    timeSorted = seconds();
    selectionSort(data.begin(), data.end());
    timeSorted = fabs(seconds() - timeSorted);

#ifdef CHECKRESULT
    if (data != sorted)
      printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

    // random data
    data = random;
    timeRandom = seconds();
    selectionSort(data.begin(), data.end());
    timeRandom = fabs(seconds() - timeRandom);

#ifdef CHECKRESULT
    if (data != sortedRandom)
      printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

    printf("Selection Sort\t\t%8.3f ms\t%8.3f ms\t%8.3f ms\t%8.3f ms\n",
           1000*timeSorted, 1000*timeInverted, 1000*timeRandom, 1000*(timeSorted+timeInverted+timeRandom));
  }
  else
    // skip bubble sort in order to prevent server overload
    printf("Selection Sort\t\t\tn/a\tn/a\tn/a\tn/a\n");

  // SelectionSort
  if (numElements < RestrictedSort)
  {
    // inverted data
    data = descending;
    timeInverted = seconds();
    radixSort(data.begin(), data.end());
    timeInverted = fabs(seconds() - timeInverted);

#ifdef CHECKRESULT
    if (data != sorted)
      printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

    // sorted data
    timeSorted = seconds();
    radixSort(data.begin(), data.end());
    timeSorted = fabs(seconds() - timeSorted);

#ifdef CHECKRESULT
    if (data != sorted)
      printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

    // random data
    data = random;
    timeRandom = seconds();
    radixSort(data.begin(), data.end());
    timeRandom = fabs(seconds() - timeRandom);

#ifdef CHECKRESULT
    if (data != sortedRandom)
      printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

    printf("Radix Sort\t\t%8.3f ms\t%8.3f ms\t%8.3f ms\t%8.3f ms\n",
           1000*timeSorted, 1000*timeInverted, 1000*timeRandom, 1000*(timeSorted+timeInverted+timeRandom));
  }
  else
    // skip radix sort in order to prevent server overload
    printf("Radix Sort\t\t\tn/a\tn/a\tn/a\tn/a\n");


#ifndef FORWARDITERATOR
  // InsertionSort
  // sorted data
  data = ascending;
  timeSorted = seconds();
  insertionSort(data.begin(), data.end());
  timeSorted = fabs(seconds() - timeSorted);

#ifdef CHECKRESULT
  if (data != sorted)
    printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

  if (numElements < RestrictedSort)
  {
    // inverted data
    data = descending;
    timeInverted = seconds();
    insertionSort(data.begin(), data.end());
    timeInverted = fabs(seconds() - timeInverted);

#ifdef CHECKRESULT
    if (data != sorted)
      printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

    // random data
    data = random;
    timeRandom = seconds();
    insertionSort(data.begin(), data.end());
    timeRandom = fabs(seconds() - timeRandom);

#ifdef CHECKRESULT
    if (data != sortedRandom)
      printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

    printf("Insertion Sort\t\t%8.3f ms\t%8.3f ms\t%8.3f ms\t%8.3f ms\n",
           1000*timeSorted, 1000*timeInverted, 1000*timeRandom, 1000*(timeSorted+timeInverted+timeRandom));
  }
  else
    // skip bubble sort in order to prevent server overload
    printf("Insertion Sort\t\t%8.3f ms\tn/a\tn/a\tn/a\n", 1000*timeSorted);
#endif // FORWARDITERATOR


#ifndef FORWARDITERATOR
  // ShellSort
  // inverted data
  data = descending;
  timeInverted = seconds();
  shellSort(data.begin(), data.end());
  timeInverted = fabs(seconds() - timeInverted);

#ifdef CHECKRESULT
  if (data != sorted)
    printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

  // sorted data
  timeSorted = seconds();
  shellSort(data.begin(), data.end());
  timeSorted = fabs(seconds() - timeSorted);

#ifdef CHECKRESULT
  if (data != sorted)
    printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

  // random data
  data = random;
  timeRandom = seconds();
  shellSort(data.begin(), data.end());
  timeRandom = fabs(seconds() - timeRandom);

#ifdef CHECKRESULT
  if (data != sortedRandom)
    printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

  printf("Shell Sort\t\t%8.3f ms\t%8.3f ms\t%8.3f ms\t%8.3f ms\n",
         1000*timeSorted, 1000*timeInverted, 1000*timeRandom, 1000*(timeSorted+timeInverted+timeRandom));
#endif // FORWARDITERATOR


#ifndef FORWARDITERATOR
  // QuickSort
  // inverted data
  data = descending;
  timeInverted = seconds();
  quickSort(data.begin(), data.end());
  timeInverted = fabs(seconds() - timeInverted);

#ifdef CHECKRESULT
  if (data != sorted)
    printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

  // sorted data
  timeSorted = seconds();
  quickSort(data.begin(), data.end());
  timeSorted = fabs(seconds() - timeSorted);

#ifdef CHECKRESULT
  if (data != sorted)
    printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

  // random data
  data = random;
  timeRandom = seconds();
  quickSort(data.begin(), data.end());
  timeRandom = fabs(seconds() - timeRandom);

#ifdef CHECKRESULT
  if (data != sortedRandom)
    printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

  printf("Quick Sort\t\t%8.3f ms\t%8.3f ms\t%8.3f ms\t%8.3f ms\n",
         1000*timeSorted, 1000*timeInverted, 1000*timeRandom, 1000*(timeSorted+timeInverted+timeRandom));
#endif // FORWARDITERATOR


#ifndef FORWARDITERATOR
  // IntroSort
  // inverted data
  data = descending;
  timeInverted = seconds();
  introSort(data.begin(), data.end());
  timeInverted = fabs(seconds() - timeInverted);

#ifdef CHECKRESULT
  if (data != sorted)
    printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

  // sorted data
  timeSorted = seconds();
  introSort(data.begin(), data.end());
  timeSorted = fabs(seconds() - timeSorted);

#ifdef CHECKRESULT
  if (data != sorted)
    printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

  // random data
  data = random;
  timeRandom = seconds();
  introSort(data.begin(), data.end());
  timeRandom = fabs(seconds() - timeRandom);

#ifdef CHECKRESULT
  if (data != sortedRandom)
    printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

  printf("Intro Sort\t\t%8.3f ms\t%8.3f ms\t%8.3f ms\t%8.3f ms\n",
         1000*timeSorted, 1000*timeInverted, 1000*timeRandom, 1000*(timeSorted+timeInverted+timeRandom));
#endif // FORWARDITERATOR


#if !defined(FORWARDITERATOR) && !defined(BIDIRECTIONALITERATOR)
  // HeapSort
  // inverted data
  data = descending;
  timeInverted = seconds();
  heapSort(data.begin(), data.end());
  timeInverted = fabs(seconds() - timeInverted);

#ifdef CHECKRESULT
  if (data != sorted)
    printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

  // sorted data
  timeSorted = seconds();
  heapSort(data.begin(), data.end());
  timeSorted = fabs(seconds() - timeSorted);

#ifdef CHECKRESULT
  if (data != sorted)
    printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

  // random data
  data = random;
  timeRandom = seconds();
  heapSort(data.begin(), data.end());
  timeRandom = fabs(seconds() - timeRandom);

#ifdef CHECKRESULT
  if (data != sortedRandom)
    printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

  printf("Heap Sort\t\t\t%8.3f ms\t%8.3f ms\t%8.3f ms\t%8.3f ms\n",
          1000*timeSorted, 1000*timeInverted, 1000*timeRandom, 1000*(timeSorted+timeInverted+timeRandom));
#endif // !defined(FORWARDITERATOR) && !defined(BIDIRECTIONALITERATOR)


#if !defined(FORWARDITERATOR)
  // n-ary HeapSort
  // inverted data
  //data = descending;
  //timeInverted = seconds();
  //naryHeapSort<8>(data.begin(), data.end());
  //timeInverted = fabs(seconds() - timeInverted);

#ifdef CHECKRESULT
  //if (data != sorted)
  //  printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

  // sorted data
  //timeSorted = seconds();
  //naryHeapSort<8>(data.begin(), data.end());
  //timeSorted = fabs(seconds() - timeSorted);

#ifdef CHECKRESULT
  //if (data != sorted)
  //  printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

  // random data
  //data = random;
  //timeRandom = seconds();
  //naryHeapSort<8>(data.begin(), data.end());
  //timeRandom = fabs(seconds() - timeRandom);

#ifdef CHECKRESULT
  //if (data != sortedRandom)
  //  printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

  //printf("n-ary Heap Sort (n=8)\t%8.3f ms\t%8.3f ms\t%8.3f ms\t%8.3f ms\n",
  //        1000*timeSorted, 1000*timeInverted, 1000*timeRandom, 1000*(timeSorted+timeInverted+timeRandom));
#endif // !defined(FORWARDITERATOR)


#if !defined(FORWARDITERATOR) && !defined(BIDIRECTIONALITERATOR)
  // MergeSort
  // inverted data
  data = descending;
  timeInverted = seconds();
  mergeSort(data.begin(), data.end());
  timeInverted = fabs(seconds() - timeInverted);

#ifdef CHECKRESULT
  if (data != sorted)
    printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

  // sorted data
  timeSorted = seconds();
  mergeSort(data.begin(), data.end());
  timeSorted = fabs(seconds() - timeSorted);

#ifdef CHECKRESULT
  if (data != sorted)
    printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

  // random data
  data = random;
  timeRandom = seconds();
  mergeSort(data.begin(), data.end());
  timeRandom = fabs(seconds() - timeRandom);

#ifdef CHECKRESULT
  if (data != sortedRandom)
    printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

  printf("Merge Sort\t\t%8.3f ms\t%8.3f ms\t%8.3f ms\t%8.3f ms\n",
          1000*timeSorted, 1000*timeInverted, 1000*timeRandom, 1000*(timeSorted+timeInverted+timeRandom));
#endif // !defined(FORWARDITERATOR) && !defined(BIDIRECTIONALITERATOR)


  // in-place MergeSort
  // sorted data
  data = ascending;
  timeSorted = seconds();
  mergeSortInPlace(data.begin(), data.end());
  timeSorted = fabs(seconds() - timeSorted);

#ifdef CHECKRESULT
  if (data != sorted)
    printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

  if (numElements < RestrictedSort)
  {
    // inverted data
    data = descending;
    timeInverted = seconds();
    mergeSortInPlace(data.begin(), data.end());
    timeInverted = fabs(seconds() - timeInverted);

#ifdef CHECKRESULT
    if (data != sorted)
      printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

    // random data
    data = random;
    timeRandom = seconds();
    mergeSortInPlace(data.begin(), data.end());
    timeRandom = fabs(seconds() - timeRandom);

#ifdef CHECKRESULT
    if (data != sortedRandom)
      printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

    printf("Merge Sort in-place\t\t%8.3f ms\t%8.3f ms\t%8.3f ms\t%8.3f ms\n",
            1000*timeSorted, 1000*timeInverted, 1000*timeRandom, 1000*(timeSorted+timeInverted+timeRandom));
  }
  else
    printf("Merge Sort in-place\t\t%8.3f ms\tn/a\tn/a\tn/a\n", 1000*timeSorted);


#if !defined(FORWARDITERATOR) && !defined(BIDIRECTIONALITERATOR)
  // std::sort
  // inverted data
  data = descending;
  timeInverted = seconds();
  std::sort(data.begin(), data.end());
  timeInverted = fabs(seconds() - timeInverted);

  // sorted data
  timeSorted = seconds();
  std::sort(data.begin(), data.end());
  timeSorted = fabs(seconds() - timeSorted);

  // random data
  data = random;
  timeRandom = seconds();
  std::sort(data.begin(), data.end());
  timeRandom = fabs(seconds() - timeRandom);

  printf("std::sort\t\t\t%8.3f ms\t%8.3f ms\t%8.3f ms\t%8.3f ms\n",
         1000*timeSorted, 1000*timeInverted, 1000*timeRandom, 1000*(timeSorted+timeInverted+timeRandom));

  // std::stable_sort
  // inverted data
  data = descending;
  timeInverted = seconds();
  std::stable_sort(data.begin(), data.end());
  timeInverted = fabs(seconds() - timeInverted);

  // sorted data
  timeSorted = seconds();
  std::stable_sort(data.begin(), data.end());
  timeSorted = fabs(seconds() - timeSorted);

  // random data
  data = random;
  timeRandom = seconds();
  std::stable_sort(data.begin(), data.end());
  timeRandom = fabs(seconds() - timeRandom);

  printf("std::stable_sort\t\t%8.3f ms\t%8.3f ms\t%8.3f ms\t%8.3f ms\n",
         1000*timeSorted, 1000*timeInverted, 1000*timeRandom, 1000*(timeSorted+timeInverted+timeRandom));
#endif // !defined(FORWARDITERATOR) && !defined(BIDRECTIONALITERATOR)

  return;
}
