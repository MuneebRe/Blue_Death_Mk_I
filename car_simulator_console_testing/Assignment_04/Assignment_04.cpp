// Assignment_03.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

const int NMAX = 2500;

class dMatrix
{

private:
    //double A[NMAX+1];
    double* A;
    static int n_objects;


    char* p_buffer_out;
    char* p, * pc;
    int* pi;
    double* pd;


    char* p_buffer_in;

public:
    int N;                                  //Limit Row
    int M;                                  //Limit Column
    int K;                                  //Limit of A[]

    dMatrix(int n, int m);
    dMatrix(const char file_name[]);
       
    void count();
    void transfer(double temp[100][100]);   //Transfer temp[M][N] to A[K]
    void print();                           //Print A[K] matrix
    void reset();                           //Reset array to 0;
    double max();
    double &e(int i, int j);
    void save(const char file_name[]);
    void load(const char file_name[]);
    int get_K() { return K; }
    int get_M() { return M; }
    int get_N() { return N; }

     ~dMatrix();
};

double add(dMatrix A, dMatrix B, dMatrix C)
{
    int value = 0;

    for (int i = 1; i < A.N; i++)
    {
        for (int j = 1; j < A.M; j++)
        {
            value = A.e(i, j) + B.e(i, j);
            C.e(i, j) = value;
        }
    }
    return 0;
}

int dMatrix::n_objects = 0;

char B[1000];
char R[1000];
int n_buffer = 0;

int main()
{
    
    //Checklist:
    //Done--Converting temp[][] to A[] in class
    //Done--Constructor for (n,m) and file_name
    //Done--Destructor
    //Done--max() function
    //Done--e(i,j) function
    //Done--add(A,B,C)
    //Done--Complete illustration, no fault.
    dMatrix t1("Array.txt"), t2(3, 3), t3(3, 2);
    cout << "\n\nMaximum of chosen matrix using t1.max() is: " << t1.max() << "\n\n";
    cout << "\nReference return test for t1.e(1,1) is: " << t1.e(1,1 ) << "\n";
    cout << "\nReference return test for t1.e(1,2) is: " << t1.e(1,2 ) << "\n";
    cout << "\nReference return test for t1.e(1,3) is: " << t1.e(1,3 ) << "\n";
    cout << "\nReference return test for t1.e(2,1) is: " << t1.e(2,1 ) << "\n";
    cout << "\nReference return test for t1.e(2,2) is: " << t1.e(2,2 ) << "\n";
    cout << "\nReference return test for t1.e(2,3) is: " << t1.e(2,3 ) << "\n";
    cout << "\nReference return test for t1.e(3,1) is: " << t1.e(3,1 ) << "\n";
    cout << "\nReference return test for t1.e(3,2) is: " << t1.e(3,2 ) << "\n";
    cout << "\nReference return test for t1.e(3,3) is: " << t1.e(3,3 ) << "\n";

    cout << "\nInitializing another array C" << endl;
    dMatrix t4("Array.txt");
    
    cout << "\nResetting array C" << endl;
    t4.reset();

    cout << "\nPrinting Array C" << endl;
    t4.print();

    /////////////////////////// --- PART A ---/////////////////////////////

    cout << "\n\n\n" << "PART A ------------------------------------------------------------" << endl << endl;

    dMatrix Dynamic_Array(6, 6);

    Dynamic_Array.print();

    cout << "\n\n\n" << "Dynamic Array Integration Successful" << endl << endl;

    /////////////////////////// --- PART B ---/////////////////////////////

    cout << "\n\n\n" << "PART B ------------------------------------------------------------" << endl << endl;

    cout << "\nUsing add(t1,t2,t4)" << endl;
    add(t1, t2, t4); //t4 is called by value this time

    cout << "\nPrinting new t4" << endl;
    t4.print();

    cout << "\n\n";

    t4.e(1, 1) = 100;

    t4.print(); //Other than A[1,1] for t4, all the other locations are printing garbage values.
                //If I'm understanding it correctly, the array in t4 has indeed been called by value
                //but add(t1,t2,t4) changes the reference location of t4.A(i,j) variable by t1.A(i,j) + t2.A(i,j)


    cout << "\n\n\n" << "Garbage Values if t4 called by value!" << endl << endl;

    cout << '\n\n\n';

    /////////////////////////// --- PART C ---/////////////////////////////

    cout << "\n\n\n" << "PART C ------------------------------------------------------------" << endl << endl;

    dMatrix t5("Array.txt");

    cout << "\n\n\n" << "Object Count Success" << endl << endl;

    /////////////////////////// --- PART D ---/////////////////////////////

    cout << "\n\n\n" << "PART D ------------------------------------------------------------" << endl << endl;

    t5.save("file.bin");

    /////////////////////////// --- PART E ---/////////////////////////////

    cout << "\n\n\n" << "PART E ------------------------------------------------------------" << endl << endl;

    t5.load("file.bin");
    
    cout << endl << endl;

    /////////////////////////// --- PART F ---/////////////////////////////

    cout << "\n\n\n" << "PART F ------------------------------------------------------------" << endl << endl;
    dMatrix A(2, 3);

    for (int i = 1; i <= A.get_M(); i++)
    {
        for (int j = 1; j <= A.get_N(); j++)
        {
            A.e(i, j) = (double)i - (double)j + 3.0;
        }
    }

    A.print();

    cout << endl;

    A.save("file.bin");

    dMatrix B(2, 2);

    B.load("file.bin");

    B.print();

    system("PAUSE");  // If not used, will cause low-level RPC error
    
    cout << "\n\n\n" << "Matrix A Saved in Buffer & Loaded in Matrix B Successfully" << endl << endl;

    return 0;
}


dMatrix::dMatrix(int n, int m)
{
    cout << "\nConstructor dMatrix(n,m) \n" << endl;
    
    N = n;
    M = m;

    N = N + 1;
    M = M + 1;

    A = new double[N*M];

    double temp[100][100] = { 0 };

    if (M == N)
    {
        for (int i = 1; i < N; i++)
        {
            for (int j = 1; j < M; j++)
            {
                if (i == j)
                {
                    temp[i][j] = 1;
                }
            }
        }
    }
    else
    {
        temp[N][M] = { 0 };
    }

    transfer(temp);
    print();

    count();
}

dMatrix::dMatrix(const char file_name[])
{

    cout << "\nConstructor dMatrix(const char file_name[]) \n" << endl;
 
    ifstream fin;

    fin.open(file_name);

    if (!fin) {
        cout << "\nfile open error in triangle(file_name)";
        return;
    }

    fin >> N;
    fin >> M;

    N = N + 1;
    M = M + 1;

    A = new double[N * M];

    double temp[100][100] = { 0 };

    for (int i = 1; i < N; i++)
    {
        for (int j = 1; j < M; j++)
        {
            fin >> temp[i][j];
        }
    }
    
    fin.close();
    
    transfer(temp);
    
    print();

    count();
}

void dMatrix::count()
{
    n_objects++;
    cout << "\nNumber of objects = " << n_objects << endl << endl;
}

void dMatrix::transfer(double temp[100][100])
{
    int k = ((N-1) * (M-1)) + 1;
    K = k;
    int count = 1;

    for (int i= 1; i < M; i++)
    {
        for (int j = 1; j < N; j++)
        {
            A[count] = temp[i][j];
            count++;
        }
    }
}

double dMatrix::max()
{
    double max = 0;

    for (int k = 0; k < K; k++)
    {
        if (A[k] > max)
        {
            max = A[k];
        }
    }

    return max;
}

double &dMatrix::e(int i, int j)
{
    int counter = 0;
    int trigger = 0;



    for (int i_count = 1; i_count < N; i_count++)
    {
        if (i > M || j > N)
        {
            cout << "\nSelection out of Range Limit" << endl;
            break;
        }

        for (int j_count = 1; j_count < M; j_count++)
        {
            counter++;
            
            if (i_count == i && j_count == j)
            {
                trigger = 1;
                break;
            }
        }
        if (trigger == 1) break;
    }
    
    return A[counter];

}

void dMatrix::reset()
{
    int count = 0;
    for (int i = 1; i < N; i++)
    {
        for (int j = 1; j < M; j++)
        {
            count++;
            A[count] = 0;
        }
    }
}

void dMatrix::print()
{
    for (int k = 1; k < K; k++)
    {
        cout << A[k] << '\t';
    }

    cout << endl;
}

void dMatrix::save(const char file_name[])
{
    
    p_buffer_out = B;
    p = p_buffer_out;

    pi = (int*)p;
    *pi = N - 1;
    cout << *pi << endl;
    p += sizeof(int);

    pi = (int*)p;
    *pi = M - 1;
    cout << *pi << endl;
    p += sizeof(int);

    K = ((N - 1) * (M - 1)) + 1;


    for (int i = 1; i < K; i++)
    {
        pd = (double*)p;
        *pd = A[i];
        cout << *pd << endl;
        p += sizeof(double);
    }

    n_buffer = (int)(p - p_buffer_out);
    cout << "\n\nnumber of bytes in buffer = " << n_buffer << endl;

    if (n_buffer > 1000) {
        cout << "\nerror buffer is too small -- wild pointer";
    }

    ofstream fout;
    fout.open(file_name, ios::binary);

    if (!fout) {
        cout << "\nfile opening error";
    }

    fout.write(p_buffer_out, n_buffer);

    fout.close();


}

void dMatrix::load(const char file_name[])
{
    p_buffer_in = R;

    ifstream fin;

    fin.open(file_name, ios::binary);

    if (!fin) {
        cout << "\nfile opening error";
    }

    fin.read(p_buffer_in, n_buffer);

    fin.close();

    p = p_buffer_in;

    pi = (int*)p;
    cout << "\n*pi = " << *pi << endl;
    N = *pi + 1;
    p += sizeof(int);

    pi = (int*)p;
    cout << "\n*pi = " << *pi << endl;
    M = *pi + 1;
    p += sizeof(int);

    K = ((N - 1) * (M - 1)) + 1;
    
    for (int i = 1; i < K; i++)
    {
        pd = (double*)p;
        cout << "\n*pd = " << *pd << "\t";
        A[i] = *pd;
        p += sizeof(double);
    }
    cout << endl;
    
}

dMatrix::~dMatrix()
{
    delete[] A;
    A = NULL;

    cout << "\nDestructor for dMatrix\n" << endl;
}