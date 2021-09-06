#pragma once

#include <vector>
#include <algorithm>
#include <iterator>
#include <string>

template<typename T, typename SortFunction>
void sort(std::vector<T>& vec, SortFunction&& sortFunction)
{
    std::stable_sort(vec.begin(), vec.end(),
        [&sortFunction](const T& lhs, const T& rhs) {
            return sortFunction(lhs, rhs) > 0;
        });
}

template<typename T, typename SortFunction>
std::vector<T> sorted(const std::vector<T>& vec, SortFunction&& sortFunction)
{
    auto result = vec;
    sort(result, sortFunction);
    return result;
}

template<typename T, typename FilterFunction>
std::vector<T> filter(const std::vector<T>& vec, FilterFunction&& filterFunction)
{
    std::vector<T> result;
    result.reserve(vec.size());
    std::copy_if(vec.begin(), vec.end(), std::back_inserter(result), filterFunction);
    return result;
}

template<typename T>
void insert(std::vector<T>* vec, const std::vector<T>& other)
{
    vec->insert(vec->end(), other.begin(), other.end());
}

template<typename TResult, typename TSource, typename TFunction>
std::vector<TResult> map(const std::vector<TSource>& vec, TFunction&& function)
{
    std::vector<TResult> result;
    result.reserve(vec.size());
    std::transform(vec.begin(), vec.end(), std::back_inserter(result), function);
    return result;
}

template<typename T>
T pop_front(std::vector<T>* vec)
{
    const T result = vec->front();
    vec->erase(vec->begin());
    return result;
}

template<typename T>
std::string join(const std::vector<T>& vec, const std::string& separator, const std::string& prefix="", const std::string& suffix="")
{
    if (vec.empty())
    {
        return prefix + suffix;
    }
    
    std::string result = prefix;
    result += std::to_string(vec[0]);
    for (size_t i = 1; i < vec.size(); ++i)
    {
        result += separator;
        result += std::to_string(vec[i]);
    }
    result += suffix;

    return result;
}
