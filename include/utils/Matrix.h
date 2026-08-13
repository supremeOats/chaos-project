#pragma once

#include <cstdint>
#include <stdexcept>
#include <iostream>

template<typename T, const unsigned N, const unsigned M>
class Matrix
{
public:
	Matrix() : data(new T[N * M]) {}
	Matrix(const T* array) : Matrix() { std::memcpy(this->data, array, N * M * sizeof(T)); }
	Matrix(Matrix&& m) : data(m.data) { m.data = nullptr; }
	
	Matrix(const Matrix& m);
	
	~Matrix() { delete[] data;}

	Matrix& operator=(const Matrix& m);
	Matrix& operator=(Matrix&& m);
	
	virtual T& at(const uint32_t row, const uint32_t col);
	virtual const T& at(const uint32_t row, const uint32_t col) const;

	uint32_t get_N() const { return N; }
	uint32_t get_M()  const { return M; }

	T* get_data() { return data; }
	const T* get_data() const { return data; }

protected:
	T* data;
};


template<typename T, const unsigned N, const unsigned M>
Matrix<T, N, M>::Matrix(const Matrix<T, N, M>& m)
	: Matrix()
{
	std::memcpy(data, m.data, N * M * sizeof(T));
}

template<typename T, const unsigned N, const unsigned M>
Matrix<T, N, M>& Matrix<T, N, M>::operator=(const Matrix<T, N, M>& m)
{
	if (this == &m)
		return *this;

	std::memcpy(data, m.data, N * M * sizeof(T));

	return *this;
}

template<typename T, const unsigned N, const unsigned M>
Matrix<T, N, M>& Matrix<T, N, M>::operator=(Matrix<T, N, M>&& m)
{
	if (this == &m)
		return *this;

	delete[] data;
	data = m.data;
	m.data = nullptr;
	
	return *this;
}

template<typename T, const unsigned N, const unsigned M>
T& Matrix<T, N, M>::at(const uint32_t row, const uint32_t col)
{
	if (row >= N || col >= M)
		throw std::out_of_range("Out of bounds");

	return data[row * M + col];
}

template<typename T, const unsigned N, const unsigned M>
const T& Matrix<T, N, M>::at(const uint32_t row, const uint32_t col) const
{
	if (row >= N || col >= M)
		throw std::out_of_range("Out of bounds");

	return data[row * M + col];
}

template<typename T, const unsigned N, const unsigned M, const unsigned P>
Matrix<T, N, P> operator*(const Matrix<T, N, M>& lhs, const Matrix<T, M, P>& rhs)
{
	Matrix<T, N, P> res;

	for (size_t i = 0; i < N; ++i) {
		for (size_t j = 0; j < P; ++j) {
			res.at(i, j) = 0;
			
			for (size_t k = 0; k < M; ++k) {
				res.at(i, j) += lhs.at(i, k) * rhs.at(k, j);
			}
			
		}
	}
	
	return res;
}
