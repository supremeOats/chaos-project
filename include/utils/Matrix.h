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
	Matrix(const char* file);
	
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

	void to_file(const char* file) const;

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
Matrix<T, N, M>::Matrix(const char* file)
{
	std::ifstream f(file, std::ios::binary);

	if (!f.is_open()) 
		throw std::runtime_error("Failed to open file for reading");

	f.read(reinterpret_cast<char*>(&N), sizeof(uint32_t));
	f.read(reinterpret_cast<char*>(&M), sizeof(uint32_t));

	data = new (std::nothrow) T[N * M];

	if (!data) {
		f.close();
		throw std::bad_alloc();
	}

	f.read(reinterpret_cast<char*>(data), sizeof(T) * N * M);
	f.close();
}

template<typename T, const unsigned N, const unsigned M>
Matrix<T, N, M>& Matrix<T, N, M>::operator=(const Matrix<T, N, M>& m)
{
	if (this == &m)
		return *this;

	// if (N != m.N || M != m.M)
	// 	throw std::invalid_argument("Matrices must have the same dimensions");

	std::memcpy(data, m.data, N * M * sizeof(T));
}

template<typename T, const unsigned N, const unsigned M>
Matrix<T, N, M>& Matrix<T, N, M>::operator=(Matrix<T, N, M>&& m)
{
	if (this == &m)
		return *this;

	// if (N != m.N || M != m.M)
	// 	throw std::invalid_argument("Matrices must have the same dimensions");

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

template<typename T, const unsigned N, const unsigned M>
void Matrix<T, N, M>::to_file(const char* file) const
{
	std::ofstream f(file, std::ios::binary);

	if (!f.is_open())
		throw std::runtime_error("Failed to open file for writing");

	f.write(reinterpret_cast<const char*>(&N), sizeof(N));
	f.write(reinterpret_cast<const char*>(&M), sizeof(M));
	f.write(reinterpret_cast<const char*>(data), sizeof(T) * N * M);
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