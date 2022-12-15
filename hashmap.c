#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <math.h>
#include <string.h>

#include "llist.h"
#include "hashmap.h"

//local function for hashing an input value (do not edit)
unsigned int _hash(hashmap_t * hm, char * val){
  //based on Java's String.hashcode()
  double k = 0;
  double len = (double) strlen(val);
  int i=1;
  for(char * c = val; *c ; c++,i++){
    k += (*c) * pow(31,len-i);
  }

  return (unsigned int) (((unsigned long) k) % hm->num_buckets);
}


//local function: resize the hashmap by doubling the number of buckets and rehashing
void _resize(hashmap_t * hm){

  // Resize hashmap by doubling buckets
  // Do this by creating new buckets and rehashing all values
  // then swap the new buckets with the old buckets and delete the old buckets

  // Create new buckets
  int old_num_buckets = hm->num_buckets;
  int new_num_buckets = old_num_buckets * 2;
  llist_t ** new_buckets = calloc(new_num_buckets, sizeof(llist_t *));
  for (int i = 0; i < new_num_buckets; i++) {
    new_buckets[i] = ll_init();
  }

  hm->num_buckets = new_num_buckets;

  // Rehash all values
  for (int i = 0; i < old_num_buckets; i++) {
    llist_t * bucket = hm->buckets[i];
    ll_node_t * node = bucket->head;
    while (node != NULL) {
      char * val = node->val;
      unsigned int h = _hash(hm, val);
      ll_push(new_buckets[h], val);
      node = node->next;
    }
  }

  // Swap new buckets with old buckets
  llist_t ** old_buckets = hm->buckets;
  hm->buckets = new_buckets;

  // Delete old buckets
  for (int i = 0; i < old_num_buckets; i++) {
    ll_delete(old_buckets[i]);
  }
  free(old_buckets);
}


//initliaze a hashmap with initial number of buckets
hashmap_t * hm_init(){

  //create the hashmap
  hashmap_t * hm = calloc(1, sizeof(hashmap_t));
  hm->num_buckets = HM_INIT_NUM_BUCKETS;
  hm->size = 0;

  //create the buckets
  hm->buckets = calloc(hm->num_buckets, sizeof(llist_t *));
  for(int i=0; i<hm->num_buckets; i++){
    hm->buckets[i] = ll_init();
  }
  
  return hm;
}


//delete/deallocate the hashmap
void hm_delete(hashmap_t * hm){
  for(int i=0; i<hm->num_buckets; i++){
    ll_delete(hm->buckets[i]);
  }
  free(hm->buckets);

  //delete the hashmap
  free(hm);
}

//add a string value to the hashmap
void hm_add(hashmap_t * hm, char * val){
  // You get this function for free :)
  
  //check the load on the hashtable, if > max load, resize!
  if(((hm->size+1)/(double) hm->num_buckets) > HM_MAX_LOAD){
    _resize(hm); 
  }

  unsigned int h = _hash(hm,val); //get hash
  ll_push(hm->buckets[h],val); //note that list does string dup
  hm->size++; //increase size


}

//see if a string value is in the hashmap
bool hm_check(hashmap_t * hm, char * val){

  unsigned int h = _hash(hm,val); //get hash

  //check if the value is in the list
  ll_node_t * cur = hm->buckets[h]->head;
  while(cur){
    if(strcmp(cur->val,val) == 0){
      return true;
    }
    cur = cur->next;
  }

  return false; //otherwise false
}


