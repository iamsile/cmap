//
//  main.c
//  cmap
//
//  Created by taowei on 14-5-19.
//  Copyright (c) 2014年 taowei. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cmap.h"

int compare(const void *key1, const void *key2)
{
	printf("%s\n", (char *)key1);
	printf("%s\n", (char *)key2);
	if (strcmp((char *)key1, (char *)key2) == 0) {
		return 0;
	}
	if (strcmp((char *)key1, (char *)key2) > 0) {
		return 1;
	}
	return -1;
	//return strcmp((char *)key1, (char *)key2);
}

int main(void)
{
	RBTree tree;
	RBTreeNode *pc, *tpc, *xx;
	int ret, find, erase, replace;

	RBTreeInit(&tree, sizeof(tree), compare);
	FILE *infile = fopen("/Users/taowei/Documents/工程/posyuliao1.txt", "r");
	ssize_t read;
	size_t len = 0;
	char *line = NULL;

//	while ((read = getline(&line, &len, infile)) != EOF) {
//		//fgets(line, 1024, infile);
//		puts(line);
//		pc = RBTreeSearch(&tree, &line);
//		if (pc != 0) {
//			*(int *)(pc->data) += 1;
//			printf("%d\n", *(int *)(pc->data));
//			puts("fuck");
//		} else {
//			pc = RBTreeNodeMalloc(&tree, 100);
//			RBTreeNodeSetKey(pc, char *, line);
//			*(int *)(pc->data) = 1;
//			RBTreeInsert(&tree, pc);
//		}
//	}

	RBTreeInit(&tree, sizeof(tree), compare);
	char a[][12] = {"中国", "美国", "日本", "中国", "韩国", "美国", "澳大利亚", "美国"};
	pc = RBTreeNodeMalloc(&tree, 100);
//	for (int i = 0; i < 8; i++) {
//		pc = RBTreeSearch(&tree, &a[i]);
//		puts(a[i]);
//		if (pc != 0) {
//			*(int *)(pc->data) += 1;
//			puts("fuck");
//		} else {
//			xx = RBTreeNodeMalloc(&tree, 100);
//			*(char **)(xx->key) = a[i];
//			*(int *)(xx->data) = 1;
//			ret = RBTreeInsert(&tree, xx);
//			printf("%d\n", ret);
//			//RBTreeNodeFree(xx);
//		}
//	}
//	pc = RBTreeSearch(&tree, "中国");
//	if (pc != 0) {
//		printf("%d\n", *(int *)(pc->data));
//	}
	char ss[20] = {0};
	while ((read = getline(&line, &len, infile)) != EOF) {
		puts(line);
		for (int i = 0; i < strlen(line) - 1; i++) {
			ss[i] = line[i];
		}
		pc = RBTreeSearch(&tree, &ss);
		if (pc != 0) {
			*(int *)(pc->data) += 1;
			printf("fuck\n");
		} else {
			xx = RBTreeNodeMalloc(&tree, 100);
			xx->key = (char *)malloc(strlen(ss) + 1);
			strcpy(xx->key, ss);
//			*(char **)(xx->key) = ss;
			*(int *)(xx->data) = 1;
			ret = RBTreeInsert(&tree, xx);
			printf("%d\n", ret);
		}
		for (int i = 0; i < strlen(line) - 1; i++) {
			ss[i] = '\0';
		}
	}
	pc = RBTreeSearch(&tree, "中国");
	if (pc != 0) {
		printf(".....................%d\n", *(int *)(pc->data));
	}
	return 0;
}


