
#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#include <stdlib.h>
#include <stdbool.h>
#include "llist.h"


#define HM_INIT_NUM_BUCKETS 16
#define HM_MAX_LOAD 0.75

typedef struct{
  llist_t ** buckets; //array of `buckets` each pointing to a list_t (see list.h)
  int num_buckets; //how many buckets, or lenght of the bucket array (should always be a power of 2)
  int size; //how many items stored
} hashmap_t;


//initliaze a hashmap with INITIAL_BUCKETS number of buckets
hashmap_t * hm_init();

//delete/deallocate the hashmap
void hm_delete(hashmap_t * hm);

//add a string value to the hashmap
void hm_add(hashmap_t * hm, char * v);

//see if a string value is in the hashmap
bool hm_check(hashmap_t * hm, char * v);


#endif
