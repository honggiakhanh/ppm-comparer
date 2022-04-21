#include "helpers.h"

double dot(double *v, double *u, uint32_t n){
	
	double result = 0.0;
	for(uint32_t i = 0; i < n; i++){
		result += v[i] * u[i];
	}

	return result;
}

void transpose(double A[][3], double B[][4])
{
    for (int i = 0; i < 4; i++)
	{
        for (int j = 0; j < 3; j++)
		{ 
            B[j][i] = A[i][j];
		}
	}
} 
