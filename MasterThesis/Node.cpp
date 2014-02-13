#include "stdafx.h"
#include <stdlib.h>
#include "Node.h"
#include <assert.h>

Node *Node_Create(int arrayIndex, int textIndex)
{
	struct Node *node = (Node*) malloc(sizeof(struct Node));
	assert (node != NULL);

	node->arrayIndex = arrayIndex;
	node->textIndex = textIndex;
	node->numberofEdges = 0;
	node->edgeStartIndex = -1;
	node->lastEdgeIndex = -1;

	return node;
}