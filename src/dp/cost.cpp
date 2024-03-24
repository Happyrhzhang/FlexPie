#include"cost.h"
#include"pythreadstatelock.h"
//#include<Python.h>


void PythonInit()
{
	if (!Py_IsInitialized())
	{
		//1.初始化Python解释器，这是调用操作的第一步
		Py_Initialize();
		if (!Py_IsInitialized()) {
			std::system("pause");
			//            emit failed();
		}
		else {

			//执行单句Python语句，用于给出调用模块的路径，否则将无法找到相应的调用模块
			//PyRun_SimpleString("import sys");
			//QString setSysPath = QString("sys.path.append('%1')").arg(QCoreApplication::applicationDirPath());
			//PyRun_SimpleString(setSysPath.toStdString().c_str());
			// 初始化线程支持
			//Py_InitThreads();
			// 启动子线程前执行，为了释放PyEval_InitThreads获得的全局锁，否则子线程可能无法获取到全局锁。
			PyEval_ReleaseThread(PyThreadState_Get());
			cout << "Initial Python Success!" << endl;

		}
	}
}

//传输评估
double cost_tran(int bandw, int throughput)
{
	
	//变量定义
	PyObject* pArgs, * pReturn; //定义python对象

	//构造给python函数传的参数

	pArgs = PyTuple_New(2);//创建一个元组，长度为2。
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("i", bandw));
	PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", throughput));//将pArgs的第一（0）个变量设置为整型(i) 传输评估模块调用测试


	//使用C++的python接口调用该函数 并获取返回参数
	//pReturn = PyObject_CallObject(pFunc, pArgs);
	//pReturn = PyObject_CallObject(pFunc, NULL);
	//pArgs = PyTuple_New(1);//创建一个元组，长度为1。
	//PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", "test C++ use python'func"));//将pArgs的第一（0）个变量设置为字符串(s) “this is a test”调用测试
	pReturn = PyObject_CallObject(pFunc_tran, pArgs);

	//接收python计算好的返回值,进行读取（转换成C++类型）
	//返回一个值：
	double nResult;
	PyArg_Parse(pReturn, "d", &nResult);
	//cout << "return result is " << nResult << endl;

	//返回两个值
	//double nResult1, nResult2;
	//// i表示转换成int型变量。
	//// d：double 型变量
	//// 在这里，最需要注意的是：PyArg_Parse的最后一个参数，必须加上“&”符号 即：函数传参引用
	//PyArg_ParseTuple(pReturn, "d|d", &nResult1, &nResult2);//分析返回的元组值
	//cout << "return result is " << nResult1 << nResult2 << endl;

	//另一种实现
	/*double nResult[2];
	PyObject* pValu;
	Py_ssize_t nums;
	nums = PyTuple_Size(pReturn);
	for (Py_ssize_t i;i < nums;i++)
	{
		pValu = PyTuple_GetItem(pReturn, i);
		nResult[i] = PyFloat_AsDouble(pValu);
	}*/

	//销毁python相关
	//Py_DECREF(pName);
	//Py_DECREF(pModule);
	return nResult;
}

//计算评估
double cost_comp(string type, int inp_c, int out_c, int img_h, int img_w, int img_h1, int img_w1, int stride)
{
	
	//变量定义
	PyObject* pArgs, * pReturn; //定义python对象

	//cout << 1 << endl;//test

	
	//构造给python函数传的参数

	pArgs = PyTuple_New(8);//创建一个元组，长度为2。
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", type));
	PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", inp_c));//将pArgs的第一（0）个变量设置为整型(i) 传输评估模块调用测试
	PyTuple_SetItem(pArgs, 2, Py_BuildValue("i", out_c));
	PyTuple_SetItem(pArgs, 3, Py_BuildValue("i", img_h));
	PyTuple_SetItem(pArgs, 4, Py_BuildValue("i", img_w));
	PyTuple_SetItem(pArgs, 5, Py_BuildValue("i", img_h1));
	PyTuple_SetItem(pArgs, 6, Py_BuildValue("i", img_w1));
	PyTuple_SetItem(pArgs, 7, Py_BuildValue("i", stride));

	//使用C++的python接口调用该函数 并获取返回参数
	//pReturn = PyObject_CallObject(pFunc, pArgs);
	//pReturn = PyObject_CallObject(pFunc, NULL);
	//pArgs = PyTuple_New(1);//创建一个元组，长度为1。
	//PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", "test C++ use python'func"));//将pArgs的第一（0）个变量设置为字符串(s) “this is a test”调用测试
	pReturn = PyObject_CallObject(pFunc_comp, pArgs);

	//接收python计算好的返回值,进行读取（转换成C++类型）
	//返回一个值：
	double nResult;
	PyArg_Parse(pReturn, "d", &nResult);
	//cout << "return result is " << nResult << endl;

	//返回两个值
	//double nResult1, nResult2;
	//// i表示转换成int型变量。
	//// d：double 型变量
	//// 在这里，最需要注意的是：PyArg_Parse的最后一个参数，必须加上“&”符号 即：函数传参引用
	//PyArg_ParseTuple(pReturn, "d|d", &nResult1, &nResult2);//分析返回的元组值
	//cout << "return result is " << nResult1 << nResult2 << endl;

	//另一种实现
	/*double nResult[2];
	PyObject* pValu;
	Py_ssize_t nums;
	nums = PyTuple_Size(pReturn);
	for (Py_ssize_t i;i < nums;i++)
	{
		pValu = PyTuple_GetItem(pReturn, i);
		nResult[i] = PyFloat_AsDouble(pValu);
	}*/

	//销毁python相关
	//Py_DECREF(pName);
	//Py_DECREF(pModule);
	return nResult;
}

//重排评估
double cost_reset(int nums)
{
	double nResult;
	string s = to_string(nums*4);
	if (cost_resetvalue.find(s) == cost_resetvalue.end())
	{
		//变量定义
		PyObject* pArgs, * pReturn; //定义python对象

		//构造给python函数传的参数
		pArgs = PyTuple_New(1);//创建一个元组，长度为1。
		PyTuple_SetItem(pArgs, 0, Py_BuildValue("i", nums*4));

		//使用C++的python接口调用该函数 并获取返回参数
		pReturn = PyObject_CallObject(pFunc_reset, pArgs);

		//接收python计算好的返回值,进行读取（转换成C++类型）
		//返回一个值：
		PyArg_Parse(pReturn, "d", &nResult);
		Py_DECREF(pModule);
		cost_resetvalue[s] = nResult;
	}
	else
	{
		nResult = cost_resetvalue[s];
	}
	////变量定义
	//PyObject* pArgs, * pReturn; //定义python对象
	//
	////构造给python函数传的参数
	//pArgs = PyTuple_New(1);//创建一个元组，长度为1。
	//PyTuple_SetItem(pArgs, 0, Py_BuildValue("i", nums));

	////使用C++的python接口调用该函数 并获取返回参数
	//pReturn = PyObject_CallObject(pFunc_reset, pArgs);

	////接收python计算好的返回值,进行读取（转换成C++类型）
	////返回一个值：
	//PyArg_Parse(pReturn, "d", &nResult);
	//Py_DECREF(pModule);
	return nResult;
}



//传输评估
double cost_tran123(int bandwidth, int throughput)
{
	class PyThreadStateLock PyThreadLock;//获取全局锁  //在该函数执行结束后，变量失效，会自动调用析构函数释放锁
	//变量定义
	PyObject* pName, * pModule, * pDict, * pFunc, * pArgs, * pReturn; //定义python对象

	//调用python文件，作为一个模块，一个对象调用进来
	pName = PyUnicode_FromString("combined_cost_eval"); //这一步作用应该是防止中文乱码
	pModule = PyImport_Import(pName); //载入的模块
	if (!pModule) {
		cout << "Can't find pModule" << endl;
		std::system("pause");
		//return -98;
	}//查看是否载入了模块

	//调用python函数，即调用对象pmodule的函数，即获取到函数pmodule.print_test2()  xgb_tran1
	pFunc = PyObject_GetAttrString(pModule, "xgb_tran");
	if (!pFunc || !PyCallable_Check(pFunc)) {
		cout << "error: pFunc" << endl;
		std::system("pause");
		//return -96;
	}

	//构造给python函数传的参数

	pArgs = PyTuple_New(2);//创建一个元组，长度为2。
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("i", bandwidth));
	PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", throughput));//将pArgs的第一（0）个变量设置为整型(i) 传输评估模块调用测试


	//使用C++的python接口调用该函数 并获取返回参数
	//pReturn = PyObject_CallObject(pFunc, pArgs);
	//pReturn = PyObject_CallObject(pFunc, NULL);
	//pArgs = PyTuple_New(1);//创建一个元组，长度为1。
	//PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", "test C++ use python'func"));//将pArgs的第一（0）个变量设置为字符串(s) “this is a test”调用测试
	pReturn = PyObject_CallObject(pFunc, pArgs);

	//接收python计算好的返回值,进行读取（转换成C++类型）
	//返回一个值：
	double nResult;
	PyArg_Parse(pReturn, "d", &nResult);
	//cout << "return result is " << nResult << endl;
	
	//返回两个值
	//double nResult1, nResult2;
	//// i表示转换成int型变量。
	//// d：double 型变量
	//// 在这里，最需要注意的是：PyArg_Parse的最后一个参数，必须加上“&”符号 即：函数传参引用
	//PyArg_ParseTuple(pReturn, "d|d", &nResult1, &nResult2);//分析返回的元组值
	//cout << "return result is " << nResult1 << nResult2 << endl;

	//另一种实现
	/*double nResult[2];
	PyObject* pValu;
	Py_ssize_t nums;
	nums = PyTuple_Size(pReturn);
	for (Py_ssize_t i;i < nums;i++)
	{
		pValu = PyTuple_GetItem(pReturn, i);
		nResult[i] = PyFloat_AsDouble(pValu);
	}*/

	//销毁python相关
	//Py_DECREF(pName);
	Py_DECREF(pModule);
	return nResult;
}

//计算评估
double cost_comp123(string type, int inp_c, int out_c, int img_h, int img_w, int stride)
{
	class PyThreadStateLock PyThreadLock;//获取全局锁  //在该函数执行结束后，变量失效，会自动调用析构函数释放锁
	//变量定义
	PyObject* pName, * pModule, * pDict, * pFunc, * pArgs, * pReturn; //定义python对象

	//cout << 1 << endl;//test

	//调用python文件，作为一个模块，一个对象调用进来
	pName = PyUnicode_FromString("combined_cost_eval"); //这一步作用应该是防止中文乱码
	pModule = PyImport_Import(pName); //载入的模块
	if (!pModule) {
		cout << "Can't find pModule" << endl;
		std::system("pause");
		//return -98;
	}//查看是否载入了模块

	//cout << 2 << endl;

	//调用python函数，即调用对象pmodule的函数，即获取到函数pmodule.print_test2()  xgb_tran1
	pFunc = PyObject_GetAttrString(pModule, "xgb_comp");
	if (!pFunc || !PyCallable_Check(pFunc)) {
		cout << "error: pFunc" << endl;
		std::system("pause");
		//return -96;
	}

	//cout << 3 << endl;

	//构造给python函数传的参数

	pArgs = PyTuple_New(6);//创建一个元组，长度为2。
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", type));
	PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", inp_c));//将pArgs的第一（0）个变量设置为整型(i) 传输评估模块调用测试
	PyTuple_SetItem(pArgs, 2, Py_BuildValue("i", out_c));
	PyTuple_SetItem(pArgs, 3, Py_BuildValue("i", img_h));
	PyTuple_SetItem(pArgs, 4, Py_BuildValue("i", img_w));
	PyTuple_SetItem(pArgs, 5, Py_BuildValue("i", stride));

	//使用C++的python接口调用该函数 并获取返回参数
	//pReturn = PyObject_CallObject(pFunc, pArgs);
	//pReturn = PyObject_CallObject(pFunc, NULL);
	//pArgs = PyTuple_New(1);//创建一个元组，长度为1。
	//PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", "test C++ use python'func"));//将pArgs的第一（0）个变量设置为字符串(s) “this is a test”调用测试
	pReturn = PyObject_CallObject(pFunc, pArgs);

	//接收python计算好的返回值,进行读取（转换成C++类型）
	//返回一个值：
	double nResult;
	PyArg_Parse(pReturn, "d", &nResult);
	//cout << "return result is " << nResult << endl;

	//返回两个值
	//double nResult1, nResult2;
	//// i表示转换成int型变量。
	//// d：double 型变量
	//// 在这里，最需要注意的是：PyArg_Parse的最后一个参数，必须加上“&”符号 即：函数传参引用
	//PyArg_ParseTuple(pReturn, "d|d", &nResult1, &nResult2);//分析返回的元组值
	//cout << "return result is " << nResult1 << nResult2 << endl;

	//另一种实现
	/*double nResult[2];
	PyObject* pValu;
	Py_ssize_t nums;
	nums = PyTuple_Size(pReturn);
	for (Py_ssize_t i;i < nums;i++)
	{
		pValu = PyTuple_GetItem(pReturn, i);
		nResult[i] = PyFloat_AsDouble(pValu);
	}*/

	//销毁python相关
	//Py_DECREF(pName);
	Py_DECREF(pModule);
	return nResult;
}

//重排评估
double cost_reset123(int nums)
{
	class PyThreadStateLock PyThreadLock;//获取全局锁  //在该函数执行结束后，变量失效，会自动调用析构函数释放锁
	//变量定义
	PyObject* pName, * pModule, * pDict, * pFunc, * pArgs, * pReturn; //定义python对象

	//调用python文件，作为一个模块，一个对象调用进来
	pName = PyUnicode_FromString("combined_cost_eval"); //这一步作用应该是防止中文乱码
	pModule = PyImport_Import(pName); //载入的模块
	if (!pModule) {
		cout << "Can't find pModule" << endl;
		std::system("pause");
		//return -98;
	}//查看是否载入了模块

	//调用python函数，即调用对象pmodule的函数，即获取到函数pmodule.print_test2()  xgb_tran1
	pFunc = PyObject_GetAttrString(pModule, "xgb_reset");
	if (!pFunc || !PyCallable_Check(pFunc)) {
		cout << "error: pFunc" << endl;
		std::system("pause");
		//return -96;
	}

	//cout << 3 << endl;

	//构造给python函数传的参数

	pArgs = PyTuple_New(1);//创建一个元组，长度为1。
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("i", nums));
	//PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", throughput));//将pArgs的第一（0）个变量设置为整型(i) 传输评估模块调用测试


	//使用C++的python接口调用该函数 并获取返回参数
	//pReturn = PyObject_CallObject(pFunc, pArgs);
	//pReturn = PyObject_CallObject(pFunc, NULL);
	//pArgs = PyTuple_New(1);//创建一个元组，长度为1。
	//PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", "test C++ use python'func"));//将pArgs的第一（0）个变量设置为字符串(s) “this is a test”调用测试
	pReturn = PyObject_CallObject(pFunc, pArgs);

	//接收python计算好的返回值,进行读取（转换成C++类型）
	//返回一个值：
	double nResult;
	PyArg_Parse(pReturn, "d", &nResult);
	//cout << "return result is " << nResult << endl;

	//返回两个值
	//double nResult1, nResult2;
	//// i表示转换成int型变量。
	//// d：double 型变量
	//// 在这里，最需要注意的是：PyArg_Parse的最后一个参数，必须加上“&”符号 即：函数传参引用
	//PyArg_ParseTuple(pReturn, "d|d", &nResult1, &nResult2);//分析返回的元组值
	//cout << "return result is " << nResult1 << nResult2 << endl;

	//另一种实现
	/*double nResult[2];
	PyObject* pValu;
	Py_ssize_t nums;
	nums = PyTuple_Size(pReturn);
	for (Py_ssize_t i;i < nums;i++)
	{
		pValu = PyTuple_GetItem(pReturn, i);
		nResult[i] = PyFloat_AsDouble(pValu);
	}*/
	Py_DECREF(pModule);
	return nResult;
}


//传输评估
void cost_tran2(int bandwidth, int throughput)
{
	//初始化python解释器
	Py_Initialize();
	if (!Py_IsInitialized()) {
		std::system("pause");
		//	return -99;
	} //查看python解释器是否成功初始化

	//变量定义
	PyObject* pName, * pModule, * pDict, * pFunc, * pArgs, * pReturn; //定义python对象

	//调用python文件，作为一个模块，一个对象调用进来
	pName = PyUnicode_FromString("combined_cost_eval"); //这一步作用应该是防止中文乱码
	pModule = PyImport_Import(pName); //载入的模块
	if (!pModule) {
		cout << "Can't find pModule" << endl;
		std::system("pause");
		//return -98;
	}//查看是否载入了模块

	//调用python函数，即调用对象pmodule的函数，即获取到函数pmodule.print_test2()  xgb_tran1
	pFunc = PyObject_GetAttrString(pModule, "xgb_tran");
	if (!pFunc || !PyCallable_Check(pFunc)) {
		cout << "error: pFunc" << endl;
		std::system("pause");
		//return -96;
	}

	//构造给python函数传的参数

	pArgs = PyTuple_New(2);//创建一个元组，长度为2。
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("i", bandwidth));
	PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", throughput));//将pArgs的第一（0）个变量设置为整型(i) 传输评估模块调用测试


	//使用C++的python接口调用该函数 并获取返回参数
	//pReturn = PyObject_CallObject(pFunc, pArgs);
	//pReturn = PyObject_CallObject(pFunc, NULL);
	//pArgs = PyTuple_New(1);//创建一个元组，长度为1。
	//PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", "test C++ use python'func"));//将pArgs的第一（0）个变量设置为字符串(s) “this is a test”调用测试
	pReturn = PyObject_CallObject(pFunc, pArgs);

	//接收python计算好的返回值,进行读取（转换成C++类型）
	//返回一个值：
	double nResult;
	PyArg_Parse(pReturn, "d", &nResult);
	cout << "return result is " << nResult << endl;

	//返回两个值
	//double nResult1, nResult2;
	//// i表示转换成int型变量。
	//// d：double 型变量
	//// 在这里，最需要注意的是：PyArg_Parse的最后一个参数，必须加上“&”符号 即：函数传参引用
	//PyArg_ParseTuple(pReturn, "d|d", &nResult1, &nResult2);//分析返回的元组值
	//cout << "return result is " << nResult1 << nResult2 << endl;

	//另一种实现
	/*double nResult[2];
	PyObject* pValu;
	Py_ssize_t nums;
	nums = PyTuple_Size(pReturn);
	for (Py_ssize_t i;i < nums;i++)
	{
		pValu = PyTuple_GetItem(pReturn, i);
		nResult[i] = PyFloat_AsDouble(pValu);
	}*/

	//销毁python相关
	//Py_DECREF(pName);
	Py_DECREF(pModule);
	Py_Finalize();
}

//计算评估
void cost_comp2(string type, int inp_c, int out_c, int img_h, int img_w, int stride)
{
	//初始化python解释器
	Py_Initialize();
	if (!Py_IsInitialized()) {
		std::system("pause");
		//	return -99;
	} //查看python解释器是否成功初始化

	//变量定义
	PyObject* pName, * pModule, * pDict, * pFunc, * pArgs, * pReturn; //定义python对象

	//cout << 1 << endl;//test

	//调用python文件，作为一个模块，一个对象调用进来
	pName = PyUnicode_FromString("combined_cost_eval"); //这一步作用应该是防止中文乱码
	pModule = PyImport_Import(pName); //载入的模块
	if (!pModule) {
		cout << "Can't find pModule" << endl;
		std::system("pause");
		//return -98;
	}//查看是否载入了模块

	//cout << 2 << endl;

	//调用python函数，即调用对象pmodule的函数，即获取到函数pmodule.print_test2()  xgb_tran1
	pFunc = PyObject_GetAttrString(pModule, "xgb_comp");
	if (!pFunc || !PyCallable_Check(pFunc)) {
		cout << "error: pFunc" << endl;
		std::system("pause");
		//return -96;
	}

	//cout << 3 << endl;

	//构造给python函数传的参数
	
	pArgs = PyTuple_New(6);//创建一个元组，长度为2。
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", type));
	PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", inp_c));//将pArgs的第一（0）个变量设置为整型(i) 传输评估模块调用测试
	PyTuple_SetItem(pArgs, 2, Py_BuildValue("i", out_c));
	PyTuple_SetItem(pArgs, 3, Py_BuildValue("i", img_h));
	PyTuple_SetItem(pArgs, 4, Py_BuildValue("i", img_w));
	PyTuple_SetItem(pArgs, 5, Py_BuildValue("i", stride));

	//使用C++的python接口调用该函数 并获取返回参数
	//pReturn = PyObject_CallObject(pFunc, pArgs);
	//pReturn = PyObject_CallObject(pFunc, NULL);
	//pArgs = PyTuple_New(1);//创建一个元组，长度为1。
	//PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", "test C++ use python'func"));//将pArgs的第一（0）个变量设置为字符串(s) “this is a test”调用测试
	pReturn = PyObject_CallObject(pFunc, pArgs);

	//接收python计算好的返回值,进行读取（转换成C++类型）
	//返回一个值：
	double nResult;
	PyArg_Parse(pReturn, "d", &nResult);
	cout << "return result is " << nResult << endl;

	//返回两个值
	//double nResult1, nResult2;
	//// i表示转换成int型变量。
	//// d：double 型变量
	//// 在这里，最需要注意的是：PyArg_Parse的最后一个参数，必须加上“&”符号 即：函数传参引用
	//PyArg_ParseTuple(pReturn, "d|d", &nResult1, &nResult2);//分析返回的元组值
	//cout << "return result is " << nResult1 << nResult2 << endl;

	//另一种实现
	/*double nResult[2];
	PyObject* pValu;
	Py_ssize_t nums;
	nums = PyTuple_Size(pReturn);
	for (Py_ssize_t i;i < nums;i++)
	{
		pValu = PyTuple_GetItem(pReturn, i);
		nResult[i] = PyFloat_AsDouble(pValu);
	}*/

	//销毁python相关
	//Py_DECREF(pName);
	Py_DECREF(pModule);
	Py_Finalize();
}

//重排评估
void cost_reset2(int nums)
{
	//初始化python解释器
	Py_Initialize();
	if (!Py_IsInitialized()) {
		std::system("pause");
		//	return -99;
	} //查看python解释器是否成功初始化

	//变量定义
	PyObject* pName, * pModule, * pDict, * pFunc, * pArgs, * pReturn; //定义python对象

	//调用python文件，作为一个模块，一个对象调用进来
	pName = PyUnicode_FromString("combined_cost_eval"); //这一步作用应该是防止中文乱码
	pModule = PyImport_Import(pName); //载入的模块
	if (!pModule) {
		cout << "Can't find pModule" << endl;
		std::system("pause");
		//return -98;
	}//查看是否载入了模块

	//调用python函数，即调用对象pmodule的函数，即获取到函数pmodule.print_test2()  xgb_tran1
	pFunc = PyObject_GetAttrString(pModule, "xgb_reset");
	if (!pFunc || !PyCallable_Check(pFunc)) {
		cout << "error: pFunc" << endl;
		std::system("pause");
		//return -96;
	}

	//cout << 3 << endl;

	//构造给python函数传的参数

	pArgs = PyTuple_New(1);//创建一个元组，长度为1。
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("i", nums));
	//PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", throughput));//将pArgs的第一（0）个变量设置为整型(i) 传输评估模块调用测试


	//使用C++的python接口调用该函数 并获取返回参数
	//pReturn = PyObject_CallObject(pFunc, pArgs);
	//pReturn = PyObject_CallObject(pFunc, NULL);
	//pArgs = PyTuple_New(1);//创建一个元组，长度为1。
	//PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", "test C++ use python'func"));//将pArgs的第一（0）个变量设置为字符串(s) “this is a test”调用测试
	pReturn = PyObject_CallObject(pFunc, pArgs);

	//接收python计算好的返回值,进行读取（转换成C++类型）
	//返回一个值：
	double nResult;
	PyArg_Parse(pReturn, "d", &nResult);
	cout << "return result is " << nResult << endl;

	//返回两个值
	//double nResult1, nResult2;
	//// i表示转换成int型变量。
	//// d：double 型变量
	//// 在这里，最需要注意的是：PyArg_Parse的最后一个参数，必须加上“&”符号 即：函数传参引用
	//PyArg_ParseTuple(pReturn, "d|d", &nResult1, &nResult2);//分析返回的元组值
	//cout << "return result is " << nResult1 << nResult2 << endl;

	//另一种实现
	/*double nResult[2];
	PyObject* pValu;
	Py_ssize_t nums;
	nums = PyTuple_Size(pReturn);
	for (Py_ssize_t i;i < nums;i++)
	{
		pValu = PyTuple_GetItem(pReturn, i);
		nResult[i] = PyFloat_AsDouble(pValu);
	}*/

	//销毁python相关
	//Py_DECREF(pName);
	Py_DECREF(pModule);
	Py_Finalize();
}
void cost_reset1(PyObject* pModule, int nums)
{


	//变量定义
	PyObject* pDict, * pFunc, * pArgs, * pReturn; //定义python对象


	//调用python函数，即调用对象pmodule的函数，即获取到函数pmodule.print_test2()  xgb_tran1
	pFunc = PyObject_GetAttrString(pModule, "xgb_reset");
	if (!pFunc || !PyCallable_Check(pFunc)) {
		cout << "error: pFunc" << endl;
		std::system("pause");
		//return -96;
	}

	//cout << 3 << endl;

	//构造给python函数传的参数

	pArgs = PyTuple_New(1);//创建一个元组，长度为1。
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("i", nums));
	//PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", throughput));//将pArgs的第一（0）个变量设置为整型(i) 传输评估模块调用测试


	//使用C++的python接口调用该函数 并获取返回参数
	//pReturn = PyObject_CallObject(pFunc, pArgs);
	//pReturn = PyObject_CallObject(pFunc, NULL);
	//pArgs = PyTuple_New(1);//创建一个元组，长度为1。
	//PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", "test C++ use python'func"));//将pArgs的第一（0）个变量设置为字符串(s) “this is a test”调用测试
	pReturn = PyObject_CallObject(pFunc, pArgs);

	//接收python计算好的返回值,进行读取（转换成C++类型）
	//返回一个值：
	double nResult;
	PyArg_Parse(pReturn, "d", &nResult);
	cout << "return result is " << nResult << endl;

	//返回两个值
	//double nResult1, nResult2;
	//// i表示转换成int型变量。
	//// d：double 型变量
	//// 在这里，最需要注意的是：PyArg_Parse的最后一个参数，必须加上“&”符号 即：函数传参引用
	//PyArg_ParseTuple(pReturn, "d|d", &nResult1, &nResult2);//分析返回的元组值
	//cout << "return result is " << nResult1 << nResult2 << endl;

	//另一种实现
	/*double nResult[2];
	PyObject* pValu;
	Py_ssize_t nums;
	nums = PyTuple_Size(pReturn);
	for (Py_ssize_t i;i < nums;i++)
	{
		pValu = PyTuple_GetItem(pReturn, i);
		nResult[i] = PyFloat_AsDouble(pValu);
	}*/

	
}
