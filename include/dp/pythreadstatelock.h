#ifndef PYTHREADSTATELOCK_H
#define PYTHREADSTATELOCK_H
#include "python.h"

//����cost.h���̹߳�����

class PyThreadStateLock
{
public:
	PyThreadStateLock(void)
	{
		_save = nullptr;
		nStatus = 0;
		nStatus = PyGILState_Check();   //��⵱ǰ�߳��Ƿ�ӵ��GIL
		PyGILState_STATE gstate;
		if (!nStatus)
		{
			gstate = PyGILState_Ensure();   //���û��GIL���������ȡGIL
			nStatus = 1;
		}
		_save = PyEval_SaveThread();
		PyEval_RestoreThread(_save);
	}
	~PyThreadStateLock(void)
	{
		_save = PyEval_SaveThread();
		PyEval_RestoreThread(_save);
		if (nStatus)
		{
			PyGILState_Release(gstate);    //�ͷŵ�ǰ�̵߳�GIL
		}
	}

private:
	PyGILState_STATE gstate;
	PyThreadState* _save;
	int nStatus;

};



#endif // !PYTHREADSTATELOCK_H
