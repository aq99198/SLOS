/*
*********************************************************************************************************
*                                          Sword Light
*
*                                       ��³�����ƴ���
*
*                               ����ְҵѧԺ08������ع�Ӳ�����Ʒ
* 
* �ļ�����	��OS_MBOX.C
* ��������	��2009/7/10
* ����		�������
* ˵��		��ϵͳ����Ϣ������صĹ��ܺ���
*********************************************************************************************************
*/
#include "Includes.h"	//����ϵͳ��ͷ�ļ�


/*
****************************************************************************************
*                                    ��Ϣ����Ĵ���
*
* ��������	��SLOS_mbox_create()
* ��������	��2009/7/10
* �޸�����	��2009/7/10
* ˵��		������һ����Ϣ����
* ����		����
* ����		��һ��ָ���¼����ƿ��ָ��
****************************************************************************************
*/
#if MBOX_EN > 0
INT16U mboxCreate()
{
	#if OS_CRITICAL_METHOD == 3                      //�������ж�ʱ����cpu״̬�Ĵ����ı���
    OS_CPU_SR  cpu_sr;
	#endif 
	
	OS_EVENT *pevent;

	OS_ENTER_CRITICAL();
	
	pevent = SLOS_event_free_list;	//��event���ƿ���������л�ȡһ��event���ƿ�
	if (pevent == (OS_EVENT *)0) {
        return (INT16U)0;	//���event���������Ѿ�Ϊ�գ���ֱ�ӷ���
    }

	SLOS_event_free_list	= (OS_EVENT *)SLOS_event_free_list->OSEventPtr;

	pevent->OSEventType		= OS_EVENT_TYPE_MBOX;
	pevent->OSEventPtr		= (void *)0;

	#if OS_EVENT_NAME_SIZE > 1	
    pevent->OSEventName[0] = '?';			//���¼����ƿ�����ֽ��г�ʼ��
    pevent->OSEventName[1] = '\0';
	#endif

	OS_EXIT_CRITICAL();
	
	SLOS_event_waitlist_init(pevent);	//��pevent�ĵȴ�����λͼ������ճ�ʼ��
	

	return pevent->EventId;	
}
#endif

/*
****************************************************************************************
*                                   ����һ����Ϣ
*
* ��������	��SLOS_mbox_post()
* ��������	��2009/5/30
* �޸�����	��2009/5/30
* ˵��		����ָ�����䷢��һ����Ϣ
* ����		��	pevent	:	ָ�������ָ��
* 			��	msg		:	Ҫ���͵���Ϣ�ĵ�ַ	
* ����		��?SLOS_ERR_NO_ERR(0)		�ɹ�
*				SLOS_ERR_FAIL(-1)		ʧ��
****************************************************************************************
*/
#if MBOX_EN > 0
INT8S mboxPost(INT16U mid,void *msg)
{
	#if OS_CRITICAL_METHOD == 3                      //�������ж�ʱ����cpu״̬�Ĵ����ı���
    OS_CPU_SR  cpu_sr;
	#endif 
	
	OS_EVENT	*pevent;

	OS_ENTER_CRITICAL();

	pevent = &SLOS_event[mid];

	if(pevent->OSEventGrp != 0){	//����������ڵȴ���Ϣ
		SLOS_event_task_ready(pevent,msg);	//�õȴ���Ϣ���������
		OS_EXIT_CRITICAL();
		
		SLOS_task_sched();
		return SLOS_ERR_NO_ERR;	
	}

	if(pevent->OSEventPtr != (void *)0){	//����������Ѿ�����Ϣ,�������������������
		OS_EXIT_CRITICAL();
		return SLOS_ERR_MBOX_FUL;
	}

	pevent->OSEventPtr = msg;	//������û���ʼ�,��ֱ�Ӱ���Ϣ�ʼ�����������
	OS_EXIT_CRITICAL();
	return SLOS_ERR_NO_ERR;

}
#endif

/*
****************************************************************************************
*                                  �ȴ�һ����Ϣ
*
* ��������	��SLOS_mbox_pend()
* ��������	��2009/5/30
* �޸�����	��2009/5/30
* ˵��		���ȴ�һ����Ϣ��ָ��ʱ��Ȳ����Ļ������ش�����Ϣ
* ����		��	pevent	:�¼����ƿ��ָ��
*			��	timeout	:��ʱ���ص�ʱ��
*			��	err		:�Ƿ�ִ�г���
* ����		�����յ�����Ϣ
****************************************************************************************
*/
#if MBOX_EN > 0
void *mboxPend(INT16U mid,INT16U timeout,INT8U *err)
{
	#if OS_CRITICAL_METHOD == 3                      //�������ж�ʱ����cpu״̬�Ĵ����ı���
    OS_CPU_SR  cpu_sr;	
	#endif

	OS_EVENT	*pevent;
	void *msg;	
	
	OS_ENTER_CRITICAL();

	pevent = &SLOS_event[mid];

	msg = pevent->OSEventPtr;	//��ȡ�¼����ƿ��е���Ϣ
	
	if(msg != (void *)0){		//����Ѿ�����Ϣ��
		pevent->OSEventPtr = (void *)0;	//�����Ϣָ���Խ�����һ����Ϣ
		OS_EXIT_CRITICAL();	//�˳��ٽ��
		*err = SLOS_ERR_NO_ERR;	
		return msg;				//������Ϣָ��		
	}

	OSTCBCur->State = OS_STATE_MBOX;	//�ı�״̬,����ȴ���Ϣ״̬
	OSTCBCur->Delay = timeout;			//������ʱ
	SLOS_event_task_wait(pevent);		//�ѵ�ǰ������𵽵ȴ�����ĵȴ��ּ�����
	OS_EXIT_CRITICAL();

	SLOS_task_sched();

	OS_ENTER_CRITICAL();
	msg = pevent->OSEventPtr;	//��ȡ�¼����ƿ��е���Ϣ 
	
	if(msg != (void *)0){		//����ȴ��������յ�����Ϣ
		pevent->OSEventPtr = (void *)0;	//�����Ϣָ���Խ�����һ����Ϣ
		OS_EXIT_CRITICAL();	//�˳��ٽ��
		*err = SLOS_ERR_NO_ERR;	
		return msg;				//������Ϣָ��
	}

	
	*err = SLOS_ERR_TIME_OUT;	//��ʱ������

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
	return (void *)0;
}

#endif

