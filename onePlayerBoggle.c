#include <stdio.h>
#include <stdlib.h>

#include "llist.h"
#include "hashmap.h"
#include "boggle.h"


//One Player Boggle Solver
// DO NOT EDIT

int main(int argc, char * argv[]){
  FILE * dict_f;
  boggle_t * bg;
  int seed=0;
  int points=0;
  llist_t * ll;
  
  if(argc < 2){
    printf("ERROR: require dictionary file\n");
    exit(1);
  }

  if (argc == 3){
    seed = atoi(argv[2]);
  }
  
  //initialize our boggle instance
  bg=bg_init(seed,argv[1]);

  //print the board
  bg_print(bg);

  //find all the words
  ll = bg_all_words(bg);

  //count up the points
  points = bg_count_points(bg,ll);

  //print out the words
  for(ll_node_t * n = ll->head; n; n=n->next){
    printf("%s\n",n->val);
  }
  ll_delete(ll); //free our list
  
  printf("\n");
  printf("Total Points: %d\n",points);

  //delete the boggle instance
  bg_delete(bg);
}
