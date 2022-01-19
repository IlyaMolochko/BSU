#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void freeArgsLength(int **argsLength, int cnt) {
    for (int i = 0; i < cnt; ++i) {
        free(argsLength[i]);
    }
    free(argsLength);
}

void freeArgs(char ***args, int cnt) {
    for (int i = 0; i < cnt; ++i) {
        free(args[i]);
    }
    free(args);
}

int getArgsLength(int argc, char **argv, int **argsLength) {
    int argsQuantity = 1;
    argsLength[0] = malloc(2 * sizeof(int));
    if (argsLength[0] == NULL) {
        fprintf(stderr, "malloc failed\n");
        free(argsLength);
        exit(1);
    }
    argsLength[0][0] = 1;
    argsLength[0][1] = 0;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "|") == 0) {
            argsLength[argsQuantity] = malloc(2 * sizeof(int));
            if (argsLength[argsQuantity] == NULL) {
                fprintf(stderr, "malloc failed\n");
                freeArgsLength(argsLength, argsQuantity);
                exit(1);
            }
            argsLength[argsQuantity][0] = i + 1;
            argsLength[argsQuantity][1] = 0;
            ++argsQuantity;
        } else {
            ++argsLength[argsQuantity - 1][1];
        }
    }
    return argsQuantity;
}

int get_args(int cnt, char **argv, int **args_length, char ***args) {
    for (int i = 0; i < cnt; ++i) {
        args[i] = calloc(args_length[i][1] + 1, sizeof(char *));
        if (args[i] == NULL) {
            fprintf(stderr, "calloc failed\n");
            freeArgsLength(args_length, cnt);
            freeArgs(args, i);
            exit(1);
        }
        for (int j = 0; j < args_length[i][1]; ++j) {
            args[i][j] = argv[args_length[i][0] + j];
        }
    }
    freeArgsLength(args_length, cnt);
    return 0;
}

int run(char **args, int readFd, int writeFd, int fd) {
    int fds[2];
    if (pipe(fds)) {
        perror("pipe");
        return -1;
    }

    if (fcntl(fds[1], F_SETFD, fcntl(fds[1], F_GETFD) | FD_CLOEXEC)) {
        perror("fcntl");
        return -1;
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return -1;
    } else if (pid > 0) {
        close(fds[1]);
        ssize_t count;
        int err;
        while ((count = read(fds[0], &err, sizeof(errno))) == -1) {
            if (errno != EAGAIN && errno != EINTR) {
                break;
            }
        }
        if (count) {
            fprintf(stderr, "Child's execvp: %s\n", strerror(err));
            return -1;
        }
        close(fds[0]);
        return pid;
    } else {
        close(fds[0]);
        if (fd != -1) {
            close(fd);
        }
        if (dup2(writeFd, 1) == -1) {
            perror("dup2 writeFd");
            _exit(1);
        }
        if (dup2(readFd, 0) == -1) {
            perror("dup2 readFd");
            _exit(1);
        }
        if (execvp(args[0], args) == -1) {
            perror("exec");
            write(fds[1], &errno, sizeof(int));
            _exit(0);
        }
        _exit(0);
    }
}

int waitPid(pid_t pid) {
    int status;
    waitpid(pid, &status, 0);
    if (WIFSTOPPED(status)) {
        printf("Child stopped by signal %d (%s)\n",
               WSTOPSIG(status), strsignal(WSTOPSIG(status)));
    }
    if (WIFEXITED(status)) {
        fprintf(stderr, "Child exited, status = %d\n", WEXITSTATUS(status));
    } else {
        fprintf(stderr, "Child terminated\n");
        return -1;
    }
    if (WEXITSTATUS(status)) {
        fprintf(stderr, "Exit status: %d\n", WEXITSTATUS(status));
        return -1;
    }
    if (WIFSIGNALED(status)) {
        fprintf(stderr, "Child killed by signal %d (%s)\n",
                WTERMSIG(status), strsignal(WTERMSIG(status)));
        return -1;
    }
    return 0;
}

int main(int argc, char **argv) {
    int **args_length = malloc(argc * sizeof(int *));
    if (args_length == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    int cnt = getArgsLength(argc, argv, args_length);
    for (int i = 0; i < cnt; ++i) {
        if (args_length[i][1] == 0) {
            fprintf(stderr, "Incorrect args\n");
            freeArgsLength(args_length, cnt);
            exit(2);
        }
    }
    char ***args = malloc((argc - 1) * sizeof(char **));
    if (args == NULL) {
        free(args_length);
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    get_args(cnt, argv, args_length, args);
    int pipes[cnt - 1][2];
    pid_t pid[cnt];
    int readFd, writeFd, fd;
    for (int i = 0; i < cnt; ++i) {
        if (i != cnt - 1) {
            if (pipe(pipes[i])) {
                perror("pipe");
                freeArgs(args, cnt);
                exit(1);
            }
            writeFd = pipes[i][1];
            fd = pipes[i][0];
        } else {
            writeFd = 1;
            fd = -1;
        }
        if (i == 0) {
            readFd = 0;
        } else {
            readFd = pipes[i - 1][0];
        }
        pid[i] = run(args[i], readFd, writeFd, fd);
        if (pid[i] == -1) {
            if (fd != -1) {
                close(fd);
            }
            close(readFd);
            close(writeFd);
            freeArgs(args, cnt);
            exit(1);
        }
        close(readFd);
        close(writeFd);
    }
    freeArgs(args, cnt);
    for (int i = 0; i < cnt; ++i) {
        if (waitPid(pid[i])) {
            return 1;
        }
    }
    return 0;
}
