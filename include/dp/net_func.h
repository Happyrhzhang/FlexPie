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

//����ṹ����
//�������
#define nn_number 27
//extern int nn_number;

//5Gbps

//���崫�� 
extern vector<int> img_height;
extern vector<int> img_width;
extern vector<int> input_channel;
extern vector<int> output_channel;

// ÿһ�����˳ߴ� kkc1c2
//int kernel[nn_number][4] = { 0 };�滻Ϊ
extern vector<int> kernel_size;
//ÿһ��������Ĳ���
extern vector<int> stride;
//ÿһ��Ҫ���еľ������ conv1*1,conv3*3,conv7*7,dwconv3*3,conv5*5
extern vector<string> operate;
extern vector<int> padding;




//���ֻ��� ��+��+ʮ��+ͨ��+ϸ�� 
//�ֱ��Ӧ a   b   c    d    e 
//          ������ ������
//�ֱ��Ӧ    1      0
//���浱ǰѡ������� 
//static string seq[nn_number][5] = { "1","2","3","4"};
extern vector<vector<string>> seq;
//����ÿһ��������ֵ
extern vector<vector<double>> dp_nn;
//���浱ǰ����ֵ 
extern vector<vector<int>> dp_nnc;

extern string type;
extern int inp_c;
extern int out_c;
extern int img_h;
extern int img_w;
extern int img_h1;
extern int img_w1;
extern int strid;

//����м������棬�Ż�����
extern map<string,double> cost_comvalue;
extern map<string, double> cost_tranvalue;
extern map<string, double> cost_resetvalue;

//Cpython��������
extern PyObject* pName;
extern PyObject* pModule;
extern PyObject* pFunc_comp;
extern PyObject* pFunc_reset;
extern PyObject* pFunc_tran;
//PyObject* pArgs;
//PyObject* pReturn;
// 
//�ַ���ת����
int stoint(string s);
//��������
void loads();
//�������еĺ��������µ�i��ĵ�j������ Ϊ ��i+1���k�����ϵ�ǰ ���� �� ���� ��Ӧ��ѡ�� 
void update_seq(int i, int j, int l, int k, string x);

//��������--���������迼��ǰ��� 
double evaluate_single(int i,int l, string s,int ext);

//�������������������  ���������Ǳ�ʾ���������ٷ� 
double evaluate_comp_extra(int i,int l, string s, int j);

//�������û�ȡ��ǰ����  ���ݸ��������н������� 
double evaluate_seq(string s, int l, int k);
void evaluate_seq_test(string s);

//������������ ---�ֱ���ƺ�����������evalute_aa_single
double evaluate_tran_single(int i, string s);
//������������Ĵ��� ---�ֱ���ƺ�����������evalute_aa_extra
//double evaluate_tran_extra(int i, string s, int ext);
double evaluate_tran_extra(int i,int l, string s);

//������������ ---�ֱ���ƺ�����������evalute_aa_single
double evaluate_reset_single(int i, string s);
//double evaluate_reset_extra(int i, string s);
double evaluate_reset_extra(int i,int l, string s);
//������ͷ���㵱ǰ���������ж��ٲ�Ϊ����
int compute_layer_extra(int now,int end,string s);
int tran_layer_extra(int now, int end, string s);

//���ڶ��⴫�䣬ֻ�����֣���������ᱻ������ֻ�����ڲ���ͬ���ֿ��Բ��ɣ���Ӷ���ߴ������⴫��������


//��һ�α����������⣺��.h�ļ����徲̬����������������cpp�ļ��У������и��ñ�����ֵ������cpp�иñ������໥�����ģ�
//bad_alloc���⣬�����������ж�������壬���ܵ������ڴ���������������嵽�����棬����һ�Σ�
//insert����ʹ�����⣬�������ַ�������������һϵ�����⣻



#endif
