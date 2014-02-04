#include "stdafx.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "Node.h"
#include "Edge.h"
#define INITIAL_ARRAY_SIZE 1024

/*The text file we are creating index for.*/
char *text;

//Index of root state and auxiliary state.
int root, auxiliary;
Edge **edges;
int edgesCount;
Node **nodes;
int nodesCount;
int *suffixPointers;
int freeNodeIndex;
int freeEdgeIndex;

int initVal = -1;

void InitTree()
{

	auxiliary = 0;
	root = 1;
	nodesCount = INITIAL_ARRAY_SIZE;
	nodes = (Node**) malloc(INITIAL_ARRAY_SIZE * sizeof(Node));
	nodes[0] = Node_Create(0);
	nodes[1] = Node_Create(1);
	int a = nodes[1]->edges[0];
	freeNodeIndex = 2;

	suffixPointers = (int*) malloc(INITIAL_ARRAY_SIZE * (sizeof(int)));
	suffixPointers[1] = 0;

	edges = (Edge**) malloc(INITIAL_ARRAY_SIZE * sizeof(Edge));
	edgesCount = INITIAL_ARRAY_SIZE;
	return;
}



int CheckEdgeExist(Node *node, int charHash)
{
	if (node->from[charHash] < node->top && node->to[node->from[charHash]] == charHash)
		return node->edges[charHash];
	else
	{
		node->from[charHash] = node->top;
		node->to[node->top] = charHash;
		node->edges[charHash] = initVal;
		node->top++;

		return node->edges[charHash];
	}
}

Edge *FindEdge(Node *n, int k)
{
	Edge *e = edges[n->edges[(int) text[k]]];
	
	return e;
}

//explicit parent? canonical reference point. we only need such pair for active point?
int CheckEndPoint(Node *node, int k, int p, char c)
{
	if(k<=p)
	{
		Edge *e = FindEdge(node, k);
		int start = e->startIndex;
		if(c == text[start + p - k + 1])
			return 1;
		else
			return 0;
	}
	else
	{
		return CheckEdgeExist(node, (int) c) != -1 ? 0 : 1;
	}
}

void Canonize(Node *n, int *k, int *p)
{
	if(k > p) return; /*explicit case*/

	Edge *e = FindEdge(n, *k);
	int start = e->startIndex;
	int end = e->endIndex;

	while((end - start) <= (p - k))
	{
		k = k + end - start + 1;
		n = nodes[e->nodeIndex];
		if(k <= p)
			e = FindEdge(n, *k);
	}
}

void ConstructSTree(char* str)
{
	int s = root;
	int k = 0;
	int i = -1;

	for (int i = 0; str[i] != '\0'; i++)
	{
		i++;
		int charHash = (int) str[i];
		printf("%c", str[i]);
	}
	return;
}

int main(int argc, char *argv[])
{
	printf("%d\n", sizeof(Edge));
	printf("%d\n", sizeof(Edge*));
	printf("%d\n", sizeof(Node));
	printf("%d\n", sizeof(Node*));
	InitTree();
	text = "cocoa";
	ConstructSTree(text);
    return 0;
}