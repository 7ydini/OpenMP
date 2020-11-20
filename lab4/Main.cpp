#include<omp.h>
#include <string>
#include <iostream>
#include <ctime>
using namespace std;
void atom(size_t* a, size_t* b, size_t n);
void noparall(size_t* a, size_t* b, size_t n);
void crit(size_t* a, size_t* b, size_t n);
void main()
{
	setlocale(LC_ALL, "Russian");
	srand(time(NULL));
	int i;	long N;
	cout << "Кол-во элементов массивов A и B >>>";
	cin >> N;

	size_t* a = new size_t[N];//объявление динамического массива
	for (i = 0; i < N; i++)//Заполнение случайными числами
	{
		a[i] = rand() % 1000;
	}

	size_t* b = new size_t[N];//объявление динамического массива
	for (i = 0; i < N; i++)//Заполнение случайными числами
	{
		b[i] = rand() % 1000;
	}


	size_t th_num, num_ths, max_th;
	max_th = omp_get_max_threads();//Максимальное кол-во потоков
	printf_s("Max threads= %zu\n", max_th);
#pragma omp parallel num_threads(max_th) private (num_ths, th_num)//проверяем использование всех потоков
	{
		th_num = omp_get_thread_num();
		num_ths = omp_get_num_threads();
		printf("I am ready %zu from %zu \n", th_num, num_ths);
	}


	//With OpenMP
	double time = omp_get_wtime();
	atom(a, b, N);
	double end = omp_get_wtime();
	cout << "With OMP(Atomic):Время >> " << omp_get_wtime() - time << endl;

	//With OpenMP critical
	double time2 = omp_get_wtime();
	crit(a, b, N);
	double end2 = omp_get_wtime();
	cout << "With OMP(Critical):Время >> " << omp_get_wtime() - time2 << endl;

	//Without OpenMP atomic
	double time3 = omp_get_wtime();
	noparall(a, b, N);
	double end3 = omp_get_wtime();
	cout << "Without OMP:Время >> " << omp_get_wtime() - time3 << endl;

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
	cout << "No Parallel >> Сумма значений MAX(A[i] + B[i],4*A[i] - B[i]) равна>>" << total << "\n \n";
}


void atom(size_t* a, size_t* b, size_t n)
{
	int sum = 0, i = 0; size_t total = 0;
#pragma omp parallel shared(a, b, n, total) private(sum, i) //Элементы массива a и b являются глобальными переменными, а для переменных i, sum - мы создаем локальные копии для работы с ними,
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
	cout << "Atomic Parallel >> Сумма значений MAX(A[i] + B[i],4*A[i] - B[i]) равна>>" << total << "\n \n";
}


void crit(size_t* a, size_t* b, size_t n)
{
	int sum = 0, i = 0; size_t total = 0;
#pragma omp parallel shared(a, b, n, total) private(sum, i) //Элементы массива a и b являются глобальными переменными, а для переменных i, sum - мы создаем локальные копии для работы с ними,
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
	cout << "Critical Parallel >> Сумма значений MAX(A[i] + B[i],4*A[i] - B[i]) равна>>" << total << "\n \n";
}
//При создании массивов на 10 000 000 элементов 
//Использовать критические секции в данном коде не эффективно.
