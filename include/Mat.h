/**
  ******************************************************************************
  矩阵计算的模板类
  使用std::vector实现
  * @file           : Mat.h
  * @author         : 35160
  * @date           : 2024/6/18
  ******************************************************************************
*/

#ifndef MatDisp_Mat_H
#define MatDisp_Mat_H

#include <vector>
#include <iostream>

/**
 * 矩阵转置
 */
template<typename T>
std::vector<std::vector<T>> transpose(std::vector<std::vector<T>> matrix) {
    std::vector<std::vector<T>> array;
    std::vector<T> tempArr;
    for (int i = 0; i < matrix[0].size(); ++i) {
        for (int j = 0; j < matrix.size(); ++j) {
            tempArr.push_back(matrix[j][i]);
        }
        array.push_back(tempArr);
        tempArr.erase(tempArr.begin(), tempArr.end());
    }
    return array;
}

/**
 * 一维矩阵加法
 */
template<typename T>
std::vector<T> operator+(std::vector<T> const& arrA, std::vector<T> const& arrB) {
    int length = arrA.size();
    std::vector<T> result(length);
    for (int i = 0; i < length; i++) {
        result[i] = arrA[i] + arrB[i];
    }
    return result;
}

/**
 * 一维矩阵+=运算
 */
template<typename T>
std::vector<T>& operator+=(std::vector<T>& arr_a, const std::vector<T>& arr_b) {
    for (int i = 0; i < arr_a.size(); i++) {
        arr_a[i] += arr_b[i];
    }
    return arr_a;
}

/**
 * 二维矩阵加法
 */
template<typename T>
std::vector<std::vector<T>>
operator+(std::vector<std::vector<T>> const& arr_a, std::vector<std::vector<T>> const& arr_b) {
    int length = arr_a.size();
    std::vector<std::vector<T>> result(length);
    for (int i = 0; i < length; i++) {
        result[i] = arr_a[i] + arr_b[i];
    }
    return result;
}

/**
 * 二维矩阵+=
 */
template<typename T>
std::vector<std::vector<T>>& operator+=(std::vector<std::vector<T>>& arr_a, std::vector<std::vector<T>> const& arr_b) {
    int length = arr_a.size();
    for (int i = 0; i < length; i++) {
        arr_a[i] += arr_b[i];
    }
    return arr_a;
}

/**
 * 一维矩阵减法
 */
template<typename T>
std::vector<T> operator-(std::vector<T> const& arr_a) {
    int length = arr_a.size();
    std::vector<T> result(length);
    for (int i = 0; i < length; i++) {
        result[i] = -arr_a[i];
    }
    return result;
}

/**
 * 一维矩阵乘法
 */
template<typename T>
T operator*(std::vector<T> const& arr_a, std::vector<T> const& arr_b) {
    T result = 0;
    for (int i = 0; i < arr_a.size(); i++) {
        result += arr_a[i] * arr_b[i];
    }
    return result;
}

/**
 * 二维矩阵乘一维矩阵
 */
template<typename T>
std::vector<T> operator*(std::vector<std::vector<T>> const& arr_a, std::vector<T> const& arr_b) {
    std::vector<T> result(arr_a.size());
    for (int i = 0; i < arr_a.size(); i++) {
        result[i] = arr_a[i] * arr_b;
    }
    return result;
}

/**
 * 二维矩阵乘二维矩阵
 */
template<typename T>
std::vector<std::vector<T>>
operator*(std::vector<std::vector<T>> const& arr_a, std::vector<std::vector<T>> const& arr_b) {
    int row = arr_a.size();
    int col = arr_b[0].size();
    std::vector<std::vector<T>> temp = transpose(arr_b);
    std::vector<std::vector<T>> result = std::vector<std::vector<double>>(row, std::vector<double>(col));
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            result[i][j] = arr_a[i] * temp[j];
        }
    }
    return result;
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& arr) {
    stream << "[";
    for (int i = 0; i < arr.size(); i++) {
        stream << arr[i] << (i == arr.size() - 1 ? "" : ",");
    }
    return stream << "]" << std::endl;
}

/**
 * 矩阵拼接 对v1进行修改
 */
template<typename T>
std::vector<T>& concatenate(std::vector<T>& v1, const std::vector<T> v2) {
    v1.insert(v1.end(), v2.begin(), v2.end());
    return v1;
}

template<typename T>
std::pair<bool, int> find(const std::vector<T>& arr, T data) {
    int idx = 0;
    for (const auto& i : arr) {
        if (i == data) {
            return {true, idx};
        }
        idx++;
    }
    return {false, 0}; // 没有找到则返回负数
}

/**
 * 矩阵删除元素
 */
template<typename T>
std::vector<T> deleteObj(std::vector<T> arr, const std::vector<int>& obj) {
    int newSize = arr.size() - obj.size();
    if (newSize == 0) return arr;
    if (newSize < 0) throw std::runtime_error("data to be delete oversize than origin array");
    std::vector<T> newArr(newSize);
    int deletedNum = 0;
    for (int i = 0; i < arr.size(); i++) {
        if (find(obj, i).first) {
            deletedNum++;
            continue;
        }
        newArr[i - deletedNum] = arr[i];
    }
    return newArr;
}

#endif //MatDisp_Mat_H
