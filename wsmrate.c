#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#define MAX_WORKSET 10
#define WINDOW_SIZE 20

int mempage=10; //内存页面总数（进程要访问的页面从0到该范围内随机生成）
int procArray[WINDOW_SIZE]; //进程访问页面序列
int win[MAX_WORKSET][2];//工作集
double maxRate=0.8,minRate=0.2;//故障率上下限
double curRate;
int cur_workset=3;
int conflictCount=0;//故障次数

void print() { //程序输出函数
	curRate=(double)conflictCount/(double)WINDOW_SIZE;
	printf("缺页故障率：%g,故障率上限/下限： %g/%g\n",curRate,maxRate, minRate);
}

void changeArray() { //动态改变访问页面序列
	int i;
	for(i=0; i<WINDOW_SIZE; i++) {
		procArray[i]=rand()%mempage;
	}
	printf("进程调用页面序列：");
	for(i=0; i<WINDOW_SIZE; i++) {
		printf("%d|",procArray[i]);
	}
	printf("\n");
}

void init() { //初始化工作集和页面序列
	int i,j;
	for(i=0; i<MAX_WORKSET; i++) {
		win[i][0]=-1;
		win[i][1]=cur_workset;
	}
}

void changePage(int number) { //换页
	int i,flag=0;
	for(i=1; i<cur_workset; i++) {
		if(win[flag][1] <= win[i][1]) { //寻找最先进入的
			flag=i;
		}
	}
	win[flag][0]=procArray[number];//换页
	win[flag][1]=1;
	conflictCount++;//冲突次数加1
	for(i=0; i<cur_workset; i++) {
		if(i!=flag && win[i][1]!=-1) { //工作集进入顺序处理
			win[i][1]++;
		}
	}
}

void step(int number) { //调用一次内存页面
	int i,hit=0;
	for(i=0; i<cur_workset; i++) {
		if(procArray[number] == win[i][0]) { //命中
			hit=1;
			break;
		}
	}
	if(hit==0) { //尚未命中
		changePage(number);//换页
	}
}

void run() { //程序运行
	int i;
	conflictCount=0;
	changeArray();
	for(i=0; i<WINDOW_SIZE; i++) {
		step(i);
	}
	printf("冲突次数：%d,",conflictCount);
}

void feedback() {   //根据故障率调整工作集大小
	curRate=(double)conflictCount/(double)WINDOW_SIZE;
	if(curRate>maxRate) {
		cur_workset++;
	} else if(curRate<minRate) {
		cur_workset--;
	}
}

int main() {
	init();//初始化
	char quit;
	do {
		run();
		print();
		feedback();
		printf("输入任意字符继续，q退出\n");
		scanf("%c",&quit);
		getchar();
	} while(quit != 'q');
}
