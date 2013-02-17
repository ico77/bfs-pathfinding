#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

/*
 * Author: Ivica Gunjaca 
 * use "gcc -Wall `pkg-config --libs --cflags glib-2.0` Pathfinding.c" to compile 
 */

/* Node represents a position in a 2D array */
typedef struct node_t {
  int x;
  int y;
} Node;

/* MAX_X and MAX_Y are boundaries for the 2D array */
#define MAX_X 10
#define MAX_Y 10

/* Globals */
int world[MAX_X][MAX_Y];
int visited[MAX_X][MAX_Y];

/*
 * Free memory allocated to queue elements
 */
void free_queue_elements(GQueue* queue){
  Node* queue_elem;

  while (!g_queue_is_empty(queue)){
    queue_elem = (Node*)g_queue_peek_head(queue);
    g_queue_pop_head(queue);
    free(queue_elem);
  }
}

/*
 * Tests Nodes @first and @second for equality
 */
gboolean is_nodes_equal(const Node* first, const Node* second){
  if (((*first).x == (*second).x) && ((*first).y == (*second).y)){
    return TRUE;
  } else {
    return FALSE;
  }
}

/*
 * Finds the shortest path between @start and @end Node
 * using Breath First Search. Breath First Search can
 * be used because links between nodes are unweighted.  
 */
gboolean bfs(const Node* start, const Node* end){
  GQueue* queue = g_queue_new();
  Node* current_node = malloc(sizeof(Node));
  (*current_node).x = (*start).x;
  (*current_node).y = (*start).y;
  int i, j;
  gboolean end_reached = FALSE;
  
  if (is_nodes_equal(start, end)){
    printf("Start node equals end node");
  }

  visited[(*start).x][(*start).y] = 1;
  g_queue_push_tail(queue, current_node);
    
  while (!g_queue_is_empty(queue)){
    current_node = (Node*)g_queue_peek_head(queue);
    g_queue_pop_head(queue);
    
    /* find neighbouring nodes, assume we can move diagonally */
    for (i = (*current_node).x - 1; i <= (*current_node).x + 1; i++){
      for (j = (*current_node).y - 1; j <= (*current_node).y + 1; j++){
	/* check for world bounds */
	if ((i < 0) || (j < 0) || (i >= MAX_X) || (j >= MAX_Y)){
	  continue;
	}

	/* check for obstacles */
	if (world[i][j] == 1){
	  continue;
	}
	
	/* check if already visited */
	if (visited[i][j] > 0){
	  continue;
	}

	Node* neighbour = malloc(sizeof(Node));
	(*neighbour).x = i;
	(*neighbour).y = j;
	
	visited[i][j] = visited[(*current_node).x][(*current_node).y] + 1;
	if (is_nodes_equal(neighbour, end)){
	  end_reached = TRUE;
	  free_queue_elements(queue);
	  g_queue_clear(queue); 
	  break;
	}
	
	g_queue_push_tail(queue, neighbour);
      }
      if (end_reached){
	break;
      }
    }
    
    free(current_node);
  }
  
  g_queue_free(queue);
  return end_reached;
}

int main(){
  printf("Pathfinding BFS\n");
  
  /* A cell in the world has a 25% probabilty to be an obstacle */
  int obstacle_probability = 25;
  int i,j,n;

  srand(time(NULL));

  /* populate world with random obstacles */
  for (i = 0;i < MAX_X;i++){
    for (j = 0;j < MAX_Y;j++){
      n = rand()%100;
      if (n < obstacle_probability) 
	world[i][j] = 1;
      else
	world[i][j] = 0;
    }
  }

  Node start = {0,0};
  Node end = {9,9};
  
  /* make sure start and end positions aren't obstacles */
  world[start.x][start.y] = 0;
  world[end.x][end.y] = 0;

  /* print world */
  for (i = 0;i < MAX_X;i++){
    for (j = 0 ;j < MAX_Y;j++){
      printf("%d", world[i][j]);
      if (j == (MAX_Y - 1)) printf("\n");
    }
  }

  gboolean result = bfs(&start, &end);

  if (result){
    printf("End node reached!!! \n");
    /* print path */
    printf("To read the path, backtrack from the highest number to the lowest...\n");
    for (i = 0;i < MAX_X;i++){
      for (j = 0 ;j < MAX_Y;j++){
	printf(" %3d ", visited[i][j]);
	if (j == (MAX_Y - 1)) printf("\n");
      }
    }
  } else {
    printf("End node can not be reached!!! \n");
  }

  return 0;  
} 
