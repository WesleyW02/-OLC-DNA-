#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#define DNASIZE  2000
#define READSSIZE 150
#define LAPSIZE 145
//生成随机DNA序列并返回起始地址
char* buildDNA();
//生成1个reads序列
char* buildreads(char* DNAArr, int begin);

//存储reads的链表结点
typedef struct LNode
{
	char* reads;
	struct LNode* next;
	int size;
	int use;//当前节点是否有效
	struct LNode* connect;//overlap后的下一个节点
}*ReadsList,ReadsNode;

//随机生成reads序列直到所有DNA结点被包含
ReadsList getreads(char *DNA);

//判断两个reads结点是否相等。
int ReadsisEqual(ReadsNode* p1, ReadsNode* p2);

//去除重复reads，对应节点use设置为0；
void deleteSame(ReadsList head);

//判断两个reads链是否能够拼接。reads1的前145个字符和reads2的后145个字符相同。reads2在reads1前
int overlap(ReadsNode* p, ReadsNode* q);

//拼接函数
void buildconnect(ReadsList head);

//通过overlap得到的最长的可能的链
ReadsList maxConnect(ReadsList head);

//根据最长链得到最长的段。
char* lapchar(ReadsList laphead);

int isEqualstring(char* DNA, char* lap);