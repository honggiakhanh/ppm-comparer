#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "sigmoid.h"
#include "helpers.h"
#include "neuron.h"

double synaptic_weights_R[3];
double synaptic_weights_G[3];
double synaptic_weights_B[3];

void thinktrain(double input[][3], double result[], double synaptic_weights[]){
    for (int i = 0; i < 4; i++)
    {
        result[i] = sigmoid(dot(input[i], synaptic_weights, 3));
    }
}
void train(double training_set_input[][3], double training_set_output[], int n, double synaptic_weights[]){
    for (int i = 0; i < n; i++)
    {
		double training_set_input_t[3][4];
		double error[4];
		double result[4];

        thinktrain(training_set_input, result, synaptic_weights);

        for (int j = 0; j < 4; j++)
        {
			error[j] = (training_set_output[j] - result[j]) * (sigmoid_derivative(sigmoid(result[j])));
        }

		transpose(training_set_input, training_set_input_t);

		for (int k = 0; k < 3; k++)
		{
			synaptic_weights[k] += dot(training_set_input_t[k], error, 4);
		}
    }
}

double analyze(double test[], double synaptic_weights[]){
	double result = sigmoid(dot(test, synaptic_weights, 3));
	return result;
}

void learn(){    

	srand(1);
	synaptic_weights_R[0] = 2*(double)rand()/RAND_MAX - 1;
	synaptic_weights_R[1] = 2*(double)rand()/RAND_MAX - 1;
	synaptic_weights_R[2] = 2*(double)rand()/RAND_MAX - 1;
	
	synaptic_weights_G[0] = 2*(double)rand()/RAND_MAX - 1;
	synaptic_weights_G[1] = 2*(double)rand()/RAND_MAX - 1;
	synaptic_weights_G[2] = 2*(double)rand()/RAND_MAX - 1;
	
	synaptic_weights_B[0] = 2*(double)rand()/RAND_MAX - 1;
	synaptic_weights_B[1] = 2*(double)rand()/RAND_MAX - 1;
	synaptic_weights_B[2] = 2*(double)rand()/RAND_MAX - 1;
	
	double training_set_inputs[4][3] = { {1,1,0},
										 {0,1,1},
										 {0,0,1},
										 {1,0,0}
									   };
	double training_set_outputs_R[4] = {1,0,0,1};
	double training_set_outputs_G[4] = {1,1,0,0};
	double training_set_outputs_B[4] = {0,1,1,0};

    int n = 10000;

    train(training_set_inputs, training_set_outputs_R, n, synaptic_weights_R);
	train(training_set_inputs, training_set_outputs_G, n, synaptic_weights_G);
	train(training_set_inputs, training_set_outputs_B, n, synaptic_weights_B);
}

















