
#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <iostream>
#include <fstream>
#include <iomanip>

#include <conio.h>
#include <windows.h>

#include "serial_com.h"

#include "timer.h"

using namespace std;

// check if key c is currently pressed
// note that multiple keys can be pressed at once
#define KEY(c) ( GetAsyncKeyState((int)(c)) & (SHORT)0x8000 )

void Serial_to_Sim();
void get_input_from_bin(float& input1, float& input2, float& input3);
void push_input_to_controller(float input1, float input2, float input3);

HANDLE h1;

int main()
{  
	
	const int NMAX = 64;
	char buffer_in[NMAX];
	char buffer_inputs[NMAX];
	ofstream fout;
	ifstream fin;
	char COM_number[10];
	int speed = 1;
	
	// read serial port settings fron a file
	fin.open("save_serial_settings.txt");
	
	if(!fin) {
		cout << "\nerror opening 'save_serial_settings.txt' file\n\n";
		system("pause");
		return 1;
	}
	
	fin >> COM_number;
	fin >> speed;
	
	fin.close();
	
	cout << "\nCOM number = " << COM_number;
	cout << "\nspeed = " << speed;
	cout << "\n\navailable speed settings are:";
	cout << "\n0 - 9600 bps (baud) -- slow but very stable";
	cout << "\n1 - 115200 bps -- fast and stable";
	cout << "\n2 - 250000 bps -- some Arduinos can only go this fast";
	cout << "\n3 - 1000000 (1 Mbps)";
	cout << "\n4 - 2000000 (2 Mbps) -- max speed for some Arduino boards";
	cout << "\n5 - 5000000 (5 Mbps)";
	cout << "\n6 - 12000000 (12 Mbps) -- max Teensy 3.2 speed";
	cout << "\n7 - 480000000 (480 Mbps) -- max Teensy 3.5 / 3.6 speed\n";
	cout << "\nmake sure the Arduino IDE Serial speed setting is the same";
	cout << "\nand don't run the Arduino Serial Monitor when using this program\n"; 

	// open output file
	//if (fout.is_open()) return;
	fout.open("output.txt");
	
	if(!fout) {
		cout << "\nerror opening output file 'output.txt'\n\n";
		system("pause");
		return 1;
	}
	
	Sleep(100);

	// note: the serial port driver should be set to the same data (baud) rate
	// in the device manager com port settings.
	// the uploader should also use the same rate.
	open_serial(COM_number,h1,speed);

	cout << "\npress c key to continue, x to quit\n";
	while( !KEY('C') ) Sleep(1);

	// send start message (ie, the 's' character) to Arduino
	buffer_in[0] = 's';
	serial_send(buffer_in,1,h1);

	// *** NOTE: don't put any delays after this point
	// or the serial buffer on the PC side will overflow
	// with data from the Arduino and will also cause
	// control performance issues on the Arduino
	
	cout << "\nreceiving data from Arduino ...";

	// wait for send message to be received, etc. before listening for data
	Sleep(100);

	cout << fixed;
	cout << setprecision(4);

	float input1, input2, input3;

	while(1) {
		/*
		// receive data from Arduino
		serial_recv(buffer_in,1,h1);
		
		// check for termination character from Arduino
		if( buffer_in[0] == '#' ) break;

		// check for termination character from keyboard
		if( KEY('X') ) break;
		
		// send serial data to output file
		//cout << buffer_in[0];
		fout << buffer_in[0];
		*/

		

		get_input_from_bin(input1, input2, input3);


		// check for termination character from Arduino
		serial_recv(buffer_in, 1, h1);
		if (buffer_in[0] == '#') break;

		push_input_to_controller(input1, input2, input3);
		
		Serial_to_Sim();
		
		
		//////////////
		//buffer_in[1] = *pi;
		//buffer_in[2] = 0;

		//buffer_in[1] = buffer_in[1] * (1.00 / 12.00) * 125;
		//buffer_in[2] = buffer_in[2] * (1.00 / 12.00) * 125;

		// check for termination character from keyboard
		if (KEY('X')) break;
		
	}



	close_serial(h1);

	fout.close();
	
	// wait for completion, etc.
	Sleep(1000);

	cout << "\n\noutput file is saved.\n\ndone.\n";

	// this just waits for any key to be pressed
	system("pause");

	return 0;
}




void Serial_to_Sim()
{
	const int nb_inputs = 8;	//If using sim_step, use 10. If using HIL, use 8.

	const int str_limit = 15;
	const int str_MAX = 120;

	char mem[nb_inputs][str_limit];
	double mem_d[nb_inputs];
	char buffer_serial[str_MAX];

	int p_index = 0;
	bool flag1 = 0;


	//char string_read[100]{ "12.345,67.8910,11.1111,22.2222,33.3333, 44.4444\n" };
	
	
	char string_read[str_MAX];
	for (int i = 0; i < str_MAX; i++)
	{
		serial_recv(buffer_serial, 1, h1);
		string_read[i] = buffer_serial[0];
		if (buffer_serial[0] == '\n') break;
		
	}
	

	for (int i = 0; i < nb_inputs; i++)
	{
		if (flag1 == 1) break;

		for (int j = 0; j < str_limit; j++)
		{
			if (string_read[p_index] == '\n')
			{
				flag1 = 1;
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

	for (int i = 0; i < nb_inputs; i++)
	{
		char mem_str[str_limit];
		for (int j = 0; j < str_limit; j++)
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
		if (i == nb_inputs) break;
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

	/*
	cout << endl;
	for (int i = 0; i < nb_inputs; i++)
	{
		cout << output[i] << "  |  ";
		if (i == nb_inputs - 1) cout << endl;
	}
	cout << endl;
	*/
}

void get_input_from_bin(float& input1, float& input2, float& input3)
{
	char* p;
	float* pf;

	ifstream bin;
	bin.open("input.bin", ios::binary);

	char p_buffer_in[12];
	bin.read(p_buffer_in, 12);


	p = p_buffer_in;
	pf = (float*)p;
	input1 = *pf;
	p += sizeof(float);
	pf = (float*)p;
	input2 = *pf;
	p += sizeof(float);
	pf = (float*)p;
	input3 = *pf;

	bin.close();

}

void push_input_to_controller(float input1, float input2, float input3)
{
	char buffer_inputs[12];

	char* p, * pc;
	float* pf;

	p = buffer_inputs;

	pf = (float*)p;
	*pf = input1;
	p += sizeof(float);

	pf = (float*)p;
	*pf = input2;
	p += sizeof(float);

	pf = (float*)p;
	*pf = input3;

	serial_send(buffer_inputs, 12, h1);

}