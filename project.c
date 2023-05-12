#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char* argv[])
{
    int x;

    if(argc==1)
    {
        printf("Please provide files/directories/links as arguments\n");
        return 1;
    }
    
    int pid1, pid2;

    struct stat st;

    for(int i=1;i<argc;i++)
    {
        stat(argv[i], &st);

        pid1=fork();
        if(pid1==0)
        {
            switch (st.st_mode & __S_IFMT)
            {
                case __S_IFREG:
                {
                    printf("Regular file\n");
                    if(strstr(argv[i],".c"))
                        system("sh script.sh");
                    else
                    {
                        printf("working on file lines\n");
                    }
                    break;
                }
            }
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