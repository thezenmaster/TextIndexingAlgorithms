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
	node->edges = (int*) malloc(128 * sizeof(int));
	node->from = (unsigned*) malloc(128 * sizeof(unsigned));
	node->to = (unsigned*) malloc(128 * sizeof(unsigned));
	node->top = 0;

	return node;
}