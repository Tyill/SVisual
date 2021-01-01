#pragma once

namespace svisual{
  
class Map{
           
  private:
     
    struct node {
      const char* key;
      const void* data;
      node* lchild,* rchild;
    };
   
    node* root_ = NULL;
    node** nods_ = NULL;
    
    int sz_ = 0;
    
    node* create(const char* key, const void* data){
      node* nd = (node*)malloc(sizeof(node));
      nd->key = key;
      nd->data = data;      
      nd->lchild = NULL;
      nd->rchild = NULL;
      return nd;
    }  
     
    void insert(node* new_node) {
      node** nd = &root_;  
      while (*nd != NULL) {
        if (strcmp(new_node->key, (*nd)->key) < 0) nd = &(*nd)->lchild;
        else nd = &(*nd)->rchild; 
      }
      ++sz_;
      nods_ = (node**)realloc(nods_, sz_ * sizeof(node*));
      nods_[sz_ - 1] = new_node;
      *nd = new_node;    
    }
     
  public:
            
    void* at(int it){
      if ((0 > it) || (it >= sz_)) return NULL;
      return (void*)nods_[it]->data;
    }
          
    void insert(const char* key, const void* data){
      node* new_node = create(key, data);
      insert(new_node);  
    }
       
    void* find(const char* key) {
      node* nd = root_;
      while (nd != NULL) {
        int res = strcmp(key, nd->key);
        if (res == 0) return (void*)nd->data;
        else if (res < 0) nd = nd->lchild;
        else nd = nd->rchild;
      }
      return NULL;
    }
    
    int size(){
      return sz_;  
    }    
  };
}
