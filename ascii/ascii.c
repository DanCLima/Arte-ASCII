/*-------------------------------------------------------------------------
 * Tranformação de intensidade
 * Por Luiz Eduardo da Silva.
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "imagelib.h"

void img_info2 (int nr, int nc, int ml) {
    char name[] = "result.txt";

    printf("Transformations: ...........: sampling and quantization\n");
    printf("Final file name ............: %s \n", name);
    printf("Number of rows..............: %d \n", nr);
    printf("Number of columns...........: %d \n", nc);
    printf("Max intensity level.........: %d \n\n", ml);
}

/* Função que reduz a dimensão da imagem de entrada de acordo com os valores passados como parâmetro */
void amostragem (image In, image Out, int nr, int nc, int ncr, int nrr) {
    int pc = nc/ncr;    // cálculo da proporção de colunas da imagem real para a imagem resultado (colReal/colAmostra)
    int pr = nr/nrr;    // cálculo da proporção de linhas da imagem real para a imagem resultado (linReal/linAmostra)
  
    for (int i = 0, j = 0; i < nr; i += pr) {
        for (int k = 0; k < nc; k += pc) {
            Out[j++] = In[i*nc + k];
        }
    }  
}

/* Função que copia os dados de uma imagem sem o cabeçalho para um arquivo.txt */
void copiaArq (char *name, int nc) {
    char lines[100];
    FILE *file1, *file2;

    /* Abertura da imagem */
    file1 = fopen(name, "r");
    if (file1 == NULL) { 
        printf("Erro na abertura de arquivo: aqui <%s>\n", name);
        exit(0); 
    }

    /* Abertura do arquivo.txt */
    file2 = fopen("result.txt", "w");
    
    /* Número mágico P1, P2 ou P3 */
    fgets(lines, 80, file1);

    /* Linhas de comentários*/
    fgets(lines, 80, file1);
    while (strchr(lines, '#')) {
        fgets(lines, 80, file1);
    }

    /* Máximo de cinza */
    fgets(lines, 80, file1);

    /* Copiando o conteúdo da imagem pro arquivo.txt sem o cabeçalho e transformando cada valor em caracter */
    int i = 0, n;
    char caracter;
    while (fscanf(file1, "%d", &n) == 1) {
        if (i == nc){     //quebra de linha de acordo com a quantidade de colunas
            fprintf(file2, "%c ", '\n');
            i = 0;
        }
        
        caracter = (char)n;
        fprintf(file2, "%c ", caracter);
        i++;
    }

    fclose(file1);
    fclose(file2);
}

//comando ascii cao.pgm 100 30 "@$#*%o!=+;:~-,. "
// logica = pixel/proporcao de cinza
void quantizacao (image In, int nc, int nr, char *caracteres, int mn) {
    int tamString = 0;
    int pMn;

    tamString = strlen(caracteres);

    pMn = mn/tamString;     //Obtendo a proporção de necessária para substuir os tons de cinza da imagem

    for (int i = 0; i < nr * nc; i++) {
        In[i] = caracteres[In[i]/pMn];
    }
}

void intensidade(image In, image Out, int nl, int nc, int mn)
{
    float T[mn + 1];
    float expo = 2;
    for (int i = 0; i < mn + 1; i++)
    {
        //--- Transformacao Potência/Raiz
        T[i] = pow(i, expo) / pow(mn, expo) * mn;
        //--- Transformacao Logaritmica
        // T[i] = log(i + 1) / log(mn + 1) * mn;
        // printf("T[%d] = %.0f\n", i, T[i]);
    }
    for (int i = 0; i < nl * nc; i++)
        Out[i] = T[In[i]];
}

void msg(char *s)
{
    printf("\nArte ASCII");
    printf("\n-------------------------------");
    printf("\nUso:  %s  nome-imagem[.pgm] \n\n", s);
    printf("    nome-imagem[.pgm] é o nome do arquivo da imagem \n");
    exit(1);
}

/*-------------------------------------------------------------------------
 * main function
 *-------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
/*-------------------------------------------------------------------------
*   nc - número de colunas
*   nr - número de linhas
*   ml ou mn - valor máximo de intensidade
*   ncr - número de colunas redimensionado
*   nrr - número de linhas redimensionadas
*-------------------------------------------------------------------------*/
    int nc, nr, ml, ncr, nrr;
    char *p, nameIn[100], nameOut[100], cmd[110];
    image In, Out, Out2;

    if (argc < 2)
        msg(argv[0]);

    img_name(argv[1], nameIn, nameOut, GRAY);

    //-- read image
    In = img_get(nameIn, &nr, &nc, &ml, GRAY);

    Out = img_alloc(nrr, ncr);      // Alocando a proporção da nova imagem

    // -- atribuição das novas colunas e linhas da imagem resultado
    ncr = atoi(argv[2]);        // conversão de char para int necessária
    nrr = atoi(argv[3]);

    // -- amostragem
    amostragem(In, Out, nr, nc, ncr, nrr);

    //-- quantização
    quantizacao(Out, ncr, nrr, argv[4], ml);

    img_info2(nrr, ncr, ml/strlen(argv[4]));

    //-- save image
    img_put(Out, nameOut, nrr, ncr, ml, GRAY);  // foi preciso mudar os parâmetros nc e nr para nrr e ncr, para que a imagem resultado tivesse a proporção desejada
    // sprintf(cmd, "%s %s &", VIEW, nameOut);  //comando para exibir a imagem no irfanview
    // system(cmd);

    // -- escrita no arquivo.txt
    copiaArq(nameOut, ncr);

    img_free(In);
    img_free(Out);

    return 0;
}
