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
	cout << "Кол-во элементов массивов A и B >>>";
	cin >> N;
	int* a = new int[N];//объявление динамического массива
	int* b = new int[N];

	for (int i = 0; i < N; i++)//Заполнение случайными числами
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
	cout << "Сумма значений MAX(A[i] + B[i],4*A[i] - B[i]) равна>>" << total << "\n";
	double end = omp_get_wtime();
	cout << omp_get_wtime() - time << endl;
}
//При создании массивов на 10 000 000 элементов 
//Используя 4 секции выходит 0.017, а без 0.019.
//Используя 8 секций выходит 0.016, а без 0.032.
//