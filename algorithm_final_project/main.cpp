#include<iostream>
#include<vector>
#include<iomanip>
#include<cassert>
#include<cstdlib>
using namespace std;

// ����
// ֻ�ṩ����Ĵ�������Դ�ͷ�
class mat
{
public:
	int n;	// �����С
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

	// �ƶ����캯��
	mat(mat &&m) noexcept
		:n(m.n), m_matrix(m.m_matrix)
	{
		cout << "right value copy" << endl;
		m.m_matrix = nullptr;
		m.n = 0;
	}

	~mat()
	{
		cout << "�ͷ��ڴ�" << endl;
		for (int i = 0;i < n;i++)
			delete[] m_matrix[i];
		delete[] m_matrix;
	}

	// �����ú���
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

// ���������ʼ������
void init_mat(mat &m)
{
	srand(time(0));
	int n = m.n;
	for (int i = 0;i < n;i++)
		for (int j = 0;j < n;j++)
			m[i][j] = rand() & 15;	// ������ķ�ΧΪ0-15
}

// ������ˣ�����һ������
// һ���㷨
mat mutiply_mat(mat &m1, mat &m2)
{
	assert(m1.n == m2.n);

	int n = m1.n;
	mat result(n);

	for (int i = 0;i < n;i++)
	{
		for (int j = 0;j < n;j++)
		{
			int s = 0;
			for (int k = 0;k < n;k++)
			{
				s = s + m1[i][k] * m2[k][j];
			}
			result[i][j] = s;
		}
	}

	return result;
}

int main()
{
	mat m(4);

	system("pause");
	return 0;
}