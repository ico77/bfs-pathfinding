#include <stdio.h>
#include <stdlib.h>

int main(){
  printf("Pathfinding techniques\n");
  int world[10][10];
  // a cell in the world has a 25% probabilty to be an obstacle
  int obstacle_probability=25,i,j,n;

  srand(time(NULL));

  // populate world with random obstacles
  for (i=0;i<10;i++){
    for (j=0;j<10;j++){
      n = rand()%100;
      if (n < obstacle_probability) 
	world[i][j]=1;
      else
	world[i][j]=0;
    }
  }

  // print world
  for (i=0;i<10;i++){
    for (j=0;j<10;j++){
      printf("%d", world[i][j]);
      if (j==9) printf("\n");
    }
  }
    
} 
