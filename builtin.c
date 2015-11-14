#include "builtin.h"
#include "parse.h"
#include "externs.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>

typedef void (*CMD_HANDLER)(void);

typedef struct builtin_cmd
{
	char *name;
	CMD_HANDLER handler;

} BUILTIN_CMD;

void do_exit(void);
void do_cd(void);
void do_type(void);
void do_about(void);
void do_jobs(void);
void do_kill(void);

BUILTIN_CMD builtins[] = 
{
	{"exit", do_exit},
	{"cd", do_cd},
	{"type", do_type},
    {"about",do_about},
    {"jobs",do_jobs},
    {"kill",do_kill},
	{NULL, NULL}
};

/*
 * 内部命令解析
 * 返回1表示为内部命令，0表示不是内部命令
 */
int builtin(void)
{
	/*
	if (check("exit"))
		do_exit();
	else if (check("cd"))
		do_cd();
	else
		return 0;

	return 1;
	*/

	int i = 0;
	int found = 0;
	while (builtins[i].name != NULL)
	{
		if (check(builtins[i].name))
		{
			builtins[i].handler();
			found = 1;
			break;
		}
		i++;
	}

	return found;
}

void do_exit(void)
{
    int Pgnum=0;
    NODE* tmp=head->next;
    while(tmp!=NULL)
    {
        Pgnum++;
        tmp=tmp->next;
    }
    if(Pgnum!=0)
    {
       printf("There are programs in the background,are you sure to exit?y/N\n");
       char c= getchar();
       if(c=='N')
           return ;
       else
           goto loop;
    }
   loop:
	printf("exit\n");
	exit(EXIT_SUCCESS);
}

void do_cd(void)
{
    get_command(0);
    int fd;
    fd=open(*(cmd[0].args),O_RDONLY);
    fchdir(fd);
    close(fd);
}

void do_about(void)
{
    printf("minishell created by Tach!\n");

}

void do_kill(void)
{
    get_command(0);
    int num=atoi(cmd[0].args[1]);
    signal(SIGQUIT,SIG_DFL);
    kill(num,SIGQUIT);
    signal(SIGQUIT,SIG_IGN);
    NODE *bng=head->next;
    NODE *pre=head;
    while(bng!=NULL)
    {
        if(bng->npid==num)
        {
            NODE* nxt=bng->next;
            pre->next=nxt;
            break;
        }
        pre=bng;
        bng=bng->next;
    }
}
void do_jobs(void)
{
    NODE *prout=head->next;
    while(prout!=NULL)
    {
        printf("%d %s\n",prout->npid,prout->backcn);
        
        prout=prout->next;
    }
}
void do_type(void)
{
	printf("do_type ... \n");
}
