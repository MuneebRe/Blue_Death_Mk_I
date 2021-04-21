#pragma once

//const int nb_inputs = 10;
//int* nb_inputs;
// 8 for HIL
// 10 for Sim Step

void get_from_Serial(double output[], int nb_inputs);

void reset_bin(int nb_inputs);