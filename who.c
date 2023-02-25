/*
 * who.c - is a clone of the original Unix program.
 *      
 * - read /etc/utmp
 * - suppresses empty records
 * - formats time into human-readable format
 */
#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define SHOWHOST
#define BUFFER_SIZE 256

void show_time(long time);
void show_info(struct utmp *record);
void show_host(char *host_name);

int main() {
    struct utmp current_record;
    int record_size = sizeof(current_record);

    int utmpfd = open(UTMP_FILE, O_RDONLY);
    if (utmpfd == -1) {
        perror(UTMP_FILE);
        exit(1);
    }

    while (read(utmpfd, &current_record, record_size) == record_size) {
        show_info(&current_record);
    }

    close(utmpfd);
    return 0;
}

/*
 * Prints the host name if it is not empty
 */
void show_host(char *host_name) {
    if (host_name[0] == '\0') {
        return;
    }

    printf("(%s)", host_name);
}

/*
 * Prints time in the format: 'yyyy-mm-dd hh:mm'
 */
void show_time(long time) {
    char time_buffer[BUFFER_SIZE] = {0};
    strftime(time_buffer, BUFFER_SIZE, "%F %R", localtime(&time));

    printf("%s", time_buffer);
}

/*
 * Prints one record if that record belongs to a user process
 */
void show_info(struct utmp *record) {
    if (record->ut_type != USER_PROCESS) {
        return;
    }

    printf("%-8.8s", record->ut_name); // the logname
    printf(" ");
    printf("%-8.8s", record->ut_line); // the tty
    printf("     ");
    show_time(record->ut_time);
    printf(" ");
#ifdef SHOWHOST
    show_host(record->ut_host);
#endif
    printf("\n");
}

