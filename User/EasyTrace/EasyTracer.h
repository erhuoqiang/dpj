#ifndef EASY_TRACER_H
#define EASY_TRACER_H

#define IMG_X 0	           //ͼƬx����
#define IMG_Y 0	           //ͼƬy����
#define IMG_W 320          //ͼƬ���
#define IMG_H 240          //ͼƬ�߶�

#define ALLOW_FAIL_PER 3   //�ݴ��ʣ�û1<<ALLOW_FAIL_PER�����������һ������㣬�ݴ���Խ��Խ����ʶ�𣬵�������Խ��
#define ITERATE_NUM    8   //������������������Խ��ʶ��Խ��ȷ����������Խ��

//ʹ�þ���
//RESULT Resu;
//TARGET_CONDI Condition={60,100,20,120,10,160,40,40,320,240};
//Trace(&Condition,&Resu);

typedef struct             //�ж�Ϊ��Ŀ������
{
	unsigned char  H_MIN;    //Ŀ����Сɫ��
	unsigned char  H_MAX;    //Ŀ�����ɫ��	
	
	unsigned char  S_MIN;    //Ŀ����С���Ͷ�  
	unsigned char  S_MAX;    //Ŀ����󱥺Ͷ�

	unsigned char  L_MIN;    //Ŀ����С����  
	unsigned char  L_MAX;    //Ŀ���������

	unsigned int   WIDTH_MIN; //Ŀ����С���
	unsigned int   HIGHT_MIN; //Ŀ����С�߶�

	unsigned int   WIDTH_MAX; //Ŀ�������
	unsigned int   HIGHT_MAX; //Ŀ�����߶�
}TARGET_CONDI;

typedef struct             //ʶ����
{
	unsigned int x;         //Ŀ���x����
	unsigned int y;         //Ŀ���y����
	unsigned int w;         //Ŀ��Ŀ��
	unsigned int h;         //Ŀ��ĸ߶�
}RESULT;


//-----------------------------------------------------------------------------------------
//Ψһ��API���û���ʶ������д��Conditionָ��Ľṹ���У��ú���������Ŀ���x��y����ͳ���
//����1ʶ��ɹ�������1ʶ��ʧ��
//-----------------------------------------------------------------------------------------
int Trace(const TARGET_CONDI *Condition,RESULT *Resu);
int Trace_1(const TARGET_CONDI *Condition,RESULT *Resu);
#endif
