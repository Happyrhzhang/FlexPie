#include"net_func.h"
#include"cost.h"

//定义
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

//添加中间结果保存，优化搜索
map<string, double> cost_comvalue;
map<string, double> cost_tranvalue;
map<string, double> cost_resetvalue;

//Cpython变量定义
PyObject* pName;
PyObject* pModule;
PyObject* pFunc_comp;
PyObject* pFunc_reset;
PyObject* pFunc_tran;


//字符串转整数
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
//加载模型参数
void loads()
{
	ifstream infile;
	infile.open("ir.txt", ios::in);
	if (!infile.is_open())
	{
		cout << "读取文件失败" << endl;
		return;
	}
	cout << "文件读取成功了！" << endl;
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
//更新序列的函数，更新第i组的第j个序列 为 第i+1组第k个加上当前 划分 或 传输 对应的选择 
void update_seq(int i, int j, int l, int k, string x)
{
	//seq[i][j] = seq[l][k] + x;
	seq[i][j] =  x+ seq[l][k];
	//cout << i << ' ' << j << ' ' << seq[i][j] << ' ' << l << ' ' << k << ' ' << seq[l][k] << endl;
}


//单层评估--适用于无需考虑前后层  行列十字通道均匀
double evaluate_single(int i,int l, string s, int ext)//根据i值，去字符串s中取对应的字符
{//ext用于初始层，若初始层不传输，相当于连续ext+1层没有传输，需要额外计算ext+1份
	double costcom;
	int reset_i;//防止i+1越界
	if (i + 1 >= nn_number)
		reset_i = i;
	else
		reset_i = i + 1;
	int n = s.length();
	int j =  i-(nn_number-n);//正数第i层，对应倒数第nnnumber-1-i层，为j
	char oper = s[j];//从字符串s中取第j个字符，对应第i层的划分策略
	if (oper == 'a')
	{
		//cout <<"查看i值：" << i << endl;
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
			img_h = img_height[i+1];//i与i+1尺寸一样，所以i或i+1都行
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


//单层包含额外计算的评估  第三个参是表示额外计算多少层的量 
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
	int k = i - (nn_number - n);//正数第i层，对应倒数第nnnumber-1-i层，为j
	char oper = s[k];//从字符串s中取第j个字符，对应第i层的划分策略
	//往下遍历j份，根据序列s，计算需要额外计算多少份
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
			img_h1 = (img_height[i] + core - 1) / core + extsize;//这里就应该是i，与extsize对应
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
			img_h = img_height[i + 1];//i与i+1尺寸一样，所以i或i+1都行
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
//注：连续不传输的多层采用的划分方案相同，若不同传输量设为大值，意为不可行
//根据配置获取当前评估  根据给定的序列进行评估 
//(0开始，正数第i层，倒数第nn_number-1-i层==字符串s中的第n-1层  所以nn_number-i=n)




double evaluate_seq(string s, int l, int k)
{

	double evaluate_seq1 = 0;
	evaluate_seq1 += dp_nn[l][k];//第l层计算后传输，往后的评估可以直接加上
	
	//评估 
	int n = s.length();//当前序列长度
	int m = l - nn_number + n;//第l层在序列s中是第l-nn_number+n个;
	//初始第m层计算后传输，第0层计算后传输
	//计算中间不传的额外计算，每往前一层，额外加一层计算 
	int ext = 0;//统计连续多少层没传
	for (int i = m - 1;i > 0;i--)
	{
		//第四个参数对应额外加了几层计算--暂时该参数未使用
		ext += 1;
		evaluate_seq1 += evaluate_comp_extra(nn_number - n + i, l, s, ext);//第i层对应总长的i+nn_number-n
	}

	//计算开始不传输的前一层（第n-1层即第nn_number-1-nn+1层）的传输量并加上 ，包含额外传输
	if (nn_number == n && s[0] >= '1' && s[0] <= '5')//若当前层为第0层，且第0层计算后不传输
	{
		evaluate_seq1 += 0;//初始层也不传输的话，此时无需传输
		ext += 1;//最前面一层不传输，额外计算--需要再多做一层
		//pingbi1:cout<<"            asfffffffffffffffffffffffffffffffff                         " << s << endl;
	}
	else
	{
		if (ext == 0)//没有不传输的，按单传输单reset处理
		{
			evaluate_seq1 += evaluate_tran_single(nn_number - n, s);//设置内部参数
			evaluate_seq1 += evaluate_reset_single(nn_number - n, s) / 1000.0;
		}
		else//额外传输计算
		{
			evaluate_seq1 += evaluate_tran_extra(nn_number - n, l, s);
			evaluate_seq1 += evaluate_reset_extra(nn_number - n, l, s) / 1000.0;
		}
	}

	//计算最前面一层，也就是当前层所需要的计算   
	//这一层是计算后传输的，如果是dp_nn_no调用需要再写一个函数实现该语句，因为首层不传输  
	//不用了！！，单层评估函数内部以及考虑了这种情况  ----后续可以拆分处理，这样有些乱，下面的函数调用可以放入if中区分
	evaluate_seq1 += evaluate_single(nn_number - n, l, s, ext);//最前面一层，对于第nn-n层 （从0开始）
	
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
	//计算最后一层计算耗时
	evaluate_seq=evaluate_single(nn_number - 1, l, s, 0);
	//cout << evaluate_seq << endl;
	ssum += evaluate_seq;
	for (int i = n-2;i >= 0;i--)//n-1最后一层单独处理
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
				//当前层计算+计算后的传输和重排
				evaluate_seq1 = evaluate_single(ii, l, s, 0);
				evaluate_seq2 = evaluate_tran_single(ii,  s);
				evaluate_seq3 = evaluate_reset_single(ii, s)/1000.0;
				evaluate_seq = evaluate_seq1 + evaluate_seq2 + evaluate_seq3;
			}
			else
			{
				//当前层计算+包含额外的计算后传输和重排
				evaluate_seq1= evaluate_single(ii, l, s, 0);
				evaluate_seq2= evaluate_tran_extra (ii, l, s);
				evaluate_seq3= evaluate_reset_extra(ii,l, s) / 1000.0;
				evaluate_seq = evaluate_seq1 + evaluate_seq2 + evaluate_seq3;
			}
			l = ii;//当前层传输，l前移至当前层
		}
		else
		{
			//l不动
			//当前层额外计算，无传输和重排
			evaluate_seq1 = evaluate_comp_extra(ii, l, s, 0);
			evaluate_seq = evaluate_seq1;
		}
		cout << ii << "层： "<<s[i]<<"-计算：  " << evaluate_seq1<<"-传输：  "
			<< evaluate_seq2<<"-reset:  "<< evaluate_seq3<<"-sum:  "<<evaluate_seq<< endl;
		ssum += evaluate_seq;

	}
	cout << ssum << endl;
}


//评估单层 ---分别设计函数进行评估evalute_aa_single
double evaluate_tran_single(int i, string s)
{
	int n = s.length();
	//取新加入头部字符，取下一个字符
	char now = s[0];
	char next = s[1];
	//字符转换--防止不能识别的情况
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
		pianyi = 1;//解决防止i+2>nn_number-1  原来是i+1+1改为i+pianyi
	}
	if (now == 'a' && next == 'a')
	{
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			//这里加了两个1，注意出错，可能会i+2>nn_number-1
			if (img_height[i + pianyi] % 4 == 0)//能被4整除，第i+1层的输出尺寸=第i+1+1层的输入尺寸 
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
			}//确认两个同时给一个传输和一个单传时间差异  暂时按两批数据处理
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
			//tran_time += cost_tran(bandwidth, tran_count);//传输两个批次
		}
		else if (operate[i+1] == "Conv1*1")//stride为1或2相同
		{
			tran_count = 0;
			tran_time += 0;//值为0，判断 不需要传输（好处理），或者直接返回一个空的（不好处理）
		}


		//相邻层行的处理
	}
	else if (now == 'a' && next == 'b')
	{
		//行接列
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_width[i + pianyi] % 4 == 0)//能被4整除，第i+1层的输出尺寸=第i+1+1层的输入尺寸 
			{
				tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 1) / 2 + 1) * input_channel[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
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
			}//确认两个同时给一个传输和一个单传时间差异  暂时按两批数据处理
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
			//tran_time += cost_tran(bandwidth, tran_count);//总共传输三个批次
		}
		else if (operate[i+1] == "Conv1*1")//stride为1或2相同
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
		//行+十字
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
			//tran_time += cost_tran(bandwidth, tran_count);//总共传输2个批次
		}
		else if (operate[i+1] == "Conv1*1")//stride为1或2相同
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
		//行+通道
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
		//行+均匀
		count=1000;
		tran_time += 1000;
	}
	else if (now == 'b' && next == 'a')
	{
		//列+行
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_height[i + pianyi] % 4 == 0)//能被4整除，第i+1层的输出尺寸=第i+1+1层的输入尺寸 
			{
				tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 1) / 2 + 1) * input_channel[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
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
			}//确认两个同时给一个传输和一个单传时间差异  暂时按两批数据处理
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
			//tran_time += cost_tran(bandwidth, tran_count);//总共传输三个批次
		}
		else if (operate[i+1] == "Conv1*1")//stride为1或2相同
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
		//相邻层列的处理
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_width[i + pianyi] % 4 == 0)//能被4整除，第i+1层的输出尺寸=第i+1+1层的输入尺寸 
			{
				tran_count = img_height[i + 1] * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				/*tran_count = img_height[i + 1] * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;*/
				//tran_time += cost_tran(bandwidth, tran_count);
			}//确认两个同时给一个传输和一个单传时间差异  暂时按两批数据处理
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
			//tran_time += cost_tran(bandwidth, tran_count);//传输两个批次
		}
		else if (operate[i+1] == "Conv1*1")//stride为1或2相同
		{
			tran_count = 0;
			tran_time += 0;
		}
	}
	else if (now == 'b' && next == 'c')
	{
		//列+十字
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
		else if (operate[i+1] == "Conv1*1")//stride为1或2相同
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
		//列+通道
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
		//相邻层行的处理
		count=1000;
		tran_time += 1000;
	}
	else if (now == 'c' && next == 'a')
	{
		//十字+行
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_width[i + pianyi] % 4 == 0)//能被4整除，第i+1层的输出尺寸=第i+1+1层的输入尺寸 
			{
				tran_count = ((img_height[i + 1] + 3) / 4 + 1) * ((img_width[i + 1] + 1) / 2) * input_channel[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = img_width[i + 1] * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);

			}//确认两个同时给一个传输和一个单传时间差异  暂时按两批数据处理
			else
			{
				tran_count = ((img_height[i + 1] + 3) / 4 + 1) * ((img_width[i + 1] + 1) / 2) * input_channel[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
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
			tran_count = ((img_height[i + 1] + 3) / 4 + 1) * ((img_width[i + 1] + 1) / 2) * input_channel[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = img_width[i + 1] * input_channel[i + 1];
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
		else if (operate[i+1] == "Conv1*1")//stride为1或2相同
		{
			tran_count = ((img_height[i + 1] + 3) / 4) * ((img_width[i + 1] + 1) / 2) * input_channel[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
	}
	else if (now == 'c' && next == 'b')
	{
		//十字+列
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_width[i + pianyi] % 4 == 0)//能被4整除，第i+1层的输出尺寸=第i+1+1层的输入尺寸 
			{
				tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = img_height[i + 1] * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);

			}//确认两个同时给一个传输和一个单传时间差异  暂时按两批数据处理
			else
			{
				tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
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
			tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
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
		else if (operate[i+1] == "Conv1*1")//stride为1或2相同
		{
			tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
	}
	else if (now == 'c' && next == 'c')
	{
		//相邻层十字划分的处理
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
		else if (operate[i+1] == "Conv1*1")//stride为1或2相同
		{
			tran_count = 0;
			tran_time += 0;
		}
	}
	else if (now == 'c' && next == 'd')
	{
		//十字+通道
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
		//相邻层行的处理
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'd' && next == 'a')
	{
		//通道+行
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_width[i + pianyi] % 4 == 0)//能被4整除，第i+1层的输出尺寸=第i+1+1层的输入尺寸 
			{
				tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 1) / 2 + 1) * img_width[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4 + 1) * img_width[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4 + 1) * img_width[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);

			}//确认两个同时给一个传输和一个单传时间差异  暂时按两批数据处理
			else
			{
				tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 1) / 2 + 2) * img_width[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4 + 2) * img_width[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4 + 2) * img_width[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 1) / 2 + 2) * img_width[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4 + 2) * img_width[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4 + 2) * img_width[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
		else if (operate[i+1] == "Conv1*1")//stride为1或2相同
		{
			tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 1) / 2) * img_width[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4) * img_width[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4) * img_width[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
	}
	else if (now == 'd' && next == 'b')
	{
		//通道+列
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_width[i + pianyi] % 4 == 0)//能被4整除，第i+1层的输出尺寸=第i+1+1层的输入尺寸 
			{
				tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 1) * img_height[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4 + 1) * img_height[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4 + 1) * img_height[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);

			}//确认两个同时给一个传输和一个单传时间差异  暂时按两批数据处理
			else
			{
				tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 2) * img_height[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4 + 2) * img_height[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4 + 2) * img_height[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 2) * img_height[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4 + 2) * img_height[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4 + 2) * img_height[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
		else if (operate[i+1] == "Conv1*1")//stride为1或2相同
		{
			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2) * img_height[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4) * img_height[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4) * img_height[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
	}
	else if (now == 'd' && next == 'c')
	{
		//通道+十字
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 1) * img_height[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 1) * ((img_height[i + 1] + 1) / 2 + 1);//之所以+3和+1是为了取最大的时间作为传输时间
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 1) * ((img_height[i + 1] + 1) / 2 + 1);//之所以+3和+1是为了取最大的时间作为传输时间
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 1) * img_height[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 1) * ((img_height[i + 1] + 1) / 2 + 1);//之所以+3和+1是为了取最大的时间作为传输时间
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 1) * ((img_height[i + 1] + 1) / 2 + 1);//之所以+3和+1是为了取最大的时间作为传输时间
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
		else if (operate[i+1] == "Conv1*1")//stride为1或2相同
		{
			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2) * img_height[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2) * ((img_height[i + 1] + 1) / 2);//之所以+3和+1是为了取最大的时间作为传输时间
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2) * ((img_height[i + 1] + 1) / 2);//之所以+3和+1是为了取最大的时间作为传输时间
			tran_num.push_back(tran_count);
			count++;
			//tran_time += cost_tran(bandwidth, tran_count);
		}
	}
	else if (now == 'd' && next == 'd')
	{
		//相邻层逐通道的处理
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
		else if (operate[i+1] == "DwConv3*3")//stride为1或2相同
		{
			tran_count = 0;
			tran_time += 0;//值为0，判断 不需要传输
			//确认两个同时给一个传输和一个单传实践差异  暂时按两批数据处理
		}
	}
	else if (now == 'd' && next == 'e')
	{
		//相邻层行的处理
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
		//相邻层行的处理
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'e' && next == 'c')
	{
		//相邻层行的处理
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'e' && next == 'd')
	{
		//相邻层行的处理
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'e' && next == 'e')
	{
		//相邻层行的处理
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

//评估单层 ---分别设计函数进行评估evalute_aa_single
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
//			if (img_height[i + 1 + 1] % 4 == 0)//能被4整除，第i+1层的输出尺寸=第i+1+1层的输入尺寸
//			{
//				tran_count = img_width[i + 1] * input_channel[i + 1];
//				tran_num.push_back(tran_count);
//				tran_num.push_back(tran_count);
//			}//确认两个同时给一个传输和一个单传时间差异  暂时按两批数据处理
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
//			tran_num.push_back(tran_count);//传输两个批次
//		}
//		else if (operate[i] == "conv 1*1")//stride为1或2相同
//		{
//			tran_count = 0;
//			tran_num.push_back(tran_count);//值为0，判断 不需要传输（好处理），或者直接返回一个空的（不好处理）
//		}
//		//相邻层行的处理
//	}
//	else if (now == 'a' && next == 'b')
//	{
//		//行接列
//		if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 2)
//		{
//			if (img_width[i + 1 + 1] % 4 == 0)//能被4整除，第i+1层的输出尺寸=第i+1+1层的输入尺寸 
//			{
//				tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 1) / 2 + 1) * input_channel[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//				tran_num.push_back(tran_count);
//				tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 3) / 4 + 1) * input_channel[i + 1];
//				tran_num.push_back(tran_count);
//				tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 3) / 4 + 1) * input_channel[i + 1];
//				tran_num.push_back(tran_count);
//			}//确认两个同时给一个传输和一个单传时间差异  暂时按两批数据处理
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
//			tran_num.push_back(tran_count);//总共传输三个批次
//		}
//		else if (operate[i] == "conv 1*1")//stride为1或2相同
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
//		//行+十字
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
//			tran_num.push_back(tran_count);//总共传输2个批次
//		}
//		else if (operate[i] == "conv 1*1")//stride为1或2相同
//		{
//			tran_count = (img_height[i + 1] + 3) / 4 * ((img_width[i + 1] + 1) / 2) * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = (img_width[i + 1] + 1) / 2 * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//		}
//	}
//	else if (now == 'a' && next == 'd')
//	{
//		//行+通道
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
//		//行+均匀
//	}
//	else if (now == 'b' && next == 'a')
//	{
//		//列+行
//		if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 2)
//		{
//			if (img_height[i + 1 + 1] % 4 == 0)//能被4整除，第i+1层的输出尺寸=第i+1+1层的输入尺寸 
//			{
//				tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 1) / 2 + 1) * input_channel[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//				tran_num.push_back(tran_count);
//				tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 3) / 4 + 1) * input_channel[i + 1];
//				tran_num.push_back(tran_count);
//				tran_count = (img_width[i + 1] + 3) / 4 * ((img_height[i + 1] + 3) / 4 + 1) * input_channel[i + 1];
//				tran_num.push_back(tran_count);
//			}//确认两个同时给一个传输和一个单传时间差异  暂时按两批数据处理
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
//			tran_num.push_back(tran_count);//总共传输三个批次
//		}
//		else if (operate[i] == "conv 1*1")//stride为1或2相同
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
//		//相邻层列的处理
//		if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 2)
//		{
//			if (img_width[i + 1 + 1] % 4 == 0)//能被4整除，第i+1层的输出尺寸=第i+1+1层的输入尺寸 
//			{
//				tran_count = img_height[i + 1] * input_channel[i + 1];
//				tran_num.push_back(tran_count);
//				tran_num.push_back(tran_count);
//			}//确认两个同时给一个传输和一个单传时间差异  暂时按两批数据处理
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
//			tran_num.push_back(tran_count);//传输两个批次
//		}
//		else if (operate[i] == "conv 1*1")//stride为1或2相同
//		{
//			tran_count = 0;
//			tran_num.push_back(tran_count);//值为0，判断 不需要传输（好处理），或者直接返回一个空的（不好处理）
//		}
//	}
//	else if (now == 'b' && next == 'c')
//	{
//		//列+十字
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
//		else if (operate[i] == "conv 1*1")//stride为1或2相同
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
//		//列+通道
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
//		//相邻层行的处理
//	}
//	else if (now == 'c' && next == 'a')
//	{
//		//十字+行
//		if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 2)
//		{
//			if (img_width[i + 1 + 1] % 4 == 0)//能被4整除，第i+1层的输出尺寸=第i+1+1层的输入尺寸 
//			{
//				tran_count = ((img_height[i + 1] + 3) / 4 + 1) * ((img_width[i + 1] + 1) / 2) * input_channel[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//				tran_num.push_back(tran_count);
//				tran_count = img_width[i + 1] * input_channel[i + 1];
//				tran_num.push_back(tran_count);
//
//			}//确认两个同时给一个传输和一个单传时间差异  暂时按两批数据处理
//			else
//			{
//				tran_count = ((img_height[i + 1] + 3) / 4 + 1) * ((img_width[i + 1] + 1) / 2) * input_channel[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//				tran_num.push_back(tran_count);
//				tran_count = img_width[i + 1] * input_channel[i + 1];
//				tran_num.push_back(tran_count);
//			}
//		}
//		else if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 1)
//		{
//			tran_count = ((img_height[i + 1] + 3) / 4 + 1) * ((img_width[i + 1] + 1) / 2) * input_channel[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//			tran_num.push_back(tran_count);
//			tran_count = img_width[i + 1] * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//		}
//		else if (operate[i] == "conv 1*1")//stride为1或2相同
//		{
//			tran_count = ((img_height[i + 1] + 3) / 4) * ((img_width[i + 1] + 1) / 2) * input_channel[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//			tran_num.push_back(tran_count);
//		}
//	}
//	else if (now == 'c' && next == 'b')
//	{
//		//十字+列
//		if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 2)
//		{
//			if (img_width[i + 1 + 1] % 4 == 0)//能被4整除，第i+1层的输出尺寸=第i+1+1层的输入尺寸 
//			{
//				tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//				tran_num.push_back(tran_count);
//				tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//				tran_num.push_back(tran_count);
//				tran_count = img_height[i + 1] * input_channel[i + 1];
//				tran_num.push_back(tran_count);
//
//			}//确认两个同时给一个传输和一个单传时间差异  暂时按两批数据处理
//			else
//			{
//				tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//				tran_num.push_back(tran_count);
//				tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//				tran_num.push_back(tran_count);
//				tran_count = img_height[i + 1] * input_channel[i + 1];
//				tran_num.push_back(tran_count);
//				tran_count = img_height[i + 1] * input_channel[i + 1];
//				tran_num.push_back(tran_count);
//			}
//		}
//		else if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 1)
//		{
//			tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//			tran_num.push_back(tran_count);
//			tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//			tran_num.push_back(tran_count);
//			tran_count = img_height[i + 1] * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//			tran_count = img_height[i + 1] * input_channel[i + 1];
//			tran_num.push_back(tran_count);
//		}
//		else if (operate[i] == "conv 1*1")//stride为1或2相同
//		{
//			tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//			tran_num.push_back(tran_count);
//			tran_count = (img_height[i + 1] + 1) / 2 * ((img_width[i + 1] + 3) / 4) * input_channel[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//			tran_num.push_back(tran_count);
//		}
//	}
//	else if (now == 'c' && next == 'c')
//	{
//		//相邻层十字划分的处理
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
//		else if (operate[i] == "conv 1*1")//stride为1或2相同
//		{
//			tran_count = 0;
//			tran_num.push_back(tran_count);
//		}
//	}
//	else if (now == 'c' && next == 'd')
//	{
//		//十字+通道
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
//		//相邻层行的处理
//	}
//	else if (now == 'd' && next == 'a')
//	{
//		//通道+行
//		if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 2)
//		{
//			if (img_width[i + 1 + 1] % 4 == 0)//能被4整除，第i+1层的输出尺寸=第i+1+1层的输入尺寸 
//			{
//				tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 1) / 2 + 1) * img_width[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//				tran_num.push_back(tran_count);
//				tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4 + 1) * img_width[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//				tran_num.push_back(tran_count);
//				tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4 + 1) * img_width[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//				tran_num.push_back(tran_count);
//
//			}//确认两个同时给一个传输和一个单传时间差异  暂时按两批数据处理
//			else
//			{
//				tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 1) / 2 + 2) * img_width[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//				tran_num.push_back(tran_count);
//				tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4 + 2) * img_width[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//				tran_num.push_back(tran_count);
//				tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4 + 2) * img_width[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//				tran_num.push_back(tran_count);
//			}
//		}
//		else if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 1)
//		{
//			tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 1) / 2 + 2) * img_width[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//			tran_num.push_back(tran_count);
//			tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4 + 2) * img_width[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//			tran_num.push_back(tran_count);
//			tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4 + 2) * img_width[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//			tran_num.push_back(tran_count);
//		}
//		else if (operate[i] == "conv 1*1")//stride为1或2相同
//		{
//			tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 1) / 2) * img_width[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//			tran_num.push_back(tran_count);
//			tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4) * img_width[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//			tran_num.push_back(tran_count);
//			tran_count = input_channel[i + 1] / 4 * ((img_height[i + 1] + 3) / 4) * img_width[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//			tran_num.push_back(tran_count);
//		}
//	}
//	else if (now == 'd' && next == 'b')
//	{
//		//通道+列
//		if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 2)
//		{
//			if (img_width[i + 1 + 1] % 4 == 0)//能被4整除，第i+1层的输出尺寸=第i+1+1层的输入尺寸 
//			{
//				tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 1) * img_height[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//				tran_num.push_back(tran_count);
//				tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4 + 1) * img_height[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//				tran_num.push_back(tran_count);
//				tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4 + 1) * img_height[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//				tran_num.push_back(tran_count);
//
//			}//确认两个同时给一个传输和一个单传时间差异  暂时按两批数据处理
//			else
//			{
//				tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 2) * img_height[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//				tran_num.push_back(tran_count);
//				tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4 + 2) * img_height[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//				tran_num.push_back(tran_count);
//				tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4 + 2) * img_height[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//				tran_num.push_back(tran_count);
//			}
//		}
//		else if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 1)
//		{
//			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 2) * img_height[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//			tran_num.push_back(tran_count);
//			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4 + 2) * img_height[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//			tran_num.push_back(tran_count);
//			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4 + 2) * img_height[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//			tran_num.push_back(tran_count);
//		}
//		else if (operate[i] == "conv 1*1")//stride为1或2相同
//		{
//			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2) * img_height[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//			tran_num.push_back(tran_count);
//			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4) * img_height[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//			tran_num.push_back(tran_count);
//			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 3) / 4) * img_height[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//			tran_num.push_back(tran_count);
//		}
//	}
//	else if (now == 'd' && next == 'c')
//	{
//		//通道+十字
//		if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 2)
//		{
//			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 1) * img_height[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//			tran_num.push_back(tran_count);
//			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 1) * ((img_height[i + 1] + 1) / 2 + 1);//之所以+3和+1是为了取最大的时间作为传输时间
//			tran_num.push_back(tran_count);
//			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 1) * ((img_height[i + 1] + 1) / 2 + 1);//之所以+3和+1是为了取最大的时间作为传输时间
//			tran_num.push_back(tran_count);
//		}
//		else if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "dwconv 3*3") && stride[i + 1] == 1)
//		{
//			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 1) * img_height[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//			tran_num.push_back(tran_count);
//			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 1) * ((img_height[i + 1] + 1) / 2 + 1);//之所以+3和+1是为了取最大的时间作为传输时间
//			tran_num.push_back(tran_count);
//			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2 + 1) * ((img_height[i + 1] + 1) / 2 + 1);//之所以+3和+1是为了取最大的时间作为传输时间
//			tran_num.push_back(tran_count);
//		}
//		else if (operate[i] == "conv 1*1")//stride为1或2相同
//		{
//			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2) * img_height[i + 1];//之所以+3和+1是为了取最大的时间作为传输时间
//			tran_num.push_back(tran_count);
//			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2) * ((img_height[i + 1] + 1) / 2);//之所以+3和+1是为了取最大的时间作为传输时间
//			tran_num.push_back(tran_count);
//			tran_count = input_channel[i + 1] / 4 * ((img_width[i + 1] + 1) / 2) * ((img_height[i + 1] + 1) / 2);//之所以+3和+1是为了取最大的时间作为传输时间
//			tran_num.push_back(tran_count);
//		}
//	}
//	else if (now == 'd' && next == 'd')
//	{
//		//相邻层逐通道的处理
//		if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "conv 1*1"))
//		{
//			tran_count = img_height[i + 1] * img_width[i + 1] * input_channel[i + 1] / 4;
//			tran_num.push_back(tran_count);
//			tran_count = img_height[i + 1] * img_width[i + 1] * input_channel[i + 1] / 4;
//			tran_num.push_back(tran_count);
//			tran_count = img_height[i + 1] * img_width[i + 1] * input_channel[i + 1] / 4;
//			tran_num.push_back(tran_count);
//		}
//		else if (operate[i] == "dwconv 3*3")//stride为1或2相同
//		{
//			tran_count = 0;
//			tran_num.push_back(tran_count);//值为0，判断 不需要传输
//			//确认两个同时给一个传输和一个单传实践差异  暂时按两批数据处理
//		}
//	}
//	else if (now == 'd' && next == 'e')
//	{
//		//相邻层行的处理
//	}
//	else if (now == 'e' && next == 'a')
//	{
//
//	}
//	else if (now == 'e' && next == 'b')
//	{
//		//相邻层行的处理
//	}
//	else if (now == 'e' && next == 'c')
//	{
//		//相邻层行的处理
//	}
//	else if (now == 'e' && next == 'd')
//	{
//		//相邻层行的处理
//	}
//	else if (now == 'e' && next == 'e')
//	{
//		//相邻层行的处理
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
//额外计算加上之后要加个判断，也就是判断是否超出原始未拆分的尺寸
//其实不加也行，这种方案因为加速比已经低于1了，本来就是要舍弃的方案，返回的值也会比较大，会被舍弃

//行、列、十字一样
//不过十字是在两个维度都加exts
/*img_h		+ext
* img_h1	+extsize
* tran		 exttran---传输行数或者列数，或者半行半列数（十字的传输可能复杂一些，细致分析一下设计）
* reset		 与tran对应，分析！
* 
* now对应i
* end对应l
*/

int  compute_layer_extra(int now,int end,string s)//now：s的第一个，即第一个不传输的 end：开始传输的第一个，该位置的前一个为不传输
{
	//double cost_compute = 0.0;
	int n = s.length();
	int nows, ends;
	nows = now - (nn_number - n);
	ends = end - (nn_number - n);
	char cord;
	//记录下最后一层划分方式
	cord=s[ends];
	//转换为无传输格式
	cord = cord - 'a' + '1';
	int ext = 0;
	int exttran = 0;
	//如果首层为计算后传输，首层后移一位
	if (s[0] >= 'a' && s[0] <= 'e')
	{
		nows+=1;
	}
	//for (int i = ends - 1;i >= nows;i--)  11.04改动-1
	//for (int i = ends - 1;i >nows;i--)11.28改动-1
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
				//行划分处理
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
				//十字划分处理
				if (operate[nn_number - n + i + 1] == "DwConv3*3" || operate[nn_number - n + i + 1] == "Conv3*3")
				{
					if (stride[nn_number - n + i + 1] == 1)
					{
						exttran = 1 * ext + 1;//修改+2变+1
						ext = exttran;
					}
					else if (stride[nn_number - n + i + 1] == 2)
					{
						exttran = 2 * ext + 1;//修改+2变+1
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
				//通道划分处理
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
				//均匀划分处理
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
	//nows = now - (nn_number - n);11.04改动-2 是下一层所以加1
	//nows = now - (nn_number - n)+1;//无论首层是计算后传输还是不传输，都是加1，两种场景均适用；不传输时的额外计算外扩尺寸--不对
	//添加判断
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
	//记录下最后一层划分方式
	cord = s[nows];
	int extsize = 0;

	
	if (ext == -1)
	{
		extsize = -1;
	}
	else
	{
		//ext对应外扩的计算量行数，外扩的尺寸对应的extsize---这个也就对应实际外扩后的传输量
		/*extsize = ext * stride[now]+2;
		exttran = extsize+2;*///这一行的加2应该是不用加的
		if (cord == '1')
		{
			//行划分处理
			if (operate[now] == "DwConv3*3" || operate[now] == "Conv3*3")
			{
				if (stride[now] == 1)
				{
					extsize = 1 * ext + 2;//加2是因为除了额外增加的计算部分尺寸，还有它本身需要外扩两行的尺寸
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
					extsize = 1 * ext + 2;//加2是因为除了额外增加的计算部分尺寸，还有它本身需要外扩两行的尺寸
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
					extsize = 1 * ext + 1;//加1是因为除了额外增加的计算部分尺寸，还有它本身需要外扩一行一列的尺寸
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
			//通道划分处理
			if (operate[now] == "DwConv3*3")
			{
				extsize = ext;//此处ext一定是等于0的
			}
			else
			{
				ext = -1;
			}
		}
		else if (cord == '5')
		{
			//均匀划分处理
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
	double value_time =  0.0000189577;//填补单个实值
	int n = s.length();
	char now = s[0];
	char next = s[1];//待修改
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
		pianyi = 1;//解决防止i+2>nn_number-1  原来是i+1+1改为i+pianyi
	}
	if (now == 'a' && next == 'a')
	{
		if ((operate[i + 1] == "conv 3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_height[i + pianyi] % 4 == 0)//能被4整除，第i+1层的输出尺寸=第i+1+1层的输入尺寸
			{
				tran_count = input_channel[i + 1] * cost_reset(img_width[i + 1]);//取
				tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 1) * zero_time;//放 0
				tran_count += 0;//放 单个变量
				tran_count += input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 1) * cost_reset(img_width[i + 1]);//放memcpy
			}
			else
			{
				tran_count = 2 * input_channel[i + 1] * cost_reset(img_width[i + 1]);//取
				tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * zero_time;//放 0
				tran_count += 0;//放 单个变量
				tran_count += input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * cost_reset(img_width[i + 1]);//放memcpy
			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = 2 * input_channel[i + 1] * cost_reset(img_width[i + 1]);//取
			tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * zero_time;//放 0
			tran_count += 0;//放 单个变量
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * cost_reset(img_width[i + 1]);//放memcpy


		}
		else if (operate[i+1] == "conv 1*1")//stride为1或2相同
		{
			tran_count = 0;//值为0，判断 不需要传输（好处理），或者直接返回一个空的（不好处理）
		}
		//相邻层行的处理
	}
	else if (now == 'a' && next == 'b')
	{
		//行接列
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_width[i + pianyi] % 4 == 0)//能被4整除，第i+1层的输出尺寸=第i+1+1层的输入尺寸 
			{
				tran_count = 3 * input_channel[i + 1] * (img_height[i + 1] + 3) / 4 * cost_reset((img_width[i + 1] + 3) / 4 + 1);//取
				tran_count += input_channel[i + 1] * (img_height[i + 1] + 2) * zero_time;//放 0
				tran_count += 0;//放 单个变量
				tran_count += input_channel[i + 1] * (img_height[i + 1] + 2) * cost_reset((img_width[i + 1] + 3) / 4);//放memcpy

			}//确认两个同时给一个传输和一个单传时间差异  暂时按两批数据处理
			else
			{
				tran_count = 3 * input_channel[i + 1] * (img_height[i + 1] + 3) / 4 * cost_reset((img_width[i + 1] + 3) / 4 + 2);//取
				tran_count += input_channel[i + 1] * (img_height[i + 1] + 2) * zero_time;//放 0
				tran_count += 0;//放 单个变量
				tran_count += input_channel[i + 1] * (img_height[i + 1] + 2) * cost_reset((img_width[i + 1] + 3) / 4 + 1);//放memcpy

			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = 3 * input_channel[i + 1] * (img_height[i + 1] + 3) / 4 * cost_reset((img_width[i + 1] + 3) / 4 + 2);//取
			tran_count += input_channel[i + 1] * (img_height[i + 1] + 2) * zero_time;//放 0
			tran_count += 0;//放 单个变量
			tran_count += input_channel[i + 1] * (img_height[i + 1] + 2) * cost_reset((img_width[i + 1] + 3) / 4 + 1);//放memcpy

		}
		else if (operate[i+1] == "Conv1*1")//stride为1或2相同
		{
			tran_count = 3 * input_channel[i + 1] * (img_height[i + 1] + 3) / 4 * cost_reset((img_width[i + 1] + 3) / 4);//取
			tran_count += 0;//放 0
			tran_count += 0;//放 单个变量
			tran_count += 4 * input_channel[i + 1] * cost_reset((img_height[i + 1] + 3) / 4 * (img_width[i + 1] + 3) / 4);//放memcpy

		}
	}
	else if (now == 'a' && next == 'c')
	{
		//行+十字
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			tran_count = input_channel[i + 1] * ((img_height[i + 1] + 3) / 4) * cost_reset((img_width[i + 1] + 1) / 2 + 1) +
				input_channel[i + 1] * cost_reset((img_width[i + 1] + 1) / 2 + 1);//取
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 2) * zero_time;//放 0
			tran_count += 0;//放 单个变量
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 2) * cost_reset((img_width[i + 1] + 1) / 2 + 1);//放memcpy

		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = input_channel[i + 1] * ((img_height[i + 1] + 3) / 4) * cost_reset((img_width[i + 1] + 1) / 2 + 1) +
				input_channel[i + 1] * cost_reset((img_width[i + 1] + 1) / 2 + 1);//取
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 2) * zero_time;//放 0
			tran_count += 0;//放 单个变量
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 2) * cost_reset((img_width[i + 1] + 1) / 2 + 1);//放memcpy


		}
		else if (operate[i+1] == "Conv1*1")//stride为1或2相同
		{
			tran_count = input_channel[i + 1] * ((img_height[i + 1] + 3) / 4) * cost_reset((img_width[i + 1] + 1) / 2);//取
			tran_count += 0;//放 0
			tran_count += 0;//放 单个变量
			tran_count += 2 * input_channel[i + 1] * cost_reset((img_height[i + 1] + 3) / 4 * (img_width[i + 1] + 1) / 2);//放memcpy
		}
	}
	else if (now == 'a' && next == 'd')
	{
		//行+通道
		if (operate[i + 1] == "DwConv3*3")
		{
			tran_count = 0;//取
			tran_count += 2 * (img_height[i + 1] + 2) * input_channel[i + 1] / 4 * zero_time;//放 0
			tran_count += 0;//放 单个变量
			tran_count += input_channel[i + 1] / 4 * (img_height[i + 1] + 2) * cost_reset(img_width[i + 1]);//放memcpy
			//cout <<"   audada hhdjdajks d   " << tran_count << endl;

		}
		else if (operate[i + 1] == "Conv1*1")
		{
			tran_count = 0;//取
			tran_count += 0 * zero_time;//放 0
			tran_count += 0;//放 单个变量
			tran_count += 4 * input_channel[i + 1] * cost_reset((img_height[i + 1] + 3) / 4 * img_width[i + 1]);//放memcpy

		}
		else
		{
			tran_count = 0;//取
			tran_count += 2 * (img_height[i + 1] + 2) * input_channel[i + 1] * zero_time;//放 0
			tran_count += 0;//放 单个变量
			tran_count += input_channel[i + 1] * (img_height[i + 1] + 2) * cost_reset(img_width[i + 1]);//放memcpy

		}
	}
	else if (now == 'a' && next == 'e')
	{
		//行+均匀
	}
	else if (now == 'b' && next == 'a')
	{
		//列+行
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_height[i + pianyi] % 4 == 0)//能被4整除，第i+1层的输出尺寸=第i+1+1层的输入尺寸
			{
				tran_count = 3 * input_channel[i + 1] * cost_reset(((img_height[i + 1] + 3) / 4 + 1) * (img_width[i + 1] + 3) / 4);//取
				tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 1) * zero_time;//放 0
				tran_count += 0;//放 单个变量
				tran_count += 4 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 1) * cost_reset((img_width[i + 1] + 3) / 4);//放memcpy
			}
			else
			{
				tran_count = 3 * input_channel[i + 1] * cost_reset(((img_height[i + 1] + 3) / 4 + 2) * (img_width[i + 1] + 3) / 4);//取
				tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * zero_time;//放 0
				tran_count += 0;//放 单个变量
				tran_count += 4 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * cost_reset((img_width[i + 1] + 3) / 4);//放memcpy

			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = 3 * input_channel[i + 1] * cost_reset(((img_height[i + 1] + 3) / 4 + 2) * (img_width[i + 1] + 3) / 4);//取
			tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * zero_time;//放 0
			tran_count += 0;//放 单个变量
			tran_count += 4 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * cost_reset((img_width[i + 1] + 3) / 4);//放memcpy


		}
		else if (operate[i+1] == "Conv1*1")//stride为1或2相同
		{
			tran_count = 3 * input_channel[i + 1] * cost_reset((img_height[i + 1] + 3) / 4 * (img_width[i + 1] + 3) / 4);//取
			tran_count += 0 * zero_time;//放 0
			tran_count += 0;//放 单个变量
			tran_count += 4 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4) * cost_reset((img_width[i + 1] + 3) / 4);//放memcpy

		}
	}
	else if (now == 'b' && next == 'b')
	{
		//相邻层列的处理
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_width[i + pianyi] % 4 == 0)//能被4整除，第i+1层的输出尺寸=第i+1+1层的输入尺寸 
			{
				tran_count = input_channel[i + 1] * img_height[i + 1] * value_time;;//取
				tran_count += 0 * zero_time;//放 0
				tran_count += input_channel[i + 1] * img_height[i + 1] * value_time;//放 单个变量
				tran_count += input_channel[i + 1] * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4)
					+ 2 * input_channel[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + 1);//放memcpy

			}//确认两个同时给一个传输和一个单传时间差异  暂时按两批数据处理
			else
			{
				tran_count = 2 * input_channel[i + 1] * img_height[i + 1] * value_time;;//取
				tran_count += 0 * zero_time;//放 0
				tran_count += 2 * input_channel[i + 1] * img_height[i + 1] * value_time;//放 单个变量
				tran_count += input_channel[i + 1] * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4)
					+ 2 * input_channel[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + 2);//放memcpy

			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = 2 * input_channel[i + 1] * img_height[i + 1] * value_time;;//取
			tran_count += 0 * zero_time;//放 0
			tran_count += 2 * input_channel[i + 1] * img_height[i + 1] * value_time;//放 单个变量
			tran_count += input_channel[i + 1] * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4)
				+ 2 * input_channel[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + 2);//放memcpy

		}
		else if (operate[i+1] == "Conv1*1")//stride为1或2相同
		{
			tran_count = 0;//取
			tran_count += 0;//放 0
			tran_count += 0;//放 单个变量
			tran_count += 0;//放memcpy

		}
	}
	else if (now == 'b' && next == 'c')
	{
		//列+十字
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			tran_count = 2 * input_channel[i + 1] * cost_reset(((img_height[i + 1] + 1) / 2) * (img_width[i + 1] + 3) / 4) +
				input_channel[i + 1] * cost_reset((img_width[i + 1] + 1) / 2) +
				input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * value_time;//取
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 2 + 1) * zero_time;//放 0
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * value_time;//放 单个变量
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 2) * cost_reset((img_width[i + 1] + 1) / 2);//放memcpy

		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = 2 * input_channel[i + 1] * cost_reset(((img_height[i + 1] + 1) / 2) * (img_width[i + 1] + 3) / 4) +
				input_channel[i + 1] * cost_reset((img_width[i + 1] + 1) / 2) +
				input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * value_time;//取
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 2 + 1) * zero_time;//放 0
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * value_time;//放 单个变量
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 2) * cost_reset((img_width[i + 1] + 1) / 2);//放memcpy
		}
		else if (operate[i+1] == "Conv1*1")//stride为1或2相同
		{
			tran_count = 2 * input_channel[i + 1] * cost_reset(((img_height[i + 1] + 1) / 2) * (img_width[i + 1] + 3) / 4);//取
			tran_count += 0;//放 0
			tran_count += 0;//放 单个变量
			tran_count += 2 * input_channel[i + 1] * (img_height[i + 1] + 1) / 2 * cost_reset((img_width[i + 1] + 3) / 4);//放memcpy
		}
	}
	else if (now == 'b' && next == 'd')
	{
		//列+通道
		if (operate[i + 1] == "DwConv3*3")
		{
			tran_count = 0;//取
			tran_count += 2 * (img_height[i + 1] + 2) * input_channel[i + 1] / 4 * zero_time;//放 0
			tran_count += 0;//放 单个变量
			tran_count += 4 * input_channel[i + 1] / 4 * (img_height[i + 1] + 2) * cost_reset((img_width[i + 1] + 3 / 4));//放memcpy

		}
		else if (operate[i + 1] == "Conv1*1")
		{
			tran_count = 0;//取
			tran_count += 0 * zero_time;//放 0
			tran_count += 0;//放 单个变量
			tran_count += 4 * input_channel[i + 1] * (img_height[i + 1]) * cost_reset((img_width[i + 1] + 3 / 4));//放memcpy

		}
		else
		{
			tran_count = 0;//取
			tran_count += 2 * (img_height[i + 1] + 2) * input_channel[i + 1] * zero_time;//放 0
			tran_count += 0;//放 单个变量
			tran_count += 4 * input_channel[i + 1] * (img_height[i + 1] + 2) * cost_reset((img_width[i + 1] + 3 / 4));//放memcpy

		}
	}
	else if (now == 'b' && next == 'e')
	{
		//相邻层行的处理
	}
	else if (now == 'c' && next == 'a')
	{
		//十字+行
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_height[i + pianyi] % 4 == 0)//能被4整除，第i+1层的输出尺寸=第i+1+1层的输入尺寸
			{
				tran_count = input_channel[i + 1] * cost_reset(((img_height[i + 1] + 3) / 4 + 1) * (img_width[i + 1] + 1) / 2)
					+ input_channel[i + 1] * cost_reset(img_width[i + 1]);//取
				tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 1) * zero_time;//放 0
				tran_count += 0;//放 单个变量
				tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 1) * cost_reset((img_width[i + 1] + 1) / 2);//放memcpy}
			}
			else
			{
				tran_count = input_channel[i + 1] * cost_reset(((img_height[i + 1] + 3) / 4 + 1) * (img_width[i + 1] + 1) / 2)
					+ input_channel[i + 1] * cost_reset(img_width[i + 1]);//取
				tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * zero_time;//放 0
				tran_count += 0;//放 单个变量
				tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * cost_reset((img_width[i + 1] + 1) / 2);//放memcpy
			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = input_channel[i + 1] * cost_reset(((img_height[i + 1] + 3) / 4 + 1) * (img_width[i + 1] + 1) / 2)
				+ input_channel[i + 1] * cost_reset(img_width[i + 1]);//取
			tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * zero_time;//放 0
			tran_count += 0;//放 单个变量
			tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * cost_reset((img_width[i + 1] + 1) / 2);//放memcpy


		}
		else if (operate[i+1] == "Conv1*1")//stride为1或2相同
		{
			tran_count = input_channel[i + 1] * cost_reset((img_height[i + 1] + 3) / 4 * (img_width[i + 1] + 1) / 2);//取
			tran_count += 0 * zero_time;//放 0
			tran_count += 0;//放 单个变量
			tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4) * cost_reset((img_width[i + 1] + 1) / 2);//放memcpy

		}

	}
	else if (now == 'c' && next == 'b')
	{
		//相邻层行的处理
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_width[i + pianyi] % 4 == 0)//能被4整除，第i+1层的输出尺寸=第i+1+1层的输入尺寸 
			{
				tran_count = 2 * input_channel[i + 1] * (img_height[i + 1] + 1) / 2 * cost_reset((img_width[i + 1] + 3) / 4) +
					1 * input_channel[i + 1] * img_height[i + 1] * value_time;//取
				tran_count += 0;//放 0
				tran_count += 1 * input_channel[i + 1] * img_height[i + 1] * value_time;;//放 单个变量
				tran_count += input_channel[i + 1] * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4)
					+ 2 * input_channel[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + 1);//放memcpy

			}//确认两个同时给一个传输和一个单传时间差异  暂时按两批数据处理
			else
			{
				tran_count = 2 * input_channel[i + 1] * (img_height[i + 1] + 1) / 2 * cost_reset((img_width[i + 1] + 3) / 4) +
					2 * input_channel[i + 1] * img_height[i + 1] * value_time;//取
				tran_count += 0;//放 0
				tran_count += 2 * input_channel[i + 1] * img_height[i + 1] * value_time;;//放 单个变量
				tran_count += input_channel[i + 1] * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4)
					+ 2 * input_channel[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + 2);//放memcpy
			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = 2 * input_channel[i + 1] * (img_height[i + 1] + 1) / 2 * cost_reset((img_width[i + 1] + 3) / 4) +
				2 * input_channel[i + 1] * img_height[i + 1] * value_time;//取
			tran_count += 0;//放 0
			tran_count += 2 * input_channel[i + 1] * img_height[i + 1] * value_time;;//放 单个变量
			tran_count += input_channel[i + 1] * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4)
				+ 2 * input_channel[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + 2);//放memcpy
		}
		else if (operate[i+1] == "Conv1*1")//stride为1或2相同
		{
			tran_count = 2 * input_channel[i + 1] * (img_height[i + 1] + 1) / 2 * cost_reset((img_width[i + 1] + 3) / 4);//取
			tran_count += 0;//放 0
			tran_count += 0;//放 单个变量
			tran_count += 2 * input_channel[i + 1] * cost_reset((img_height[i + 1] + 1) / 2 * (img_width[i + 1] + 3) / 4);//放memcpy

		}
	}
	else if (now == 'c' && next == 'c')
	{
		//相邻层十字划分的处理
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			tran_count = input_channel[i + 1] * cost_reset((img_width[i + 1] + 1) / 2) +
				input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * value_time;//取
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 2 + 1) * zero_time;//放 0
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * value_time;//放 单个变量
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 2) * cost_reset((img_width[i + 1] + 1) / 2);//放memcpy

		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = input_channel[i + 1] * cost_reset((img_width[i + 1] + 1) / 2) +
				input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * value_time;//取
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 2 + 1) * zero_time;//放 0
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * value_time;//放 单个变量
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 2) * cost_reset((img_width[i + 1] + 1) / 2);//放memcpy
		}
		else if (operate[i+1] == "Conv1*1")//stride为1或2相同
		{
			tran_count = 0;//取
			tran_count += 0;//放 0
			tran_count += 0;//放 单个变量
			tran_count += 0;//放memcpy
		}
	}
	else if (now == 'c' && next == 'd')
	{
		//十字+通道
		if (operate[i + 1] == "DwConv3*3")
		{
			tran_count = 0;//取
			tran_count += 2 * (img_height[i + 1] + 2) * input_channel[i + 1] / 4 * zero_time;//放 0
			tran_count += 0;//放 单个变量
			tran_count += 2 * input_channel[i + 1] / 4 * (img_height[i + 1] + 2) * cost_reset((img_width[i + 1] + 1 / 2));//放memcpy

		}
		else if (operate[i + 1] == "Conv1*1")
		{
			tran_count = 0;//取
			tran_count += 0 * zero_time;//放 0
			tran_count += 0;//放 单个变量
			tran_count += 2 * input_channel[i + 1] * (img_height[i + 1]) * cost_reset((img_width[i + 1] + 1 / 2));//放memcpy

		}
		else
		{
			tran_count = 0;//取
			tran_count += 2 * (img_height[i + 1] + 2) * input_channel[i + 1] * zero_time;//放 0
			tran_count += 0;//放 单个变量
			tran_count += 2 * input_channel[i + 1] * (img_height[i + 1] + 2) * cost_reset((img_width[i + 1] + 1 / 2));//放memcpy

		}
	}
	else if (now == 'c' && next == 'e')
	{
		//相邻层行的处理
	}
	else if (now == 'd' && next == 'a')
	{
		//相邻层行的处理
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_height[i + pianyi] % 4 == 0)//能被4整除，第i+1层的输出尺寸=第i+1+1层的输入尺寸
			{
				tran_count = 3 * input_channel[i + 1] / 4 * cost_reset(((img_height[i + 1] + 3) / 4 + 1) * img_width[i + 1]);//取
				tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 1) * zero_time;//放 0
				tran_count += 0;//放 单个变量
				tran_count += input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 1) * cost_reset(img_width[i + 1]);//放memcpy
			}
			else
			{
				tran_count = 3 * input_channel[i + 1] / 4 * cost_reset(((img_height[i + 1] + 3) / 4 + 2) * img_width[i + 1]);//取
				tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * zero_time;//放 0
				tran_count += 0;//放 单个变量
				tran_count += input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * cost_reset(img_width[i + 1]);//放memcpy
			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = 3 * input_channel[i + 1] / 4 * cost_reset(((img_height[i + 1] + 3) / 4 + 2) * img_width[i + 1]);//取
			tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * zero_time;//放 0
			tran_count += 0;//放 单个变量
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + 2) * cost_reset(img_width[i + 1]);//放memcpy
		}
		else if (operate[i+1] == "Conv1*1")//stride为1或2相同
		{
			tran_count = 3 * input_channel[i + 1] / 4 * cost_reset((img_height[i + 1] + 3) / 4 * img_width[i + 1]);//取
			tran_count += 0 * zero_time;//放 0
			tran_count += 0;//放 单个变量
			tran_count += 0;//放memcpy

		}
	}
	else if (now == 'd' && next == 'b')
	{
		//通道+列的处理
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_width[i + pianyi] % 4 == 0)//能被4整除，第i+1层的输出尺寸=第i+1+1层的输入尺寸 
			{
				tran_count = 3 * input_channel[i + 1] / 4 * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + 1);//取
				tran_count += input_channel[i + 1] * (img_height[i + 1]) * zero_time;//放 0
				tran_count += 0;//放 单个变量
				tran_count += input_channel[i + 1] * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4)
					+ 2 * input_channel[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + 1);//放memcpy

			}//确认两个同时给一个传输和一个单传时间差异  暂时按两批数据处理
			else
			{
				tran_count = 3 * input_channel[i + 1] / 4 * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + 2);//取
				tran_count += input_channel[i + 1] * (img_height[i + 1]) * zero_time;//放 0
				tran_count += 0;//放 单个变量
				tran_count += input_channel[i + 1] * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + 1)
					+ 2 * input_channel[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + 2);//放memcpy

			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = 3 * input_channel[i + 1] / 4 * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + 2);//取
			tran_count += input_channel[i + 1] * (img_height[i + 1]) * zero_time;//放 0
			tran_count += 0;//放 单个变量
			tran_count += input_channel[i + 1] * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + 1)
				+ 2 * input_channel[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + 2);//放memcpy

		}
		else if (operate[i+1] == "Conv1*1")//stride为1或2相同
		{
			tran_count = 3 * input_channel[i + 1] / 4 * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4);//取
			tran_count += 0 * zero_time;//放 0
			tran_count += 0;//放 单个变量
			tran_count += 0;//放memcpy

		}
	}
	else if (now == 'd' && next == 'c')
	{
		//通道+十字
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			tran_count = input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * cost_reset((img_width[i + 1] + 1) / 2 + 1) +
				input_channel[i + 1] * ((img_height[i + 1] + 1) / 2) * cost_reset((img_width[i + 1] + 1) / 2 + 1) +
				input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * cost_reset((img_width[i + 1] + 1) / 2);//取
			tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 2) * zero_time;//放 0
			tran_count += 0 * value_time;//放 单个变量
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 2) * cost_reset((img_width[i + 1] + 1) / 2);//放memcpy

		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = 3 * input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * cost_reset((img_width[i + 1] + 1) / 2 + 1);//取
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * zero_time;//放 0
			tran_count += 0 * value_time;//放 单个变量
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * cost_reset((img_width[i + 1] + 1) / 2 + 1) +
				input_channel[i + 1] * ((img_width[i + 1] + 1) / 2 + 2);//放memcpy
		}
		else if (operate[i+1] == "Conv1*1")//stride为1或2相同
		{
			tran_count = 3 * input_channel[i + 1] * ((img_height[i + 1] + 1) / 2) * cost_reset((img_width[i + 1] + 1) / 2);//取
			tran_count += 0;//放 0
			tran_count += 0;//放 单个变量
			tran_count += 0;//放memcpy
		}
	}
	else if (now == 'd' && next == 'd')
	{
		//相邻层逐通道的处理
		if (operate[i + 1] == "DwConv3*3")
		{
			tran_count = 0;//取
			tran_count += 2 * (img_height[i + 1] + 2) * input_channel[i + 1] / 4 * zero_time;//放 0
			tran_count += 0;//放 单个变量
			tran_count += input_channel[i + 1] / 4 * (img_height[i + 1] + 2) * cost_reset(img_width[i + 1]);//放memcpy

		}
		else if (operate[i + 1] == "Conv1*1")
		{
			tran_count = 0;//取
			tran_count += 0;//放 0
			tran_count += 0;//放 单个变量
			tran_count += 0;//放memcpy

		}
		else
		{
			tran_count = 0;//取
			tran_count += 2 * (img_height[i + 1] + 2) * input_channel[i + 1] * zero_time;//放 0
			tran_count += 0;//放 单个变量
			tran_count += input_channel[i + 1] * (img_height[i + 1] + 2) * cost_reset(img_width[i + 1]);//放memcpy

		}
	}
	else if (now == 'd' && next == 'e')
	{
		//相邻层行的处理
	}
	else if (now == 'e' && next == 'a')
	{
		//相邻层行的处理
	}
	else if (now == 'e' && next == 'b')
	{
		//相邻层行的处理
	}
	else if (now == 'e' && next == 'c')
	{
		//相邻层行的处理
	}
	else if (now == 'e' && next == 'd')
	{
		//相邻层行的处理
	}
	else if (now == 'e' && next == 'e')
	{
		//相邻层行的处理
	}
	return tran_count;
	//return 0;
}

//评估包含额外的传输 ---分别设计函数进行评估evalute_aa_extra
double evaluate_tran_extra(int i, int l, string s)
{
	int ext= tran_layer_extra(i, l, s);
	int n = s.length();
	char now = s[0];
	char next = s[1];
	int pianyi = 2;
	if (i >= 25)
	{
		pianyi = 1;//解决防止i+2>nn_number-1  原来是i+1+1改为i+pianyi 不会影响结果因为25层以后尺寸不变
	}
	//转换成统一格式
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
			//这里加了两个1，注意出错，可能会i+2>nn_number-1
			if (img_height[i + pianyi] % 4 == 0)//能被4整除，第i+1层的输出尺寸=第i+1+1层的输入尺寸 
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
			}//确认两个同时给一个传输和一个单传时间差异  暂时按两批数据处理
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
			//tran_time += cost_tran(bandwidth, tran_count);//传输两个批次
		}
		else if (operate[i + 1] == "Conv1*1")//stride为1或2相同
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
			//tran_time += 0;//值为0，判断 不需要传输（好处理），或者直接返回一个空的（不好处理）
		}


		//相邻层行的处理
	}
	else if (now == 'a' && next == 'b')
	{
		//行接列
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'a' && next == 'c')
	{
		//行+十字
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'a' && next == 'd')
	{
		//行+通道
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'a' && next == 'e')
	{
		//行+均匀
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'b' && next == 'a')
	{
		//列+行
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'b' && next == 'b')
	{
		//相邻层列的处理
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_width[i + pianyi] % 4 == 0)//能被4整除，第i+1层的输出尺寸=第i+1+1层的输入尺寸 
			{
				tran_count = (ext / 2 - 1)* img_height[i + 1] * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				tran_count = ext / 2* img_height[i + 1] * input_channel[i + 1];
				tran_num.push_back(tran_count);
				count++;
				//tran_time += cost_tran(bandwidth, tran_count);
				
				//tran_time += cost_tran(bandwidth, tran_count);
			}//确认两个同时给一个传输和一个单传时间差异  暂时按两批数据处理
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
			//tran_time += cost_tran(bandwidth, tran_count);//传输两个批次
		}
		else if (operate[i + 1] == "Conv1*1")//stride为1或2相同
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
		//列+十字
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'b' && next == 'd')
	{
		//列+通道
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'b' && next == 'e')
	{
		//相邻层行的处理
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'c' && next == 'a')
	{
		//十字+行
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'c' && next == 'b')
	{
		//十字+列
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'c' && next == 'c')
	{
		//相邻层十字划分的处理
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
		else if (operate[i + 1] == "Conv1*1")//stride为1或2相同
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
		//十字+通道
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'c' && next == 'e')
	{
		//相邻层行的处理
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'd' && next == 'a')
	{
		//通道+行
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'd' && next == 'b')
	{
		//通道+列
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'd' && next == 'c')
	{
		//通道+十字
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'd' && next == 'd')
	{
		//相邻层逐通道的处理
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
		else if (operate[i + 1] == "DwConv3*3")//stride为1或2相同
		{
			tran_count = 0;
			tran_time += 0;//值为0，判断 不需要传输
			//确认两个同时给一个传输和一个单传实践差异  暂时按两批数据处理
		}
	}
	else if (now == 'd' && next == 'e')
	{
		//相邻层行的处理
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
		//相邻层行的处理
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'e' && next == 'c')
	{
		//相邻层行的处理
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'e' && next == 'd')
	{
		//相邻层行的处理
		count = 1000;
		tran_time += 1000;
	}
	else if (now == 'e' && next == 'e')
	{
		//相邻层行的处理
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
	double value_time = 0.0000189577;//填补单个实值
	int n = s.length();
	char now = s[0];
	char next = s[1];//待修改
	int pianyi = 2;
	if (i >= 25)
	{
		pianyi = 1;//解决防止i+2>nn_number-1  原来是i+1+1改为i+pianyi
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
			if (img_height[i + pianyi] % 4 == 0)//能被4整除，第i+1层的输出尺寸=第i+1+1层的输入尺寸
			{
				tran_count = (ext-1)*input_channel[i + 1] * cost_reset(img_width[i + 1]);//取
				tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + ext-1) * zero_time;//放 0
				tran_count += 0;//放 单个变量
				tran_count += input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + ext-1) * cost_reset(img_width[i + 1]);//放memcpy
			}
			else
			{
				tran_count = ext * input_channel[i + 1] * cost_reset(img_width[i + 1]);//取
				tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + ext) * zero_time;//放 0
				tran_count += 0;//放 单个变量
				tran_count += input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + ext) * cost_reset(img_width[i + 1]);//放memcpy
			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = 2 * input_channel[i + 1] * cost_reset(img_width[i + 1]);//取
			tran_count += 2 * input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + ext) * zero_time;//放 0
			tran_count += 0;//放 单个变量
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + ext) * cost_reset(img_width[i + 1]);//放memcpy


		}
		else if (operate[i + 1] == "conv 1*1")//stride为1或2相同
		{
			//tran_count = 0;//值为0，判断 不需要传输（好处理），或者直接返回一个空的（不好处理）
			tran_count = ext * input_channel[i + 1] * cost_reset(img_width[i + 1]);//取
			tran_count += 0;//放 0
			tran_count += 0;//放 单个变量
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 3) / 4 + ext) * cost_reset( img_width[i + 1]);
		}
		//相邻层行的处理
	}
	else if (now == 'a' && next == 'b')
	{
		//行接列
		tran_count = 1000;
	}
	else if (now == 'a' && next == 'c')
	{
		//行+十字
		tran_count = 1000;
	}
	else if (now == 'a' && next == 'd')
	{
		//行+通道
		tran_count = 1000;
	}
	else if (now == 'a' && next == 'e')
	{
		//行+均匀
		tran_count = 1000;
	}
	else if (now == 'b' && next == 'a')
	{
		//列+行
		tran_count = 1000;
	}
	else if (now == 'b' && next == 'b')
	{
		//相邻层列的处理
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			if (img_width[i + pianyi] % 4 == 0)//能被4整除，第i+1层的输出尺寸=第i+1+1层的输入尺寸 
			{
				tran_count = (ext - 1) *input_channel[i + 1] * img_height[i + 1] * value_time;;//取
				tran_count += 0 * zero_time;//放 0
				tran_count += (ext - 1) *input_channel[i + 1] * img_height[i + 1] * value_time;//放 单个变量
				tran_count += input_channel[i + 1] * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4)
					+ 2 * input_channel[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + ext-1);//放memcpy

			}//确认两个同时给一个传输和一个单传时间差异  暂时按两批数据处理
			else
			{
				tran_count = ext * input_channel[i + 1] * img_height[i + 1] * value_time;;//取
				tran_count += 0 * zero_time;//放 0
				tran_count += ext * input_channel[i + 1] * img_height[i + 1] * value_time;//放 单个变量
				tran_count += input_channel[i + 1] * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4)
					+ 2 * input_channel[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 +ext);//放memcpy

			}
		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = ext * input_channel[i + 1] * img_height[i + 1] * value_time;;//取
			tran_count += 0 * zero_time;//放 0
			tran_count += ext * input_channel[i + 1] * img_height[i + 1] * value_time;//放 单个变量
			tran_count += input_channel[i + 1] * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4)
				+ 2 * input_channel[i + 1] * cost_reset((img_width[i + 1] + 3) / 4 + ext);//放memcpy

		}
		else if (operate[i + 1] == "Conv1*1")//stride为1或2相同
		{
			//tran_count = 0;//取
			//tran_count += 0;//放 0
			//tran_count += 0;//放 单个变量
			//tran_count += 0;//放memcpy
			tran_count = ext * input_channel[i + 1] * img_height[i + 1] * value_time;;//取
			tran_count += 0 * zero_time;//放 0
			tran_count += ext * input_channel[i + 1] * img_height[i + 1] * value_time;//放 单个变量
			tran_count += input_channel[i + 1] * img_height[i + 1] * cost_reset((img_width[i + 1] + 3) / 4);//放memcpy

		}
	}
	else if (now == 'b' && next == 'c')
	{
		//列+十字
		tran_count = 1000;
	}
	else if (now == 'b' && next == 'd')
	{
		//列+通道
		tran_count = 1000;
	}
	else if (now == 'b' && next == 'e')
	{
		//相邻层行的处理
		tran_count = 1000;
	}
	else if (now == 'c' && next == 'a')
	{
		//十字+行
		tran_count = 1000;

	}
	else if (now == 'c' && next == 'b')
	{
		//相邻层行的处理
		tran_count = 1000;
	}
	else if (now == 'c' && next == 'c')
	{
		//相邻层十字划分的处理
		if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 2)
		{
			tran_count = ext*input_channel[i + 1] * cost_reset((img_width[i + 1] + 1) / 2) +
				ext*input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * value_time;//取
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + ext) * zero_time;//放 0
			//tran_count += ext*input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + ext) * value_time;//放 单个变量
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + ext) * value_time;//放 单个变量
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1+ext) * cost_reset((img_width[i + 1] + 1) / 2);//放memcpy

		}
		else if ((operate[i + 1] == "Conv3*3" || operate[i + 1] == "DwConv3*3") && stride[i + 1] == 1)
		{
			tran_count = ext*input_channel[i + 1] * cost_reset((img_width[i + 1] + 1) / 2) +
				ext*input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * value_time;//取
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 +ext+ 1) * zero_time;//放 0
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + ext) * value_time;//放 单个变量
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1+ext) * cost_reset((img_width[i + 1] + 1) / 2);//放memcpy
		}
		else if (operate[i + 1] == "Conv1*1")//stride为1或2相同
		{
			//tran_count = 0;//取
			//tran_count += 0;//放 0
			//tran_count += 0;//放 单个变量
			//tran_count += 0;//放memcpy
			tran_count = ext * input_channel[i + 1] * cost_reset((img_width[i + 1] + 1) / 2) +
				ext * input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + 1) * value_time;//取
			tran_count += 0;//放 0
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + ext) * value_time;//放 单个变量
			tran_count += input_channel[i + 1] * ((img_height[i + 1] + 1) / 2 + ext) * cost_reset((img_width[i + 1] + 1) / 2);//放memcpy
		}
	}
	else if (now == 'c' && next == 'd')
	{
		//十字+通道
		tran_count = 1000;
	}
	else if (now == 'c' && next == 'e')
	{
		//相邻层行的处理
		tran_count = 1000;
	}
	else if (now == 'd' && next == 'a')
	{
		//相邻层行的处理
		tran_count = 1000;
	}
	else if (now == 'd' && next == 'b')
	{
		//通道+列的处理
		tran_count = 1000;
	}
	else if (now == 'd' && next == 'c')
	{
		//通道+十字
		tran_count = 1000;
	}
	else if (now == 'd' && next == 'd')
	{
		//相邻层逐通道的处理
		if (operate[i + 1] == "DwConv3*3")
		{
			tran_count = 0;//取
			tran_count += 2 * (img_height[i + 1] + 2) * input_channel[i + 1] / 4 * zero_time;//放 0
			tran_count += 0;//放 单个变量
			tran_count += input_channel[i + 1] / 4 * (img_height[i + 1] + 2) * cost_reset(img_width[i + 1]);//放memcpy

		}
		else if (operate[i + 1] == "Conv1*1")
		{
			tran_count = 0;//取
			tran_count += 0;//放 0
			tran_count += 0;//放 单个变量
			tran_count += 0;//放memcpy

		}
		else
		{
			tran_count = 0;//取
			tran_count += 2 * (img_height[i + 1] + 2) * input_channel[i + 1] * zero_time;//放 0
			tran_count += 0;//放 单个变量
			tran_count += input_channel[i + 1] * (img_height[i + 1] + 2) * cost_reset(img_width[i + 1]);//放memcpy

		}
	}
	else if (now == 'd' && next == 'e')
	{
		//相邻层行的处理
		tran_count = 1000;
	}
	else if (now == 'e' && next == 'a')
	{
		//相邻层行的处理
		tran_count = 1000;
	}
	else if (now == 'e' && next == 'b')
	{
		//相邻层行的处理
		tran_count = 1000;
	}
	else if (now == 'e' && next == 'c')
	{
		//相邻层行的处理
		tran_count = 1000;
	}
	else if (now == 'e' && next == 'd')
	{
		//相邻层行的处理
		tran_count = 1000;
	}
	else if (now == 'e' && next == 'e')
	{
		//相邻层行的处理
		tran_count = 1000;
	}
	return tran_count;
	//return 0;
}


//double evaluate_seq(string s, int l, int k)
//{
//
//	double evaluate_seq1 = 0;
//	evaluate_seq1 += dp_nn[l][k];//第l层计算后传输，往后的评估可以直接加上
//	/*if (s.length()>=2&&s.length() <= 5)
//	{
//		cout<<s <<"    kanzheli---1   " << evaluate_seq1 << endl;
//	}
//	if (s.length() >= 25 )
//	{
//		cout << s << "    kanzheli---1   " << evaluate_seq1 << endl;
//	}*/
//	//评估 
//
//	int n = s.length();//当前序列长度
//	int m = l - nn_number + n;//第l层在序列s中是第l-nn_number+n;
//	//初始第m层计算后传输，末尾第n-1层计算后传输
//	//计算中间不传的额外计算，每往前一层，额外加一层计算 
//	int ext = 0;//统计连续多少层没传
//	//cout<<"kanzheliiiii  " << m - 1 << endl;
//	for (int i = m - 1;i > 0;i--)
//	{
//		//第三个参数对应额外加了几层计算 额外i-m-1层计算 ，序列中第i层对应模型第nn_number-1-i层，
//		//evaluate_seq1 += evaluate_extra_single(nn_number - i - 1, s, i - m-1);
//		ext += 1;
//		evaluate_seq1 += evaluate_comp_extra(nn_number - n + i, l, s, ext);//第i层对应总长的i+nn_number-n
//	}
//
//	/*if (s == "aaa")
//	{
//		cout << "zaikanzheli  " << n<<" <-nm->  "<<m << endl;
//	}*/
//
//	//计算开始不传输的前一层（第n-1层即第nn_number-1-nn+1层）的传输量并加上 ，包含额外传输
//	if (nn_number == n && s[0] >= '1' && s[0] <= '5')//若当前层为第0层，且第0层计算后不传输
//	{
//		evaluate_seq1 += 0;//设置if进入的条件
//		ext += 1;//最前面一层不传输，需要再多做一层
//		//pingbi1:cout<<"            asfffffffffffffffffffffffffffffffff                         " << s << endl;
//	}
//	else
//	{
//		if (ext == 0)
//		{
//			evaluate_seq1 += evaluate_tran_single(nn_number - n, s);//设置内部参数
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
//		//evaluate_seq1 += evaluate_tran_extra(nn_number - n, s, ext);//设置内部参数
//		//evaluate_seq1 += evaluate_reset_extra(nn_number - n, s, ext);//设置内部参数
//	}
//
//	//计算最前面一层，也就是当前层所需要的计算   
//	//这一层是计算后传输的，如果是dp_nn_no调用需要再写一个函数实现该语句，因为首层不传输  
//	//不用了！！，单层评估函数内部以及考虑了这种情况
//	evaluate_seq1 += evaluate_single(nn_number - n, l, s, ext);//最前面一层，对于第nn-n层 （从0开始）
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

