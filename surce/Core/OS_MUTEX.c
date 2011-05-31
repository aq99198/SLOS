/*
*********************************************************************************************************
*                                          Sword Light
*
*                                       ��³�����ƴ���
*
*                               ����ְҵѧԺ08������ع�Ӳ�����Ʒ
* 
* �ļ�����	��OS_MUTEX.C
* ��������	��2009/5/30
* ����		�������
* ˵��		��ϵͳ�и������ݽṹ��ȫ�ֱ����Ķ���
*********************************************************************************************************
*/
#include "Includes.h"	//����ϵͳ��ͷ�ļ�


/*
****************************************************************************************
*                                     �����ź�������
*
* ��������	��INT8U semCreate(void)
* ��������	��2009/5/30
* �޸�����	��2009/7/31
* ˵��		������һ���������ź���
* ����		����
* ����		���ź�����ʶsid, <0: ʧ��
****************************************************************************************
*/
#if MUTEX_EN > 0
INT16U semCreate()
{
	#if OS_CRITICAL_METHOD == 3                      //�������ж�ʱ����cpu״̬�Ĵ����ı���
    OS_CPU_SR  cpu_sr;
	#endif
	
	OS_EVENT *pevent;

	OS_ENTER_CRITICAL();
	pevent = SLOS_event_free_list;					//��event���ƿ���������л�ȡһ��event���ƿ�
	if (pevent == (OS_EVENT *)0) {
		OS_EXIT_CRITICAL();
        return SLOS_ERR_FAIL;						//���event���������Ѿ�Ϊ�գ���ֱ�ӷ���
    }

	SLOS_event_free_list = (OS_EVENT *)SLOS_event_free_list->OSEventPtr;
	OS_EXIT_CRITICAL();

	pevent->OSEventType = OS_EVENT_TYPE_MUTEX;
	pevent->OSEventCnt 	= 0x00FF;					//��ǰռ�д��ź�������������ȼ���������ff�����Կ�����ff��ʾ���ź�����ǰδ��ռ��
	pevent->OSEventPtr	= (void *)0;
	
	#if OS_EVENT_NAME_SIZE > 0						//�Ժ���չ��    
    pevent->OSEventName[0] = '\0';					//��ֹ��ȡʱ���
	#endif

	#if TASK_STATA_EN > 0							//���������Ϣͳ��
	pevent->State = OS_MUTEX_STATE_FREE;
	#endif
	
	SLOS_event_waitlist_init(pevent);				//��pevent�ĵȴ�����λͼ������ճ�ʼ��

	return pevent->EventId;							
}
#endif

/*
****************************************************************************************
*                                   �ȴ�һ���������ź���
*
* ��������	��INT8S semTake(INT16U sid)
* ��������	��2009/5/31
* �޸�����	��2009/5/31
* ˵��		���ȴ�һ���������ź���
* ����		��	sid	:�������ź������¼����ƿ�ı��ID
* ����		��	SLOS_ERR_NO_ERR(0)		�ɹ�
*				SLOS_ERR_FAIL(-1)		ʧ��
****************************************************************************************
*/
#if MUTEX_EN > 0
INT8S semTake(INT16U sid)
{
	#if OS_CRITICAL_METHOD == 3							 //�������ж�ʱ����cpu״̬�Ĵ����ı���
    OS_CPU_SR  cpu_sr;
	#endif 

	OS_EVENT	*pevent;
	INT8U 		mprio;									//mutex�ȴ���������ߵ����ȼ�
	OS_TCB		*ptcb;									//��ǰռ�д��ź�����tcb

	pevent = &SLOS_event[sid];

	if(pevent->OSEventType != OS_EVENT_TYPE_MUTEX){		//�����������sid����һ���������ź������¼����ƿ�
		return SLOS_ERR_FAIL;							//���ش���
	}

	OS_ENTER_CRITICAL();

	if((pevent->OSEventCnt & 0x00FF) == 0x00FF){		//�����ǰmutex��û�б�ռ��

		pevent->OSEventCnt = (INT16U)(OSTCBCur->Prio | (OSTCBCur->Prio << 8));
		pevent->OSEventPtr = OSTCBCur;

		#if TASK_STATA_EN > 0		//���������Ϣͳ��
		pevent->State = OS_MUTEX_STATE_TAKE;
		#endif
	
		OS_EXIT_CRITICAL();
		return SLOS_ERR_NO_ERR;

	}
	//pevent->OSEventCnt	��8λ�洢��Ȩ������ȼ�����8λ�洢ԭ�������ȼ�

	mprio	= (INT8U)(pevent->OSEventCnt >> 8);			//��ǰ����ռ��mutex����������ȼ�
	ptcb	= (OS_TCB *)pevent->OSEventPtr;				//��ǰ����ռ��mutex������

	if(OSTCBCur->Prio < mprio){							//���������ź�������������ȼ��ȵ�ǰռ�д��ź��������ȼ��ߣ���������ǰռ�д��ź�������������ȼ�
		
		pevent->OSEventCnt &= 0X00FF;					//�������������ȼ��洢��event���ƿ���
		pevent->OSEventCnt |= OSTCBCur->Prio << 8;		//�������������ȼ��洢��event���ƿ��� 
		
		if(ptcb->State == OS_STATE_READY){				//ֻ�е������Ǿ����ģ��ſ��԰������룬Ȼ����뵽���������У�������tcb�ڷּ������е�λ��
			slos_out_ready(ptcb);						//��ռ�д��ź���������������������

			#if MIN_PRIO<= 63							//�������øı����ȼ���������x��y��־λ
			ptcb->OSTCBY         = (INT8U)(OSTCBCur->Prio >> 3);			//����x,y��־λ
			ptcb->OSTCBBitY      = (INT8U)(1 << ptcb->OSTCBY);
			ptcb->OSTCBX         = (INT8U)(OSTCBCur->Prio & 0x07);
			ptcb->OSTCBBitX      = (INT8U)(1 << ptcb->OSTCBX);
			#else
			ptcb->OSTCBY         = (INT8U)((OSTCBCur->Prio >> 4) & 0xFF);	//����x,y��־λ
			ptcb->OSTCBBitY      = (INT16U)(1 << ptcb->OSTCBY);
			ptcb->OSTCBX         = (INT8U)(OSTCBCur->Prio & 0x0F);
			ptcb->OSTCBBitX      = (INT16U)(1 << ptcb->OSTCBX);
			#endif
				
			ptcb->Prio = OSTCBCur->Prio;									//����ռ�д��ź�������������ȼ�
			
			slos_insert_ready(ptcb);										//���������ȼ����������뵽�����������
		}else{																//������Ǿ����ģ���ôֻ��������ȼ�
			#if MIN_PRIO<= 63												//�������øı����ȼ���������x��y��־λ
			ptcb->OSTCBY         = (INT8U)(OSTCBCur->Prio >> 3);			//����x,y��־λ
			ptcb->OSTCBBitY      = (INT8U)(1 << ptcb->OSTCBY);
			ptcb->OSTCBX         = (INT8U)(OSTCBCur->Prio & 0x07);
			ptcb->OSTCBBitX      = (INT8U)(1 << ptcb->OSTCBX);
			#else
			ptcb->OSTCBY         = (INT8U)((OSTCBCur->Prio >> 4) & 0xFF);	//����x,y��־λ
			ptcb->OSTCBBitY      = (INT16U)(1 << ptcb->OSTCBY);
			ptcb->OSTCBX         = (INT8U)(OSTCBCur->Prio & 0x0F);
			ptcb->OSTCBBitX      = (INT16U)(1 << ptcb->OSTCBX);
			#endif
			
			ptcb->Prio = OSTCBCur->Prio;									//����ռ�д��ź�������������ȼ�			
			
		}
		
		
	}

	OSTCBCur->Delay = EVENT_DELAY;											//������ʱ	��ĿΪʲô��ȡ����ʱ����??
	OSTCBCur->State = OS_STATE_MUTEX;

	#if TASK_STATA_EN > 0													//���������Ϣͳ��
	pevent->State = OS_MUTEX_STATE_WAIT;
	#endif
		
	SLOS_event_task_wait(pevent);											//����ǰ������뵽�ȴ��ź���״̬
		
	SLOS_task_sched();														//�����������

	if(OSTCBCur->Request == SLOS_ERR_TIME_OUT){								//��Ϊ��ʱ����������
	
		//����¼����ƿ�ĵȴ�λͼ
		pevent->OSEventTbl[OSTCBCur->OSTCBY] &= ~OSTCBCur->OSTCBBitX;		//����¼����ƿ�ĵȴ�λͼ
		if (pevent->OSEventTbl[OSTCBCur->OSTCBY] == 0) {
		  	pevent->OSEventGrp &= ~OSTCBCur->OSTCBBitY;
		}

		//�����¼����ƿ�ĵȴ�����
		if(OSTCBCur->ELastTcb != (OS_TCB *)0){
			OSTCBCur->ELastTcb->ENextTcb = OSTCBCur->ENextTcb;
		}else{
			pevent->SLOS_tcb_by_prio[OSTCBCur->Prio] = OSTCBCur->ENextTcb;
		}
		if(OSTCBCur->ENextTcb != (OS_TCB *)0){
			OSTCBCur->ENextTcb->ELastTcb = OSTCBCur->ELastTcb;
		}
		OS_EXIT_CRITICAL();
		return SLOS_ERR_FAIL;
	}
	
	OS_EXIT_CRITICAL();														//��ȷ�õ��˻������ź���
	return SLOS_ERR_NO_ERR;
	
}
#endif

/*
****************************************************************************************
*                                   �ͷ�һ���������ź���
*
* ��������	��INT8S semGive(INT16U sid)
* ��������	��2009/5/31
* �޸�����	��2009/5/31
* ˵��		���ͷ�һ���������ź���
* ����		��	sid	:�������ź������¼����ƿ�ı��ID
* ����		��	SLOS_ERR_NO_ERR(0)		�ɹ�
*				SLOS_ERR_FAIL(-1)		ʧ��
****************************************************************************************
*/
#if MUTEX_EN > 0
INT8S semGive(INT16U sid)
{
	#if OS_CRITICAL_METHOD == 3												//�������ж�ʱ����cpu״̬�Ĵ����ı���
    OS_CPU_SR  cpu_sr;
	#endif	
	OS_EVENT	*pevent;
	
	pevent = &SLOS_event[sid];
	
	if(pevent->OSEventType != OS_EVENT_TYPE_MUTEX){							//�����������sid����һ���������ź������¼����ƿ�
		return SLOS_ERR_FAIL;												//���ش���
	}

	OS_ENTER_CRITICAL();

	if((pevent->OSEventCnt & 0x00FF) != (pevent->OSEventCnt >> 8)){			//�����������ȼ�������ˣ���Ӧ�ý���ԭ�������ȼ�
		slos_out_ready(OSTCBCur);											//�����������
		
		OSTCBCur->Prio = (INT8U)(pevent->OSEventCnt & 0x00FF);				//���͵�ԭ�������ȼ�
		
		#if MIN_PRIO<= 63													//�������øı����ȼ���������x��y��־λ
	    OSTCBCur->OSTCBY         = (INT8U)(OSTCBCur->Prio >> 3);			//����x,y��־λ
	    OSTCBCur->OSTCBBitY      = (INT8U)(1 << OSTCBCur->OSTCBY);
	    OSTCBCur->OSTCBX         = (INT8U)(OSTCBCur->Prio & 0x07);
	    OSTCBCur->OSTCBBitX      = (INT8U)(1 << OSTCBCur->OSTCBX);
		#else
	    OSTCBCur->OSTCBY         = (INT8U)((OSTCBCur->Prio >> 4) & 0xFF);	//����x,y��־λ
	    OSTCBCur->OSTCBBitY      = (INT16U)(1 << OSTCBCur->OSTCBY);
	    OSTCBCur->OSTCBX         = (INT8U)(OSTCBCur->Prio & 0x0F);
	    OSTCBCur->OSTCBBitX      = (INT16U)(1 << OSTCBCur->OSTCBX);
		#endif

		slos_insert_ready(OSTCBCur);										//���²����������
	}

	SLOS_event_task_ready(pevent,(void *)0);								//ʹһ���µĵȴ����������

	SLOS_task_sched();														//�����������

	OS_EXIT_CRITICAL();

	return SLOS_ERR_NO_ERR;
	
}
#endif
