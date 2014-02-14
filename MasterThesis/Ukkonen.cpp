#include "stdafx.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "Node.h"
#include "NodeEdge.h"
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
unsigned *from;
unsigned *to;
unsigned top;

NodeEdge **nodeEdges;
int nodeEdgesCount;

int freeNodeIndex;
int freeEdgeIndex;
int freeNodeEdgeIndex;
int currentEnd = 0;

int initVal = -1;
void InitTree();
void ConstructSTree(char* str);
int CheckEdgeExist(Node *node, char c, int *index);
void SetEdge(Node *n, char c, int val);
int CheckSuffixExist(int index);
void SetSuffix(int index, int val);
int CheckEndPoint(Node *node, int k, int p, char c);
//Edge *FindEdge(Node *n, int k);
Node *CreateNewNode(int textIndex);
Edge *CreateNewEdge(int startIndex, int endIndex, int nodeIndex);
void Canonize(Node *n, int *k, int *p);
void UpdateTree(Node *s, int *k, int p);

void InitTree()
{
	auxiliary = 0;
	root = 1;
	nodesCount = INITIAL_ARRAY_SIZE;
	nodes = (Node**) malloc(INITIAL_ARRAY_SIZE * sizeof(Node));
	//Don't really need to set an edge from the auxiliary state to the root.
	nodes[0] = Node_Create(0, -1, -1);
	nodes[1] = Node_Create(1, -1, -1);
	//int a = nodes[1]->edges[0];
	freeNodeIndex = 2;

	suffixPointers = (int*) malloc(INITIAL_ARRAY_SIZE * (sizeof(int)));
	from = (unsigned*) malloc(INITIAL_ARRAY_SIZE * (sizeof(unsigned)));
	to = (unsigned*) malloc(INITIAL_ARRAY_SIZE * (sizeof(unsigned)));
	top = 0;
	suffixPointers[1] = 0;
	SetSuffix(1, 0);

	edges = (Edge**) malloc(INITIAL_ARRAY_SIZE * sizeof(Edge));
	edges[0] = Edge_Create(0, 0, -1, 1);
	edgesCount = INITIAL_ARRAY_SIZE;
	freeEdgeIndex = 1;

	nodeEdges = (NodeEdge**) malloc(INITIAL_ARRAY_SIZE * sizeof(NodeEdge));
	nodeEdgesCount = INITIAL_ARRAY_SIZE;
	freeNodeEdgeIndex = 0;

	return;
}

int CheckEdgeExist(Node *node, char c, int *index)
{
	//If this is the auxiliary state, a transition exists.
	if(node->arrayIndex == 0)
	{
		*index = 0;
		return 1;
	}

	int edgeIndex = node->edgeStartIndex;
	if(edgeIndex < 0)
		return 0;

	for (int i = 0; i < node->numberofEdges; i++)
	{
		NodeEdge* ne = nodeEdges[edgeIndex];
		if(ne->symbol == c)
		{
			(*index) = edgeIndex;
			return 1;
		}
		
		if(ne->nextElement < 0)
			break;
		else
			edgeIndex = ne->nextElement;
	}
	//Set Edge??
	return 0;
}

void SetEdge(Node *node, char c, int val)
{
	nodeEdges[freeNodeEdgeIndex] = NodeEdge_Create(c, val);
	node->lastEdgeIndex = freeNodeEdgeIndex;
	freeNodeEdgeIndex++;
	
	if(freeNodeEdgeIndex == nodeEdgesCount)
	{
		nodeEdgesCount *=2;
		NodeEdge **temp = (NodeEdge**) realloc(nodeEdges, nodeEdgesCount * sizeof(NodeEdge));

		if(temp != NULL)
			nodeEdges = temp;
	}

}

int CheckSuffixExist(int index)
{
	if(from[index] < top && to[from[index]] == index)
		return suffixPointers[index];
	else
	{
		SetSuffix(index, initVal);

		return suffixPointers[index];
	}
}

void SetSuffix(int index, int val)
{
	from[index] = top;
	to[top] = index;
	suffixPointers[index] = val;
	top++;
}

//Edge *FindEdge(Node *n, int k)
//{
//	//Edge *e = edges[n->edges[(int) text[k]]];
//	Edge *e = Edge_Create(0,0,0,0);
//	return e;
//}

//explicit parent? canonical reference point. we only need such pair for active point?
int CheckEndPoint(Node *node, int k, int p, char c)
{
	if(k<=p)
	{
		int edgeIndex = 0;
		Edge *e = edges[CheckEdgeExist(node, text[k], &edgeIndex)];
		int start = e->startIndex;
		if(c == text[start + p - k + 1])
			return 1;
		else
			return 0;
	}
	else
	{
		int index = -1;
		return CheckEdgeExist(node, c, &index);
	}
}

void Canonize(Node *n, int *k, int p)
{
	if(*k > p) return; /*explicit case*/

	int edgeIndex = 0;
	CheckEdgeExist(n, text[(*k)], &edgeIndex);
	Edge *e = edges[edgeIndex];
	int start = e->startIndex;
	int end = e->endIndex;

	while((end - start) <= (p - (*k)))
	{
		*k = (*k) + end - start + 1;
		n = nodes[e->nodeIndex];
		if(*k <= p)
		{
			CheckEdgeExist(n, text[(*k)], &edgeIndex);
			e = edges[edgeIndex];
		}
	}
}

void ConstructSTree(char* str)
{
	Node* s = nodes[root];
	int k = 0;

	for (int i = 0; str[i] != '\0'; i++)
	{
		currentEnd = i;
		UpdateTree(s, &k, currentEnd);
	}
	return;
}

/*Returns the index of the newly created node*/
int SplitEdge(Node *s, int k, int p)
{
	char c = text[k];
	int edgeIndex = -1;
	CheckEdgeExist(s, c, &edgeIndex);

	//let (s, (k', p'), s') be the w[k]-edge from s
	Edge *e = edges[edgeIndex];
	//k'
	int start = e->startIndex;
	//p'
	int end = e->endIndex;
	int endNodeIndex = e->nodeIndex;

	Node *r = CreateNewNode(end-1);

	//(s, (k', k' + p - k, r)
	e->endIndex = end + p - k;
	e->nodeIndex = r->arrayIndex;

	//(r, (k' + p - k +1, p'), s')
	int newStart = start + p - k + 1;
	Edge* newEdge = CreateNewEdge(newStart, end, endNodeIndex);

	//r->edges[(char) text[newStart]] = newEdge->arrayIndex;
	SetEdge(r, text[newStart], newEdge->arrayIndex);

	return r->arrayIndex;
}

Node *CreateNewNode(int textIndex)
{
	Node *newNode = Node_Create(freeNodeIndex, textIndex, -1);
	nodes[freeNodeIndex] = newNode;
	freeNodeIndex++;

	if(freeNodeIndex == nodesCount)
	{
		nodesCount *=2;
		Node **temp =(Node**) realloc(nodes, nodesCount * sizeof(Node));
		if(temp != NULL)
			nodes = temp;
	}

	return newNode;
}

Edge *CreateNewEdge(int startIndex, int endIndex, int nodeIndex)
{
	Edge *newEdge = Edge_Create(freeEdgeIndex, startIndex, endIndex, nodeIndex);
	edges[freeEdgeIndex] = newEdge;
	freeEdgeIndex++;

	if(freeEdgeIndex == edgesCount)
	{
		edgesCount *=2;
		Edge **temp =(Edge**) realloc(edges, edgesCount * sizeof(Edge));
		if(temp != NULL)
			edges = temp;
	}

	return newEdge;
}

void UpdateTree(Node *s, int *k, int p)
{
	/*(s, (k, p-1)) is the canonical reference pair for the active point*/
	char c = text[p];
	Node *oldr = NULL;
	
	while(!CheckEndPoint(s, *k, p - 1, c))
	{
		Node *r = NULL;
		/*implicit case*/
		if(*k <= (p-1))
			r = nodes[SplitEdge(s, *k, p - 1)];
		/*explicit case*/
		else
			r = s;

		//TODO: Set edgeStartIndex of the previous node. Include newNode to nodes
		Node *newNode = CreateNewNode(p);
		Edge *e = CreateNewEdge(p, currentEnd, newNode->arrayIndex);
		SetEdge(r, text[p], e->arrayIndex);

		if(oldr != NULL)
			SetSuffix(oldr->arrayIndex, r->arrayIndex);

		oldr = r;
		s = nodes[suffixPointers[s->arrayIndex]];
		Canonize(s, k, p - 1);
	}
	
	if(oldr != NULL)
	{
		//If the suffix is not already set, do it.
		int index = oldr->arrayIndex;
		if(!(from[index] < top && to[from[index]] == index))
			SetSuffix(index, s->arrayIndex);
	}

	Canonize(s, k, p);
}

int main(int argc, char *argv[])
{
	InitTree();
	text = "cocoa";
	ConstructSTree(text);
    return 0;
}