#include"buildDNA.h"

int main()
{
	char* DNA;
	DNA = buildDNA();//生成随机DNA序列
	printf("%s\n", DNA);
	ReadsList head;
	head = getreads(DNA);
	printf("\n%d", head->size);
	//对head去重
	deleteSame(head);
	printf("\n%d", head->size);
	//根据reads生成原始DNA序列
	buildconnect(head);//此时可以相连的reads通过connect相连
	//通过overlap得到的最长的可能的链
	ReadsList laphead = maxConnect(head);
	//根据最长链得到最长的段。
	char* lap = lapchar(laphead);
	printf("%s", lap);
	//int ret = isEqualstring(DNA, lap);
	//if (ret == 1)
	//{
	//	return 0;
	//}
}