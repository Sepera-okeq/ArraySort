// //////////////////////////////////////////////////////////
// sort.h
// Copyright (c) 2023 Sergey Leshkevich.

// g++ -O3 sort.cpp -o sort -std=c++11

// Весь алгоритм сортировки следует тому же синтаксису, что и std::sort
// т.е.: быстрая сортировка(container.begin(), container.end());
//
// Все виды сортировки, кроме сортировки слиянием, не требуют значительной дополнительной памяти
// (просто некоторый стек для нескольких переменных и, возможно, копия одного элемента).
//
// Чтобы реализовать новую сортировку, реализуйте less-than operator.
// т.е.: quickSort(container.begin(), container.end(), myless());

#pragma once

#include <algorithm>  // std::iter_swap
#include <iterator>   // std::advance, std::iterator_traits
#include <functional> // std::less
#include <numeric>
#include <array>
#include <vector>     // std::vector

/// Сортировка тестовых данных (оболочка, где мы и сортируем)
/// Передаем количество элементов, все остальное за нас сделает генератор.
void testSortData(int numElements);

/// BubbleSort, реализация
template <typename iterator, typename LessThan>
void bubbleSort(iterator first, iterator last, LessThan lessThan)
{
  if (first == last)
    return;

  // обычно "последний" указывает за пределы последнего элемента
  // теперь он указывает непосредственно на этот последний элемент
  --last;
  // только один элемент => он отсортирован
  if (first == last)
    return;

  bool swapped;
  do
  {
    // сбросить замененный флаг
    swapped = false;

    auto current = first;
    while (current != last)
    {
      // пузыриться вверх
      auto next = current;
      ++next;

      // два соседа в неправильном порядке ? поменяйте их местами!
      if (lessThan(*next, *current))
      {
        std::iter_swap(current, next);
        swapped = true;
      }

      ++current;
    }

    // последний элемент уже отсортирован
    --last; // удалите эту строку, если у вас есть только прямой итератор

    // последний будет приближаться к первому
  } while (swapped && first != last);
}


/// BubbleSort
template <typename iterator>
void bubbleSort(iterator first, iterator last)
{
  bubbleSort(first, last, std::less<typename std::iterator_traits<iterator>::value_type>());
}

/// RadixSort, реализация поразрядной сортировки
template<typename iterator>
void radix_sort(iterator first, iterator last, std::less<typename std::iterator_traits<iterator>::value_type>)
{
    if (first == last) return;

    using value_type = typename std::iterator_traits<iterator>::value_type;
    std::vector<value_type> buffer(std::distance(first, last));

    for (int shift = 0; shift < 8 * sizeof(value_type); shift += 8)
    {
        auto mask = 0xFF << shift;
        std::array<int, 256> count{};
        for (auto it = first; it != last; ++it)
        {
            ++count[(*it & mask) >> shift];
        }
        std::partial_sum(count.begin(), count.end(), count.begin());
        for (auto it = last - 1; it >= first; --it)
        {
            buffer[--count[(*it & mask) >> shift]] = std::move(*it);
        }
        std::move(buffer.begin(), buffer.begin() + std::distance(first, last), first);
    }
}

/// RadixSort
template <typename iterator>
void radixSort(iterator first, iterator last)
{
  radix_sort(first, last, std::less<typename std::iterator_traits<iterator>::value_type>());
}

// /////////////////////////////////////////////////////////////////////


/// Selection Sort, реализация
template <typename iterator, typename LessThan>
void selectionSort(iterator first, iterator last, LessThan lessThan)
{
  for (iterator current = first; current != last; ++current)
  {
    // найдите наименьший элемент в несортированной части и запомните его итератор в "minimum"
    auto minimum = current;
    auto compare = current;
    ++compare;

    // пройтись по всем еще несортированным элементам
    while (compare != last)
    {
      // найден новый минимум ? сохраните его итератор
      if (lessThan(*compare, *minimum))
        minimum = compare;

      // следующий элемент
      ++compare;
    }

    // добавить минимум в конец уже отсортированной части
    if (current != minimum)
      std::iter_swap(current, minimum);
  }
}


/// Selection Sort
template <typename iterator>
void selectionSort(iterator first, iterator last)
{
  selectionSort(first, last, std::less<typename std::iterator_traits<iterator>::value_type>());
}


// /////////////////////////////////////////////////////////////////////


/// Insertion Sort, реализация
template <typename iterator, typename LessThan>
void insertionSort(iterator first, iterator last, LessThan lessThan)
{
  if (first == last)
    return;

  // пропустите первый элемент, считайте его отсортированным
  auto current = first;
  ++current;

  // вставить все оставшиеся несортированные элементы в отсортированные элементы
  while (current != last)
  {
    // вставить "сравнить" в уже отсортированные элементы
    auto compare = std::move(*current);

    // найти местоположение внутри отсортированного диапазона, начиная с правого конца
    auto pos = current;
    while (pos != first)
    {
      // остановить, если левый сосед не меньше
      auto left = pos;
      --left;
      if (!lessThan(compare, *left))
        break;

      // сдвиньте этого левого соседа на одну позицию вправо
      *pos = std::move(*left);
      pos  = std::move( left); // PS: то же, что и --pos
    }

    // найдено конечное положение
    if (pos != current)
      *pos = std::move(compare);

    // отсортировать следующий элемент
    ++current;
  }
}


/// Insertion Sort
template <typename iterator>
void insertionSort(iterator first, iterator last)
{
  insertionSort(first, last, std::less<typename std::iterator_traits<iterator>::value_type>());
}


// /////////////////////////////////////////////////////////////////////


/// Shell Sort, реализация
template <typename iterator, typename LessThan>
void shellSort(iterator first, iterator last, LessThan lessThan)
{
  auto numElements = std::distance(first, last);
  if (numElements <= 1)
    return;

  // последовательность взята из Википедии (Марцин Чура)
  static const int OptimalIncrements[] =
  { 68491, 27396, 10958, 4383, 1750, 701, 301, 132, 57, 23, 10, 4, 1, 0 };
  size_t increment = OptimalIncrements[0];
  size_t incrementIndex = 0;
  // приращение не должно быть больше количества сортируемых элементов
  while (increment >= numElements)
    increment = OptimalIncrements[++incrementIndex];

  // перебирать все приращения в порядке убывания
  while (increment > 0)
  {
    auto stripe = first;
    auto offset = increment;
    std::advance(stripe, offset);
    while (stripe != last)
    {
      // эти итераторы всегда "увеличиваются" друг от друга
      auto right = stripe;
      auto left  = stripe;
      std::advance(left, -int(increment));

      // значение, подлежащее сортировке
      auto compare = *right;

      // примечание: полоса просто такая же, как первая + смещение
      // но operator+() является дорогостоящим для итераторов с произвольным доступом
      auto posRight = offset;
      // смотрите только на значения между "первым" и "последним"
      while (true)
      {
        // нашли нужное место ?
        if (!lessThan(compare, *left))
          break;

        // нет, все еще неправильный порядок: сдвиньте больший элемент вправо
        *right = std::move(*left);

        // сделайте один шаг влево
        right  = left;

        posRight -= increment;
        if (posRight < increment)
          break;
        std::advance(left, -int(increment));
      }

      // найдена отсортированная позиция
      if (posRight != offset)
        *right = std::move(compare);

      // следующее
      ++stripe;
      ++offset;
    }

    // меньшее приращение
    increment = OptimalIncrements[incrementIndex++];
  }
}


/// Shell Sort
template <typename iterator>
void shellSort(iterator first, iterator last)
{
  shellSort(first, last, std::less<typename std::iterator_traits<iterator>::value_type>());
}


// /////////////////////////////////////////////////////////////////////


/// Heap Sort, реализация
template <typename iterator, typename LessThan>
void heapSort(iterator first, iterator last, LessThan lessThan)
{
  // просто используйте STL-код
  std::make_heap(first, last, lessThan);
  std::sort_heap(first, last, lessThan);
}


/// Heap Sort
template <typename iterator>
void heapSort(iterator first, iterator last)
{
  // просто используйте STL-код
  std::make_heap(first, last);
  std::sort_heap(first, last);
}

// /////////////////////////////////////////////////////////////////////


/// Merge Sort, реализация
template <typename iterator, typename LessThan>
void mergeSort(iterator first, iterator last, LessThan lessThan, size_t size = 0)
{
  // определить размер, если он еще не известен
  if (size == 0 && first != last)
    size = std::distance(first, last);
  // кстати, параметр size можно опустить,
  // но тогда мы должны вычислять его каждый раз,
  // что может быть дорогостоящим для итераторов с произвольным доступом

  // один элемент всегда сортируется
  if (size <= 1)
    return;

  // разделить на две части
  auto firstHalf  = size / 2;
  auto secondHalf = size - firstHalf;
  auto mid = first;
  std::advance(mid, firstHalf);

  // рекурсивно сортировать их
  mergeSort(first, mid,  lessThan, firstHalf);
  mergeSort(mid,   last, lessThan, secondHalf);

  // объединить отсортированные разделы
  std::inplace_merge(first, mid, last, lessThan);
}


/// Merge Sort
template <typename iterator>
void mergeSort(iterator first, iterator last)
{
  mergeSort(first, last, std::less<typename std::iterator_traits<iterator>::value_type>());
}


// /////////////////////////////////////////////////////////////////////


/// in-place Merge Sort, реализация
template <typename iterator, typename LessThan>
void mergeSortInPlace(iterator first, iterator last, LessThan lessThan, size_t size = 0)
{
  // определить размер, если он еще не известен
  if (size == 0 && first != last)
    size = std::distance(first, last);
  // кстати, параметр size можно опустить,
  // но тогда мы должны вычислять его каждый раз,
  // что может быть дорогостоящим для итераторов с произвольным доступом

  // один элемент всегда сортируется
  if (size <= 1)
    return;

  // разделить на две части
  auto firstHalf  = size / 2;
  auto secondHalf = size - firstHalf;
  auto mid = first;
  std::advance(mid, firstHalf);

  // рекурсивно сортировать их
  mergeSortInPlace(first, mid,  lessThan, firstHalf);
  mergeSortInPlace(mid,   last, lessThan, secondHalf);

  // объединить разделы (левый начинается с "first", правый начинается с "mid")
  // перемещайте итераторы ближе к концу, пока они не встретятся
  auto right = mid;
  while (first != mid)
  {
    // следующее значение обоих разделов в неправильном порядке (меньший раздел находится слева)
    if (lessThan(*right, *first))
    {
      // это значение должно быть перемещено в нужный раздел
      auto misplaced = std::move(*first);

      // это значение относится к левому разделу
      *first = std::move(*right);

      // неуместное значение должно быть вставлено в правильное положение в нужном разделе
      auto scan = right;
      auto next = scan;
      ++next;
      // переместить меньший размер на одну позицию влево
      while (next != last && lessThan(*next, misplaced))
        *scan++ = std::move(*next++);

      // нашел нужное место!
      *scan = std::move(misplaced);
    }

    ++first;
  }
}


/// in-place Merge Sort
template <typename iterator>
void mergeSortInPlace(iterator first, iterator last)
{
  mergeSortInPlace(first, last, std::less<typename std::iterator_traits<iterator>::value_type>());
}


// /////////////////////////////////////////////////////////////////////


/// Quick Sort, реализация
template <typename iterator, typename LessThan>
void quickSort(iterator first, iterator last, LessThan lessThan)
{
  auto numElements = std::distance(first, last);
  // уже отсортировано ?
  if (numElements <= 1)
    return;

  auto pivot = last;
  --pivot;

  // выберите средний элемент в качестве опорного (хороший выбор для частично отсортированных данных)
  if (numElements > 2)
  {
    auto middle = first;
    std::advance(middle, numElements/2);
    std::iter_swap(middle, pivot);
  }

  // сканируйте, начиная с левого и правого концов, и меняйте местами неуместные элементы
  auto left  = first;
  auto right = pivot;
  while (left != right)
  {
    // ищите несоответствия
    while (!lessThan(*pivot, *left)  && left != right)
      ++left;
    while (!lessThan(*right, *pivot) && left != right)
      --right;
    // поменяйте местами два значения, которые оба находятся на неправильной стороне сводного элемента
    if (left != right)
      std::iter_swap(left, right);
  }

  // переместить ось поворота в ее конечное положение
  if (pivot != left && lessThan(*pivot, *left))
    std::iter_swap(pivot, left);

  quickSort(first,  left, lessThan);
  quickSort(++left, last, lessThan); // *сам left уже отсортирован!!!!
}


/// Quick Sort
template <typename iterator>
void quickSort(iterator first, iterator last)
{
  quickSort(first, last, std::less<typename std::iterator_traits<iterator>::value_type>());
}


// /////////////////////////////////////////////////////////////////////


/// Intro Sort, реализация
template <typename iterator, typename LessThan>
void introSort(iterator first, iterator last, LessThan lessThan)
{
  // переключитесь на сортировку по вставке, если массив (вложенный) невелик
  auto numElements = std::distance(first, last);
  if (numElements <= 16)
  {
    // уже отсортировано ?
    if (numElements <= 1)
      return;

    // микрооптимизация ровно для 2 элементов
    if (numElements == 2)
    {
      if (lessThan(*(first + 1), *first))
        std::iter_swap(first + 1, first);
      return;
    }

    // от 3 до 16 элементов
    insertionSort(first, last, lessThan);
    return;
  }

  auto pivot = last;
  --pivot;

  // выберите средний элемент в качестве опорного (хороший выбор для частично отсортированных данных)
  auto middle = first;
  std::advance(middle, numElements/2);
  std::iter_swap(middle, pivot);

  // сканируйте, начиная с левого и правого концов, и меняйте местами неуместные элементы
  auto left  = first;
  auto right = pivot;
  while (left != right)
  {
    // ищите несоответствия
    while (!lessThan(*pivot, *left)  && left != right)
      ++left;
    while (!lessThan(*right, *pivot) && left != right)
      --right;
    // поменяйте местами два значения, которые оба находятся на неправильной стороне сводного элемента
    if (left != right)
      std::iter_swap(left, right);
  }

  // переместить ось поворота в ее конечное положение
  if (pivot != left && lessThan(*pivot, *left))
    std::iter_swap(pivot, left);

  introSort(first,  left, lessThan);
  introSort(++left, last, lessThan); // *сам left уже отсортирован!!!
}


/// Intro Sort
template <typename iterator>
void introSort(iterator first, iterator last)
{
  introSort(first, last, std::less<typename std::iterator_traits<iterator>::value_type>());
}
