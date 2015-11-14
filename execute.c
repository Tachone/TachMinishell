#include "execute.h"
#include "def.h"
#include "externs.h"
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include <fcntl.h>
#include <string.h>

void forkexec(int i)
{
	pid_t pid;
	pid = fork();
	if (pid == -1)
		ERR_EXIT("fork");

	if (pid > 0)
	{
		/* 父进程 */
		if (backgnd == 1)
        {
           /*添加入jobs的链表*/
               NODE *p=(NODE*)malloc(sizeof(NODE));
               p->npid=pid;
               printf("%s",cmd[0].args[0]);
               strcpy(p->backcn,cmd[0].args[0]);
               // printf("%s",p->backcn);

               NODE* tmp=head->next;
               head->next=p;
               p->next=tmp;
        }
		lastpid = pid;
        usleep(800);

	}
	else if (pid == 0)
	{
                 
		/* backgnd=1时，将第一条简单命令的infd重定向至/dev/null */
		/* 当第一条命令试图从标准输入获取数据的时候立即返回EOF */

    	if (cmd[i].infd == 0 && backgnd == 1)
			cmd[i].infd = open("/dev/null", O_RDONLY);

		/* 将第一个简单命令进程作为进程组组长 
		if (i == 0)
			setpgid(0, 0);*/
		/* 子进程 */
		if (cmd[i].infd != 0)
		{
			close(0);
			dup(cmd[i].infd);
		}
		if (cmd[i].outfd != 1)
		{
			close(1);
			dup(cmd[i].outfd);
		}
                 
          
		int j;
		for (j=3; j<OPEN_MAX; ++j)
			close(j);

		/* 前台作业能够接收SIGINT、SIGQUIT信号 */
		/* 这两个信号要恢复为默认操作 */
		if (backgnd == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
		}
       // printf("child\n");
		execvp(cmd[i].args[0], cmd[i].args);
		exit(EXIT_FAILURE);
	}
}

int execute_disk_command(void)
{
	if (cmd_count == 0)
		return 0;


	if (infile[0] != '\0')
		cmd[0].infd = open(infile, O_RDONLY);

	if (outfile[0] != '\0')
	{
		if (append)
			cmd[cmd_count-1].outfd = open(outfile, O_WRONLY | O_CREAT
				| O_APPEND, 0666);
		else
			cmd[cmd_count-1].outfd = open(outfile, O_WRONLY | O_CREAT
				| O_TRUNC, 0666);
	}

	/* 因为后台作业不会调用wait等待子进程退出 */
	/* 为避免僵死进程，可以忽略SIGCHLD信号 */
	if (backgnd == 1)
		signal(SIGCHLD, SIG_IGN);
	else
		signal(SIGCHLD, SIG_DFL);

	int i;
	int fd;
	int fds[2];
	for (i=0; i<cmd_count; ++i)
	{
		/* 如果不是最后一条命令，则需要创建管道 */
		if (i<cmd_count-1)
		{
			pipe(fds);
			cmd[i].outfd = fds[1];
			cmd[i+1].infd = fds[0];
		}
                
		forkexec(i);

		if ((fd = cmd[i].infd) != 0)
			close(fd);

		if ((fd = cmd[i].outfd) != 1)
			close(fd);
	}

	if (backgnd == 0)
	{
		/* 前台作业，需要等待管道中最后一个命令退出 */
		while (wait(NULL) != lastpid)
			;
	}

	return 0;
}
