#include<omp.h>
#include <string>
#include <iostream>
#include <ctime>
using namespace std;
void parall(size_t* a, size_t* b, size_t n);
void noparall(size_t* a, size_t* b, size_t n);
//void crit(size_t* a, size_t* b, size_t n);
size_t noparallx(size_t* a, size_t* b, size_t n, size_t k, size_t j);
void main()
{
	setlocale(LC_ALL, "Russian");
	srand(time(NULL));
	int i;	long N;
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


	//With OpenMP
	double time = omp_get_wtime();
	parall(a, b, N);
	double end = omp_get_wtime();
	cout << "With OMP(Atomic):����� >> " << omp_get_wtime() - time << endl;

	/*With OpenMP critical
	double time2 = omp_get_wtime();
	crit(a, b, N);
	double end2 = omp_get_wtime();
	cout << "With OMP(Critical):����� >> " << omp_get_wtime() - time2 << endl;*/

	//Without OpenMP atomic
	double time3 = omp_get_wtime();
	noparall(a, b, N);
	double end3 = omp_get_wtime();
	cout << "Without OMP:����� >> " << omp_get_wtime() - time3 << endl;


	double time4 = omp_get_wtime();
size_t total = 0;
#pragma omp parallel sections
	{
#pragma omp section//1 section
		{
			total += noparallx(a, b, N, 1, 8);//j - �� ������� ������ ������� ������ k-���� ����� �������
		}
#pragma omp section//2 section
		{
			total += noparallx(a, b, N, 2, 8);
		}
#pragma omp section//3 section
		{
			total += noparallx(a, b, N, 3, 8);
		}
#pragma omp section//4 section
		{
			total += noparallx(a, b, N, 4, 8);
		}
#pragma omp section//5 section
		{
			total += noparallx(a, b, N, 5, 8);
		}
#pragma omp section//6 section
		{
			total += noparallx(a, b, N, 6, 8);
		}
#pragma omp section//7 section
		{
			total += noparallx(a, b, N, 7, 8);
		}
#pragma omp section//8 section
		{
			total += noparallx(a, b, N, 8, 8);
		}
	}
	cout << "8 sections >> ����� �������� MAX(A[i] + B[i],4*A[i] - B[i]) �����>> " << total << endl;
	double end4 = omp_get_wtime();
	cout << "With OMP:����� >> " << end4 - time4 << endl;



	double time5 = omp_get_wtime();
	total = 0;
#pragma omp parallel sections
	{
#pragma omp section//1 section
		{
			total += noparallx(a, b, N, 1, 4);//j - �� ������� ������ ������� ������ k-���� ����� ������� n1 - 1 ���� ������, 0 � ��� �������
		}
#pragma omp section//2 section
		{
			total += noparallx(a, b, N, 2, 4);
		}
#pragma omp section//3 section
		{
			total += noparallx(a, b, N, 3, 4);
		}
#pragma omp section//4 section
		{
			total += noparallx(a, b, N, 4, 4);
		}
	}
	cout << "4 sections >> ����� �������� MAX(A[i] + B[i],4*A[i] - B[i]) �����>> " << total << endl;
	double end5 = omp_get_wtime();
	cout << "With OMP:����� >> " << end5 - time5 << endl;


}

void noparall(size_t* a, size_t* b, size_t n)
{
	int sum = 0, i = 0;	size_t total = 0;
	{
		for (i = 0; i < n; i++)
		{
			sum = max(a[i] + b[i], 4 * a[i] - b[i]);
			if (sum > 1)
			{
				total += sum;
			}
		}
	}
	cout << "No Parallel >> ����� �������� MAX(A[i] + B[i],4*A[i] - B[i]) �����>>" << total << "\n";
}


void parall(size_t* a, size_t* b, size_t n)
{
	int sum = 0, i = 0; size_t total = 0;
#pragma omp parallel shared(a, b, n) private(sum, i) reduction(+:total) //�������� ������� a � b �������� ����������� �����������, � ��� ���������� i, sum - �� ������� ��������� ����� ��� ������ � ����,
	{
#pragma omp for
		for (i = 0; i < n; i++)
		{
			sum = max(a[i] + b[i], 4 * a[i] - b[i]);
			if (sum > 1)
			{
#pragma omp atomic
				total += sum;
			}
		}
	}
	cout << "Atomic Parallel >> ����� �������� MAX(A[i] + B[i],4*A[i] - B[i]) �����>>" << total << "\n";
}



size_t noparallx(size_t* a, size_t* b, size_t n, size_t k, size_t j)//j - �� ������� ������ ������� ������, k-���� �����
{
	int sum = 0, i = 0; size_t total = 0;
	for (i = (k - 1) * n / j; i < k * n / j; i++)//������� ������ �� j ������ ������, � ������� ������ k-� ����� � ���������� ��
	{
		sum = max(a[i] + b[i], 4 * a[i] - b[i]);
		if (sum > 1)
		{
			total += sum;
		}
	}
	return total;//��������� �����, ��� ���������� ����������
}

/*
void crit(size_t* a, size_t* b, size_t n)
{
	int sum = 0, i = 0; size_t total = 0;
#pragma omp parallel shared(a, b, n) private(sum, i) reduction(+:total) //�������� ������� a � b �������� ����������� �����������, � ��� ���������� i, sum - �� ������� ��������� ����� ��� ������ � ����,
	{
#pragma omp for
		for (i = 0; i < n; i++)
		{
			sum = max(a[i] + b[i], 4 * a[i] - b[i]);
			if (sum > 1)
			{
#pragma omp critical
				{
					total += sum;
				}
			}
		}
	}
	cout << "Critical Parallel >> ����� �������� MAX(A[i] + B[i],4*A[i] - B[i]) �����>>" << total << "\n";
}*/
//��� �������� �������� �� 10 000 000 ��������� 