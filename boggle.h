#ifndef __BOGGLE_H__
#define __BOGGLE_H__

#include "llist.h"
#include "hashmap.h"

//header information about the board

#define BOGGLE_NUM_DICE 25
#define BOGGLE_DICE_SIDES 6
#define BOGGLE_DIMENSION 5


typedef struct {
  char board[BOGGLE_DIMENSION][BOGGLE_DIMENSION]; //the boggle board
  hashmap_t * dict; //dictionary mapping
} boggle_t;


//create a new boggle instance with a random seed and a path to dictionary file name
boggle_t * bg_init(int seed, char * dict_fn);

//delete boggle instance
void bg_delete(boggle_t * bg);

//print the boggle board
void bg_print(boggle_t * bg);


//return the number of points for the list of words
int bg_count_points(boggle_t * bg, llist_t * words);

//find and print all the words, returns a llist with newly allocated strs
llist_t * bg_all_words(boggle_t *bg);



#endif
