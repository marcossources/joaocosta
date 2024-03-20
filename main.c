#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include "sisRecom.h"

#define ALEATORIO ((double)random() / (double)RAND_MAX)


char nome_arquivo[100];
int numero_iteracoes,numeroFatores;
double alpha;
//linha e coluna da nossa matris// numero de elemento difernte da matriz de entrada 
int m, n, nnz;

//aqui estamos a multiplicar a nossa matrix

//inicializando a nossa matrix
void inicializarMatrix(double matrixA[m][n],int m, int n){	
	int i,j;
	for(i=0;i<m;i++){
	    for(j=0;j<n;j++){
			matrixA[i][j] = 0;	
		}
	}
}

void imprimirMatriz(double matriz[][n], int linhas, int colunas) {
    int i, j;
    for (i = 0; i < linhas; i++) {
        for (j = 0; j < colunas; j++) {
            printf("%.2lf\t", matriz[i][j]);
        }
        printf("\n");
    }
}
//Prencher a matrix de forma Aleatoria obs:nao esquecer de voltar na funcao
void preencherMatrizesAleatorias(double L[m][numeroFatores],double R[numeroFatores][numero_iteracoes],int m,int numeroFatores){
	srandom(0);
	int i,j;
	for(i = 0; i < m; i++){
		for(j = 0; j < numeroFatores; j++){
			L[i][j] = ALEATORIO / (double) numeroFatores;
		}
	}
	
	for(i = 0; i < numeroFatores; i++){
		for(j = 0; j < numero_iteracoes; j++){
			R[i][j] = ALEATORIO / (double) numeroFatores;
		}
	}
	
}


/*
double matrixA[m][n];
	double matrixB[m][numero_iteracoes];
	double L[m][numeroFatores];
	double R[numeroFatores][numero_iteracoes];
*/
void calcularDiferencaAoQuadrado( double matrixA[m][n], double matrixB[m][numero_iteracoes], double C[m][n]) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = (matrixA[i][j] - matrixB[i][j]) * (matrixA[i][j] - matrixB[i][j]);
        }
    }
}

void operacao(int n, int m, double L[m][numeroFatores], double alpha,  double C[m][n]) {
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < m; k++) {
            double soma = 0;
            for (int j = 0; j < m; j++) {
                soma += C[i][j] / L[i][k];
            }
            L[i][k] = L[i][k] * L[i][k] - alpha * soma;
        }
    }
}

void operacao1(int n, int m, double R[numeroFatores][numero_iteracoes], double alpha,  double C[m][n]) {
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < m; k++) {
            double soma = 0;
            for (int j = 0; j < m; j++) {
                soma += C[i][j] / R[i][k];
            }
            R[i][k] = R[i][k] * R[i][k] - alpha * soma;
        }
    }
}


void recomendarItens(int m, int n, int numero_iteracoes, double matrixA[m][n], double matrixB[m][n]) {
    for (int i = 0; i < m; i++) {
        double max_value = -1;
        int max_index = -1;
        for (int j = 0; j < n; j++) {
            if (matrixA[i][j] == 0 && matrixB[i][j] > max_value) {
                max_value = matrixB[i][j];
                max_index = j;
            }
        }
        printf("%d\n", max_index);
    }
}


int main(){
    double tempo1, tempo2,t;
	tempo1 = omp_get_wtime();
	 FILE *ficheiro;
	
		
	printf("\nDigite o nome do seu ficheiro :  ");
	scanf("%s",nome_arquivo);
	ficheiro = fopen(nome_arquivo, "r");
	
	if(ficheiro == NULL ){
		printf("\n\t erro ao abriro ficheiro .\n");
		return 1;
	}
	// Ler número de iterações
	fscanf(ficheiro,"%d\n",&numero_iteracoes);
	fscanf(ficheiro,"%lf\n",&alpha);
	fscanf(ficheiro,"%d\n",&numeroFatores);
	fscanf(ficheiro, "%d %d %d", &m, &n, &nnz);
	int i,j;
	
	//Aqui estamos a cria a nossa matrixA
	double matrixA[m][n];
	double matrixB[m][numero_iteracoes];
	double L[m][numeroFatores];
	double R[numeroFatores][numero_iteracoes];
	double C[m][n];
	//inicializando a nossa matrix
	inicializarMatrix(matrixA,m,n);	
	//fim da matrix
	for (int i = 0; i < nnz; i++) {
    	int row, col;
    	double value;
    	fscanf(ficheiro, "%d %d %lf", &row, &col, &value);
    	matrixA[row][col] = value;
  	}
	
	//fim leitura do ficheiro
	
	
	imprimirMatriz(matrixA, m, n);
	
	//Prencher a matrix de forma Aleatoria obs:nao esquecer de voltar na funcao
	preencherMatrizesAleatorias(L,R,m,numeroFatores);
	
	
	//fim funao;
	
	//multiplicar a matrix obs:cria uma funcao para multipicar a matrix
	for(i=0;i<m;i++){
		
		for(j=0;j<n;j++){
			int k;
			matrixB[i][j]=0.0;
			for(k=0;k<numeroFatores;k++){
				matrixB[i][j]=matrixB[i][j]+L[i][j]*R[k][j];
			}
		}
	}
	calcularDiferencaAoQuadrado(matrixA,matrixB,C);
	
	operacao(m, numeroFatores,L, alpha,C);
	operacao1(numeroFatores,numero_iteracoes,R,alpha,C);
	
	
    printf("\n\n---  Saida  ----\n");
    recomendarItens(m,n,numero_iteracoes,matrixA, matrixB);
    
    tempo2 = omp_get_wtime();
    printf("Tempo decorrido Funcao distancia %f s\n" , (tempo2 - tempo1));
	
	return 0;
}

