#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/syscall.h>
#include <string.h>

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

void allocIndividuo(individuo* ind, int n){
  ind->cromossomo = malloc(n*sizeof(int));
  for(int i = 0; i < n; i++){
    ind->cromossomo[i] = rand();
  }

  ind->solution = malloc((n+1)*sizeof(int));
}

void printArray(int *vet, int n){
  printf("Array: ");
  for(int i = 0; i < n; i++){
    printf("%d ", vet[i]);
  }
  printf("\n");

}

void printSolution(int *vet, int n){
  printf("Solution :");
  for(int i = 0; i < n; i++){
    printf("%d ", vet[i] + 1);
  }
  printf("\n");

}

void selectionSort(int *vet, int *idx, int n){
  int aux, min;
  for(int i = 0; i < n; i++){
    min = i;
    for(int j = i+1; j < n; j++){
      if(vet[j] < vet[min]){
        min = j;
      }
    }
    aux = vet[i];
    vet[i] = vet[min];
    vet[min] = aux;
    
    aux = idx[i];
    idx[i] = idx[min];
    idx[min] = aux;
  }
}

int decoder(int **grafo, individuo* ind, int n){
  int idx[n+1], copy[n];
  int fitness = 0;
  for(int i = 0; i < n; i++){
    idx[i] = i;
  }
  printf("antes do memcpy\n");
  memcpy(copy, ind->cromossomo, n*sizeof(int));

  printArray(copy, n);
  printArray(idx, n);
  selectionSort(copy, idx, n);
  printArray(copy, n);
  printArray(idx, n);

  idx[n] = idx[0];
  
  printSolution(idx, n+1);

  for(int i = 0; i < n; i++){
    fitness += grafo[idx[i]][idx[i+1]];
  }

  
  memcpy(ind->solution, idx, ((n)*sizeof(int)));

  ind->fitness = fitness;
  printf("Fitness: %d\n", fitness);
}

void preencherPopulacao(individuo *populacao, int n, int popIni, int popFim){
  for(int i = popIni; i < popFim; i++){
    allocIndividuo(&populacao[i], n);
  }
}


void printPopulation(individuo *populacao, int n, int popSize){
  for(int i = 0; i < popSize; i++){
    printf("Individuo %d\n", i);
    printArray(populacao[i].cromossomo, n);
    printSolution(populacao[i].solution, n+1);
    printf("Fitness: %d\n", populacao[i].fitness);
  }
}

void calcularFitness(individuo *populacao, int **grafo, int n, int popSize){
  for(int i = 0; i < popSize; i++){
    decoder(grafo, &populacao[i], n);
  }
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
  
  int popSize = 10;
  
  /*
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      printf("%d ", grafo[i][j]);
    }
    printf("\n");
  }

  individuo ind = newIndividuo(n);


  decoder(grafo, ind, n);

  printSolution(ind.solution, n+1);
  */


  individuo *populacao = malloc(popSize*sizeof(individuo));
  
  printf("Preenchendo populacao\n");
  preencherPopulacao(populacao, n, 0, popSize);
  printf("Populacao preenchida\n");
  

  calcularFitness(populacao, grafo, n, popSize);
  printf("Populacao decodificada\n");



  printf("imprimindo Populacao\n");
  for(int i = 0; i < popSize; i++){
    printf("%d\n", populacao[i].fitness);
  }
  printf("Populacao impressa\n");


  free(grafo);
}
