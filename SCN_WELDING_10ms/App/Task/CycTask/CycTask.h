#ifndef _CycTask_H
#define _CycTask_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include  "types.h"

	
typedef enum{
	S_SELF_TEST,//�Լ�
	S_RUNNING,//����
	S_ALRM,//����
	S_TRIG_DELAY,//�ȴ�����ʱ��
	S_SEND_NOISE,//���䳬����
	S_ADJ_BLESS,//�������
	S_ADJ_TIME,//����ʱ��
	S_SET_PARA,//���ò���
}TaskState;

extern uint8_t PWRFlag;//ϵͳ����������־
extern uint8_t AlarmCode;
extern uint8_t Trackstep;
extern void  CycTask_Creat(void);
extern void SwitchIOInit();
extern uint32_t FrequencySweep();
extern uint32_t FrequencyTracking2(void);
extern void SetCycTaskState(uint8_t state);
extern uint8_t search_flag;
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
