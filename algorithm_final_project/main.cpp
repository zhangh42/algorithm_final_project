#include<iostream>
#include<vector>
#include<iomanip>
#include<cassert>
#include<cstdlib>
using namespace std;

// 矩阵
// 只提供矩阵的创建和资源释放
class mat
{
private:
	int n;	// 矩阵大小
	int **m_matrix;
public:
	mat(int _n) :n(_n)
	{
		m_matrix = new int*[n];
		for (int i = 0;i < n;i++)
			m_matrix[i] = new int[n];
	}

	~mat()
	{
		cout << "释放内存" << endl;
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
};

int main()
{
	mat m(4);
	for (int i = 0;i < 4;i++)
		for (int j = 0;j < 4;j++)
			m[i][j] = i + j;

	cout << m << endl;

	system("pause");
	return 0;
}