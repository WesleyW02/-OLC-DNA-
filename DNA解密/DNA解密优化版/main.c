#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

//�洢reads�ε�����
typedef struct node
{
	int size;
	char* reads;
	struct node* next;//ָ����һ��reads�Ρ�
}ReadsNode, * ReadsList;

//�洢overlap��ϵ������
typedef struct
{
	ReadsNode* data;
	int addr;//��������һ��reads���������е�λ�á�
	int paddr;//������ǰһ��reads���������λ��
}lapreads;

//�������DNA
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
			return 0;//��ʱ����DNAԭʼ�εĵ�λδ������
		}
	}
	return 1;//���жζ�������
}

//���ɳ�ʼreads��
void buildReads(char* DNA,ReadsList head, int DNALen,int ReadsLen)
{
	int* flag = (int*)malloc(sizeof(int) * DNALen);
	for (int i = 0; i < DNALen; i++)
	{
		flag[i] = 0;//��ʼ��flag���飬��־DNA�Ƿ����е㱻reads�θ��ǡ�
	}
	int begin;//reads�������DNA���еı��
	int num = 0;
	while (0 == isZero(flag, DNALen) || num <= DNALen * 10)
	{
		begin = rand() % DNALen;//����0-DNALen-1�������
		ReadsList p = (ReadsNode*)malloc(sizeof(ReadsNode));
		p->reads = (char*)malloc(sizeof(char) * ReadsLen);
		p->size = 0;
		for (int i = 0; i < ReadsLen; i++)
		{
			p->reads[i] = DNA[(begin + i) % DNALen];
			flag[(begin + i) % DNALen]++;
		}
		head->size++;//��¼reads������
		p->next = head->next;//ͷ�巨���뵥����
		head->next = p;
		num++;
	}
	free(flag);//ʹ����ϣ��ͷŶ�������
}

//�ж�����reads���Ƿ��ظ�
int isEqualReads(char* reads1, char* reads2, int ReadsLen)
{
	for (int i = 0; i < ReadsLen; i++)
	{
		if (reads1[i] != reads2[i])
		{
			return 1;//�����β�ͬ
		}
	}
	return 0;//��������ͬ
}

//ȥ���ظ�reads�β��ͷŶ�Ӧ�����ռ�
void DeleteSame(ReadsList head,int ReadsLen)
{
	ReadsList p = head->next;
	ReadsList q = p->next;
	ReadsList preq = p;//ָ��q��ǰ������ֹɾ��ʱ����
	while (p != NULL)
	{
		q = p->next;
		preq = p;
		while (q != NULL)
		{
			if (0 == isEqualReads(p->reads, q->reads, ReadsLen))//p��q��Ӧreads���ظ���ɾ��q���,qָ��q���
			{
				preq->next = q->next;
				q->next = NULL;
				free(q->reads);
				q->reads = NULL;
				head->size--;
				free(q);
				q = preq->next;
			}
			else//����ͬ��q��preqͬʱ����
			{
				preq = q;
				q = q->next;
			}
		}
		p = p->next;
	}
}
//�ж��������Ƿ����ƴ��
int isoverlap(char* reads1, char* reads2, int lapLen, int ReadsLen)
{
	for (int i = 0; i < lapLen; i++)
	{
		if (reads1[i] != reads2[i + ReadsLen - lapLen])
		{
			return 0;//����ƴ��
		}
	}
	return 1;//����ƴ�ӣ���2��1��ǰ��
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
				if (1 == isoverlap(lapArr[i].data->reads, lapArr[j].data->reads, lapLen, ReadsLen))//����ƴ�ӣ���j��i֮ǰ
				{
					lapArr[j].addr = i;//i��j�ĺ��
					lapArr[i].paddr = j;//j��i��ǰ��
				}
			}
		}
	}
}

//layout������������DNA��
char* layout(lapreads* lapArr, int readsNum,int ReadsLen,int lapLen,int DNALen)
{
	//����ĳ��reads��ֻ�ܳ�����1�����ɵ�DNA��
	int* flag = (int*)malloc(sizeof(int) * readsNum);
	char* lapDNA = (char*)malloc(sizeof(char) * DNALen);
	for (int j = 0; j < readsNum; j++)
	{
		flag[j] = 0;
	}
	int maxlen=0;//��ǰ��󳤶�
	int maxid = 0;//��ǰ��󳤶ȵ�address
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
	printf("��󳤶ȵĶ�%d\n", maxlen);
	//printf("��ʼ�α��%d", maxid);
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
	srand((unsigned)time(NULL));//���������
	int DNALen, ReadsLen, lapLen;
	printf("������������֤��DNA����");
	scanf("%d", &DNALen);
	printf("������������ɵ�reads�γ���");
	scanf("%d", &ReadsLen);
	printf("������reads�εĿ�ƴ�ӳ���");
	scanf("%d", &lapLen);
	//����DNALen�������DNA��
	char* DNA = (char*)malloc(sizeof(char) * DNALen);
	BuildDNA(DNA,DNALen);
	printf("%s", DNA);
	//���ɳ�ʼ��reads��
	ReadsList head;//ͷ���
	head =(ReadsNode *) malloc(sizeof(ReadsNode));
	head->size = 0;
	head->reads = NULL;
	head->next = NULL;
	buildReads(DNA, head, DNALen, ReadsLen);
	printf("\n��ʼreads������Ϊ��%d\n", head->size);//�����ʼ�ε�����
	//ȥ���ظ��β�ɾ����Ӧ��㡣
	DeleteSame(head, ReadsLen);
	printf("ȥ�غ�reads�ε�����Ϊ��%d\n", head->size);//���ȥ�غ�ε�����
	//��ʼoverlap
	lapreads* lapArr = (lapreads*)malloc(sizeof(lapreads) * head->size);
	ReadsNode* p = head->next;
	for (int i = 0; i < head->size; i++)
	{
		lapArr[i].data = p;
		lapArr[i].addr = -1;
		lapArr[i].paddr = -1;
		p = p->next;
	}
	//overlap����
	overlapreads(lapArr, lapLen, ReadsLen, head->size);
	//��ʼ����DNA�Ρ�
	layout(lapArr, head->size,ReadsLen,lapLen,DNALen);
	return 0;
}