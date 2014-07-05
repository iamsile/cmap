//
//  RBtree.c
//  cmap
//
//  Created by taowei on 14-5-19.
//  Copyright (c) 2014年 taowei. All rights reserved.
//

#include "rbtree.h"
#include <stdio.h>

static void __rb_rotate_left(struct RBNode *node, struct RBRoot *root)
{
	struct RBNode *right = node->RBRight;
	struct RBNode *parent = RBParent(node);

	if ((node->RBRight = right->RBLeft))
		RBSetParent(right->RBLeft, node);
	right->RBLeft = node;

	RBSetParent(right, parent);

	if (parent)
	{
		if (node == parent->RBLeft)
			parent->RBLeft = right;
		else
			parent->RBRight = right;
	}
	else
		root->RB_Node = right;
	RBSetParent(node, right);
}

static void __rb_rotate_right(struct RBNode *node, struct RBRoot *root)
{
	struct RBNode *left = node->RBLeft;
	struct RBNode *parent = RBParent(node);

	if ((node->RBLeft = left->RBRight))
		RBSetParent(left->RBRight, node);
	left->RBRight = node;

	RBSetParent(left, parent);

	if (parent)
	{
		if (node == parent->RBRight)
			parent->RBRight = left;
		else
			parent->RBLeft = left;
	}
	else
		root->RB_Node = left;
	RBSetParent(node, left);
}

void RBInsertColor(struct RBNode *node, struct RBRoot *root)
{
	struct RBNode *parent, *gparent;

	while ((parent = RBParent(node)) && RBIsRed(parent))
	{
		gparent = RBParent(parent);

		if (parent == gparent->RBLeft)
		{
			{
				register struct RBNode *uncle = gparent->RBRight;
				if (uncle && RBIsRed(uncle))
				{
					RBSetBlack(uncle);
					RBSetBlack(parent);
					RBSetRed(gparent);
					node = gparent;
					continue;
				}
			}

			if (parent->RBRight == node)
			{
				register struct RBNode *tmp;
				__rb_rotate_left(parent, root);
				tmp = parent;
				parent = node;
				node = tmp;
			}

			RBSetBlack(parent);
			RBSetRed(gparent);
			__rb_rotate_right(gparent, root);
		} else {
			{
				register struct RBNode *uncle = gparent->RBLeft;
				if (uncle && RBIsRed(uncle))
				{
					RBSetBlack(uncle);
					RBSetBlack(parent);
					RBSetRed(gparent);
					node = gparent;
					continue;
				}
			}

			if (parent->RBLeft == node)
			{
				register struct RBNode *tmp;
				__rb_rotate_right(parent, root);
				tmp = parent;
				parent = node;
				node = tmp;
			}

			RBSetBlack(parent);
			RBSetRed(gparent);
			__rb_rotate_left(gparent, root);
		}
	}

	RBSetBlack(root->RB_Node);
}
//EXPORT_SYMBOL(RBInsertColor);

static void __RBErase_color(struct RBNode *node, struct RBNode *parent,
							 struct RBRoot *root)
{
	struct RBNode *other;

	while ((!node || RBIsBlack(node)) && node != root->RB_Node)
	{
		if (parent->RBLeft == node)
		{
			other = parent->RBRight;
			if (RBIsRed(other))
			{
				RBSetBlack(other);
				RBSetRed(parent);
				__rb_rotate_left(parent, root);
				other = parent->RBRight;
			}
			if ((!other->RBLeft || RBIsBlack(other->RBLeft)) &&
			    (!other->RBRight || RBIsBlack(other->RBRight)))
			{
				RBSetRed(other);
				node = parent;
				parent = RBParent(node);
			}
			else
			{
				if (!other->RBRight || RBIsBlack(other->RBRight))
				{
					RBSetBlack(other->RBLeft);
					RBSetRed(other);
					__rb_rotate_right(other, root);
					other = parent->RBRight;
				}
				RBSetColor(other, RBColor(parent));
				RBSetBlack(parent);
				RBSetBlack(other->RBRight);
				__rb_rotate_left(parent, root);
				node = root->RB_Node;
				break;
			}
		}
		else
		{
			other = parent->RBLeft;
			if (RBIsRed(other))
			{
				RBSetBlack(other);
				RBSetRed(parent);
				__rb_rotate_right(parent, root);
				other = parent->RBLeft;
			}
			if ((!other->RBLeft || RBIsBlack(other->RBLeft)) &&
			    (!other->RBRight || RBIsBlack(other->RBRight)))
			{
				RBSetRed(other);
				node = parent;
				parent = RBParent(node);
			}
			else
			{
				if (!other->RBLeft || RBIsBlack(other->RBLeft))
				{
					RBSetBlack(other->RBRight);
					RBSetRed(other);
					__rb_rotate_left(other, root);
					other = parent->RBLeft;
				}
				RBSetColor(other, RBColor(parent));
				RBSetBlack(parent);
				RBSetBlack(other->RBLeft);
				__rb_rotate_right(parent, root);
				node = root->RB_Node;
				break;
			}
		}
	}
	if (node)
		RBSetBlack(node);
}

void RBErase(struct RBNode *node, struct RBRoot *root)
{
	struct RBNode *child, *parent;
	int color;

	if (!node->RBLeft)
		child = node->RBRight;
	else if (!node->RBRight)
		child = node->RBLeft;
	else
	{
		struct RBNode *old = node, *left;

		node = node->RBRight;
		while ((left = node->RBLeft) != NULL)
			node = left;

		if (RBParent(old)) {
			if (RBParent(old)->RBLeft == old)
				RBParent(old)->RBLeft = node;
			else
				RBParent(old)->RBRight = node;
		} else
			root->RB_Node = node;

		child = node->RBRight;
		parent = RBParent(node);
		color = RBColor(node);

		if (parent == old) {
			parent = node;
		} else {
			if (child)
				RBSetParent(child, parent);
			parent->RBLeft = child;

			node->RBRight = old->RBRight;
			RBSetParent(old->RBRight, node);
		}

		node->RBParentColor = old->RBParentColor;
		node->RBLeft = old->RBLeft;
		RBSetParent(old->RBLeft, node);

		goto color;
	}

	parent = RBParent(node);
	color = RBColor(node);

	if (child)
		RBSetParent(child, parent);
	if (parent)
	{
		if (parent->RBLeft == node)
			parent->RBLeft = child;
		else
			parent->RBRight = child;
	}
	else
		root->RB_Node = child;

color:
	if (color == RB_BLACK)
		__RBErase_color(child, parent, root);
}
//EXPORT_SYMBOL(RBErase);

static void rb_augment_path(struct RBNode *node, RBArgumentFunction func, void *data)
{
	struct RBNode *parent;

up:
	func(node, data);
	parent = RBParent(node);
	if (!parent)
		return;

	if (node == parent->RBLeft && parent->RBRight)
		func(parent->RBRight, data);
	else if (parent->RBLeft)
		func(parent->RBLeft, data);

	node = parent;
	goto up;
}

/*
 * after inserting @node into the tree, update the tree to account for
 * both the new entry and any damage done by rebalance
 */
void RBArgumentInsert(struct RBNode *node, RBArgumentFunction func, void *data)
{
	if (node->RBLeft)
		node = node->RBLeft;
	else if (node->RBRight)
		node = node->RBRight;

	rb_augment_path(node, func, data);
}
//EXPORT_SYMBOL(RBArgumentInsert);

/*
 * before removing the node, find the deepest node on the rebalance path
 * that will still be there after @node gets removed
 */
struct RBNode *RBArgumentEraseBegin(struct RBNode *node)
{
	struct RBNode *deepest;

	if (!node->RBRight && !node->RBLeft)
		deepest = RBParent(node);
	else if (!node->RBRight)
		deepest = node->RBLeft;
	else if (!node->RBLeft)
		deepest = node->RBRight;
	else {
		deepest = RBNext(node);
		if (deepest->RBRight)
			deepest = deepest->RBRight;
		else if (RBParent(deepest) != node)
			deepest = RBParent(deepest);
	}

	return deepest;
}
//EXPORT_SYMBOL(RBArgumentEraseBegin);

/*
 * after removal, update the tree to account for the removed entry
 * and any rebalance damage.
 */
void RBArgumentEraseEnd(struct RBNode *node, RBArgumentFunction func, void *data)
{
	if (node)
		rb_augment_path(node, func, data);
}
//EXPORT_SYMBOL(RBArgumentEraseEnd);

/*
 * This function returns the first node (in sort order) of the tree.
 */
struct RBNode *RBFirst(const struct RBRoot *root)
{
	struct RBNode	*n;

	n = root->RB_Node;
	if (!n)
		return NULL;
	while (n->RBLeft)
		n = n->RBLeft;
	return n;
}
//EXPORT_SYMBOL(RBFirst);

struct RBNode *RBLast(const struct RBRoot *root)
{
	struct RBNode	*n;

	n = root->RB_Node;
	if (!n)
		return NULL;
	while (n->RBRight)
		n = n->RBRight;
	return n;
}
//EXPORT_SYMBOL(RBLast);

struct RBNode *RBNext(const struct RBNode *node)
{
	struct RBNode *parent;

	if (RBParent(node) == node)
		return NULL;

	/* If we have a right-hand child, go down and then left as far
	 as we can. */
	if (node->RBRight) {
		node = node->RBRight;
		while (node->RBLeft)
			node=node->RBLeft;
		return (struct RBNode *)node;
	}

	/* No right-hand children.  Everything down and left is
	 smaller than us, so any 'next' node must be in the general
	 direction of our parent. Go up the tree; any time the
	 ancestor is a right-hand child of its parent, keep going
	 up. First time it's a left-hand child of its parent, said
	 parent is our 'next' node. */
	while ((parent = RBParent(node)) && node == parent->RBRight)
		node = parent;

	return parent;
}
//EXPORT_SYMBOL(RBNext);

struct RBNode *RBPrev(const struct RBNode *node)
{
	struct RBNode *parent;

	if (RBParent(node) == node)
		return NULL;

	/* If we have a left-hand child, go down and then right as far
	 as we can. */
	if (node->RBLeft) {
		node = node->RBLeft;
		while (node->RBRight)
			node=node->RBRight;
		return (struct RBNode *)node;
	}

	/* No left-hand children. Go up till we find an ancestor which
	 is a right-hand child of its parent */
	while ((parent = RBParent(node)) && node == parent->RBLeft)
		node = parent;

	return parent;
}
//EXPORT_SYMBOL(RBPrev);

void RBReplaceNode(struct RBNode *victim, struct RBNode *new,
					 struct RBRoot *root)
{
	struct RBNode *parent = RBParent(victim);

	/* Set the surrounding nodes to point to the replacement */
	if (parent) {
		if (victim == parent->RBLeft)
			parent->RBLeft = new;
		else
			parent->RBRight = new;
	} else {
		root->RB_Node = new;
	}
	if (victim->RBLeft)
		RBSetParent(victim->RBLeft, new);
	if (victim->RBRight)
		RBSetParent(victim->RBRight, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
//EXPORT_SYMBOL(RBReplaceNode);
