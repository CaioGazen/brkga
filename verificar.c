#include <stdio.h>
#include <stdlib.h>


int** lergrafo(char *nomearquivo, int *n, int *m){
  FILE *arquivo;
  int **grafo;
  arquivo = fopen(nomearquivo, "r");
  if(arquivo == NULL){
    printf("Erro ao abrir o arquivo do grafo\n");
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


int* lerSolucao(char *nomearquivo, int *fitnessSolucao, int n){
  FILE *arquivo;
  int *solucao;
  arquivo = fopen(nomearquivo, "r");
  if(arquivo == NULL){
    printf("Erro ao abrir o arquivo da Solução\n");
    return NULL;
  }

  fscanf(arquivo, "%d", fitnessSolucao);
  printf("fitness da Solução: %d\n", *fitnessSolucao);

  solucao = (int*)malloc((n)*sizeof(int));

  for(int i = 0; i < n; i++){
    printf("i: %d  ", i);
    fscanf(arquivo, "%d", &solucao[i]);
    printf("%d \n", solucao[i]);
  }

  printf("\n");

  fclose(arquivo);
  return solucao;
}


int calcularFitness(int **grafo, int *solucao, int n){
  int fitness = 0;
  for(int i = 0; i < n; i++){
    int custo = grafo[solucao[i] - 1][solucao[i + 1] - 1];
    fitness += custo;

    if (custo == 0){
      printf("Caminho inválido\n");
      return -1;
    }
  }

  return fitness;
}


int verificarDuplicados(int *solucao, int n){
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      if(solucao[i] == solucao[j] && i != j){
        return 1;
      }
    }
  }
  return 0;
}


int main(){
  int n, m, fitnessSolucao, fitnessCalculada;
  int **grafo;
  int *solucao;

  grafo = lergrafo("grafo.txt", &n, &m);
  if(grafo == NULL){
    printf("Erro ao ler o grafo\n");
    return -1;

  }

  solucao = lerSolucao("melhorSolucao.txt", &fitnessSolucao, n + 1);

  if(solucao == NULL){
    printf("Erro ao ler a solução\n");
    return -1;
  }

  if(solucao[0] != 1 || solucao[n] != 1){
    printf("Solução inválida, não começa ou não termina no primeiro nó\n");
    return -1;
  }

  if(verificarDuplicados(solucao, n)){
    printf("Solução inválida, contém vértices duplicados\n");
    return -1;
  }
  

  fitnessCalculada = calcularFitness(grafo, solucao, n);

  if (fitnessCalculada == -1){
    return -1;
  }

  printf("Fitness Calculada: %d\n", fitnessCalculada);
  printf("Fitness Solução: %d\n", fitnessSolucao);

  if(fitnessCalculada != fitnessSolucao){
    printf("Solução inválida, fitness calculada diferente do fitness da solução\n");
    return -1;
  }

  printf("\nSolução válida\n");

  return 0;
}
