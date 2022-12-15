#include <stdlib.h>
#include <string.h>

#include "llist.h"

//initialize a linked list
llist_t * ll_init(){
  llist_t * ll = malloc(sizeof(llist_t));
  ll->size = 0; //initiali size is 0
  ll->head = NULL; //the head is null
  return ll; 
}

//delete a linked list
void ll_delete(llist_t * ll){
  ll_node_t * cur = ll->head;
  while(ll->head){
    cur = ll->head;
    ll->head = cur->next;
    
    //free the node
    free(cur->val); //free's from strdup()    
    free(cur);

    //reduce the size
    ll->size--;
  }    
  free(ll); //free the llist itself
}


//push an item onto the front of the list
void ll_push(llist_t * ll, char * s){
  //allocate
  ll_node_t * n = malloc(sizeof(ll_node_t));
  
  n->val = strdup(s); //duplicate the string
  
  //set this node to the new head
  n->next = ll->head;
  ll->head = n;
  
  ll->size++; //increase size
}




