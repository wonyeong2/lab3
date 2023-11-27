#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define PASSWORDSIZE 12

int main(void)
{
	int fd;
	int nread, cnt = 0, errcnt = 0;
        char ch, text[] = "I want to go to Canada";
        struct termios init_attr, new_attr;
        time_t start_time, end_time; // 시작 시간, 종료 시간
	
	fd = open(ttyname(fileno(stdin)), O_RDWR);
        tcgetattr(fd, &init_attr);

	new_attr = init_attr;
        new_attr.c_lflag &= ~ICANON;
        new_attr.c_lflag &= ~ECHO;
        new_attr.c_cc[VMIN] = 1;
        new_attr.c_cc[VTIME] = 0;

	if (tcsetattr(fd, TCSANOW, &new_attr) != 0) {
            fprintf(stderr, "터미널 속성을 설정할 수 없음.\n");
        }
        printf("다음 문장을 그대로 입력하세요.\n%s\n", text);
        time(&start_time); // 시작 시간

        while ((nread=read(fd, &ch, 1)) > 0 && ch != '\n') {
            if (ch == text[cnt++])
                write(fd, &ch, 1);
            else {
                write(fd, "*", 1);
                errcnt++;
            }
        }
        time(&end_time); // 종료 시간

	printf("\n타이핑 오류의 횟수: %d\n", errcnt);
        printf("분당 평균 타자수: %.f\n", (float)(strlen(text)) / ((float)(end_time - start_time)) * 60.0);

        tcsetattr(fd, TCSANOW, &init_attr);
        close(fd);

        return 0;
}




