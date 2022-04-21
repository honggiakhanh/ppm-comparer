#ifndef _NEURON_H_
#define _NEURON_H_

void thinktrain(double input[][3], double result[], double synaptic_weights[]);
void train(double training_set_input[][3], double training_set_output[], int n, double synaptic_weights[]);
double analyze(double test[], double synaptic_weights[]);
void learn();

#endif
