#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "termios.h"
#include "unistd.h"

char * openFilename(int argc, char **argv, char * buffer);
char * loadFile(char * filename);
void showBuffer(char *  buffer);
int getKeyPress();
void applyKey(int key, char * filename, char * buffer);

#define BUFFERSIZE 64
#define CLEAR "\e[2J\e[H"

int main(int argc, char **argv) {
    char * buffer = NULL;
    char * filename;
    printf(CLEAR);
    filename = openFilename(argc, argv, buffer);

    while(1){
        int key = getKeyPress();
        applyKey(key, filename, buffer);
    }
    return 0;
}

char * openFilename(int argc, char **argv, char * buffer) {
    char * filename;
    if (argc > 1) {
        filename = argv[1];
        buffer = loadFile(filename);
        if (strlen(filename) < 1) {
            filename = "New file";
            printf("New buffer: %s\n", filename);
        } else {
            if (buffer == NULL) printf("New buffer: %s\n", filename);
            else printf("Load buffer: %s\n", filename);
        }
    } else {
        filename = "New file";
        printf("New buffer: %s\n", filename);
    }
    return filename;
}

char * loadFile(char * filename) {
    FILE * f;
    size_t size = 0;
    size_t len = 0;
    char * buf = NULL;

    if (filename == NULL || strlen(filename) < 1) return NULL;

    //openFile(char * filename)
    f = fopen(filename, "r");
    if (f == NULL) {
        return NULL;
    }

    do {
        size += BUFFERSIZE;
        buf = realloc(buf, size);
        fgets(buf+len, size, f);
        len = strlen(buf);
    } while(!feof(f));
    return buf;
}
void showBuffer(char * buffer) {
    if (buffer != NULL) {
        printf(CLEAR);
        printf("%s", buffer);
    }
}
void applyKey(int key, char * filename, char * buffer) {
    //printf("%i", key);
    if (key != 0) {
        printf("%d\n", key);
    } else printf(".");
}

int getKeyPress() {
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}