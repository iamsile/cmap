//
//  cmap.h
//  cmap
//
//  Created by taowei on 14-5-19.
//  Copyright (c) 2014年 taowei. All rights reserved.
//

#ifndef cmap_cmap_h
#define cmap_cmap_h

#include "RBtree.h"

//红黑树通用容器
typedef struct
{
	struct RBNode RBNode; //rbtree结构
	size_t dataLen; //data的长度
	void *key; //指向键
	void *data; //值
} RBTreeNode; //节点

/* 比较函数 key1 - key2 为从小到大排序*/
typedef int (*key_compare)(void *key1, void *key2);

typedef struct
{
	struct RBRoot root; //rbtreeƒ结构
	size_t keyLen; //key的长度（定长key）
	key_compare compare; //key的比较函数
} RBTree; //容器

/* 给节点设置key（参数为：节点指针，key的实际类型、值*/
#define RBTreeNodeSetKey(pnode,type,value) (*((type*)((pnode)->key)) = (value))

/*
 * 基本操作
 */

/* 初始化 */
void RBTreeInit(RBTree *tree, size_t key_length, key_compare key_compare_function);

/* 查找（无则返回NULL */
RBTreeNode* RBTreeSearch(RBTree *tree, void *key);

/* 插入（成功返回0，键重复返回-1 */
int RBTreeInsert(RBTree *tree, RBTreeNode *cont);

/* 删除（成功返回删除除了的节点，key不存在返回NULL） */
RBTreeNode *RBTreeDelete(RBTree *tree, void *key);

/* 替换（成功返回被替换掉的节点，cont的key不存在则返回NULL */
RBTreeNode *RBTreeReplace(RBTree *tree, RBTreeNode *cont);

/*
 * 迭代器（无则返回NULL）
 */

/* 获取迭代的开始节点 */
RBTreeNode *RBTreeFirst(RBTree *tree);

/* 获取迭代的结束节点 */
RBTreeNode *RBTreeLast(RBTree *tree);

/* 获取下一个节点（向前迭代） */
RBTreeNode *RBTreeNext(RBTreeNode *cont);

/* 获取上一个节点（向后迭代） */
RBTreeNode *RBTreePrev(RBTreeNode *cont);

/* 直接删除节点（必须保证cont在tree里面，否则产生未知错误 */
void RBTreeErase(RBTree *tree, RBTreeNode *cont);

/*
 * 内存分配
 */

/* 给节点分配内存（高效，节点的dataLen已赋值，key、data的位置已经挂载好，直接用即可 */
RBTreeNode *RBTreeNodeMalloc(RBTree *tree, size_t data_length);

/* 给节点数组分配内存，要求数据部分定长（高效，节点的dataLen已赋值，key、data的位置已经挂载好，直接用即可 */
RBTreeNode *RBTreeNodeArrayMalloc(RBTree *tree, size_t data_length, size_t array_length);

/* 释放节点的内存（和RBTreeNodeMalloc或RBTreeNodeMalloc_array匹配使用 */
void RBTreeNodeFree(RBTreeNode *tree_node);


#endif
