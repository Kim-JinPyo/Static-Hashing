#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TableLength 33

struct Table
{
	char key[15];
	int HFvalue; //Value obtained by HashFunction
	struct Table *next;
};

int HashFunction(int);
int ChangeToHash(char[]);
void Table_Init(struct Table[], char[][15]);
int Table_Search(struct Table[], char[]);
void Table_Insert(struct Table[], char[]);
void Table_Delete(struct Table[], char[]);
void Table_Print(struct Table[]);

void main()
{
	/*Read the words from text files*/
	FILE *fp = fopen("keyinput.txt", "r");
	char keys[25][15] = { NULL };
	char buffer[15] = { NULL }; //Variable that temporarily stores the word read from a text file
	int i = 0, j = 0;
	while (fgetc(fp) != EOF)
	{
		fseek(fp, -1, SEEK_CUR);
		fgets(buffer, sizeof(buffer), fp);
		for (j = 0; buffer[j] != '\n'; j++); //Erasing '\n' in a file
		buffer[j] = NULL;
		strcpy(keys[i], buffer);
		i++;
	}
	fclose(fp);

	struct Table ht[33] = { NULL };
	Table_Init(ht, keys);
	Table_Print(ht);

	char test_Blue[10] = { "Blue" };
	char test_black[10] = { "black" };
	char test_Purple[10] = { "Purple" };
	char test_Green[10] = { "Green" };
	char test_White[10] = { "White" };
	char test_Golden[10] = { "Golden" };
	char test_nedloG[10] = { "nedloG" };
	char test_Yellow[10] = { "Yellow" };

	if (Table_Search(ht, test_Blue))
		printf("found\n");
	else
		printf("not found\n");

	if (Table_Search(ht, test_black))
		printf("found\n");
	else
		printf("not found\n");
	
	if (Table_Search(ht, test_Purple))
		printf("found\n");
	else
		printf("not found\n");

	Table_Delete(ht, test_Purple);
	Table_Delete(ht, test_Blue);
	Table_Delete(ht, test_Green);
	Table_Print(ht);

	Table_Insert(ht, test_Green);
	Table_Insert(ht, test_White);
	Table_Insert(ht, test_Golden);
	Table_Insert(ht, test_nedloG);
	Table_Print(ht);

	if (Table_Search(ht, test_Blue))
		printf("found\n");
	else
		printf("not found\n");

	if (Table_Search(ht, test_nedloG))
		printf("found\n");
	else
		printf("not found\n");

	if (Table_Search(ht, test_Yellow))
		printf("found\n");
	else
		printf("not found\n");

	if (Table_Search(ht, test_Green))
		printf("found\n");
	else
		printf("not found\n");

}

int HashFunction(int value)
{
	return value % 33;
}

/*Returns the hash value from the hash function 
  by adding the ASCII code value of each alphabet of the word*/
int ChangeToHash(char arr[])
{
	int sum = 0;
	
	for (int i = 0; i < sizeof(arr); i++)
		sum += arr[i];

	int hash = HashFunction(sum);
	return hash;
}

/*Initializing Table*/
void Table_Init(struct Table table[], char words[][15])
{
	for (int i = 0; words[i][0] != NULL; i++)
		Table_Insert(table, words[i]);
}

/*Search a specific word
   After the word is input, the hash value is obtained, and the corresponding index is examined.*/
int Table_Search(struct Table table[], char arr[])
{
	int hash = ChangeToHash(arr);

	struct Table *t = &table[hash]; //The structure pointer variable points to the address of the index and checks the overflowed value
	while (t != NULL)
	{
		if (strcmp(t->key, arr) == 0)
			return 1;

		t = t->next;
	}

	return 0;
}

//Insert the word into the table
void Table_Insert(struct Table table[], char arr[])
{
	if (Table_Search(table, arr) == 1) //the word already exists in the table
	{
		printf("exists\n");
		return;
	}

	int hash = ChangeToHash(arr);
	if (table[hash].key[0] == NULL) //When overflow does not occur
	{
		strcpy(table[hash].key, arr);
		table[hash].HFvalue = hash;
		table[hash].next = NULL;
	}

	else //Chaining
	{
		//Storing information into the created node
		struct Table *newTable = (struct Table*)malloc(sizeof(struct Table));
		strcpy(newTable->key, arr);
		newTable->HFvalue = ChangeToHash(arr);
		newTable->next = NULL;

		struct Table *t = &table[hash];
		while (t->next != NULL)
			t = t->next;

		t->next = newTable;
	}

	printf("inserted\n");
}

//Deleting a specific word
void Table_Delete(struct Table table[], char arr[])
{
	if (Table_Search(table, arr) == 0) //No word in the table
	{
		printf("not found\n");
		return;
	}

	int hash = ChangeToHash(arr);
	struct Table *t = &table[hash]; //Examine the corresponding index

	while (t != NULL)
	{
		if (t->next == NULL) //When overflow does not occur and one value is stored
		{
			table[hash] = { NULL };
			break;
		}
		else
		{
			if (strcmp(t->next->key, arr) == 0) //Chained index
			{
				struct Table *del = t->next;
				t->next = del->next;
				free(del);
				break;
			}

			else
				t = t->next;
		}
	}
	printf("deleted\n");
}

//Print the table
void Table_Print(struct Table table[])
{
	struct Table *t;

	for (int i = 0; i < TableLength; i++)
	{
		t = &table[i];
		printf("%d ", i + 1);
		if (table[i].key[0] != NULL)
		{
			while (t != NULL) //Print all chained words
			{
				printf("%s->", t->key);
				t = t->next;
			}
		}
		printf("NULL\n");
	}
}