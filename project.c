#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>

int fileLines(char* file_path)
{
    FILE *file = fopen(file_path,"r");

    if(file == NULL){
        printf("Error opening file or '%s' doesn't exist\n", file_path);
        return 0;
    }

    int lines=0;
    char c;
    while((c=getc(file))!=EOF)
    {
        if(c=='\n')
        {
            lines++;
            c=getc(file);
        }
    }

    fclose(file);
    return lines+1;
}

void createFile(char* dir_path)
{
    /*DIR *dir = opendir(dir_path);
    if (dir == NULL) {
            printf ("Error opening directory or '%s' doesn't exist\n", dir_path);
            exit(1);
        }
        else
    printf("succesfully opened\n");
    dirent = readdir(dir);*/
    dir_path = strcat(dir_path, "_text.txt");
    open(dir_path, O_RDWR | O_CREAT, 0777); //eu am vrut sa-l scriu in directory, dar el n-o vrut :(

    //closedir (dir);
}

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
        printf("Input name: %s\n", argv[i]);
        stat(argv[i], &st);

        pid1=fork();
        if(pid1==0)
        {
            switch (st.st_mode & __S_IFMT)
            {
                case __S_IFREG:
                {
                    printf("Input type: Regular file\n");
                    if(strstr(argv[i],".c"))
                    {
                        system("sh script.sh");
                    }
                    else
                    {
                        printf("Number of lines in '%s' is: %d\n", argv[i], fileLines(argv[i]));
                    }
                    break;
                }

                case __S_IFDIR:
                {
                    printf("Input type: Directory\n");
                    createFile(argv[i]);
                    break;
                }

                default:
                {
                    printf("Wrong file format or '%s' doesn't exist\n", argv[i]);
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