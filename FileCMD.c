#include<stdio.h>
#include<dirent.h>
#include <errno.h>
#include<string.h>
#include<windows.h>
#include <unistd.h>
#include <limits.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#define RD_WR 0666


char path[1000];

void init_path()
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        strcpy(path, cwd);
    }
    else
    {
        perror("getcwd() error");
    }
}

void change_dir(char* arg)
{
    if(chdir(arg)!=0)
    {
        perror("Error");
    }
    else
    {
        init_path();
    }
}

void clear_screen()
{
    system("cls");
}

void print_current_dir()
{
    printf("Current Directory: %s\n", path);
}

void show_files()
{
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    if (d)
    {
        int count = 0;
        while ((dir = readdir(d)) != NULL)
        {
            if(strcmp(dir->d_name,".") == 0 || strcmp(dir->d_name,"..") == 0)
                continue;
            if(count%3==0)
                printf("\n");
            printf("%s\t", dir->d_name);
            count++;
        }
        printf("\n");
        closedir(d);
    }
    else
    {
        printf("%s",strerror(errno));
    }
}

void print_commands()
{
    printf("1. Change Directory\n");
    printf("2. Show Files and Folders\n");
    printf("3. Create New File\n");
    printf("4. Open File\n");
    printf("5. Delete Existing File\n");
    printf("6. Clear Screen\n");
    printf("7. Exit\n");
}

void process_file(char ch[])
{
    printf("Press 1 to Read the selected file\n");
    printf("Press 2 to Write in the selected file\n");
    printf("Press 3 to Copy the selected file\n");
    int option;
    scanf("%d", &option);
    if(option == 1)
    {
        int fd = open(ch, O_RDWR);
        if (fd == -1)
        {
            printf("Error Number % d\n", errno);
            perror("Program");
        }
        printf("Reading %s:\n", ch);
        char buff[50000];
        int n = read(fd,buff,50000);
        write(1,buff,n);
    }
    else if(option == 2)
    {
        printf("Press 1 to append text in the file\n");
        printf("Press 2 to overwrite text in the file\n");
        int num;
        scanf("%d", &num);
        int fd;
        if(num == 1)
        {
            fd = open(ch, O_RDWR | O_APPEND);
            if (fd == -1)
            {
                printf("Error Number % d\n", errno);
                perror("Program");
            }
        }
        else if(num == 2)
        {
            fd = open(ch, O_RDWR);
            if (fd == -1)
            {
                printf("Error Number % d\n", errno);
                perror("Program");
            }
        }
        else {
            printf("Wrong Option Selected\n");
            return;
        }
        char text_write[2000];
        printf("Enter the text to write in the file : ");
        fflush(stdin);
        gets(text_write);
        write(fd, text_write, strlen(text_write));
        printf("Text Write Done Successfully!\n");
    }
    else if (option == 3) {
        printf("Enter destination: ");
        fflush(stdin);
        char arg[1000];
        fgets(arg, 1000, stdin);
        //arg = C:\Users\Omar Haroon\Desktop\FileCMD\hello.txt
        arg[strlen(arg) - 1] = '\\';
        strcat(arg, ch);
        arg[strlen(arg)] = '\0';
//        printf(arg);
        int fd = open(arg, O_WRONLY | O_CREAT, 0644);
        copy_file(ch, arg);
    }

    else
    {
        printf("Wrong Option Selected\n");
    }

}

void delete_file(char ch[])
{
    int fd = unlink(ch);
    printf("%d\n", fd);
    if(fd == -1)
    {
        printf("Error Occured! File doesnt exists!");
    }
    else
    {
        printf("File Deleted Successfully!");

    }
    return;
}

void create_file(char ch[])
{
    int fd = open(ch, O_WRONLY | O_CREAT | O_EXCL, 0644);
    printf("%d\n", fd);

    if(fd == -1)
    {
        printf("Error Occured! Same file already exists!");
    }
    else
    {
        printf("File Created Successfully!");

    }
    return;
}

void copy_file(char src[], char dest[])
{
    FILE *source, *destination;
    char ch;

    source = open(src, O_RDONLY);
    if (source == -1)
    {
        printf("Error Number % d\n", errno);
        perror("Program");
    }

    destination = open(dest, O_WRONLY);
    if (destination == NULL)
    {
        printf("Error Number % d\n", errno);
        perror("Program");
    }

    char buff[50000];

    int n = read(source, buff, 50000);
    write(destination, buff, n);

    printf("File Copied Successfully!\n");

    close(source);
    close(destination);
}

int main(void)
{
    init_path();
    int cmd;
    char arg[1000];

    new_command:
        printf("\n");
        print_current_dir();
        printf("\n");
        print_commands();
        printf("Choose Operation: ");
        scanf("%d", &cmd);
        switch(cmd)
        {
        case 1:
            printf("Enter path: ");
            fflush(stdin);
            fgets(arg, 1000, stdin);
            arg[strlen(arg)-1] = '\0';
            change_dir(arg);
            goto new_command;
        case 2:
            show_files();
            goto new_command;
        case 3:
            printf("Enter Filename: ");
            fflush(stdin);
            fgets(arg, 1000, stdin);
            arg[strlen(arg)-1] = '\0';
            create_file(arg);
            goto new_command;
        case 4:
            printf("Enter Filename: ");
            fflush(stdin);
            fgets(arg, 1000, stdin);
            arg[strlen(arg)-1] = '\0';
            process_file(arg);
            goto new_command;
        case 5:
        printf("Enter Filename: ");
        fflush(stdin);
        fgets(arg, 1000, stdin);
        arg[strlen(arg)-1] = '\0';
        delete_file(arg);
        goto new_command;
        case 6:
            clear_screen();
            goto new_command;
        case 7:
            exit(0);
        default:
            printf("\nInvalid command\n");
            goto new_command;
        }


    return(0);
}
