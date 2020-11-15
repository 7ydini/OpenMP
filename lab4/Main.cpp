#include<omp.h>
#include <string>
#include <iostream>
#include <ctime>
using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");
	srand(time(NULL));
	int i;
	int N;
	cout << "���-�� ��������� �������� A � B >>>";
	cin >> N;
	int* a = new int[N];
	for (int i = 0; i < N; i++)
	{
		a[i] = rand() % 1000;
	}
	int* b = new int[N];
	for (int i = 0; i < N; i++)
	{
		b[i] = rand() % 1000;
	}
	double time = omp_get_wtime();
	int total = 0, sum = 0, s;
#pragma omp parallel shared(a, b, total) private(i,sum)//�������� ������� a � b �������� ����������� �����������, � ��� ���������� i, sum - �� ������� ��������� ����� ��� ������ � ����
	{
		#pragma omp for
		for (i = 0; i < N; i++)
		{
			sum = a[i] + b[i];
			if (sum > (4 * a[i] - b[i]) && sum > 1)
			{
			#pragma omp atomic
				total += sum;
			}
		}
		#pragma omp for
		for (i = 0; i < N; i++)
		{
			sum = 4 * a[i] - b[i];
			if (sum > (a[i] - b[i]) && sum > 1)
			{
			#pragma omp atomic
				total += sum;
			}
		}
	}/* ���������� ������������� ��������� */
	cout << "����� �������� MAX(A[i] + B[i],4*A[i] - B[i]) �����>>" << total << "\n";
	double end = omp_get_wtime();
	cout << omp_get_wtime() - time << endl;
}
//��� �������� �������� �� 10 000 000 ��������� 
//��������� critical ������� 1.7, � ��� 0.037, ��� � 4-5 ���� �������, ��������� atomic ������� 0.021