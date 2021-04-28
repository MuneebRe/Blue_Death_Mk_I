#pragma once

const int nb_inputs = 10;
// 8 for HIL
// 10 for Sim Step

void get_from_Serial(double output[]);

void reset_bin();