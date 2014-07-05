//
//  cmap.c
//  cmap
//
//  Created by taowei on 14-5-19.
//  Copyright (c) 2014年 taowei. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "cmap.h"

void RBTreeInit(RBTree *tree, size_t key_length, key_compare key_compare_function)
{
#ifdef _C99
	tree->root = RB_ROOT;
#else
	tree->root.RBNode = NULL;
#endif
	tree->compare = key_compare_function;
	tree->keyLen = key_length;
}

RBTreeNode *RBTreeSearch(RBTree *tree, void *key)
{
	struct RBNode *node = tree->root.RB_Node;

	while (node)
	{
		RBTreeNode *cur = rb_entry(node, RBTreeNode, RBNode);
		printf("123123    %s\n", (char *)key);
		printf("........%s\n", (char *)(cur->key));
		int result = tree->compare((char *)key, (char *)(cur->key));

		if (result < 0)
			node = node->RBLeft;
		else if (result > 0)
			node = node->RBRight;
		else
			return cur;
	}
	return NULL;
}

int RBTreeInsert(RBTree *tree, RBTreeNode *cont)
{
	struct RBNode **new = &(tree->root.RB_Node);
	struct RBNode  *parent = NULL;

	while (*new)
	{
		RBTreeNode *cur = rb_entry(*new, RBTreeNode, RBNode);
		//printf("111   %s\n", *(char **)(cont->data));
		int result = tree->compare(cont->key, cur->key);
		//printf("222   %s\n", *(char **)(cur->key));

		parent = *new;

		if (result < 0)
			new = &((*new)->RBLeft);
		else if (result > 0)
			new = &((*new)->RBRight);
		else
			return -1;
	}

	RBLinkNode(&(cont->RBNode), parent, new);
	RBInsertColor(&(cont->RBNode), &(tree->root));

	return 0;
}

RBTreeNode *RBTreeDelete(RBTree *tree, void *key)
{
	RBTreeNode *find = RBTreeSearch(tree, key);
	if (!find)
		return NULL;
	RBErase(&find->RBNode, &(tree->root));
	return find;
}

RBTreeNode *RBTreeReplace(RBTree *tree, RBTreeNode *cont)
{
	RBTreeNode *find = RBTreeSearch(tree, cont->key);
	if (!find)
		return NULL;
	RBReplaceNode(&(find->RBNode), &(cont->RBNode), &(tree->root));
	return find;
}

RBTreeNode *RBTreeFirst(RBTree *tree)
{
	struct RBNode *node = RBFirst(&(tree->root));
	if (!node)
		return NULL;
	return rb_entry(node, RBTreeNode, RBNode);
}

RBTreeNode *RBTreeLast(RBTree *tree)
{
	struct RBNode *node = RBLast(&(tree->root));
	if (!node)
		return NULL;
	return rb_entry(node, RBTreeNode, RBNode);
}

RBTreeNode *RBTreeNext(RBTreeNode *cont)
{
	struct RBNode *node = RBNext(&(cont->RBNode));
	if (!node)
		return NULL;
	return rb_entry(node, RBTreeNode, RBNode);
}

RBTreeNode *RBTreePrev(RBTreeNode *cont)
{
	struct RBNode *node = RBPrev(&(cont->RBNode));
	if (!node)
		return NULL;
	return rb_entry(node, RBTreeNode, RBNode);
}

void RBTreeErase(RBTree *tree, RBTreeNode *cont)
{
	RBErase(&(cont->RBNode), &(tree->root));
}

RBTreeNode *RBTreeNodeMalloc(RBTree *tree, size_t data_length)
{
	RBTreeNode *p = (RBTreeNode *)malloc(sizeof(RBTreeNode) + tree->keyLen + data_length);
	p->dataLen = data_length;
	p->key = (void*)(((char*)p) + sizeof(RBTreeNode));
	p->data = (unsigned char*)(((char*)p) + sizeof(RBTreeNode) + tree->keyLen);
	return p;
}

RBTreeNode *RBTreeNodeArrayMalloc(RBTree *tree,
														  size_t data_length, size_t array_length)
{
	RBTreeNode *p = (RBTreeNode *)malloc((sizeof(RBTreeNode) + tree->keyLen + data_length) * array_length);
	size_t i;
	for(i=0; i<array_length; i++)
	{
		p[i].dataLen = data_length;
		p[i].key = (void*)(((char*)p) + sizeof(RBTreeNode) * array_length + tree->keyLen * i);
		p[i].data = (unsigned char*)(((char*)p) + (sizeof(RBTreeNode) + tree->keyLen) * array_length + data_length * i);
	}
	return p;
}

void RBTreeNodeFree(RBTreeNode *tree_node)
{
	free(tree_node);
}
