#include"buildDNA.h"

int main()
{
	char* DNA;
	DNA = buildDNA();//�������DNA����
	printf("%s\n", DNA);
	ReadsList head;
	head = getreads(DNA);
	printf("\n%d", head->size);
	//��headȥ��
	deleteSame(head);
	printf("\n%d", head->size);
	//����reads����ԭʼDNA����
	buildconnect(head);//��ʱ����������readsͨ��connect����
	//ͨ��overlap�õ�����Ŀ��ܵ���
	ReadsList laphead = maxConnect(head);
	//��������õ���ĶΡ�
	char* lap = lapchar(laphead);
	printf("%s", lap);
	//int ret = isEqualstring(DNA, lap);
	//if (ret == 1)
	//{
	//	return 0;
	//}
}