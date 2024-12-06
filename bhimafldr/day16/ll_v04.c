#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
	int val;
	struct node *ptr; //Self referencial pointer (SELF/SRP)
}NODE;

void printList(NODE *);
NODE* appendNode(NODE *,NODE *);

int main()
{
	//creating nodes & allocated mem
	NODE h1,h2,h3,h4;

	//ptr1 is used to navigate/travese betwn nodes
	NODE *ptr1=NULL;

	//initialze values to each node

	h1.val = 10;
	h1.ptr = NULL;

	h2.val = 20;
	h2.ptr = NULL;

	h3.val = 30;
	h3.ptr = NULL;

	//Making relationship (travese)/creating LList

	h1.ptr = &h2;
	h2.ptr = &h3;

	
	//traversing the list
	ptr1 = &h1; // ptr1 is pointing to BA of list
	printList(ptr1); //Send the BA of list to function

	h4.val = 40;
	h4.ptr = NULL;

	//h3.ptr = &h4;
	ptr1 = &h1;
	appendNode(ptr1,&h4);

	//traversing the list
	ptr1 = &h1; // ptr1 is pointing to BA of list
	printList(ptr1); //Send the BA of list to function


	printf("\n\n");

	return 0;

}

void printList(NODE *head)
{
	printf("\nlist using loops and function\n");
	while(head!= NULL)
	{
		printf("%d->",head->val);
		head = head->ptr;	
	}
	printf("NULL");
}

NODE *appendNode(NODE *head,NODE *nn)
{
	printf("\nIn appendNode\n");
	NODE *temp = head;

	if(head == NULL)
	{
		//the list is empty
		head = nn;
		temp = nn;
	}
	else
	{
		while(head->ptr!= NULL)
		{
			printf("%d->",head->val);
			head = head->ptr;	
		}
		
		head->ptr = nn; //adding node to the end of the list
	}
	head = temp; //head is again pointed to the BA

	return head;
}