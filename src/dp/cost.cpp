#include"cost.h"
#include"pythreadstatelock.h"
//#include<Python.h>


void PythonInit()
{
	if (!Py_IsInitialized())
	{
		//1.��ʼ��Python�����������ǵ��ò����ĵ�һ��
		Py_Initialize();
		if (!Py_IsInitialized()) {
			std::system("pause");
			//            emit failed();
		}
		else {

			//ִ�е���Python��䣬���ڸ�������ģ���·���������޷��ҵ���Ӧ�ĵ���ģ��
			//PyRun_SimpleString("import sys");
			//QString setSysPath = QString("sys.path.append('%1')").arg(QCoreApplication::applicationDirPath());
			//PyRun_SimpleString(setSysPath.toStdString().c_str());
			// ��ʼ���߳�֧��
			//Py_InitThreads();
			// �������߳�ǰִ�У�Ϊ���ͷ�PyEval_InitThreads��õ�ȫ�������������߳̿����޷���ȡ��ȫ������
			PyEval_ReleaseThread(PyThreadState_Get());
			cout << "Initial Python Success!" << endl;

		}
	}
}

//��������
double cost_tran(int bandw, int throughput)
{
	
	//��������
	PyObject* pArgs, * pReturn; //����python����

	//�����python�������Ĳ���

	pArgs = PyTuple_New(2);//����һ��Ԫ�飬����Ϊ2��
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("i", bandw));
	PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", throughput));//��pArgs�ĵ�һ��0������������Ϊ����(i) ��������ģ����ò���


	//ʹ��C++��python�ӿڵ��øú��� ����ȡ���ز���
	//pReturn = PyObject_CallObject(pFunc, pArgs);
	//pReturn = PyObject_CallObject(pFunc, NULL);
	//pArgs = PyTuple_New(1);//����һ��Ԫ�飬����Ϊ1��
	//PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", "test C++ use python'func"));//��pArgs�ĵ�һ��0������������Ϊ�ַ���(s) ��this is a test�����ò���
	pReturn = PyObject_CallObject(pFunc_tran, pArgs);

	//����python����õķ���ֵ,���ж�ȡ��ת����C++���ͣ�
	//����һ��ֵ��
	double nResult;
	PyArg_Parse(pReturn, "d", &nResult);
	//cout << "return result is " << nResult << endl;

	//��������ֵ
	//double nResult1, nResult2;
	//// i��ʾת����int�ͱ�����
	//// d��double �ͱ���
	//// ���������Ҫע����ǣ�PyArg_Parse�����һ��������������ϡ�&������ ����������������
	//PyArg_ParseTuple(pReturn, "d|d", &nResult1, &nResult2);//�������ص�Ԫ��ֵ
	//cout << "return result is " << nResult1 << nResult2 << endl;

	//��һ��ʵ��
	/*double nResult[2];
	PyObject* pValu;
	Py_ssize_t nums;
	nums = PyTuple_Size(pReturn);
	for (Py_ssize_t i;i < nums;i++)
	{
		pValu = PyTuple_GetItem(pReturn, i);
		nResult[i] = PyFloat_AsDouble(pValu);
	}*/

	//����python���
	//Py_DECREF(pName);
	//Py_DECREF(pModule);
	return nResult;
}

//��������
double cost_comp(string type, int inp_c, int out_c, int img_h, int img_w, int img_h1, int img_w1, int stride)
{
	
	//��������
	PyObject* pArgs, * pReturn; //����python����

	//cout << 1 << endl;//test

	
	//�����python�������Ĳ���

	pArgs = PyTuple_New(8);//����һ��Ԫ�飬����Ϊ2��
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", type));
	PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", inp_c));//��pArgs�ĵ�һ��0������������Ϊ����(i) ��������ģ����ò���
	PyTuple_SetItem(pArgs, 2, Py_BuildValue("i", out_c));
	PyTuple_SetItem(pArgs, 3, Py_BuildValue("i", img_h));
	PyTuple_SetItem(pArgs, 4, Py_BuildValue("i", img_w));
	PyTuple_SetItem(pArgs, 5, Py_BuildValue("i", img_h1));
	PyTuple_SetItem(pArgs, 6, Py_BuildValue("i", img_w1));
	PyTuple_SetItem(pArgs, 7, Py_BuildValue("i", stride));

	//ʹ��C++��python�ӿڵ��øú��� ����ȡ���ز���
	//pReturn = PyObject_CallObject(pFunc, pArgs);
	//pReturn = PyObject_CallObject(pFunc, NULL);
	//pArgs = PyTuple_New(1);//����һ��Ԫ�飬����Ϊ1��
	//PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", "test C++ use python'func"));//��pArgs�ĵ�һ��0������������Ϊ�ַ���(s) ��this is a test�����ò���
	pReturn = PyObject_CallObject(pFunc_comp, pArgs);

	//����python����õķ���ֵ,���ж�ȡ��ת����C++���ͣ�
	//����һ��ֵ��
	double nResult;
	PyArg_Parse(pReturn, "d", &nResult);
	//cout << "return result is " << nResult << endl;

	//��������ֵ
	//double nResult1, nResult2;
	//// i��ʾת����int�ͱ�����
	//// d��double �ͱ���
	//// ���������Ҫע����ǣ�PyArg_Parse�����һ��������������ϡ�&������ ����������������
	//PyArg_ParseTuple(pReturn, "d|d", &nResult1, &nResult2);//�������ص�Ԫ��ֵ
	//cout << "return result is " << nResult1 << nResult2 << endl;

	//��һ��ʵ��
	/*double nResult[2];
	PyObject* pValu;
	Py_ssize_t nums;
	nums = PyTuple_Size(pReturn);
	for (Py_ssize_t i;i < nums;i++)
	{
		pValu = PyTuple_GetItem(pReturn, i);
		nResult[i] = PyFloat_AsDouble(pValu);
	}*/

	//����python���
	//Py_DECREF(pName);
	//Py_DECREF(pModule);
	return nResult;
}

//��������
double cost_reset(int nums)
{
	double nResult;
	string s = to_string(nums*4);
	if (cost_resetvalue.find(s) == cost_resetvalue.end())
	{
		//��������
		PyObject* pArgs, * pReturn; //����python����

		//�����python�������Ĳ���
		pArgs = PyTuple_New(1);//����һ��Ԫ�飬����Ϊ1��
		PyTuple_SetItem(pArgs, 0, Py_BuildValue("i", nums*4));

		//ʹ��C++��python�ӿڵ��øú��� ����ȡ���ز���
		pReturn = PyObject_CallObject(pFunc_reset, pArgs);

		//����python����õķ���ֵ,���ж�ȡ��ת����C++���ͣ�
		//����һ��ֵ��
		PyArg_Parse(pReturn, "d", &nResult);
		Py_DECREF(pModule);
		cost_resetvalue[s] = nResult;
	}
	else
	{
		nResult = cost_resetvalue[s];
	}
	////��������
	//PyObject* pArgs, * pReturn; //����python����
	//
	////�����python�������Ĳ���
	//pArgs = PyTuple_New(1);//����һ��Ԫ�飬����Ϊ1��
	//PyTuple_SetItem(pArgs, 0, Py_BuildValue("i", nums));

	////ʹ��C++��python�ӿڵ��øú��� ����ȡ���ز���
	//pReturn = PyObject_CallObject(pFunc_reset, pArgs);

	////����python����õķ���ֵ,���ж�ȡ��ת����C++���ͣ�
	////����һ��ֵ��
	//PyArg_Parse(pReturn, "d", &nResult);
	//Py_DECREF(pModule);
	return nResult;
}



//��������
double cost_tran123(int bandwidth, int throughput)
{
	class PyThreadStateLock PyThreadLock;//��ȡȫ����  //�ڸú���ִ�н����󣬱���ʧЧ�����Զ��������������ͷ���
	//��������
	PyObject* pName, * pModule, * pDict, * pFunc, * pArgs, * pReturn; //����python����

	//����python�ļ�����Ϊһ��ģ�飬һ��������ý���
	pName = PyUnicode_FromString("combined_cost_eval"); //��һ������Ӧ���Ƿ�ֹ��������
	pModule = PyImport_Import(pName); //�����ģ��
	if (!pModule) {
		cout << "Can't find pModule" << endl;
		std::system("pause");
		//return -98;
	}//�鿴�Ƿ�������ģ��

	//����python�����������ö���pmodule�ĺ���������ȡ������pmodule.print_test2()  xgb_tran1
	pFunc = PyObject_GetAttrString(pModule, "xgb_tran");
	if (!pFunc || !PyCallable_Check(pFunc)) {
		cout << "error: pFunc" << endl;
		std::system("pause");
		//return -96;
	}

	//�����python�������Ĳ���

	pArgs = PyTuple_New(2);//����һ��Ԫ�飬����Ϊ2��
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("i", bandwidth));
	PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", throughput));//��pArgs�ĵ�һ��0������������Ϊ����(i) ��������ģ����ò���


	//ʹ��C++��python�ӿڵ��øú��� ����ȡ���ز���
	//pReturn = PyObject_CallObject(pFunc, pArgs);
	//pReturn = PyObject_CallObject(pFunc, NULL);
	//pArgs = PyTuple_New(1);//����һ��Ԫ�飬����Ϊ1��
	//PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", "test C++ use python'func"));//��pArgs�ĵ�һ��0������������Ϊ�ַ���(s) ��this is a test�����ò���
	pReturn = PyObject_CallObject(pFunc, pArgs);

	//����python����õķ���ֵ,���ж�ȡ��ת����C++���ͣ�
	//����һ��ֵ��
	double nResult;
	PyArg_Parse(pReturn, "d", &nResult);
	//cout << "return result is " << nResult << endl;
	
	//��������ֵ
	//double nResult1, nResult2;
	//// i��ʾת����int�ͱ�����
	//// d��double �ͱ���
	//// ���������Ҫע����ǣ�PyArg_Parse�����һ��������������ϡ�&������ ����������������
	//PyArg_ParseTuple(pReturn, "d|d", &nResult1, &nResult2);//�������ص�Ԫ��ֵ
	//cout << "return result is " << nResult1 << nResult2 << endl;

	//��һ��ʵ��
	/*double nResult[2];
	PyObject* pValu;
	Py_ssize_t nums;
	nums = PyTuple_Size(pReturn);
	for (Py_ssize_t i;i < nums;i++)
	{
		pValu = PyTuple_GetItem(pReturn, i);
		nResult[i] = PyFloat_AsDouble(pValu);
	}*/

	//����python���
	//Py_DECREF(pName);
	Py_DECREF(pModule);
	return nResult;
}

//��������
double cost_comp123(string type, int inp_c, int out_c, int img_h, int img_w, int stride)
{
	class PyThreadStateLock PyThreadLock;//��ȡȫ����  //�ڸú���ִ�н����󣬱���ʧЧ�����Զ��������������ͷ���
	//��������
	PyObject* pName, * pModule, * pDict, * pFunc, * pArgs, * pReturn; //����python����

	//cout << 1 << endl;//test

	//����python�ļ�����Ϊһ��ģ�飬һ��������ý���
	pName = PyUnicode_FromString("combined_cost_eval"); //��һ������Ӧ���Ƿ�ֹ��������
	pModule = PyImport_Import(pName); //�����ģ��
	if (!pModule) {
		cout << "Can't find pModule" << endl;
		std::system("pause");
		//return -98;
	}//�鿴�Ƿ�������ģ��

	//cout << 2 << endl;

	//����python�����������ö���pmodule�ĺ���������ȡ������pmodule.print_test2()  xgb_tran1
	pFunc = PyObject_GetAttrString(pModule, "xgb_comp");
	if (!pFunc || !PyCallable_Check(pFunc)) {
		cout << "error: pFunc" << endl;
		std::system("pause");
		//return -96;
	}

	//cout << 3 << endl;

	//�����python�������Ĳ���

	pArgs = PyTuple_New(6);//����һ��Ԫ�飬����Ϊ2��
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", type));
	PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", inp_c));//��pArgs�ĵ�һ��0������������Ϊ����(i) ��������ģ����ò���
	PyTuple_SetItem(pArgs, 2, Py_BuildValue("i", out_c));
	PyTuple_SetItem(pArgs, 3, Py_BuildValue("i", img_h));
	PyTuple_SetItem(pArgs, 4, Py_BuildValue("i", img_w));
	PyTuple_SetItem(pArgs, 5, Py_BuildValue("i", stride));

	//ʹ��C++��python�ӿڵ��øú��� ����ȡ���ز���
	//pReturn = PyObject_CallObject(pFunc, pArgs);
	//pReturn = PyObject_CallObject(pFunc, NULL);
	//pArgs = PyTuple_New(1);//����һ��Ԫ�飬����Ϊ1��
	//PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", "test C++ use python'func"));//��pArgs�ĵ�һ��0������������Ϊ�ַ���(s) ��this is a test�����ò���
	pReturn = PyObject_CallObject(pFunc, pArgs);

	//����python����õķ���ֵ,���ж�ȡ��ת����C++���ͣ�
	//����һ��ֵ��
	double nResult;
	PyArg_Parse(pReturn, "d", &nResult);
	//cout << "return result is " << nResult << endl;

	//��������ֵ
	//double nResult1, nResult2;
	//// i��ʾת����int�ͱ�����
	//// d��double �ͱ���
	//// ���������Ҫע����ǣ�PyArg_Parse�����һ��������������ϡ�&������ ����������������
	//PyArg_ParseTuple(pReturn, "d|d", &nResult1, &nResult2);//�������ص�Ԫ��ֵ
	//cout << "return result is " << nResult1 << nResult2 << endl;

	//��һ��ʵ��
	/*double nResult[2];
	PyObject* pValu;
	Py_ssize_t nums;
	nums = PyTuple_Size(pReturn);
	for (Py_ssize_t i;i < nums;i++)
	{
		pValu = PyTuple_GetItem(pReturn, i);
		nResult[i] = PyFloat_AsDouble(pValu);
	}*/

	//����python���
	//Py_DECREF(pName);
	Py_DECREF(pModule);
	return nResult;
}

//��������
double cost_reset123(int nums)
{
	class PyThreadStateLock PyThreadLock;//��ȡȫ����  //�ڸú���ִ�н����󣬱���ʧЧ�����Զ��������������ͷ���
	//��������
	PyObject* pName, * pModule, * pDict, * pFunc, * pArgs, * pReturn; //����python����

	//����python�ļ�����Ϊһ��ģ�飬һ��������ý���
	pName = PyUnicode_FromString("combined_cost_eval"); //��һ������Ӧ���Ƿ�ֹ��������
	pModule = PyImport_Import(pName); //�����ģ��
	if (!pModule) {
		cout << "Can't find pModule" << endl;
		std::system("pause");
		//return -98;
	}//�鿴�Ƿ�������ģ��

	//����python�����������ö���pmodule�ĺ���������ȡ������pmodule.print_test2()  xgb_tran1
	pFunc = PyObject_GetAttrString(pModule, "xgb_reset");
	if (!pFunc || !PyCallable_Check(pFunc)) {
		cout << "error: pFunc" << endl;
		std::system("pause");
		//return -96;
	}

	//cout << 3 << endl;

	//�����python�������Ĳ���

	pArgs = PyTuple_New(1);//����һ��Ԫ�飬����Ϊ1��
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("i", nums));
	//PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", throughput));//��pArgs�ĵ�һ��0������������Ϊ����(i) ��������ģ����ò���


	//ʹ��C++��python�ӿڵ��øú��� ����ȡ���ز���
	//pReturn = PyObject_CallObject(pFunc, pArgs);
	//pReturn = PyObject_CallObject(pFunc, NULL);
	//pArgs = PyTuple_New(1);//����һ��Ԫ�飬����Ϊ1��
	//PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", "test C++ use python'func"));//��pArgs�ĵ�һ��0������������Ϊ�ַ���(s) ��this is a test�����ò���
	pReturn = PyObject_CallObject(pFunc, pArgs);

	//����python����õķ���ֵ,���ж�ȡ��ת����C++���ͣ�
	//����һ��ֵ��
	double nResult;
	PyArg_Parse(pReturn, "d", &nResult);
	//cout << "return result is " << nResult << endl;

	//��������ֵ
	//double nResult1, nResult2;
	//// i��ʾת����int�ͱ�����
	//// d��double �ͱ���
	//// ���������Ҫע����ǣ�PyArg_Parse�����һ��������������ϡ�&������ ����������������
	//PyArg_ParseTuple(pReturn, "d|d", &nResult1, &nResult2);//�������ص�Ԫ��ֵ
	//cout << "return result is " << nResult1 << nResult2 << endl;

	//��һ��ʵ��
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


//��������
void cost_tran2(int bandwidth, int throughput)
{
	//��ʼ��python������
	Py_Initialize();
	if (!Py_IsInitialized()) {
		std::system("pause");
		//	return -99;
	} //�鿴python�������Ƿ�ɹ���ʼ��

	//��������
	PyObject* pName, * pModule, * pDict, * pFunc, * pArgs, * pReturn; //����python����

	//����python�ļ�����Ϊһ��ģ�飬һ��������ý���
	pName = PyUnicode_FromString("combined_cost_eval"); //��һ������Ӧ���Ƿ�ֹ��������
	pModule = PyImport_Import(pName); //�����ģ��
	if (!pModule) {
		cout << "Can't find pModule" << endl;
		std::system("pause");
		//return -98;
	}//�鿴�Ƿ�������ģ��

	//����python�����������ö���pmodule�ĺ���������ȡ������pmodule.print_test2()  xgb_tran1
	pFunc = PyObject_GetAttrString(pModule, "xgb_tran");
	if (!pFunc || !PyCallable_Check(pFunc)) {
		cout << "error: pFunc" << endl;
		std::system("pause");
		//return -96;
	}

	//�����python�������Ĳ���

	pArgs = PyTuple_New(2);//����һ��Ԫ�飬����Ϊ2��
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("i", bandwidth));
	PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", throughput));//��pArgs�ĵ�һ��0������������Ϊ����(i) ��������ģ����ò���


	//ʹ��C++��python�ӿڵ��øú��� ����ȡ���ز���
	//pReturn = PyObject_CallObject(pFunc, pArgs);
	//pReturn = PyObject_CallObject(pFunc, NULL);
	//pArgs = PyTuple_New(1);//����һ��Ԫ�飬����Ϊ1��
	//PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", "test C++ use python'func"));//��pArgs�ĵ�һ��0������������Ϊ�ַ���(s) ��this is a test�����ò���
	pReturn = PyObject_CallObject(pFunc, pArgs);

	//����python����õķ���ֵ,���ж�ȡ��ת����C++���ͣ�
	//����һ��ֵ��
	double nResult;
	PyArg_Parse(pReturn, "d", &nResult);
	cout << "return result is " << nResult << endl;

	//��������ֵ
	//double nResult1, nResult2;
	//// i��ʾת����int�ͱ�����
	//// d��double �ͱ���
	//// ���������Ҫע����ǣ�PyArg_Parse�����һ��������������ϡ�&������ ����������������
	//PyArg_ParseTuple(pReturn, "d|d", &nResult1, &nResult2);//�������ص�Ԫ��ֵ
	//cout << "return result is " << nResult1 << nResult2 << endl;

	//��һ��ʵ��
	/*double nResult[2];
	PyObject* pValu;
	Py_ssize_t nums;
	nums = PyTuple_Size(pReturn);
	for (Py_ssize_t i;i < nums;i++)
	{
		pValu = PyTuple_GetItem(pReturn, i);
		nResult[i] = PyFloat_AsDouble(pValu);
	}*/

	//����python���
	//Py_DECREF(pName);
	Py_DECREF(pModule);
	Py_Finalize();
}

//��������
void cost_comp2(string type, int inp_c, int out_c, int img_h, int img_w, int stride)
{
	//��ʼ��python������
	Py_Initialize();
	if (!Py_IsInitialized()) {
		std::system("pause");
		//	return -99;
	} //�鿴python�������Ƿ�ɹ���ʼ��

	//��������
	PyObject* pName, * pModule, * pDict, * pFunc, * pArgs, * pReturn; //����python����

	//cout << 1 << endl;//test

	//����python�ļ�����Ϊһ��ģ�飬һ��������ý���
	pName = PyUnicode_FromString("combined_cost_eval"); //��һ������Ӧ���Ƿ�ֹ��������
	pModule = PyImport_Import(pName); //�����ģ��
	if (!pModule) {
		cout << "Can't find pModule" << endl;
		std::system("pause");
		//return -98;
	}//�鿴�Ƿ�������ģ��

	//cout << 2 << endl;

	//����python�����������ö���pmodule�ĺ���������ȡ������pmodule.print_test2()  xgb_tran1
	pFunc = PyObject_GetAttrString(pModule, "xgb_comp");
	if (!pFunc || !PyCallable_Check(pFunc)) {
		cout << "error: pFunc" << endl;
		std::system("pause");
		//return -96;
	}

	//cout << 3 << endl;

	//�����python�������Ĳ���
	
	pArgs = PyTuple_New(6);//����һ��Ԫ�飬����Ϊ2��
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", type));
	PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", inp_c));//��pArgs�ĵ�һ��0������������Ϊ����(i) ��������ģ����ò���
	PyTuple_SetItem(pArgs, 2, Py_BuildValue("i", out_c));
	PyTuple_SetItem(pArgs, 3, Py_BuildValue("i", img_h));
	PyTuple_SetItem(pArgs, 4, Py_BuildValue("i", img_w));
	PyTuple_SetItem(pArgs, 5, Py_BuildValue("i", stride));

	//ʹ��C++��python�ӿڵ��øú��� ����ȡ���ز���
	//pReturn = PyObject_CallObject(pFunc, pArgs);
	//pReturn = PyObject_CallObject(pFunc, NULL);
	//pArgs = PyTuple_New(1);//����һ��Ԫ�飬����Ϊ1��
	//PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", "test C++ use python'func"));//��pArgs�ĵ�һ��0������������Ϊ�ַ���(s) ��this is a test�����ò���
	pReturn = PyObject_CallObject(pFunc, pArgs);

	//����python����õķ���ֵ,���ж�ȡ��ת����C++���ͣ�
	//����һ��ֵ��
	double nResult;
	PyArg_Parse(pReturn, "d", &nResult);
	cout << "return result is " << nResult << endl;

	//��������ֵ
	//double nResult1, nResult2;
	//// i��ʾת����int�ͱ�����
	//// d��double �ͱ���
	//// ���������Ҫע����ǣ�PyArg_Parse�����һ��������������ϡ�&������ ����������������
	//PyArg_ParseTuple(pReturn, "d|d", &nResult1, &nResult2);//�������ص�Ԫ��ֵ
	//cout << "return result is " << nResult1 << nResult2 << endl;

	//��һ��ʵ��
	/*double nResult[2];
	PyObject* pValu;
	Py_ssize_t nums;
	nums = PyTuple_Size(pReturn);
	for (Py_ssize_t i;i < nums;i++)
	{
		pValu = PyTuple_GetItem(pReturn, i);
		nResult[i] = PyFloat_AsDouble(pValu);
	}*/

	//����python���
	//Py_DECREF(pName);
	Py_DECREF(pModule);
	Py_Finalize();
}

//��������
void cost_reset2(int nums)
{
	//��ʼ��python������
	Py_Initialize();
	if (!Py_IsInitialized()) {
		std::system("pause");
		//	return -99;
	} //�鿴python�������Ƿ�ɹ���ʼ��

	//��������
	PyObject* pName, * pModule, * pDict, * pFunc, * pArgs, * pReturn; //����python����

	//����python�ļ�����Ϊһ��ģ�飬һ��������ý���
	pName = PyUnicode_FromString("combined_cost_eval"); //��һ������Ӧ���Ƿ�ֹ��������
	pModule = PyImport_Import(pName); //�����ģ��
	if (!pModule) {
		cout << "Can't find pModule" << endl;
		std::system("pause");
		//return -98;
	}//�鿴�Ƿ�������ģ��

	//����python�����������ö���pmodule�ĺ���������ȡ������pmodule.print_test2()  xgb_tran1
	pFunc = PyObject_GetAttrString(pModule, "xgb_reset");
	if (!pFunc || !PyCallable_Check(pFunc)) {
		cout << "error: pFunc" << endl;
		std::system("pause");
		//return -96;
	}

	//cout << 3 << endl;

	//�����python�������Ĳ���

	pArgs = PyTuple_New(1);//����һ��Ԫ�飬����Ϊ1��
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("i", nums));
	//PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", throughput));//��pArgs�ĵ�һ��0������������Ϊ����(i) ��������ģ����ò���


	//ʹ��C++��python�ӿڵ��øú��� ����ȡ���ز���
	//pReturn = PyObject_CallObject(pFunc, pArgs);
	//pReturn = PyObject_CallObject(pFunc, NULL);
	//pArgs = PyTuple_New(1);//����һ��Ԫ�飬����Ϊ1��
	//PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", "test C++ use python'func"));//��pArgs�ĵ�һ��0������������Ϊ�ַ���(s) ��this is a test�����ò���
	pReturn = PyObject_CallObject(pFunc, pArgs);

	//����python����õķ���ֵ,���ж�ȡ��ת����C++���ͣ�
	//����һ��ֵ��
	double nResult;
	PyArg_Parse(pReturn, "d", &nResult);
	cout << "return result is " << nResult << endl;

	//��������ֵ
	//double nResult1, nResult2;
	//// i��ʾת����int�ͱ�����
	//// d��double �ͱ���
	//// ���������Ҫע����ǣ�PyArg_Parse�����һ��������������ϡ�&������ ����������������
	//PyArg_ParseTuple(pReturn, "d|d", &nResult1, &nResult2);//�������ص�Ԫ��ֵ
	//cout << "return result is " << nResult1 << nResult2 << endl;

	//��һ��ʵ��
	/*double nResult[2];
	PyObject* pValu;
	Py_ssize_t nums;
	nums = PyTuple_Size(pReturn);
	for (Py_ssize_t i;i < nums;i++)
	{
		pValu = PyTuple_GetItem(pReturn, i);
		nResult[i] = PyFloat_AsDouble(pValu);
	}*/

	//����python���
	//Py_DECREF(pName);
	Py_DECREF(pModule);
	Py_Finalize();
}
void cost_reset1(PyObject* pModule, int nums)
{


	//��������
	PyObject* pDict, * pFunc, * pArgs, * pReturn; //����python����


	//����python�����������ö���pmodule�ĺ���������ȡ������pmodule.print_test2()  xgb_tran1
	pFunc = PyObject_GetAttrString(pModule, "xgb_reset");
	if (!pFunc || !PyCallable_Check(pFunc)) {
		cout << "error: pFunc" << endl;
		std::system("pause");
		//return -96;
	}

	//cout << 3 << endl;

	//�����python�������Ĳ���

	pArgs = PyTuple_New(1);//����һ��Ԫ�飬����Ϊ1��
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("i", nums));
	//PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", throughput));//��pArgs�ĵ�һ��0������������Ϊ����(i) ��������ģ����ò���


	//ʹ��C++��python�ӿڵ��øú��� ����ȡ���ز���
	//pReturn = PyObject_CallObject(pFunc, pArgs);
	//pReturn = PyObject_CallObject(pFunc, NULL);
	//pArgs = PyTuple_New(1);//����һ��Ԫ�飬����Ϊ1��
	//PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", "test C++ use python'func"));//��pArgs�ĵ�һ��0������������Ϊ�ַ���(s) ��this is a test�����ò���
	pReturn = PyObject_CallObject(pFunc, pArgs);

	//����python����õķ���ֵ,���ж�ȡ��ת����C++���ͣ�
	//����һ��ֵ��
	double nResult;
	PyArg_Parse(pReturn, "d", &nResult);
	cout << "return result is " << nResult << endl;

	//��������ֵ
	//double nResult1, nResult2;
	//// i��ʾת����int�ͱ�����
	//// d��double �ͱ���
	//// ���������Ҫע����ǣ�PyArg_Parse�����һ��������������ϡ�&������ ����������������
	//PyArg_ParseTuple(pReturn, "d|d", &nResult1, &nResult2);//�������ص�Ԫ��ֵ
	//cout << "return result is " << nResult1 << nResult2 << endl;

	//��һ��ʵ��
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
