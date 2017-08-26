
/*******************************************************************
** �ļ���:	spinLock.h
** ������:	�Ľ���
** ��  ��:	2015-06-27
** ��  ��:	1.0
** ��  ��:	������
** Ӧ  ��: 
**************************** �޸ļ�¼ ******************************
** �޸���: 
** ��  ��: 
** ��  ��: 
********************************************************************/

#pragma once
#include <intrin.h>     // _ReadWriteBarrier(), InterlockedCompareExchange()

extern "C" void _ReadWriteBarrier(void);
#pragma intrinsic(_ReadWriteBarrier)

#ifndef spin_val_compare_and_swap32
#define spin_val_compare_and_swap32(destPtr, oldValue, newValue)        \
	InterlockedCompareExchange((volatile LONG *)(destPtr),              \
	(LONG)(newValue), (LONG)(oldValue))
#endif

#ifndef spin_bool_compare_and_swap32
#define spin_bool_compare_and_swap32(destPtr, oldValue, newValue)       \
	(InterlockedCompareExchange((volatile LONG *)(destPtr),             \
	(LONG)(newValue), (LONG)(oldValue))         \
	== (LONG)(oldValue))
#endif

// ����������
struct spin_mutex_s
{
#define SLEEP_SPIN_TIME		50				// ������������
	enum _SpinState
	{
		_StateUnLock	= 0,
		_StateLock		= 1,
		_StateSleep		= SLEEP_SPIN_TIME,
	};
	//*/
	spin_mutex_s():spinState(_StateUnLock){};	
	spin_mutex_s(size_t n):spinState(_StateUnLock){};
	void UnLock()		// ����
	{		
		spinState = _StateUnLock;
		_ReadWriteBarrier();
	}
	void Lock()			// ����
	{			
		spinState = _StateLock;
		_ReadWriteBarrier();
	}
	bool isLocked()		// �Ƿ�����
	{			
		return spinState != _StateUnLock;
	}

	// bSleepImmed: false(ʧ�ܺ���N����sleep) true(ʧ�ܾ�sleep)
	bool TryGetLock(bool bSleepImmed = true)	// ���Ի����
	{
		bool bWriteSuc = false;
		while(!bWriteSuc)
		{
			size_t tStateTemp = spinState;
			if (tStateTemp == _StateUnLock)
			{
				_ReadWriteBarrier();
				bWriteSuc = spin_bool_compare_and_swap32(&spinState, tStateTemp, _StateLock);
				if (bWriteSuc)
				{
					_ReadWriteBarrier();
					return true;
				}
			}
			else if (bSleepImmed || tStateTemp >= _StateSleep)
			{
				bWriteSuc = spin_bool_compare_and_swap32(&spinState, tStateTemp, _StateLock);
				if (bWriteSuc)
				{
					Sleep(1);
					return false;
				}
			}
			else
			{
				bWriteSuc = spin_bool_compare_and_swap32(&spinState, tStateTemp, tStateTemp+1);
				if (bWriteSuc)
				{
					return false;
				}
			}			
		}
		return false;
	}

protected:
	volatile size_t spinState;
};
