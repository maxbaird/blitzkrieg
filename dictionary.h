#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

typedef void* ElementType;
struct AvlNode;
typedef struct AvlNode *Position;
typedef struct AvlNode *AvlTree;


AvlTree MakeEmpty( AvlTree T);
Position Find( ElementType X, AvlTree T );
AvlTree Insert( ElementType X, AvlTree T );
ElementType Retrieve( Position P );

#endif /* __DICTIONARY_H__ */
