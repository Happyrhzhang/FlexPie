//127���Ժ� �����Ÿĳ�˫���� 
//update�������if �������Ƿ��в� 

#include<iostream>
#include"net_func.h"
#include"cost.h"
#include <ctime>
//using namespace std;





////���㴫���ʱ--��ģ������ȡ��Ҫ��������ݣ��������ڲ㲻ͬ�Ĵ�������в�ͬ�Ĵ��䴦�� 
//double evaluate_tran_single(int i,string s)
//{
//	//���������㣬��ȡ��Ҫ���еĴ�������
//	//��+��
//	
//	//ͨ��+��
//	
//	//ϸ��+��--��Ϊ��Щ�����ϸ�²������� 
//	
//	// 
//	//�����ǰ����봫�䣬�򲻴����Ӧ��ֵΪ999999�� 
//	return 0;
//}
////�����������⴫�����ĺ�ʱ 
//double evaluate_tran_extra(int i,int extra,string s)
//{
//	//���������㣬��ȡ��Ҫ���еĴ�������
//	//�����ǰ����봫�䣬�򲻴����Ӧ��ֵΪ999999�� 
//	return 0;
//}

////��ǰ�㴫������ ----���Ӧ���ò��ϣ� ���Ҫ�ã�Ӧ���񵥲����������������һ���������⴫��ѡ������������� 
//double evaluate_tran(int i,bool a)
//{
//	//���������㣬��ȡ��Ҫ���еĴ�������
//	//�����ǰ����봫�䣬�򲻴����Ӧ��ֵΪ999999�� 
//	return 0;
//}


////��ʼ ��ǰ�� ��ϱȽ�  ��i�� ��j�� ���i+1���5����� ѡȡ���Ž��и���
//void update_dpnn_zero(int i,int j,char x) 
//{
//	//��ʼ��
//	double min=evaluate_seq(seq[i+1][0]+x);
//	int cord=0;
//	//�Ƚ� 
//	for(int k=1;k<5;k++)
//	{
//		if(evaluate_seq(seq[i+1][k]+x)>min)
//		{
//			min=evaluate_seq(seq[i+1][k]+x);
//			cord=k;
//		}		
//	}
//	//���� ���� ��ǰ����ֵ 
//	update_seq(i,j,i+1,cord,x);
//	 
//}


////��ǰ�� ��ϱȽ�  ��i�� ��j�� ���i+1���10����� ѡȡ���Ž��и���
//void update_dpnn(int i,int j,char x) 
//{
//	//��ʼ��
//	double min=evaluate_seq(seq[i+1][0]+x);
//	int cord=0;
//	//�Ƚ� 
//	for(int k=1;k<10;k++)
//	{
//		if(evaluate_seq(seq[i+1][k]+x)<min)
//		{
//			min=evaluate_seq(seq[i+1][k]+x);
//			cord=k;
//		}		
//	}
//	//���� ���� ��ǰ����ֵ 
//	update_seq(i,j,cord,x);
//	 
//}


string choice[5] = { "a","b","c","d","e" };
//string choice_no[5] = { "a","b","c","d","e" };

string choice_no[5] = { "1","2","3","4","5" };
//��ǰ�� ��ϱȽ�  ��i�� ��j�� ���i+1(cord1)���10����� ѡȡ���Ž��и���
void update_dpnn1(int i, int j)
{
	//��ʼ��
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
	//�Ƚ� ��һ�����괫��
	for (int k = 1;k < 5;k++)
	{
		snow = seq[l][k];
		snow.insert(0, s0);
		double ll = evaluate_seq(snow, l, k);
		//double ll = evaluate_seq(seq[l][k] + s0, l, k);
		if ( ll< min)//�Ż�Ϊ  
		{
			min = ll;
			cord2 = k;
		//pingbi:6	cout <<ll<<" " << "gengxin" << seq[l][k] + s0 << " " << i << " " << k << endl;
		}
	}

	//�Ƚ� ����1�㲻�����  
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
	//�Ƚ� ����2�㲻����� 
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
	
	//�Ƚ� ����3�㲻�����
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
	
	//�Ƚ� ����4�㲻�����
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

	//�Ƚ� ����5�㲻�����
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
	//���� ���� ��ǰ����ֵ 
	update_seq(i, j, cord1, cord2, ss);
	/*cout << "����" << ss << endl;*/
	
	dp_nn[i][j] = min;

}

void update_dpnn(int i, int j)
{
	//��ʼ��
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
	//�Ƚ� ��һ�����괫��
	for (int k = 1;k < 5;k++)
	{
		/*snow = seq[l][k];
		snow.insert(0, s0);*/
		double ll = evaluate_seq(s0+seq[l][k], l, k);
		//double ll = evaluate_seq(seq[l][k] + s0, l, k);
		if (ll < min)//�Ż�Ϊ  
		{
			min = ll;
			cord1 = l;
			cord2 = k;
		//pingbi3:	cout << ll << "-" << "gengxin"<<" " << s0 + seq[l][k] << " " << i << " " << k << endl;
		}
	}
	//�Ƚ� ����1�㲻�����  
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
	////�Ƚ� ����2�㲻����� 
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
	////�Ƚ� ����3�㲻�����
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
	////�Ƚ� ����4�㲻�����
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
	////�Ƚ� ����5�㲻�����
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
	//���� ���� ��ǰ����ֵ 
	update_seq(i, j, cord1, cord2, ss);
	/*cout << "����" << ss << endl;*/

	dp_nn[i][j] = min;

}

//��ǰ�� ��ϱȽ�  ��i�� ��j�� ���i+1(cord1)���10����� ѡȡ���Ž��и���
void update_dpnn_no(int i, int j)
{
	//��ʼ��
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
	//�Ƚ� ��һ�����괫��
	for (int k = 1;k < 5;k++)
	{
		double ll = evaluate_seq(s0 + seq[l][0], l, k);
		if (ll < min)//�Ż�Ϊ  
		{
			min = ll;
			cord1 = l;
			cord2 = k;
		}
	}

	//�Ƚ� ����1�㲻����� +��ʼ������1��=2�������β��� 
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
	//�Ƚ� ����2�㲻����� +1=3 
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

	//�Ƚ� ����3�㲻����� +1=4 
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

	//�Ƚ� ����4�㲻����� +1=5 
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
	//���� ���� ��ǰ����ֵ 
	update_seq(i, j, cord1, cord2, ss);
	dp_nn[i][j] = min;

}






//��̬�滮 
void dp()
{
	//��ʼ��---������ֻ��ֻᵼ�����һ��Ķ��������ʱ��������������ϡ�    
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
	//pingbi12:cout << "��ʼ��dp�ɹ���" << endl;
	for (int i = nn_number - 2;i > 0;i--)
	//for (int i =  nn_number-2;i > nn_number-4;i--)
	{
		//��ȡ��ǰ�㴫�� 
		//dp_nnc[i][0] = evaluate_tran_single(i, seq[i][0]);
		//dp_nnc[i][1] = evaluate_tran_single(i, true);
		//cout<<i<<endl;
		//���㵱ǰ�������  --�Ե�ǰ�㴫��Ϊ��׼ 
		update_dpnn(i, 0);
		//cout<<111<<endl;
		update_dpnn(i, 1);
		update_dpnn(i, 2);
		update_dpnn(i, 3);
		update_dpnn(i, 4);
	//	cout << "��ǰִ�в���Ϊ��" <<i+1<<"�㣡" << endl;
	}
	//cout << "��n-1��ִ�н�����" << endl;
	for (int i = -1;i >= 0;i--)
	//for (int i = 0;i >= 0;i--)
	{
		//��ȡ��ǰ�㴫�� 
		//dp_nnc[i][0] = evaluate_tran_single(i, false);
		//dp_nnc[i][1] = evaluate_tran_single(i, true);
		//��ʼ�������� 
		update_dpnn(i, 0);
		update_dpnn(i, 1);
		update_dpnn(i, 2);
		update_dpnn(i, 3);
		update_dpnn(i, 4);
		//��ʼ�����󲻴��� 
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
	//��ʹ��PythonInit����ʱ����Ҫ�ֹ�Py_Initialize����ʼ��
	Py_Initialize();
	if (!Py_IsInitialized()) {
		std::system("pause");
		//	return -99;
	}

	//������ȡ�ļ�ȡֵ
	pName = PyUnicode_FromString("combined_cost_eval"); //��һ�������Ƿ�ֹ��������
	pModule = PyImport_Import(pName); //�����ģ��
	if (!pModule) {
		cout << "Can't find pModule" << endl;
		std::system("pause");
		//return -98;
	}//�鿴�Ƿ�������ģ��
	//��ʼ������ģ��
	PyObject* pFunc, * pReturn;
	//����python�����������ö���pmodule�ĺ�����
	pFunc = PyObject_GetAttrString(pModule, "init");
	if (!pFunc || !PyCallable_Check(pFunc)) {
		cout << "error: pFunc" << endl;
		std::system("pause");
		//return -96;
	}
	pReturn = PyObject_CallObject(pFunc, NULL);
	//����python�����������ö���pmodule�ĺ���������ȡ������pmodule.print_test2()  xgb_tran1
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