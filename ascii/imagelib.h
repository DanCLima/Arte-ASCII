/*---------------------------------------------------------------+
|            UNIFAL - Universidade Federal de Alfenas            |
|              BACHARELADO EM CIÊNCIA DA COMPUTAÇÃO              |
|       Trabalho....:   Imagem ASCII                             |
|       Disciplina..:   Processamento de Imagens                 |
|       Professor...:   Luiz Eduardo da Silva                    |
|       Aluno.......:   Daniel da Costa Lima                     |
|       Data........:   01/05/2023                               |
+----------------------------------------------------------------*/

#define CREATOR "# CREATOR: Image Processing using C-Ansi - ByDu\n"

typedef int *image;

#define ERROR(x, y) \
    if (x)          \
    y

enum
{
    BW = 1,
    GRAY,
    COLOR
};

#if defined(_WIN32) || defined(__WIN64__) || defined(__CYGWIN__)
#define VIEW "i_view32"
#elif defined(__linux__)
#define VIEW "eog"
#endif

void errormsg(char *, ...);
image img_alloc(int, int);
int img_free(image);
void img_name(char *name, char *in, char *out, int tp);
image img_get(char *name, int *nr, int *nc, int *ml, int tp);
void img_put(image img, char *name, int nr, int nc, int ml, int tp);