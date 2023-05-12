#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

void menuFile(char *input)
{
    char *options = malloc(10*sizeof(char));
    printf("Regular file options menu:\n");
    printf(" (-n) name \n (-d) size \n (-h) hard link count \n (-m) time of last modification \n (-a) access rights \n (-l) create symbolic link \n");
    printf("Please input your options, preceded by the '-' sign: ");
    scanf("%s",options);

    struct stat st;
    stat(input, &st);

    for(int i=1; i<strlen(options); i++)
    {
        int out=0;
        switch (options[i])
        {
        case 'n':
            printf("Name: %s\n",input);
            break;

        case 'd':
            printf("Size: %ld\n",st.st_size);
            break;

        case 'h':
            printf("Hard link count: %ld\n",st.st_nlink);
            break;

        case 'm':
            printf("Last modified: %ld\n",st.st_mtime);
            break;
        
        case 'a':
            printf("working on acces rights\n");
            break;

        case 'l':
            printf("working on symlink\n");
            break;

        default:
            printf("Input %c does not exist, please try again:\n", options[i]);
            menuFile(input);
            out=1;
            break;
        }

        if(out==1)
        {
            break;
        }
    }
}

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
    open(dir_path, O_RDWR | O_CREAT, 0777);
    //eu am vrut sa-l scriu in directory, dar el n-o vrut :(

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
                    //printf("Input type: Regular file\n");
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
                    //printf("Input type: Directory\n");
                    createFile(argv[i]);
                    break;
                }

                case __S_IFLNK:
                {
                    //printf("Input type: Symbolic link\n");
                    chmod(argv[i],760);
                    break;
                }

                default:
                {
                    printf("Wrong input format or '%s' doesn't exist\n", argv[i]);
                    break;
                }
            }
            exit(0);
        }

        pid2=fork();
        if(pid2==0)
        {
            switch (st.st_mode & __S_IFMT)
            {
                case __S_IFREG:
                    printf("Input type: Regular file\n");
                    menuFile(argv[i]);
                    break;

                case __S_IFDIR:
                    printf("Input type: Directory\n");
                    //menuDir(argv[i]);
                    break;

                case __S_IFLNK:
                    printf("Input type: Symbolic link\n");
                    //menuSymLink(argv[i]);
                    break;
                
                default:
                printf("Wrong input format or '%s' doesn't exist\n", argv[i]);
                    break;
            }
            exit(0);
        }
        wait(&x);
        wait(&x);
    }

    int status1 = waitpid(pid1, &status1, 0);
    int es1 = WEXITSTATUS(status1);
    //printf("The process with PID %d has ended with the exit code %d\n", pid1, es1);
    int status2 = waitpid(pid2, &status2, 0);
    int es2 = WEXITSTATUS(status2);
    //printf("The process with PID %d has ended with the exit code %d\n", pid2, es2);

    return 0;
}