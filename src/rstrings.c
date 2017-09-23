/* rstrings.c */
/* Raw Strings */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MINSTR 4

typedef enum {
    ERR_USAGE,
    ERR_OPEN ,
    ERR_ALLOC,
    ERR_READ ,
    ERR_WRITE
} errors;

void error(errors code)
{
    switch (code) {
    case ERR_USAGE:
        fprintf(stderr, "Usage: rstrings [infile] [outfile]\n");
        break;

    case ERR_OPEN:
        fprintf(stderr, "File Open Error!\n");
        break;

    case ERR_ALLOC:
        fprintf(stderr, "Insufficient Memory!\n");
        break;

    case ERR_READ:
        fprintf(stderr, "File Read Error!\n");
        break;

    case ERR_WRITE:
        fprintf(stderr, "File Write Error!\n");
        break;

    default:
        fprintf(stderr, "Unknown Error!\n");
        break;
    }

    exit(code);
}

size_t readfile(FILE *fin, char *buf)
{
    size_t i;
    for (i = 0; i < MINSTR - 1; ++i) buf[i] = buf[BUFSIZ + i];
    return fread(buf + MINSTR - 1, sizeof(*buf), BUFSIZ, fin);
}

int my_isprint(int c)
{
    return isprint(c) || c == '\t';
}

int main(int argc, char **argv)
{
    FILE    *fin, *fout;
    char    *buf, c;
    size_t  read, i, j;
    int     wrap;

    switch (argc) {
    case 1:
        fin  = stdin ;
        fout = stdout;
        break;

    case 2:
        if (!(fin = fopen(argv[1], "rb")))
            error(ERR_OPEN);

        fout = stdout;

        break;

    case 3:
        if (!(fin  = fopen(argv[1], "rb")))
            error(ERR_OPEN);

        if (!(fout = fopen(argv[2], "wb")))
            error(ERR_OPEN);

        break;

    default:
        error(ERR_USAGE);
        break;
    }

    if (!(buf = malloc((BUFSIZ + MINSTR - 1) * sizeof(*buf))))
        error(ERR_ALLOC);

    for (i = BUFSIZ; i < BUFSIZ + MINSTR - 1; ++i)
        buf[i] = '\0';

    wrap = 0;

    do {
        read = readfile(fin, buf);
        if (ferror(fin))
            error(ERR_READ);

        for (i = wrap ? MINSTR - 1 : 0; i < read; ++i) {
            if (!wrap) {
                for (j = 0; j < MINSTR && my_isprint(buf[i + j]); ++j);
                if (j != MINSTR) continue;

                fwrite(buf + i, sizeof(*buf), MINSTR, fout);

                i += MINSTR;
            }

            while (i < read + MINSTR - 1 && my_isprint(c = buf[i])) {
                fputc(c, fout);
                ++i;
            }

            wrap = (i == BUFSIZ + MINSTR - 1);

            if (!wrap)
                fputc('\n', fout);

            if (ferror(fout))
                error(ERR_WRITE);
        }
    } while (!feof(fin));

    fclose(fin );
    fclose(fout);
    free(buf);
    return 0;
}
/* end of rstrings.c */
