#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int x;

    if(argc==1)
    {
        printf("please provide files/directories/links as arguments\n");
        return 1;
    }
    
    int pid1, pid2;

    for(int i=1;i<argc;i++)
    {
        pid1=fork();
        if(pid1==0)
        {
            printf("this is the first child process\n");
            exit(0);
        }

        pid2=fork();
        if(pid2==0)
        {
            printf("this is the second child process\n");
            exit(0);
        }
        wait(&x);
        wait(&x);
    }
    
    return 0;
}