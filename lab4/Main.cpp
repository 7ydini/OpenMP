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
	cout << " ол-во элементов массивов A и B >>>";
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
#pragma omp parallel shared(a, b, total) private(i,sum)//Ёлементы массива a и b €вл€ютс€ глобальными переменными, а дл€ переменных i, sum - мы создаем локальные копии дл€ работы с ними
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
	}/* «авершение параллельного фрагмента */
	cout << "—умма значений MAX(A[i] + B[i],4*A[i] - B[i]) равна>>" << total << "\n";
	double end = omp_get_wtime();
	cout << omp_get_wtime() - time << endl;
}
//ѕри создании массивов на 10 000 000 элементов 
//»спользу€ critical выходит 1.7, а без 0.037, что в 4-5 раза быстрее, использу€ atomic выходит 0.021