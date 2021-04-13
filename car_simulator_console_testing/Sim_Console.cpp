// Assignment_03.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

/*
int main()
{
    char A[1000];

    char* p_buffer_out;
    char* p, * pc;
    int* pi;
    double* pd;

    char* p_buffer_in;

    p_buffer_out = A;

    p = p_buffer_out;

    pd = (double*)p;
    *pd = 5.2323;
    p += sizeof(double);

    pd = (double*)p;
    *pd = 49.2323;


    ofstream fout;

    fout.open("binary.bin", ios::binary);
    fout.write(p_buffer_out, 16);

    fout.close();

    p_buffer_in = A;

    ifstream fin;
    fin.open("binary.bin", ios::binary);

    fin.read(p_buffer_in, 16);
    fin.close();


    p = p_buffer_in;

    pd = (double*)p;
    *pd = 5.2323;
    cout << *pd << endl;
    p += sizeof(double);

    pd = (double*)p;
    cout << *pd << endl;
    
}
*/

int main()
{
    //read_string(char string);


    //comm_reception(char string, double output);

    char string_read[100]{ "12.345,67.8910,11.1111,22.2222,33.3333, 44.4444, 52.5235\n" };
    
    const int nb_inputs = 7;

    char mem[nb_inputs][10];
    double mem_d[nb_inputs];

    int p_index = 0;
    bool flag = 0;

    

    for (int i = 0; i < nb_inputs; i++)
    {
        if (flag == 1) break;

        for (int j = 0; j < 10; j++)
        {
            if (string_read[p_index] == '\n')
            {
                flag = 1;
                break;
            }

            if (string_read[p_index] == ',')
            {
                mem[i][j] = '\0';
                p_index++;
                break;
            }
            mem[i][j] = string_read[p_index];

            p_index++;
        }
    }

    for (int i= 0; i < nb_inputs; i++)
    {
        char mem_str[10];
        for (int j = 0; j < 10; j++)
        {
            if (mem[i][j] == '\0')
            {
                mem_str[j] = '\0';
                break;
            }
            mem_str[j] = mem[i][j];
        }
       
        mem_d[i] = atof(mem_str);
    }

    for (int i = 0; i < nb_inputs; i++)
    {
       //cout << mem_d[i] << endl;
    }

    char A[1000];
    char* p, * pc;
    double* pd;
    char* p_buffer_out;
    int n = 0;

    p_buffer_out = A;
    p = p_buffer_out;

    for (int i = 0; i < nb_inputs; i++)
    {
        n += sizeof(double);
        pd = (double*)p;
        *pd = mem_d[i];
        if (i == 6) break;
        p += sizeof(double);
    }
    
    ofstream fout;
    fout.open("binary.bin", ios::binary);
    
    fout.write(p_buffer_out, n);

    fout.close();

    
    ifstream fin;
    fin.open("binary.bin", ios::binary);

    double output[nb_inputs];

    char* p_buffer_in;

    p_buffer_in = A;

    fin.read(p_buffer_in, n);
    fin.close();

    p = p_buffer_in;

    for (int i = 0; i < nb_inputs; i++)
    {
        pd = (double*)p;
        output[i] = *pd;
        if (i == nb_inputs) break;
        p += sizeof(double);
    }
    
    for (int i = 0; i < nb_inputs; i++)
    {
        cout << output[i] << endl;
    }
}