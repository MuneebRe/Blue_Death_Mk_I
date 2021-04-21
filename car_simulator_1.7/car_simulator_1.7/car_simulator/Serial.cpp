#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>

#include "robot.h"

#include "Serial.h"

using namespace std;

void get_from_Serial(double output[], int nb_inputs)
{
    char A[150];
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

void reset_bin(int nb_inputs)
{
    static bool init = 0;
    int n = 0;

    if (!init)
    {
        init = 1;
        char A[100];
        char* p;
        char* p_buffer_out;
        double* pd;

        p_buffer_out = A;
        p = p_buffer_out;

        for (int i = 0; i < nb_inputs; i++)
        {
            n += sizeof(double);
            pd = (double*)p;
            *pd = 0.00;
            if (i == nb_inputs) break;
            p += sizeof(double);
        }

        ofstream fout;

        fout.open("../../../Serial/binary.bin", ios::binary);
        fout.write(p_buffer_out, n);
        fout.close();
    }
}