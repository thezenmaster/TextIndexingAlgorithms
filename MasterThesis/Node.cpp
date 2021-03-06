#include "stdafx.h"
#include <stdlib.h>
#include "Node.h"
#include <assert.h>

Node *Node_Create(int arrayIndex, int stringIndex, int isLeaf)
{
	struct Node *node = (Node*) malloc(sizeof(struct Node));
	assert (node != NULL);

	node->arrayIndex = arrayIndex;
	node->stringIndex = stringIndex;
	node->isLeaf = isLeaf;
	node->numberofEdges = 0;
	node->edgeStartIndex = -1;
	node->currentEdgeIndex = -1;
	node->baseAddress = -1;

	return node;
}