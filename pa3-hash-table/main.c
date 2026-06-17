#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256
#define SIZE_5 5000
#define SIZE_10 10000
#define SIZE_15 15000

// hash table structs and functions implemented from hash table lecture
// node struct
typedef struct node
{
    char *key;
    struct node *next;
} Node;

// hash table struct
typedef struct openHashTable
{
    int size;
    Node **table;
} OpenHashTable;

// initialize node
Node *initializeNode(char *key, Node *next)
{
    Node *newNode = malloc(sizeof(Node));
    newNode->key = strdup(key);
    newNode->next = next;
    return newNode;
}

// initialize hash table
OpenHashTable *initializeHashTable(int size)
{
    OpenHashTable *hashTable = malloc(sizeof(OpenHashTable));
    hashTable->size = size;
    hashTable->table = malloc(sizeof(Node *) * size);
    for(int i = 0; i < size; i++)
        hashTable->table[i] = NULL;
    return hashTable;
}

// HASHING STRINGS
unsigned long hash(char *word, int size)
{
    unsigned long total = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        total = total + word[i]; // adds up the ASCII values of characters in the word
        total = total * 101;
    }
    return total % size;
}

// insert function
void insert(OpenHashTable *hashTable, char *key)
{
    int position = hash(key, hashTable->size);
    Node *ptr = hashTable->table[position];
    
    while (ptr != NULL)
    {
        if(strcmp(ptr->key, key) == 0)
            return;
        else
            ptr = ptr->next;
    }
    hashTable->table[position] = initializeNode(key, hashTable->table[position]);
}

// isMember function
int isMember(OpenHashTable *hashTable, char *key)
{
    int position = hash(key, hashTable->size);
    Node *ptr = hashTable->table[position];
    
    while (ptr != NULL)
    {
        if(strcmp(ptr->key, key) == 0)
            return 1;
        else
            ptr = ptr->next;
    }
    return 0;
}

// delete function
void delete(OpenHashTable *hashTable, char *key)
{
    int position = hash(key, hashTable->size);
    Node *ptr = hashTable->table[position];
    Node **prev = &hashTable->table[position];
    
    while (ptr != NULL)
    {
        if(strcmp(ptr->key, key) == 0)
        {    
            *prev = ptr->next;
            free(ptr->key);
            free(ptr);
            return;            
        }
        else
        {
            prev = &ptr->next;
            ptr = ptr->next;
        }
    }
}

// print function for debugging
void printHashTable(OpenHashTable *hashTable)
{
    for (int i = 0; i < hashTable->size; i++)
    {
        printf("Row %d: [", i);
        Node *ptr = hashTable->table[i];
        while (ptr != NULL)
        {
            printf(" %s", ptr->key);
            ptr = ptr->next;
        }
        printf(" ]\n");
    }
}

int main(int argc, char **argv)
{
	char *dictionaryFilePath = argv[1]; //this keeps the path to the dictionary file file
	char *inputFilePath = argv[2]; //this keeps the path to the input text file
	char *check = argv[3]; // this keeps the flag to whether we should insert mistyped words into dictionary or ignore
	int numOfWords=0; //this variable will tell us how much memory to allocate

	int insertToDictionary;
	if(strcmp(check,"add")==0)
		insertToDictionary = 1;
	else
		insertToDictionary = 0;
    
	////////////////////////////////////////////////////////////////////
	//read dictionary file
    FILE *fp = fopen(dictionaryFilePath, "r");
    char *line = NULL; //variable to be used for line counting
    size_t lineBuffSize = 0; //variable to be used for line counting
    ssize_t lineSize; //variable to be used for line counting

    //check if the file is accessible, just to make sure...
    if(fp == NULL)
    {
        fprintf(stderr, "Error opening file\n");
        exit(1);
    }

    //First, let's count number of words in the dictionary.
    //This will help us know how much memory to allocate for our hash table
    while((lineSize = getline(&line,&lineBuffSize,fp)) !=-1)
        numOfWords++;
    
    //HINT: You can initialize your hash table here, since you know the size of the dictionary
    OpenHashTable *dictionaryTable = initializeHashTable(SIZE_15);

    //rewind file pointer to the beginning of the file, to be able to read it line by line.
    fseek(fp, 0, SEEK_SET);

    char wrd[BUFSIZE];
    for (int i = 0; i < numOfWords; i++)
    {
        fscanf(fp, "%s \n", wrd);
        
        //HINT: here is a good place to insert the words into your hash table
        insert(dictionaryTable, wrd);
    }
    fclose(fp);
        
	////////////////////////////////////////////////////////////////////
	//read the input text file word by word
    fp = fopen(inputFilePath, "r");
	
	//check if the file is accessible, just to make sure...
	if(fp == NULL)
	{
		fprintf(stderr, "Error opening file\n");
		return -1;
	}

    //HINT: You can use a flag to indicate if there is a misspleed word or not, which is initially set to 1
	int noTypo = 1;

	//read a line from the input file
	while((lineSize = getline(&line, &lineBuffSize,fp)) !=-1)
	{
		char *word;
        //These are the delimiters you are expected to check for. Nothing else is needed here.
		const char delimiter[]= " ,.:;!\n";

		//split the buffer by delimiters to read a single word
		word = strtok(line, delimiter); 
		
		//read the line word by word
		while(word!=NULL)
		{            
            // HINT: Since this nested while loop will keep reading the input text word by word, here is a good place to check for misspelled words
            if (!isMember(dictionaryTable, word))
            {
                noTypo = 0;
                // INPUT/OUTPUT SPECS: use the following line for printing a "word" that is misspelled.
                printf("Misspelled word: %s\n",word);
                
                // INPUT/OUTPUT SPECS: use the following line for printing suggestions, each of which will be separated by a comma and whitespace.
                printf("Suggestions: "); //the suggested words should follow

                int suggestions = 0;

                // inverted adjacent letters
                for (int i = 0; i < strlen(word) - 1 && suggestions < 3; i++)
                {
                    char temp = word[i];
                    word[i] = word[i + 1];
                    word[i + 1] = temp;
                    if (isMember(dictionaryTable, word))
                    {
                        printf("%s ", word);
                        suggestions++;
                    }
                    temp = word[i];
                    word[i] = word[i + 1];
                    word[i + 1] = temp;
                }

                // missing letter at the beginning or end
                for (int i = 0; i <= strlen(word) && suggestions < 3; i++)
                {
                    char temp[strlen(word)];
                    strncpy(temp, word, i);
                    temp[i] = '\0';
                    strcat(temp, word + i + 1);
                    if (isMember(dictionaryTable, temp))
                    {
                        printf("%s ", temp);
                        suggestions++;
                    }
                }

                // extra letter at the beginning or end
                for (int i = 0; i <= strlen(word) && suggestions < 3; i++)
                {
                    for (char t = 'a'; t <= 'z'; t++)
                    {
                        char temp[strlen(word) + 2];
                        strncpy(temp, word, i);
                        temp[i] = t;
                        temp[i + 1] = '\0';
                        strcat(temp, word + i);
                        if (isMember(dictionaryTable, temp))
                        {
                            printf("%s ", temp);
                            suggestions++;
                        }
                    }
                }
                printf("\n");

                // add to dictionary if specified by user
                if (insertToDictionary)
                {
                    insert(dictionaryTable, word);
                }
            }                    
			word = strtok(NULL,delimiter); 
		}
	}
	fclose(fp);
    
    //HINT: If the flag noTypo is not altered (which you should do in the loop above if there exists a word not in the dictionary), then you should print "No typo!"
    if(noTypo==1)
        printf("No typo!\n");
    

    // DON'T FORGET to free the memory that you allocated
    for (int i = 0; i < dictionaryTable->size; i++)
    {
        Node *ptr = dictionaryTable->table[i];
        while (ptr != NULL)
        {
            Node *temp = ptr;
            ptr = ptr->next;
            free(temp->key);
            free(temp);
        }
    }
    free(line);
    free(dictionaryTable->table);
    free(dictionaryTable);

	return 0;
}
