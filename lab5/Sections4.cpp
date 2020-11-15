#include<omp.h>
#include <string>
#include <iostream>
#include <ctime>
#include<algorithm> 
using namespace std;
int sections4()
{
	setlocale(LC_ALL, "Russian");
	srand(time(NULL));
	int i;
	int N;
	cout << "���-�� ��������� �������� A � B >>>";
	cin >> N;
	int* a = new int[N];//���������� ������������� �������
	int* b = new int[N];

	for (int i = 0; i < N; i++)//���������� ���������� �������
	{
		a[i] = rand() % 1000;
	}

	for (int i = 0; i < N; i++)
	{
		b[i] = rand() % 1000;
	}

	double time = omp_get_wtime();
	int total = 0, sum = 0;
#pragma omp parallel sections private(sum, i)
	{
#pragma omp section		//1 section	
		{
			for (i = 0; i < N / 4; i++)
			{
				sum = a[i] + b[i];
				if (sum > (4 * a[i] - b[i]) && sum > 1)
				{
					total += sum;
				}
				else
				{
					total += 4 * a[i] - b[i];
				}
			}
		}
#pragma omp section		//2 section	
		{
			for (i = N / 4 + 1; i < N / 2; i++)
			{
				sum = a[i] + b[i];
				if (sum > (4 * a[i] - b[i]) && sum > 1)
				{
					total += sum;
				}
				else
				{
					total += 4 * a[i] - b[i];
				}
			}
		}
#pragma omp section		//3 section	
		{
			for (i = N / 2 + 1; i < 3 * N / 4; i++)
			{
				sum = a[i] + b[i];
				if (sum > (4 * a[i] - b[i]) && sum > 1)
				{
					total += sum;
				}
				else
				{
					total += 4 * a[i] - b[i];
				}
			}
		}
#pragma omp section		//4 section	
		{
			for (i = 3 * N / 4 + 1; i < N; i++)
			{
				sum = a[i] + b[i];
				if (sum > (4 * a[i] - b[i]) && sum > 1)
				{
					total += sum;
				}
				else
				{
					total += 4 * a[i] - b[i];
				}
			}
		}
	}
	cout << "����� �������� MAX(A[i] + B[i],4*A[i] - B[i]) �����>>" << total << "\n";
	double end = omp_get_wtime();
	cout << omp_get_wtime() - time << endl;
}
//��� �������� �������� �� 10 000 000 ��������� 
//��������� 4 ������ ������� 0.017, � ��� 0.019.
//��������� 8 ������ ������� 0.016, � ��� 0.032.
//