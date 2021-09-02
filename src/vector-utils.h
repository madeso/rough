#pragma once

#include <vector>
#include <algorithm>
#include <iterator>

template<typename T, typename SortFunction>
void sort(std::vector<T>& vec, SortFunction&& sortFunction)
{
    std::stable_sort(vec.begin(), vec.end(),
        [&sortFunction](const T& lhs, const T& rhs) {
            return sortFunction(lhs, rhs) > 0;
        });
}

template<typename T, typename FilterFunction>
std::vector<T> filter(const std::vector<T>& vec, FilterFunction&& filterFunction)
{
    std::vector<T> result;
    result.reserve(vec.size());
    std::copy_if(vec.begin(), vec.end(), std::back_inserter(result), filterFunction);
    return result;
}

