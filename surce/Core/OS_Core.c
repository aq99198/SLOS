/*
*********************************************************************************************************
*                                          Sword Light
*
*                                       ��³�����ƴ���
*
*                               ����ְҵѧԺ08������ع�Ӳ�����Ʒ
* 
* �ļ�����	��SLOS_Core.c
* ��������	��2009/5/14
* ����		��������
* ˵��		��ϵͳ�йؼ����㷨����ʵ��
*********************************************************************************************************
*/
#ifndef quan_ju
    #define quan_ju
#endif

#include "Includes.h"	//����ϵͳ��ͷ�ļ�

/*
****************************************************************************************
*
*                              ϵͳ�����Ķ���
* 
****************************************************************************************
*/

INT8U   const  OSUnMapTbl[256] = {
    0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
};

/*
****************************************************************************************
*                                     ϵͳ�Ŀ�������
*
* ��������	��SLOS_free_task(void *pdd)
* ��������	��2009/5/19
* �޸�����	��2009/8/8
* ˵��		������ϵͳ�Ŀ�������������ȼ����У���ϵͳ�����ڼ�ʼ�մ���
* ����		��	pdd		:��������Ĳ���
* ����		����
****************************************************************************************
*/
void SLOS_free_task(void *pdd)
{
	#if OS_CRITICAL_METHOD == 3			//�������ж�ʱ����cpu״̬�Ĵ����ı���
    OS_CPU_SR  cpu_sr;
	#endif 

	pdd = pdd;
	
	while(1){
		OS_ENTER_CRITICAL();
		free_task_count++;				//���������������ǰ����ȫ�ֵļ������ϵĽ��м�һ
		OS_EXIT_CRITICAL();

		#if CPU_HOOKS_EN > 0
		OSTaskIdleHook();
		#endif
	}
}

/*
****************************************************************************************
*                                     ϵͳ��ͳ������
*
* ��������	��SLOS_state_task()
* ��������	��2009/7/21
* �޸�����	��2009/7/21
* ˵��		������ϵͳ��ͳ�����񡣴�������ȼ����У���ͨ������ѡ���Ƿ�����
* ����		����
* ����		����
****************************************************************************************
*/
#if TASK_STATA_EN > 0
void SLOS_state_task(void *pdd)
{	
	pdd = pdd;

	while (OS_Stat_Rdy == FALSE) {
        taskSleep(2 * OS_TICKS_PER_SEC);
    }
	
	
	while(1){		
		cpu_Usage = (INT8S)(100 - free_task_count*100/cpu_count_max);
		if(cpu_Usage < 0)
			cpu_Usage = 1;
		if(cpu_Usage > 100)
			cpu_Usage = 100;
		free_task_count = 0;

		#if CPU_HOOKS_EN > 0
		OSTaskStatHook();
		#endif
		
		taskSleep(OS_TICKS_PER_SEC);
	}
}
#endif

/*
****************************************************************************************
*                                     ϵͳ��ͳ�������ʼ��
*
* ��������	��SLOS_state_init()
* ��������	��2009/7/21
* �޸�����	��2009/7/21
* ˵��		������ϵͳ��ͳ�����񡣴�������ȼ����У���ͨ������ѡ���Ƿ�����
* ����		����
* ����		����
****************************************************************************************
*/
#if TASK_STATA_EN > 0
void SLOS_state_init()
{
	#if OS_CRITICAL_METHOD == 3		//�������ж�ʱ����cpu״̬�Ĵ����ı���
    OS_CPU_SR  cpu_sr;
	#endif 
	
	OS_ENTER_CRITICAL();
	free_task_count = 0;			//Ϊ��ȡcpuʹ������׼��
	OS_EXIT_CRITICAL();
	taskSleep(OS_TICKS_PER_SEC);
	OS_ENTER_CRITICAL();
	cpu_count_max= free_task_count;
	OS_EXIT_CRITICAL();
	OS_Stat_Rdy = TRUE;	
}
#endif

/*
****************************************************************************************
*                                     ϵͳ�ĳ�ʼ��
*
* ��������	��INT8S taskInit(void)
* ��������	��2009/5/15
* �޸�����	��2009/7/30
* ˵��		����ʼ������ϵͳ�����������񻷾�. �ú������ú�����ϵͳ���������
* ����		����			
* ����		��	SLOS_ERR_NO_ERR(0)		�ɹ�
*				SLOS_ERR_FAIL(-1)		ʧ��
****************************************************************************************
*/
INT8S taskInit()	//ϵͳ��ʼ��
{  	
	INT16U i;				//��ʼ��������ʱʹ��
	INT8U j;				//��ʼ����λͼʱʹ��
	OS_TCB		*ptcb1;		//����ѭ�����������õ�������ʱTCBָ��
	OS_TCB		*ptcb2;	
	#if OS_EVENT_COUNT > 1
	OS_EVENT	*pevent1;	//����ѭ�����������õ�������ʱEVENTָ��
	OS_EVENT	*pevent2;
	#endif
	#if MIN_PRIO<= 63		//��ʼ������λͼ��
    INT8U   	*prdytbl;	//����һ��ָ�����λͼ���ָ��
	#else
    INT16U  	*prdytbl;
	#endif
	INT16U		tid;		//������룬��ȡ��������ķ���ֵ

	#if CPU_HOOKS_EN > 0
	OSInitHookBegin();
	#endif
	
	SLOS_tcb_delay_list		= (OS_TCB*)0;
	SLOS_tcb_suspend_list	= (OS_TCB*)0;
	OSTCBCur				= (OS_TCB*)0;
	OSTCBHighRdy			= (OS_TCB*)0;	
	OSRunning				= FALSE;
	SLOS_Prio_Cur     		= 0; 			//��ǰ���������ȼ���ʼ��Ϊ0
	OSIntNesting 			= 0;	

	for(i = 0;i < MIN_PRIO + 1;i++){		//��ʼ���ּ�����ı�ͷΪ0
		SLOS_tcb_by_prio[i] = (OS_TCB*)0;
	}

	SLOS_tcb_free_list	= SLOS_tcb;			//��ʼ������TCB�����ͷָ��
	ptcb1				= &SLOS_tcb[0];		//֮��������������Ϊһ��������ϵͳ���ٻ�����������
	ptcb2				= &SLOS_tcb[1];
	ptcb1->TaskId		= 0;
	ptcb1->State		= OS_STATE_FREE;
	for(i = 0;i < TASK_COUNT - 1;i++){		//��������TCB����
		ptcb2->TaskId	= i + 1;
		ptcb2->State	= OS_STATE_FREE;
		ptcb1->NextTcb	= ptcb2;
		ptcb1++;
		ptcb2++;		
	}
	
	#if OS_EVENT_EN >0						//��ʼ���¼����ƿ�����,�����Ҫ�Ļ�
	SLOS_event_free_list = &SLOS_event[0];	//��ʼ�����������ͷָ��	
	#if OS_EVENT_COUNT > 1					//ֻ�����ж���1���¼����ƿ�ʱ���б�Ҫ�齨����
	pevent1	= &SLOS_event[0];
	pevent2	= &SLOS_event[1];
	pevent1->EventId = 0;
	for(i = 0;i < OS_EVENT_COUNT - 1;i++){	//���������¼����ƿ�����
		pevent2->EventId	= i + 1;
		pevent1->OSEventPtr	= pevent2;
		pevent1++;
		pevent2++;
	}
	#else									//���ֻ��һ���¼����ƿ�,��ôֻ��򵥵Ĵ����¼���
	SLOS_event_free_list->EventId = 0;
	SLOS_event_free_list->OSEventPtr = (OS_EVENT *)0;
	#endif
	#endif
	
	OSRdyGrp      = 0;						//��������־���Լ�����λͼȫ����ʼ��Ϊ0
    prdytbl       = &OSRdyTbl[0];
    for (j = 0; j < OS_RDY_TBL_SIZE; j++) {
        *prdytbl++ = 0;
    }
	
	tid = taskCreate((void *)"free task", (void *)SLOS_free_task, FREE_TASK_STK_SIZE, MIN_PRIO);	//������������
	if(tid < 0){
		return SLOS_ERR_FAIL;
	}
	taskStart((unsigned char)tid);
	
	#if TASK_STATA_EN > 0	
	tid = taskCreate((void *)"state task", (void *)SLOS_state_task, STATE_TASK_STK_SIZE, MIN_PRIO - 1);
	if(tid < 0){
		return SLOS_ERR_FAIL;
	}
	taskStart((unsigned char)tid);
	#endif

	#if CPU_HOOKS_EN > 0
	OSInitHookEnd();
	#endif

	return SLOS_ERR_NO_ERR;
}
/*
****************************************************************************************
*                                     ����һ������
*
* ��������	��INT16U taskCreate(char *name, void *entryPtr, INT16U stack_size, INT16U priority)
* ��������	��2009/5/15
* �޸�����	��2009/7/31
* ˵��		������һ������
* ����		��	name			: ��������
*				entryPtr		: ���������
*				stack_size		: ����ջ��С
*				priority		: �������ȼ�
*			
* ����		��������tid, >0: �ɹ�, ����: ʧ��
****************************************************************************************
*/
INT16U taskCreate(INT8U *name, void *entryPtr, INT16U stack_size, INT8U priority)
{	
	OS_TCB		*ptcb;
	OS_STK		*pstk;
	INT16U		index;											//ѭ������
	#if OS_CRITICAL_METHOD == 3									//�������ж�ʱ����cpu״̬�Ĵ����ı���
    OS_CPU_SR  cpu_sr;
	#endif 
		
	stack_size = stack_size;

	OS_ENTER_CRITICAL();
	ptcb = SLOS_tcb_init((void (*)(void *))entryPtr, (INT8U *)name, priority);	//��ʼ������TCB�ṹ
	if(ptcb == (OS_TCB *)0){
		OS_EXIT_CRITICAL();
		return SLOS_ERR_NO_TCB;
	}

	#if TASK_STATA_EN > 0										//���������Ҫ���ж�ջ��⣬Ӧ������Ķ�ջȫ������
	for(index = 0;index < TASK_STATIC_SIZE;index++){
		SLOS_stk[ptcb->TaskId][index] = (OS_STK)0;
	}
	#endif


	#if STATIC_MALLOC_EN > 0									//���ϵͳ���þ�̬�ڴ����

	#if OS_STK_GROWTH == 1										//�����ջ�ǵݼ��ģ������ָ�뵽��ջ����
	pstk = &SLOS_stk[ptcb->TaskId][TASK_STATIC_SIZE - 1];
	#else
	pstk = &SLOS_stk[ptcb->TaskId][0];							//����tcb��tid�õ���ջ����ջ�����tcb���������Ӧ��	
	#endif

	#else														//���ϵͳ���ö�̬�ڴ����

	#if OS_STK_GROWTH == 1										//�����ջ�ǵݼ��ģ������ָ�뵽��ջ����
	pstk = (OS_STK *)((INT16U)malloc(stack_size) + stack_size);
	#if OS_TASK_DEL_EN > 0										//��������ɾ�������¼�õ��Ķ�ջ��ַ�����ͷ��ڴ�ʱ��
	ptcb->StkPtr = (void *)((INT16U)pstk - stack_size);					//ʹ��malloc�õ��Ķ�ջ��ַ�����ͷŶ�ջ��
	#endif
	#else
	pstk = (OS_STK *)malloc(stack_size);						//����tcb��tid�õ���ջ����ջ�����tcb���������Ӧ��	
	#if OS_TASK_DEL_EN > 0										//��������ɾ�������¼�õ��Ķ�ջ��ַ�����ͷ��ڴ�ʱ��
	ptcb->StkPtr = (void *)pstk;								//ʹ��malloc�õ��Ķ�ջ��ַ�����ͷŶ�ջ��
	#endif
	#endif
	

	#endif
	

	
	pstk = OSTaskStkInit((void (*)(void *))entryPtr, (void *)0, (OS_STK *)pstk, (INT16U)priority);	//��ʼ�������ջ

	ptcb->OSTCBStkPtr = pstk;									//����TCB�еĶ�ջָ��	

	#if TASK_STATA_EN > 0
	task_count++;
	#endif
	
	OS_EXIT_CRITICAL();

	#if CPU_HOOKS_EN > 0
	OSTaskCreateHook(ptcb);
	#endif

	return ptcb->TaskId;										//�����ôҪת��?��Ŀ���⡣ƽ̨�����
}

/*
****************************************************************************************
*                                    ����һ������
*
* ��������	��INT8S taskStart(unsigned char tid)
* ��������	��2009/8/1
* �޸�����	��2009/8/1
* ˵��		������һ������, ��taskCreat����������μӵ���
* ����		��	tid		:��Ҫ���������������ID���		
* ����		��	SLOS_ERR_NO_ERR(0)		�ɹ�
*				SLOS_ERR_FAIL(-1)		ʧ��
****************************************************************************************
*/
INT8S taskStart(INT16U tid){
	#if OS_CRITICAL_METHOD == 3					//�������ж�ʱ����cpu״̬�Ĵ����ı���
    OS_CPU_SR  cpu_sr;
	#endif 
	OS_TCB	*ptcb;

	OS_ENTER_CRITICAL();
	ptcb	= &SLOS_tcb[tid];
	if(ptcb->State != OS_STATE_CREATED){
		OS_EXIT_CRITICAL();
		return SLOS_ERR_FAIL;
	}
	slos_insert_ready(ptcb);					//��������뵽�����ּ�����		
	OS_EXIT_CRITICAL();

	if(OSRunning){								//ֻ��ϵͳ����������ʱ�Ž��е���		
		SLOS_task_sched();						//�����������
		return SLOS_ERR_NO_ERR;	
	}

	#if TASK_STATA_EN == 0						//�ж��Ƿ��Ѿ�����ϵͳ���������״̬������
	if(ptcb->Prio < MIN_PRIO){
		SLOS_start();	
	}
	#endif
	#if TASK_STATA_EN > 0
	if(ptcb->Prio < MIN_PRIO - 1){
		SLOS_start();	
	}
	#endif
	
	return SLOS_ERR_FAIL;						//��Ϊϵͳ��û�н��������״̬��������������ʧ�ܣ����Ѿ�����ּ�����������
}


/*
****************************************************************************************
*                                     ɾ��һ������
*
* ��������	��INT8S taskDelete(unsigned char tid)
* ��������	��2009/7/31
* �޸�����	��2009/7/31
* ˵��		��������ɾ�� �������˳������һ�������������Դ
* ����		��	tid			: ����ı��ID
* ����		��	SLOS_ERR_NO_ERR(0)		�ɹ�
*				SLOS_ERR_FAIL(-1)		ʧ��
****************************************************************************************
*/
#if OS_TASK_DEL_EN > 0
INT8S	taskDelete(INT16U tid)
{
	#if OS_CRITICAL_METHOD == 3			//�������ж�ʱ����cpu״̬�Ĵ����ı���
    OS_CPU_SR	cpu_sr;
	#endif
	OS_TCB		*tcbdelete;
	#if OS_EVENT_EN > 0
	OS_EVENT	*pevent;
	#endif

	OS_ENTER_CRITICAL();
	tcbdelete = &SLOS_tcb[tid];	
	

	if(tcbdelete->Prio == MIN_PRIO){	//���Ҫɾ�����ǿ������������
		return SLOS_ERR_FAIL;
	}
	

	#if CPU_HOOKS_EN > 0
	OSTaskDelHook(tcbdelete);
	#endif

	#if OS_EVENT_EN > 0
	if(tcbdelete->State == OS_STATE_MUTEX || tcbdelete->State == OS_STATE_MBOX){	//�����TCB���¼����ƿ�ĵȴ�������
		pevent = tcbdelete->Eventptr;
		pevent->OSEventTbl[tcbdelete->OSTCBY] &= ~tcbdelete->OSTCBBitX;				//����¼����ƿ�ĵȴ�λͼ
	    if (pevent->OSEventTbl[tcbdelete->OSTCBY] == 0) {
	       	pevent->OSEventGrp &= ~tcbdelete->OSTCBBitY;
	    }	
		//����ʱ������ɾ����TCB
		if(tcbdelete->LastTcb != (OS_TCB *)0){						//�ж��ǲ�������ĵ�һ��Ԫ��
			tcbdelete->LastTcb->NextTcb = tcbdelete->NextTcb; 		//������ǵ�һ��
		}else{
			SLOS_tcb_delay_list = tcbdelete->NextTcb;				//����ǵ�һ����Ҫ������ͷָ��
		}
		if(tcbdelete->NextTcb != (OS_TCB *)0){						//�ж��ǲ�����������һ��Ԫ��
			tcbdelete->NextTcb->LastTcb = tcbdelete->LastTcb; 		//����ǣ��Ͳ���������һ��Ԫ�ص�LastTcbָ��
		}	
		//�����¼����ƿ�ĵȴ�����	
		if(tcbdelete->ELastTcb != (OS_TCB *)0){
			tcbdelete->ELastTcb->ENextTcb = tcbdelete->ENextTcb;
		}else{
			pevent->SLOS_tcb_by_prio[tcbdelete->Prio] = tcbdelete->ENextTcb;
		}
		if(tcbdelete->ENextTcb != (OS_TCB *)0){
			tcbdelete->ENextTcb->ELastTcb = tcbdelete->ELastTcb;
		}
	}
	#endif	
	

	#if TASK_DELAY_EN > 0
	if(tcbdelete->State == OS_STATE_DEALY){						//�����TCB������ʱ����������ʱ����
		if(tcbdelete->LastTcb != (OS_TCB *)0){					//�ж��ǲ�������ĵ�һ��Ԫ��
			tcbdelete->LastTcb->NextTcb = tcbdelete->NextTcb; 	//������ǵ�һ��
		}else{
			SLOS_tcb_delay_list = tcbdelete->NextTcb;			//����ǵ�һ����Ҫ������ͷָ��
		}
		if(tcbdelete->NextTcb != (OS_TCB *)0){					//�ж��ǲ�����������һ��Ԫ��
			tcbdelete->NextTcb->LastTcb = tcbdelete->LastTcb; 	//����ǣ��Ͳ���������һ��Ԫ�ص�LastTcbָ��
		}
	}
	#endif

	if(tcbdelete->State == OS_STATE_READY){						//������������ھ���״̬������������ּ�����
		slos_out_ready(tcbdelete);								//��������ּ�����
	}	

	#if TASK_STATA_EN > 0
	task_count--;												//����ϵͳ���������
	#endif

	#if STATIC_MALLOC_EN == 0									//����ڴ��Ƕ�̬����ģ���˴���Ҫ�ͷ��ڴ�
	free(tcbdelete->StkPtr);											//ʹ��malloc�õ��Ķ�ջ��ַ�����ͷŶ�ջ��
	#endif
	
	tcbdelete->State   = OS_STATE_FREE;
	tcbdelete->NextTcb = SLOS_tcb_free_list;					//���뵽����tcb����
	SLOS_tcb_free_list = tcbdelete;
	
	OS_EXIT_CRITICAL();
	
	SLOS_task_sched();											//�����������

	return SLOS_ERR_NO_ERR;
}
#endif

/*
****************************************************************************************
*                                     ����һ������
*
* ��������	��INT8S taskSuspend(unsigned char tid)
* ��������	��2009/5/17
* �޸�����	��2009/7/31
* ˵��		����һ��������𡣴Ӿ��������ƶ�����������
* ����		��	tid		:��Ҫ����������tcbָ��		
* ����		��	SLOS_ERR_NO_ERR(0)		�ɹ�
*				SLOS_ERR_FAIL(-1)		ʧ��
****************************************************************************************
*/
#if TASK_SUSPEND_EN > 0
INT8S taskSuspend(INT16U tid)
{	
	#if OS_CRITICAL_METHOD == 3									//�������ж�ʱ����cpu״̬�Ĵ����ı���
    OS_CPU_SR  cpu_sr;
	#endif
	#if OS_EVENT_EN > 0
	OS_EVENT	*pevent;
	#endif
	OS_TCB	*tcbsuspend;
	
	OS_ENTER_CRITICAL();
	tcbsuspend	= &SLOS_tcb[tid];
	OS_EXIT_CRITICAL();

	if(tcbsuspend->State == OS_STATE_SUSPEND){
		return SLOS_ERR_FAIL;									//��������Ѿ��ǹ���״̬�ˣ��򷵻ش���
	}

	if(tcbsuspend->Prio == MIN_PRIO){
		return SLOS_ERR_FAIL;									//��������ǿ��������򷵻ش���
	}

	if(tcbsuspend->State == OS_STATE_READY){					//�������״̬�����񣬳��ּ���������	
		OS_ENTER_CRITICAL();
		slos_out_ready(tcbsuspend);								//����������ּ���������
		OS_EXIT_CRITICAL();		
	}

	OS_ENTER_CRITICAL();

	#if TASK_DELAY_EN > 0
	if(tcbsuspend->State == OS_STATE_DEALY){ 					//�����TCB������ʱ����������ʱ����
		if(tcbsuspend->LastTcb != (OS_TCB *)0){					//�ж��ǲ�������ĵ�һ��Ԫ��
			tcbsuspend->LastTcb->NextTcb = tcbsuspend->NextTcb;	//������ǵ�һ��
		}else{
			SLOS_tcb_delay_list = tcbsuspend->NextTcb;			//����ǵ�һ����Ҫ������ͷָ��
		}
		if(tcbsuspend->NextTcb != (OS_TCB *)0){					//�ж��ǲ�����������һ��Ԫ��
			tcbsuspend->NextTcb->LastTcb = tcbsuspend->LastTcb;	//����ǣ��Ͳ���������һ��Ԫ�ص�LastTcbָ��
		}
	}
	#endif


	#if OS_EVENT_EN > 0
	if(tcbsuspend->State == OS_STATE_MUTEX || tcbsuspend->State == OS_STATE_MBOX){	//�����TCB���¼����ƿ�ĵȴ�������
		pevent = tcbsuspend->Eventptr;
		pevent->OSEventTbl[tcbsuspend->OSTCBY] &= ~tcbsuspend->OSTCBBitX; 			//����¼����ƿ�ĵȴ�λͼ
		if (pevent->OSEventTbl[tcbsuspend->OSTCBY] == 0) {
			pevent->OSEventGrp &= ~tcbsuspend->OSTCBBitY;
		}	
		//����ʱ������ɾ����TCB
		if(tcbsuspend->LastTcb != (OS_TCB *)0){						//�ж��ǲ�������ĵ�һ��Ԫ��
			tcbsuspend->LastTcb->NextTcb = tcbsuspend->NextTcb;		//������ǵ�һ��
		}else{
			SLOS_tcb_delay_list = tcbsuspend->NextTcb;				//����ǵ�һ����Ҫ������ͷָ��
		}
		if(tcbsuspend->NextTcb != (OS_TCB *)0){						//�ж��ǲ�����������һ��Ԫ��
			tcbsuspend->NextTcb->LastTcb = tcbsuspend->LastTcb;		//����ǣ��Ͳ���������һ��Ԫ�ص�LastTcbָ��
		}	
		//�����¼����ƿ�ĵȴ�����	
		if(tcbsuspend->ELastTcb != (OS_TCB *)0){
			tcbsuspend->ELastTcb->ENextTcb = tcbsuspend->ENextTcb;
		}else{
			pevent->SLOS_tcb_by_prio[tcbsuspend->Prio] = tcbsuspend->ENextTcb;
		}
		if(tcbsuspend->ENextTcb != (OS_TCB *)0){
			tcbsuspend->ENextTcb->ELastTcb = tcbsuspend->ELastTcb;
		}
	}
	#endif

	
	tcbsuspend->State 		= OS_STATE_SUSPEND;			//�޸�����״̬Ϊ����״̬
	
	tcbsuspend->LastTcb		= (OS_TCB *)0;				//Ϊ������������׼��
	tcbsuspend->NextTcb		= SLOS_tcb_suspend_list;	//��TCB���뵽����TCB����
	if(tcbsuspend->NextTcb != (OS_TCB *)0){				//���ԭ���Ĺ��������Ѿ��й���TCB��
		tcbsuspend->NextTcb->LastTcb = tcbsuspend;
	}
	SLOS_tcb_suspend_list	= tcbsuspend;
	
	OS_EXIT_CRITICAL();
	

	
	SLOS_task_sched();									//�����������

	return SLOS_ERR_NO_ERR;
}
#endif

/*
****************************************************************************************
*                                    �ָ������һ������
*
* ��������	��SLOS_task_resume(OS_TCB *tcbsuspend)
* ��������	��2009/5/17
* �޸�����	��2009/7/31
* ˵��		����һ�����������ָ����ӹ��������ƶ�����������
* ����		��	tid		:��Ҫ�ָ�������������tid	
* ����		��	SLOS_ERR_NO_ERR(0)		�ɹ�
*				SLOS_ERR_FAIL(-1)		ʧ��
****************************************************************************************
*/
#if TASK_SUSPEND_EN > 0
INT8S taskResume(INT16U tid)
{
  	#if OS_CRITICAL_METHOD == 3								//�������ж�ʱ����cpu״̬�Ĵ����ı���
    OS_CPU_SR  cpu_sr;
	#endif 
	OS_TCB	*tcbresume;

	OS_ENTER_CRITICAL();
	tcbresume	= &SLOS_tcb[tid];
	OS_EXIT_CRITICAL();
	
	if(tcbresume->State != OS_STATE_SUSPEND){				//ֻ�����Ѿ������������ſ��Խ��лָ�
		return SLOS_ERR_FAIL;
	}	
	
	OS_ENTER_CRITICAL();	

	if(tcbresume->LastTcb != (OS_TCB*)0){					//��Ҫ�ָ���tcb�ӹ�������������
		tcbresume->LastTcb->NextTcb = tcbresume->NextTcb;
	}else{
		SLOS_tcb_suspend_list = tcbresume->NextTcb;			//��һ��Ϊ0��˵����������ĵ�һ��������Ҫ��������ͷָ����һ��
	}
	if(tcbresume->NextTcb != (OS_TCB*)0){
		tcbresume->NextTcb->LastTcb = tcbresume->LastTcb;
	}
	
	slos_insert_ready(tcbresume);							//���뵽����������

	tcbresume->State		= OS_STATE_READY;				//��TCB��״̬����Ϊ����״̬
	
	OS_EXIT_CRITICAL();
	
	SLOS_task_sched();

	

	return SLOS_ERR_NO_ERR;

}
#endif
/*
****************************************************************************************
*                                   ��ʱһ������
*
* ��������	��INT8S taskSleep(INT16U ticks)
* ��������	��2009/5/29
* �޸�����	��2009/7/31
* ˵��		������˯��(��æ��), �������Լ�����, ��������һ��ʱ��. ʱ�䵥λΪtick, ��10����
* ����		��	ticks	:Ҫ��ʱ��ʱ��ĳ���,��λΪtick			
* ����		��	SLOS_ERR_NO_ERR(0)		�ɹ�
*				SLOS_ERR_FAIL(-1)		ʧ��
****************************************************************************************
*/
#if TASK_DELAY_EN > 0
INT8S taskSleep(INT16U ticks)
{
	#if OS_CRITICAL_METHOD == 3					//�������ж�ʱ����cpu״̬�Ĵ����ı���
    OS_CPU_SR  cpu_sr;
	#endif 
	INT16U	time;

	if(ticks == 0){								//�����ʱʱ����0��������Զ���ỽ�ѣ����ǲ�����ġ�����ʱ0Ҳ���ǲ���ʱ������ֱ�ӷ��ء� Ϊ0��Զ��ʱ��Ϊ�����ź�����Ƶ�
		return SLOS_ERR_FAIL;
	}
	
	OS_ENTER_CRITICAL();
	time = (INT16U)ticks;
	
	slos_out_ready(OSTCBCur);					//����ǰ���������������

	OSTCBCur->State = 0;						//���������״̬
	OSTCBCur->State = OS_STATE_DEALY;
	
	
	OSTCBCur->LastTcb = (OS_TCB *)0;			//Ϊ����ʱ������׼��
	OSTCBCur->NextTcb = SLOS_tcb_delay_list;	//���뵽��ʱ������		
	
	if(OSTCBCur->NextTcb != (OS_TCB *)0){		//���ԭ������ʱ�����Ѿ�����ʱtcb��
		OSTCBCur->NextTcb->LastTcb = OSTCBCur;	//����˫������
	}
	SLOS_tcb_delay_list = OSTCBCur;

	OSTCBCur->Delay = time;						//������ʱʱ��

	OS_EXIT_CRITICAL();

	SLOS_task_sched();							//�����������

	

	return SLOS_ERR_NO_ERR;
	
}
#endif

/*
****************************************************************************************
*                                    �������
*
* ��������	��SLOS_task_sched()
* ��������	��2009/5/17
* �޸�����	��2009/5/17
* ˵��		���жϵ�ǰ�����Ƿ�����������ȼ������ǵĻ����е�������������ȼ�������
* ����		����
* ����		����
****************************************************************************************
*/
void SLOS_task_sched()
{
	#if OS_CRITICAL_METHOD == 3								//�������ж�ʱ����cpu״̬�Ĵ����ı���
    OS_CPU_SR  cpu_sr;
	#endif 
	
	OS_ENTER_CRITICAL();
	
	SLOS_get_heighready();									//�ҵ���ǰ������ȼ�������

	if(OSTCBCur != SLOS_tcb_by_prio[SLOS_Prio_highready]){	//ֻ�е�������ȼ������ǵ�ǰ���������²Ž��������л�

		#if CPU_HOOKS_EN > 0
		OSTaskSwHook();
		#endif
	
		OSTCBHighRdy = SLOS_tcb_by_prio[SLOS_Prio_highready];

		#if TASK_STATA_EN > 0
		task_switch_count++;								//���񽻻��ļ���ֵ
		#endif	
		
		OS_TASK_SW();										//�����������
	}

	OS_EXIT_CRITICAL();
}

/*
****************************************************************************************
*                                     ��ʼ��TCB�ṹ
*
* ��������	��SLOS_tcb_init()
* ��������	��2009/5/15
* �޸�����	��2009/5/15
* ˵��	����ʼ�������TCB�ṹ�������񴴽�����ʱ����
* ����	��	task	:�����ִ�д����ַ
*			stk	:��ջ�ĵ�ַ
*			prio	:��������ȼ�
* ����	����
****************************************************************************************
*/
OS_TCB * SLOS_tcb_init(void (*task)(void *pd), INT8U *name, INT8U prio)
{
	OS_TCB	*ptcb;										//��ʱ�õ�TCBָ��
	INT8U	i;	
	
	ptcb = SLOS_tcb_free_list;							//��TCB����������ȡ��һ��TCB�ṹ
	if(ptcb == (OS_TCB*)0){								//���TCB�б���ȡ�õ�tcb�Ƿ�Ϊ�գ������tcb�б��Ƿ�Ϊ��
		return (OS_TCB *)0;		
	}
	
	SLOS_tcb_free_list = ptcb->NextTcb;					//��tcb�����б�ȡ��һ��tcb�ṹ�󣬽�tcb�����б�ı�ͷָ��ָ����һ��tcb
	
	ptcb->Prio				= prio;						//��TCB�м�¼���ȼ�
	ptcb->State				= OS_STATE_CREATED;			//��TCB������״̬��Ϊ�Ѵ���
	
	#if TASK_SCHED_TIME_EN > 0
	ptcb->TimeLen			= TASK_TIME_LEN;			//��TCB�м�¼ͬ����ת��ʱ��Ƭ�����
	ptcb->CurTime			= TASK_TIME_LEN;			//��TCB�м�¼ͬ����ת��ʣ�¶���ʱ��
	#endif	
	
	#if MIN_PRIO<= 63
    ptcb->OSTCBY         = (INT8U)(prio >> 3);			//����x,y��־λ
    ptcb->OSTCBBitY      = (INT8U)(1 << ptcb->OSTCBY);
    ptcb->OSTCBX         = (INT8U)(prio & 0x07);
    ptcb->OSTCBBitX      = (INT8U)(1 << ptcb->OSTCBX);
	#else
    ptcb->OSTCBY         = (INT8U)((prio >> 4) & 0xFF);	//����x,y��־λ
    ptcb->OSTCBBitY      = (INT16U)(1 << ptcb->OSTCBY);
    ptcb->OSTCBX         = (INT8U)(prio & 0x0F);
    ptcb->OSTCBBitX      = (INT16U)(1 << ptcb->OSTCBX);
	#endif    
	
	#if TASK_DELAY_EN > 0
	ptcb->Delay			= 0;							//��TCB�м�¼��ʱʱ��
	#endif

	for(i = 0;i < TASK_NAME_LEN;i++){					//����name�������������ֵ�TCB��
		ptcb->Name[i] = *name;
		if(*name == '\0'){
			break;
		}
		name++;
	}
	ptcb->Name[TASK_NAME_LEN - 1] = '\0';				//��ֹ���

	#if CPU_HOOKS_EN > 0
	OSTCBInitHook(ptcb);
	#endif

	return ptcb;	
}

/*
****************************************************************************************
*                                    ��ʼϵͳ������
*
* ��������	��SLOS_start()
* ��������	��2009/5/16
* �޸�����	��2009/5/16
* ˵��		��ѡ��������ȼ������񣬲����С�ʹϵͳ���������״̬
* ����		����
* ����		����
****************************************************************************************
*/
void SLOS_start()
{	
	SLOS_get_heighready();
	OSTCBCur = SLOS_tcb_by_prio[SLOS_Prio_highready];		//���õ�ǰ�������е�����TCBָ��
	OSTCBHighRdy = SLOS_tcb_by_prio[SLOS_Prio_highready];	//���ü������е�����TCBָ�룬��OSStartHighRdy()�õ�	
	OSStartHighRdy();										//����Ӳ����صĻ����뿪ʼ������ȼ�������
}

/*
****************************************************************************************
*                                    ʱ���жϴ�����
*
* ��������	��OSTimeTick()
* ��������	��2009/5/28
* �޸�����	��2009/5/28
* ˵��		��ѡ��������ȼ������񣬲����С�ʹϵͳ���������״̬
* ����		����
* ����		����
****************************************************************************************
*/
void OSTimeTick()
{
	#if OS_CRITICAL_METHOD == 3							//�������ж�ʱ����cpu״̬�Ĵ����ı���
    OS_CPU_SR  cpu_sr;
	#endif	

	OS_TCB *ptcb;
	#if TASK_DELAY_EN > 0								//��������ʱ������м���
	OS_TCB *ptcb2;
	#endif

	#if TASK_STATA_EN > 0
	time_tick_count++;	
	#endif
	
	ptcb = SLOS_tcb_delay_list;

	#if TASK_DELAY_EN > 0 || OS_EVENT_EN > 0			//��������ʱ������м���
	OS_ENTER_CRITICAL();
	while(ptcb != (OS_TCB *)0){							//ѭ������������ʱ�����ݼ�ÿ��Ԫ�ص�Delay������ʱ����
	
		if(ptcb->Delay == 0){							//���delay���õ���0�Ļ���������Զ����ʱ�����軽��
			ptcb = ptcb->NextTcb;
			continue;					
		}
			
		if(--ptcb->Delay == 0){							//���������ʱʱ�䣬��������ʱ����
		
			if(ptcb->LastTcb != (OS_TCB *)0){			//�ж��ǲ�������ĵ�һ��Ԫ��
				ptcb->LastTcb->NextTcb = ptcb->NextTcb;	//������ǵ�һ��
			}else{
				SLOS_tcb_delay_list = ptcb->NextTcb;	//����ǵ�һ����Ҫ������ͷָ��
			}
			if(ptcb->NextTcb != (OS_TCB *)0){			//�ж��ǲ�����������һ��Ԫ��
				ptcb->NextTcb->LastTcb = ptcb->LastTcb;	//����ǣ��Ͳ���������һ��Ԫ�ص�LastTcbָ��
			}

			#if OS_EVENT_EN >0
			if(ptcb->State != OS_STATE_DEALY){
				ptcb->Request = SLOS_ERR_TIME_OUT;
			}
			#endif
			
 			ptcb2 = ptcb->NextTcb;						//��һ��tcb

			slos_insert_ready(ptcb);					//���뵽��������
			ptcb = ptcb2;
		}else{
			ptcb = ptcb->NextTcb;						//��һ��tcb	//֮���Ժ��Ͼ���Ͼ�һ��������Ϊ����Ѿ����뵽��������ôptcb��nexttcb���Ǿ������е�next��		
		}
	}	
	#endif

	#if TASK_SCHED_TIME_EN > 0							//��ͬ����ת������м���	
	if(--OSTCBCur->CurTime == 0){						//����ͳ��ʱ��Ƭ��תʣ��ʱ��ļ��㣬�����ǰ�����Ѿ�������һ��ʱ��Ƭ
		if(OSTCBCur->NextTcb != OSTCBCur){				//�����ǰ������ͬ���ȼ�����������		
			SLOS_tcb_by_prio[OSTCBCur->Prio] = OSTCBCur->NextTcb;			
		}
		OSTCBCur->CurTime = TASK_TIME_LEN;
	}	
	#endif

	#if CPU_HOOKS_EN > 0
	OSTimeTickHook();
	#endif

	OS_EXIT_CRITICAL();
}

/*
****************************************************************************************
*                                    ʱ���жϴ�����
*
* ��������	��OSIntEnter()
* ��������	��2009/7/16
* �޸�����	��2009/7/16
* ˵��		�������жϴ���ǰ����
* ����		����
* ����		����
****************************************************************************************
*/
void OSIntEnter()
{
	#if OS_CRITICAL_METHOD == 3					//�������ж�ʱ����cpu״̬�Ĵ����ı���
    OS_CPU_SR  cpu_sr;
	#endif

	OS_ENTER_CRITICAL();
    if (OSIntNesting < 255) {
        OSIntNesting++;							//�����жϼ���
    }
    OS_EXIT_CRITICAL();
}

/*
****************************************************************************************
*                                    ʱ���жϴ�����
*
* ��������	��OSIntExit()
* ��������	��2009/7/16
* �޸�����	��2009/7/16
* ˵��		���жϴ�������
* ����		����
* ����		����
****************************************************************************************
*/
void OSIntExit()
{
	#if OS_CRITICAL_METHOD == 3									//�������ж�ʱ����cpu״̬�Ĵ����ı���
    OS_CPU_SR  cpu_sr;
	#endif

	if (OSIntNesting > 0) {										//�����жϼ���
    	OSIntNesting--;
    }
	
	if(OSIntNesting == 0){
		SLOS_get_heighready();									//�ҵ���ǰ������ȼ��ľ�������
		if(OSTCBCur != SLOS_tcb_by_prio[SLOS_Prio_highready]){	//ֻ�е�������ȼ������ǵ�ǰ���������²Ž��������л�
			OS_ENTER_CRITICAL();
			
			#if CPU_HOOKS_EN > 0
			OSTaskSwHook();			
			#endif
			
			OSTCBHighRdy = SLOS_tcb_by_prio[SLOS_Prio_highready];

			#if TASK_STATA_EN > 0
			task_switch_count++;								//���񽻻��ļ���ֵ
			#endif	

			OS_EXIT_CRITICAL();
			
			OSIntCtxSw();										//�����������
			
		}
	}
}


/*
****************************************************************************************
*                                 ��һ��tcb���뵽��������
*
* ��������	��slos_insert_ready(OS_TCB *ptcb)
* ��������	��2009/5/29
* �޸�����	��2009/5/29
* ˵��		����һ��tcb���뵽�ּ���������
* ����		��	ptcb:	TCB�ṹ��ָ��
* ����		����
****************************************************************************************
*/
void slos_insert_ready(OS_TCB *ptcb)
{
	INT8U	prio;
	OS_TCB	*ptcbtem;

	prio = ptcb->Prio;							//��ȡҪ������������������ȼ�
	ptcbtem = SLOS_tcb_by_prio[prio];			//��ȡ��Ӧ���ȼ���ѭ�����������ͷָ��
	
	if(ptcbtem == (OS_TCB *)0){
		ptcb->NextTcb = ptcb;
		ptcb->LastTcb = ptcb;
		SLOS_tcb_by_prio[prio] = ptcb;		
	}else{
		ptcb->NextTcb = ptcbtem->NextTcb;		//��tcb���뵽��ͷ��ָ���tcb����һ��λ�ã�������ָ���λ�á���������ʹ���ȸ��Ӻ���
		ptcb->LastTcb = ptcbtem;
		ptcbtem->NextTcb->LastTcb = ptcb;
		ptcbtem->NextTcb = ptcb;
	}

	ptcb->State = OS_STATE_READY;

	OSRdyGrp               |= ptcb->OSTCBBitY;	//������������λͼ
    OSRdyTbl[ptcb->OSTCBY] |= ptcb->OSTCBBitX;

}

/*
****************************************************************************************
*                                 ��һ��tcb�����������
*
* ��������	��slos_out_ready(OS_TCB *ptcb)
* ��������	��2009/5/29
* �޸�����	��2009/5/29
* ˵��		����һ��tcb����ּ���������
* ����		��	ptcb:	TCB�ṹ��ָ��
* ����		����
****************************************************************************************
*/
void slos_out_ready(OS_TCB *ptcb)
{
	INT8U prio;
	INT8U y;	

 	prio = ptcb->Prio;	
	
	if(ptcb->NextTcb == ptcb){					//�����ǰ���ȼ���ѭ��������ֻ����һ��tcb��
		SLOS_tcb_by_prio[prio] = (OS_TCB *)0;	//���ڽ����������ʱҪ��SLOS_tcb_by_prio[prio]��OSTCBCur���жԱȣ��������ﻹ��Ҫ�������õ�		
		
		y            = ptcb->OSTCBY;			//��վ���λͼ�ϵı��
    	OSRdyTbl[y] &= ~ptcb->OSTCBBitX;                      
    	if (OSRdyTbl[y] == 0) {
        	OSRdyGrp &= ~ptcb->OSTCBBitY;
    	}
	}else{
		ptcb->NextTcb->LastTcb = ptcb->LastTcb;
		ptcb->LastTcb->NextTcb = ptcb->NextTcb;

		if(SLOS_tcb_by_prio[prio] == ptcb){
			SLOS_tcb_by_prio[prio] = ptcb->NextTcb;
		}
	}

}

/*
****************************************************************************************
*                          �õ���ǰ������ȼ��ľ�������
*
* ��������	��SLOS_get_heighready()
* ��������	��2009/7/8
* �޸�����	��2009/7/8
* ˵��		���õ���ǰ������ȼ��ľ������񲢽���ֵSLOS_Prio_highready
* ����		����			
* ����		����
****************************************************************************************
*/
void SLOS_get_heighready()
{			   	
	#if OS_CRITICAL_METHOD == 3                      //�������ж�ʱ����cpu״̬�Ĵ����ı���
	    OS_CPU_SR  cpu_sr;
	#endif 
	
	#if MIN_PRIO <= 63								//����ܵ����ȼ���С��64�������8*8�ľ���λͼ
    INT8U   y;
    y             = OSUnMapTbl[OSRdyGrp];
	OS_ENTER_CRITICAL();
    SLOS_Prio_highready = (INT8U)((y << 3) + OSUnMapTbl[OSRdyTbl[y]]);
	#else											//����ܵ����ȼ�������64�������16*16�ľ���λͼ
    INT8U   y;
    INT16U *ptbl;
	OS_ENTER_CRITICAL();
    if ((OSRdyGrp & 0xFF) != 0) {
        y = OSUnMapTbl[OSRdyGrp & 0xFF];
    } else {
        y = OSUnMapTbl[(OSRdyGrp >> 8) & 0xFF] + 8;
    }
    ptbl = &OSRdyTbl[y];
    if ((*ptbl & 0xFF) != 0) {
        SLOS_Prio_highready = (INT8U)((y << 4) + OSUnMapTbl[(*ptbl & 0xFF)]);
    } else {
        SLOS_Prio_highready = (INT8U)((y << 4) + OSUnMapTbl[(*ptbl >> 8) & 0xFF] + 8);
    }
	#endif
	OS_EXIT_CRITICAL();
}

/*
****************************************************************************************
*                          ��ʼ��ʱ����ƿ�ĵȴ�����λͼ
*
* ��������	��SLOS_event_waitlist_init()
* ��������	��2009/7/9
* �޸�����	��2009/7/9
* ˵��		����ʼ��ʱ����ƿ�ĵȴ�����λͼ��ȫ�����Ϊ0
* ����		��	pevent:	Ҫ���г�ʼ�����¼����ƿ�
* ����		����
****************************************************************************************
*/
#if OS_EVENT_EN	> 0
void	SLOS_event_waitlist_init(OS_EVENT *pevent)
{
	#if OS_CRITICAL_METHOD == 3                      //�������ж�ʱ����cpu״̬�Ĵ����ı���
    OS_CPU_SR  cpu_sr;
	#endif 
	
	#if MIN_PRIO <= 63
    INT8U  *ptbl;
	#else
    INT16U *ptbl;
	#endif
    INT8U   i;

	OS_ENTER_CRITICAL();
	pevent->OSEventGrp	= 0;
	ptbl				= &pevent->OSEventTbl[0];

	for (i = 0; i < OS_EVENT_TBL_SIZE; i++) {
        *ptbl++ = 0;
    }
	OS_EXIT_CRITICAL();
}
#endif

/*
****************************************************************************************
*                          ʹһ���������ȴ�ĳ�¼�����״̬
*
* ��������	��SLOS_event_task_wait()
* ��������	��2009/7/9
* �޸�����	��2009/8/1
* ˵��		������ǰ������������������¼����ƿ�ĵȴ�����������ʱ�����������¼����ƿ�ĵȴ�λͼ
* ����		��	pevent:	�¼����ƿ�			
* ����		����
****************************************************************************************
*/
#if OS_EVENT_EN	> 0
void SLOS_event_task_wait(OS_EVENT *pevent)
{
	#if OS_CRITICAL_METHOD == 3                      //�������ж�ʱ����cpu״̬�Ĵ����ı���
    OS_CPU_SR  cpu_sr;
	#endif 
	

	OS_ENTER_CRITICAL();

	slos_out_ready(OSTCBCur);						//����ǰ����������������
	

	OSTCBCur->ELastTcb = (OS_TCB *)0;				//����ǰ���������ƿ�ĵȴ�����
	OSTCBCur->ENextTcb = pevent->SLOS_tcb_by_prio[OSTCBCur->Prio];	
	if(OSTCBCur->ENextTcb != (OS_TCB *)0){			//���ԭ���ĵȴ������Ѿ�����ʱtcb��
		OSTCBCur->ENextTcb->ELastTcb = OSTCBCur;	//����˫������
	}
	pevent->SLOS_tcb_by_prio[OSTCBCur->Prio] = OSTCBCur;
	

	OSTCBCur->LastTcb = (OS_TCB *)0;				//Ϊ������ʱ������׼��
	OSTCBCur->NextTcb = SLOS_tcb_delay_list;		//���뵽��ʱ������		
	if(OSTCBCur->NextTcb != (OS_TCB *)0){			//���ԭ������ʱ�����Ѿ�����ʱtcb��
		OSTCBCur->NextTcb->LastTcb = OSTCBCur;		//����˫������
	}
	SLOS_tcb_delay_list = OSTCBCur;					//���뵽��ʱ����

	OSTCBCur->Eventptr = pevent;					//�޸�������¼����ƿ�ָ�룬ɾ������ʱ���õ�	

	pevent->OSEventGrp |= OSTCBCur->OSTCBBitY;		//����������¼����ƿ�ȴ�λͼ
	pevent->OSEventTbl[OSTCBCur->OSTCBY] |= OSTCBCur->OSTCBBitX;
	
	OS_EXIT_CRITICAL();
	
}
#endif

/*
****************************************************************************************
*                          ʹһ������������״̬
*
* ��������	��SLOS_event_task_ready()
* ��������	��2009/7/9
* �޸�����	��2009/8/1
* ˵��		����������ȼ��ĵȴ���������¼����ƿ�ĵȴ�λͼ��������ʱ���������¼����ƿ�ĵȴ��������������
* ����		��	pevent:	�¼����ƿ�
*				pmsg:	Ҫ���ݵ���Ϣ				
*			
* ����		����
****************************************************************************************
*/
#if OS_EVENT_EN	> 0
void SLOS_event_task_ready(OS_EVENT *pevent, void *pmsg)
{
	#if OS_CRITICAL_METHOD == 3			//�������ж�ʱ����cpu״̬�Ĵ����ı���
    OS_CPU_SR  cpu_sr;
	#endif 
	
	INT8U	hprio;
	OS_TCB	*ptcb;
	#if MIN_PRIO <= 63
	INT8U   y;
	#else
	INT16U	y;
	INT16U *ptbl;
	#endif

	OS_ENTER_CRITICAL();
	
	if(pevent->OSEventGrp == 0){		//����Ѿ�û�еȴ���������

		#if TASK_STATA_EN > 0			//���������Ϣͳ��
		pevent->State = OS_MUTEX_STATE_FREE;
		#endif
	
		pevent->OSEventCnt = 0x00FF;	//����Ϊ�գ�������־�Ѿ�û������ʹ�ô˻������ź���
		OS_EXIT_CRITICAL();
		return;
	}
	
	//�õ�������ȼ��ĵȴ�����
	#if MIN_PRIO <= 63					//����ܵ����ȼ���С��64�������8*8�ľ���λͼ    
    y             = OSUnMapTbl[pevent->OSEventGrp];
    hprio = (INT8U)((y << 3) + OSUnMapTbl[pevent->OSEventTbl[y]]);
	#else 
    if ((pevent->OSEventGrp & 0xFF) != 0) {
        y = OSUnMapTbl[pevent->OSEventGrp & 0xFF];
    } else {
        y = OSUnMapTbl[(pevent->OSEventGrp >> 8) & 0xFF] + 8;
    }
    ptbl = &pevent->OSEventTbl[y];
    if ((*ptbl & 0xFF) != 0) {
        hprio= (INT8U)((y << 4) + OSUnMapTbl[(*ptbl & 0xFF)]);
    } else {
        hprio= (INT8U)((y << 4) + OSUnMapTbl[(*ptbl >> 8) & 0xFF] + 8);
    }
	#endif	
	ptcb = pevent->SLOS_tcb_by_prio[hprio];

	

	//����¼����ƿ�ĵȴ�λͼ	
	pevent->OSEventTbl[ptcb->OSTCBY] &= ~ptcb->OSTCBBitX;		//����¼����ƿ�ĵȴ�λͼ
    if (pevent->OSEventTbl[ptcb->OSTCBY] == 0) {
       	pevent->OSEventGrp &= ~ptcb->OSTCBBitY;
    }	
	
	//������ʱ����
	if(ptcb->LastTcb != (OS_TCB *)0){							//�ж��ǲ�������ĵ�һ��Ԫ��
		ptcb->LastTcb->NextTcb = ptcb->NextTcb;					//������ǵ�һ��
	}else{
		SLOS_tcb_delay_list = ptcb->NextTcb;					//����ǵ�һ����Ҫ������ͷָ��
	}
	if(ptcb->NextTcb != (OS_TCB *)0){							//�ж��ǲ�����������һ��Ԫ��
		ptcb->NextTcb->LastTcb = ptcb->LastTcb;					//����ǣ��Ͳ���������һ��Ԫ�ص�LastTcbָ��
	}

	//�����¼����ƿ�ĵȴ�����	
	if(ptcb->ELastTcb != (OS_TCB *)0){
		ptcb->ELastTcb->ENextTcb = ptcb->ENextTcb;
	}else{
		pevent->SLOS_tcb_by_prio[hprio] = ptcb->ENextTcb;		//����Ƿּ�����ĵ�һ������˵ȼ������ͷָ��Ӧָ���TCB����һ��
	}
	if(ptcb->ENextTcb != (OS_TCB *)0){
		ptcb->ENextTcb->ELastTcb = ptcb->ELastTcb;
	}

	OS_EXIT_CRITICAL();

	ptcb->State = 0;											//�޸�����״̬��־
	ptcb->State = OS_STATE_READY;

	ptcb->Request = SLOS_ERR_NO_ERR;							//�޸ı�־Ϊ��������

	#if MBOX_EN> 0
	if(pevent->OSEventType == OS_EVENT_TYPE_MBOX){				//���������Ļ��⴫����Ϣ������
		pevent->OSEventPtr = pmsg;
	}	
	#endif

	
	#if MUTEX_EN > 0	
	
	if(pevent->OSEventType == OS_EVENT_TYPE_MUTEX){						//�����¼����ƿ��ָ�룬ָ��ǰ�õ����ź�����TCB���Ա��������ȼ�ʱ�õ���ǰռ���ź�����TCB
		pevent->OSEventCnt = (INT16U)(ptcb->Prio | (ptcb->Prio << 8));	//�����¼����ƿ����¼��ԭ���ȼ�������������ȼ�	
		pevent->OSEventPtr = (void *)ptcb;
	}	
	#endif

	//���������
	slos_insert_ready(ptcb);
	
}
#endif

/*
****************************************************************************************
*                                     ��ȡ��������ȼ�
*
* ��������	��INT8U taskPriorityGet(INT16U tid)
* ��������	��2009/7/30
* �޸�����	��2009/7/31
* ˵��		����ȡ��������ȼ�
* ����		��	tid		:������		
* ����		����������ȼ�
****************************************************************************************
*/
#if GET_SET_PRIO_EN	> 0
INT8U taskPriorityGet(INT16U tid)
{
	OS_TCB *ptcb;
	ptcb = &SLOS_tcb[tid];		//��TCB�����еõ���Ӧ��tcb
	return ptcb->TaskId;	//������Ӧtcb��tid,Ϊ��Ӧ���Ͻӿ�Ҫ����ת��
}
#endif

/*
****************************************************************************************
*                                     ������������ȼ�
*
* ��������	��INT8S taskPrioritySet(INT16U tid, INT8U newPriority)
* ��������	��2009/7/30
* �޸�����	��2009/7/31
* ˵��		�������������ȼ�, ���ڶ�̬����
* ����		��	tid				:������
*				newPriority		:�µ����ȼ�
* ����		��	SLOS_ERR_NO_ERR(0)		�ɹ�
*				SLOS_ERR_FAIL(-1)		ʧ��
****************************************************************************************
*/
#if GET_SET_PRIO_EN	> 0
INT8S taskPrioritySet(INT16U tid, INT8U newPriority)
{
	OS_TCB	*ptcb;
	INT8U	newprio;

	ptcb	= &SLOS_tcb[tid];
	newprio	= (INT8U)newPriority;
	
	if(ptcb->State == OS_STATE_READY){							//ֻ�е������Ǿ����ģ��ſ��԰����������룬Ȼ����뵽���������У�������tcb�ڷּ����������е�λ��
		slos_out_ready(ptcb);									//��������������������

		#if MIN_PRIO<= 63										//�������øı����ȼ���������x��y��־λ
		ptcb->OSTCBY         = (INT8U)(newprio >> 3);			//����x,y��־λ
		ptcb->OSTCBBitY      = (INT8U)(1 << ptcb->OSTCBY);
		ptcb->OSTCBX         = (INT8U)(newprio & 0x07);
		ptcb->OSTCBBitX      = (INT8U)(1 << ptcb->OSTCBX);
		#else			
		ptcb->OSTCBY         = (INT8U)((newprio >> 4) & 0xFF);	//����x,y��־λ
		ptcb->OSTCBBitY      = (INT16U)(1 << ptcb->OSTCBY);
		ptcb->OSTCBX         = (INT8U)(newprio & 0x0F);
		ptcb->OSTCBBitX      = (INT16U)(1 << ptcb->OSTCBX);
		#endif
			
		ptcb->Prio = newprio;									//�ı����������ȼ�
			
		slos_insert_ready(ptcb);								//���������ȼ����������뵽�����������
	}else{														//������Ǿ����ģ���ôֻ��������ȼ�
		#if MIN_PRIO<= 63										//�������øı����ȼ���������x��y��־λ
		ptcb->OSTCBY         = (INT8U)(newprio >> 3);			//����x,y��־λ
		ptcb->OSTCBBitY      = (INT8U)(1 << ptcb->OSTCBY);
		ptcb->OSTCBX         = (INT8U)(newprio & 0x07);
		ptcb->OSTCBBitX      = (INT8U)(1 << ptcb->OSTCBX);
		#else
		ptcb->OSTCBY         = (INT8U)((newprio >> 4) & 0xFF);	//����x,y��־λ
		ptcb->OSTCBBitY      = (INT16U)(1 << ptcb->OSTCBY);
		ptcb->OSTCBX         = (INT8U)(newprio & 0x0F);
		ptcb->OSTCBBitX      = (INT16U)(1 << ptcb->OSTCBX);
		#endif		
			
		ptcb->Prio = newprio;									//�ı����������ȼ�
	}

	return SLOS_ERR_NO_ERR;
}
#endif


/*
****************************************************************************************
*                               ��ѯ����Ķ�ջʹ�����
*
* ��������	��void staticCheck(INT16U tid,OS_STKINFO *pstkinfo)
* ��������	��2009/8/5
* �޸�����	��2009/8/5
* ˵��		���������Ķ�ջʹ�����
* ����		��	tid				:������
*				pstkinfo			:ָ��OS_STKINFO�ṹ��ָ��
* ����		��	��
****************************************************************************************
*/
#if TASK_STATA_EN > 0
void staticCheck(INT16U tid,OS_STKINFO *pstkinfo)
{	
	INT16U	stkfree;
	OS_STK	*pstk;
	OS_TCB	*ptcb;

	ptcb	= &SLOS_tcb[tid];
	
	#if OS_STK_GROWTH ==0							//��ջ�ǵ�����
	pstk = &SLOS_stk[ptcb->TaskId][SLOS_stk_SIZE - 1];
	stkfree = 0;
	for(;pstk == (OS_STK *)0;pstk--){
		stkfree++;
	}
	#else											//��ջ�ǵݼ���
	pstk = &SLOS_stk[ptcb->TaskId][0];
	stkfree = 0;
	for(;*pstk == (OS_STK)0;pstk++){
		stkfree++;
	}
	#endif

	pstkinfo->StkFree = stkfree;
	pstkinfo->StkUsed = (INT16U)(TASK_STATIC_SIZE - stkfree);
}
#endif

/*
****************************************************************************************
*                                     ���񽻻���HOOK����
*
* ��������	��void OSTaskSwHook (void)
* ��������	��2009/8/2
* �޸�����	��2009/8/2
* ˵��		��Ϊ����ucos��Ӳ������������˺���
* ����		����
* ����		����
****************************************************************************************
*/
#if CPU_HOOKS_EN == 0  //��Ϊ��ucos��Os_cpu_a.s�л�Դ�HOOK�����������е��ã���SLOS��ֹhooksʱ���ṩ�˺���							
void OSTaskSwHook ()
{

}
#endif
