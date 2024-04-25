#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGUMENTS 64

void parseCommand(char *command, char **arguments) {
    int i = 0;
    char *token = strtok(command, " \t\n");

    while (token != NULL && i < MAX_ARGUMENTS - 1) {
        arguments[i++] = token;
        token = strtok(NULL, " \t\n");
    }

    arguments[i] = NULL;
}

void executeCommand(char **arguments) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process
        execvp(arguments[0], arguments);
        perror("execvp failed");
        exit(1);
    } else {
        // Parent process
        wait(NULL);
    }
}

int main() {
    char command[MAX_COMMAND_LENGTH];
    char *arguments[MAX_ARGUMENTS];

    while (1) {
        printf("%% "); // ��ӡ������ʾ��

        if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL) {
            break;
        }

        // �Ƴ�����ǰ��Ŀո�
        char *trimmedCommand = strtok(command, " \t\n");
        if (trimmedCommand == NULL) {
            continue;
        }

        // ����Ƿ��˳�shell
        if (strcmp(trimmedCommand, "exit") == 0 || strcmp(trimmedCommand, "logout") == 0) {
            break;
        }

        // ���������в���
        parseCommand(trimmedCommand, arguments);

        // ִ������
        executeCommand(arguments);
    }

    return 0;
}
