#include "stdafx.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "Node.h"
#include "NodeEdge.h"
#include "Edge.h"
#include "CompressedTransitionTable.h"
#define INITIAL_ARRAY_SIZE 1024

const int asciiStartIndex = 32;

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

int wordsCount;
int *words;

NodeEdge **nodeEdges;
int nodeEdgesCount;

int freeNodeIndex;
int freeEdgeIndex;
int freeNodeEdgeIndex;
int currentEnd = 0;

/*Properties of the compressed transition table.*/
int* check;
int* next;
int compressedTableLength;

/*The original 95 printable ASCII characters*/
const int alphabetSize = 95;
int initVal = -1;
void InitTree();
void ConstructSTree(char* str);
int CheckEdgeExist(Node *node, char c, int *index);
void SetEdge(Node *n, char c, int val);
int CheckSuffixExist(int index);
void SetSuffix(int index, int val);
int CheckEndPoint(Node *node, int k, int p, char c);
//Edge *FindEdge(Node *n, int k);
Node *CreateNewNode(int stringIndex, int isLeaf);
Edge *CreateNewEdge(int startIndex, int nodeIndex);
void Canonize(Node *n, int *k, int p);
Node* UpdateTree(Node *s, int *k, int p, char c, int stringIndex);
int SplitEdge(Node *s, int k, int p, int stringIndex);
char* GetInputAsString();

void InitTree()
{
	auxiliary = 0;
	root = 1;
	nodesCount = INITIAL_ARRAY_SIZE;
	nodes = (Node**) malloc(INITIAL_ARRAY_SIZE * sizeof(Node));
	//Don't really need to set an edge from the auxiliary state to the root.
	nodes[0] = Node_Create(0, -1, 0);
	nodes[1] = Node_Create(1, -1, 0);
	//int a = nodes[1]->edges[0];
	freeNodeIndex = 2;

	suffixPointers = (int*) malloc(INITIAL_ARRAY_SIZE * (sizeof(int)));
	from = (unsigned*) malloc(INITIAL_ARRAY_SIZE * (sizeof(unsigned)));
	to = (unsigned*) malloc(INITIAL_ARRAY_SIZE * (sizeof(unsigned)));
	top = 0;
	suffixPointers[1] = 0;
	SetSuffix(1, 0);

	wordsCount = INITIAL_ARRAY_SIZE;
	words = (int*) malloc(INITIAL_ARRAY_SIZE * (sizeof(int)));

	edges = (Edge**) malloc(INITIAL_ARRAY_SIZE * sizeof(Edge));
	//There is an edge from the auxiliary state to the root for every letter of the alphabet.
	edges[0] = Edge_Create(0, -1, 1);
	edgesCount = INITIAL_ARRAY_SIZE;
	freeEdgeIndex = 1;

	nodeEdges = (NodeEdge**) malloc(INITIAL_ARRAY_SIZE * sizeof(NodeEdge));
	nodeEdgesCount = INITIAL_ARRAY_SIZE;
	freeNodeEdgeIndex = 0;

	return;
}

int CheckEdgeExist(Node *node, char c, int *index)
{
	/*If this is the auxiliary state, a transition exists.*/
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
			(*index) = nodeEdges[edgeIndex]->edgeIndex;
			return 1;
		}
		
		if(ne->nextElement < 0)
			break;
		else
			edgeIndex = ne->nextElement;
	}
	return 0;
}

void SetEdge(Node *node, char c, int val)
{
	nodeEdges[freeNodeEdgeIndex] = NodeEdge_Create(c, val);
	if(node->edgeStartIndex == -1)
		node->edgeStartIndex = freeNodeEdgeIndex;
	
	if(node->currentEdgeIndex != -1)
	{
		nodeEdges[node->currentEdgeIndex]->nextElement = freeNodeEdgeIndex;
	}

	node->currentEdgeIndex = freeNodeEdgeIndex;

	node->numberofEdges++;
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
	if(k<=p) //Implicit case.
	{
		int edgeIndex = 0;
		//Should we check if the edge is not null?
		CheckEdgeExist(node, text[k], &edgeIndex);
		Edge *e = edges[edgeIndex];
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

void Canonize(Node **n, int *k, int p)
{
	if(*k > p) return; /*explicit case*/

	int edgeIndex = 0;
	CheckEdgeExist(*n, text[(*k)], &edgeIndex);
	Edge *e = edges[edgeIndex];

	int start = e->startIndex;
	//Is this a leaf?	
	int end = (*n)->isLeaf == 1 ? words[(*n)->stringIndex] : e->endIndex;

	//TODO: Fix loop.
	while((end - start) <= (p - (*k)))
	{
 		*k = (*k) + end - start + 1;
		(*n) = nodes[e->nodeIndex];
		if(*k <= p)
		{
			CheckEdgeExist(*n, text[(*k)], &edgeIndex);
			e = edges[edgeIndex];
			start = e->startIndex;
			end = (*n)->isLeaf == 1 ? words[(*n)->stringIndex] : e->endIndex;
		}
	}
}

void ConstructSTree(char* str)
{
	Node* s = nodes[root];
	int k = 0;
	int stringIndex=0;

	for (int i = 0; str[i] != '\0'; i++)
	{
		char c = str[i];
		/*The current word has ended, and a new one begins.
		Move the active point to the root node.*/
		if(c == ' ' || c == '\n')
		{
			/*Insert a terminal character*/
			c = '$';
			str[i] = '$';
			s = nodes[root];
			k = i;
		}
		currentEnd = i;

		/*Update the end index of the current word*/
		words[stringIndex] = i;
		s = UpdateTree(s, &k, currentEnd, c, stringIndex);

		/*Update the index to point to the new word in the text*/
		if(c == '$')
		{
			stringIndex++;

			if(stringIndex == wordsCount)
			{
				wordsCount *=2;
				words = (int*) realloc(words, wordsCount * sizeof(int));
			}
		}
	}
	return;
}

/*Returns the index of the newly created node*/
int SplitEdge(Node *s, int k, int p, int stringIndex)
{
	char c = text[k];
	int edgeIndex = -1;
	CheckEdgeExist(s, c, &edgeIndex);

	/*let (s, (k', p'), s') be the w[k]-edge from s*/
	Edge *e = edges[edgeIndex];
	//k'
	int start = e->startIndex;
	//p'
	int end = s->isLeaf == 1 ? words[s->stringIndex] : e->endIndex;
	int endNodeIndex = e->nodeIndex;

	/*Create an internal (transition) node*/
	Node *r = CreateNewNode(stringIndex, 0);

	/*(s, (k', k' + p - k, r)*/
	e->endIndex = start + p - k;
	//e->isLeaf = 0;
	e->nodeIndex = r->arrayIndex;

	/*(r, (k' + p - k +1, p'), s')*/
	int newStart = start + p - k + 1;
	Edge* newEdge = CreateNewEdge(newStart, endNodeIndex);

	//r->edges[(char) text[newStart]] = newEdge->arrayIndex;
	SetEdge(r, text[newStart], newEdge->arrayIndex);

	return r->arrayIndex;
}

Node *CreateNewNode(int stringIndex, int isLeaf)
{
	Node *newNode = Node_Create(freeNodeIndex, stringIndex, isLeaf);
	nodes[freeNodeIndex] = newNode;
	freeNodeIndex++;

	if(freeNodeIndex == nodesCount)
	{
		nodesCount *=2;
		Node **temp =(Node**) realloc(nodes, nodesCount * sizeof(Node));
		if(temp != NULL)
			nodes = temp;

		suffixPointers = (int*) realloc(suffixPointers, nodesCount * sizeof(int));
		from = (unsigned*) realloc(from, nodesCount * sizeof(unsigned));
		to = (unsigned*) realloc(to, nodesCount * sizeof(unsigned));
	}

	return newNode;
}

Edge *CreateNewEdge(int startIndex, int nodeIndex)
{
	Edge *newEdge = Edge_Create(freeEdgeIndex, startIndex, nodeIndex);
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

Node* UpdateTree(Node *s, int *k, int p, char c, int stringIndex)
{
	/*(s, (k, p-1)) is the canonical reference pair for the active point*/
	
	Node *oldr = NULL;
	
	while(!CheckEndPoint(s, *k, p - 1, c))
	{
		Node *r = NULL;
		/*implicit case*/
		if(*k <= (p-1))
			r = nodes[SplitEdge(s, *k, p - 1, stringIndex)];
		/*explicit case*/
		else
			r = s;

		/*Add a new leaf to the tree*/
		Node *newNode = CreateNewNode(stringIndex, 1);
		Edge *e = CreateNewEdge(p, newNode->arrayIndex);
		SetEdge(r, text[p], e->arrayIndex);

		if(oldr != NULL)
			SetSuffix(oldr->arrayIndex, r->arrayIndex);

		oldr = r;
		if(s->arrayIndex > 0)
			s = nodes[suffixPointers[s->arrayIndex]];

		Canonize(&s, k, p - 1);
	}
	
	if(oldr != NULL)
	{
		//If the suffix is not already set, do it.
		int index = oldr->arrayIndex;
		if(!(from[index] < top && to[from[index]] == index))
			SetSuffix(index, s->arrayIndex);
	}

	Canonize(&s, k, p);

	return s;
}

//int FindSubstring(char* str)
//{
//	/*Start search from the root node.*/
//	Node* n = nodes[1];
//	for (int i = 0; str[i] != '\0'; i++)
//	{
//		int index = n->baseAddress + ((int) str[i]) - asciiStartIndex;
//		if(check[index] != n->arrayIndex)
//			return 0;
//		Edge* e = edges[next[index]];
//		/*TODO: Fix for set of strings*/
//		int end = e->isLeaf == 1 ? currentEnd : e->endIndex;
//		for (int j = (e->startIndex + 1); j <= end; j++)
//		{
//			i++;
//			if(str[i] == '\0')
//				return 1;
//
//			if(str[i] != text[j])
//				return 0;
//		}
//		n = nodes[e->nodeIndex];
//	}
//	/*TODO*/
//	/*if(str[i] == '\0')
//		return 1;
//	else
//		return 0;*/
//	return 1;
//}
//
//void TestResult()
//{
//	char *tests[] = { "co", "oco", "oa","oY"};
//	for (int i = 0; i < 4; i++)
//	{
//		char* substring = tests[i];
//		if(FindSubstring(substring) == 1)
//			printf("Match!");
//		else
//			printf("There was no match!");	
//	}
//}

void PrintTree()
{
	for (int i = 1; i < freeNodeIndex; i++)
	{
		printf("Node %d:\n", i);
		Node* n = nodes[i];
		int index = n->edgeStartIndex;
		for (int j = 0; j  < n->numberofEdges; j ++)
		{
			NodeEdge* ne = nodeEdges[index];
			printf("%c ", ne->symbol);
			Edge* e = edges[ne->edgeIndex];
			int end = n->isLeaf == 1 ? words[n->stringIndex] : e->endIndex;
			printf("[%d, %d]", e->startIndex, end);
			printf(" End node: '%d'", e->nodeIndex);
			printf("\n");
			index = ne->nextElement;
		}
	}
}

void PrintCompressedTable()
{
	for (int i = 0; i < compressedTableLength; i++)
	{
		printf("check[%d] = %d", i, check[i]);
		if(check[i] != 0)
		{
			Edge* e = edges[next[i]];
			printf(" '%c' -> %d", text[e->startIndex], e->nodeIndex);
		}
		printf("\n");
	}
}

char* GetInputAsString()
{
	FILE* inputFile = fopen("./Sample.txt", "rb");
	assert(inputFile != NULL);
	long inputFileSize;
	fseek(inputFile, 0, SEEK_END);
	inputFileSize = ftell(inputFile);
	rewind(inputFile);

	/*In case the file does not end with \0*/
	char *input = (char*) malloc((inputFileSize + 1) * (sizeof(char)));
	fread(input, sizeof(char), inputFileSize, inputFile);
	fclose(inputFile);
	input[inputFileSize] = '\0';

	return input;
}

int main(int argc, char *argv[])
{
	InitTree();
	//text = "COCOA";
	//text = "ABCABXABCD";
	ConstructSTree(GetInputAsString());
	PrintTree();
	/*Optimistically assume we'll only need as many slots in the compressed table,
	as the count of edges in the tree.*/
	compressedTableLength = (freeNodeEdgeIndex - 1) > alphabetSize ? (freeNodeEdgeIndex - 1) : alphabetSize;
	check = (int*) malloc(compressedTableLength*sizeof(int));
	next =  (int*) malloc(compressedTableLength*sizeof(int));
	ConstructTable(nodeEdges, nodes, freeNodeIndex - 1, edges, &compressedTableLength, check, next);
	free(nodeEdges);
	PrintCompressedTable();
    return 0;
}