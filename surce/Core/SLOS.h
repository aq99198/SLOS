/*
*********************************************************************************************************
*                                          Sword Light
*
*                                       ��³�����ƴ���
*
*                               ����ְҵѧԺ08������ع�Ӳ�����Ʒ
* 
* �ļ�����	��SLOS.H
* ��������	��2009/5/14
* ����		��������
* ˵��		��ϵͳ�и������ݽṹ��ȫ�ֱ����Ķ���
*********************************************************************************************************
*/

#include    "Includes.h"


/*
*********************************************************************************************************
*                                       �����궨��
*
*********************************************************************************************************
*/

/* �Ծ�����־���Լ�������־λͼ���ȵĶ���  */
#if MIN_PRIO <= 63
#define  OS_EVENT_TBL_SIZE ((MIN_PRIO) / 8 + 1)		//�¼��ȴ�����λͼ�Ĵ�С
#define  OS_RDY_TBL_SIZE   ((MIN_PRIO) / 8 + 1)		//���������λͼ�Ĵ�С
#else
#define  OS_EVENT_TBL_SIZE ((MIN_PRIO) / 16 + 1)	//�¼��ȴ�����λͼ�Ĵ�С
#define  OS_RDY_TBL_SIZE   ((MIN_PRIO) / 16 + 1)	//���������λͼ�Ĵ�С
#endif

/* OS_TCB->State �Ŀ�ѡֵ  */
#define  OS_STATE_READY          0x00				//�������
#define  OS_STATE_FREE           0x01				//���е�TCB
#define  OS_STATE_MBOX           0x02				//�ȴ�����
#define  OS_STATE_CREATED        0x04				//���񴴽�������δִ��taskStart()�������
#define  OS_STATE_SUSPEND        0x08				//�������
#define  OS_STATE_MUTEX          0x10				//�ȴ��������ź���	
#define  OS_STATE_DEALY          0x40				//������ʱ


/* OS_EVENT->OSEventType �Ŀ�ѡֵ  */
#define  OS_EVENT_TYPE_UNUSED		0u
#define  OS_EVENT_TYPE_MBOX			1u
#define  OS_EVENT_TYPE_Q			2u
#define  OS_EVENT_TYPE_SEM			3u
#define  OS_EVENT_TYPE_MUTEX		4u
#define  OS_EVENT_TYPE_FLAG			5u

/* OS_EVENT->State �Ŀ�ѡֵ  */ 
#define  OS_MUTEX_STATE_FREE		0u				//û�б�ʹ�ã�û�б�take
#define  OS_MUTEX_STATE_TAKE		1u				//��take������û����һ�������ڵȴ�
#define  OS_MUTEX_STATE_WAIT		2u				//�������ڵȴ�


/*         ���ִ������            */
#define SLOS_ERR_FAIL				-1
#define SLOS_ERR_NO_TCB        		-3

#define SLOS_ERR_NO_ERR        		0
#define SLOS_ERR_TIME_OUT			1
#define SLOS_ERR_MBOX_FUL			2

#ifndef TRUE
	#define TRUE	1
	#define FALSE	0
#endif

/*
*********************************************************************************************************
*                                       ������ƿ�
*						              OS_TCB
*********************************************************************************************************
*/

#ifndef SwordLight
#define SwordLight

typedef struct os_tcb
{

	OS_STK			*OSTCBStkPtr;			//��ǰTCB��ջ����ָ�룬����tcb��ǰ������ΪΪ���������ʱ����ȡ�ö�ջ��ַ	
	
	struct os_tcb	*NextTcb;				//����tcb���������õ���ָ��
	struct os_tcb	*LastTcb;				//����tcb���������õ���ָ��

	INT8U			Prio;					//��ǰ��������ȼ�
	INT8U			State;					//��ǰ���������״̬
	INT16U			TaskId;					//�������������id��
	INT8S			Name[TASK_NAME_LEN];	//��ǰ���������

	INT8U            OSTCBX;				//�ھ�����־���е�λ��
    INT8U            OSTCBY;				//�ھ���λͼ�е�λ��
	
	#if MIN_PRIO <= 63
	INT8U			 OSTCBBitX; 			//�ھ�����־����ı�־λ
	INT8U			 OSTCBBitY; 			//�ھ���λͼ��ı�־λ
	#else
	INT16U			 OSTCBBitX; 			//�ھ�����־����ı�־λ
	INT16U			 OSTCBBitY; 			//�ھ���λͼ��ı�־λ
	#endif

	#if OS_EVENT_EN >0
	struct os_tcb	*ENextTcb;				//����������ƿ�����ּ����õ���ָ��
	struct os_tcb	*ELastTcb;				//����������ƿ�����ּ����õ���ָ��
	INT8U			Request;				//�����Ƿ�õ������ǳ�ʱ
	struct os_event	*Eventptr;				//ָ���¼����ƿ��ָ�룬��־��TCB����mutex��mbox�ȵ�״̬
	#endif
	
	#if STATIC_MALLOC_EN == 0 && OS_TASK_DEL_EN == 1
	void			*StkPtr;				//ʹ��malloc�õ��Ķ�ջ��ַ�����ͷŶ�ջ��
	#endif
	
	#if TASK_DELAY_EN > 0
	INT16U			Delay;					//��ǰ�������ʱʱ��
	#endif

	#if TASK_SCHED_TIME_EN > 0				//�������ͬ��ʱ��Ƭ��ת
	INT8U			TimeLen;				//��ǰ����ÿ����תҪʹ�õ�ʱ��Ƭ����
	INT8U			CurTime;				//��ǰ����ǰ��ת��ʣ�¶���ʱ��Ƭ���˲�����ʼֵ��TimeLen��ͬ	
	#endif
	
	#if TASK_STATA_EN > 0					//�������������Ϣͳ��
	void			*statptr;
	#endif

}OS_TCB;

/*
*********************************************************************************************************
*                                       ��ջʹ����Ϣ�ṹ
*						               OS_STKINFO
*********************************************************************************************************
*/
typedef struct os_stkinfo {
	INT16U	StkUsed;
	INT16U	StkFree;
} OS_STKINFO;

/*
*********************************************************************************************************
*                                       �¼����ƿ�
*						             OS_EVENT
*********************************************************************************************************
*/

typedef struct os_event {
	INT16U	EventId;
    INT8U	OSEventType;                    //�¼����ƿ������
    void    *OSEventPtr;					//ָ��������ʼ�������Ϣ���еĶ��п��ƿ�
    INT16U	OSEventCnt;                     //���ź����ļ���
#if MIN_PRIO <= 63
    INT8U	OSEventGrp;                     //������־��
    INT8U	OSEventTbl[OS_EVENT_TBL_SIZE];  //������־λͼ
#else
    INT16U	OSEventGrp;                     //������־��
    INT16U	OSEventTbl[OS_EVENT_TBL_SIZE];  //������־λͼ
#endif

	OS_TCB	*SLOS_tcb_by_prio[MIN_PRIO + 1];//���ȼ��ּ�����ĸ����ȼ��ı�ͷָ��
	
#if TASK_STATA_EN > 0						//�������������Ϣͳ��
	INT8U	State;
#endif

#if OS_EVENT_NAME_SIZE > 0
    INT8U    OSEventName[OS_EVENT_NAME_SIZE];//�¼�������
#endif
} OS_EVENT;




/*
*********************************************************************************************************
*                                       ����ͷ����
*
*********************************************************************************************************
*/

/******  SLOS_Core.c  *******/
INT8S		taskInit(void);						//��ʼ������ϵͳ�����������񻷾�. �ú������ú�����ϵͳ���������
INT16U 		taskCreate(INT8U *name, void *entryPtr, INT16U stack_size, INT8U priority);	//����һ������
INT8S 		taskStart(INT16U tid);				//����һ������, ��taskCreat����������μӵ���
INT8S		taskDelete(INT16U tid);				//������ɾ��

#if TASK_SUSPEND_EN > 0
INT8S		taskSuspend(INT16U tid);			//��������
INT8S		taskResume(INT16U tid);				//�ָ����������
#endif

#if GET_SET_PRIO_EN	> 0
INT8U		taskPriorityGet(INT16U tid);		//��ȡ��������ȼ�
INT8S		taskPrioritySet(INT16U tid, INT8U newPriority);	//������������ȼ�,���ڶ�̬����
#endif

#if TASK_DELAY_EN > 0
INT8S 		taskSleep(INT16U ticks);			//��һ��������ʱ
#endif

void		SLOS_free_task(void *pdd);			//ϵͳ�Ŀ�������
void		SLOS_start(void);					//ϵͳ���������
OS_TCB		*SLOS_tcb_init(void (*task)(void *pd), INT8U *name, INT8U prio);	//tcb�ṹ��ʼ��
void		SLOS_task_sched(void);				//�����������
void		OSIntEnter(void);							
void 		OSTimeTick(void);					//ʱ���жϴ���
void 		OSIntExit(void);

void 		slos_insert_ready(OS_TCB *ptcb);						//��TCB���뵽��������
void 		slos_out_ready(OS_TCB *ptcb);							//��һ�����������������
void		SLOS_get_heighready(void);								//�õ���ǰ�����������ߵ����ȼ�
void		SLOS_event_waitlist_init(OS_EVENT *pevent);				//��ʼ��ʱ����ƿ�ĵȴ�����λͼ
void 		SLOS_event_task_ready(OS_EVENT *pevent, void *pmsg);	//ʹһ������������״̬
void 		SLOS_event_task_wait(OS_EVENT *pevent);					//ʹһ���������ȴ�ĳ�¼�����״̬
#if TASK_STATA_EN > 0
void		SLOS_state_task(void *pdd);								//ϵͳ��ͳ������
void		SLOS_state_init(void);									//��ʼ��ϵͳ��ͳ������
void		staticCheck(INT16U tid,OS_STKINFO *pstkinfo);			//�������Ķ�ջʹ�����
#endif

/******  SLOS_MUTEX.C  *******/
#if MUTEX_EN > 0
INT16U 		semCreate(void);										//���ź������ƿ�����б��д���һ���ź������ƿ�
INT8S 		semTake(INT16U sid);									//����һ���������ź���
INT8S 		semGive(INT16U sid);									//�ͷ�һ���������ź���
#endif

/******  SLOS_MBOX.C  *******/
#if MBOX_EN > 0
INT16U		mboxCreate(void);								//��Ϣ����Ĵ���
INT8S		mboxPost(INT16U mid,void *msg);					//����һ����Ϣ
void		*mboxPend(INT16U mid,INT16U timeout,INT8U *err);//�ȴ�һ����Ϣ
#endif

/********     SLOS_CPU.H   **********/
OS_STK		*OSTaskStkInit(void (*task)(void *pd), void *pdata, OS_STK *ptos, INT16U opt);
void		OSStartHighRdy(void);
void		OSIntCtxSw(void);
void		OSCtxSw(void);
void		OSTickISR(void);										//ʱ���жϴ���

#if CPU_HOOKS_EN > 0
void		OSInitHookBegin(void);									//�����û�hook��������SL.H�ж����Ƿ�ʹ�ã����Ƿ����û��Լ�����
void		OSInitHookEnd(void);
void		OSTaskCreateHook(OS_TCB *ptcb);
void		OSTaskDelHook(OS_TCB *ptcb);
void		OSTaskIdleHook(void);
void		OSTaskStatHook(void);
void		OSTaskSwHook(void);
void		OSTCBInitHook(OS_TCB *ptcb);
void		OSTimeTickHook(void);
#endif


/*
*********************************************************************************************************
*                                       ȫ�ֱ���
*						      
*********************************************************************************************************
*/
#ifdef quan_ju				//����ȫ�ֺ;ֲ��������õ��ĺ�
#define EXT
#else
#define EXT extern
#endif

#if MIN_PRIO<= 63										//�����������С��64��ʹ��8*8�ľ���λͼ��������64����ʹ��16*16��λͼ��
EXT  INT8U				OSRdyGrp;						//���������
EXT  INT8U				OSRdyTbl[OS_RDY_TBL_SIZE];		//���������
#else
EXT  INT16U				OSRdyGrp;						//���������
EXT  INT16U				OSRdyTbl[OS_RDY_TBL_SIZE];		//���������
#endif

extern INT8U   const	OSUnMapTbl[256];         		//���ڷ���һ���ֽ����������1��λ��λ�õĿ������  �����������SLOS_Core.c�ж����

EXT 	OS_TCB			SLOS_tcb[TASK_COUNT];			//����OS_TCB������ƿ�Ŀռ�
EXT 	OS_TCB			*SLOS_tcb_free_list;			//����ȫ�ֵ�TCB����ı�ͷָ�룬�����洢����tcb

#if OS_EVENT_EN >0
EXT 	OS_EVENT		SLOS_event[OS_EVENT_COUNT];		//����OS_EVENT������ƿ�Ŀռ�
EXT  	OS_EVENT		*SLOS_event_free_list;			//����ȫ�ֵ�event����ı�ͷָ�룬�����洢����event
#endif


EXT OS_TCB				*SLOS_tcb_delay_list;			//����ȫ�ֵ�TCB����ı�ͷָ�룬�����洢��Ҫ��ʱ��tcb

EXT OS_TCB				*SLOS_tcb_suspend_list;			//����ȫ�ֵ�TCB����ı�ͷָ�룬�����洢��Ҫ�����tcb

EXT OS_TCB				*OSTCBHighRdy;					//��һ���������е������������ʱ��

EXT OS_TCB				*OSTCBCur;						//��ǰ�������еĵ�TCB��ָ��

EXT OS_TCB				*SLOS_tcb_by_prio[MIN_PRIO + 1];//�������ȼ����ֵķ����ȼ�����ĸ������ͷָ�룬����ͨ������λͼ�ҵ�������ȼ��ľ�������

EXT INT16U				SLOS_Prio_Cur;					//��ǰ���е���������ȼ�

EXT INT8U				SLOS_Prio_highready;			//��ǰ�����������ߵ����ȼ�

EXT INT32U 				free_task_count;				//��������ļ���ֵ�����������������ǲ��ϵļ������


#if TASK_STATA_EN > 0
EXT INT16U				time_tick_count;				//�жϵļ���ֵ����¼һ���ж��˶��ٴ�

EXT INT32U				task_switch_count;				//���񽻻��ļ���ֵ

EXT INT16U				task_count;						//�������������

EXT INT32U				cpu_count_max;					//cpuÿһ��ʱ������������ļ���

EXT INT8S				cpu_Usage;						//cpu��ʹ����

EXT BOOLEAN				OS_Stat_Rdy;					//�Ƿ�׼���ý���ͳ��
#endif


EXT  BOOLEAN			OSRunning;						//��־ϵͳ�Ѿ���ʼ����

EXT  INT8U				OSIntNesting;					//�жϼ���

#if STATIC_MALLOC_EN > 0
EXT	 OS_STK				SLOS_stk[TASK_COUNT][TASK_STATIC_SIZE];		//��̬��������Ķ�ջ�ռ�
#endif

#if FIT_UCOS_HARDWARE > 0								//����Ϊ����Ӧucos��Ӳ�������Ĵ��룬�ں��в�δʹ�õı���
EXT  INT8U             OSPrioCur;						//��ǰ�������ȼ�
EXT  INT8U             OSPrioHighRdy;					//��������������ȼ�
#endif

#endif
