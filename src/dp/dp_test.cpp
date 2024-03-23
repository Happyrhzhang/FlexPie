//127行以后 单引号改成双引号 
//update函数添加if 看后面是否还有层 

#include<iostream>
#include"net_func.h"
#include"cost.h"
#include <ctime>
//using namespace std;





////单层传输耗时--从模型中提取需要传输的数据，对于相邻层不同的传输组合有不同的传输处理 
//double evaluate_tran_single(int i,string s)
//{
//	//分析关联层，获取需要进行的传输量，
//	//行+行
//	
//	//通道+行
//	
//	//细致+行--因为有些层可能细致并不合算 
//	
//	// 
//	//如果当前层必须传输，则不传输对应的值为999999； 
//	return 0;
//}
////评估包含额外传输量的耗时 
//double evaluate_tran_extra(int i,int extra,string s)
//{
//	//分析关联层，获取需要进行的传输量，
//	//如果当前层必须传输，则不传输对应的值为999999； 
//	return 0;
//}

////当前层传输评估 ----这个应该用不上， 如果要用，应该像单层计算的那样，在设计一个包含额外传输选项的评估函数。 
//double evaluate_tran(int i,bool a)
//{
//	//分析关联层，获取需要进行的传输量，
//	//如果当前层必须传输，则不传输对应的值为999999； 
//	return 0;
//}


////初始 当前层 组合比较  第i层 第j个 与第i+1层的5个组合 选取最优进行更新
//void update_dpnn_zero(int i,int j,char x) 
//{
//	//初始化
//	double min=evaluate_seq(seq[i+1][0]+x);
//	int cord=0;
//	//比较 
//	for(int k=1;k<5;k++)
//	{
//		if(evaluate_seq(seq[i+1][k]+x)>min)
//		{
//			min=evaluate_seq(seq[i+1][k]+x);
//			cord=k;
//		}		
//	}
//	//更新 序列 当前最优值 
//	update_seq(i,j,i+1,cord,x);
//	 
//}


////当前层 组合比较  第i层 第j个 与第i+1层的10个组合 选取最优进行更新
//void update_dpnn(int i,int j,char x) 
//{
//	//初始化
//	double min=evaluate_seq(seq[i+1][0]+x);
//	int cord=0;
//	//比较 
//	for(int k=1;k<10;k++)
//	{
//		if(evaluate_seq(seq[i+1][k]+x)<min)
//		{
//			min=evaluate_seq(seq[i+1][k]+x);
//			cord=k;
//		}		
//	}
//	//更新 序列 当前最优值 
//	update_seq(i,j,cord,x);
//	 
//}


string choice[5] = { "a","b","c","d","e" };
//string choice_no[5] = { "a","b","c","d","e" };

string choice_no[5] = { "1","2","3","4","5" };
//当前层 组合比较  第i层 第j个 与第i+1(cord1)层的10个组合 选取最优进行更新
void update_dpnn1(int i, int j)
{
	//初始化
	int l = i + 1;
	int cord2 = 0;
	int cord1 = l;
	string ss;
	string snow;
	string s0;
	string s1;
	string s2;
	string s3;
	string s4;
	string s5;
	s0 = choice[j];
	ss = s0;

	//double min = evaluate_seq(seq[l][0] + s0 , l, 0);
	snow = seq[l][0];
	snow.insert(0, s0);
	double min = evaluate_seq(snow, l, 0);
	//比较 下一层做完传输
	for (int k = 1;k < 5;k++)
	{
		snow = seq[l][k];
		snow.insert(0, s0);
		double ll = evaluate_seq(snow, l, k);
		//double ll = evaluate_seq(seq[l][k] + s0, l, k);
		if ( ll< min)//优化为  
		{
			min = ll;
			cord2 = k;
		//pingbi:6	cout <<ll<<" " << "gengxin" << seq[l][k] + s0 << " " << i << " " << k << endl;
		}
	}

	//比较 做完1层不传输的  
	l += 1;
	if (l < nn_number)
	{
		for (int m = 0;m < 5;m++)
		{
			s1 = s0;
			//s1.insert(0, choice[m]);
			// 
			//cout << s0 << "shuchus1" << s1 << endl;
			for (int k = 0;k < 5;k++)
			{
				double ll = evaluate_seq(seq[l][k] + s1, l, k);
				//cout << seq[l][k] + s1 << "shuchus1" << seq[l][k] << endl;
				if (ll < min)
				{
					min = ll;
					cord1 = l;
					cord2 = k;
					ss = s1;
			//pingbi5:		cout << i << " " << m << " " << k << endl;
				}
			}
		}
	}
	//比较 做完2层不传输的 
	l += 1;
	if (l < nn_number)
	{
		for (int m = 0;m < 5;m++)
		{
			s1 = s0;
			s1.insert(0, choice[m]);
			//s1 = s0.insert(0, choice_no[m]);
			for (int n = 0;n < 5;n++)
			{
				s2 = s1;
				s2.insert(0, choice_no[n]);
				for (int k = 0;k < 5;k++)
				{
					double ll = evaluate_seq(seq[l][k] + s2, l, k);
					if (ll < min)
					{
						min = ll;
						cord1 = l;
						cord2 = k;
						ss = s2;
					}
				}
			}
		}
	}
	
	//比较 做完3层不传输的
	l += 1;
	if (l < nn_number)
	{
		for (int m = 0;m < 5;m++)
		{
			s1 = s0;
			s1.insert(0, choice_no[m]);
			for (int n = 0;n < 5;n++)
			{
				s2 = s1;
				s2.insert(0, choice_no[n]);
				for (int o = 0;o < 5;o++)
				{
					s3 = s2;
					s3.insert(0, choice_no[o]);
					for (int k = 0;k < 5;k++)
					{
						double ll = evaluate_seq(seq[l][k] + s3, l, k);
						if (ll< min)
						{
							min = ll;
							cord1 = l;
							cord2 = k;
							ss = s3;
						}
					}
				}
			}
		}
	}
	
	//比较 做完4层不传输的
	l += 1;
	if (l < nn_number)
	{
		for (int m = 0;m < 5;m++)
		{
			s1 = s0;
			s1.insert(0, choice_no[m]);
			for (int n = 0;n < 5;n++)
			{
				s2 = s1;
				s2.insert(0, choice_no[n]);
				for (int o = 0;o < 5;o++)
				{
					s3 = s2;
					s3.insert(0, choice_no[o]);
					for (int p = 0;p < 5;p++)
					{
						s4 = s3;
						s4.insert(0, choice_no[p]);
						for (int k = 0;k < 5;k++)
						{
							double ll = evaluate_seq(seq[l][k] + s4, l, k);
							if (ll < min)
							{
								min = ll;
								cord1 = l;
								cord2 = k;
								ss = s4;
							}
						}
					}
				}
			}
		}
	}

	//比较 做完5层不传输的
	l += 1;
	if (l < nn_number)
	{
	//pingbi7:	cout << 10 << endl;
		for (int m = 0;m < 5;m++)
		{
			s1 = s0;
			s1.insert(0, choice_no[m]);
			
			for (int n = 0;n < 5;n++)
			{
				s2 = s1;
				s2.insert(0, choice_no[n]);
				for (int o = 0;o < 5;o++)
				{
					s3 = s2;
					s3.insert(0, choice_no[o]);
					for (int p = 0;p < 5;p++)
					{
						//cout << m << "   " << n << "     " << o << "    " << p << "    " << 11 << endl;
						s4 = s3;
						s4.insert(0, choice_no[p]);
						for (int q = 0;q < 5;q++)
						{
							s5 = s4;
							s5.insert(0, choice_no[q]);
							for (int k = 0;k < 5;k++)
							{
								double ll = evaluate_seq(seq[l][k] + s5, l, k);
								if (ll < min)
								{
									min = ll;
									cord1 = l;
									cord2 = k;
									ss = s5;
								}
								//if(i<=3)
								//	cout << m << "   " << n << "     " << o << "    " << p << "    " << q << "   " << 11 << endl;
							}
						}
					}
				}
			}//cout << "daozhel" << endl;
		}
	}
	//pingbi4:cout << 111 << endl;
	//更新 序列 当前最优值 
	update_seq(i, j, cord1, cord2, ss);
	/*cout << "更新" << ss << endl;*/
	
	dp_nn[i][j] = min;

}

void update_dpnn(int i, int j)
{
	//初始化
	int l = i + 1;
	int cord2 = 0;
	int cord1 = l;
	string ss;
	//string snow;
	string s0;
	string s1;
	string s2;
	string s3;
	string s4;
	string s5;
	s0 = choice[j];
	ss = s0;

	//double min = evaluate_seq(seq[l][0] + s0 , l, 0);
	/*snow = seq[l][0];
	snow.insert(0, s0);*/
	double min = evaluate_seq(s0+ seq[l][0], l, 0);
	//比较 下一层做完传输
	for (int k = 1;k < 5;k++)
	{
		/*snow = seq[l][k];
		snow.insert(0, s0);*/
		double ll = evaluate_seq(s0+seq[l][k], l, k);
		//double ll = evaluate_seq(seq[l][k] + s0, l, k);
		if (ll < min)//优化为  
		{
			min = ll;
			cord1 = l;
			cord2 = k;
		//pingbi3:	cout << ll << "-" << "gengxin"<<" " << s0 + seq[l][k] << " " << i << " " << k << endl;
		}
	}
	//比较 做完1层不传输的  
	l += 1;
	if (l < nn_number)
	{
		for (int m = 0;m < 5;m++)
		{
			s1 = s0+ choice_no[m];
			for (int k = 0;k < 5;k++)
			{
				double ll = evaluate_seq(s1+seq[l][k], l, k);
				//cout << seq[l][k] + s1 << "shuchus1" << seq[l][k] << endl;
				if (ll < min)
				{
					cout <<"min:::  " << min << endl;
					cout << ll << endl;
					cout << s1 << endl;
					cout << seq[l][k] << endl;
					cout << l << endl;
					cout << k << endl;
					cout << dp_nn[26][k] << endl;
					min = ll;
					cord1 = l;
					cord2 = k;
					ss = s1;
				//	cout << ll << "- " << "gengxin" <<" "<<"s1:" <<s1<<" " << s1 + seq[l][k] << " " << i << " " << k << endl;
				}
			}
		}
	}
	////比较 做完2层不传输的 
	l += 1;
	if (l < nn_number)
	{
		for (int m = 0;m < 5;m++)
		{
			s1 = s0+ choice_no[m];
			for (int n = 0;n < 5;n++)
			{
				s2 = s1+ choice_no[n];
				for (int k = 0;k < 5;k++)
				{
					double ll = evaluate_seq(s2+seq[l][k], l, k);
					if (ll < min)
					{
						min = ll;
						cord1 = l;
						cord2 = k;
						ss = s2;
					}
				}
			}
		}
	}
	////比较 做完3层不传输的
	l += 1;
	if (l < nn_number)
	{
		for (int m = 0;m < 5;m++)
		{
			s1 = s0+ choice_no[m];
			for (int n = 0;n < 5;n++)
			{
				s2 = s1+ choice_no[n];
				for (int o = 0;o < 5;o++)
				{
					s3 = s2+ choice_no[o];
					for (int k = 0;k < 5;k++)
					{
						double ll = evaluate_seq(s3+seq[l][k], l, k);
						if (ll < min)
						{
							min = ll;
							cord1 = l;
							cord2 = k;
							ss = s3;
						}
					}
				}
			}
		}
	}
	////比较 做完4层不传输的
	l += 1;
	if (l < nn_number)
	{
		for (int m = 0;m < 5;m++)
		{
			s1 = s0+choice_no[m];
			for (int n = 0;n < 5;n++)
			{
				s2 = s1+ choice_no[n];
				for (int o = 0;o < 5;o++)
				{
					s3 = s2+ choice_no[o];
					for (int p = 0;p < 5;p++)
					{
						s4 = s3+ choice_no[p];
						for (int k = 0;k < 5;k++)
						{
							double ll = evaluate_seq(s4+ seq[l][k], l, k);
							if (ll < min)
							{
								min = ll;
								cord1 = l;
								cord2 = k;
								ss = s4;
							}
						}
					}
				}
			}
		}
	}
	////比较 做完5层不传输的
	l += 1;
	if (l < nn_number)
	{
		//pingbi:8cout << 10 << endl;
		for (int m = 0;m < 5;m++)
		{
			s1 = s0+ choice_no[m];
			for (int n = 0;n < 5;n++)
			{
				s2 = s1 + choice_no[n];
				for (int o = 0;o < 5;o++)
				{
					s3 = s2 + choice_no[o];
					for (int p = 0;p < 5;p++)
					{
						//cout << m << "   " << n << "     " << o << "    " << p << "    " << 11 << endl;
						s4 = s3 + choice_no[p];
						for (int q = 0;q < 5;q++)
						{
							s5 = s4 + choice_no[q];
							for (int k = 0;k < 5;k++)
							{
								double ll = evaluate_seq(s5+ seq[l][k], l, k);
								if (ll < min)
								{
									min = ll;
									cord1 = l;
									cord2 = k;
									ss = s5;
								}
								//if(i<=3)
								//	cout << m << "   " << n << "     " << o << "    " << p << "    " << q << "   " << 11 << endl;
							}
						}
					}
				}
			}//cout << "daozhel" << endl;
		}
	}
	//pingbi2:cout << 111 << endl;
	//更新 序列 当前最优值 
	update_seq(i, j, cord1, cord2, ss);
	/*cout << "更新" << ss << endl;*/

	dp_nn[i][j] = min;

}

//当前层 组合比较  第i层 第j个 与第i+1(cord1)层的10个组合 选取最优进行更新
void update_dpnn_no(int i, int j)
{
	//初始化
	int l = i + 1;
	int cord2 = 0;
	int cord1 = l;
	string ss;
	string s0;
	string s1;
	string s2;
	string s3;
	string s4;
	string s5;
	s0 = choice_no[j];
	ss = s0;

	double min = evaluate_seq(s0+seq[l][0], l, 0);
	//比较 下一层做完传输
	for (int k = 1;k < 5;k++)
	{
		double ll = evaluate_seq(s0 + seq[l][0], l, k);
		if (ll < min)//优化为  
		{
			min = ll;
			cord1 = l;
			cord2 = k;
		}
	}

	//比较 做完1层不传输的 +初始不传的1层=2连续两次不传 
	l += 1;
	if (l < nn_number)
	{
		for (int m = 0;m < 5;m++)
		{
			s1 = s0+ choice_no[m];
			for (int k = 0;k < 5;k++)
			{
				double ll = evaluate_seq(s1 + seq[l][k], l, k);
				if (ll < min)
				{
					min = ll;
					cord1 = l;
					cord2 = k;
					ss = s1;
				}
			}
		}
	}
	//比较 做完2层不传输的 +1=3 
	l += 1;
	if (l < nn_number)
	{
		for (int m = 0;m < 5;m++)
		{
			s1 = s0+ choice_no[m];
			for (int n = 0;n < 5;n++)
			{
				s2 = s1+ choice_no[n];
				for (int k = 0;k < 5;k++)
				{
					double ll = evaluate_seq(s2 + seq[l][k], l, k);
					if (ll < min)
					{
						min = ll;
						cord1 = l;
						cord2 = k;
						ss = s2;
					}
				}
			}
		}
	}

	//比较 做完3层不传输的 +1=4 
	l += 1;
	if (l < nn_number)
	{
		for (int m = 0;m < 5;m++)
		{
			s1 = s0+ choice_no[m];
			for (int n = 0;n < 5;n++)
			{
				s2 = s1+ choice_no[n];
				for (int o = 0;o < 5;o++)
				{
					s3 = s2+ choice_no[o];
					for (int k = 0;k < 5;k++)
					{
						double ll = evaluate_seq(s3 + seq[l][k], l, k);
						if (ll< min)
						{
							min = ll;
							cord1 = l;
							cord2 = k;
							ss = s3;
						}
					}
				}
			}
		}
	}

	//比较 做完4层不传输的 +1=5 
	l += 1;
	if (l < nn_number)
	{
		for (int m = 0;m < 5;m++)
		{
			s1 = s0+ choice_no[m];
			for (int n = 0;n < 5;n++)
			{
				s2 = s1+ choice_no[n];
				for (int o = 0;o < 5;o++)
				{
					s3 = s2+ choice_no[o];
					for (int p = 0;p < 5;p++)
					{
						s4 = s3+ choice_no[p];
						for (int k = 0;k < 5;k++)
						{
							double ll = evaluate_seq(s4 + seq[l][k], l, k);
							if (ll < min)
							{
								min = ll;
								cord1 = l;
								cord2 = k;
								ss = s4;
							}
						}
					}
				}
			}
		}
	}
	//pingbi11:cout << 111 << endl;
	//更新 序列 当前最优值 
	update_seq(i, j, cord1, cord2, ss);
	dp_nn[i][j] = min;

}






//动态规划 
void dp()
{
	//初始化---如果这种划分会导致最后一层的额外操作耗时，可以在这里加上。    
	dp_nn[nn_number - 1][0] = evaluate_single(nn_number - 1,0, "a",-1);
	seq[nn_number - 1][0] = "a";
	dp_nn[nn_number - 1][1] = evaluate_single(nn_number - 1,0, "b",-1);
	seq[nn_number - 1][1] = "b";
	dp_nn[nn_number - 1][2] = evaluate_single(nn_number - 1,0, "c",-1);
	seq[nn_number - 1][2] = "c";
	dp_nn[nn_number - 1][3] = evaluate_single(nn_number - 1, 0,"d",-1);
	seq[nn_number - 1][3] = "d";
	dp_nn[nn_number - 1][4] = evaluate_single(nn_number - 1,0, "e",-1);
	seq[nn_number - 1][4] = "e";
	//pingbi12:cout << "初始化dp成功！" << endl;
	for (int i = nn_number - 2;i > 0;i--)
	//for (int i =  nn_number-2;i > nn_number-4;i--)
	{
		//获取当前层传输 
		//dp_nnc[i][0] = evaluate_tran_single(i, seq[i][0]);
		//dp_nnc[i][1] = evaluate_tran_single(i, true);
		//cout<<i<<endl;
		//计算当前最优组合  --以当前层传输为基准 
		update_dpnn(i, 0);
		//cout<<111<<endl;
		update_dpnn(i, 1);
		update_dpnn(i, 2);
		update_dpnn(i, 3);
		update_dpnn(i, 4);
	//	cout << "当前执行层数为第" <<i+1<<"层！" << endl;
	}
	//cout << "后n-1层执行结束！" << endl;
	for (int i = -1;i >= 0;i--)
	//for (int i = 0;i >= 0;i--)
	{
		//获取当前层传输 
		//dp_nnc[i][0] = evaluate_tran_single(i, false);
		//dp_nnc[i][1] = evaluate_tran_single(i, true);
		//初始层计算后传输 
		update_dpnn(i, 0);
		update_dpnn(i, 1);
		update_dpnn(i, 2);
		update_dpnn(i, 3);
		update_dpnn(i, 4);
		//初始层计算后不传输 
	/*	update_dpnn_no(i, 0);
		update_dpnn_no(i, 1);
		update_dpnn_no(i, 2);
		update_dpnn_no(i, 3);
		update_dpnn_no(i, 4);*/
	}
	//pingbi10:cout << 50 << endl;
}


int main()
{
	//PythonInit();
	printf("search begin!");
	time_t time1 = time(nullptr);
	//不使用PythonInit函数时，需要手工Py_Initialize。初始化
	Py_Initialize();
	if (!Py_IsInitialized()) {
		std::system("pause");
		//	return -99;
	}

	//变量读取文件取值
	pName = PyUnicode_FromString("combined_cost_eval"); //这一步作用是防止中文乱码
	pModule = PyImport_Import(pName); //载入的模块
	if (!pModule) {
		cout << "Can't find pModule" << endl;
		std::system("pause");
		//return -98;
	}//查看是否载入了模块
	//初始化加载模型
	PyObject* pFunc, * pReturn;
	//调用python函数，即调用对象pmodule的函数，
	pFunc = PyObject_GetAttrString(pModule, "init");
	if (!pFunc || !PyCallable_Check(pFunc)) {
		cout << "error: pFunc" << endl;
		std::system("pause");
		//return -96;
	}
	pReturn = PyObject_CallObject(pFunc, NULL);
	//调用python函数，即调用对象pmodule的函数，即获取到函数pmodule.print_test2()  xgb_tran1
	pFunc_comp = PyObject_GetAttrString(pModule, "xgb_comp");
	if (!pFunc_comp || !PyCallable_Check(pFunc_comp)) {
		cout << "error: pFunc" << endl;
		std::system("pause");
		//return -96;
	}
	pFunc_reset = PyObject_GetAttrString(pModule, "xgb_reset");
	if (!pFunc_reset || !PyCallable_Check(pFunc_reset)) {
		cout << "error: pFunc" << endl;
		std::system("pause");
		//return -96;
	}
	pFunc_tran = PyObject_GetAttrString(pModule, "xgb_tran");
	if (!pFunc_tran || !PyCallable_Check(pFunc_tran)) {
		cout << "error: pFunc" << endl;
		std::system("pause");
		//return -96;
	}

	loads();
	dp();
	string ss = "aaaa111aaaaaaaaaaaaaaaaaaaa";
	//string ss = "ccccccccccccccccccccccccccc";
	evaluate_seq_test(ss);
	time_t time2 = time(nullptr);

	double time_diff = difftime(time2, time1);
	cout << "time1: " << time1 << endl;
	cout << "time2: " << time2 << endl;
	cout << "time_diff: " << time_diff << "s" << endl;
	printf("search end!\n");
	for (int i = 0;i < nn_number;i++)
	{
		cout << i << endl;
		for (int j = 0;j < 5;j++)
		{
			cout << seq[i][j] << endl;
		}
	}
	cout << dp_nn[1][0] << endl;
	cout << dp_nn[25][0] << endl;
	cout << dp_nn[25][1] << endl;
	cout << dp_nn[25][2] << endl;
	cout << dp_nn[25][3] << endl;
	cout << dp_nn[25][4] << endl;
	cout << dp_nn[24][0] << endl;
	cout << dp_nn[24][1] << endl;
	cout << dp_nn[24][2] << endl;
	cout << dp_nn[24][3] << endl;
	cout << dp_nn[24][4] << endl;
	cout << dp_nn[23][0] << endl;
	cout << dp_nn[23][1] << endl;
	cout << dp_nn[23][2] << endl;
	cout << dp_nn[23][3] << endl;
	cout << dp_nn[23][4] << endl;
	
}