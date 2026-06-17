// PROGRAMMING ASSIGNMENT I STARTER CODE
/*In order to complete this assignment, you need to complete the function defintions in the section commented FUNCTION DEFINITIONS.
When you implement a function, make sure that you uncomment its provided function call from main so that your program can utilize it.*/

// HEADER FILES
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// CONSTANT DECLARATIONS
#define INITIAL_CAPACITY 2

// STRUCT DECLARATIONS
typedef struct entry
{
	char* name;
	char* lastname;
	float height;
	int age;
} Entry;

typedef struct list 
{
	int capacity;
	int size;
	Entry** data;
} List;

// FUNCTION DEFINTIONS
/* The function declarations have been given to you which should make the parameters and the return value for each function obvious.
The initializeEntry, freeEntry, printList, and printEntry function have been coded to start you off.*/

// Returns a pointer to an Entry struct initialized with the specified values for each of the 4 members of the struct (NO NEED TO CHANGE).
Entry* initializeEntry(char* name, char* lastname, float height, int age)
{
	Entry* newEntry = malloc(sizeof(Entry));
	
	newEntry->name = malloc(sizeof(char)*(strlen(name)+1));
	strcpy(newEntry->name, name);
	
	newEntry->lastname = malloc(sizeof(char)*(strlen(lastname)+1));
	strcpy(newEntry->lastname, lastname);
	
	newEntry->height = height;
	
	newEntry->age = age;
	
	return newEntry;
}

// Given a pointer to an Entry struct, this function frees the memory allocated for that Entry struct (NO NEED TO CHANGE).
void freeEntry(Entry* entry)
{
	if (entry != NULL)
	{
		free(entry->name);
		free(entry->lastname);
		free(entry);
	}
}

List* initializeList()
{
	List* newList = malloc(sizeof(List));

	newList->capacity = INITIAL_CAPACITY;
	newList->size = 0;
	
	// allocate memort for array of entries
	newList->data = malloc(sizeof(Entry*) * INITIAL_CAPACITY);

	return newList;
}

void deleteList(List* myList)
{
	// delete each entry
	for (int i = 0; i < myList->size; i++) {
		freeEntry(myList->data[i]);
	}

	// delete array for entrys
	free(myList->data);

	// delete list
	free(myList);
}

void doubleCapacity(List* myList)
{
	// double capacity
	myList->capacity *= 2;

	// reallocate for new capacity
	myList->data = realloc(myList->data, sizeof(Entry*) * myList->capacity);
}

void halveCapacity(List* myList)
{
	// halve capacity
	myList->capacity /= 2;

	// reallocate for new capacity
	myList->data = realloc(myList->data, sizeof(Entry*) * myList->capacity);
}

void insertToHead(List* myList, char* name, char* lastname, float height, int age)
{
	// resize if needed
	if (myList->size == myList->capacity){
		doubleCapacity(myList);
	}

	// shift entries to the right
	for (int i = myList->size - 1; i >= 0; i--) {
		myList->data[i + 1] = myList->data[i];
	}

	// allocate memory for Entry and insert at front
	Entry* newEntry = initializeEntry(name, lastname, height, age);
	myList->data[0] = newEntry;

	// increment list size
	myList->size++;
}

void insertToTail(List* myList, char* name, char* lastname, float height, int age)
{
	// resize if needed
	if (myList->size == myList->capacity){
		doubleCapacity(myList);
	}

	// allocate memory for Entry and insert at end
	Entry* newEntry = initializeEntry(name, lastname, height, age);
	myList->data[myList->size] = newEntry;

	// increment list size
	myList->size++;
}

void insertToPosition(List* myList, int position, char* name, char* lastname, float height, int age)
{
	// check for valid postion
	if (position < 0 || position > myList->size) {
		printf("Invalid position\n");
		return;
	}

	// resize if needed
	if (myList->size == myList->capacity){
		doubleCapacity(myList);
	}

	// shift entries to the right
	for (int i = myList->size - 1; i >= position; i--) {
		myList->data[i + 1] = myList->data[i];
	}

	// allocate memory for Entry and insert at front
	Entry* newEntry = initializeEntry(name, lastname, height, age);
	myList->data[position] = newEntry;

	// increment list size
	myList->size++;
}

int findPosition(List* myList, char* name)
{
	// iterate through list to find name
	for (int i = 0; i < myList->size; i++) {
		if (strcmp(myList->data[i]->name, name) == 0) {
			return i;
		}
	}
	return -1;
}

void deleteFromHead(List* myList)
{
	// check if list is empty
	if (myList->size == 0) {
		printf("List is empty. Cannot delete entry.\n");
		return;
	}

	// delete entry
	freeEntry(myList->data[0]);

	// shift entries to left
	for (int i = 0; i < myList->size - 1; i++) {
		myList->data[i] = myList->data[i + 1];
	}

	// decrement list size
	myList->size--;

	// resize if needed
	if (myList->size == myList->capacity / 2) {
		halveCapacity(myList);
	}
}

void deleteFromTail(List* myList)
{
	// check if list is empty
	if (myList->size == 0) {
		printf("List is empty. Cannot delete entry.\n");
		return;
	}

	// delete entry
	freeEntry(myList->data[myList->size - 1]);

	// decrement list size
	myList->size--;

	// resize if needed
	if (myList->size == myList->capacity / 2) {
		halveCapacity(myList);
	}
}

void deleteFromPosition(List* myList, int position)
{
	// check if list is empty
	if (myList->size == 0) {
		printf("List is empty. Cannot delete entry.\n");
		return;
	}

	// check for valid postion
	if (position < 0 || position > myList->size) {
		printf("Invalid position\n");
		return;
	}

	// delete entry
	freeEntry(myList->data[position]);

	// shift entries to left
	for (int i = position; i < myList->size - 1; i++) {
		myList->data[i] = myList->data[i + 1];
	}

	// decrement list size
	myList->size--;

	// resize if needed
	if (myList->size == myList->capacity / 2) {
		halveCapacity(myList);
	}
}

// Given a pointer to a List struct, this function prints each Entry in that list (NO NEED TO CHANGE).
void printList(List* myList)
{
	if (myList->size == 0)
	{
		printf("List is empty!\n");
	}
	else
	{
		for (int i = 0; i < myList->size; i++)
		{
			printf("[%d]\t%s\t%s\t%0.2f\t%d\n", i, myList->data[i]->name, myList->data[i]->lastname, myList->data[i]->height, myList->data[i]->age);
		}
	}
}

// Given a pointer to a List struct, this function prints out the size and capacity of that List (NO NEED TO CHANGE).
void printListInfo(List* myList)
{
	printf("size:%d, capacity:%d\n", myList->size, myList->capacity);
}

int main(int argc, char** argv) 
{
	FILE* fp = fopen(argv[1], "r");
	char* line = NULL;
	size_t lineBuffSize = 0;
	ssize_t lineSize;
	
	if (fp == NULL)
	{
		fprintf(stderr, "Error opening file\n");
		return -1;
	}
	
	List* myList;
	// Uncomment the following function call when you implement the initializeList() function
	myList = initializeList(); 
	
	while ((lineSize = getline(&line, &lineBuffSize, fp)) != -1)
	{
		char* token;
		const char delimiter[2] = " ";
		
		if (line[strlen(line) - 1] == '\n')
		{
			line[strlen(line) - 1] = '\0';
		}

		token = strtok(line, delimiter);
					
		if (strcmp(token, "insertToHead") == 0)
		{
			char* name;
			char* lastname;
			float height;
			int age;
			
			name = strtok(NULL, delimiter);
			lastname = strtok(NULL, delimiter);
			height = atof(strtok(NULL, delimiter));
			age = atoi(strtok(NULL, delimiter));
			// Uncomment the following insertToHead function call when you have implemented it
			insertToHead(myList, name, lastname, height, age);
		}
		else if (strcmp(token, "insertToTail") == 0)
		{
			char* name;
			char* lastname;
			float height;
			int age;
			
			name = strtok(NULL, delimiter);
			lastname = strtok(NULL, delimiter);
			height = atof(strtok(NULL, delimiter));
			age = atoi(strtok(NULL, delimiter));
			// Uncomment the following insertToTail function call when you have implemented it
			insertToTail(myList, name, lastname, height, age);
		}
		else if (strcmp(token, "insertToPosition") == 0)
		{
			char* name;
			char* lastname;
			float height;
			int age;
			int position;
			
			position = atoi(strtok(NULL, delimiter));
			name = strtok(NULL, delimiter);
			lastname = strtok(NULL, delimiter);
			height = atof(strtok(NULL, delimiter));
			age = atoi(strtok(NULL, delimiter));
			// Uncomment the following insertToPosition function call when you have implemented it
			insertToPosition(myList, position, name, lastname, height, age);
		}
		else if (strcmp(token, "findPosition") == 0)
		{
			char* name;
			
			name = strtok(NULL, delimiter);
			// Uncomment the following printf statement using the return value from a findPosition function call when you have implemented the function
			printf("%d\n", findPosition(myList, name));
		}
		else if (strcmp(token, "deleteFromHead") == 0)
		{
			// Uncomment the following deleteFromHead function call when you have implemented it
			deleteFromHead(myList);
		}
		else if (strcmp(token, "deleteFromTail") == 0)
		{
			// Uncomment the following insertToHead function call when you have implemented it
			deleteFromTail(myList);
		}
		else if (strcmp(token, "deleteFromPosition") == 0)
		{
			int position;
			
			position = atoi(strtok(NULL, delimiter));
			
			// Uncomment the following deleteFromPosition function call when you have implemented it
			deleteFromPosition(myList, position);
		}
		else if (strcmp(token, "printList") == 0)
		{
			printList(myList);
		}
		else if(strcmp(token, "printListInfo") == 0)
		{
			printListInfo(myList);
		}
		else if (strcmp(token, "deleteList") == 0)
		{
			// Uncomment the following deleteList function call when you have implemented it
			deleteList(myList);
		}
		else
		{
			printf("Invalid command: <%s>\n", token);
		}
	}

	//getline internally allocates memory, so we need to free it here so as not to leak memory!!
	free(line);
	
	//we should close the file that we have been reading from before exiting!
	fclose(fp);

	return 0;
} 
