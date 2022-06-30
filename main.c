#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define LINE_LENGTH 38
#define LINES_COUNT 5

char* get_month_name(int number) {
    switch (number)
    {
        case 1: return "Jan";
        case 2: return "Feb";
        case 3: return "Mar";
        case 4: return "Apr";
        case 5: return "May";
        case 6: return "Jun";
        case 7: return "Jul";
        case 8: return "Aug";
        case 9: return "Sep";
        case 10: return "Oct";
        case 11: return "Nov";
        case 12: return "Dec";
        default: return "Undefined";
    }
}

int get_month_day_count(int month, int year){
    bool is_leap = true;
    if(year % 100 == 0 && year % 400 != 0)
        is_leap = false;
    if(year % 4 != 0){
        is_leap = false;
    }

    switch (month)
    {
        case 1: return 31;
        case 2: return is_leap ? 29 : 28;
        case 3: return 31;
        case 4: return 30;
        case 5: return 31;
        case 6: return 30;
        case 7: return 31;
        case 8: return 31;
        case 9: return 30;
        case 10: return 31;
        case 11: return 30;
        case 12: return 31;
        default: return -1;
    }
}

int get_first_wday_of_month(int current_mday, int current_wday){
    int result = current_wday;
    for (int i = current_mday; i > 1; i--)
    {
        if(result > 1) {
            result--;
        } else {
            result = 7;
        }
    }

    return result;
}

void sprint_week(char* week_line, int start_mday, int start_wday, int current_mday, int day_count){
    sprintf(week_line, " ");
    for (int i = 0; i < 7; i++){

        int current_day = start_mday + i - start_wday + 1;
        bool is_this_week = start_mday < current_mday && start_mday + 7 > current_mday;
        int offset =
                current_day > current_mday && is_this_week
                    ? 14
                    : 0;
        char *week_line_ptr = week_line + 3 * i + 1 + offset;

        if(start_mday + i < start_wday || current_day > day_count) {
            sprintf(week_line_ptr, "   ");
            continue;
        }
        if (current_day == current_mday)
        {
            sprintf(week_line_ptr, 
            current_day < 10 
            ? "\033[30m\033[47m %i\033[0m "
            : "\033[30m\033[47m%i\033[0m "
            , current_day);
        }
        else
        {
            sprintf(week_line_ptr,
                    current_day < 10
                        ? " %i "
                        : "%i ",
                    current_day);
        }
    }
}

int main(void) {

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    int current_mday = tm.tm_mday;
    int current_wday = tm.tm_wday;
    int day_count = get_month_day_count(tm.tm_mon + 1, tm.tm_year + 1900);

    char **calendar_lines = malloc(LINES_COUNT * sizeof(char *));
    for (int i = 0; i < LINES_COUNT; i++){
        *(calendar_lines + i) = malloc(LINE_LENGTH * sizeof(char));
        for (int j = 0; j < LINE_LENGTH; j++){
            *(*(calendar_lines + i) + j) = ' ';
        }
    }

    for (int i = 0; i < LINES_COUNT; i++) {
        int start_wday = 1;
        int first_wday = get_first_wday_of_month(current_mday, current_wday);
        if (i == 0)
        {
            start_wday = first_wday;
        }
        sprint_week(*(calendar_lines + i), 1 + 7 * i, first_wday, current_mday, day_count);
    }

    printf("       %s %d       \n", get_month_name(tm.tm_mon + 1), tm.tm_year + 1900);
    printf(" Mo Tu We Th Fr Sa Su \n");

    for (int i = 0; i < LINES_COUNT; i++) {
        printf("%s\n", *(calendar_lines + i));
    }

    for (int i = 0; i < LINES_COUNT; i++){
        free(*(calendar_lines + i));
    }
    free(calendar_lines);

    return 0;
}
