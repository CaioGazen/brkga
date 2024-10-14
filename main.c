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


int particiona(individuo *populacao, int ini, int fim){
  int pivo = populacao[ini].fitness;
  int i = ini;
  individuo aux;

  for(int j = ini+1; j <= fim; j++){
    if(populacao[j].fitness <= pivo){

      i++;

      aux = populacao[i];
      populacao[i] = populacao[j];
      populacao[j] = aux;
    }
  }

  aux = populacao[i];
  populacao[i] = populacao[ini];
  populacao[ini] = aux;

  return i;
}


void quickSortPopulacao(individuo *populacao,  int ini, int fim){
  int meio;

  if (ini < fim){
    meio = particiona(populacao, ini, fim);
    quickSortPopulacao(populacao, ini, meio-1);
    quickSortPopulacao(populacao, meio+1, fim);
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

  selectionSort(copy, idx, n);

  idx[n] = idx[0];

  for(int i = 0; i < n; i++){
    fitness += grafo[idx[i]][idx[i+1]];
  }

  memcpy(ind->solution, idx, ((n)*sizeof(int)));

  ind->fitness = fitness;
}


void preencherPopulacao(individuo *populacao, int n, int popIni, int popFim){
  for(int i = popIni; i < popFim; i++){
    allocIndividuo(&populacao[i], n);
    for(int j = 0; j < n; j++){
      populacao[i].cromossomo[j] = rand();
    }
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

void copiarElite(individuo *populacao, individuo *nextPopulacao, int eliteSize){
  for(int i = 0; i < eliteSize; i++){
    nextPopulacao[i] = populacao[i];
  }
}


void crossover(individuo *populacao, int n, individuo *nextPopulacao, int popSize, int eliteSize, int mutanteSize, int bias){
  int parenteElite, parenteAleatorio;

  for(int i = eliteSize; i < popSize - mutanteSize; i++){
    parenteElite = rand() % (eliteSize);
    parenteAleatorio = rand() % (popSize);

    allocIndividuo(&nextPopulacao[i], n);

    for(int j = 0; j < n; j++){
      if((rand() % 100)  < bias){
        nextPopulacao[i].cromossomo[j] = populacao[parenteElite].cromossomo[j];
      }else{
        nextPopulacao[i].cromossomo[j] = populacao[parenteAleatorio].cromossomo[j];
      }
    }
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
  int eliteSize = 2;
  int mutanteSize = 2;
  int bias = 80;
  int geracoes = 1000;


  individuo *populacao = malloc(popSize*sizeof(individuo));
  individuo *nextPopulacao = malloc(popSize*sizeof(individuo));
  
  printf("Preenchendo populacao\n");
  preencherPopulacao(populacao, n, 0, popSize);
  

  for(int i = 0; i < geracoes; i++){
    printf("Calculando Fitness\n");
    calcularFitness(populacao, grafo, n, popSize);
    
    printf("Ordenando Populacao\n");
    quickSortPopulacao(populacao, 0, popSize-1);

    printf("copiando elite\n");
    copiarElite(populacao, nextPopulacao, eliteSize);

    printf("gerando mutantes\n");
    preencherPopulacao(nextPopulacao, n, popSize - mutanteSize, popSize);

    printf("crossover\n");
    crossover(populacao, n, nextPopulacao, popSize, eliteSize, mutanteSize, bias);

    populacao = nextPopulacao;
  }


  printf("Calculando Fitness\n");
  calcularFitness(populacao, grafo, n, popSize);
    
  printf("Ordenando Populacao\n");
  quickSortPopulacao(populacao, 0, popSize-1);


  printf("imprimindo Populacao\n");
  for(int i = 0; i < popSize; i++){
    printArray(populacao[i].cromossomo, n);
    printf("%d\n", populacao[i].fitness);
  }
  printf("Populacao impressa\n");

  

  free(grafo);
}
