#include<iostream>  
#include<random>
#include<cstdlib>
#include<vector>
#include<limits.h>
using namespace std;

#define Max1 25 //访问序列数组大小  
#define Max2 15  //内存块表数组大小  

int saveCount[Max1][Max1];
int x_save, y_save;
int now_couont = 0;


struct pt {
	int pno;    //页号  
	int bno;    //块号  
	int flag;   //状态位，为0时在不内存，为1时在内存  
	int order;  //优先序列  
};

// 输入   
void input(int* a, int n)
{
	srand(time(NULL));
	 for (int i = 0; i < n; i++) {
	    *a= rand()%10;
	    a++;
	}
	/*int arr[20] = { 0,0,1,2,3,0,4,5,0,0,1,2,0,0,1,0,0,0,0,0 };
	for (int i = 0; i < n; i++) {
		*a = arr[i];
		a++;
	}*/


}

// 输出序列  
void output(int* a, int n)
{
	for (int i = 0; i < n; i++) {
		cout << *a << ' ';
		a++;
	}
	cout << '\n';
}

// 算法本体  
void fifo(int* List_pages, int(*bs)[2], int n, int m)//n 序列个数  m 块个数  
{
	pt ptlist[Max1];//定义结构数组  


	int k = 0, flag, cn = 0, i, j;//cn——统计缺页数  
	for (j = 0; j < m; j++)//赋初值 m 块个数  
	{
		bs[j][0] = -100;
		bs[j][1] = -100;
	}
	cout << "操作顺序：\n";
	cout<<"*********************************************************************\n";
	for (i = 0; i < n; i++)// 访问序列循环  n 序列个数  
	{
		cout << "第 " << i + 1 << "次进入 \n";
		flag = 0;
		// 判断当前的值是否已经在 内存块中 ，如果在 flag 直接 = 1  
		for (j = 0; j < m; j++)
			if (List_pages[i] == bs[j][0]) {
				flag = 1;

				break;
			}

		if (flag == 1)//命中  
		{
			ptlist[i].bno = j + 1;
			ptlist[i].flag = 1; // 命中，已存入，未缺页
			ptlist[i].pno = List_pages[i];
			ptlist[i].order = i;

			bs[j][1] = i;
			cout << List_pages[i] << "\t!!命中,已经存入!!\t" << "操作：查询块号为：" << ptlist[i].bno << "--页号为" << ptlist[i].pno << "的进程" << '\n';
			cout << "本次序列： >>>>>  ";
			for (int cixu = 0; cixu < 3; cixu++)
			{
				if (bs[cixu][0] != -100 && bs[cixu][1] != -100)
					cout << bs[cixu][0] << ' ';
			}

			cout << endl;
			cout << "---------------------------------------------------------------------\n";

		}
		else { // 因为 目标值不在块中  
			ptlist[i].flag = 0;//缺页，还未存入  
			ptlist[i].pno = List_pages[i];
			int s = 0;
			int tmpmin = INT_MAX;
			int mm = 0;
			for (; s < m; s++)
			{
				if (tmpmin > bs[s][1])
				{
					tmpmin = bs[s][1];
					mm = s;
				}
			}
			bs[mm][0] = List_pages[i];//将当前页存入内存块 
			bs[mm][1] = i;
			ptlist[i].bno = mm;
			cn++;
			cout << List_pages[i] << "\t!!缺页,还未存入!!\t" << "操作：调出块号为：" << ptlist[i].bno << "--页号为" << ptlist[i].pno << "的进程" << '\n';
			cout << "本次序列： >>>>>  ";
			for (int cixu = 0; cixu < 3; cixu++)
			{
				if (bs[cixu][0] != -100 && bs[cixu][1] != -100)
					cout << bs[cixu][0] << ' ';
			}

			cout << endl;
			cout << "---------------------------------------------------------------------\n";
		}


		for (int i = 0; i < m; i++)
		{
			saveCount[now_couont][i] = bs[i][0];
		}
		now_couont++;
	}
	cout << "计算结果：\n";
	cout << "---------------------------------------------------------------------\n";
	cout << "缺页个数：" << '\t' << cn << '\n';
	cout << "---------------------------------------------------------------------\n";
	cout << "访问次数：" << '\t' << n << '\n';
	cout << "---------------------------------------------------------------------\n";
	cout << "缺页率为：" << '\t' << (float)cn / n << '\n';
	cout << "---------------------------------------------------------------------\n";
	
	cout << "---------------------------------------------------------------------\n";
}

void initGroup(int x, int y)
{
	now_couont = 0;
	x_save = x;
	y_save = y;
	for (int i = 0; i < Max1; i++)
		for (int j = 0; j < Max1; j++)
			saveCount[i][j] = -1;
}

int main()
{
	int List_pages[Max1], bs[Max1][2];
	int n = 20, m = 3, ci = 1;
	string tmp;



	do {
		initGroup(n, m);
		cout << "输入任意字符继续，q退出" << endl;
		cin >> tmp;
		if (tmp == "q")
		{
			break;
		}
		cout << "第" << ci++ << "次测试***********************************************************************************" << endl;
		input(List_pages, n);
		cout << "访问序列：" << endl;
		output(List_pages, n);
		cout << "---------------------------------------------------------------------\n";
		cout << '\n';
		fifo(List_pages, bs, n, m);

	} while (1);
	cout << "-----------------程序结束，感谢您的使用-------------------" << endl;
}