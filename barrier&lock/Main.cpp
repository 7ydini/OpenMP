#include<omp.h>
#include <string>
#include <iostream>
#include <ctime>
using namespace std;
void lock_parall(size_t* a, size_t* b, size_t n);
void noparall(size_t* a, size_t* b, size_t n);
void barrier_parall(size_t* a, size_t* b, size_t n);
void main()
{
	setlocale(LC_ALL, "Russian");
	srand(time(NULL));
	int i;	size_t N;
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
	lock_parall(a, b, N);
	double end = omp_get_wtime();
	cout << "With OMP(lock):Время >> " << omp_get_wtime() - time << endl;

	double time3 = omp_get_wtime();
	barrier_parall(a, b, N);
	double end3 = omp_get_wtime();
	cout << "With OMP(barier):Время >> " << omp_get_wtime() - time << endl;


	//Without OpenMP
	double time2 = omp_get_wtime();
	noparall(a, b, N);
	double end2 = omp_get_wtime();
	cout << "Without OMP:Время >> " << omp_get_wtime() - time2 << endl;
}

void noparall(size_t* a, size_t* b, size_t n)
{
	int sum = 0, i = 0;	size_t total = 0;
	{
		for (i = 0; i < n; i++)
		{
			sum = max(a[i] + b[i], 4 * a[i] - b[i]);// Находим какое из 2х выражений мы приравняем к sum
			if (sum > 1)
			{
				total += sum;//Прибавляем к total наибольшее из 2х выражений.
			}
		}
	} /* Завершение параллельного фрагмента */
	cout << "\n No Parallel >> Сумма значений MAX(A[i] + B[i],4*A[i] - B[i]) равна>>" << total << "\n";
}
void lock_parall(size_t* a, size_t* b, size_t n)
{
	omp_lock_t lock;
	omp_init_lock(&lock);
	int sum = 0, i = 0; size_t total = 0;
#pragma omp parallel shared(a, b, n) private(sum, i) //Элементы массива a и b являются глобальными переменными, а для переменных i, sum - мы создаем локальные копии для работы с ними,
	{
#pragma omp for
		for (i = 0; i < n; i++)
		{
			sum = max(a[i] + b[i], 4 * a[i] - b[i]);// Находим какое из 2х выражений мы приравняем к sum
			omp_set_lock(&lock);
			if (sum > 1)
			{				
				total += sum;//Прибавляем к total наибольшее из 2х выражений.
			}
			omp_unset_lock(&lock);
		}	
	} /* Завершение параллельного фрагмента */
	omp_destroy_lock(&lock);
	cout << "\n Lock Parallel >> Сумма значений MAX(A[i] + B[i],4*A[i] - B[i]) равна>>" << total << "\n";
}


void barrier_parall(size_t* a, size_t* b, size_t n)
{
	int sum = 0, i = 0; size_t total = 0;
#pragma omp parallel shared(a, b, n) private(sum, i) //Элементы массива a и b являются глобальными переменными, а для переменных i, sum - мы создаем локальные копии для работы с ними,
	{
#pragma omp for
		for (i = 0; i < n; i++)
		{
			sum = max(a[i] + b[i], 4 * a[i] - b[i]);// Находим какое из 2х выражений мы приравняем к sum

			if (sum > 1)
			{
				total += sum;//Прибавляем к total наибольшее из 2х выражений.
			}
#pragma omp barrier
		}
	} /* Завершение параллельного фрагмента */
	cout << "\n Barrier Parallel >> Сумма значений MAX(A[i] + B[i],4*A[i] - B[i]) равна>>" << total << "\n";
}
//При создании массивов на 10 000 000 элементов 
/*
With OMP(lock):Время >> 0.267502
With OMP(barier):Время >> 0.286165


1 000 000 Элементов:

With OMP(lock):Время >> 0.0292186
With OMP(barier):Время >> 0.0353553



100 000 Элементов:

With OMP(lock):Время >> 0.0069531
With OMP(barier):Время >> 0.0128843



10 000 Элементов:

With OMP(lock):Время >> 0.0031049
With OMP(barier):Время >> 0.0066064



1 000 Элементов:

With OMP(lock):Время >> 0.0029748
With OMP(barier):Время >> 0.0077086


*/
