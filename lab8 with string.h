#include <string.h>
#include <stdio.h>

int main() {
    char first_string[100], second_string[100];
    scanf("%s %s", first_string, second_string);
    // 1
    char current_first_string[100];
    strcpy(current_first_string, first_string);
    strcat(current_first_string, second_string);
    printf("concatenation is %s\n", current_first_string);
    // 3
    int compare = strcmp(first_string, second_string);
    printf("compare : ");
    if (compare > 0) printf("s1 > s2\n");
    else if (compare < 0) printf("s1 < s2\n");
    else printf("s1 = s2");
    // 7
    printf("size is %llu\n", strlen(first_string));
    // 10
    printf("Find the first symbol of s2 in s1:\n");
    char *index = strchr(first_string, second_string[0]);
    if (index == NULL) printf("No symbol here\n");
    else {
        printf("String after %c is %s\n", second_string[0], index + 1);
    }
    //12
    int ans = 0;
    int cur = 0;
    for (int i = 0; i < strlen(first_string); i++) {
        if (strchr(second_string, first_string[i]) == NULL) {
            cur++;
        } else {
            if (ans < cur) ans = cur;
            cur = 0;
        }
    }
    if (ans < cur) ans = cur;
    printf("Max length of subsegment which dont include symbols from second string is %d\n", ans);
}
