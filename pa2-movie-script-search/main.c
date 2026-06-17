#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256
#define MIN 10

typedef struct item{
  char *word;
  int weight;
}Item;

// define  functions for sorting. implemented with pseudocode from quicksort lecture
// swap function
void swap(Item *A, int i, int j)
{
    Item temp = A[i];
    A[i] = A[j];
    A[j] = temp;
}

// partiton function
int partition(Item *A, int start, int stop)
{
    Item pivot = A[stop];
    int i = start;
    for (int j = start; j < stop; j++)
    {
        char* word1 = A[j].word;
        char* word2 = pivot.word;
        while (*word1 == '\'' && *word2 == '\'')
        {
            word1++;
            word2++;
            if (*word1 == '\0' || *word2 == '\0')
                break;
        }
        if (strcmp(word1, word2) <= 0)
        {
            swap(A, i, j);
            i++;
        }
    }
    swap(A, i, stop);
    return i;
}

// Quick Sort
void myQuickSort(Item *A, int start, int stop, int size)
{
    if (start < stop)
    {
        int p = partition(A, start, stop);
        //printf("start: %d, p: = %d, stop: %d\n", start, p, stop);
        if (p > start)
            myQuickSort(A, start, p - 1, size);
        if (p < stop)
            myQuickSort(A, p + 1, stop, size);
    }
}

// function to sort matches
void matchSort(Item *matches, int size)
{
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (matches[j].weight < matches[j + 1].weight) {
                Item temp = matches[j];
                matches[j] = matches[j + 1];
                matches[j + 1] = temp;
            }
        }
    }
}

// define  function for searching. implemented with pseudocode from binary search lecture
// search for query words in dictionary list
Item* myBinarySearch(char* query, Item* dictionaryList, int wordCount)
{
    int start = 0;
    int stop = wordCount - 1;
    Item *result = NULL;
    while (start <= stop)
    {
        int middle = start + ((stop - start) / 2);
        int match = strncmp(dictionaryList[middle].word, query, strlen(query));
        if (match >= 0)
        {
            if (strncmp(dictionaryList[middle].word, query, strlen(query)) == 0)
            {
                result = &dictionaryList[middle];
                break;
            }
            else
            {
                stop = middle - 1;
            }
        }
        else
        {
            start = middle + 1;
        }
    }
    return result;
}

int main(int argc, char **argv) {
    char *dictionaryFilePath = argv[1]; //this keeps the path to dictionary file
    char *queryFilePath = argv[2]; //this keeps the path to the file that keeps a list of query wrods, 1 query per line
    int wordCount = 0; //this variable will keep a count of words in the dictionary, telling us how much memory to allocate
    int queryCount = 0; //this variable will keep a count of queries in the query file, telling us how much memory to allocate for the query words
    
    ////////////////////////////////////////////////////////////////////////
    ///////////////////////// read dictionary file /////////////////////////
    ////////////////////////////////////////////////////////////////////////
    FILE *fp = fopen(dictionaryFilePath, "r");
    char *line = NULL; //variable to be used for line counting
    size_t lineBuffSize = 0; //variable to be used for line counting
    ssize_t lineSize; //variable to be used for line counting
    
    //check if the file is accessible, just to make sure...
    if(fp == NULL){
        fprintf(stderr, "Error opening file:%s\n", dictionaryFilePath);
        return -1;
    }

    //First, let's count number of lines. This will help us know how much memory to allocate
    while((lineSize = getline(&line, &lineBuffSize, fp)) !=-1)
    {
        wordCount++;
    }    
    
    // allocate memory for storing dictionary data
    Item *dictionaryList = (Item *)malloc(wordCount * sizeof(Item));
    if (dictionaryList == NULL)
    {
        fprintf(stderr, "Memort allocation failed\n");
        return -1;
    }
    
    //Read the file once more, this time to fill in the data into memory
    fseek(fp, 0, SEEK_SET);// rewind to the beginning of the file, before reading it line by line.
    char word[BUFSIZE]; //to be used for reading lines in the loop below
    int weight;
    for(int i = 0; i < wordCount; i++)
    {
        fscanf(fp, "%s %d\n", word, &weight);

        // allocate memory for word
        dictionaryList[i].word = (char *)malloc((strlen(word) + 1) * sizeof(char));
        if (dictionaryList[i].word == NULL)
        {
            fprintf(stderr, "Memort allocation failed\n");
            return -1;
        }

        // copy dictionary word to list
        strcpy(dictionaryList[i].word, word);
        dictionaryList[i].word[strlen(word)] = '\0'; // manually add null terminator

        dictionaryList[i].weight = weight;
    }
    //close the input file
    fclose(fp);

    ////////////////////////////////////////////////////////////////////////
    ///////////////////////// read query list file /////////////////////////
    ////////////////////////////////////////////////////////////////////////
    fp = fopen(queryFilePath, "r");
        
    //check if the file is accessible, just to make sure...
    if(fp == NULL) {
        fprintf(stderr, "Error opening file:%s\n", queryFilePath);
        return -1;
    }

    //First, let's count number of queries. This will help us know how much memory to allocate
    while((lineSize = getline(&line,&lineBuffSize,fp)) !=-1)
    {
        queryCount++;
    }
    free(line); //getline internally allocates memory, so we need to free it here so as not to leak memory!!

    // allocate memory for storing query data
    char **queryList = (char **)malloc(queryCount * sizeof(char *));
    if (queryList == NULL)
    {
        fprintf(stderr, "Memort allocation failed\n");
        return -1;
    }

    fseek(fp, 0, SEEK_SET);// rewind to the beginning of the file, before reading it line by line.
    for(int i = 0; i < queryCount; i++)
    {
        fscanf(fp, "%s\n", word);
        
        // allocate memory for word
        queryList[i] = (char *)malloc((strlen(word) + 1) * sizeof(char));
        if (queryList[i] == NULL)
        {
            fprintf(stderr, "Memort allocation failed\n");
            return -1;
        }

        // copy query word to list
        strcpy(queryList[i], word);
        //queryList[i].word[strlen(word)] = '\0'; // manually add null terminator  
    }
    //close the input file
    fclose(fp);

    ////////////////////////////////////////////////////////////////////////
    ///////////////////////// reading input is done ////////////////////////
    ////////////////////////////////////////////////////////////////////////

    // sort dictionary in alphabetical order
    myQuickSort(dictionaryList, 0, wordCount - 1, wordCount);

    // loop to search query words in dictionary list
    for (int i = 0; i < queryCount; i++)
    {
        printf("Query word:%s\n", queryList[i]);
        Item* matches = myBinarySearch(queryList[i], dictionaryList, wordCount);

        // find top 10 suggestions
        if (matches != NULL)
        {
            Item topTen[MIN];
            int counter = 0;
            for (int j = 0; j < wordCount; j++)
            {
                if (strncmp(dictionaryList[j].word, queryList[i], strlen(queryList[i])) == 0)
                {
                    topTen[counter] = dictionaryList[j];                    
                    counter++;
                    if (counter >= MIN) break;
                }
            }
            // sort suggestions by weight
            matchSort(topTen, counter);

            // print suggestions
            for (int k = 0; k < counter; k++) {
                printf("%s %d\n", topTen[k].word, topTen[k].weight);
            }
        } else {
            printf("No Suggestion!\n");
        }
    }
    
    // free allocated memory
    for (int i = 0; i < wordCount; i++)
    {
        free(dictionaryList[i].word);
    }
    free(dictionaryList);

    for (int i = 0; i < queryCount; i++)
    {
        free(queryList[i]);
    }
    free(queryList);
    
    return 0;
}
