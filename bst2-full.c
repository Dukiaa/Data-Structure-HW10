#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int key;
	struct node *left;
	struct node *right;
} Node;

/* for stack */
#define MAX_STACK_SIZE		20
Node* stack[MAX_STACK_SIZE];
int top = -1;

Node* pop();
void push(Node* aNode);

/* for queue */
#define MAX_QUEUE_SIZE		20
Node* queue[MAX_QUEUE_SIZE];
int front = -1;
int rear = -1;

Node* deQueue();
void enQueue(Node* aNode);


int initializeBST(Node** h);

/* functions that you have to implement */
void recursiveInorder(Node* ptr);	  /* recursive inorder traversal */
void iterativeInorder(Node* ptr);     /* iterative inorder traversal */
void levelOrder(Node* ptr);	          /* level order traversal */
int insert(Node* head, int key);      //트리에 노드 삽입
int deleteNode(Node* head, int key);  //트리에 노드 삭제
int freeBST(Node* head); //메모리에 할당되어있는 공간 free

void printStack();



int main()
{
	char command;
	int key;
	Node* head = NULL;
	printf("[----- [조정동] [2017015041] -----]\n");
	do{
		printf("\n\n");
		printf("----------------------------------------------------------------\n");
		printf("                   Binary Search Tree #2                        \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize BST       = z                                       \n");
		printf(" Insert Node          = i      Delete Node                  = d \n");
		printf(" Recursive Inorder    = r      Iterative Inorder (Stack)    = t \n");
		printf(" Level Order (Queue)  = l      Quit                         = q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			initializeBST(&head); //트리를 초기화하는 함수 
			break;
		case 'q': case 'Q':
			freeBST(head); //free
			break;
		case 'i': case 'I':
			printf("Your Key = "); //트리에 key값 삽입
			scanf("%d", &key);
			insert(head, key);
			break;
		case 'd': case 'D':
			printf("Your Key = "); //트리에 key값 삭제
			scanf("%d", &key);
			deleteNode(head, key);
			break;

		case 'r': case 'R':
			recursiveInorder(head->left); //recursive방식으로 트리에서 head->left로 find
			break;
		case 't': case 'T':
			iterativeInorder(head->left); //iterative방식으로 트리에서 head->left로 find
			break;

		case 'l': case 'L':
			levelOrder(head->left); //level방식으로 트리에서 head->left로 find
			break;

		case 'p': case 'P':
			printStack();
			break;

		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

int initializeBST(Node** h) {

    //트리가 비어있지 않으면 할당된 모든 노드 제거
	if(*h != NULL)
		freeBST(*h);

    //head노드 생성
	*h = (Node*)malloc(sizeof(Node));
	(*h)->left = NULL;	/* root */
	(*h)->right = *h;
	(*h)->key = -9999;

	top = -1;

	front = rear = -1;

	return 1;
}



void recursiveInorder(Node* ptr)
{
	if(ptr) {
		recursiveInorder(ptr->left);
		printf(" [%d] ", ptr->key);
		recursiveInorder(ptr->right);
	}
}

/**
 * textbook: p 224
 */
void iterativeInorder(Node* node)
{
	for(;;)
	{
		for(; node; node = node->left)
			push(node);
		node = pop();

		if(!node) break;
		printf(" [%d] ", node->key);

		node = node->right;
	}
}

/**
 * textbook: p 225
 */
void levelOrder(Node* ptr)
{
	// int front = rear = -1;

	if(!ptr) return; /* empty tree */

	enQueue(ptr);

	for(;;)
	{
		ptr = deQueue();
		if(ptr) {
			printf(" [%d] ", ptr->key);

			if(ptr->left)
				enQueue(ptr->left);
			if(ptr->right)
				enQueue(ptr->right);
		}
		else
			break;

	}

}


int insert(Node* head, int key)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->key = key;
	newNode->left = NULL;
	newNode->right = NULL;

	if (head->left == NULL) {
		head->left = newNode;
		return 1;
	}

	/* head->left is the root */
	Node* ptr = head->left;

	Node* parentNode = NULL;
	while(ptr != NULL) {

		//키에 대한 노드가 있으면 그냥 반환
		if(ptr->key == key) return 1;

		//parentNode를 사용하여 부모를 계속 추적
		parentNode = ptr;

		//키 비교, 현재 노드의 키가 입력 키보다 큰 경우, 새 노드가 오른쪽 하위 트리에 삽입
		//그렇지 않으면 왼쪽 하위 트리
		if(ptr->key < key)
			ptr = ptr->right;
		else
			ptr = ptr->left;
	}

	//새 노드를 부모 노드에 연결
	if(parentNode->key > key)
		parentNode->left = newNode;
	else
		parentNode->right = newNode;
	return 1;
}


int deleteNode(Node* head, int key)
{
	if (head == NULL) {
		printf("\n Nothing to delete!!\n");
		return -1;
	}

	if (head->left == NULL) {
		printf("\n Nothing to delete!!\n");
		return -1;
	}

	/* head->left is the root */
	Node* root = head->left;



	Node* parent = NULL;
	Node* ptr = root;

	while((ptr != NULL)&&(ptr->key != key)) {
		if(ptr->key != key) {

			parent = ptr;	// parent에 저장

			if(ptr->key > key)
				ptr = ptr->left;
			else
				ptr = ptr->right;
		}
	}

	//key에 대한 노드 x
	if(ptr == NULL)
	{
		printf("No node for key [%d]\n ", key);
		return -1;
	}

	//case 1:제거해야 하는 노드는 리프 노드
	if(ptr->left == NULL && ptr->right == NULL)
	{
		if(parent != NULL) { //부모 존재, 부모의 왼쪽 및 오른쪽 링크가 조정
			if(parent->left == ptr)
				parent->left = NULL;
			else
				parent->right = NULL;
		} else {
			//부모는 null이며 삭제할 노드가 루트임을 의미
			head->left = NULL;

		}

		free(ptr);
		return 1;
	}

	// case 2: 삭제할 노드에 자식이 하나 있는 경우
	
	if ((ptr->left == NULL || ptr->right == NULL))
	{
		Node* child;
		if (ptr->left != NULL)
			child = ptr->left;
		else
			child = ptr->right;

		if(parent != NULL)
		{
			if(parent->left == ptr)
				parent->left = child;
			else
				parent->right = child;
		} else {
			//부모는 null이며 삭제할 노드가 루트임을 의미
			root = child;
		}

		free(ptr);
		return 1;
	}

	/**
	 * case 3: the node (ptr)는 2의 자식 노드를 들고있음
	 *
	 * ptr의 왼쪽 하위 트리에서 가장 큰 자손 노드를 찾아야한다. 또는 ptr의 오른쪽 하위 트리에서 가장 작은 자손을 찾아야한다.
	 * or the smallest descendant in the right subtree of the ptr.
	 *
	 * ptr의 오른쪽 하위 트리에서 가장 작은 자손을 찾
	 *
	 */

	Node* candidate;
	parent = ptr;


	candidate = ptr->right;

	//가장 작은 노드는 ptr의 오른쪽 하위 트리에서 가장 왼쪽에 있는 노드
	while(candidate->left != NULL)
	{
		parent = candidate;
		candidate = candidate->left;
	}

	/* 후보 노드는 삭제해야 하는 오른쪽 노드
	 * note that candidate's left is null
	 */
	if (parent->right == candidate)
		parent->right = candidate->right;
	else
		parent->left = candidate->right;

	/* ptr을 제거하는 대신 ptr의 키만 변경
	 * with the key of candidate node and remove the candidate node
	 */

	ptr->key = candidate->key;

	free(candidate);
	return 1;
}


void freeNode(Node* ptr)
{
	if(ptr) {
		freeNode(ptr->left);
		freeNode(ptr->right);
		free(ptr);
	}
}

int freeBST(Node* head)
{

	if(head->left == head)
	{
		free(head);
		return 1;
	}

	Node* p = head->left;

	freeNode(p);

	free(head);
	return 1;
}



Node* pop()
{
	if (top < 0) return NULL;
	return stack[top--];
}

void push(Node* aNode)
{
	stack[++top] = aNode;
}


void printStack()
{
	int i = 0;
	printf("--- stack ---\n");
	while(i <= top)
	{
		printf("stack[%d] = %d\n", i, stack[i]->key);
	}
}


Node* deQueue()
{
	if (front == rear) {
		// printf("\n....Now Queue is empty!!\n" );
		return NULL;
	}

	front = (front + 1) % MAX_QUEUE_SIZE;
	return queue[front];

}

void enQueue(Node* aNode)
{
	rear = (rear + 1) % MAX_QUEUE_SIZE;
	if (front == rear) {
		// printf("\n....Now Queue is full!!\n");
		return;
	}

	queue[rear] = aNode;
}