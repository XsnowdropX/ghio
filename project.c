#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

void accRights(unsigned short mode)
{
    printf("User:\n\n");
    if(mode & S_IRUSR)
        printf("Read - yes\n");
        else
            printf("Read - no\n");
    if(mode & S_IWUSR)
        printf("Write - yes\n");
        else
            printf("Write - no\n");
    if(mode & S_IXUSR)
        printf("Exec - yes\n\n");
        else
            printf("Exec - no\n\n");
    
    printf("Group:\n\n");
    if(mode & S_IRGRP)
        printf("Read - yes\n");
        else
            printf("Read - no\n");
    if(mode & S_IWGRP)
        printf("Write - yes\n");
        else
            printf("Write - no\n");
    if(mode & S_IXGRP)
        printf("Exec - yes\n\n");
        else
            printf("Exec - no\n\n");
    
    printf("Others:\n\n");
    if(mode & S_IROTH)
        printf("Read - yes\n");
        else
            printf("Read - no\n");
    if(mode & S_IWOTH)
        printf("Write - yes\n");
        else
            printf("Write - no\n");
    if(mode & S_IXOTH)
        printf("Exec - yes\n\n");
        else
            printf("Exec - no\n\n");
}

void createSymLink(char *file_path)
{
    char link[20];
    printf("Please input the name of the symbolic link file: ");
    scanf("%s",link);

    if(!symlink(file_path, link)){
        printf("Symbolic link created successfuly\n");
    }
    else
    {
        printf("Error creating symbolic link or '%s' already exists\n", link);
        exit(1);
    }
}

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
            printf("Size: %ld bytes\n",st.st_size);
            break;

        case 'h':
            printf("Hard link count: %ld\n",st.st_nlink);
            break;

        case 'm':
            printf("Last modified: %ld\n",st.st_mtime);
            break;
        
        case 'a':
            printf("Access rights:\n");
            accRights(st.st_mode);
            break;

        case 'l':
            printf("Create symbolic link:\n");
            createSymLink(input);
            break;

        default:
            printf("Input '%c' does not exist, please try again:\n", options[i]);
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

int countCFiles(char* dir_path)
{
    int files=0;
    char *c;
    struct dirent *entry;
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
            printf ("Error opening directory or '%s' doesn't exist\n", dir_path);
            exit(1);
        }
        else
    //printf("succesfully opened\n");

    while( (entry=readdir(dir)) )
    {
        if(strstr(entry->d_name,".c"))
        {
            printf("%s\n", entry->d_name);
            files++;
        }
    }

    closedir(dir);
    return files;
}

void menuDir(char* input)
{
    char *options = malloc(10*sizeof(char));
    printf("Directory options menu:\n");
    printf(" (-n) name \n (-d) size \n (-a) access rights \n (-c) total number of files with the .c extension\n");
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
            printf("Size: %ld bytes\n",st.st_size);
            break;
        
        case 'a':
            printf("Access rights:\n");
            accRights(st.st_mode);
            break;

        case 'c':
            printf("Nr of files with .c extension: %d\n", countCFiles(input));
            break;

        default:
            printf("Input '%c' does not exist, please try again:\n",options[i]);
            menuDir(input);
            out=1;
            break;
        }

        if(out==1){
            break;
        }
    }
}

void menuSymLink(char* input)
{
    char *options = malloc(10*sizeof(char));
    printf("Symbolic link options menu:\n");
    printf(" (-n) name \n (-l) delete symbolic link \n (-d) size of symbolic link \n (-t) size of target file \n (-a) access rights \n");
    printf("Please input your options, preceded by the '-' sign: ");
    scanf("%s",options);

    struct stat st;
    stat(input, &st);
    int x;
    x = lstat(input, &st);

    for(int i=1; i<strlen(options); i++){
        int out=0;
        switch (options[i])
        {
        case 'n':
            printf("Name: %s\n",input);
            break;

        case 'l':
            printf("working on delete link function\n");
            break;

        case 'd':
            printf("Size: %ld bytes\n",st.st_size);
            break;

        case 't':
            printf("working on target file function\n");
            break;
        
        case 'a':
            accRights(st.st_mode);
            break;

        default:
            printf("Input '%c' does not exist, please try again:\n",options[i]);
            menuDir(input);
            out=1;
            break;
        }

        if(out==1){
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
    if(argc==1)
    {
        printf("Please provide files/directories/links as arguments\n");
        return 1;
    }
    
    int pid1, pid2;

    struct stat st;
    int x;

    for(int i=1;i<argc;i++)
    {
        printf("Input name: %s\n", argv[i]);
        x = lstat(argv[i], &st);

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
                    chmod(argv[i],0760);
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
                    menuDir(argv[i]);
                    break;

                case __S_IFLNK:
                    printf("Input type: Symbolic link\n");
                    menuSymLink(argv[i]);
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