/*
*********************************************************************************************************
*                                          Sword Light
*
*                                       ��³�����ƴ���
*
*                               ����ְҵѧԺ08������ع�Ӳ�����Ʒ
* 
* �ļ�����	��SL_CFG.H
* ��������	��2009/5/14
* ����		��������
* ˵��		������ϵͳ�������ļ�
*********************************************************************************************************
*/

#ifndef SF_CFG
#define SF_CFG



#define	FIT_UCOS_HARDWARE	1				//�����Ƿ��ʺ�ucos��Ӳ������� ��Ҫ����SLOS.H�ж����˳���OSPrioCur��OSPrioHighRdy����Ӧucos��Ӳ����������


#define CPU_HOOKS_EN		1				//�����Ƿ�����hook����

	#define USE_OS_HOOKS	1				//�����Ƿ�ʹ��ϵͳ�Դ���HOOK����


#define GET_SET_PRIO_EN		1				//�����Ƿ������ȡ,�������ȼ�


#define TASK_SCHED_TIME_EN	1				//�����Ƿ�����ͬ��ʱ��Ƭ��ת
	
	#define TASK_TIME_LEN	1				//����ͬ��ʱ��Ƭ��ת��ʱ��Ƭ��

#define TASK_DELAY_EN		1				//�����Ƿ������������ʱ

#define OS_TASK_DEL_EN		1				//�����Ƿ�����ɾ������

#define OS_EVENT_EN			1				//�����Ƿ������¼�

	#define OS_EVENT_COUNT		1			//�����¼����ƿ������

	#define EVENT_DELAY			1			//����mutex�ȴ��ź���ʱ�ĵȴ�ʱ�䣬���Ƿ�ֹ�����ķ�����������Ŀapiû����Ӧ���������ڴ����á�0����Զ�ȴ�

	#define OS_EVENT_NAME_SIZE	1			//�����¼����ƿ�����֣�0��ʾ��ʹ���¼�����

	#define MUTEX_EN			1			//�����Ƿ����������ź��� 		

	#define MBOX_EN				1			//�����Ƿ�������Ϣ����

#define TASK_SUSPEND_EN		1				//�����Ƿ���������Ĺ�����ָ�

#define TASK_STATA_EN		1				//�����Ƿ���������״̬��Ϣͳ��

#define TASK_NAME_LEN		1				//�������ֵĳ���

#define MIN_PRIO			1				//����������ȼ�����������������ȼ�,Ҳ�������ȼ�������

#define TASK_COUNT			1				//����ϵͳ������������������������,ͳ������(�������ʹ��ͳ�ƹ��ܵĻ�)



#define	STATIC_MALLOC_EN	1				//�����Ƿ�����̬�ڴ����

	#define TASK_STATIC_SIZE	1			//���徲̬����ʱÿ������Ķ�ջ�Ĵ�С����λOS_STK

	#define FREE_TASK_STK_SIZE	1			//���嶯̬����ʱ��������Ķ�ջ��С����λbyte

	#define STATE_TASK_STK_SIZE	1			//���嶯̬����ʱͳ������Ķ�ջ��С����λbyte


#define OS_TICKS_PER_SEC	1				//����ϵͳûÿ���ӵ��жϴ���


/************   ����û��Ķ�����е�һЩ����(�û���������)  ******************/
#if CPU_HOOKS_EN == 0						//����Ѿ���ֹʹ��hooks����ôUSE_OS_HOOKSû�����壬����Ϊ0
	#if defined USE_OS_HOOKS
    	#undef USE_OS_HOOKS
    #endif
	#define USE_OS_HOOKS		0
#endif	

#if USE_OS_HOOKS == 0						//�������ucos��Ӳ��������һ��ת��
	#if defined OS_CPU_HOOKS_EN
		#undef  OS_CPU_HOOKS_EN	
	#endif
	#define OS_CPU_HOOKS_EN		0
#endif

#if USE_OS_HOOKS == 1	
    #define OS_VERSION 		252				//��ϵͳ������hooks��ucos2.52�汾����������
    #if defined OS_CPU_HOOKS_EN
		#undef  OS_CPU_HOOKS_EN
	#endif
	#define OS_CPU_HOOKS_EN	1
#endif

#if OS_EVENT_EN == 0						//�����ֹ���¼����ƿ飬��ôҲ�����ֹ���õ��¼����ƿ������
	#undef	OS_EVENT_COUNT
	#undef	OS_EVENT_NAME_SIZE
	#undef	MUTEX_EN
	#undef	MBOX_EN

	#define OS_EVENT_COUNT		0
	#define OS_EVENT_NAME_SIZE	0
	#define MUTEX_EN			0
	#define MBOX_EN				0
#endif

#ifndef OS_CRITICAL_METHOD					//ĳЩӲ�������û�ж���˺궨��
	#define OS_CRITICAL_METHOD	1
#endif




#endif
