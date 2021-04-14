#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>

#include "robot.h"

#include "Serial.h"

using namespace std;

const int nb_inputs = 6;

void get_from_Serial(double output[])
{
    char A[1000];
    char* p, * pc;
    double* pd;
    char* p_buffer_out;
    int n = nb_inputs * 8;

    ifstream fin;

    if (fin.is_open()) return;
    fin.open("../../../Serial/binary.bin", ios::binary);

    char* p_buffer_in;

    p_buffer_in = A;

    fin.read(p_buffer_in, n);
    fin.close();

    p = p_buffer_in;
    pd = (double*)p;
    if (*pd < -9.5e40) return;

    p = p_buffer_in;

    for (int i = 0; i < nb_inputs; i++)
    {
        pd = (double*)p;
        output[i] = *pd;
        if (i == nb_inputs) break;
        p += sizeof(double);
    }


}
