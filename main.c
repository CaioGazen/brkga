#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct {
  int *cromossomo;
  int *solution;
  int solved;
  int fitness;
} individuo; 


int** lerGrafo(char *nomearquivo, int *n, int *m){
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

int search(int *vet, int n, int key){
  for(int i = 0; i < n; i++){
    if(vet[i] == key){
      return i;
    }
  }
  return -1;
}

void fprintSolution(FILE *output, int *vet, int n){
  int first = search(vet, n - 1, 0);

  for(int i = first; i < n - 1; i++){
    fprintf(output, "%d ", vet[i] + 1);
  }

  for (int i = 0; i <= first; i++){
    fprintf(output, "%d ", vet[i] + 1);
  }

  fprintf(output, "\n");
}

void printSolution(int *vet, int n){
  int first = search(vet, n - 1, 0);

  for(int i = first; i < n - 1; i++){
    printf("%d ", vet[i] + 1);
  }

  for (int i = 0; i <= first; i++){
    printf("%d ", vet[i] + 1);
  }

  printf("\n");
}


int particionaDecoder(int *vet, int *idx, int ini, int fim){
  int pivo = vet[ini];
  int i = ini;
  int aux;

  for(int j = ini+1; j <= fim; j++){
    if(vet[j] <= pivo){

      i++;

      aux = vet[i];
      vet[i] = vet[j];
      vet[j] = aux;

      aux = idx[i];
      idx[i] = idx[j];
      idx[j] = aux;
    }
  }

  aux = vet[i];
  vet[i] = vet[ini];
  vet[ini] = aux;

  aux = idx[i];
  idx[i] = idx[ini];
  idx[ini] = aux;

  return i;
}


void quickSortDecoder(int *vet, int *idx,  int ini, int fim){
  int meio;

  if (ini < fim){
    meio = particionaDecoder(vet, idx, ini, fim);
    quickSortDecoder(vet, idx, ini, meio-1);
    quickSortDecoder(vet, idx, meio+1, fim);
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


void decoder(int **grafo, individuo* ind, int n){
  int idx[n+1], copy[n];
  int fitness = 0;

  // Inicializa o vetor idx com os indices dos vertices
  for(int i = 0; i < n; i++){
    idx[i] = i;
  }

  // Faz uma copia do cromossomo para ordenar
  memcpy(copy, ind->cromossomo, ((n)*sizeof(int)));

  // Ordena o cromossomo e idx em relacao ao cromossomo
  quickSortDecoder(copy, idx, 0, n - 1);

  // Adiciona o vertice inicial no final do caminho
  idx[n] = idx[0];

  // Calcula o fitness da solucao
  for(int i = 0; i < n; i++){
    fitness += grafo[idx[i]][idx[i+1]];
  }

  // Copia a solucao para o individuo
  memcpy(ind->solution, idx, ((n+1)*sizeof(int)));

  // Atualiza o fitness do individuo
  ind->fitness = fitness;
}


void alocarPopulacao(individuo *populacao, int n,int popIni, int popFim){
  for(int i = popIni; i < popFim; i++){
    allocIndividuo(&populacao[i], n);
  }
}

void preencherPopulacao(individuo *populacao, int n, int popIni, int popFim){
  // Preenche os cromossomos dos individuos com numeros aleatorios
  for(int i = popIni; i < popFim; i++){
    for(int j = 0; j < n; j++){
      populacao[i].cromossomo[j] = rand();
    }
  }
}


void printPopulation(individuo *populacao, int n, int popSize){
  for(int i = 0; i < popSize; i++){
    printf("\nIndividuo %d\n", i);
    //printArray(populacao[i].cromossomo, n);
    printSolution(populacao[i].solution, n+1);
    printf("Fitness: %d\n", populacao[i].fitness);
  }
}


void calcularFitness(individuo *populacao, int **grafo, int n, int popSize){
  // Chamada da funcao decoder para todos os individuos da popul
  for(int i = 0; i < popSize; i++){
    decoder(grafo, &populacao[i], n);
  }
}

void copiarElite(individuo *populacao, individuo *nextPopulacao, int eliteSize, int n){
  // Copia as cromossomos dos individuos de elite da populacao atual para a proxima populacao
  for(int i = 0; i < eliteSize; i++){
    for(int j = 0; j < n; j++){
      nextPopulacao[i].cromossomo[j] = populacao[i].cromossomo[j];
    }
  }
}


void crossover(individuo *populacao, int n, individuo *nextPopulacao, int popSize, int eliteSize, int mutanteSize, int bias){
  int parenteElite, parenteAleatorio;

  // exucuta o crossover para os individuos que nao sao de elite nem mutantes
  for(int i = eliteSize; i < popSize - mutanteSize; i++){
    // escolhe um individuo de elite e um aleatorio para serem os pais
    parenteElite = rand() % (eliteSize);
    parenteAleatorio = rand() % (popSize);

    // faz o crossover dos individuos dando prioridade para os cromoossomos do individuo de elite
    for(int j = 0; j < n; j++){
      if((rand() % 100)  < bias){
        nextPopulacao[i].cromossomo[j] = populacao[parenteElite].cromossomo[j];
      }else{
        nextPopulacao[i].cromossomo[j] = populacao[parenteAleatorio].cromossomo[j];
      }
    }
  }
}

int avgFitness(individuo *populacao, int popSize){
  int sum = 0;
  for(int i = 0; i < popSize; i++){
    sum += populacao[i].fitness;
  }

  return sum/popSize;
}

int main(){
  // Inicializa a semente do gerador de numeros aleatorios
  srand(time(NULL));
  //srand(0);
  
  // Le o grafo do arquivo
  int n, m;
  int **grafo;
  grafo = lerGrafo("grafo.txt", &n, &m);
  if(grafo == NULL){
    printf("Erro ao ler o grafo\n");
    return 0;
  }
  
  // Parametros do algoritmo genetico
  int popSize = 100;
  float eliteSize = 0.2;
  float mutanteSize = 0.2;
  float bias = 0.8;
  int geracoes = 100000;
  
  int geracao;
  clock_t inicio;
  inicio = clock();

  //conversão de porcentagem para valor absoluto
  eliteSize = popSize * eliteSize;
  mutanteSize = popSize * mutanteSize;
  bias = bias * 100;

  printf("eliteSize: %f\n", eliteSize);
  printf("mutanteSize: %f\n", mutanteSize);
  printf("bias: %f\n", bias);

  // Abre o arquivo csv para salvar os resultados de cada geracao
  FILE *csv;
  csv = fopen("resultados.csv", "w");

  // Escreve o cabecalho do arquivo csv
  fprintf(csv, "geracao;minFitness;maxFitness;avgFitness;time\n");

  // Aloca memoria para a populacao atual e a proxima populacao
  individuo *aux;
  individuo *populacao = malloc(popSize*sizeof(individuo));
  individuo *nextPopulacao = malloc(popSize*sizeof(individuo));
  
  // Aloca e Preenche os individuos da populacao inicial
  printf("Preenchendo populacao inicial\n");
  alocarPopulacao(populacao, n, 0, popSize);
  preencherPopulacao(populacao, n, 0, popSize);

  // Aloca memoria para os individuos da proxima populacao
  alocarPopulacao(nextPopulacao, n, 0, popSize);

  // Loop principal do algoritmo genetico
  for(geracao = 0; geracao < geracoes; geracao++){
    // Calcula o Fitness da populacao
    calcularFitness(populacao, grafo, n, popSize);
    
    // Ordena a populacao pelo fitness
    quickSortPopulacao(populacao, 0, popSize-1);

    // Salva os resultados da geracao no arquivo csv
    fprintf(csv, "%d;%d;%d;%d;%f\n", geracao, populacao[0].fitness, populacao[popSize-1].fitness, avgFitness(populacao, popSize), (double)(clock()-inicio)/CLOCKS_PER_SEC);

    // Copia a elite para a proxima populacao
    copiarElite(populacao, nextPopulacao, eliteSize, n);

    // Preenche uma parte da populacao com mutantes(individuos aleatorios novos)
    preencherPopulacao(nextPopulacao, n, popSize - mutanteSize, popSize);

    // Faz o crossover dos individuos com a chave aleatoria
    crossover(populacao, n, nextPopulacao, popSize, eliteSize, mutanteSize, bias);

    // troca os ponteiros da população atual e da proxima população
    aux = populacao;
    populacao = nextPopulacao;
    nextPopulacao = aux;
  }

  // Calcula o Fitness da ultima populacao
  printf("Calculando Fitness\n");
  calcularFitness(populacao, grafo, n, popSize);
  
  // Ordena a ultima populacao pelo fitness
  printf("Ordenando Populacao\n");
  quickSortPopulacao(populacao, 0, popSize-1);

 
  // Imprime a populacao
  printPopulation(populacao, n, popSize);
  
  // Imprime o fitness de todos os individuos da ultima populacao
  printf("imprimindo Fitness\n");
  for(int i = 0; i < popSize; i++){
    //printArray(populacao[i].cromossomo, n);
    printf("%d\n", populacao[i].fitness);
  }

  // Imprime o tempo de execucao total
  printf("Tempo de execução: %f\n", (double)(clock()-inicio)/CLOCKS_PER_SEC);

  // Imprime a melhor solucao encontrada
  printf("Melhor solução: \n");
  printf("%d\n", populacao[0].fitness);
  printSolution(populacao[0].solution, n + 1);

  // Salva os resultados da ultima geracao no csv
  fprintf(csv, "%d;%d;%d;%d;%f\n", geracao, populacao[0].fitness, populacao[popSize-1].fitness, avgFitness(populacao, popSize), (double)(clock()-inicio)/CLOCKS_PER_SEC);

  // Fecha o arquivo csv
  fclose(csv);

  // Salva a melhor solucao encontrada no arquivo melhorSolucao.txt
  FILE *output;
  output = fopen("melhorSolucao.txt", "w");

  fprintf(output, "%d\n", populacao[0].fitness);
  fprintSolution(output, populacao[0].solution, n + 1);

  fclose(output);


  // Libera a memoria alocada
  for(int i = 0; i < popSize; i++){
    free(populacao[i].cromossomo);
    free(populacao[i].solution);
    free(nextPopulacao[i].cromossomo);
    free(nextPopulacao[i].solution);
  }

  free(populacao);
  free(nextPopulacao);

  free(grafo);
  

  return 1;
}
