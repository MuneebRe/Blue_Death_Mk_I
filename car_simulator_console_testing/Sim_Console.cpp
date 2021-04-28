// Assignment_03.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;




void get_from_Serial(double output[]);
const int nb_inputs = 10;

int main()
{

    while (1)
    {
        
        double output[nb_inputs];
        get_from_Serial(output);

        for (int i = 0; i < nb_inputs; i++)
        {
            cout << output[i] << "\t\t";
            if (i == nb_inputs - 1) cout << endl;
        }
    }
    
}

void get_from_Serial(double output[])
{
    char A[1000];
    char* p, * pc;
    double* pd;
    char* p_buffer_out;
    int n = nb_inputs * 8;

    ifstream fin;
    
    if (fin.is_open()) return;
    fin.open("../Serial/binary.bin", ios::binary);

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
