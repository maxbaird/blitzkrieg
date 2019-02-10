#ifndef __TRIE_H__
#define __TRIE_H__

#include <stdbool.h>

struct TrieNode;
typedef struct TrieNode* Trie;

Trie TrieMakeEmpty(Trie t);
void TrieInsert(Trie t, const char *key);
bool TrieSearch(Trie t, const char *key);
Trie TrieFree(Trie t);

#endif /* __TRIE_H__ */
