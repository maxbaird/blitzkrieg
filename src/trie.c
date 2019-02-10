#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "trie.h"

#define ALPHABET_SIZE 26

struct TrieNode{
  struct TrieNode* children[ALPHABET_SIZE];
  bool isEndOfWord;
};

static void safeFree (void **ptr){
  if (ptr != NULL && *ptr != NULL){
    free(*ptr);
    *ptr = NULL;
  }
}

static size_t char_to_index(char c){
  return (size_t)c - (size_t)'a';
}

static bool uninitialized(Trie t){
  return t == NULL;
}

static bool checkInit(Trie t, const char *func){
	if(uninitialized(t)){
		fprintf(stderr, "[%s Error]: Trie not initialized!\n", func);
		return false;
	}

	return true;
}

static Trie makeTrie(){
  Trie trie = NULL;

  trie = malloc(sizeof(struct TrieNode));

  if(trie == NULL){
    fprintf(stderr, "Out of memory for trie!\n");
    exit(EXIT_FAILURE);
  }

  trie->isEndOfWord = false;
  memset(trie->children, 0, sizeof(Trie) * ALPHABET_SIZE);

  return trie;
}

static void freeTrie(Trie t){
  if(!uninitialized(t)){
    size_t i = 0;
    for(i = 0; i < ALPHABET_SIZE; i++){
      freeTrie(t->children[i]);
    }
  }

  safeFree((void **)&t);
}

Trie TrieMakeEmpty(Trie t){
  freeTrie(t);
  t = makeTrie();
  return t;
}

Trie TrieFree(Trie t){
  freeTrie(t);
	return NULL;
}

void TrieInsert(Trie t, const char *key){
  if(!checkInit(t, __func__)){
    return;
  }

  size_t level = 0;
  size_t length = 0;
  size_t index = 0;

  length = strlen(key);
  Trie trie = t;

  for(level = 0; level < length; level++){
    index = char_to_index(key[level]);

    if(trie->children[index] == NULL){
      trie->children[index] = makeTrie();
    }
    trie = trie->children[index];
  }

  // Mark last node as leaf
  trie->isEndOfWord = true;
}

bool TrieSearch(Trie t, const char *key){
  if(!checkInit(t, __func__)){
    return false;
  }

  size_t level = 0;
  size_t length = 0;
  size_t index = 0;

  length = strlen(key);
  Trie trie = t;

  for(level = 0; level < length; level++){
    index = char_to_index(key[level]);

    if(trie->children[index] == NULL){
      return false;
    }

    trie = trie->children[index];
  }

  return (trie != NULL && trie->isEndOfWord);
}
