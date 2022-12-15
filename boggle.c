#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "llist.h"
#include "hashmap.h"
#include "boggle.h"

//constant values on all the dice in global
const char * boggle_dice[] = {
        "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
        "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCENST",
        "CEIILT", "CEILPT", "CEIPST", "DDHNOT", "DHHLOR",
        "DHLNOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
        "FIPRSY", "GORRVW", "IPRRRY", "NOOTUW", "OOOTTU",
};


//initialize a boggle instance (do not edit until after the TODO: comment)
boggle_t * bg_init(int seed, char * dict_fn){
  //You get this first part of the function for free :)

  //set a random seed
  srandom(seed);

  //create the instance
  boggle_t * bg = calloc(sizeof(boggle_t), 1);

  //setup the board by rolling dice
  int used[BOGGLE_NUM_DICE] = {0};
  for(int i=0;i<BOGGLE_NUM_DICE;i++){
    while(1){
      //choose a dice to roll
      int d = random()%BOGGLE_NUM_DICE;
      if(used[d]) continue; //keep choosing if already rolles

      //choose a random side
      int s = random()%BOGGLE_DICE_SIDES;
      bg->board[i/BOGGLE_DIMENSION][i%BOGGLE_DIMENSION] = boggle_dice[d][s];
      used[d] = 1; //mark that dice as rolled
      break;
    }
  }

  // Open dictionary
  FILE * dict_f;
  char word[1024];
  hashmap_t * hm;
  if((dict_f = fopen(dict_fn,"r")) == NULL){
    perror("opening dictionary file");
    exit(1);
  }

  // Load the valid words (8≥length≥3) into the dictionary (hashmap)
  hm = hm_init();
  while(fscanf(dict_f, "%1023s", word) != EOF){
    for(char *c=word;(*c=toupper(*c)) != 0;c++);
    if (strlen(word) >= 3 && strlen(word) <= 8) {
      hm_add(hm,word);
    }
  }
  fclose(dict_f);

  bg->dict = hm;

  return bg;
}

//delete/free the boggle instance (do not edit)
void bg_delete(boggle_t * bg){
  //You get this function for free :)
  hm_delete(bg->dict); 
  free(bg);
}


//print the board using fancy unicode (do not edit)
void bg_print(boggle_t * bg){
  printf(".-----------.\n");
  for (int r = 0; r < 5; r++) {
    printf("| ");
    for (int c = 0; c < 5; c++) {
      printf("%c",bg->board[r][c]);      
      if (bg->board[r][c]=='Q')
	printf("u");
      else
	printf(" ");
    }
    printf("|\n");
  }
  printf("'-----------'\n");
}


//count the points up (do not edit)
// 3 and 4 length words are worth 1 point
// 5 length worth 2
// 6 length worth 3
// 7 length worth 5
// 8 length worth 11
// note we stop searching at 8 length words 
int bg_count_points(boggle_t * bg, llist_t * words){
  int pts=0;
  for(ll_node_t * n = words->head; n; n = n->next){
    if(hm_check(bg->dict,n->val)){
      switch(strlen(n->val)){
        case 0:
        case 1:
        case 2: break;
        case 3:
        case 4:
          pts+=1;
          break;
        case 5:
          pts+=2;
          break;
        case 6:
          pts+=3;
          break;
        case 7:
          pts+=5;
          break;
        default:
          pts+=11;
      }
    }
  }
  return pts; 
}


//helper functions for the recursion, declaring here so can call below
void _bg_all_words(boggle_t * bg, //boggle instance
		   hashmap_t * hm, //hashmap to track found words
		   int r, int c, //current row (r) / column (c)
		   char * last_word, //what the last word was
		   int last_visited[BOGGLE_DIMENSION][BOGGLE_DIMENSION] //what has been previously visited
		                                                        //don't want to double back
		   );

//entry function for finding all the words
llist_t * bg_all_words(boggle_t *bg){

  //0'ed out -- currently visited nowhere (all 0's), and the current word is the empty string
  int visited[BOGGLE_DIMENSION][BOGGLE_DIMENSION] = {0};
  char cur_word[9] = {0}; //empty string 
  // Note using byte array of length 9 b/c max word length is 8!

  //initialize a hashmap to track status
  hashmap_t * hm = hm_init();

  //start a recurssion at every space on the board
  for(int i=0;i<BOGGLE_DIMENSION;i++){
    for(int j=0;j<BOGGLE_DIMENSION;j++){
      _bg_all_words(bg, hm, i, j, cur_word, visited);
    }
  }

  // Store all the words in a linked list
  llist_t * words = ll_init();
  for(int i = 0; i < hm->num_buckets; i++){
    if(hm->buckets[i] != NULL){
      ll_node_t * node = hm->buckets[i]->head;
      while(node != NULL){
        ll_push(words, node->val);
        node = node->next;
      }
    }
  }
  
  hm_delete(hm); //delete the hashmap

  //return the list of words
  return words;
}



void _bg_all_words(boggle_t *bg, hashmap_t * hm, int r, int c,
		   char * last_word, int last_visited[BOGGLE_DIMENSION][BOGGLE_DIMENSION]){

  char cur_word[9] = {0}; //use to store the current word
  int visited[BOGGLE_DIMENSION][BOGGLE_DIMENSION] = {0};

  // Return out of recurssion if word is too long, off the board, or already visited
  if (strlen(last_word) > 8 || r < 0 || r >= BOGGLE_DIMENSION || c < 0 || c >= BOGGLE_DIMENSION || last_visited[r][c] == 1) {
    return;
  }

  // Set visited to last_visited
  for (int i = 0; i < BOGGLE_DIMENSION; i++) {
    for (int j = 0; j < BOGGLE_DIMENSION; j++) {
      visited[i][j] = last_visited[i][j];
    }
  }

  // Create new word (add current letter to last word)
  strcpy(cur_word, last_word);
  cur_word[strlen(cur_word)] = bg->board[r][c];
  // Check if current letter is Q, if so add U (if space)
  if (bg->board[r][c] == 'Q' && strlen(cur_word) < 8) {
    cur_word[strlen(cur_word)] = 'U';
  }
  cur_word[strlen(cur_word)] = '\0'; // Null terminate

  // Check if current word is a word and not already found, if so save it in hashmap
  if (hm_check(bg->dict, cur_word) == 1 && hm_check(hm, cur_word) == 0) {
    hm_add(hm, cur_word);
  }

  // Mark this tile as visited
  visited[r][c] = 1;
  
  //Perform 8 recursive calls on all surrounding dice...
  // you get this part for free  :)
  _bg_all_words(bg,hm,r-1,c,cur_word,visited);
  _bg_all_words(bg,hm,r-1,c-1,cur_word,visited);
  _bg_all_words(bg,hm,r-1,c+1,cur_word,visited);
  _bg_all_words(bg,hm,r+1,c,cur_word,visited);
  _bg_all_words(bg,hm,r+1,c-1,cur_word,visited);
  _bg_all_words(bg,hm,r+1,c+1,cur_word,visited);
  _bg_all_words(bg,hm,r,c-1,cur_word,visited);
  _bg_all_words(bg,hm,r,c+1,cur_word,visited);

}

