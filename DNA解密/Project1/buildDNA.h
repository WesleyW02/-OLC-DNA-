#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#define DNASIZE  2000
#define READSSIZE 150
#define LAPSIZE 145
//�������DNA���в�������ʼ��ַ
char* buildDNA();
//����1��reads����
char* buildreads(char* DNAArr, int begin);

//�洢reads��������
typedef struct LNode
{
	char* reads;
	struct LNode* next;
	int size;
	int use;//��ǰ�ڵ��Ƿ���Ч
	struct LNode* connect;//overlap�����һ���ڵ�
}*ReadsList,ReadsNode;

//�������reads����ֱ������DNA��㱻����
ReadsList getreads(char *DNA);

//�ж�����reads����Ƿ���ȡ�
int ReadsisEqual(ReadsNode* p1, ReadsNode* p2);

//ȥ���ظ�reads����Ӧ�ڵ�use����Ϊ0��
void deleteSame(ReadsList head);

//�ж�����reads���Ƿ��ܹ�ƴ�ӡ�reads1��ǰ145���ַ���reads2�ĺ�145���ַ���ͬ��reads2��reads1ǰ
int overlap(ReadsNode* p, ReadsNode* q);

//ƴ�Ӻ���
void buildconnect(ReadsList head);

//ͨ��overlap�õ�����Ŀ��ܵ���
ReadsList maxConnect(ReadsList head);

//��������õ���ĶΡ�
char* lapchar(ReadsList laphead);

int isEqualstring(char* DNA, char* lap);