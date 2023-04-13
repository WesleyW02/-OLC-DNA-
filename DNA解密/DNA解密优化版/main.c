#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

//存储reads段的链表
typedef struct node
{
	int size;
	char* reads;
	struct node* next;//指向下一个reads段。
}ReadsNode, * ReadsList;

//存储overlap关系的数组
typedef struct
{
	ReadsNode* data;
	int addr;//相连的下一个reads段在数组中的位置。
	int paddr;//相连的前一个reads段在数组的位置
}lapreads;

//随机生成DNA
void BuildDNA(char* DNA,int DNALen)
{
	char base[4] = { 'A','T','C','G' };
	int choice = 0;
	for (int i = 0; i < DNALen; i++)
	{
		choice = rand() % 4;
		DNA[i] = base[choice];
	}
}

int isZero(int* flag,int DNALen)
{
	for (int i = 0; i < DNALen; i++)
	{
		if (0 == flag[i])
		{
			return 0;//此时还有DNA原始段的点位未被覆盖
		}
	}
	return 1;//所有段都被覆盖
}

//生成初始reads段
void buildReads(char* DNA,ReadsList head, int DNALen,int ReadsLen)
{
	int* flag = (int*)malloc(sizeof(int) * DNALen);
	for (int i = 0; i < DNALen; i++)
	{
		flag[i] = 0;//初始化flag数组，标志DNA是否所有点被reads段覆盖。
	}
	int begin;//reads的起点在DNA段中的编号
	int num = 0;
	while (0 == isZero(flag, DNALen) || num <= DNALen * 10)
	{
		begin = rand() % DNALen;//生成0-DNALen-1的随机数
		ReadsList p = (ReadsNode*)malloc(sizeof(ReadsNode));
		p->reads = (char*)malloc(sizeof(char) * ReadsLen);
		p->size = 0;
		for (int i = 0; i < ReadsLen; i++)
		{
			p->reads[i] = DNA[(begin + i) % DNALen];
			flag[(begin + i) % DNALen]++;
		}
		head->size++;//记录reads段数量
		p->next = head->next;//头插法插入单链表
		head->next = p;
		num++;
	}
	free(flag);//使用完毕，释放堆区数据
}

//判断两个reads段是否重复
int isEqualReads(char* reads1, char* reads2, int ReadsLen)
{
	for (int i = 0; i < ReadsLen; i++)
	{
		if (reads1[i] != reads2[i])
		{
			return 1;//两个段不同
		}
	}
	return 0;//两个段相同
}

//去除重复reads段并释放对应堆区空间
void DeleteSame(ReadsList head,int ReadsLen)
{
	ReadsList p = head->next;
	ReadsList q = p->next;
	ReadsList preq = p;//指向q的前驱，防止删除时断链
	while (p != NULL)
	{
		q = p->next;
		preq = p;
		while (q != NULL)
		{
			if (0 == isEqualReads(p->reads, q->reads, ReadsLen))//p和q对应reads段重复，删除q结点,q指向q后继
			{
				preq->next = q->next;
				q->next = NULL;
				free(q->reads);
				q->reads = NULL;
				head->size--;
				free(q);
				q = preq->next;
			}
			else//不相同，q和preq同时后移
			{
				preq = q;
				q = q->next;
			}
		}
		p = p->next;
	}
}
//判断两个段是否可以拼接
int isoverlap(char* reads1, char* reads2, int lapLen, int ReadsLen)
{
	for (int i = 0; i < lapLen; i++)
	{
		if (reads1[i] != reads2[i + ReadsLen - lapLen])
		{
			return 0;//不可拼接
		}
	}
	return 1;//可以拼接，且2在1的前面
}

//overlap
void overlapreads(lapreads* lapArr, int lapLen,int ReadsLen,int readsNum)
{
	for (int i = 0; i < readsNum; i++)
	{
		for (int j = 0; j < readsNum; j++)
		{
			if (i != j)
			{
				if (1 == isoverlap(lapArr[i].data->reads, lapArr[j].data->reads, lapLen, ReadsLen))//可以拼接，且j在i之前
				{
					lapArr[j].addr = i;//i是j的后继
					lapArr[i].paddr = j;//j是i的前驱
				}
			}
		}
	}
}

//layout函数——生成DNA段
char* layout(lapreads* lapArr, int readsNum,int ReadsLen,int lapLen,int DNALen)
{
	//假设某个reads段只能出现在1个生成的DNA中
	int* flag = (int*)malloc(sizeof(int) * readsNum);
	char* lapDNA = (char*)malloc(sizeof(char) * DNALen);
	for (int j = 0; j < readsNum; j++)
	{
		flag[j] = 0;
	}
	int maxlen=0;//当前最大长度
	int maxid = 0;//当前最大长度的address
	int len = 0;
	for (int i = 0; i < readsNum;i++)
	{
		len = 0;
		int temp = i;
		while (1)
		{
			if (temp!=-1&&flag[temp] == 0)
			{
				flag[temp] = 1;
				temp = lapArr[temp].addr;
				len++;
			}
			else
			{
				break;
			}
		}
		if (maxlen < len)
		{
			maxlen = len;
			maxid = temp;
		}
		for (int j = 0; j < readsNum; j++)
		{
			flag[j] = 0;
		}
	}
	printf("最大长度的段%d\n", maxlen);
	//printf("起始段编号%d", maxid);
	int num = 0;
	for (int i = 0; i < ReadsLen; i++)
	{
		lapDNA[num] = lapArr[maxid].data->reads[i];
		num++;
	}	
	int temp = maxid;
	while (num < DNALen)
	{
		if (temp != -1 && flag[temp] == 0)
		{
			flag[temp] = 1;
			temp = lapArr[temp].addr;
			for (int i = 0; i < ReadsLen - lapLen; i++)
			{
				lapDNA[num] = lapArr[temp].data->reads[lapLen + i];
				num++;
			}
		}
	}
	printf("%s", lapDNA);
}

int main()
{
	srand((unsigned)time(NULL));//随机数种子
	int DNALen, ReadsLen, lapLen;
	printf("请输入您想验证的DNA长度");
	scanf("%d", &DNALen);
	printf("请输入随机生成的reads段长度");
	scanf("%d", &ReadsLen);
	printf("请输入reads段的可拼接长度");
	scanf("%d", &lapLen);
	//根据DNALen随机生成DNA段
	char* DNA = (char*)malloc(sizeof(char) * DNALen);
	BuildDNA(DNA,DNALen);
	printf("%s", DNA);
	//生成初始的reads段
	ReadsList head;//头结点
	head =(ReadsNode *) malloc(sizeof(ReadsNode));
	head->size = 0;
	head->reads = NULL;
	head->next = NULL;
	buildReads(DNA, head, DNALen, ReadsLen);
	printf("\n初始reads段数量为：%d\n", head->size);//输出初始段的数量
	//去除重复段并删除对应结点。
	DeleteSame(head, ReadsLen);
	printf("去重后reads段的数量为：%d\n", head->size);//输出去重后段的数量
	//开始overlap
	lapreads* lapArr = (lapreads*)malloc(sizeof(lapreads) * head->size);
	ReadsNode* p = head->next;
	for (int i = 0; i < head->size; i++)
	{
		lapArr[i].data = p;
		lapArr[i].addr = -1;
		lapArr[i].paddr = -1;
		p = p->next;
	}
	//overlap函数
	overlapreads(lapArr, lapLen, ReadsLen, head->size);
	//开始生成DNA段。
	layout(lapArr, head->size,ReadsLen,lapLen,DNALen);
	return 0;
}