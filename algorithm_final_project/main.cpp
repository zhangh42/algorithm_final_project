#include<iostream>
#include<vector>
#include<iomanip>
#include<cassert>
#include<cstdlib>
#include<ctime>
#include<omp.h>
using namespace std;

// 矩阵
// 只提供矩阵的创建和资源释放
class mat
{
public:
	int n;	// 矩阵大小
	int **m_matrix;

	mat(int _n) :n(_n)
	{
		m_matrix = new int*[n];
		for (int i = 0;i < n;i++)
			m_matrix[i] = new int[n];

		for (int i = 0;i < n;i++)
			for (int j = 0;j < n;j++)
				m_matrix[i][j] = 0;
	}

	mat(mat& m)
	{
		//cout << "mat copy happen..." << endl;
		n = m.n;
		m_matrix = new int*[n];
		for (int i = 0;i < n;i++)
			m_matrix[i] = new int[n];
		for (int i = 0;i < n;i++)
			for (int j = 0;j < n;j++)
				m_matrix[i][j] = m[i][j];
	}

	// 移动构造函数
	mat(mat &&m) noexcept
		:n(m.n), m_matrix(m.m_matrix)
	{
		//cout << "right value copy" << endl;
		m.m_matrix = nullptr;
		m.n = 0;
	}

	~mat()
	{
		//cout << "释放内存 size = " << n << endl;
		for (int i = 0;i < n;i++)
			delete[] m_matrix[i];
		delete[] m_matrix;
	}

	// 测试用函数
	friend ostream& operator<<(ostream& os, const mat& m)
	{
		int n = m.n;
		if (m.n > 20)
		{
			cout << "the matrix is too big..." << endl;
		}
		else
		{
			for (int i = 0;i < n;i++)
			{
				for (int j = 0;j < n;j++)
					cout << setw(4) << left << m.m_matrix[i][j];
				cout << endl;
			}
		}
		return os;
	}

	int* operator[](int index)
	{
		assert(index >= 0 && index < n);
		return m_matrix[index];
	}

	bool operator==(mat &m)
	{
		assert(n == m.n);
		for (int i = 0;i < n;i++)
			for (int j = 0;j < n;j++)
				if (m_matrix[i][j] != m[i][j])
					return false;
		return true;
	}
};

// 矩阵减法
mat sub_mat(mat &m1, mat &m2)
{
	int n = m1.n;
	mat result(n);

	for (int i = 0;i < n;i++)
		for (int j = 0;j < n;j++)
			result[i][j] = m1[i][j] - m2[i][j];

	return result;
}

// 转置矩阵
mat transpose_mat(mat &m)
{
	int n = m.n;
	mat result(n);
	for (int i = 0;i < n;i++)
		for (int j = 0;j < n;j++)
			result[j][i] = m[i][j];
	return result;
}

// 矩阵加法
mat add_mat(mat &m1, mat &m2)
{
	int n = m1.n;
	mat result(n);

	for (int i = 0;i < n;i++)
		for (int j = 0;j < n;j++)
			result[i][j] = m1[i][j] + m2[i][j];

	return result;
}

// 用随机数初始化矩阵
void init_mat(mat &m)
{
	static auto seed = time(nullptr);
	seed++;
	srand(seed);
	int n = m.n;
	for (int i = 0;i < n;i++)
		for (int j = 0;j < n;j++)
			m[i][j] = rand() & 15;	// 随机数的范围为0-15
}

// 矩阵相乘，返回一个矩阵
// 一般算法
mat mutiply_mat(mat &m1, mat &m2)
{
	assert(m1.n == m2.n);
	auto start_time = clock();
	int n = m1.n;
	mat result(n);
	omp_set_num_threads(2);
	int k, i, j;

	for (i = 0;i < n;i++)
	{
		for (j = 0;j < n;j++)
		{
			int s = 0;
			for (k = 0;k < n;k++)
			{
				s += m1[i][k] * m2[k][j];
			}
			result[i][j] = s;
		}
	}

	return result;
}


// 矩阵相乘，返回一个矩阵
// 并行算法
mat mutiply_mat_parallel(mat &m1, mat &m2)
{
	assert(m1.n == m2.n);
	auto start_time = clock();
	int n = m1.n;
	mat result(n);
	omp_set_num_threads(4);
	int k, i, j;
#pragma omp parallel private(i,j,k) shared(result)
	{ 
#pragma omp for
		for (i = 0;i < n;i++)
		{
			for (j = 0;j < n;j++)
			{				
				for (k = 0;k < n;k++)
				{
					result[i][j] += m1[i][k] * m2[k][j];
				}
			}
		}
	}
	return result;
}


// 一般矩阵，cache优化
mat mutiply_mat_cache(mat &m1, mat &m2)
{
	assert(m1.n == m2.n);
	auto start_time = clock();
	int n = m1.n;
	mat result(n);

	mat m3 = transpose_mat(m2);

	for (int i = 0;i < n;i++)
	{
		for (int j = 0;j < n;j++)
		{
			int s = 0;
			for (int k = 0;k < n;k++)
			{
				s += m1[i][k] * m3[j][k];
			}
			result[i][j] = s;
		}
	}
	return result;
}

// strassen方法
mat strassen_mutiply_mat_version1(mat &m1, mat &m2)
{
	assert(m1.n == m2.n);

	int n = m1.n;
	if (n <= 100)
		return mutiply_mat(m1, m2);

	mat result(n);
	// 是否是2的整数倍
	int half_n = n / 2;
	if (n % 2 == 1)
		half_n++;
	int up_left = n / 2;
	int down_right = n / 2;
	if (n % 2 == 1)
		up_left++;

	// 复制m1
	mat A11(half_n), A12(half_n), A21(half_n), A22(half_n);
	for (int i = 0;i < up_left;i++)
		for (int j = 0;j < up_left;j++)
		{
			A11[i][j] = m1[i][j];
		}
	for (int i = 0;i < up_left;i++)
		for (int j = 0;j < down_right;j++)
			A12[i][j] = m1[i][j + up_left];
	for (int i = 0;i < down_right;i++)
		for (int j = 0;j < up_left;j++)
		{
			A21[i][j] = m1[i + up_left][j];
		}
	for (int i = 0;i < down_right;i++)
		for (int j = 0;j < down_right;j++)
			A22[i][j] = m1[i + up_left][j + up_left];

	// 复制m2
	mat B11(half_n), B12(half_n), B21(half_n), B22(half_n);
	for (int i = 0;i < up_left;i++)
		for (int j = 0;j < up_left;j++)
		{
			B11[i][j] = m2[i][j];
		}
	for (int i = 0;i < up_left;i++)
		for (int j = 0;j < down_right;j++)
			B12[i][j] = m2[i][j + up_left];
	for (int i = 0;i < down_right;i++)
		for (int j = 0;j < up_left;j++)
		{
			B21[i][j] = m2[i + up_left][j];
		}
	for (int i = 0;i < down_right;i++)
		for (int j = 0;j < down_right;j++)
			B22[i][j] = m2[i + up_left][j + up_left];

	// 7次乘法
	mat M1 = strassen_mutiply_mat_version1(
		sub_mat(A12, A22), add_mat(B21, B22)
	);
	mat M2 = strassen_mutiply_mat_version1(
		add_mat(A11, A22), add_mat(B11, B22)
	);
	mat M3 = strassen_mutiply_mat_version1(
		sub_mat(A11, A21), add_mat(B11, B12)
	);
	mat M4 = strassen_mutiply_mat_version1(
		add_mat(A11, A12), B22
	);
	mat M5 = strassen_mutiply_mat_version1(
		A11, sub_mat(B12, B22)
	);
	mat M6 = strassen_mutiply_mat_version1(
		A22, sub_mat(B21, B11)
	);
	mat M7 = strassen_mutiply_mat_version1(
		add_mat(A21, A22), B11
	);

	mat C1 = add_mat(
		M1, add_mat(M2, sub_mat(
			M6, M4
		))
	);
	mat C2 = add_mat(M4, M5);
	mat C3 = add_mat(M6, M7);
	mat C4 = add_mat(
		M2, sub_mat(M5, add_mat(
			M3, M7
		))
	);
	for (int i = 0;i < up_left;i++)
		for (int j = 0;j < up_left;j++)
		{
			result[i][j] = C1[i][j];
		}
	for (int i = 0;i < up_left;i++)
		for (int j = 0;j < down_right;j++)
			result[i][j + up_left] = C2[i][j];
	for (int i = 0;i < down_right;i++)
		for (int j = 0;j < up_left;j++)
			result[i+up_left][j] = C3[i][j];
	for (int i = 0;i < down_right;i++)
		for (int j = 0;j < down_right;j++)
			result[i + up_left][j + up_left] = C4[i][j];
	return result;
}

int main()
{
		
	int n = 1000;
	mat m1(n), m2(n);
	init_mat(m1);
	init_mat(m2);
	auto start_time = clock();
	mat m3 = mutiply_mat(m1, m2);
	auto mid = clock();
	mat m4 = mutiply_mat_parallel(m1, m2);
	//mat m4 = strassen_mutiply_mat_version1(m1, m2);
	auto end = clock();

	cout << m3 << endl;
	cout << m4 << endl;
	cout << "time: " << mid - start_time << endl;
	cout << "time: " << end - mid << endl;
	
	assert(m3 == m4);
	system("pause");
	return 0;
}