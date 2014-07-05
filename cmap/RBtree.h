//
//  RBtree.h
//  cmap
//
//  Created by taowei on 14-5-19.
//  Copyright (c) 2014年 taowei. All rights reserved.
//

#ifndef cmap_RBtree_h
#define cmap_RBtree_h

#ifdef _WIN32
#else
#define _C99
#endif

#include <stddef.h>

struct RBNode
{
	unsigned long  RBParentColor;
#define	RB_RED		0
#define	RB_BLACK	1
	struct RBNode *RBRight;
	struct RBNode *RBLeft;
} /*__attribute__((aligned(sizeof(long))))*/; // is need?

struct RBRoot
{
	struct RBNode *RB_Node;
};

#define RBParent(r)   ((struct RBNode *)((r)->RBParentColor & ~3))
#define RBColor(r)   ((r)->RBParentColor & 1)
#define RBIsRed(r)   (!RBColor(r))
#define RBIsBlack(r) RBColor(r)
#define RBSetRed(r)  do { (r)->RBParentColor &= ~1; } while (0)
#define RBSetBlack(r)  do { (r)->RBParentColor |= 1; } while (0)

static
#ifdef _C99
inline
#endif
void RBSetParent(struct RBNode *rb, struct RBNode *p)
{
	rb->RBParentColor = (rb->RBParentColor & 3) | (unsigned long)p;
}
static
#ifdef _C99
inline
#endif
void RBSetColor(struct RBNode *rb, int color)
{
	rb->RBParentColor = (rb->RBParentColor & ~1) | color;
}

/**********************************************************/
#ifndef offsetof
#define offsetof(type, member) \
(size_t)&(((type *)0)->member)
#endif

#ifndef container_of
#define container_of(ptr, type, member)  \
({\
const typeof(((type *)0)->member) * __mptr = (ptr);\
(type *)((char *)__mptr - offsetof(type, member)); \
})
#endif
/**********************************************************/

#define RB_ROOT	(struct RBRoot) { NULL, }
#ifdef _C99
#define	rb_entry(ptr, type, member) container_of(ptr, type, member)
#else
#define rb_entry(ptr, type, member)  ((type *)ptr)
#endif

#define RB_EMPTY_ROOT(root)	((root)->RBNode == NULL)
#define RB_EMPTY_NODE(node)	(RBParent(node) == node)
#define RB_CLEAR_NODE(node)	(RBSetParent(node, node))

static
#ifdef _C99
inline
#endif
void RBInit(struct RBNode *rb)
{
	rb->RBParentColor = 0;
	rb->RBRight = NULL;
	rb->RBLeft = NULL;
	RB_CLEAR_NODE(rb);
}

extern void RBInsertColor(struct RBNode *, struct RBRoot *);
extern void RBErase(struct RBNode *, struct RBRoot *);

typedef void (*RBArgumentFunction)(struct RBNode *node, void *data);

extern void RBArgumentInsert(struct RBNode *node, RBArgumentFunction func, void *data);
extern struct RBNode *RBArgumentEraseBegin(struct RBNode *node);
extern void RBArgumentEraseEnd(struct RBNode *node, RBArgumentFunction func, void *data);

/* Find logical next and previous nodes in a tree */
extern struct RBNode *RBNext(const struct RBNode *);
extern struct RBNode *RBPrev(const struct RBNode *);
extern struct RBNode *RBFirst(const struct RBRoot *);
extern struct RBNode *RBLast(const struct RBRoot *);

/* Fast replacement of a single node without remove/rebalance/add/rebalance */
extern void RBReplaceNode(struct RBNode *victim, struct RBNode *new, struct RBRoot *root);

static
#ifdef _C99
inline
#endif
void RBLinkNode(struct RBNode * node, struct RBNode * parent,
				  struct RBNode ** rb_link)
{
	node->RBParentColor = (unsigned long )parent;
	node->RBLeft = node->RBRight = NULL;

	*rb_link = node;
}


#endif
