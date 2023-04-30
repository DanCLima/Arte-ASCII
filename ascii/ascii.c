/*-------------------------------------------------------------------------
 * Tranformação de intensidade
 * Por Luiz Eduardo da Silva.
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "imagelib.h"

// void amostragem (image In, image Out, int nr, int nc, int ncr, int nrr) {
//     int pc = nc/ncr;
//     int pr = nr/nrr;

//     puts(" eyta");
//     for (int i = 0, j = 0, cont = 0; i < nr * nc;) {
//         Out[j] = In[i];
//         j++;
//         cont++;
        
//         if (cont < ncr) {   // cont - contabiliza se já passou da quantidade máxima de coluna redimensionada
//             i = i + pc;
//         } else {
//             i = i + pc + (pr*nc);
//             cont = 0;
//         }
//     }  
//     puts(" eyyta");
// }

/* Função que reduz a dimensão da imagem de entrada de acordo com os valores passados como parâmetro */
void amostragem (image In, image Out, int nr, int nc, int ncr, int nrr) {
    int pc = nc/ncr;    // cálculo da proporção de colunas da imagem real para a imagem resultado (colReal/colAmostra)
    int pr = nr/nrr;    // cálculo da proporção de linhas da imagem real para a imagem resultado (linReal/linAmostra)
    int borda_c = (nc - pc*ncr)/2; // número de pixels extras na borda da imagem na dimensão coluna
    int borda_r = (nr - pr*nrr)/2; // número de pixels extras na borda da imagem na dimensão linha

    for (int i = borda_r, j = 0; i < nr - borda_r; i += pr) {
        for (int k = borda_c; k < nc - borda_c; k += pc) {
            Out[j++] = In[i*nc + k];
        }
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
    image In, Out;

    if (argc < 2)
        msg(argv[0]);

    img_name(argv[1], nameIn, nameOut, GRAY);

    //-- read image
    In = img_get(nameIn, &nr, &nc, &ml, GRAY);

    Out = img_alloc(nrr, ncr);      // Alocando a proporção da nova imagem
    
    //-- transformation
    // intensidade(In, Out, nr, nc, ml);

    // -- atribuição das novas colunas e linhas da imagem resultado
    ncr = atoi(argv[2]);        // conversão de char para int necessária
    nrr = atoi(argv[3]);

    // -- amostragem
    amostragem(In, Out, nr, nc, ncr, nrr);  

    //-- save image
    img_put(Out, nameOut, nrr, ncr, ml, GRAY);  // foi preciso mudar os parâmetros nc e nr para nrr e ncr, para que a imagem resultado tivesse a proporção desejada
    sprintf(cmd, "%s %s &", VIEW, nameOut);
    system(cmd);
    img_free(In);
    img_free(Out);
    return 0;
}
