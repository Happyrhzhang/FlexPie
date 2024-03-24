//#pragma once
#ifndef NET_FUNC_H
#define NET_FUNC_H
#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include <string>
#include<Python.h>
//#include "python.h"
using namespace std;

#define core 4
#define bandwidth 5
#define mobilenet 1
#define resnet18 0
#define resnet101 0

//网络结构定义
//定义层数
#define nn_number 27
//extern int nn_number;

//5Gbps

//定义传输 
extern vector<int> img_height;
extern vector<int> img_width;
extern vector<int> input_channel;
extern vector<int> output_channel;

// 每一层卷积核尺寸 kkc1c2
//int kernel[nn_number][4] = { 0 };替换为
extern vector<int> kernel_size;
//每一层卷积计算的步长
extern vector<int> stride;
//每一层要进行的卷积操作 conv1*1,conv3*3,conv7*7,dwconv3*3,conv5*5
extern vector<string> operate;
extern vector<int> padding;




//五种划分 行+列+十字+通道+细致 
//分别对应 a   b   c    d    e 
//          传输与 不传输
//分别对应    1      0
//保存当前选择的序列 
//static string seq[nn_number][5] = { "1","2","3","4"};
extern vector<vector<string>> seq;
//保存每一步的最优值
extern vector<vector<double>> dp_nn;
//保存当前传输值 
extern vector<vector<int>> dp_nnc;

extern string type;
extern int inp_c;
extern int out_c;
extern int img_h;
extern int img_w;
extern int img_h1;
extern int img_w1;
extern int strid;

//添加中间结果保存，优化搜索
extern map<string,double> cost_comvalue;
extern map<string, double> cost_tranvalue;
extern map<string, double> cost_resetvalue;

//Cpython变量声明
extern PyObject* pName;
extern PyObject* pModule;
extern PyObject* pFunc_comp;
extern PyObject* pFunc_reset;
extern PyObject* pFunc_tran;
//PyObject* pArgs;
//PyObject* pReturn;
// 
//字符串转整数
int stoint(string s);
//函数定义
void loads();
//更新序列的函数，更新第i组的第j个序列 为 第i+1组第k个加上当前 划分 或 传输 对应的选择 
void update_seq(int i, int j, int l, int k, string x);

//单层评估--适用于无需考虑前后层 
double evaluate_single(int i,int l, string s,int ext);

//单层包含额外计算的评估  第三个参是表示额外计算多少份 
double evaluate_comp_extra(int i,int l, string s, int j);

//根据配置获取当前评估  根据给定的序列进行评估 
double evaluate_seq(string s, int l, int k);
void evaluate_seq_test(string s);

//传输评估单层 ---分别设计函数进行评估evalute_aa_single
double evaluate_tran_single(int i, string s);
//评估包含额外的传输 ---分别设计函数进行评估evalute_aa_extra
//double evaluate_tran_extra(int i, string s, int ext);
double evaluate_tran_extra(int i,int l, string s);

//后处理评估单层 ---分别设计函数进行评估evalute_aa_single
double evaluate_reset_single(int i, string s);
//double evaluate_reset_extra(int i, string s);
double evaluate_reset_extra(int i,int l, string s);
//从序列头计算当前层已连续有多少层为传输
int compute_layer_extra(int now,int end,string s);
int tran_layer_extra(int now, int end, string s);

//关于额外传输，只有五种，其他情况会被舍弃，只有相邻层相同划分可以采纳，添加额外尺寸计算额外传输量即可


//第一次变量定义问题：在.h文件定义静态变量，导致在两个cpp文件中，各自有个该变量的值，两个cpp中该变量是相互独立的；
//bad_alloc问题，大量迭代，有多变量定义，可能导致了内存溢出；将变量定义到最外面，定义一次；
//insert函数使用问题，导致了字符串超长等引起一系列问题；



#endif
