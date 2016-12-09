#include<iostream>
#include<vector>
#include<iomanip>
#include<cassert>
#include<cstdlib>
#include<ctime>
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
	}

	mat(mat& m)
	{
		cout << "mat copy happen..." << endl;
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
		cout << "right value copy" << endl;
		m.m_matrix = nullptr;
		m.n = 0;
	}

	~mat()
	{
		cout << "释放内存 size = " << n << endl;
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
mat mutiply_mat(mat &m1,mat &m2)
{
	assert(m1.n == m2.n);
	auto start_time = clock();
	int n = m1.n;
	mat result(n);

	for (int i = 0;i < n;i++)
	{
		for (int j = 0;j < n;j++)
		{
			int s = 0;
			for (int k = 0;k < n;k++)
			{
				s += m1[i][k] * m2[k][j];
			}
			result[i][j] = s;
		}
	}
	cout << "exm?" << endl;
	cout << "time: " << clock() - start_time << endl;
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

// strassen方法
mat strassen_mutiply_mat_version1(const mat &m1, const mat &m2)
{
	mat result(0);
	return result;
}

int main()
{
	//auto start_time = clock();
	//mat m1(1000);
	//mat m2(1000);
	//init_mat(m1);
	//init_mat(m2);
	//cout << "time: " << clock() - start_time << endl;
	//cout << m1 << endl << m2 << endl;
	//cout << mutiply_mat(m1, m2) << endl;
	//cout << "time: " << clock() - start_time << endl;

	mat m1(5), m2(5);
	init_mat(m1);
	init_mat(m2);

	cout << m1 << endl;
	mat m3 = transpose_mat(m1);
	cout << m3 << endl;
	mat m4 = transpose_mat(m3);
	cout << (m4 == m1) << endl;


	system("pause");
	return 0;
}