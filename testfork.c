#include <stdio.h>
#include <wait.h>
#include <stdlib.h>
#include <unistd.h>

const int NONSENSE = -5;

int main(int argc, char **argv) {

    puts("main process begining");

    int spawnpid = NONSENSE;
    spawnpid = fork();

    if(spawnpid==-1 || spawnpid == NONSENSE){
        puts("failure to fork");
        return 1;
    }
    if(spawnpid==0){
        //child
        execlp(argv[1],argv[1], NULL);

    } else {
        int status;
        waitpid(spawnpid,&status,0);

        puts("process completed");
        if(WIFEXITED(status)){
            puts(" sucsessfully");
        } else {
            puts (" unsucsessful");
        }
    }

    return 0;
}
