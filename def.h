#ifndef _DEF_H_
#define _DEF_H_
#include <stdio.h>
#include <stdlib.h>

#define ERR_EXIT(m) \
  do \
  { \
    perror(m); \
	exit(EXIT_FAILURE); \
  } \
  while (0)

#define MAXLINE 1024	/* 输入行的最大长度 */
#define MAXARG 20		/* 每个简单命令的参数最多个数 */
#define PIPELINE 5		/* 一个管道行中简单命令的最多个数 */
#define MAXNAME 100		/* IO重定向文件名的最大长度 */
#define OPEN_MAX 1024
typedef struct command
{
	char *args[MAXARG+1];	/* 解析出的命令参数列表 */
	int infd;
	int outfd;
} COMMAND;


typedef struct node
{
   pid_t npid;
   char *backcn[20];
   struct node *next;
}NODE;
#endif /* _DEF_H_ */
