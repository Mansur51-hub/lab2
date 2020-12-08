#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <locale.h>

typedef struct {
    unsigned char signature[3];
    unsigned char v[2];
    unsigned char flag[1];
    int size;
    unsigned char length[4];
} head;

typedef struct {
    unsigned char tag[3];
    int size;
    unsigned char flag;
    unsigned char *value;
    unsigned char length[3];
} frame;

head head_;
frame frame_;

frame all_frames[200];
int all_frames_size = 0;

int get_the_number(const unsigned char s[], int size) {
    int number = 0;
    int pow = 1;
    for (int i = size - 1; i >= 0; i--) {
        number += s[i] * pow;
        pow *= 100;
    }
    return number;
}

void put_head() {
    printf("signatures: %.3s\nversion: %d\nflag: %d\nsize: %d\n", head_.signature, head_.v[0], head_.flag[0],
           head_.size);
}

void read(FILE *file) {
    fread(head_.signature, sizeof(head_.signature), 1, file);
    fread(head_.v, sizeof(head_.v), 1, file);
    fread(head_.flag, sizeof(head_.flag), 1, file);
    fread(head_.length, sizeof(head_.length), 1, file);
    head_.size = get_the_number(head_.length, 4);
}

int read_tag(FILE *file) {
    unsigned char frame_name[4];
    fread(frame_name, 1, 3, file);
    for (int i = 0; i < 3; i++) frame_.tag[i] = frame_name[i];
    int ans = 0;
    for (int i = 0; i < 3; i++) {
        if ((frame_.tag[i] >= 'A' && frame_.tag[i] <= 'Z') || (frame_.tag[i] >= '0' && frame_.tag[i] <= '9')) ans++;
    }
    if (ans != 3) return 0;
    fread(frame_.length, sizeof(frame_.length), 1, file);
    frame_.size = get_the_number(frame_.length, 3);
    unsigned char frame_flag[1];
    fread(frame_flag, 1, 1, file);
    frame_.flag = frame_flag[0];
    frame_.value = (unsigned char *) malloc(frame_.size - 1);
    fread(frame_.value, 1, frame_.size - 1, file);
    for (int i = 0; i < 3; i++) {
        if ((frame_.tag[i] >= 'A' && frame_.tag[i] <= 'Z') || (frame_.tag[i] >= '0' && frame_.tag[i] <= '9')) ans++;
    }
    return 1;
}

void print_frame(frame frame1) {
    printf("%s %s\n", frame1.tag, frame1.value);
}

void get_all_frames(FILE *file) {
    all_frames_size = 0;
    while (read_tag(file) > 0) {
        all_frames[all_frames_size] = frame_;
        all_frames_size++;
    }
    //fseek(file, -3, SEEK_CUR);
}

void show() {
    for (int i = 0; i < all_frames_size; i++) {
        printf("%s %s\n", all_frames[i].tag, all_frames[i].value);
    }
}

void cerr(FILE *file) {
    if (!file) {
        perror((const char *) file);
        exit(1);
    }
    for (int i = 0; i < 100; i++) {
        char c[1];
        fread(c, 1, 1, file);
        printf("%c %d\n", c[0], c[0]);
    }
}

int size(const unsigned char s[]) {
    int k = 0;
    while (s[k] != 0) k++;
    return k;
}

frame find(unsigned char s[]) {
    int len = size(s);
    if (len != 3) {
        printf("No such tag");
        exit(1);
    }
    for (int i = 0; i < all_frames_size; i++) {
        int k = 0;
        for (int j = 0; j < 3; j++) {
            if (s[j] == all_frames[i].tag[j]) {
                k++;
            }
        }
        if (k == 3) return all_frames[i];
    }
    printf("No such tag");
    exit(1);
}

void get_string(int size, char length[]) {
    for (int i = 0; i < 3; i++) {
        length[2 - i] = size % 100;
        size /= 100;
    }
}

void find_and_replace_tag(unsigned char s[], unsigned char val[]) {
    int len = size(s);
    if (len != 3) {
        printf("No such tag");
        exit(1);
    }
    for (int i = 0; i < all_frames_size; i++) {
        int k = 0;
        for (int j = 0; j < 3; j++) {
            if (s[j] == all_frames[i].tag[j]) {
                k++;
            }
        }
        if (k == 3) {
            all_frames[i].value = val;
            all_frames[i].size = size(val) + 2;
            get_string(all_frames[i].size, all_frames[i].length);
            return;
        }
    }
    printf("No such tag");
    exit(1);
}

void get_tag(unsigned char tag[]) {
    frame cur = find(tag);
    printf("%s\n", cur.value);
}

void set(unsigned char tag[], unsigned char val[]) {
    find_and_replace_tag(tag, val);
}

void copy_head(FILE *file) {
    fwrite(head_.signature, sizeof(head_.signature), 1, file);
    fwrite(head_.v, sizeof(head_.v), 1, file);
    fwrite(head_.flag, sizeof(head_.flag), 1, file);
    fwrite(head_.length, sizeof(head_.length), 1, file);
}

void copy_all_frames(FILE *file) {
    for (int i = 0; i < all_frames_size; i++) {
        fwrite(all_frames[i].tag, sizeof(all_frames[i].tag), 1, file);
        fwrite(all_frames[i].length, sizeof(all_frames[i].length), 1, file);
        fwrite(&all_frames[i].flag, sizeof(all_frames[i].flag), 1, file);
        for (int j = 0; j < all_frames[i].size - 2; j++) {
            fwrite(&*(all_frames[i].value + j), 1, 1, file);
        }
        char c[1];
        c[0] = 0;
        fwrite(c, 1, 1, file);
    }
}

void copy_end(FILE *file, FILE *copy_file) {
    char c[1];
    while (fread(c, 1, 1, file) == 1) {
        fwrite(c, 1, 1, copy_file);
    }
}

void copy_again(FILE *file, FILE *copy_file) {
    fseek(copy_file, 0, SEEK_SET);
    char c[1];
    while (fread(c, 1, 1, copy_file) == 1) {
        fwrite(c, 1, 1, file);
    }
}

int main(int argc, char **argv) {
    setlocale(LC_ALL, "Rus");
    int c;
    while (true) {
        int option_index = 0;
        static struct option long_options[] = {
                {"filepath", 1, 0, 0},
                {"set", 1, 0, 'p'},
                {"show", 0, 0, 's'},
                {"value", 1, 0, 'v'},
                {"get", 1, 0, 'g'},
                {"help", 0, 0, 'h'},
                {0, 0, 0, 'e'}
        };

        c = getopt_long(argc, argv, "smth", long_options, &option_index);
        if (c == -1)
            break;
        FILE *file = NULL;
        char* tag;
        char *value;
        char *path;
        switch (c) {
            case 0:
                path = optarg;
                file = fopen(path, "rb");
                read(file);
                get_all_frames(file);
                if (!file) {
                    perror((const char *) file);
                    return 1;
                }
                break;
            case'h':
                printf("--h                                Выводит справочную информацию о командах\n");
                printf("--show                             Отображение всей метаинформации в виде таблицы\n");
                printf("--set=prop_name --value=prop_value Выставляет значение определенного поля метаинформации с именем prop_name в значение prop_value\n");
                printf("--get=prop_name                    Вывести определенное поле метаинформации с именем prop_name\n");
                break;
            case 's':
                show();
                break;
            case 'g':
                get_tag(optarg);
                break;
            case 'p':
                tag = optarg;
                break;
            case 'v':
                value = optarg;
                file = fopen(path, "rb");
                read(file);
                get_all_frames(file);
                set(tag, value);
                FILE *copyfile_ = fopen("output.mp3", "wb+");
                copy_head(copyfile_);
                copy_all_frames(copyfile_);
                copy_end(file, copyfile_);
                file = fopen(path, "wb");
                copy_again(file, copyfile_);
                break;
            default:
                exit(1);
        }
    }

}