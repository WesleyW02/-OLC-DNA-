#include"buildDNA.h"
//�������DNA���в�������ʼ��ַ
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

//����1��reads����
char* buildreads(char* DNAArr, int begin,int* flag)
{
	char* reads = (char*)malloc(sizeof(char) * READSSIZE);
	for (int i = 0; i < READSSIZE; i++)
	{
		reads[i] = DNAArr[((i + begin) % DNASIZE)];
		flag[(i + begin)%DNASIZE]++;//������λ��+1
	}
	return reads;
}

//�ж������Ƿ�ȫΪ0
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

//�������reads����ֱ������DNA��㱻����
ReadsList getreads(char *DNA)
{
	int flag[DNASIZE] = { 0 };//��־����
	srand((unsigned)time(NULL));//���������
	int begin;
	ReadsList head;//headΪͷ��㣬���洢���ݣ�ֻ�洢��ǰ��������Ч��������reads������
	head = (ReadsNode*)malloc(sizeof(ReadsNode));
	head->size = 0;
	head->use = 0;
	head->reads = NULL;
	head->next = NULL;
	head->connect = NULL;
	int i = 0;
	while (isZero(flag) == 0||i<=20000)//����ȫ����Ϊ0������λ�ö���������
	{
		begin = rand() % 1851;
		ReadsList p;
		p = (ReadsNode*)malloc(sizeof(ReadsNode));
		p->reads = buildreads(DNA, begin,flag);
		head->size++;
		p->use = 1;//useΪ1��ʾ��ǰ����Ч
		p->next = head->next;
		head->next = p;//ͷ�巨
		p->connect = NULL;
		i++;
	}
	return head;
}

//�ж�����reads����Ƿ���ȡ�
int ReadsisEqual(ReadsNode* p1, ReadsNode* p2)
{
	for (int i = 0; i < READSSIZE; i++)
	{
		if (p1->reads[i] != p2->reads[i])
		{
			return 1;//˵������ͬ
		}
	}
	return 0;//˵����ȫ��ͬ��
}

//ȥ���ظ�reads����Ӧ�ڵ�use����Ϊ0��
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
				if (0 == ReadsisEqual(p, q))//p��q��reads���ظ�
				{
					q->use = 0;
					//printf("ȥ���ظ�");
					head->size--;
				}
				q = q->next;
			}
		}
		p = p->next;
	}
}

//�ж�����reads���Ƿ��ܹ�ƴ�ӡ�reads1��ǰ145���ַ���reads2�ĺ�145���ַ���ͬ��reads2��reads1ǰ
int overlap(ReadsNode*p,ReadsNode *q)
{
	for (int i = 0; i < LAPSIZE; i++)
	{
		if (p->reads[i] != q->reads[READSSIZE - LAPSIZE + i])
		{
			return 0;//������ƴ��
		}
	}
	q->connect = p;
	return 1;//����ƴ��
}
//ƴ�Ӻ���
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
//ͨ��overlap�õ�����Ŀ��ܵ���
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

//��������õ���ĶΡ�
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
			printf("lap������DNA");
			return 0;
		}
	}
	printf("lap==DNA");
	return 1;
}