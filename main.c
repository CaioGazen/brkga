#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/syscall.h>

typedef struct {
  int *cromossomo;
  int *solution;
  int fitness;
} individuo; 

int** lergrafo(char *nomearquivo, int *n, int *m){
  FILE *arquivo;
  int **grafo;
  arquivo = fopen(nomearquivo, "r");
  if(arquivo == NULL){
    printf("Erro ao abrir o arquivo\n");
    return NULL;
  }
  fscanf(arquivo, "%d %d", n, m);
  printf("n: %d m: %d\n", *n, *m);
  grafo = (int**)malloc(*n*sizeof(int*));
  for(int i = 0; i < *n; i++){
    grafo[i] = (int*)malloc(*n*sizeof(int));
    for(int j = 0; j < *n; j++){
      fscanf(arquivo, "%d", &grafo[i][j]);
    }
  }
  fclose(arquivo);
  return grafo;
}

individuo newIndividuo(int n){
  individuo ind;
  ind.cromossomo = (int*)malloc(n*sizeof(int));
  for(int i = 0; i < n; i++){
    ind.cromossomo[i] = rand();
  }
  return ind;
}

int generateSolution(individuo *ind){
  
}

void main(){
  srand(time(NULL));
  int n, m;
  int **grafo;
  grafo = lergrafo("grafo.txt", &n, &m);
  if(grafo == NULL){
    printf("Erro ao ler o grafo\n");
    return;
  }
  
  

  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      printf("%d ", grafo[i][j]);
    }
    printf("\n");
  }

  individuo ind = newIndividuo(n);

  for (int i = 0; i < n; i++){
    printf("%d ", ind.cromossomo[i]);
  }
  

  free(grafo);
  free(ind.cromossomo);
}
