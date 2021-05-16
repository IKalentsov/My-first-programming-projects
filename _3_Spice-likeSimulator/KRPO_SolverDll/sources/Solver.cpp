#include "Solver.hpp"
#include <iostream>

using namespace std;

void Solver_My::Solve(double **Y, double *x, double *I, int sleSize) {
	double max;
	int k = 0, index = 0;

	while (k < sleSize)
	{
		// Поиск строки с максимальным Y[i][k]
		max = abs(Y[k][k]);
		index = k;
		for (int i = k + 1; i < sleSize; i++)
		{
			if (abs(Y[i][k]) > max)
			{
				max = abs(Y[i][k]);
				index = i;
			}
		}

		for (int j = 0; j < sleSize; j++)
		{
			double temp = Y[k][j];
			Y[k][j] = Y[index][j];
			Y[index][j] = temp;
		}

		double temp = I[k];
		I[k] = I[index];
		I[index] = temp;

		// Нормализация уравнений
		for (int i = k; i < sleSize; i++)
		{
			double temp = Y[i][k];
			for (int j = 0; j < sleSize; j++)
			{
				Y[i][j] = Y[i][j] / temp;
			}
			I[i] = I[i] / temp;

			if (i == k)  continue; // уравнение не вычитать само из себя

			for (int j = 0; j < sleSize; j++)
			{
				Y[i][j] = Y[i][j] - Y[k][j];
			}

			I[i] = I[i] - I[k];
		}
		k++;
	}
	// обратная подстановка
	for (k = sleSize - 1; k >= 0; k--)
	{
		x[k] = I[k];
		for (int i = 0; i < k; i++)
			I[i] = I[i] - Y[i][k] * x[k];
	}
}

PluginType GetType() {
  return PluginType::solver;
}

void GetStringID(std::string &_id) {
	_id = "SolverID";
}

Solver *GetSolver() {
	return new Solver_My;
}

void FreeSolver(Solver *_p_solver) {
  if (_p_solver) {
    delete _p_solver;
    _p_solver = nullptr;
  }
}
