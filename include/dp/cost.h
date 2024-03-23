#ifndef COST_H
#define COST_H
#include<iostream>
#include<string>
#include<map>
#include<Python.h>
using namespace std;

//python���ô�������ģ��

void PythonInit();
double cost_tran(int bandw, int throughput);
double cost_comp(string type, int inp_c, int out_c, int img_h, int img_w, int img_h1, int img_w1, int stride);
double cost_reset(int nums);

//Cpython��������
extern PyObject* pName;
extern PyObject* pModule;
extern PyObject* pFunc_comp;
extern PyObject* pFunc_reset;
extern PyObject* pFunc_tran;
//����м������棬�Ż�����
extern map<string, double> cost_comvalue;
extern map<string, double> cost_tranvalue;
extern map<string, double> cost_resetvalue;


#endif