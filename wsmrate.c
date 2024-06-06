#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#define MAX_WORKSET 10
#define WINDOW_SIZE 20

int mempage=10; //�ڴ�ҳ������������Ҫ���ʵ�ҳ���0���÷�Χ��������ɣ�
int procArray[WINDOW_SIZE]; //���̷���ҳ������
int win[MAX_WORKSET][2];//������
double maxRate=0.8,minRate=0.2;//������������
double curRate;
int cur_workset=3;
int conflictCount=0;//���ϴ���

void print() { //�����������
	curRate=(double)conflictCount/(double)WINDOW_SIZE;
	printf("ȱҳ�����ʣ�%g,����������/���ޣ� %g/%g\n",curRate,maxRate, minRate);
}

void changeArray() { //��̬�ı����ҳ������
	int i;
	for(i=0; i<WINDOW_SIZE; i++) {
		procArray[i]=rand()%mempage;
	}
	printf("���̵���ҳ�����У�");
	for(i=0; i<WINDOW_SIZE; i++) {
		printf("%d|",procArray[i]);
	}
	printf("\n");
}

void init() { //��ʼ����������ҳ������
	int i,j;
	for(i=0; i<MAX_WORKSET; i++) {
		win[i][0]=-1;
		win[i][1]=cur_workset;
	}
}

void changePage(int number) { //��ҳ
	int i,flag=0;
	for(i=1; i<cur_workset; i++) {
		if(win[flag][1] <= win[i][1]) { //Ѱ�����Ƚ����
			flag=i;
		}
	}
	win[flag][0]=procArray[number];//��ҳ
	win[flag][1]=1;
	conflictCount++;//��ͻ������1
	for(i=0; i<cur_workset; i++) {
		if(i!=flag && win[i][1]!=-1) { //����������˳����
			win[i][1]++;
		}
	}
}

void step(int number) { //����һ���ڴ�ҳ��
	int i,hit=0;
	for(i=0; i<cur_workset; i++) {
		if(procArray[number] == win[i][0]) { //����
			hit=1;
			break;
		}
	}
	if(hit==0) { //��δ����
		changePage(number);//��ҳ
	}
}

void run() { //��������
	int i;
	conflictCount=0;
	changeArray();
	for(i=0; i<WINDOW_SIZE; i++) {
		step(i);
	}
	printf("��ͻ������%d,",conflictCount);
}

void feedback() {   //���ݹ����ʵ�����������С
	curRate=(double)conflictCount/(double)WINDOW_SIZE;
	if(curRate>maxRate) {
		cur_workset++;
	} else if(curRate<minRate) {
		cur_workset--;
	}
}

int main() {
	init();//��ʼ��
	char quit;
	do {
		run();
		print();
		feedback();
		printf("���������ַ�������q�˳�\n");
		scanf("%c",&quit);
		getchar();
	} while(quit != 'q');
}
