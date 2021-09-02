#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <malloc.h>

#define CFG_FILE            "./sample.cfg"
#define MAXLINE             1024

/*
 # simple.cfg
 # comment
 k1    v1
 k2    v2    # comment
*/

void split_line(const char *line, ssize_t n)
{
    int ki = 0;
    int vi = 0;
    int i = 0;
    int change = 0;
    char k[MAXLINE + 1];
    char v[MAXLINE + 1];
    char c;

    if(n > MAXLINE) {
        printf("length gt %d, %s", MAXLINE, line);
        exit(1);
    }

    for (i = 0; i < n; ++i) {
        if((c = line[i]) == '#') {
            break;
        }
        else if(c == '\r' || c == '\n') {
            break;
        }
        else if(c == ' ' || c == '\t') {
            if (ki > 0 && change == 0) {
                change = 1;
            }
            else if(vi > 0 && change == 1) {
                break;
            }
            continue;
        }
        else {
            if(change == 0) {
                k[ki++] = c;
            }
            else {
                v[vi++] = c;
            }
        }
    }

    if(ki > 0 && vi > 0) {
        k[ki] = '\0';
        v[vi] = '\0';
        printf("'%s', '%s'\n", k, v);
    }
}

void read_configuration()
{
    FILE *fp        = NULL;
    size_t n        = 0;
    ssize_t nread   = 0;
    char *line      = NULL;

    if((fp = fopen(CFG_FILE, "r")) == NULL) {
        printf("open %s error: %s\n", CFG_FILE, strerror(errno));
        exit(1);
    }

    while((nread = getline(&line, &n, fp)) > 0) {
        // printf("%ld, %ld\n", nread, n);
        // printf(line);
        split_line(line, nread);
    }

    if(line)
        free(line);
}

int main(int argc, char *argv[])
{
    read_configuration();
    return 0;
}
