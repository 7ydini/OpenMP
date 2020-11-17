#include<omp.h>
#include <string>
#include <iostream>
#include <ctime>
using namespace std;
void main()
{
	setlocale(LC_ALL, "Russian");
	srand(time(NULL));
	int i, N;
	cout << "���-�� ��������� �������� A � B >>>";
	cin >> N;
	size_t* a = new size_t[N];//���������� ������������� �������
	for (i = 0; i < N; i++)//���������� ���������� �������
	{
		a[i] = rand() % 1000;
	}
	size_t* b = new size_t[N];//���������� ������������� �������
	for (i = 0; i < N; i++)//���������� ���������� �������
	{
		b[i] = rand() % 1000;
	}

	size_t th_num, num_ths, max_th;
	max_th = omp_get_max_threads();//������������ ���-�� �������
	printf_s("Max threads= %zu\n", max_th);
#pragma omp parallel num_threads(max_th) private (num_ths, th_num)//��������� ������������� ���� �������
	{
		th_num = omp_get_thread_num();
		num_ths = omp_get_num_threads();
		printf("I am ready %zu from %zu \n", th_num, num_ths);
	}

	double time = omp_get_wtime();
	int total = 0, sum = 0;
#pragma omp parallel shared(a, b, N) private(sum, i) reduction(+:total) //�������� ������� a � b �������� ����������� �����������, � ��� ���������� i, sum - �� ������� ��������� ����� ��� ������ � ����,
	{  
#pragma omp for
		for (i = 0; i < N; i++)
		{
			sum = max(a[i] + b[i], 4*a[i] - b[i]);
			if (sum > 1)
			{
				total += sum;
			}
		}
	} /* ���������� ������������� ��������� */
	cout << "����� �������� MAX(A[i] + B[i],4*A[i] - B[i]) �����>>" << total << "\n";
	double end = omp_get_wtime();
	cout << omp_get_wtime() - time << endl;
}
//��� �������� �������� �� 10 000 000 ��������� 
//��������� ����������������� ������� 0.007, � ��� 0.012, ��� � 5-6 ��� ���������.