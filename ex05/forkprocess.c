#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    pid_t pid; /* 부모에서 프로세스 식별번호 저장 */
    printf("Calling fork \n");
    pid = fork(); /* 새로운 프로세스 생성 */

    if (pid == 0) /* 자식 프로세스일 경우  fork()가 0 반환*/
        printf("I'm the child process\n");
    
    else if (pid > 0)
        printf("I'm the parent process\n");
    
    else /* 비정상적 실행일 경우  음수 값 반환 */
        printf("fork failed\n");
}
