#include <stdio.h>
#include <stdlib.h>

void main(){
  FILE *arquivo;
  int** grafo;
  int n;

  printf("Digite o número de vértices do grafo: ");
  scanf("%d", &n);

  grafo = (int**)malloc(n*sizeof(int*));

  for(int i = 0; i < n; i++){
    grafo[i] = (int*)malloc(n*sizeof(int));
    for(int j = 0; j < n; j++){
      if(i == j){
        grafo[i][j] = 0;
      }
      else{
        grafo[i][j] = (rand() % 9) + 1;
      }
    }
  }


  arquivo = fopen("grafo.txt", "w");
  
  printf("%d %d\n", n, (((n*n)-n)/2));
  fprintf(arquivo, "%d %d\n", n, (((n*n)-n)/2));

  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      fprintf(arquivo, "%d ", grafo[i][j]);
      printf("%d ", grafo[i][j]);
    }
    fprintf(arquivo, "\n");
    printf("\n");
  }
}
