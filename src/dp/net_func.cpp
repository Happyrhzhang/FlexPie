#include"net_func.h"
#include"cost.h"

//����
vector<int> img_height(nn_number);
vector<int> img_width(nn_number);
vector<int> input_channel(nn_number);
vector<int> output_channel(nn_number);
vector<int> kernel_size(nn_number);
vector<int> stride(nn_number);
vector<int> padding(nn_number);
vector<string> operate(nn_number);

vector<vector<string>>seq(nn_number, vector<string>(5));
vector<vector<double>>dp_nn(nn_number, vector<double>(5));
vector<vector<int>>dp_nnc(nn_number, vector<int>(2));

string type = " ";
int inp_c = 0;
int out_c = 0;
int img_h = 0;
int img_w = 0;
int img_h1 = 0;
int img_w1 = 0;
int strid = 0;

//����м������棬�Ż�����
map<string, double> cost_comvalue;
map<string, double> cost_tranvalue;
map<string, double> cost_resetvalue;

//Cpython��������
PyObject* pName;
PyObject* pModule;
PyObject* pFunc_comp;
PyObject* pFunc_reset;
PyObject* pFunc_tran;


//�ַ���ת����
int stoint(string s)
{
	int nn = s.length();
	int sum = 0;
	for (int i = 0;i < nn;i++)
	{
		sum = sum * 10;
		sum += s[i] - '0';
	}
	return sum;
}
//����ģ�Ͳ���
void loads()
{
	ifstream infile;
	infile.open("ir.txt", ios::in);
	if (!infile.is_open())
	{
		cout << "��ȡ�ļ�ʧ��" << endl;
		return;
	}
	cout << "�ļ���ȡ�ɹ��ˣ�" << endl;
	string buf;
	getline(infile, buf);
	//int nn_number = stoint(buf);
	cout << buf << endl;

	getline(infile, buf);
	int buf_n = buf.length();
	string buf1;
	int i = 0;
	int lef, rig;
	lef = -1;
	rig = -1;
	for (int j = 0;j < nn_number;j++)
	{
		for (;i < buf_n;i++)
		{
			if (buf[i] >= '0' && buf[i] <= '9')
			{
				lef = i;
				break;
			}
		}
		for (;i < buf_n;i++)
		{
			if (buf[i] < '0' || buf[i] > '9')
			{
				rig = i - 1;
				break;
			}
		}
		if (lef != -1 && rig != -1)
		{
			buf1 = buf.substr(lef, rig - lef + 1);
			img_height[j] = stoint(buf1);
			//cout << img_height[j] << " sss  " << endl;
		}
		lef = -1;
		rig = -1;
	}
	getline(infile, buf);
	buf_n = buf.length();
	i = 0;
	lef = -1;
	rig = -1;
	for (int j = 0;j < nn_number;j++)
	{
		for (;i < buf_n;i++)
		{
			if (buf[i] >= '0' && buf[i] <= '9')
			{
				lef = i;
				break;
			}
		}
		for (;i < buf_n;i++)
		{
			if (buf[i] < '0' || buf[i] > '9')
			{
				rig = i - 1;
				break;
			}
		}
		if (lef != -1 && rig != -1)
		{
			buf1 = buf.substr(lef, rig - lef + 1);
			img_width[j] = stoint(buf1);
		}
		lef = -1;
		rig = -1;

	}
	getline(infile, buf);
	buf_n = buf.length();
	i = 0;
	lef = -1;
	rig = -1;
	for (int j = 0;j < nn_number;j++)
	{
		for (;i < buf_n;i++)
		{
			if (buf[i] >= '0' && buf[i] <= '9')
			{
				lef = i;
				break;
			}
		}
		for (;i < buf_n;i++)
		{
			if (buf[i] < '0' || buf[i] > '9')
			{
				rig = i - 1;
				break;
			}
		}
		if (lef != -1 && rig != -1)
		{
			buf1 = buf.substr(lef, rig - lef + 1);
			input_channel[j] = stoint(buf1);
		}
		lef = -1;
		rig = -1;

	}
	getline(infile, buf);
	buf_n = buf.length();
	i = 0;
	lef = -1;
	rig = -1;
	for (int j = 0;j < nn_number;j++)
	{
		for (;i < buf_n;i++)
		{
			if (buf[i] >= '0' && buf[i] <= '9')
			{
				lef = i;
				break;
			}
		}
		for (;i < buf_n;i++)
		{
			if (buf[i] < '0' || buf[i] > '9')
			{
				rig = i - 1;
				break;
			}
		}
		if (lef != -1 && rig != -1)
		{
			buf1 = buf.substr(lef, rig - lef + 1);
			output_channel[j] = stoint(buf1);
		}
		lef = -1;
		rig = -1;
	}
	getline(infile, buf);
	buf_n = buf.length();
	i = 0;
	lef = -1;
	rig = -1;
	for (int j = 0;j < nn_number;j++)
	{
		for (;i < buf_n;i++)
		{
			if (buf[i] == '\'')
			{
				lef = i + 1;
				i++;
				break;
			}
		}
		for (;i < buf_n;i++)
		{
			if (buf[i] == '\'')
			{
				rig = i - 1;
				i++;
				break;
			}
		}
		if (lef != -1 && rig != -1)
		{
			buf1 = buf.substr(lef, rig - lef + 1);
			operate[j] =buf1;
		}
		lef = -1;
		rig = -1;

	}
	getline(infile, buf);
	buf_n = buf.length();
	i = 0;
	lef = -1;
	rig = -1;
	for (int j = 0;j < nn_number;j++)
	{
		for (;i < buf_n;i++)
		{
			if (buf[i] >= '0' && buf[i] <= '9')
			{
				lef = i;
				break;
			}
		}
		for (;i < buf_n;i++)
		{
			if (buf[i] < '0' || buf[i] > '9')
			{
				rig = i - 1;
				break;
			}
		}
		if (lef != -1 && rig != -1)
		{
			buf1 = buf.substr(lef, rig - lef + 1);
			kernel_size[j] = stoint(buf1);
		}
		lef = -1;
		rig = -1;
	}
	getline(infile, buf);
	buf_n = buf.length();
	i = 0;
	lef = -1;
	rig = -1;
	for (int j = 0;j < nn_number;j++)
	{
		for (;i < buf_n;i++)
		{
			if (buf[i] >= '0' && buf[i] <= '9')
			{
				lef = i;
				break;
			}
		}
		for (;i < buf_n;i++)
		{
			if (buf[i] < '0' || buf[i] > '9')
			{
				rig = i - 1;
				break;
			}
		}
		if (lef != -1 && rig != -1)
		{
			buf1 = buf.substr(lef, rig - lef + 1);
			padding[j] = stoint(buf1);
			//cout << padding[j] << "  sss  " << endl;
		}
		lef = -1;
		rig = -1;
	}
	getline(infile, buf);
	buf_n = buf.length();
	i = 0;
	lef = -1;
	rig = -1;
	for (int j = 0;j < nn_number;j++)
	{
		for (;i < buf_n;i++)
		{
			if (buf[i] >= '0' && buf[i] <= '9')
			{
				lef = i;
				break;
			}
		}
		for (;i < buf_n;i++)
		{
			if (buf[i] < '0' || buf[i] > '9')
			{
				rig = i - 1;
				break;
			}
		}
		if (lef != -1 && rig != -1)
		{
			buf1 = buf.substr(lef, rig - lef + 1);
			stride[j] = stoint(buf1);
		}
		lef = -1;
		rig = -1;
	}
	for (int i = 0;i < nn_number;i++)
	{
		cout << img_height[i]<<' ' << img_width[i] << ' ' << operate[i] << ' ' << kernel_size[i] << endl;

	}
}
//�������еĺ��������µ�i��ĵ�j������ Ϊ ��i+1���k�����ϵ�ǰ ���� �� ���� ��Ӧ��ѡ�� 
void update_seq(int i, int j, int l, int k, string x)
{
	//seq[i][j] = seq[l][k] + x;
	seq[i][j] =  x+ seq[l][k];
	//cout << i << ' ' << j << ' ' << seq[i][j] << ' ' << l << ' ' << k << ' ' << seq[l][k] << endl;
}


//��������--���������迼��ǰ���  ����ʮ��ͨ������
double evaluate_single(int i,int l, string s, int ext)//����iֵ��ȥ�ַ���s��ȡ��Ӧ���ַ�
{//ext���ڳ�ʼ�㣬����ʼ�㲻���䣬�൱������ext+1��û�д��䣬��Ҫ�������ext+1��
	double costcom;
	int reset_i;//��ֹi+1Խ��
	if (i + 1 >= nn_number)
		reset_i = i;
	else
		reset_i = i + 1;
	int n = s.length();
	int j =  i-(nn_number-n);//������i�㣬��Ӧ������nnnumber-1-i�㣬Ϊj
	char oper = s[j];//���ַ���s��ȡ��j���ַ�����Ӧ��i��Ļ��ֲ���
	if (oper == 'a')
	{
		//cout <<"�鿴iֵ��" << i << endl;
		/*try {
			type = operate[i];
		}
		catch (std::bad_alloc)
		{
			cout <<i<<"  "<<s<<" "<<ext<< endl;
		}*/
		//cout << "daozhele" << endl;
		type = operate[i];
		if (operate[i] == "Conv1*1")
		{
			inp_c = input_channel[i];
			out_c = output_channel[i]/8;
			/*img_h = (img_height[i+1] + core - 1) / core;
			img_w = img_width[i+1];
			img_h1 = (img_height[i+1] + core - 1) / core;
			img_w1 = img_width[i+1];*/
			img_h = (img_height[reset_i] + core - 1) / core;
			img_w = img_width[reset_i];
			img_h1 = (img_height[reset_i] + core - 1) / core;
			img_w1 = img_width[reset_i];
			strid = stride[i];
		}
		else if (operate[i] == "Conv3*3")
		{
			strid = stride[i];
			if (strid == 1)
			{
				inp_c = input_channel[i];
				out_c = output_channel[i]/8;
				/*img_h = (img_height[i + 1] + core - 1) / core;
				img_w = img_width[i + 1];
				img_h1 = (img_height[i + 1] + core - 1) / core+2;
				img_w1 = img_width[i + 1]+2;*/
				img_h = (img_height[reset_i] + core - 1) / core;
				img_w = img_width[reset_i];
				img_h1 = (img_height[reset_i] + core - 1) / core + 2;
				img_w1 = img_width[reset_i] + 2;
			}
			else//stride=2
			{
				inp_c = input_channel[i];
				out_c = output_channel[i]/8;
				/*img_h = (img_height[i + 1] + core - 1) / core;
				img_w = img_width[i + 1];
				img_h1 = (img_height[i + 1] + core - 1) / core*2+2;
				img_w1 = img_width[i + 1] *2+2;*/
				img_h = (img_height[reset_i] + core - 1) / core;
				img_w = img_width[reset_i];
				img_h1 = (img_height[reset_i] + core - 1) / core * 2 + 2;
				img_w1 = img_width[reset_i] * 2 + 2;
			}
		}
		else if (operate[i] == "DwConv3*3" )
		{
			strid = stride[i];
			if (strid == 1)
			{
				//inp_c = input_channel[i];
				out_c = output_channel[i] / 8;
				inp_c = out_c;
				/*img_h = (img_height[i + 1] + core - 1) / core;
				img_w = img_width[i + 1];
				img_h1 = (img_height[i + 1] + core - 1) / core + 2;
				img_w1 = img_width[i + 1] + 2;*/
				img_h = (img_height[reset_i] + core - 1) / core;
				img_w = img_width[reset_i];
				img_h1 = (img_height[reset_i] + core - 1) / core + 2;
				img_w1 = img_width[reset_i] + 2;
			}
			else//stride=2
			{
				//inp_c = input_channel[i];
				out_c = output_channel[i] / 8;
				inp_c = out_c;
				/*img_h = (img_height[i + 1] + core - 1) / core;
				img_w = img_width[i + 1];
				img_h1 = (img_height[i + 1] + core - 1) / core * 2 + 2;
				img_w1 = img_width[i + 1] * 2 + 2;*/
				img_h = (img_height[reset_i] + core - 1) / core;
				img_w = img_width[reset_i];
				img_h1 = (img_height[reset_i] + core - 1) / core * 2 + 2;
				img_w1 = img_width[reset_i] * 2 + 2;
			}
		}
		string s = operate[i] + "+" + to_string(inp_c) + "+" + to_string(out_c) +
			"+" + to_string(img_h) + "+" + to_string(img_w) +
			"+" + to_string(img_h1) + "+" + to_string(img_w1) + "+" + to_string(strid);
		//cout <<i<< s << endl;
		if (cost_comvalue.find(s) == cost_comvalue.end())
		{
			costcom = cost_comp(type, inp_c, out_c, img_h, img_w,img_h1,img_w1,strid);
			cost_comvalue[s] = costcom;
		}
		else
		{
			costcom = cost_comvalue[s];
		}

		//time_t time1 = time(nullptr);
		//for(int ii=0;ii<100;ii++)
		//costcom=cost_comp(type, inp_c, out_c, img_h, img_w, strid);
		//time_t time2 = time(nullptr);

		//double time_diff = difftime(time2, time1);
		//cout << "time1: " << time1 << endl;
		//cout << "time2: " << time2 << endl;
		//cout << "time_diff: " << time_diff << "s" << endl;
	}
	else if (oper == 'b')
	{
		type = operate[i];
		if (operate[i] == "Conv1*1")
		{
			inp_c = input_channel[i];
			out_c = output_channel[i]/8;
			img_h = img_height[i + 1];
			img_w = (img_width[i + 1] + core - 1) / core;
			img_h1 = img_height[i + 1];
			img_w1 = (img_width[i + 1] + core - 1) / core;
			strid = stride[i];
		}
		else if ( operate[i] == "Conv3*3")
		{
			strid = stride[i];
			if (strid == 1)
			{
				inp_c = input_channel[i];
				out_c = output_channel[i]/8;
				img_h = img_height[i + 1];
				img_w = (img_width[i + 1] + core - 1) / core;
				img_h1 = img_height[i + 1]+2;
				img_w1 = (img_width[i + 1] + core - 1) / core+2;
			}
			else//stride=2
			{
				inp_c = input_channel[i];
				out_c = output_channel[i]/8;
				img_h = img_height[i + 1];
				img_w = (img_width[i + 1] + core - 1) / core;
				img_h1 = img_height[i + 1]*2+2;
				img_w1 = (img_width[i + 1] + core - 1) / core*2+2;
			}
		}
		else if (operate[i] == "DwConv3*3" )
		{
			strid = stride[i];
			if (strid == 1)
			{
				//inp_c = input_channel[i];
				out_c = output_channel[i] / 8;
				inp_c = out_c;
				img_h = img_height[i + 1];
				img_w = (img_width[i + 1] + core - 1) / core;
				img_h1 = img_height[i + 1] + 2;
				img_w1 = (img_width[i + 1] + core - 1) / core + 2;
			}
			else//stride=2
			{
				//inp_c = input_channel[i];
				out_c = output_channel[i] / 8;
				inp_c = out_c;
				img_h = img_height[i + 1];
				img_w = (img_width[i + 1] + core - 1) / core;
				img_h1 = img_height[i + 1] * 2 + 2;
				img_w1 = (img_width[i + 1] + core - 1) / core * 2 + 2;
			}
		}
		string s = operate[i] + "+" + to_string(inp_c) + "+" + to_string(out_c) +
			"+" + to_string(img_h) + "+" + to_string(img_w) +
			"+" + to_string(img_h1) + "+" + to_string(img_w1) + "+" + to_string(strid);
		if (cost_comvalue.find(s) == cost_comvalue.end())
		{
			costcom = cost_comp(type, inp_c, out_c, img_h, img_w, img_h1, img_w1, strid);
			cost_comvalue[s] = costcom;
		}
		else
		{
			costcom = cost_comvalue[s];
		}
		//costcom=cost_comp(type, inp_c, out_c, img_h, img_w, strid);
		//cout <<type<<" " << inp_c <<" "<< out_c<<" "<< img_h <<" " <<img_w <<" "<< strid<<" " << "woshicost " << costcom << endl;
	}
	else if (oper == 'c')
	{
		type = operate[i];
		if (operate[i] == "Conv1*1")
		{
			inp_c = input_channel[i];
			out_c = output_channel[i]/8;
			img_h = (img_height[i + 1]+1)/2;
			img_w = (img_width[i + 1] +1)/2;
			img_h1 = (img_height[i + 1] + 1) / 2;
			img_w1 = (img_width[i + 1] + 1) / 2;
			strid = stride[i];
		}
		else if (operate[i] == "Conv3*3")
		{
			strid = stride[i];
			if (strid == 1)
			{
				inp_c = input_channel[i];
				out_c = output_channel[i]/8;
				img_h = (img_height[i + 1] + 1) / 2;
				img_w = (img_width[i + 1] + 1) / 2;
				img_h1 = (img_height[i + 1] + 1) / 2+2;
				img_w1 = (img_width[i + 1] + 1) / 2+2;
			}
			else//stride=2
			{
				inp_c = input_channel[i];
				out_c = output_channel[i]/8;
				img_h = (img_height[i + 1] + 1) / 2;
				img_w = (img_width[i + 1] + 1) / 2;
				img_h1 = (img_height[i + 1] + 1) / 2*2+2;
				img_w1 = (img_width[i + 1] + 1) / 2*2+2;
			}
		}
		else if (operate[i] == "DwConv3*3" )
		{
			strid = stride[i];
			if (strid == 1)
			{
				//inp_c = input_channel[i];
				out_c = output_channel[i] / 8;
				inp_c = out_c;
				img_h = (img_height[i + 1] + 1) / 2;
				img_w = (img_width[i + 1] + 1) / 2;
				img_h1 = (img_height[i + 1] + 1) / 2 + 2;
				img_w1 = (img_width[i + 1] + 1) / 2 + 2;
			}
			else//stride=2
			{
				//inp_c = input_channel[i];
				out_c = output_channel[i] / 8;
				inp_c = out_c;
				img_h = (img_height[i + 1] + 1) / 2;
				img_w = (img_width[i + 1] + 1) / 2;
				img_h1 = (img_height[i + 1] + 1) / 2 * 2 + 2;
				img_w1 = (img_width[i + 1] + 1) / 2 * 2 + 2;
			}
		}
	//	cout << img_w << "kankandkd" << endl;
	//	strid = stride[i];
		string s = operate[i] + "+" + to_string(inp_c) + "+" + to_string(out_c) +
			"+" + to_string(img_h) + "+" + to_string(img_w) +
			"+" + to_string(img_h1) + "+" + to_string(img_w1) + "+" + to_string(strid);
		if (cost_comvalue.find(s) == cost_comvalue.end())
		{
			costcom = cost_comp(type, inp_c, out_c, img_h, img_w, img_h1, img_w1, strid);
			cost_comvalue[s] = costcom;
		}
		else
		{
			costcom = cost_comvalue[s];
		}
	}
	else if (oper == 'd')
	{
		type = operate[i];
		if (operate[i] == "Conv1*1")
		{
			inp_c = input_channel[i];
			out_c = output_channel[i]/4/8;
			img_h = img_height[i+1];//i��i+1�ߴ�һ��������i��i+1����
			img_w = img_width[i + 1];
			img_h1 = img_height[i + 1];
			img_w1 = img_width[i + 1];
			strid = stride[i];
		}
		else if ( operate[i] == "Conv3*3")
		{
			strid = stride[i];
			if (strid == 1)
			{
				inp_c = input_channel[i];
				out_c = output_channel[i]/4/8;
				img_h = img_height[i + 1];
				img_w = img_width[i + 1];
				img_h1 = img_height[i + 1]+2;
				img_w1 = img_width[i + 1]+2;
			}
			else//stride=2
			{
				inp_c = input_channel[i];
				out_c = output_channel[i]/4/8;
				img_h = img_height[i + 1];
				img_w = img_width[i + 1];
				img_h1 = img_height[i + 1]*2+2;
				img_w1 = img_width[i + 1]*2+2;
			}
		}
		else if (operate[i] == "DwConv3*3" )
		{
			strid = stride[i];
			if (strid == 1)
			{
				//inp_c = input_channel[i];
				out_c = output_channel[i] / 4 / 8;
				inp_c = out_c;
				img_h = img_height[i + 1];
				img_w = img_width[i + 1];
				img_h1 = img_height[i + 1] + 2;
				img_w1 = img_width[i + 1] + 2;
			}
			else//stride=2
			{
				//inp_c = input_channel[i];
				out_c = output_channel[i] / 4 / 8;
				inp_c = out_c;
				img_h = img_height[i + 1];
				img_w = img_width[i + 1];
				img_h1 = img_height[i + 1] * 2 + 2;
				img_w1 = img_width[i + 1] * 2 + 2;
			}
		}
		string s = operate[i] + "+" + to_string(inp_c) + "+" + to_string(out_c) +
			"+" + to_string(img_h) + "+" + to_string(img_w) +
			"+" + to_string(img_h1) + "+" + to_string(img_w1) + "+" + to_string(strid);
		if (cost_comvalue.find(s) == cost_comvalue.end())
		{
			costcom = cost_comp(type, inp_c, out_c, img_h, img_w, img_h1, img_w1, strid);
			cost_comvalue[s] = costcom;
		}
		else
		{
			costcom = cost_comvalue[s];
		}
	}
	else if (oper == 'e')
	{
		costcom = 1000;
		/*type = operate[i];
		inp_c = input_channel[i];
		out_c = output_channel[i];
		img_h = img_height[i];
		img_w = img_width[i];
		strid = stride[i];
		string s = operate[i] + "+" + to_string(inp_c) + "+" + to_string(inp_c) +
			"+" + to_string(inp_c) + "+" + to_string(inp_c) +
			"+" + to_string(inp_c) + "+" + to_string(inp_c);
		if (cost_comvalue.find(s) == cost_comvalue.end())
		{
			costcom = cost_comp(type, inp_c, out_c, img_h, img_w, strid);
			cost_comvalue[s] = costcom;
		}
		else
		{
			costcom = cost_comvalue[s];
		}*/
	}
	else if (oper == '1')
	{
		//costcom = evaluate_comp_extra(i,l, s, ext + 1);
		costcom = evaluate_comp_extra(i, l, s, ext);
	}
	else if (oper == '2')
	{
		//costcom = evaluate_comp_extra(i,l, s, ext + 1);
		costcom = evaluate_comp_extra(i, l, s, ext);
	}
	else if (oper == '3')
	{
		//costcom = evaluate_comp_extra(i,l, s, ext + 1);
		costcom = evaluate_comp_extra(i, l, s, ext);
	}
	else if (oper == '4')
	{
		//costcom = evaluate_comp_extra(i,l, s, ext + 1);
		costcom = evaluate_comp_extra(i, l, s, ext);
	}
	else if (oper == '5')
	{
		//costcom = evaluate_comp_extra(i,l, s, ext + 1);
		costcom = evaluate_comp_extra(i, l, s, ext);
	}
	return costcom;

}


//�������������������  ���������Ǳ�ʾ���������ٲ���� 
double evaluate_comp_extra(int i,int l, string s, int j)
{
	double costcom=0.0;
	int ext = compute_layer_extra(i, l, s);
	int extsize = tran_layer_extra(i, l, s);
	//cout << i << " " << ext << "  " << extsize << endl;
	if (ext == -1 || extsize == -1)
	{
		costcom = 1000;
		return costcom;
	}
	int n = s.length();
	int k = i - (nn_number - n);//������i�㣬��Ӧ������nnnumber-1-i�㣬Ϊj
	char oper = s[k];//���ַ���s��ȡ��j���ַ�����Ӧ��i��Ļ��ֲ���
	//���±���j�ݣ���������s��������Ҫ���������ٷ�
	if (oper == '1')
	{
		//cout << "sdsdsdssddsddss" << endl;
		type = operate[i];
		if (operate[i] == "Conv1*1")
		{
			inp_c = input_channel[i];
			out_c = output_channel[i] / 8;
			img_h = (img_height[i+1] + core - 1) / core + ext;
			img_w = img_width[i + 1];
			img_h1 = (img_height[i] + core - 1) / core + extsize;//�����Ӧ����i����extsize��Ӧ
			img_w1 = img_width[i + 1];//+2 *2+1
			strid = stride[i];
		}
		else if (operate[i] == "Conv3*3")
		{
			strid = stride[i];
			if (strid == 1)
			{
				inp_c = input_channel[i];
				out_c = output_channel[i] / 8;
				img_h = (img_height[i+1] + core - 1) / core + ext;
				img_w = img_width[i + 1];
				img_h1 = (img_height[i] + core - 1) / core + extsize;
				img_w1 = img_width[i + 1]+2;//+2 *2+1
			}
			else//stride=2
			{
				inp_c = input_channel[i];
				out_c = output_channel[i] / 8;
				img_h = (img_height[i+1] + core - 1) / core + ext;
				img_w = img_width[i + 1];
				img_h1 = (img_height[i] + core - 1) / core + extsize;
				img_w1 = img_width[i + 1]*2+2;//+2 *2+1
			}
		}
		else if (operate[i] == "DwConv3*3")
		{
			strid = stride[i];
			if (strid == 1)
			{
				//inp_c = input_channel[i];
				out_c = output_channel[i] / 8;
				inp_c = out_c;
				img_h = (img_height[i + 1] + core - 1) / core + ext;
				img_w = img_width[i + 1];
				img_h1 = (img_height[i] + core - 1) / core + extsize;
				img_w1 = img_width[i + 1] + 2;//+2 *2+1
			}
			else//stride=2
			{
				//inp_c = input_channel[i];
				out_c = output_channel[i] / 8;
				inp_c = out_c;
				img_h = (img_height[i + 1] + core - 1) / core + ext;
				img_w = img_width[i + 1];
				img_h1 = (img_height[i] + core - 1) / core + extsize;
				img_w1 = img_width[i + 1] * 2 + 2;//+2 *2+1
			}
		}
		string s = operate[i] + "+" + to_string(inp_c) + "+" + to_string(out_c) +
			"+" + to_string(img_h) + "+" + to_string(img_w) +
			"+" + to_string(img_h1) + "+" + to_string(img_w1) + "+" + to_string(strid);
		//cout << i << s << endl;
		if (cost_comvalue.find(s) == cost_comvalue.end())
		{
			costcom = cost_comp(type, inp_c, out_c, img_h, img_w, img_h1, img_w1, strid);
			cost_comvalue[s] = costcom;
		}
		else
		{
			costcom = cost_comvalue[s];
		}
	}
	else if (oper == '2')
	{
		type = operate[i];
		if (operate[i] == "Conv1*1")
		{
			inp_c = input_channel[i];
			out_c = output_channel[i] / 8;
			img_h = img_height[i + 1];
			img_w = (img_width[i + 1] + core - 1) / core + ext;
			img_h1 = img_height[i + 1];//+2 *2+1
			img_w1 = (img_width[i] + core - 1) / core + extsize;
			strid = stride[i];
		}
		else if (operate[i] == "Conv3*3")
		{
			strid = stride[i];
			if (strid == 1)
			{
				inp_c = input_channel[i];
				out_c = output_channel[i] / 8;
				img_h = img_height[i + 1];
				img_w = (img_width[i + 1] + core - 1) / core + ext;
				img_h1 = img_height[i + 1]+2;//+2 *2+1
				img_w1 = (img_width[i] + core - 1) / core + extsize;
			}
			else//stride=2
			{
				inp_c = input_channel[i];
				out_c = output_channel[i] / 8;
				img_h = img_height[i + 1];
				img_w = (img_width[i + 1] + core - 1) / core + ext;
				img_h1 = img_height[i + 1]*2+2;//+2 *2+1
				img_w1 = (img_width[i] + core - 1) / core + extsize;
			}
		}
		else if (operate[i] == "DwConv3*3")
		{
			strid = stride[i];
			if (strid == 1)
			{
				//inp_c = input_channel[i];
				out_c = output_channel[i] / 8;
				inp_c = out_c;
				img_h = img_height[i + 1];
				img_w = (img_width[i + 1] + core - 1) / core + ext;
				img_h1 = img_height[i + 1] + 2;//+2 *2+1
				img_w1 = (img_width[i] + core - 1) / core + extsize;
			}
			else//stride=2
			{
				//inp_c = input_channel[i];
				out_c = output_channel[i] / 8;
				inp_c = out_c;
				img_h = img_height[i + 1];
				img_w = (img_width[i + 1] + core - 1) / core + ext;
				img_h1 = img_height[i + 1] * 2 + 2;//+2 *2+1
				img_w1 = (img_width[i] + core - 1) / core + extsize;
			}
		}
		string s = operate[i] + "+" + to_string(inp_c) + "+" + to_string(out_c) +
			"+" + to_string(img_h) + "+" + to_string(img_w) +
			"+" + to_string(img_h1) + "+" + to_string(img_w1) + "+" + to_string(strid);
		if (cost_comvalue.find(s) == cost_comvalue.end())
		{
			costcom = cost_comp(type, inp_c, out_c, img_h, img_w, img_h1, img_w1, strid);
			cost_comvalue[s] = costcom;
		}
		else
		{
			costcom = cost_comvalue[s];
		}
		//costcom=cost_comp(type, inp_c, out_c, img_h, img_w, strid);
		//cout <<type<<" " << inp_c <<" "<< out_c<<" "<< img_h <<" " <<img_w <<" "<< strid<<" " << "woshicost " << costcom << endl;
	}
	else if (oper == '3')
	{
		type = operate[i];
		if (operate[i] == "Conv1*1")
		{
			inp_c = input_channel[i];
			out_c = output_channel[i] / 8;
			img_h = (img_height[i + 1] + 1) / 2 + ext;
			img_w = (img_width[i + 1] + 1) / 2 + ext;
			img_h1 = (img_height[i] + 1) / 2 + extsize;
			img_w1 = (img_width[i] + 1) / 2 + extsize;
			strid = stride[i];
		}
		else if ( operate[i] == "Conv3*3")
		{
			strid = stride[i];
			if (strid == 1)
			{
				inp_c = input_channel[i];
				out_c = output_channel[i] / 8;
				img_h = (img_height[i + 1] + 1) / 2 + ext;
				img_w = (img_width[i + 1] + 1) / 2 + ext;
				img_h1 = (img_height[i] + 1) / 2 + extsize;
				img_w1 = (img_width[i] + 1) / 2 + extsize;
			}
			else//stride=2
			{
				inp_c = input_channel[i];
				out_c = output_channel[i] / 8;
				img_h = (img_height[i + 1] + 1) / 2 + ext;
				img_w = (img_width[i + 1] + 1) / 2 + ext;
				img_h1 = (img_height[i] + 1) / 2 + extsize;
				img_w1 = (img_width[i] + 1) / 2 + extsize;
			}
		}
		else if (operate[i] == "DwConv3*3")
		{
			strid = stride[i];
			if (strid == 1)
			{
				//inp_c = input_channel[i];
				out_c = output_channel[i] / 8;
				inp_c = out_c;
				img_h = (img_height[i + 1] + 1) / 2 + ext;
				img_w = (img_width[i + 1] + 1) / 2 + ext;
				img_h1 = (img_height[i] + 1) / 2 + extsize;
				img_w1 = (img_width[i] + 1) / 2 + extsize;
			}
			else//stride=2
			{
				//inp_c = input_channel[i];
				out_c = output_channel[i] / 8;
				inp_c = out_c;
				img_h = (img_height[i + 1] + 1) / 2 + ext;
				img_w = (img_width[i + 1] + 1) / 2 + ext;
				img_h1 = (img_height[i] + 1) / 2 + extsize;
				img_w1 = (img_width[i] + 1) / 2 + extsize;
			}
		}
		//	cout << img_w << "kankandkd" << endl;
		//	strid = stride[i];
		string s = operate[i] + "+" + to_string(inp_c) + "+" + to_string(out_c) +
			"+" + to_string(img_h) + "+" + to_string(img_w) +
			"+" + to_string(img_h1) + "+" + to_string(img_w1) + "+" + to_string(strid);
		if (cost_comvalue.find(s) == cost_comvalue.end())
		{
			costcom = cost_comp(type, inp_c, out_c, img_h, img_w, img_h1, img_w1, strid);
			cost_comvalue[s] = costcom;
		}
		else
		{
			costcom = cost_comvalue[s];
		}
	}
	else if (oper == '4')
	{
		type = operate[i];
		if (operate[i] == "Conv1*1")
		{
			inp_c = input_channel[i];
			out_c = output_channel[i] / 4 / 8;
			img_h = img_height[i + 1];//i��i+1�ߴ�һ��������i��i+1����
			img_w = img_width[i + 1];
			img_h1 = img_height[i + 1];
			img_w1 = img_width[i + 1];
			strid = stride[i];
		}
		else if ( operate[i] == "Conv3*3")
		{
			strid = stride[i];
			if (strid == 1)
			{
				inp_c = input_channel[i];
				out_c = output_channel[i] / 4 / 8;
				img_h = img_height[i + 1];
				img_w = img_width[i + 1];
				img_h1 = img_height[i + 1] + 2;
				img_w1 = img_width[i + 1] + 2;
			}
			else//stride=2
			{
				inp_c = input_channel[i];
				out_c = output_channel[i] / 4 / 8;
				img_h = img_height[i + 1];
				img_w = img_width[i + 1];
				img_h1 = img_height[i + 1] * 2 + 2;
				img_w1 = img_width[i + 1] * 2 + 2;
			}
		}
		else if (operate[i] == "DwConv3*3")
		{
			strid = stride[i];
			if (strid == 1)
			{
				//inp_c = input_channel[i];
				out_c = output_channel[i] / 4 / 8;
				inp_c = out_c;
				img_h = img_height[i + 1];
				img_w = img_width[i + 1];
				img_h1 = img_height[i + 1] + 2;
				img_w1 = img_width[i + 1] + 2;
			}
			else//stride=2
			{
				//inp_c = input_channel[i];
				out_c = output_channel[i] / 4 / 8;
				inp_c = out_c;
				img_h = img_height[i + 1];
				img_w = img_width[i + 1];
				img_h1 = img_height[i + 1] * 2 + 2;
				img_w1 = img_width[i + 1] * 2 + 2;
			}
		}
		string s = operate[i] + "+" + to_string(inp_c) + "+" + to_string(out_c) +
			"+" + to_string(img_h) + "+" + to_string(img_w) +
			"+" + to_string(img_h1) + "+" + to_string(img_w1) + "+" + to_string(strid);
		if (cost_comvalue.find(s) == cost_comvalue.end())
		{
			costcom = cost_comp(type, inp_c, out_c, img_h, img_w, img_h1, img_w1, strid);
			cost_comvalue[s] = costcom;
		}
		else
		{
			costcom = cost_comvalue[s];
		}
	}
	else if (oper == '5')
	{
		costcom = 1000;
		/*type = operate[i];
		inp_c = input_channel[i];
		out_c = output_channel[i];
		img_h = img_height[i];
		img_w = img_width[i];
		strid = stride[i];
		string s = operate[i] + "+" + to_string(inp_c) + "+" + to_string(inp_c) +
			"+" + to_string(inp_c) + "+" + to_string(inp_c) +
			"+" + to_string(inp_c) + "+" + to_string(inp_c);
		if (cost_comvalue.find(s) == cost_comvalue.end())
		{
			costcom = cost_comp(type, inp_c, out_c, img_h, img_w, strid);
			cost_comvalue[s] = costcom;
		}
		else
		{
			costcom = cost_comvalue[s];
		}*/
	}

	return costcom;

}
//ע������������Ķ����õĻ��ַ�����ͬ������ͬ��������Ϊ��ֵ����Ϊ������
//�������û�ȡ��ǰ����  ���ݸ��������н������� 
//(0��ʼ��������i�㣬������nn_number-1-i��==�ַ���s�еĵ�n-1��  ����nn_number-i=n)




double evaluate_seq(string s, int l, int k)
{

	double evaluate_seq1 = 0;
	evaluate_seq1 += dp_nn[l][k];//��l�������䣬�������������ֱ�Ӽ���
	
	//���� 
	int n = s.length();//��ǰ���г���
	int m = l - nn_number + n;//��l��������s���ǵ�l-nn_number+n��;
	//��ʼ��m�������䣬��0��������
	//�����м䲻���Ķ�����㣬ÿ��ǰһ�㣬�����һ����� 
	int ext = 0;//ͳ���������ٲ�û��
	for (int i = m - 1;i > 0;i--)
	{
		//���ĸ�������Ӧ������˼������--��ʱ�ò���δʹ��
		ext += 1;
		evaluate_seq1 += evaluate_comp_extra(nn_number - n + i, l, s, ext);//��i���Ӧ�ܳ���i+nn_number-n
	}

	//���㿪ʼ�������ǰһ�㣨��n-1�㼴��nn_number-1-nn+1�㣩�Ĵ����������� ���������⴫��
	if (nn_number == n && s[0] >= '1' && s[0] <= '5')//����ǰ��Ϊ��0�㣬�ҵ�0�����󲻴���
	{
		evaluate_seq1 += 0;//��ʼ��Ҳ������Ļ�����ʱ���贫��
		ext += 1;//��ǰ��һ�㲻���䣬�������--��Ҫ�ٶ���һ��
		//pingbi1:cout<<"            asfffffffffffffffffffffffffffffffff                         " << s << endl;
	}
	else
	{
		if (ext == 0)//û�в�����ģ��������䵥reset����
		{
			evaluate_seq1 += evaluate_tran_single(nn_number - n, s);//�����ڲ�����
			evaluate_seq1 += evaluate_reset_single(nn_number - n, s) / 1000.0;
		}
		else//���⴫�����
		{
			evaluate_seq1 += evaluate_tran_extra(nn_number - n, l, s);
			evaluate_seq1 += evaluate_reset_extra(nn_number - n, l, s) / 1000.0;
		}
	}

	//������ǰ��һ�㣬Ҳ���ǵ�ǰ������Ҫ�ļ���   
	//��һ���Ǽ������ģ������dp_nn_no������Ҫ��дһ������ʵ�ָ���䣬��Ϊ�ײ㲻����  
	//�����ˣ������������������ڲ��Լ��������������  ----�������Բ�ִ���������Щ�ң�����ĺ������ÿ��Է���if������
	evaluate_seq1 += evaluate_single(nn_number - n, l, s, ext);//��ǰ��һ�㣬���ڵ�nn-n�� ����0��ʼ��
	
	return evaluate_seq1;

}

void evaluate_seq_test(string s)
{
	double ssum = 0;
	int n = s.length();
	double evaluate_seq = 0;
	double evaluate_seq1 = 0;
	double evaluate_seq2 = 0;
	double evaluate_seq3 = 0;
	int l = nn_number-1;
	int ii;
	//�������һ������ʱ
	evaluate_seq=evaluate_single(nn_number - 1, l, s, 0);
	//cout << evaluate_seq << endl;
	ssum += evaluate_seq;
	for (int i = n-2;i >= 0;i--)//n-1���һ�㵥������
	{
		evaluate_seq = 0;
		evaluate_seq1 = 0;
		evaluate_seq2 = 0;
		evaluate_seq3 = 0;
		ii = nn_number - n + i;
		if (s[i] >= 'a' && s[i] <= 'e')
		{
			if (ii == l - 1)
			{
				//��ǰ�����+�����Ĵ��������
				evaluate_seq1 = evaluate_single(ii, l, s, 0);
				evaluate_seq2 = evaluate_tran_single(ii,  s);
				evaluate_seq3 = evaluate_reset_single(ii, s)/1000.0;
				evaluate_seq = evaluate_seq1 + evaluate_seq2 + evaluate_seq3;
			}
			else
			{
				//��ǰ�����+��������ļ�����������
				evaluate_seq1= evaluate_single(ii, l, s, 0);
				evaluate_seq2= evaluate_tran_extra (ii, l, s);
				evaluate_seq3= evaluate_reset_extra(ii,l, s) / 1000.0;
				evaluate_seq = evaluate_seq1 + evaluate_seq2 + evaluate_seq3;
			}
			l = ii;//��ǰ�㴫�䣬lǰ������ǰ��
		}
		else
		{
			//l����
			//��ǰ�������㣬�޴��������
			evaluate_seq1 = evaluate_comp_extra(ii, l, s, 0);
			evaluate_seq = evaluate_seq1;
		}
		cout << ii << "�㣺 "<<s[i]<<"-���㣺  " << evaluate_seq1<<"-���䣺  "
			<< evaluate_seq2<<"-reset:  "<< evaluate_seq3<<"-sum:  "<<evaluate_seq<< endl;
		ssum += evaluate_seq;

	}
	cout << ssum << endl;
}


//�������� ---�ֱ���ƺ�����������evalute_aa_single
double evaluate_tran_single(int i, string s)
{
	int n = s.length();
	//ȡ�¼���ͷ���ַ���ȡ��һ���ַ�
	char now = s[0];
	char next = s[1];
	//�ַ�ת��--��ֹ����ʶ������
	if (now >= '1' && now <= '5')
	{
		now = now - '1' + 'a';
	}
	if (next >= '1' && next <= '5')
	{
		next = next - '1' + 'a';
	}
	vector<double> tran_num;
	int tran_count;
	int count = 0;
	double tran_time=0.0;
	int pianyi = 2;
	if (i >= 25)
	{
		pianyi = 1;//�����ֹi+2>nn_number-1  ԭ����i+1+1��Ϊi+pianyi
	}
	if (now == 'a' && next == 'a')
	{
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			//�����������1��ע��������ܻ�i+2>nn_number-1
			if (img_height[i + pianyi] % 4 == 0)//�ܱ�4��������i+1�������ߴ�=��i+1+1�������ߴ� 
			{
				tran_count = img_width[i + 1] * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				/*tran_count = img_width[i + 1] * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;*/
				//tran_time += cost_tran(bandwidth, tran_count);
				//tran_time += cost_tran(bandwidth, tran_count);
				//cout << "sssdsfsfsfssffs          " << tran_time << endl;
			}//ȷ������ͬʱ��һ�������һ������ʱ�����  ��ʱ���������ݴ���
			else
			{
				tran_count = img_width[i + 1] * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				tran_count = img_width[i + 1] * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = img_width[i + 1] * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			tran_count = img_width[i + 1] * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			//tran_time += cost_tran(bandwidth, tran_count);//������������
		}
		else if (operate[i+1] == "Conv1*1")//strideΪ1��2��ͬ
		{
			tran_count = 0;
			tran_time += 0;//ֵΪ0���ж� ����Ҫ���䣨�ô���������ֱ�ӷ���һ���յģ����ô���
		}


		//���ڲ��еĴ���
	}
	else if (now == 'a' && next == 'b')
	{
		//�н���
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_width[i + pianyi] % 4 == 0)//�ܱ�4��������i+1�������ߴ�=��i+1+1�������ߴ� 
			{
				tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 1) / 2 + 1) * input_channel[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 3) / 4 + 1) * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 3) / 4 + 1) * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
			}//ȷ������ͬʱ��һ�������һ������ʱ�����  ��ʱ���������ݴ���
			else
			{
				tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 1) / 2 + 2) * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 3) / 4 + 2) * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 3) / 4 + 2) * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 1) / 2 + 2) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 3) / 4 + 2) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 3) / 4 + 2) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);//�ܹ�������������
		}
		else if (operate[i+1] == "Conv1*1")//strideΪ1��2��ͬ
		{
			tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 1) / 2) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			//cout << "sssdsfsfsfssffs          " << tran_time << endl;
		}
	}
	else if (now == 'a' && next == 'c')
	{
		//��+ʮ��
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 1) / 2 + 1) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = ((img_width[i + 1] + 1) / 2 + 1) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 1) / 2 + 1) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = ((img_width[i + 1] + 1) / 2 + 1) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);//�ܹ�����2������
		}
		else if (operate[i+1] == "Conv1*1")//strideΪ1��2��ͬ
		{
			tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 1) / 2) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_width[i + 1] + 1) / 2 * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
	}
	else if (now == 'a' && next == 'd')
	{
		//��+ͨ��
		if (operate[i + 1] == "DwConv3*3")
		{
			tran_count = (img_height[i + 1] + 3) / 4 * img_width[i + 1] * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_height[i + 1] + 3) / 4 * img_width[i + 1] * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_height[i + 1] + 3) / 4 * img_width[i + 1] * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
		else
		{
			tran_count = (img_height[i + 1] + 3) / 4 * img_width[i + 1] * input_channel[i + 1] / 2;
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_height[i + 1] + 3) / 4 * img_width[i + 1] * input_channel[i + 1] / 4;
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_height[i + 1] + 3) / 4 * img_width[i + 1] * input_channel[i + 1] / 4;
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
	}
	else if (now == 'a' && next == 'e')
	{
		//��+����
		count=1000;
		tran_time += 1000;
	}
	else if (now == 'b' && next == 'a')
	{
		//��+��
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_height[i + pianyi] % 4 == 0)//�ܱ�4��������i+1�������ߴ�=��i+1+1�������ߴ� 
			{
				tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 1) / 2 + 1) * input_channel[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 3) / 4 + 1) * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 3) / 4 + 1) * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
			}//ȷ������ͬʱ��һ�������һ������ʱ�����  ��ʱ���������ݴ���
			else
			{
				tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 1) / 2 + 2) * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 3) / 4 + 2) * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 3) / 4 + 2) * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 1) / 2 + 2) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 3) / 4 + 2) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 3) / 4 + 2) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);//�ܹ�������������
		}
		else if (operate[i+1] == "Conv1*1")//strideΪ1��2��ͬ
		{
			tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 1) / 2) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 3) / 4) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 3) / 4) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
	}
	else if (now == 'b' && next == 'b')
	{
		//���ڲ��еĴ���
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_width[i + pianyi] % 4 == 0)//�ܱ�4��������i+1�������ߴ�=��i+1+1�������ߴ� 
			{
				tran_count = img_height[i + 1] * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				/*tran_count = img_height[i + 1] * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;*/
				//tran_time += cost_tran(bandwidth, tran_count);
			}//ȷ������ͬʱ��һ�������һ������ʱ�����  ��ʱ���������ݴ���
			else
			{
				tran_count = img_height[i + 1] * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				tran_count = img_height[i + 1] * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = img_height[i + 1] * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);//������������
		}
		else if (operate[i+1] == "Conv1*1")//strideΪ1��2��ͬ
		{
			tran_count = 0;
			tran_time += 0;
		}
	}
	else if (now == 'b' && next == 'c')
	{
		//��+ʮ��
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 1) / 2) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 1) / 2) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 1) / 2) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = ((img_width[i + 1] + 1) / 2 + 1) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 1) / 2) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 1) / 2) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 1) / 2) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = ((img_width[i + 1] + 1) / 2 + 1) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = ((img_width[i + 1] + 1) / 2 + 1) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
		else if (operate[i+1] == "Conv1*1")//strideΪ1��2��ͬ
		{
			tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 1) / 2) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 1) / 2) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 1) / 2) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
	}
	else if (now == 'b' && next == 'd')
	{
		//��+ͨ��
		if (operate[i + 1] == "DwConv3*3")
		{
			tran_count = (img_width[i + 1] + 3) / 4 * img_height[i + 1] * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_width[i + 1] + 3) / 4 * img_height[i + 1] * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_width[i + 1] + 3) / 4 * img_height[i + 1] * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
		else
		{
			tran_count = (img_width[i + 1] + 3) / 4 * img_height[i + 1] * input_channel[i + 1] / 2;
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_width[i + 1] + 3) / 4 * img_height[i + 1] * input_channel[i + 1] / 4;
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_width[i + 1] + 3) / 4 * img_height[i + 1] * input_channel[i + 1] / 4;
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
	}
	else if (now == 'b' && next == 'e')
	{
		//���ڲ��еĴ���
		count=1000;
		tran_time += 1000;
	}
	else if (now == 'c' && next == 'a')
	{
		//ʮ��+��
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_width[i + pianyi] % 4 == 0)//�ܱ�4��������i+1�������ߴ�=��i+1+1�������ߴ� 
			{
				tran_count = ((img_height[i + 1] + 3) / 4 + 1) * ((img_width[i + 1] + 1) / 2) * input_channel[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = img_width[i + 1] * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);

			}//ȷ������ͬʱ��һ�������һ������ʱ�����  ��ʱ���������ݴ���
			else
			{
				tran_count = ((img_height[i + 1] + 3) / 4 + 1) * ((img_width[i + 1] + 1) / 2) * input_channel[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = img_width[i + 1] * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = ((img_height[i + 1] + 3) / 4 + 1) * ((img_width[i + 1] + 1) / 2) * input_channel[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = img_width[i + 1] * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
		else if (operate[i+1] == "Conv1*1")//strideΪ1��2��ͬ
		{
			tran_count = ((img_height[i + 1] + 3) / 4) * ((img_width[i + 1] + 1) / 2) * input_channel[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
	}
	else if (now == 'c' && next == 'b')
	{
		//ʮ��+��
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_width[i + pianyi] % 4 == 0)//�ܱ�4��������i+1�������ߴ�=��i+1+1�������ߴ� 
			{
				tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = img_height[i + 1] * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);

			}//ȷ������ͬʱ��һ�������һ������ʱ�����  ��ʱ���������ݴ���
			else
			{
				tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = img_height[i + 1] * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = img_height[i + 1] * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = img_height[i + 1] * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = img_height[i + 1] * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
		else if (operate[i+1] == "Conv1*1")//strideΪ1��2��ͬ
		{
			tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
	}
	else if (now == 'c' && next == 'c')
	{
		//���ڲ�ʮ�ֻ��ֵĴ���
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			tran_count = (img_width[i + 1] + 1) / 2 * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = ((img_width[i + 1] + 1) / 2 + 1) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = (img_width[i + 1] + 1) / 2 * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_width[i + 1] + 1) / 2 * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
		else if (operate[i+1] == "Conv1*1")//strideΪ1��2��ͬ
		{
			tran_count = 0;
			tran_time += 0;
		}
	}
	else if (now == 'c' && next == 'd')
	{
		//ʮ��+ͨ��
		if (operate[i + 1] == "DwConv3*3")
		{
			tran_count = (img_height[i + 1] + 1) * (img_width[i + 1] + 1) / 2 * input_channel[i + 1] / 4;
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_height[i + 1] + 1) / 2 * (img_width[i + 1] + 1) / 2 * input_channel[i + 1] / 4;
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_height[i + 1] + 1) / 2 * (img_width[i + 1] + 1) / 2 * input_channel[i + 1] / 4;
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
		else
		{
			tran_count = (img_height[i + 1] + 1) / 2 * (img_width[i + 1] + 1) / 2 * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_height[i + 1] + 1) / 2 * (img_width[i + 1] + 1) / 2 * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_height[i + 1] + 1) / 2 * (img_width[i + 1] + 1) / 2 * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
	}
	else if (now == 'c' && next == 'e')
	{
		//���ڲ��еĴ���
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'd' && next == 'a')
	{
		//ͨ��+��
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_width[i + pianyi] % 4 == 0)//�ܱ�4��������i+1�������ߴ�=��i+1+1�������ߴ� 
			{
				tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 1) / 2 + 1) * img_width[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4 + 1) * img_width[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4 + 1) * img_width[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);

			}//ȷ������ͬʱ��һ�������һ������ʱ�����  ��ʱ���������ݴ���
			else
			{
				tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 1) / 2 + 2) * img_width[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4 + 2) * img_width[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4 + 2) * img_width[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 1) / 2 + 2) * img_width[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4 + 2) * img_width[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4 + 2) * img_width[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
		else if (operate[i+1] == "Conv1*1")//strideΪ1��2��ͬ
		{
			tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 1) / 2) * img_width[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4) * img_width[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4) * img_width[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
	}
	else if (now == 'd' && next == 'b')
	{
		//ͨ��+��
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_width[i + pianyi] % 4 == 0)//�ܱ�4��������i+1�������ߴ�=��i+1+1�������ߴ� 
			{
				tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 1) * img_height[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4 + 1) * img_height[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4 + 1) * img_height[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);

			}//ȷ������ͬʱ��һ�������һ������ʱ�����  ��ʱ���������ݴ���
			else
			{
				tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 2) * img_height[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4 + 2) * img_height[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4 + 2) * img_height[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 2) * img_height[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4 + 2) * img_height[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4 + 2) * img_height[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
		else if (operate[i+1] == "Conv1*1")//strideΪ1��2��ͬ
		{
			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2) * img_height[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4) * img_height[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4) * img_height[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
	}
	else if (now == 'd' && next == 'c')
	{
		//ͨ��+ʮ��
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 1) * img_height[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 1) * ((img_height[i + 1] + 1) / 2 + 1);//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 1) * ((img_height[i + 1] + 1) / 2 + 1);//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 1) * img_height[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 1) * ((img_height[i + 1] + 1) / 2 + 1);//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 1) * ((img_height[i + 1] + 1) / 2 + 1);//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
		else if (operate[i+1] == "Conv1*1")//strideΪ1��2��ͬ
		{
			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2) * img_height[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2) * ((img_height[i + 1] + 1) / 2);//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2) * ((img_height[i + 1] + 1) / 2);//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
	}
	else if (now == 'd' && next == 'd')
	{
		//���ڲ���ͨ���Ĵ���
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "Conv1*1"))
		{
			tran_count = img_height[i + 1] * img_width[i + 1] * input_channel[i + 1] / 4;
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = img_height[i + 1] * img_width[i + 1] * input_channel[i + 1] / 4;
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = img_height[i + 1] * img_width[i + 1] * input_channel[i + 1] / 4;
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
		else if (operate[i+1] == "DwConv3*3")//strideΪ1��2��ͬ
		{
			tran_count = 0;
			tran_time += 0;//ֵΪ0���ж� ����Ҫ����
			//ȷ������ͬʱ��һ�������һ������ʵ������  ��ʱ���������ݴ���
		}
	}
	else if (now == 'd' && next == 'e')
	{
		//���ڲ��еĴ���
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'e' && next == 'a')
	{
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'e' && next == 'b')
	{
		//���ڲ��еĴ���
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'e' && next == 'c')
	{
		//���ڲ��еĴ���
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'e' && next == 'd')
	{
		//���ڲ��еĴ���
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'e' && next == 'e')
	{
		//���ڲ��еĴ���
		count = 1000;
		tran_time += 1000;
	}
	if (count > 0 && count < 1000)
	{
		for (int i = 0;i < count;i++)
		{
			string s = to_string(5) +  "+" + to_string(tran_num[i]*4);
			if (cost_tranvalue.find(s) == cost_tranvalue.end())
			{
				cost_tranvalue[s] = cost_tran(bandwidth, tran_num[i]*4);
				tran_time += cost_tranvalue[s];
			}
			else
			{
				tran_time += cost_tranvalue[s];
			}
		}
	}

	return tran_time;
	//return 0;

}

//�������� ---�ֱ���ƺ�����������evalute_aa_single
//vector<double> evaluate_tran_single(int i, string s, vector<double>& tran_num)
////double evaluate_tran_single(int i, string s)
//{
//	int n = s.length();
//	char now = s[n - 1];
//	char next = s[n - 2];
//	//vector<double> tran_num;
//	double tran_count;
//	if (now == 'a' && next == 'a')
//	{
//		if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 2)
//		{
//			if (img_height[i + 1 + 1] % 4 == 0)//�ܱ�4��������i+1�������ߴ�=��i+1+1�������ߴ�
//			{
//				tran_count = img_width[i + 1] * input_channel[i + 1];
//				tran_num.push_back(tran_count);
//				tran_num.push_back(tran_count);
//			}//ȷ������ͬʱ��һ�������һ������ʱ�����  ��ʱ���������ݴ���
//			else
//			{
//				tran_count = img_width[i + 1] * input_channel[i + 1];
//				tran_num.push_back(tran_count);
//			}
//		}
//		else if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 1)
//		{
//			tran_count = img_width[i + 1] * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_num.push_back(tran_count);//������������
//		}
//		else if (operate[i] == "conv 1*1")//strideΪ1��2��ͬ
//		{
//			tran_count = 0;
//			tran_num.push_back(tran_count);//ֵΪ0���ж� ����Ҫ���䣨�ô���������ֱ�ӷ���һ���յģ����ô���
//		}
//		//���ڲ��еĴ���
//	}
//	else if (now == 'a' && next == 'b')
//	{
//		//�н���
//		if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 2)
//		{
//			if (img_width[i + 1 + 1] % 4 == 0)//�ܱ�4��������i+1�������ߴ�=��i+1+1�������ߴ� 
//			{
//				tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 1) / 2 + 1) * input_channel[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//				tran_num.push_back(tran_count);
//				tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 3) / 4 + 1) * input_channel[i + 1];
//				tran_num.push_back(tran_count);
//				tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 3) / 4 + 1) * input_channel[i + 1];
//				tran_num.push_back(tran_count);
//			}//ȷ������ͬʱ��һ�������һ������ʱ�����  ��ʱ���������ݴ���
//			else
//			{
//				tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 1) / 2 + 2) * input_channel[i + 1];
//				tran_num.push_back(tran_count);
//				tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 3) / 4 + 2) * input_channel[i + 1];
//				tran_num.push_back(tran_count);
//				tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 3) / 4 + 2) * input_channel[i + 1];
//				tran_num.push_back(tran_count);
//			}
//		}
//		else if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 1)
//		{
//			tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 1) / 2 + 2) * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 3) / 4 + 2) * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 3) / 4 + 2) * input_channel[i + 1];
//			tran_num.push_back(tran_count);//�ܹ�������������
//		}
//		else if (operate[i] == "conv 1*1")//strideΪ1��2��ͬ
//		{
//			tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 1) / 2) * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//		}
//	}
//	else if (now == 'a' && next == 'c')
//	{
//		//��+ʮ��
//		if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 2)
//		{
//			tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 1) / 2 + 1) * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = ((img_width[i + 1] + 1) / 2 + 1) * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//		}
//		else if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 1)
//		{
//			tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 1) / 2 + 1) * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = ((img_width[i + 1] + 1) / 2 + 1) * input_channel[i + 1];
//			tran_num.push_back(tran_count);//�ܹ�����2������
//		}
//		else if (operate[i] == "conv 1*1")//strideΪ1��2��ͬ
//		{
//			tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 1) / 2) * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = (img_width[i + 1] + 1) / 2 * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//		}
//	}
//	else if (now == 'a' && next == 'd')
//	{
//		//��+ͨ��
//		if (operate[i + 1] == "dwconv 3*3")
//		{
//			tran_count = (img_height[i + 1] + 3) / 4 * img_width[i + 1] * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = (img_height[i + 1] + 3) / 4 * img_width[i + 1] * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = (img_height[i + 1] + 3) / 4 * img_width[i + 1] * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//		}
//		else
//		{
//			tran_count = (img_height[i + 1] + 3) / 4 * img_width[i + 1] * input_channel[i + 1] / 2;
//			tran_num.push_back(tran_count);
//			tran_count = (img_height[i + 1] + 3) / 4 * img_width[i + 1] * input_channel[i + 1] / 4;
//			tran_num.push_back(tran_count);
//			tran_count = (img_height[i + 1] + 3) / 4 * img_width[i + 1] * input_channel[i + 1] / 4;
//			tran_num.push_back(tran_count);
//		}
//	}
//	else if (now == 'a' && next == 'e')
//	{
//		//��+����
//	}
//	else if (now == 'b' && next == 'a')
//	{
//		//��+��
//		if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 2)
//		{
//			if (img_height[i + 1 + 1] % 4 == 0)//�ܱ�4��������i+1�������ߴ�=��i+1+1�������ߴ� 
//			{
//				tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 1) / 2 + 1) * input_channel[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//				tran_num.push_back(tran_count);
//				tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 3) / 4 + 1) * input_channel[i + 1];
//				tran_num.push_back(tran_count);
//				tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 3) / 4 + 1) * input_channel[i + 1];
//				tran_num.push_back(tran_count);
//			}//ȷ������ͬʱ��һ�������һ������ʱ�����  ��ʱ���������ݴ���
//			else
//			{
//				tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 1) / 2 + 2) * input_channel[i + 1];
//				tran_num.push_back(tran_count);
//				tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 3) / 4 + 2) * input_channel[i + 1];
//				tran_num.push_back(tran_count);
//				tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 3) / 4 + 2) * input_channel[i + 1];
//				tran_num.push_back(tran_count);
//			}
//		}
//		else if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 1)
//		{
//			tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 1) / 2 + 2) * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 3) / 4 + 2) * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 3) / 4 + 2) * input_channel[i + 1];
//			tran_num.push_back(tran_count);//�ܹ�������������
//		}
//		else if (operate[i] == "conv 1*1")//strideΪ1��2��ͬ
//		{
//			tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 1) / 2) * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 3) / 4) * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 3) / 4) * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//		}
//	}
//	else if (now == 'b' && next == 'b')
//	{
//		//���ڲ��еĴ���
//		if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 2)
//		{
//			if (img_width[i + 1 + 1] % 4 == 0)//�ܱ�4��������i+1�������ߴ�=��i+1+1�������ߴ� 
//			{
//				tran_count = img_height[i + 1] * input_channel[i + 1];
//				tran_num.push_back(tran_count);
//				tran_num.push_back(tran_count);
//			}//ȷ������ͬʱ��һ�������һ������ʱ�����  ��ʱ���������ݴ���
//			else
//			{
//				tran_count = img_height[i + 1] * input_channel[i + 1];
//				tran_num.push_back(tran_count);
//			}
//		}
//		else if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 1)
//		{
//			tran_count = img_height[i + 1] * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_num.push_back(tran_count);//������������
//		}
//		else if (operate[i] == "conv 1*1")//strideΪ1��2��ͬ
//		{
//			tran_count = 0;
//			tran_num.push_back(tran_count);//ֵΪ0���ж� ����Ҫ���䣨�ô���������ֱ�ӷ���һ���յģ����ô���
//		}
//	}
//	else if (now == 'b' && next == 'c')
//	{
//		//��+ʮ��
//		if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 2)
//		{
//			tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 1) / 2) * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 1) / 2) * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 1) / 2) * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = ((img_width[i + 1] + 1) / 2 + 1) * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//		}
//		else if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 1)
//		{
//			tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 1) / 2) * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 1) / 2) * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 1) / 2) * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = ((img_width[i + 1] + 1) / 2 + 1) * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = ((img_width[i + 1] + 1) / 2 + 1) * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//		}
//		else if (operate[i] == "conv 1*1")//strideΪ1��2��ͬ
//		{
//			tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 1) / 2) * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 1) / 2) * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 1) / 2) * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//		}
//	}
//	else if (now == 'b' && next == 'd')
//	{
//		//��+ͨ��
//		if (operate[i + 1] == "dwconv 3*3")
//		{
//			tran_count = (img_width[i + 1] + 3) / 4 * img_height[i + 1] * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = (img_width[i + 1] + 3) / 4 * img_height[i + 1] * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = (img_width[i + 1] + 3) / 4 * img_height[i + 1] * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//		}
//		else
//		{
//			tran_count = (img_width[i + 1] + 3) / 4 * img_height[i + 1] * input_channel[i + 1] / 2;
//			tran_num.push_back(tran_count);
//			tran_count = (img_width[i + 1] + 3) / 4 * img_height[i + 1] * input_channel[i + 1] / 4;
//			tran_num.push_back(tran_count);
//			tran_count = (img_width[i + 1] + 3) / 4 * img_height[i + 1] * input_channel[i + 1] / 4;
//			tran_num.push_back(tran_count);
//		}
//	}
//	else if (now == 'b' && next == 'e')
//	{
//		//���ڲ��еĴ���
//	}
//	else if (now == 'c' && next == 'a')
//	{
//		//ʮ��+��
//		if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 2)
//		{
//			if (img_width[i + 1 + 1] % 4 == 0)//�ܱ�4��������i+1�������ߴ�=��i+1+1�������ߴ� 
//			{
//				tran_count = ((img_height[i + 1] + 3) / 4 + 1) * ((img_width[i + 1] + 1) / 2) * input_channel[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//				tran_num.push_back(tran_count);
//				tran_count = img_width[i + 1] * input_channel[i + 1];
//				tran_num.push_back(tran_count);
//
//			}//ȷ������ͬʱ��һ�������һ������ʱ�����  ��ʱ���������ݴ���
//			else
//			{
//				tran_count = ((img_height[i + 1] + 3) / 4 + 1) * ((img_width[i + 1] + 1) / 2) * input_channel[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//				tran_num.push_back(tran_count);
//				tran_count = img_width[i + 1] * input_channel[i + 1];
//				tran_num.push_back(tran_count);
//			}
//		}
//		else if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 1)
//		{
//			tran_count = ((img_height[i + 1] + 3) / 4 + 1) * ((img_width[i + 1] + 1) / 2) * input_channel[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//			tran_num.push_back(tran_count);
//			tran_count = img_width[i + 1] * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//		}
//		else if (operate[i] == "conv 1*1")//strideΪ1��2��ͬ
//		{
//			tran_count = ((img_height[i + 1] + 3) / 4) * ((img_width[i + 1] + 1) / 2) * input_channel[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//			tran_num.push_back(tran_count);
//		}
//	}
//	else if (now == 'c' && next == 'b')
//	{
//		//ʮ��+��
//		if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 2)
//		{
//			if (img_width[i + 1 + 1] % 4 == 0)//�ܱ�4��������i+1�������ߴ�=��i+1+1�������ߴ� 
//			{
//				tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//				tran_num.push_back(tran_count);
//				tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//				tran_num.push_back(tran_count);
//				tran_count = img_height[i + 1] * input_channel[i + 1];
//				tran_num.push_back(tran_count);
//
//			}//ȷ������ͬʱ��һ�������һ������ʱ�����  ��ʱ���������ݴ���
//			else
//			{
//				tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//				tran_num.push_back(tran_count);
//				tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//				tran_num.push_back(tran_count);
//				tran_count = img_height[i + 1] * input_channel[i + 1];
//				tran_num.push_back(tran_count);
//				tran_count = img_height[i + 1] * input_channel[i + 1];
//				tran_num.push_back(tran_count);
//			}
//		}
//		else if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 1)
//		{
//			tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//			tran_num.push_back(tran_count);
//			tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//			tran_num.push_back(tran_count);
//			tran_count = img_height[i + 1] * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = img_height[i + 1] * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//		}
//		else if (operate[i] == "conv 1*1")//strideΪ1��2��ͬ
//		{
//			tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//			tran_num.push_back(tran_count);
//			tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//			tran_num.push_back(tran_count);
//		}
//	}
//	else if (now == 'c' && next == 'c')
//	{
//		//���ڲ�ʮ�ֻ��ֵĴ���
//		if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 2)
//		{
//			tran_count = (img_width[i + 1] + 1) / 2 * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = ((img_width[i + 1] + 1) / 2 + 1) * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//		}
//		else if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 1)
//		{
//			tran_count = (img_width[i + 1] + 1) / 2 * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = (img_width[i + 1] + 1) / 2 * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = input_channel[i + 1];
//			tran_num.push_back(tran_count);
//		}
//		else if (operate[i] == "conv 1*1")//strideΪ1��2��ͬ
//		{
//			tran_count = 0;
//			tran_num.push_back(tran_count);
//		}
//	}
//	else if (now == 'c' && next == 'd')
//	{
//		//ʮ��+ͨ��
//		if (operate[i + 1] == "dwconv 3*3")
//		{
//			tran_count = (img_height[i + 1] + 1) * (img_width[i + 1] + 1) / 2 * input_channel[i + 1] / 4;
//			tran_num.push_back(tran_count);
//			tran_count = (img_height[i + 1] + 1) / 2 * (img_width[i + 1] + 1) / 2 * input_channel[i + 1] / 4;
//			tran_num.push_back(tran_count);
//			tran_count = (img_height[i + 1] + 1) / 2 * (img_width[i + 1] + 1) / 2 * input_channel[i + 1] / 4;
//			tran_num.push_back(tran_count);
//		}
//		else
//		{
//			tran_count = (img_height[i + 1] + 1) / 2 * (img_width[i + 1] + 1) / 2 * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = (img_height[i + 1] + 1) / 2 * (img_width[i + 1] + 1) / 2 * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = (img_height[i + 1] + 1) / 2 * (img_width[i + 1] + 1) / 2 * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//		}
//	}
//	else if (now == 'c' && next == 'e')
//	{
//		//���ڲ��еĴ���
//	}
//	else if (now == 'd' && next == 'a')
//	{
//		//ͨ��+��
//		if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 2)
//		{
//			if (img_width[i + 1 + 1] % 4 == 0)//�ܱ�4��������i+1�������ߴ�=��i+1+1�������ߴ� 
//			{
//				tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 1) / 2 + 1) * img_width[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//				tran_num.push_back(tran_count);
//				tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4 + 1) * img_width[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//				tran_num.push_back(tran_count);
//				tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4 + 1) * img_width[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//				tran_num.push_back(tran_count);
//
//			}//ȷ������ͬʱ��һ�������һ������ʱ�����  ��ʱ���������ݴ���
//			else
//			{
//				tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 1) / 2 + 2) * img_width[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//				tran_num.push_back(tran_count);
//				tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4 + 2) * img_width[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//				tran_num.push_back(tran_count);
//				tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4 + 2) * img_width[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//				tran_num.push_back(tran_count);
//			}
//		}
//		else if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 1)
//		{
//			tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 1) / 2 + 2) * img_width[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//			tran_num.push_back(tran_count);
//			tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4 + 2) * img_width[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//			tran_num.push_back(tran_count);
//			tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4 + 2) * img_width[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//			tran_num.push_back(tran_count);
//		}
//		else if (operate[i] == "conv 1*1")//strideΪ1��2��ͬ
//		{
//			tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 1) / 2) * img_width[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//			tran_num.push_back(tran_count);
//			tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4) * img_width[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//			tran_num.push_back(tran_count);
//			tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4) * img_width[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//			tran_num.push_back(tran_count);
//		}
//	}
//	else if (now == 'd' && next == 'b')
//	{
//		//ͨ��+��
//		if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 2)
//		{
//			if (img_width[i + 1 + 1] % 4 == 0)//�ܱ�4��������i+1�������ߴ�=��i+1+1�������ߴ� 
//			{
//				tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 1) * img_height[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//				tran_num.push_back(tran_count);
//				tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4 + 1) * img_height[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//				tran_num.push_back(tran_count);
//				tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4 + 1) * img_height[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//				tran_num.push_back(tran_count);
//
//			}//ȷ������ͬʱ��һ�������һ������ʱ�����  ��ʱ���������ݴ���
//			else
//			{
//				tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 2) * img_height[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//				tran_num.push_back(tran_count);
//				tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4 + 2) * img_height[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//				tran_num.push_back(tran_count);
//				tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4 + 2) * img_height[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//				tran_num.push_back(tran_count);
//			}
//		}
//		else if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 1)
//		{
//			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 2) * img_height[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//			tran_num.push_back(tran_count);
//			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4 + 2) * img_height[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//			tran_num.push_back(tran_count);
//			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4 + 2) * img_height[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//			tran_num.push_back(tran_count);
//		}
//		else if (operate[i] == "conv 1*1")//strideΪ1��2��ͬ
//		{
//			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2) * img_height[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//			tran_num.push_back(tran_count);
//			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4) * img_height[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//			tran_num.push_back(tran_count);
//			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4) * img_height[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//			tran_num.push_back(tran_count);
//		}
//	}
//	else if (now == 'd' && next == 'c')
//	{
//		//ͨ��+ʮ��
//		if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 2)
//		{
//			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 1) * img_height[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//			tran_num.push_back(tran_count);
//			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 1) * ((img_height[i + 1] + 1) / 2 + 1);//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//			tran_num.push_back(tran_count);
//			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 1) * ((img_height[i + 1] + 1) / 2 + 1);//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//			tran_num.push_back(tran_count);
//		}
//		else if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 1)
//		{
//			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 1) * img_height[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//			tran_num.push_back(tran_count);
//			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 1) * ((img_height[i + 1] + 1) / 2 + 1);//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//			tran_num.push_back(tran_count);
//			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 1) * ((img_height[i + 1] + 1) / 2 + 1);//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//			tran_num.push_back(tran_count);
//		}
//		else if (operate[i] == "conv 1*1")//strideΪ1��2��ͬ
//		{
//			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2) * img_height[i + 1];//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//			tran_num.push_back(tran_count);
//			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2) * ((img_height[i + 1] + 1) / 2);//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//			tran_num.push_back(tran_count);
//			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2) * ((img_height[i + 1] + 1) / 2);//֮����+3��+1��Ϊ��ȡ����ʱ����Ϊ����ʱ��
//			tran_num.push_back(tran_count);
//		}
//	}
//	else if (now == 'd' && next == 'd')
//	{
//		//���ڲ���ͨ���Ĵ���
//		if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "conv 1*1"))
//		{
//			tran_count = img_height[i + 1] * img_width[i + 1] * input_channel[i + 1] / 4;
//			tran_num.push_back(tran_count);
//			tran_count = img_height[i + 1] * img_width[i + 1] * input_channel[i + 1] / 4;
//			tran_num.push_back(tran_count);
//			tran_count = img_height[i + 1] * img_width[i + 1] * input_channel[i + 1] / 4;
//			tran_num.push_back(tran_count);
//		}
//		else if (operate[i] == "dwconv 3*3")//strideΪ1��2��ͬ
//		{
//			tran_count = 0;
//			tran_num.push_back(tran_count);//ֵΪ0���ж� ����Ҫ����
//			//ȷ������ͬʱ��һ�������һ������ʵ������  ��ʱ���������ݴ���
//		}
//	}
//	else if (now == 'd' && next == 'e')
//	{
//		//���ڲ��еĴ���
//	}
//	else if (now == 'e' && next == 'a')
//	{
//
//	}
//	else if (now == 'e' && next == 'b')
//	{
//		//���ڲ��еĴ���
//	}
//	else if (now == 'e' && next == 'c')
//	{
//		//���ڲ��еĴ���
//	}
//	else if (now == 'e' && next == 'd')
//	{
//		//���ڲ��еĴ���
//	}
//	else if (now == 'e' && next == 'e')
//	{
//		//���ڲ��еĴ���
//	}
//
//	return tran_num;
//
//}

//int compute_layer_extra(string s)
//{
//	int n = s.length();
//	int cord = 0;
//	for (int i = n - 2;i >= 0;i--)
//	{
//		if (s[i] >= '0' && s[i] <= '5')
//		{
//			cord += 1;
//		}
//		else
//			break;
//	}
//	return cord;
//}
//����������֮��Ҫ�Ӹ��жϣ�Ҳ�����ж��Ƿ񳬳�ԭʼδ��ֵĳߴ�
//��ʵ����Ҳ�У����ַ�����Ϊ���ٱ��Ѿ�����1�ˣ���������Ҫ�����ķ��������ص�ֵҲ��Ƚϴ󣬻ᱻ����

//�С��С�ʮ��һ��
//����ʮ����������ά�ȶ���exts
/*img_h		+ext
* img_h1	+extsize
* tran		 exttran---���������������������߰��а�������ʮ�ֵĴ�����ܸ���һЩ��ϸ�·���һ����ƣ�
* reset		 ��tran��Ӧ��������
* 
* now��Ӧi
* end��Ӧl
*/

int  compute_layer_extra(int now,int end,string s)//now��s�ĵ�һ��������һ��������� end����ʼ����ĵ�һ������λ�õ�ǰһ��Ϊ������
{
	//double cost_compute = 0.0;
	int n = s.length();
	int nows, ends;
	nows = now - (nn_number - n);
	ends = end - (nn_number - n);
	char cord;
	//��¼�����һ�㻮�ַ�ʽ
	cord=s[ends];
	//ת��Ϊ�޴����ʽ
	cord = cord - 'a' + '1';
	int ext = 0;
	int exttran = 0;
	//����ײ�Ϊ������䣬�ײ����һλ
	if (s[0] >= 'a' && s[0] <= 'e')
	{
		nows+=1;
	}
	//for (int i = ends - 1;i >= nows;i--)  11.04�Ķ�-1
	//for (int i = ends - 1;i >nows;i--)11.28�Ķ�-1
	for (int i = ends - 1;i >= nows;i--)
	{
		if (s[i] != cord)
		{
			//
			ext = -1;
			break;
		}
		else
		{
			if (cord == '1')
			{
				//�л��ִ���
				if (operate[nn_number - n + i+1] == "DwConv3*3"|| operate[nn_number - n + i+1] == "Conv3*3")
				{
					if (stride[nn_number - n + i + 1] == 1)
					{
						exttran = 1*ext+2;
						ext = exttran;
					}
					else if (stride[nn_number - n + i + 1] == 2)
					{
						exttran = 2 * ext + 2;
						ext = exttran;
					}
				}
				else if (operate[nn_number - n + i + 1] == "Conv1*1")
				{
					exttran = ext+0;
					ext = exttran;
				}
			}
			else if (cord == '2')
			{
				if (operate[nn_number - n + i + 1] == "DwConv3*3" || operate[nn_number - n + i + 1] == "Conv3*3")
				{
					if (stride[nn_number - n + i + 1] == 1)
					{
						exttran = 1 * ext + 2;
						ext = exttran;
					}
					else if (stride[nn_number - n + i + 1] == 2)
					{
						exttran = 2 * ext + 2;
						ext = exttran;
					}
				}
				else if (operate[nn_number - n + i + 1] == "Conv1*1")
				{
					exttran = ext + 0;
					ext = exttran;
				}
			}
			else if (cord == '3')
			{
				//ʮ�ֻ��ִ���
				if (operate[nn_number - n + i + 1] == "DwConv3*3" || operate[nn_number - n + i + 1] == "Conv3*3")
				{
					if (stride[nn_number - n + i + 1] == 1)
					{
						exttran = 1 * ext + 1;//�޸�+2��+1
						ext = exttran;
					}
					else if (stride[nn_number - n + i + 1] == 2)
					{
						exttran = 2 * ext + 1;//�޸�+2��+1
						ext = exttran;
					}
				}
				else if (operate[nn_number - n + i + 1] == "Conv1*1")
				{
					exttran = ext + 0;
					ext = exttran;
				}
			}
			else if (cord == '4')
			{
				//ͨ�����ִ���
				if (operate[nn_number - n + i+1] == "DwConv3*3")
				{
					/*ext = -1;
					break;*/
					continue;
				}
				else
				{
					ext = -1;
					break;
				}
			}
			else if (cord == '5')
			{
				//���Ȼ��ִ���
				ext = -1;
			}
		}
	}
	return ext;
}
int  tran_layer_extra(int now, int end, string s)
{
	int ext = compute_layer_extra(now, end, s);
	//double cost_compute = 0.0;
	int n = s.length();
	int nows;
	//int ends;
	//nows = now - (nn_number - n);11.04�Ķ�-2 ����һ�����Լ�1
	//nows = now - (nn_number - n)+1;//�����ײ��Ǽ�����仹�ǲ����䣬���Ǽ�1�����ֳ��������ã�������ʱ�Ķ�����������ߴ�--����
	//����ж�
	if (s[0] >= '0' && s[0] <= '5')
	{
		nows = now - (nn_number - n);
	}
	else if (s[0] >= 'a' && s[0] <= 'e')
	{
		nows = now - (nn_number - n)+1;
	}
	//ends = end - (nn_number - n);
	char cord;
	//��¼�����һ�㻮�ַ�ʽ
	cord = s[nows];
	int extsize = 0;

	
	if (ext == -1)
	{
		extsize = -1;
	}
	else
	{
		//ext��Ӧ�����ļ����������������ĳߴ��Ӧ��extsize---���Ҳ�Ͷ�Ӧʵ��������Ĵ�����
		/*extsize = ext * stride[now]+2;
		exttran = extsize+2;*///��һ�еļ�2Ӧ���ǲ��üӵ�
		if (cord == '1')
		{
			//�л��ִ���
			if (operate[now] == "DwConv3*3" || operate[now] == "Conv3*3")
			{
				if (stride[now] == 1)
				{
					extsize = 1 * ext + 2;//��2����Ϊ���˶������ӵļ��㲿�ֳߴ磬������������Ҫ�������еĳߴ�
				}
				else if (stride[now] == 2)
				{
					extsize = 2 * ext + 2;
				}
			}
			else if (operate[now] == "Conv1*1")
			{
				extsize = ext;
			}
		}
		else if (cord == '2')
		{
			if (operate[now] == "DwConv3*3" || operate[now] == "Conv3*3")
			{
				if (stride[now] == 1)
				{
					extsize = 1 * ext + 2;//��2����Ϊ���˶������ӵļ��㲿�ֳߴ磬������������Ҫ�������еĳߴ�
				}
				else if (stride[now] == 2)
				{
					extsize = 2 * ext + 2;
				}
			}
			else if (operate[now] == "Conv1*1")
			{
				extsize = ext;
			}
		}
		else if (cord == '3')
		{
			if (operate[now] == "DwConv3*3" || operate[now] == "Conv3*3")
			{
				if (stride[now] == 1)
				{
					extsize = 1 * ext + 1;//��1����Ϊ���˶������ӵļ��㲿�ֳߴ磬������������Ҫ����һ��һ�еĳߴ�
				}
				else if (stride[now] == 2)
				{
					extsize = 2 * ext + 1;
				}
			}
			else if (operate[now] == "Conv1*1")
			{
				extsize = ext;
			}
		}
		else if (cord == '4')
		{
			//ͨ�����ִ���
			if (operate[now] == "DwConv3*3")
			{
				extsize = ext;//�˴�extһ���ǵ���0��
			}
			else
			{
				ext = -1;
			}
		}
		else if (cord == '5')
		{
			//���Ȼ��ִ���
			ext = -1;
		}

	}

	/*if (ext == -1)
	{
		cost_compute = 1000;
	}
	else
	{
		cost_compute = evaluate_comp_extra(now, s, ext);
	}*/
	return extsize;
}

double evaluate_reset_single(int i, string s)
//vector<double> evaluate_ch_single(int i, string s)
{
	//vector<double> tran_num;
	double tran_count=0.0;
	double reset_time = 0.0;
	double zero_time = 0.00000204065;//padding 0 
	double value_time =  0.0000189577;//�����ʵֵ
	int n = s.length();
	char now = s[0];
	char next = s[1];//���޸�
	if (now >= '1' && now <= '5')
	{
		now = now - '1' + 'a';
	}
	if (next >= '1' && next <= '5')
	{
		next = next - '1' + 'a';
	}
	int pianyi = 2;
	if (i >= 25)
	{
		pianyi = 1;//�����ֹi+2>nn_number-1  ԭ����i+1+1��Ϊi+pianyi
	}
	if (now == 'a' && next == 'a')
	{
		if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_height[i + pianyi] % 4 == 0)//�ܱ�4��������i+1�������ߴ�=��i+1+1�������ߴ�
			{
				tran_count = input_channel[i + 1] * cost_reset(img_width[i + 1]);//ȡ
				tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 1) * zero_time;//�� 0
				tran_count += 0;//�� ��������
				tran_count += input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 1) * cost_reset(img_width[i + 1]);//��memcpy
			}
			else
			{
				tran_count = 2 * input_channel[i + 1] * cost_reset(img_width[i + 1]);//ȡ
				tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * zero_time;//�� 0
				tran_count += 0;//�� ��������
				tran_count += input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * cost_reset(img_width[i + 1]);//��memcpy
			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = 2 * input_channel[i + 1] * cost_reset(img_width[i + 1]);//ȡ
			tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * zero_time;//�� 0
			tran_count += 0;//�� ��������
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * cost_reset(img_width[i + 1]);//��memcpy


		}
		else if (operate[i+1] == "conv 1*1")//strideΪ1��2��ͬ
		{
			tran_count = 0;//ֵΪ0���ж� ����Ҫ���䣨�ô���������ֱ�ӷ���һ���յģ����ô���
		}
		//���ڲ��еĴ���
	}
	else if (now == 'a' && next == 'b')
	{
		//�н���
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_width[i + pianyi] % 4 == 0)//�ܱ�4��������i+1�������ߴ�=��i+1+1�������ߴ� 
			{
				tran_count = 3 * input_channel[i + 1] * (img_height[i + 1] + 3) / 4 * cost_reset((img_width[i + 1] + 3) / 4 + 1);//ȡ
				tran_count += input_channel[i + 1] * (img_height[i + 1] + 2) * zero_time;//�� 0
				tran_count += 0;//�� ��������
				tran_count += input_channel[i + 1] * (img_height[i + 1] + 2) * cost_reset((img_width[i + 1] + 3) / 4);//��memcpy

			}//ȷ������ͬʱ��һ�������һ������ʱ�����  ��ʱ���������ݴ���
			else
			{
				tran_count = 3 * input_channel[i + 1] * (img_height[i + 1] + 3) / 4 * cost_reset((img_width[i + 1] + 3) / 4 + 2);//ȡ
				tran_count += input_channel[i + 1] * (img_height[i + 1] + 2) * zero_time;//�� 0
				tran_count += 0;//�� ��������
				tran_count += input_channel[i + 1] * (img_height[i + 1] + 2) * cost_reset((img_width[i + 1] + 3) / 4 + 1);//��memcpy

			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = 3 * input_channel[i + 1] * (img_height[i + 1] + 3) / 4 * cost_reset((img_width[i + 1] + 3) / 4 + 2);//ȡ
			tran_count += input_channel[i + 1] * (img_height[i + 1] + 2) * zero_time;//�� 0
			tran_count += 0;//�� ��������
			tran_count += input_channel[i + 1] * (img_height[i + 1] + 2) * cost_reset((img_width[i + 1] + 3) / 4 + 1);//��memcpy

		}
		else if (operate[i+1] == "Conv1*1")//strideΪ1��2��ͬ
		{
			tran_count = 3 * input_channel[i + 1] * (img_height[i + 1] + 3) / 4 * cost_reset((img_width[i + 1] + 3) / 4);//ȡ
			tran_count += 0;//�� 0
			tran_count += 0;//�� ��������
			tran_count += 4 * input_channel[i + 1] * cost_reset((img_height[i + 1] + 3) / 4 * (img_width[i + 1] + 3) / 4);//��memcpy

		}
	}
	else if (now == 'a' && next == 'c')
	{
		//��+ʮ��
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			tran_count = input_channel[i + 1] * ((img_height[i + 1] + 3) / 4) * cost_reset((img_width[i + 1] + 1) / 2 + 1) +
				input_channel[i + 1] * cost_reset((img_width[i + 1] + 1) / 2 + 1);//ȡ
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 2) * zero_time;//�� 0
			tran_count += 0;//�� ��������
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 2) * cost_reset((img_width[i + 1] + 1) / 2 + 1);//��memcpy

		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = input_channel[i + 1] * ((img_height[i + 1] + 3) / 4) * cost_reset((img_width[i + 1] + 1) / 2 + 1) +
				input_channel[i + 1] * cost_reset((img_width[i + 1] + 1) / 2 + 1);//ȡ
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 2) * zero_time;//�� 0
			tran_count += 0;//�� ��������
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 2) * cost_reset((img_width[i + 1] + 1) / 2 + 1);//��memcpy


		}
		else if (operate[i+1] == "Conv1*1")//strideΪ1��2��ͬ
		{
			tran_count = input_channel[i + 1] * ((img_height[i + 1] + 3) / 4) * cost_reset((img_width[i + 1] + 1) / 2);//ȡ
			tran_count += 0;//�� 0
			tran_count += 0;//�� ��������
			tran_count += 2 * input_channel[i + 1] * cost_reset((img_height[i + 1] + 3) / 4 * (img_width[i + 1] + 1) / 2);//��memcpy
		}
	}
	else if (now == 'a' && next == 'd')
	{
		//��+ͨ��
		if (operate[i + 1] == "DwConv3*3")
		{
			tran_count = 0;//ȡ
			tran_count += 2 * (img_height[i + 1] + 2) * input_channel[i + 1] / 4 * zero_time;//�� 0
			tran_count += 0;//�� ��������
			tran_count += input_channel[i + 1] / 4 * (img_height[i + 1] + 2) * cost_reset(img_width[i + 1]);//��memcpy
			//cout <<"   audada hhdjdajks d   " << tran_count << endl;

		}
		else if (operate[i + 1] == "Conv1*1")
		{
			tran_count = 0;//ȡ
			tran_count += 0 * zero_time;//�� 0
			tran_count += 0;//�� ��������
			tran_count += 4 * input_channel[i + 1] * cost_reset((img_height[i + 1] + 3) / 4 * img_width[i + 1]);//��memcpy

		}
		else
		{
			tran_count = 0;//ȡ
			tran_count += 2 * (img_height[i + 1] + 2) * input_channel[i + 1] * zero_time;//�� 0
			tran_count += 0;//�� ��������
			tran_count += input_channel[i + 1] * (img_height[i + 1] + 2) * cost_reset(img_width[i + 1]);//��memcpy

		}
	}
	else if (now == 'a' && next == 'e')
	{
		//��+����
	}
	else if (now == 'b' && next == 'a')
	{
		//��+��
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_height[i + pianyi] % 4 == 0)//�ܱ�4��������i+1�������ߴ�=��i+1+1�������ߴ�
			{
				tran_count = 3 * input_channel[i + 1] * cost_reset(((img_height[i + 1] + 3) / 4 + 1) * (img_width[i + 1] + 3) / 4);//ȡ
				tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 1) * zero_time;//�� 0
				tran_count += 0;//�� ��������
				tran_count += 4 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 1) * cost_reset((img_width[i + 1] + 3) / 4);//��memcpy
			}
			else
			{
				tran_count = 3 * input_channel[i + 1] * cost_reset(((img_height[i + 1] + 3) / 4 + 2) * (img_width[i + 1] + 3) / 4);//ȡ
				tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * zero_time;//�� 0
				tran_count += 0;//�� ��������
				tran_count += 4 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * cost_reset((img_width[i + 1] + 3) / 4);//��memcpy

			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = 3 * input_channel[i + 1] * cost_reset(((img_height[i + 1] + 3) / 4 + 2) * (img_width[i + 1] + 3) / 4);//ȡ
			tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * zero_time;//�� 0
			tran_count += 0;//�� ��������
			tran_count += 4 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * cost_reset((img_width[i + 1] + 3) / 4);//��memcpy


		}
		else if (operate[i+1] == "Conv1*1")//strideΪ1��2��ͬ
		{
			tran_count = 3 * input_channel[i + 1] * cost_reset((img_height[i + 1] + 3) / 4 * (img_width[i + 1] + 3) / 4);//ȡ
			tran_count += 0 * zero_time;//�� 0
			tran_count += 0;//�� ��������
			tran_count += 4 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4) * cost_reset((img_width[i + 1] + 3) / 4);//��memcpy

		}
	}
	else if (now == 'b' && next == 'b')
	{
		//���ڲ��еĴ���
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_width[i + pianyi] % 4 == 0)//�ܱ�4��������i+1�������ߴ�=��i+1+1�������ߴ� 
			{
				tran_count = input_channel[i + 1] * img_height[i + 1] * value_time;;//ȡ
				tran_count += 0 * zero_time;//�� 0
				tran_count += input_channel[i + 1] * img_height[i + 1] * value_time;//�� ��������
				tran_count += input_channel[i + 1] * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4)
					+ 2 * input_channel[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + 1);//��memcpy

			}//ȷ������ͬʱ��һ�������һ������ʱ�����  ��ʱ���������ݴ���
			else
			{
				tran_count = 2 * input_channel[i + 1] * img_height[i + 1] * value_time;;//ȡ
				tran_count += 0 * zero_time;//�� 0
				tran_count += 2 * input_channel[i + 1] * img_height[i + 1] * value_time;//�� ��������
				tran_count += input_channel[i + 1] * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4)
					+ 2 * input_channel[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + 2);//��memcpy

			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = 2 * input_channel[i + 1] * img_height[i + 1] * value_time;;//ȡ
			tran_count += 0 * zero_time;//�� 0
			tran_count += 2 * input_channel[i + 1] * img_height[i + 1] * value_time;//�� ��������
			tran_count += input_channel[i + 1] * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4)
				+ 2 * input_channel[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + 2);//��memcpy

		}
		else if (operate[i+1] == "Conv1*1")//strideΪ1��2��ͬ
		{
			tran_count = 0;//ȡ
			tran_count += 0;//�� 0
			tran_count += 0;//�� ��������
			tran_count += 0;//��memcpy

		}
	}
	else if (now == 'b' && next == 'c')
	{
		//��+ʮ��
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			tran_count = 2 * input_channel[i + 1] * cost_reset(((img_height[i + 1] + 1) / 2) * (img_width[i + 1] + 3) / 4) +
				input_channel[i + 1] * cost_reset((img_width[i + 1] + 1) / 2) +
				input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * value_time;//ȡ
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 2 + 1) * zero_time;//�� 0
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * value_time;//�� ��������
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 2) * cost_reset((img_width[i + 1] + 1) / 2);//��memcpy

		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = 2 * input_channel[i + 1] * cost_reset(((img_height[i + 1] + 1) / 2) * (img_width[i + 1] + 3) / 4) +
				input_channel[i + 1] * cost_reset((img_width[i + 1] + 1) / 2) +
				input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * value_time;//ȡ
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 2 + 1) * zero_time;//�� 0
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * value_time;//�� ��������
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 2) * cost_reset((img_width[i + 1] + 1) / 2);//��memcpy
		}
		else if (operate[i+1] == "Conv1*1")//strideΪ1��2��ͬ
		{
			tran_count = 2 * input_channel[i + 1] * cost_reset(((img_height[i + 1] + 1) / 2) * (img_width[i + 1] + 3) / 4);//ȡ
			tran_count += 0;//�� 0
			tran_count += 0;//�� ��������
			tran_count += 2 * input_channel[i + 1] * (img_height[i + 1] + 1) / 2 * cost_reset((img_width[i + 1] + 3) / 4);//��memcpy
		}
	}
	else if (now == 'b' && next == 'd')
	{
		//��+ͨ��
		if (operate[i + 1] == "DwConv3*3")
		{
			tran_count = 0;//ȡ
			tran_count += 2 * (img_height[i + 1] + 2) * input_channel[i + 1] / 4 * zero_time;//�� 0
			tran_count += 0;//�� ��������
			tran_count += 4 * input_channel[i + 1] / 4 * (img_height[i + 1] + 2) * cost_reset((img_width[i + 1] + 3 / 4));//��memcpy

		}
		else if (operate[i + 1] == "Conv1*1")
		{
			tran_count = 0;//ȡ
			tran_count += 0 * zero_time;//�� 0
			tran_count += 0;//�� ��������
			tran_count += 4 * input_channel[i + 1] * (img_height[i + 1]) * cost_reset((img_width[i + 1] + 3 / 4));//��memcpy

		}
		else
		{
			tran_count = 0;//ȡ
			tran_count += 2 * (img_height[i + 1] + 2) * input_channel[i + 1] * zero_time;//�� 0
			tran_count += 0;//�� ��������
			tran_count += 4 * input_channel[i + 1] * (img_height[i + 1] + 2) * cost_reset((img_width[i + 1] + 3 / 4));//��memcpy

		}
	}
	else if (now == 'b' && next == 'e')
	{
		//���ڲ��еĴ���
	}
	else if (now == 'c' && next == 'a')
	{
		//ʮ��+��
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_height[i + pianyi] % 4 == 0)//�ܱ�4��������i+1�������ߴ�=��i+1+1�������ߴ�
			{
				tran_count = input_channel[i + 1] * cost_reset(((img_height[i + 1] + 3) / 4 + 1) * (img_width[i + 1] + 1) / 2)
					+ input_channel[i + 1] * cost_reset(img_width[i + 1]);//ȡ
				tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 1) * zero_time;//�� 0
				tran_count += 0;//�� ��������
				tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 1) * cost_reset((img_width[i + 1] + 1) / 2);//��memcpy}
			}
			else
			{
				tran_count = input_channel[i + 1] * cost_reset(((img_height[i + 1] + 3) / 4 + 1) * (img_width[i + 1] + 1) / 2)
					+ input_channel[i + 1] * cost_reset(img_width[i + 1]);//ȡ
				tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * zero_time;//�� 0
				tran_count += 0;//�� ��������
				tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * cost_reset((img_width[i + 1] + 1) / 2);//��memcpy
			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = input_channel[i + 1] * cost_reset(((img_height[i + 1] + 3) / 4 + 1) * (img_width[i + 1] + 1) / 2)
				+ input_channel[i + 1] * cost_reset(img_width[i + 1]);//ȡ
			tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * zero_time;//�� 0
			tran_count += 0;//�� ��������
			tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * cost_reset((img_width[i + 1] + 1) / 2);//��memcpy


		}
		else if (operate[i+1] == "Conv1*1")//strideΪ1��2��ͬ
		{
			tran_count = input_channel[i + 1] * cost_reset((img_height[i + 1] + 3) / 4 * (img_width[i + 1] + 1) / 2);//ȡ
			tran_count += 0 * zero_time;//�� 0
			tran_count += 0;//�� ��������
			tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4) * cost_reset((img_width[i + 1] + 1) / 2);//��memcpy

		}

	}
	else if (now == 'c' && next == 'b')
	{
		//���ڲ��еĴ���
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_width[i + pianyi] % 4 == 0)//�ܱ�4��������i+1�������ߴ�=��i+1+1�������ߴ� 
			{
				tran_count = 2 * input_channel[i + 1] * (img_height[i + 1] + 1) / 2 * cost_reset((img_width[i + 1] + 3) / 4) +
					1 * input_channel[i + 1] * img_height[i + 1] * value_time;//ȡ
				tran_count += 0;//�� 0
				tran_count += 1 * input_channel[i + 1] * img_height[i + 1] * value_time;;//�� ��������
				tran_count += input_channel[i + 1] * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4)
					+ 2 * input_channel[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + 1);//��memcpy

			}//ȷ������ͬʱ��һ�������һ������ʱ�����  ��ʱ���������ݴ���
			else
			{
				tran_count = 2 * input_channel[i + 1] * (img_height[i + 1] + 1) / 2 * cost_reset((img_width[i + 1] + 3) / 4) +
					2 * input_channel[i + 1] * img_height[i + 1] * value_time;//ȡ
				tran_count += 0;//�� 0
				tran_count += 2 * input_channel[i + 1] * img_height[i + 1] * value_time;;//�� ��������
				tran_count += input_channel[i + 1] * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4)
					+ 2 * input_channel[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + 2);//��memcpy
			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = 2 * input_channel[i + 1] * (img_height[i + 1] + 1) / 2 * cost_reset((img_width[i + 1] + 3) / 4) +
				2 * input_channel[i + 1] * img_height[i + 1] * value_time;//ȡ
			tran_count += 0;//�� 0
			tran_count += 2 * input_channel[i + 1] * img_height[i + 1] * value_time;;//�� ��������
			tran_count += input_channel[i + 1] * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4)
				+ 2 * input_channel[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + 2);//��memcpy
		}
		else if (operate[i+1] == "Conv1*1")//strideΪ1��2��ͬ
		{
			tran_count = 2 * input_channel[i + 1] * (img_height[i + 1] + 1) / 2 * cost_reset((img_width[i + 1] + 3) / 4);//ȡ
			tran_count += 0;//�� 0
			tran_count += 0;//�� ��������
			tran_count += 2 * input_channel[i + 1] * cost_reset((img_height[i + 1] + 1) / 2 * (img_width[i + 1] + 3) / 4);//��memcpy

		}
	}
	else if (now == 'c' && next == 'c')
	{
		//���ڲ�ʮ�ֻ��ֵĴ���
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			tran_count = input_channel[i + 1] * cost_reset((img_width[i + 1] + 1) / 2) +
				input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * value_time;//ȡ
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 2 + 1) * zero_time;//�� 0
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * value_time;//�� ��������
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 2) * cost_reset((img_width[i + 1] + 1) / 2);//��memcpy

		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = input_channel[i + 1] * cost_reset((img_width[i + 1] + 1) / 2) +
				input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * value_time;//ȡ
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 2 + 1) * zero_time;//�� 0
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * value_time;//�� ��������
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 2) * cost_reset((img_width[i + 1] + 1) / 2);//��memcpy
		}
		else if (operate[i+1] == "Conv1*1")//strideΪ1��2��ͬ
		{
			tran_count = 0;//ȡ
			tran_count += 0;//�� 0
			tran_count += 0;//�� ��������
			tran_count += 0;//��memcpy
		}
	}
	else if (now == 'c' && next == 'd')
	{
		//ʮ��+ͨ��
		if (operate[i + 1] == "DwConv3*3")
		{
			tran_count = 0;//ȡ
			tran_count += 2 * (img_height[i + 1] + 2) * input_channel[i + 1] / 4 * zero_time;//�� 0
			tran_count += 0;//�� ��������
			tran_count += 2 * input_channel[i + 1] / 4 * (img_height[i + 1] + 2) * cost_reset((img_width[i + 1] + 1 / 2));//��memcpy

		}
		else if (operate[i + 1] == "Conv1*1")
		{
			tran_count = 0;//ȡ
			tran_count += 0 * zero_time;//�� 0
			tran_count += 0;//�� ��������
			tran_count += 2 * input_channel[i + 1] * (img_height[i + 1]) * cost_reset((img_width[i + 1] + 1 / 2));//��memcpy

		}
		else
		{
			tran_count = 0;//ȡ
			tran_count += 2 * (img_height[i + 1] + 2) * input_channel[i + 1] * zero_time;//�� 0
			tran_count += 0;//�� ��������
			tran_count += 2 * input_channel[i + 1] * (img_height[i + 1] + 2) * cost_reset((img_width[i + 1] + 1 / 2));//��memcpy

		}
	}
	else if (now == 'c' && next == 'e')
	{
		//���ڲ��еĴ���
	}
	else if (now == 'd' && next == 'a')
	{
		//���ڲ��еĴ���
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_height[i + pianyi] % 4 == 0)//�ܱ�4��������i+1�������ߴ�=��i+1+1�������ߴ�
			{
				tran_count = 3 * input_channel[i + 1] / 4 * cost_reset(((img_height[i + 1] + 3) / 4 + 1) * img_width[i + 1]);//ȡ
				tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 1) * zero_time;//�� 0
				tran_count += 0;//�� ��������
				tran_count += input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 1) * cost_reset(img_width[i + 1]);//��memcpy
			}
			else
			{
				tran_count = 3 * input_channel[i + 1] / 4 * cost_reset(((img_height[i + 1] + 3) / 4 + 2) * img_width[i + 1]);//ȡ
				tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * zero_time;//�� 0
				tran_count += 0;//�� ��������
				tran_count += input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * cost_reset(img_width[i + 1]);//��memcpy
			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = 3 * input_channel[i + 1] / 4 * cost_reset(((img_height[i + 1] + 3) / 4 + 2) * img_width[i + 1]);//ȡ
			tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * zero_time;//�� 0
			tran_count += 0;//�� ��������
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * cost_reset(img_width[i + 1]);//��memcpy
		}
		else if (operate[i+1] == "Conv1*1")//strideΪ1��2��ͬ
		{
			tran_count = 3 * input_channel[i + 1] / 4 * cost_reset((img_height[i + 1] + 3) / 4 * img_width[i + 1]);//ȡ
			tran_count += 0 * zero_time;//�� 0
			tran_count += 0;//�� ��������
			tran_count += 0;//��memcpy

		}
	}
	else if (now == 'd' && next == 'b')
	{
		//ͨ��+�еĴ���
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_width[i + pianyi] % 4 == 0)//�ܱ�4��������i+1�������ߴ�=��i+1+1�������ߴ� 
			{
				tran_count = 3 * input_channel[i + 1] / 4 * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + 1);//ȡ
				tran_count += input_channel[i + 1] * (img_height[i + 1]) * zero_time;//�� 0
				tran_count += 0;//�� ��������
				tran_count += input_channel[i + 1] * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4)
					+ 2 * input_channel[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + 1);//��memcpy

			}//ȷ������ͬʱ��һ�������һ������ʱ�����  ��ʱ���������ݴ���
			else
			{
				tran_count = 3 * input_channel[i + 1] / 4 * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + 2);//ȡ
				tran_count += input_channel[i + 1] * (img_height[i + 1]) * zero_time;//�� 0
				tran_count += 0;//�� ��������
				tran_count += input_channel[i + 1] * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + 1)
					+ 2 * input_channel[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + 2);//��memcpy

			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = 3 * input_channel[i + 1] / 4 * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + 2);//ȡ
			tran_count += input_channel[i + 1] * (img_height[i + 1]) * zero_time;//�� 0
			tran_count += 0;//�� ��������
			tran_count += input_channel[i + 1] * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + 1)
				+ 2 * input_channel[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + 2);//��memcpy

		}
		else if (operate[i+1] == "Conv1*1")//strideΪ1��2��ͬ
		{
			tran_count = 3 * input_channel[i + 1] / 4 * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4);//ȡ
			tran_count += 0 * zero_time;//�� 0
			tran_count += 0;//�� ��������
			tran_count += 0;//��memcpy

		}
	}
	else if (now == 'd' && next == 'c')
	{
		//ͨ��+ʮ��
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			tran_count = input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * cost_reset((img_width[i + 1] + 1) / 2 + 1) +
				input_channel[i + 1] * ((img_height[i + 1] + 1) / 2) * cost_reset((img_width[i + 1] + 1) / 2 + 1) +
				input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * cost_reset((img_width[i + 1] + 1) / 2);//ȡ
			tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 2) * zero_time;//�� 0
			tran_count += 0 * value_time;//�� ��������
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 2) * cost_reset((img_width[i + 1] + 1) / 2);//��memcpy

		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = 3 * input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * cost_reset((img_width[i + 1] + 1) / 2 + 1);//ȡ
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * zero_time;//�� 0
			tran_count += 0 * value_time;//�� ��������
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * cost_reset((img_width[i + 1] + 1) / 2 + 1) +
				input_channel[i + 1] * ((img_width[i + 1] + 1) / 2 + 2);//��memcpy
		}
		else if (operate[i+1] == "Conv1*1")//strideΪ1��2��ͬ
		{
			tran_count = 3 * input_channel[i + 1] * ((img_height[i + 1] + 1) / 2) * cost_reset((img_width[i + 1] + 1) / 2);//ȡ
			tran_count += 0;//�� 0
			tran_count += 0;//�� ��������
			tran_count += 0;//��memcpy
		}
	}
	else if (now == 'd' && next == 'd')
	{
		//���ڲ���ͨ���Ĵ���
		if (operate[i + 1] == "DwConv3*3")
		{
			tran_count = 0;//ȡ
			tran_count += 2 * (img_height[i + 1] + 2) * input_channel[i + 1] / 4 * zero_time;//�� 0
			tran_count += 0;//�� ��������
			tran_count += input_channel[i + 1] / 4 * (img_height[i + 1] + 2) * cost_reset(img_width[i + 1]);//��memcpy

		}
		else if (operate[i + 1] == "Conv1*1")
		{
			tran_count = 0;//ȡ
			tran_count += 0;//�� 0
			tran_count += 0;//�� ��������
			tran_count += 0;//��memcpy

		}
		else
		{
			tran_count = 0;//ȡ
			tran_count += 2 * (img_height[i + 1] + 2) * input_channel[i + 1] * zero_time;//�� 0
			tran_count += 0;//�� ��������
			tran_count += input_channel[i + 1] * (img_height[i + 1] + 2) * cost_reset(img_width[i + 1]);//��memcpy

		}
	}
	else if (now == 'd' && next == 'e')
	{
		//���ڲ��еĴ���
	}
	else if (now == 'e' && next == 'a')
	{
		//���ڲ��еĴ���
	}
	else if (now == 'e' && next == 'b')
	{
		//���ڲ��еĴ���
	}
	else if (now == 'e' && next == 'c')
	{
		//���ڲ��еĴ���
	}
	else if (now == 'e' && next == 'd')
	{
		//���ڲ��еĴ���
	}
	else if (now == 'e' && next == 'e')
	{
		//���ڲ��еĴ���
	}
	return tran_count;
	//return 0;
}

//������������Ĵ��� ---�ֱ���ƺ�����������evalute_aa_extra
double evaluate_tran_extra(int i, int l, string s)
{
	int ext= tran_layer_extra(i, l, s);
	int n = s.length();
	char now = s[0];
	char next = s[1];
	int pianyi = 2;
	if (i >= 25)
	{
		pianyi = 1;//�����ֹi+2>nn_number-1  ԭ����i+1+1��Ϊi+pianyi ����Ӱ������Ϊ25���Ժ�ߴ粻��
	}
	//ת����ͳһ��ʽ
	if (now >= '1' && now <= '5')
	{
		now = now - '1' + 'a';
	}
	if (next >= '1' && next <= '5')
	{
		next = next - '1' + 'a';
	}
	vector<double> tran_num;
	int tran_count;
	int count = 0;
	double tran_time = 0.0;
	if (now == 'a' && next == 'a')
	{
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			//�����������1��ע��������ܻ�i+2>nn_number-1
			if (img_height[i + pianyi] % 4 == 0)//�ܱ�4��������i+1�������ߴ�=��i+1+1�������ߴ� 
			{
				tran_count =(ext/2-1) * img_width[i + 1] * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				tran_count =ext/2* img_width[i + 1] * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				//tran_time += cost_tran(bandwidth, tran_count);
				//cout << "sssdsfsfsfssffs          " << tran_time << endl;
			}//ȷ������ͬʱ��һ�������һ������ʱ�����  ��ʱ���������ݴ���
			else
			{
				tran_count = ext / 2 * img_width[i + 1] * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				tran_count = ext / 2 * img_width[i + 1] * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = ext / 2 * img_width[i + 1] * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			tran_count = ext / 2 * img_width[i + 1] * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			//tran_time += cost_tran(bandwidth, tran_count);//������������
		}
		else if (operate[i + 1] == "Conv1*1")//strideΪ1��2��ͬ
		{
			//tran_count = (ext / 2 - 1) * img_width[i + 1] * input_channel[i + 1];
			tran_count = ext / 2  * img_width[i + 1] * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_count = (ext / 2 - 1) * img_width[i + 1] * input_channel[i + 1];
			tran_count = ext / 2 * img_width[i + 1] * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_count = 0;
			//tran_time += 0;//ֵΪ0���ж� ����Ҫ���䣨�ô���������ֱ�ӷ���һ���յģ����ô���
		}


		//���ڲ��еĴ���
	}
	else if (now == 'a' && next == 'b')
	{
		//�н���
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'a' && next == 'c')
	{
		//��+ʮ��
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'a' && next == 'd')
	{
		//��+ͨ��
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'a' && next == 'e')
	{
		//��+����
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'b' && next == 'a')
	{
		//��+��
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'b' && next == 'b')
	{
		//���ڲ��еĴ���
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_width[i + pianyi] % 4 == 0)//�ܱ�4��������i+1�������ߴ�=��i+1+1�������ߴ� 
			{
				tran_count = (ext / 2 - 1)* img_height[i + 1] * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				tran_count = ext / 2* img_height[i + 1] * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				
				//tran_time += cost_tran(bandwidth, tran_count);
			}//ȷ������ͬʱ��һ�������һ������ʱ�����  ��ʱ���������ݴ���
			else
			{
				tran_count = ext / 2 * img_height[i + 1] * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				tran_count = ext / 2 * img_height[i + 1] * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = ext / 2 * img_height[i + 1] * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			tran_count = ext / 2 * img_height[i + 1] * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);//������������
		}
		else if (operate[i + 1] == "Conv1*1")//strideΪ1��2��ͬ
		{
			//tran_count = (ext / 2 - 1) * img_height[i + 1] * input_channel[i + 1];
			tran_count = ext / 2  * img_height[i + 1] * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_count = (ext / 2 - 1) * img_height[i + 1] * input_channel[i + 1];
			tran_count = ext / 2  * img_height[i + 1] * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
		}
	}
	else if (now == 'b' && next == 'c')
	{
		//��+ʮ��
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'b' && next == 'd')
	{
		//��+ͨ��
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'b' && next == 'e')
	{
		//���ڲ��еĴ���
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'c' && next == 'a')
	{
		//ʮ��+��
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'c' && next == 'b')
	{
		//ʮ��+��
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'c' && next == 'c')
	{
		//���ڲ�ʮ�ֻ��ֵĴ���
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			//tran_count = ext / 2  *(img_width[i + 1] + 1) / 2 * input_channel[i + 1];
			tran_count = ext * (img_width[i + 1] + 1) / 2 * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			//tran_count = ext / 2 * ((img_width[i + 1] + 1) / 2 + 1) * input_channel[i + 1];
			tran_count = ext * ((img_width[i + 1] + 1) / 2 + 1) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			//tran_count = ext / 2 * (img_width[i + 1] + 1) / 2 * input_channel[i + 1];
			tran_count = ext  * (img_width[i + 1] + 1) / 2 * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			//tran_count =ext / 2  * (img_width[i + 1] + 1) / 2 * input_channel[i + 1];
			tran_count = ext * (img_width[i + 1] + 1) / 2 * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
		else if (operate[i + 1] == "Conv1*1")//strideΪ1��2��ͬ
		{
			//tran_count = (ext / 2-1) * (img_width[i + 1] + 1) / 2 * input_channel[i + 1];
			tran_count = ext * (img_width[i + 1] + 1) / 2 * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			//tran_count = (ext / 2 - 1) * ((img_width[i + 1] + 1) / 2 + 1) * input_channel[i + 1];
			tran_count = ext  * ((img_width[i + 1] + 1) / 2 + 1) * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
		}
	}
	else if (now == 'c' && next == 'd')
	{
		//ʮ��+ͨ��
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'c' && next == 'e')
	{
		//���ڲ��еĴ���
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'd' && next == 'a')
	{
		//ͨ��+��
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'd' && next == 'b')
	{
		//ͨ��+��
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'd' && next == 'c')
	{
		//ͨ��+ʮ��
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'd' && next == 'd')
	{
		//���ڲ���ͨ���Ĵ���
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "Conv1*1"))
		{
			tran_count = img_height[i + 1] * img_width[i + 1] * input_channel[i + 1] / 4;
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = img_height[i + 1] * img_width[i + 1] * input_channel[i + 1] / 4;
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = img_height[i + 1] * img_width[i + 1] * input_channel[i + 1] / 4;
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
		else if (operate[i + 1] == "DwConv3*3")//strideΪ1��2��ͬ
		{
			tran_count = 0;
			tran_time += 0;//ֵΪ0���ж� ����Ҫ����
			//ȷ������ͬʱ��һ�������һ������ʵ������  ��ʱ���������ݴ���
		}
	}
	else if (now == 'd' && next == 'e')
	{
		//���ڲ��еĴ���
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'e' && next == 'a')
	{
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'e' && next == 'b')
	{
		//���ڲ��еĴ���
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'e' && next == 'c')
	{
		//���ڲ��еĴ���
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'e' && next == 'd')
	{
		//���ڲ��еĴ���
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'e' && next == 'e')
	{
		//���ڲ��еĴ���
		count = 1000;
		tran_time += 1000;
	}
	if (count > 0 && count < 1000)
	{
		for (int i = 0;i < count;i++)
		{
			string s = to_string(5) + "+" + to_string(tran_num[i]*4);
			if (cost_tranvalue.find(s) == cost_tranvalue.end())
			{
				cost_tranvalue[s] = cost_tran(bandwidth, tran_num[i]*4);
				tran_time += cost_tranvalue[s];
			}
			else
			{
				tran_time += cost_tranvalue[s];
			}
		}
	}

	return tran_time;
	//return 0;

}
double evaluate_reset_extra(int i,int l, string s)
{
	int ext = tran_layer_extra(i, l, s);
	double tran_count = 0.0;
	double reset_time = 0.0;
	double zero_time = 0.00000204065;//padding 0 
	double value_time = 0.0000189577;//�����ʵֵ
	int n = s.length();
	char now = s[0];
	char next = s[1];//���޸�
	int pianyi = 2;
	if (i >= 25)
	{
		pianyi = 1;//�����ֹi+2>nn_number-1  ԭ����i+1+1��Ϊi+pianyi
	}
	if (now >= '1' && now <= '5')
	{
		now = now - '1' + 'a';
	}
	if (next >= '1' && next <= '5')
	{
		next = next - '1' + 'a';
	}
	if (now == 'a' && next == 'a')
	{
		if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_height[i + pianyi] % 4 == 0)//�ܱ�4��������i+1�������ߴ�=��i+1+1�������ߴ�
			{
				tran_count = (ext-1)*input_channel[i + 1] * cost_reset(img_width[i + 1]);//ȡ
				tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + ext-1) * zero_time;//�� 0
				tran_count += 0;//�� ��������
				tran_count += input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + ext-1) * cost_reset(img_width[i + 1]);//��memcpy
			}
			else
			{
				tran_count = ext * input_channel[i + 1] * cost_reset(img_width[i + 1]);//ȡ
				tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + ext) * zero_time;//�� 0
				tran_count += 0;//�� ��������
				tran_count += input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + ext) * cost_reset(img_width[i + 1]);//��memcpy
			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = 2 * input_channel[i + 1] * cost_reset(img_width[i + 1]);//ȡ
			tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + ext) * zero_time;//�� 0
			tran_count += 0;//�� ��������
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + ext) * cost_reset(img_width[i + 1]);//��memcpy


		}
		else if (operate[i + 1] == "conv 1*1")//strideΪ1��2��ͬ
		{
			//tran_count = 0;//ֵΪ0���ж� ����Ҫ���䣨�ô���������ֱ�ӷ���һ���յģ����ô���
			tran_count = ext * input_channel[i + 1] * cost_reset(img_width[i + 1]);//ȡ
			tran_count += 0;//�� 0
			tran_count += 0;//�� ��������
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + ext) * cost_reset( img_width[i + 1]);
		}
		//���ڲ��еĴ���
	}
	else if (now == 'a' && next == 'b')
	{
		//�н���
		tran_count = 1000;
	}
	else if (now == 'a' && next == 'c')
	{
		//��+ʮ��
		tran_count = 1000;
	}
	else if (now == 'a' && next == 'd')
	{
		//��+ͨ��
		tran_count = 1000;
	}
	else if (now == 'a' && next == 'e')
	{
		//��+����
		tran_count = 1000;
	}
	else if (now == 'b' && next == 'a')
	{
		//��+��
		tran_count = 1000;
	}
	else if (now == 'b' && next == 'b')
	{
		//���ڲ��еĴ���
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_width[i + pianyi] % 4 == 0)//�ܱ�4��������i+1�������ߴ�=��i+1+1�������ߴ� 
			{
				tran_count = (ext - 1) *input_channel[i + 1] * img_height[i + 1] * value_time;;//ȡ
				tran_count += 0 * zero_time;//�� 0
				tran_count += (ext - 1) *input_channel[i + 1] * img_height[i + 1] * value_time;//�� ��������
				tran_count += input_channel[i + 1] * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4)
					+ 2 * input_channel[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + ext-1);//��memcpy

			}//ȷ������ͬʱ��һ�������һ������ʱ�����  ��ʱ���������ݴ���
			else
			{
				tran_count = ext * input_channel[i + 1] * img_height[i + 1] * value_time;;//ȡ
				tran_count += 0 * zero_time;//�� 0
				tran_count += ext * input_channel[i + 1] * img_height[i + 1] * value_time;//�� ��������
				tran_count += input_channel[i + 1] * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4)
					+ 2 * input_channel[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 +ext);//��memcpy

			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = ext * input_channel[i + 1] * img_height[i + 1] * value_time;;//ȡ
			tran_count += 0 * zero_time;//�� 0
			tran_count += ext * input_channel[i + 1] * img_height[i + 1] * value_time;//�� ��������
			tran_count += input_channel[i + 1] * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4)
				+ 2 * input_channel[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + ext);//��memcpy

		}
		else if (operate[i + 1] == "Conv1*1")//strideΪ1��2��ͬ
		{
			//tran_count = 0;//ȡ
			//tran_count += 0;//�� 0
			//tran_count += 0;//�� ��������
			//tran_count += 0;//��memcpy
			tran_count = ext * input_channel[i + 1] * img_height[i + 1] * value_time;;//ȡ
			tran_count += 0 * zero_time;//�� 0
			tran_count += ext * input_channel[i + 1] * img_height[i + 1] * value_time;//�� ��������
			tran_count += input_channel[i + 1] * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4);//��memcpy

		}
	}
	else if (now == 'b' && next == 'c')
	{
		//��+ʮ��
		tran_count = 1000;
	}
	else if (now == 'b' && next == 'd')
	{
		//��+ͨ��
		tran_count = 1000;
	}
	else if (now == 'b' && next == 'e')
	{
		//���ڲ��еĴ���
		tran_count = 1000;
	}
	else if (now == 'c' && next == 'a')
	{
		//ʮ��+��
		tran_count = 1000;

	}
	else if (now == 'c' && next == 'b')
	{
		//���ڲ��еĴ���
		tran_count = 1000;
	}
	else if (now == 'c' && next == 'c')
	{
		//���ڲ�ʮ�ֻ��ֵĴ���
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			tran_count = ext*input_channel[i + 1] * cost_reset((img_width[i + 1] + 1) / 2) +
				ext*input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * value_time;//ȡ
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + ext) * zero_time;//�� 0
			//tran_count += ext*input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + ext) * value_time;//�� ��������
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + ext) * value_time;//�� ��������
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1+ext) * cost_reset((img_width[i + 1] + 1) / 2);//��memcpy

		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = ext*input_channel[i + 1] * cost_reset((img_width[i + 1] + 1) / 2) +
				ext*input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * value_time;//ȡ
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 +ext+ 1) * zero_time;//�� 0
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + ext) * value_time;//�� ��������
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1+ext) * cost_reset((img_width[i + 1] + 1) / 2);//��memcpy
		}
		else if (operate[i + 1] == "Conv1*1")//strideΪ1��2��ͬ
		{
			//tran_count = 0;//ȡ
			//tran_count += 0;//�� 0
			//tran_count += 0;//�� ��������
			//tran_count += 0;//��memcpy
			tran_count = ext * input_channel[i + 1] * cost_reset((img_width[i + 1] + 1) / 2) +
				ext * input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * value_time;//ȡ
			tran_count += 0;//�� 0
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + ext) * value_time;//�� ��������
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + ext) * cost_reset((img_width[i + 1] + 1) / 2);//��memcpy
		}
	}
	else if (now == 'c' && next == 'd')
	{
		//ʮ��+ͨ��
		tran_count = 1000;
	}
	else if (now == 'c' && next == 'e')
	{
		//���ڲ��еĴ���
		tran_count = 1000;
	}
	else if (now == 'd' && next == 'a')
	{
		//���ڲ��еĴ���
		tran_count = 1000;
	}
	else if (now == 'd' && next == 'b')
	{
		//ͨ��+�еĴ���
		tran_count = 1000;
	}
	else if (now == 'd' && next == 'c')
	{
		//ͨ��+ʮ��
		tran_count = 1000;
	}
	else if (now == 'd' && next == 'd')
	{
		//���ڲ���ͨ���Ĵ���
		if (operate[i + 1] == "DwConv3*3")
		{
			tran_count = 0;//ȡ
			tran_count += 2 * (img_height[i + 1] + 2) * input_channel[i + 1] / 4 * zero_time;//�� 0
			tran_count += 0;//�� ��������
			tran_count += input_channel[i + 1] / 4 * (img_height[i + 1] + 2) * cost_reset(img_width[i + 1]);//��memcpy

		}
		else if (operate[i + 1] == "Conv1*1")
		{
			tran_count = 0;//ȡ
			tran_count += 0;//�� 0
			tran_count += 0;//�� ��������
			tran_count += 0;//��memcpy

		}
		else
		{
			tran_count = 0;//ȡ
			tran_count += 2 * (img_height[i + 1] + 2) * input_channel[i + 1] * zero_time;//�� 0
			tran_count += 0;//�� ��������
			tran_count += input_channel[i + 1] * (img_height[i + 1] + 2) * cost_reset(img_width[i + 1]);//��memcpy

		}
	}
	else if (now == 'd' && next == 'e')
	{
		//���ڲ��еĴ���
		tran_count = 1000;
	}
	else if (now == 'e' && next == 'a')
	{
		//���ڲ��еĴ���
		tran_count = 1000;
	}
	else if (now == 'e' && next == 'b')
	{
		//���ڲ��еĴ���
		tran_count = 1000;
	}
	else if (now == 'e' && next == 'c')
	{
		//���ڲ��еĴ���
		tran_count = 1000;
	}
	else if (now == 'e' && next == 'd')
	{
		//���ڲ��еĴ���
		tran_count = 1000;
	}
	else if (now == 'e' && next == 'e')
	{
		//���ڲ��еĴ���
		tran_count = 1000;
	}
	return tran_count;
	//return 0;
}


//double evaluate_seq(string s, int l, int k)
//{
//
//	double evaluate_seq1 = 0;
//	evaluate_seq1 += dp_nn[l][k];//��l�������䣬�������������ֱ�Ӽ���
//	/*if (s.length()>=2&&s.length() <= 5)
//	{
//		cout<<s <<"    kanzheli---1   " << evaluate_seq1 << endl;
//	}
//	if (s.length() >= 25 )
//	{
//		cout << s << "    kanzheli---1   " << evaluate_seq1 << endl;
//	}*/
//	//���� 
//
//	int n = s.length();//��ǰ���г���
//	int m = l - nn_number + n;//��l��������s���ǵ�l-nn_number+n;
//	//��ʼ��m�������䣬ĩβ��n-1��������
//	//�����м䲻���Ķ�����㣬ÿ��ǰһ�㣬�����һ����� 
//	int ext = 0;//ͳ���������ٲ�û��
//	//cout<<"kanzheliiiii  " << m - 1 << endl;
//	for (int i = m - 1;i > 0;i--)
//	{
//		//������������Ӧ������˼������ ����i-m-1����� �������е�i���Ӧģ�͵�nn_number-1-i�㣬
//		//evaluate_seq1 += evaluate_extra_single(nn_number - i - 1, s, i - m-1);
//		ext += 1;
//		evaluate_seq1 += evaluate_comp_extra(nn_number - n + i, l, s, ext);//��i���Ӧ�ܳ���i+nn_number-n
//	}
//
//	/*if (s == "aaa")
//	{
//		cout << "zaikanzheli  " << n<<" <-nm->  "<<m << endl;
//	}*/
//
//	//���㿪ʼ�������ǰһ�㣨��n-1�㼴��nn_number-1-nn+1�㣩�Ĵ����������� ���������⴫��
//	if (nn_number == n && s[0] >= '1' && s[0] <= '5')//����ǰ��Ϊ��0�㣬�ҵ�0�����󲻴���
//	{
//		evaluate_seq1 += 0;//����if���������
//		ext += 1;//��ǰ��һ�㲻���䣬��Ҫ�ٶ���һ��
//		//pingbi1:cout<<"            asfffffffffffffffffffffffffffffffff                         " << s << endl;
//	}
//	else
//	{
//		if (ext == 0)
//		{
//			evaluate_seq1 += evaluate_tran_single(nn_number - n, s);//�����ڲ�����
//	/*		if (s.length() >= 2 && s.length() <= 5)
//			{
//				cout << s << "    kanzheli---2  " << evaluate_seq1 << endl;
//			}
//			if (s.length() >= 25)
//			{
//				cout << s << "    kanzheli---2   " << evaluate_seq1 << endl;
//			}*/
//			evaluate_seq1 += evaluate_reset_single(nn_number - n, s) / 1000.0;
//			/*if (s.length() >= 2 && s.length() <= 5)
//			{
//				cout << s << "    kanzheli---2.1  " << evaluate_seq1 << endl;
//			}
//			if (s.length() >= 25)
//			{
//				cout << s << "    kanzheli---2.1   " << evaluate_seq1 << endl;
//			}*/
//		}
//		else
//		{
//			//evaluate_seq1 += 1000;
//			evaluate_seq1 += evaluate_tran_extra(nn_number - n, l, s);
//			//evaluate_seq1 += 1000;
//			evaluate_seq1 += evaluate_reset_extra(nn_number - n, l, s) / 1000.0;
//		}
//
//		//evaluate_seq1 += evaluate_tran_extra(nn_number - n, s, ext);//�����ڲ�����
//		//evaluate_seq1 += evaluate_reset_extra(nn_number - n, s, ext);//�����ڲ�����
//	}
//
//	//������ǰ��һ�㣬Ҳ���ǵ�ǰ������Ҫ�ļ���   
//	//��һ���Ǽ������ģ������dp_nn_no������Ҫ��дһ������ʵ�ָ���䣬��Ϊ�ײ㲻����  
//	//�����ˣ������������������ڲ��Լ��������������
//	evaluate_seq1 += evaluate_single(nn_number - n, l, s, ext);//��ǰ��һ�㣬���ڵ�nn-n�� ����0��ʼ��
//	/*if (s.length() >= 2 && s.length() <= 5)
//	{
//		cout << s << "    kanzheli---3  " << evaluate_seq1 << endl;
//	}
//	if (s.length() >= 25)
//	{
//		cout << s << "    kanzheli---3   " << evaluate_seq1 << endl;
//	}*/
//	//cout << evaluate_seq1 << "zheglijjk" << endl;
//	return evaluate_seq1;
//
//}

