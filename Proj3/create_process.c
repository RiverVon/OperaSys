#include <stdio.h>
#include <unistd.h>

int main(){
    pid_t pid = fork();

    if (pid < 0){
        perror("fork failed");
    }else if (pid == 0){
        printf("Child process with PID %d\n", getpid());
    }else {
        printf("Parent process with PID %d\n", getpid());
    }

    return 0;

}