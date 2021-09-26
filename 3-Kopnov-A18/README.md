
# Table of Contents

1.  [Лабораторная A: Связные списки](#orgbf0e9ac)
    1.  [Вариант 18. Сортировка односвязного списка.](#orge267dbc)
    2.  [Алгоритм сортировки](#org65e17ca)



<a id="orgbf0e9ac"></a>

# Лабораторная A: Связные списки


<a id="orge267dbc"></a>

## Вариант 18. Сортировка односвязного списка.

Пусть вам дан односвязный список, элементами которого являются ASCII-Z строки. Вам требуется отсортировать данный список.

-   Придумайте и реализуйте алгоритм сортировки в данном случае. Какова его алгоритмическая сложность?


<a id="org65e17ca"></a>

## Алгоритм сортировки

Я использую алгоритм сортировки вставками, имеющий сложность `O(n^2)`, где `n` размер массива, в котором используется функция сравнения строк, имеющая сложность `O(k)`, где `k` длина строки.
Результирующий алгоритм имеет сложность `O(n^2 * k)`
