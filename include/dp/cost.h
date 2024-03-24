#ifndef COST_H
#define COST_H
#include<iostream>
#include<string>
#include<map>
#include<Python.h>
using namespace std;

//python调用代价评估模型

void PythonInit();
double cost_tran(int bandw, int throughput);
double cost_comp(string type, int inp_c, int out_c, int img_h, int img_w, int img_h1, int img_w1, int stride);
double cost_reset(int nums);

//Cpython变量声明
extern PyObject* pName;
extern PyObject* pModule;
extern PyObject* pFunc_comp;
extern PyObject* pFunc_reset;
extern PyObject* pFunc_tran;
//添加中间结果保存，优化搜索
extern map<string, double> cost_comvalue;
extern map<string, double> cost_tranvalue;
extern map<string, double> cost_resetvalue;


#endif