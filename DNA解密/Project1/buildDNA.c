#include"buildDNA.h"
//生成随机DNA序列并返回起始地址
char* buildDNA()
{
	char* DNAArr;
	DNAArr = (char*)malloc(DNASIZE*sizeof(char));
	srand((unsigned)time(NULL));
	int choice;
	for (int i = 0; i < DNASIZE; i++)
	{
		choice = rand() % 4;
		switch (choice)
		{
		case 0:
			DNAArr[i] = 'A';
			break;
		case 1:
			DNAArr[i] = 'C';
			break;
		case 2:
			DNAArr[i] = 'T';
			break;
		case 3:
			DNAArr[i] = 'G';
			break;
		default:
			i--;
			break;
		}
	}
	return DNAArr;
}

//生成1个reads序列
char* buildreads(char* DNAArr, int begin,int* flag)
{
	char* reads = (char*)malloc(sizeof(char) * READSSIZE);
	for (int i = 0; i < READSSIZE; i++)
	{
		reads[i] = DNAArr[((i + begin) % DNASIZE)];
		flag[(i + begin)%DNASIZE]++;//被复制位置+1
	}
	return reads;
}

//判断数组是否全为0
int isZero(int* flag)
{
	int temp = 0;
	for (int i = 0; i < DNASIZE; i++)
	{
		if (flag[i] == 0)
		{
			return 0;
		}
	}
	return 1;
}

//随机生成reads序列直到所有DNA结点被包含
ReadsList getreads(char *DNA)
{
	int flag[DNASIZE] = { 0 };//标志数组
	srand((unsigned)time(NULL));//随机数种子
	int begin;
	ReadsList head;//head为头结点，不存储数据，只存储当前单链表有效结点个数（reads段数）
	head = (ReadsNode*)malloc(sizeof(ReadsNode));
	head->size = 0;
	head->use = 0;
	head->reads = NULL;
	head->next = NULL;
	head->connect = NULL;
	int i = 0;
	while (isZero(flag) == 0||i<=20000)//数组全部不为0，所有位置都被复制了
	{
		begin = rand() % 1851;
		ReadsList p;
		p = (ReadsNode*)malloc(sizeof(ReadsNode));
		p->reads = buildreads(DNA, begin,flag);
		head->size++;
		p->use = 1;//use为1表示当前段有效
		p->next = head->next;
		head->next = p;//头插法
		p->connect = NULL;
		i++;
	}
	return head;
}

//判断两个reads结点是否相等。
int ReadsisEqual(ReadsNode* p1, ReadsNode* p2)
{
	for (int i = 0; i < READSSIZE; i++)
	{
		if (p1->reads[i] != p2->reads[i])
		{
			return 1;//说明不相同
		}
	}
	return 0;//说明完全相同。
}

//去除重复reads，对应节点use设置为0；
void deleteSame(ReadsList head)
{
	ReadsNode* p = head->next;
	ReadsNode* q = p->next;
	while (p != NULL)
	{
		q = p->next;
		if (p->use == 1)
		{
			while (q != NULL)
			{
				if (0 == ReadsisEqual(p, q))//p和q的reads段重复
				{
					q->use = 0;
					//printf("去除重复");
					head->size--;
				}
				q = q->next;
			}
		}
		p = p->next;
	}
}

//判断两个reads链是否能够拼接。reads1的前145个字符和reads2的后145个字符相同。reads2在reads1前
int overlap(ReadsNode*p,ReadsNode *q)
{
	for (int i = 0; i < LAPSIZE; i++)
	{
		if (p->reads[i] != q->reads[READSSIZE - LAPSIZE + i])
		{
			return 0;//不可以拼接
		}
	}
	q->connect = p;
	return 1;//可以拼接
}
//拼接函数
void buildconnect(ReadsList head)
{
	ReadsNode* p = head->next;
	ReadsNode* q = head->next;
	int sum = 0;
	while (p != NULL)
	{
		q = head->next;
		if (p->use == 1)
		{
			while (q != NULL)
			{
				if (1 == q->use)
				{
					if (p != q)
					{
						overlap(p, q);
					}
				}
				q = q->next;
			}
		}
		p = p->next;
	}
}
//通过overlap得到的最长的可能的链
ReadsList maxConnect(ReadsList head)
{
	ReadsList p = head->next;
	ReadsList q = p;
	int maxlen=0;
	ReadsList maxreads=p;
	int len = 0;
	while (p != NULL)
	{
		if (p->use == 1)
		{
			q = p;
			while (q != NULL)
			{
				q = q->connect;
				len++;
			}
			if (len > maxlen)
			{
				maxlen = len;
				maxreads = p;
			}
		}
		len = 0;
		p = p->next;
	}
	printf("\n%d\n", maxlen);
	return maxreads;
}

//根据最长链得到最长的段。
char* lapchar(ReadsList laphead)
{
	char* lap = (char*)malloc(sizeof(char) * DNASIZE);
	ReadsNode* p = laphead;
	int i;
	for ( i = 0; i < READSSIZE; i++)
	{
		lap[i] = p->reads[i];
	}
	while (p != NULL)
	{
		p = p->connect;
		if (p != NULL)
		{
			for (int j = 0; j < 5; j++)
			{
				lap[i++] = p->reads[j + LAPSIZE];
			}
		}
	}
	return lap;
}

int isEqualstring(char*DNA,char*lap)
{
	for (int i = 0; i < DNASIZE; i++)
	{
		if (DNA[i] != lap[i])
		{
			printf("lap不等于DNA");
			return 0;
		}
	}
	printf("lap==DNA");
	return 1;
}