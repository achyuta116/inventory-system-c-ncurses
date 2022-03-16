#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "header.h"
//Table headers
char* header[] = {"ID", "Tag", "Item Name", "Cost Price", "List Price", "Quantity", "Company"};
//Initialise array of structures from CSV file
void initialisation(char *fileName){
	FILE *file = fopen(fileName,"r");
	char ch;
	int linesCount = 0;
	while((ch=fgetc(file)) != EOF) {
		if(ch=='\n')
		linesCount++;
	} //Counts total number of lines in the CSV file
	if(linesCount != 0 && linesCount != 1){
		rewind(file);
		item* data = (item*)calloc(sizeof(item),linesCount-1);
		char str[1000];
		fgets(str,1000,file); //Header
		char *token;
		//Parses through the data and adds it to the array
		for(int i = 0;i < linesCount-1; i++){
			fgets(str,1000,file);
			token = strtok(str,",");
			data[i].ID = atoi(token);
			token = strtok(NULL,",");
			strcpy(data[i].tag,token);
			token = strtok(NULL,",");
			strcpy(data[i].itemName,token);
			token = strtok(NULL,",");
			data[i].costPrice = atof(token);
			token = strtok(NULL,",");
			data[i].listPrice = atof(token);
			token = strtok(NULL,",");
			data[i].quantity = atoi(token);
			token = strtok(NULL,"\n");
			strcpy(data[i].company,token);
		}
		fclose(file);
		linesCount--;
		item* newData = displayMainMenu(data,&linesCount);

		//Rewrite modified data to the CSV file
		FILE *fp = fopen(fileName,"w");
		//Writing the headers
		fprintf(fp, "%s,%s,%s,%s,%s,%s,%s\n",
			header[0],
			header[1],
			header[2],
			header[3],
			header[4],
			header[5],
			header[6]
			);
		//Writing the items
		for(int i = 0;i < linesCount; i++){
			fprintf(fp, "%d,%s,%s,%.2f,%.2f,%d,%s\n", 
				newData[i].ID,
				newData[i].tag,
				newData[i].itemName,
				newData[i].costPrice,
				newData[i].listPrice,
				newData[i].quantity,
				newData[i].company
				);
		}
		fclose(fp);

	} else {
		printw("File does not contain any data");
		refresh();
		getch();
	}
}
//Recursive function to display the main menu
item* displayMainMenu(item *data, int *size){

	printw("1.List All Items\n");
	printw("2.Add Item to Inventory\n");
	printw("3.Update Item\n");
	printw("4.Filter By Tags\n");
	printw("5.Find Item by ID\n");
	printw("6.Delete Item By ID\n");
	printw("7.Find Item by Name\n");
	printw("8.Sort Items\n");
	printw("9.Exit Program\n");
	int choice;
	scanw("%d", &choice);
	fflush(stdin);
	item* newData = data;
	switch(choice){
		case 1:
			listAllItems(data, *size);
		break;
		case 2:
			newData = addItem(data, *size);
			(*size)++;
		break;
		case 3:
		 	newData = updateItemByID(data, *size);
		break;
		case 4:
			filterByTags(data, *size);
		break;
		case 5:
			findByID(data, *size);
		break;
		case 6:
			newData = deleteByID(data, size);
		break;
		case 7:
			findByName(data, *size);
		break;
		case 8:
			sortData(data, *size);
			break;
		case 9:
			return newData;
		break;
		default: 
			printw("\nRe-enter valid choice\n");
			int rows,columns;
			getmaxyx(stdscr, rows, columns);
			move(rows-1,0);
			printw("<-Press Any Key->");
			getch();
			move(rows-1,0);
			clrtoeol();
			move(0,0);

	}
	clear();
	return displayMainMenu(newData,size);
}

//Display all items
void listAllItems(item data[], int size){
	int x, y;
	int rows, columns;
	getmaxyx(stdscr,rows,columns);
	//Display table headers
	printw("%-6s | %-15s | %-75s | %11s | %11s | %-10s | %-11s\n",
				header[0],
				header[1],
				header[2],
				header[3],
				header[4],
				header[5],
				header[6]
				);
	for(int i = 0; i < size; i++){
		getyx(stdscr, y, x);
		if(y == (rows - 1)){ //Implements pages in terminal
			printw("<-Press Any Key->");
			getch();
			clear();
			move(0, 0);
			//Display table headers on screen refresh
			printw("%-6s | %-15s | %-75s | %11s | %11s | %-10s | %-11s\n",
				header[0],
				header[1],
				header[2],
				header[3],
				header[4],
				header[5],
				header[6]
			);
			//Display next item in page
			printw("%-6d | %-15s | %-75s | %11.2f | %11.2f | %-10d | %-11s\n",
				data[i].ID,
				data[i].tag,
				data[i].itemName,
				data[i].costPrice,
				data[i].listPrice,
				data[i].quantity,
				data[i].company
			);
		}
		else{ 
			//Display item in page
			printw("%-6d | %-15s | %-75s | %11.2f | %11.2f | %-10d | %-11s\n",
				data[i].ID,
				data[i].tag,
				data[i].itemName,
				data[i].costPrice,
				data[i].listPrice,
				data[i].quantity,
				data[i].company
			);	
		}
	}
	 //Exit the screen on pressing a key
	move(rows-1,0);
	printw("<-Press Any Key->");
	getch();

	move(rows-1,0);
	clrtoeol();
	move(0,0); //Resetting the cursor
}

//Find data by ID and display if it exists
void findByID(item* data,int size){
	printw("Enter the ID of the item to find: ");
	int toFindID;
	int items = 0;
	scanw("%d",&toFindID);
	fflush(stdin);
	for(int i = 0; i < size; i++){
		if(data[i].ID == toFindID){
			//Display table header when item with the given ID is found
			printw("%-6s | %-15s | %-75s | %11s | %11s | %-10s | %-11s\n",
				header[0],
				header[1],
				header[2],
				header[3],
				header[4],
				header[5],
				header[6]
			);
			//Display item with given ID
			printw("%-6d | %-15s | %-75s | %11.2f | %11.2f | %-10d | %-11s\n",
				data[i].ID,
				data[i].tag,
				data[i].itemName,
				data[i].costPrice,
				data[i].listPrice,
				data[i].quantity,
				data[i].company
			);
			items++;
			break;
		}
	}
	if(items == 0){ //Checks whether the item is found
		printw("Item with ID %d is not found", toFindID);
	}
	//Exit the screen on pressing a key
	int rows,columns;
	getmaxyx(stdscr, rows, columns);

	move(rows-1,0);
	printw("<-Press Any Key->");
	getch();

	move(rows-1,0);
	clrtoeol();
	move(0,0); //Resetting the cursor
}

void filterByTags(item* data, int size){
	//Linked list used to store variable number of tagNames
	typedef struct node{
		char tagName[16];
		struct node* next;
	} node;
	node* head = (node*) malloc(sizeof(node));
	head->next = NULL;
	for(int i = 0; i < size; i++){
		node* temp = head;
		int found = 1;
		while(temp->next != NULL){ //Steps through the linked list

			if(strcmp(temp->next->tagName,data[i].tag) == 0){
				found = 0;
				break;
			}

			temp = temp->next;
		}
		if(found){ //Adds new tagName to the end of the linked list if not already present

			temp->next = (node*)malloc(sizeof(node));
			strcpy(temp->next->tagName,data[i].tag);
			temp->next->next = NULL;

		}
		
	}

	//Display tags gathered from the data
	printw("TAGS: \n");
	node* tmp = head->next;
	while(tmp != NULL){
		printw("%s\t",tmp->tagName);
		tmp = tmp->next;
	}
	printw("\n");
	while(1){
		//Input for the tag user desires to filter by
		printw("Enter tag name to filter by: ");
		char choice[16];
		scanw("%s",choice);
		fflush(stdin);
		//Stepping through the linked list to find the tag if present
		node* temp = head->next;
		int notFound = 1;
		while(temp != NULL){
			if(strcmp(temp->tagName,choice) == 0){//Tag is present
				notFound = 0;
				int x, y;
				int rows, columns;
				getmaxyx(stdscr,rows,columns);
				//Display headers
				printw("%-6s | %-15s | %-75s | %11s | %11s | %-10s | %-11s\n",
							header[0],
							header[1],
							header[2],
							header[3],
							header[4],
							header[5],
							header[6]
							);
				for(int i = 0; i < size; i++){
					getyx(stdscr, y, x);
					if(y == (rows - 1)){
						printw("<-Press Any Key->");
						getch();
						clear();
						move(0, 0);
						//Display headers
						printw("%-6s | %-15s | %-75s | %11s | %11s | %-10s | %-11s\n",
							header[0],
							header[1],
							header[2],
							header[3],
							header[4],
							header[5],
							header[6]
						);
						//Display item if it corresponds to the tag
						if(strcmp(data[i].tag,choice) == 0){
							printw("%-6d | %-15s | %-75s | %11.2f | %11.2f | %-10d | %-11s\n",
								data[i].ID,
								data[i].tag,
								data[i].itemName,
								data[i].costPrice,
								data[i].listPrice,
								data[i].quantity,
								data[i].company
							);
						}
					} else { 
						//Display item if it corresponds to the tag
						if(strcmp(data[i].tag,choice) == 0){
							printw("%-6d | %-15s | %-75s | %11.2f | %11.2f | %-10d | %-11s\n",
								data[i].ID,
								data[i].tag,
								data[i].itemName,
								data[i].costPrice,
								data[i].listPrice,
								data[i].quantity,
								data[i].company
							);
						}	
					}
				}
				move(rows-1,0);
				printw("<-Press Any Key->");//Exit screen on pressing a key

				getch();
				move(rows-1,0);
				clrtoeol();
				move(0,0);//Resetting the cursor
				break;
			}
			temp = temp->next;
		}
		if(notFound){//Tag is not present, rerun the loop
			int x, y;
			getyx(stdscr, y, x);
			printw("Re-enter correct tag name");

			int rows,columns;
			getmaxyx(stdscr, rows, columns);
			move(rows-1,0);
			printw("<-Press Any Key->");//Exit screen on key press

			getch();//Resetting cursor and input
			move(rows-1,0);
			clrtoeol();
			move(y,0);
			clrtoeol();
			move(--y,0);
			clrtoeol();
		} else {
			break;
		}
	}
}

item* deleteByID(item* data, int *size){
	int deleteID;
	//Taking input of ID of item user wishes to delete
	printw("Enter ID of item to delete: ");
	scanw("%d",&deleteID);
	int notFound = 1;
	for(int i = 0; i < (*size); i++){
		if(data[i].ID == deleteID){
			notFound = 0;
			break;
		}
	}

	if(notFound){//Check if ID is present or not
		printw("Item with ID %d not found.",deleteID);

		int rows,columns;
		getmaxyx(stdscr, rows, columns);
		move(rows-1,0);
		printw("<-Press Any Key->");//Exit the screen

		getch();
		move(rows-1,0);
		clrtoeol();
		move(0,0);//resetting the cursor

		return data;
	} else {
		int j = 0;
		item* newData = (item*)calloc(sizeof(item),(*size));
		int s = *size;
		for(int i = 0; i < s; i++){
			if(data[i].ID == deleteID){
				(*size)--;
				continue;
			}
			newData[j] = data[i];
			j++;
		}
		printw("Item has been deleted.");
		int rows,columns;
		getmaxyx(stdscr, rows, columns);
		move(rows-1,0);
		printw("<-Press Any Key->");

		getch();
		move(rows-1,0);
		clrtoeol();

		return newData;
	}
}

item* addItem(item* data, int size){
	item* newData = (item*) calloc(sizeof(item),++size);
	//Copying over the prexisting data to the new data array
	for(int i = 0; i < size-1; i++){
		newData[i] = data[i];
	}

	//Entering details for the new item
	printw("Enter unique ID: ");
	scanw("%d",&newData[size-1].ID);
	fflush(stdin);
	printw("Enter tag (<=15 characters): ");
	scanw("%s",newData[size-1].tag);
	fflush(stdin);
	printw("Enter item name (<=79 characters): ");
	scanw("%[^\n]",newData[size-1].itemName);
	fflush(stdin);
	printw("Enter Cost Price: ");
	scanw("%f",&newData[size-1].costPrice);
	fflush(stdin);
	printw("Enter List Price: ");
	scanw("%f",&newData[size-1].listPrice);
	fflush(stdin);
	printw("Enter Quantity: ");
	scanw("%d",&newData[size-1].quantity);
	fflush(stdin);
	printw("Enter Company Name (<=19 characters): ");
	scanw("%s",newData[size-1].company);
	fflush(stdin);
	printw("Item has been added.");

	int rows,columns;
	getmaxyx(stdscr, rows, columns);
	move(rows-1,0);
	printw("<-Press Any Key->");//Exit the screen on pressing a key

	getch();
	move(rows-1,0);
	clrtoeol();//Resetting the cursor
	return newData;
}

item* updateItemByID(item* data, int size){

	item* newData = (item*) calloc(sizeof(item),size);
	//Copying over the prexisting data to the new data array
	for(int i = 0; i < size; i++){
		newData[i] = data[i];
	}

	int IDtoUpdate;
	printw("Enter ID of item to update: ");
	scanw("%d", &IDtoUpdate);
	fflush(stdin);

	int index = -1;
	for(int i = 0; i < size; i++){
		if(data[i].ID == IDtoUpdate){
			index = i;
			break;
		}
	}

	//Exits function if the item with ID IDtoUpdate isn't present in the data
	if(index == -1){
		printw("Item not found.");

		int rows,columns;
		getmaxyx(stdscr, rows, columns);
		move(rows-1,0);
		printw("<-Press Any Key->");//Exit the screen on pressing a key

		getch();
		move(rows-1,0);
		clrtoeol();//Resetting the cursor
		return data;
	}

	//Lists current data if present
	printw("Current entry: \n");
	printw("%-6s | %-15s | %-75s | %11s | %11s | %-10s | %-11s\n",
							header[0],
							header[1],
							header[2],
							header[3],
							header[4],
							header[5],
							header[6]
							);
	printw("%-6d | %-15s | %-75s | %11.2f | %11.2f | %-10d | %-11s\n",
								data[index].ID,
								data[index].tag,
								data[index].itemName,
								data[index].costPrice,
								data[index].listPrice,
								data[index].quantity,
								data[index].company
							);

	//Entering new data for updation
	printw("Enter new tag (<=15 characters): ");
	scanw("%s",newData[index].tag);
	fflush(stdin);
	printw("Enter new item name (<=79 characters): ");
	scanw("%[^\n]",newData[index].itemName);
	fflush(stdin);
	printw("Enter new Cost Price: ");
	scanw("%f",&newData[index].costPrice);
	fflush(stdin);
	printw("Enter new List Price: ");
	scanw("%f",&newData[index].listPrice);
	fflush(stdin);
	printw("Enter new Quantity: ");
	scanw("%d",&newData[index].quantity);
	fflush(stdin);
	printw("Enter new Company Name (<=19 characters): ");
	scanw("%s",newData[index].company);
	fflush(stdin);
	printw("Item has been updated.");

	int rows,columns;
	getmaxyx(stdscr, rows, columns);
	move(rows-1,0);
	printw("<-Press Any Key->");//Exit the screen on pressing a key

	getch();
	move(rows-1,0);
	clrtoeol();//Resetting the cursor

	return newData;
}


void sortFunction(item* data, int size, int (*comparator)(item a, item b)){
	for (int i = 0; i < size-1; i++)
	{
		// Find the minimum element in unsorted array
		int min_idx = i;
		for (int j = i + 1; j < size; j++)
		if (comparator(data[min_idx],data[j])){
			min_idx = j;
		}
			if(min_idx != i)
			swapItems(&data[min_idx], &data[i]);
	}
}

void sortData(item* data, int size){
	printw("1. Sort By ID\n");
	printw("2. Sort By Tag\n");
	printw("3. Sort By Item Name\n");
	printw("4. Sort By Cost Price\n");
	printw("5. Sort By List Price\n");
	printw("6. Sort By Quantity\n");
	printw("7. Sort By Company\n");
	int correct = 0;
	while(1){
		printw("Enter your choice: ");
		int choice;
		scanw("%d",&choice);
		switch(choice){
			case 1:
				sortFunction(data, size, comparatorID);
				correct = 1;
			break;
			case 2:
				sortFunction(data, size, comparatorTag);
				correct = 1;
			break;
			case 3:
				sortFunction(data, size, comparatorName);
				correct = 1;
			break;
			case 4:
				sortFunction(data, size, comparatorCostPrice);
				correct = 1;
			break;
			case 5:
				sortFunction(data, size, comparatorListPrice);
				correct = 1;
			break;
			case 6:
				sortFunction(data, size, comparatorQuantity);
				correct = 1;
			break;
			case 7:
				sortFunction(data, size, comparatorCompany);
				correct = 1;
			break;
			default:
			{ 
				printw("Re-enter correct option");
				int x, y;
				getyx(stdscr, y, x);
				int rows,columns;
				getmaxyx(stdscr, rows, columns);
				move(rows-1,0);
				printw("<-Press Any Key->");//Exit screen on key press

				getch();//Resetting cursor and input
				move(rows-1,0);
				clrtoeol();
				move(y,0);
				clrtoeol();
				move(--y,0);
				clrtoeol();
			}	
		}
		if(correct){
			printw("Finished sorting.");
			int rows,columns;
			getmaxyx(stdscr, rows, columns);
			move(rows-1,0);
			printw("<-Press Any Key->");
			getch();
			break;
		}
	}
}

void swapItems(item* a,item* b)
{
	item temp = *a;
	*a = *b;
	*b = temp;
}

int comparatorCostPrice(item a, item b){
	return a.costPrice >= b.costPrice ? 1:0;
}
int comparatorID(item a, item b){
	return a.ID >= b.ID ? 1:0;
}
int comparatorTag(item a, item b){
	return strcmp(a.tag, b.tag) >= 0;
}
int comparatorListPrice(item a, item b){
	return a.listPrice >= b.listPrice ? 1:0;
}
int comparatorName(item a, item b){
	return strcmp(a.itemName,b.itemName) >= 0 ? 1:0;
}
int comparatorCompany(item a, item b){
	return strcmp(a.company,b.company) >= 0 ? 1:0;
}
int comparatorQuantity(item a, item b){
	return a.quantity >= b.quantity ? 1:0;
}

int compsubs(char *s,char *yu)//defining a separate function for generating and checking all the substrings
{
    char nullstr = '\0';
    int s1 = 0, s2 = strlen(s),retval;//retval 0:1 if it matches or not
    while (yu[s1] != nullstr)
    {
        s1++;
        for(int i = 0;i <= s1-s2;i++) // this loop is only enterd if length of secondary string is less than the main string
        {
            for(int j = i;j < i+s2;j++)
            {
                retval = 1;
                if(yu[j] != s[j-i])
                {
                    retval = 0;
                    break;
                }
            }
            if(retval == 1)
            {
                break;
            }
        }
    }
    return retval; // effectively returns if the secondary string is a substring of the first string
}//function ends

void findByName(item* data, int size)
{
    char searchele[79];//item name to be searched up
    printw("Please enter the item name: ");
    scanw("%[^\n]",&searchele);
    fflush(stdin); // taking input
    int rach = 0;
    for(int i = 0; i < (size); i++)
    {
        int res;
        res = compsubs(searchele,data[i].itemName);
        if(res == 1)
        {   rach = 1;
            int x,y;
            int rows, columns;
            getmaxyx(stdscr,rows,columns);
            getyx(stdscr, y, x);
            if(y == (rows - 1)){
                printw("<-Press Any Key->");
                getch();
                clear();
                move(0, 0);
                //Display headers
                printw("%-6s | %-15s | %-75s | %11s | %11s | %-10s | %-11s\n",
                    header[0],
                    header[1],
                    header[2],
                    header[3],
                    header[4],
                    header[5],
                    header[6]
                );
                //Display item if it corresponds to the tag
                printw("%-6d | %-15s | %-75s | %11.2f | %11.2f | %-10d | %-11s\n",
                    data[i].ID,
                    data[i].tag,
                    data[i].itemName,
                    data[i].costPrice,
                    data[i].listPrice,
                    data[i].quantity,
                    data[i].company
                );
            } else {
                //Display item if it corresponds to the tag
                printw("%-6d | %-15s | %-75s | %11.2f | %11.2f | %-10d | %-11s\n",
                    data[i].ID,
                    data[i].tag,
                    data[i].itemName,
                    data[i].costPrice,
                    data[i].listPrice,
                    data[i].quantity,
                    data[i].company
                );
            
            }
        }

    }
    if(rach == 0)
    {
        printw("Not Found in the present inventory.\nPlease choose again.\n");
    }
    int rows,columns;
    getmaxyx(stdscr, rows, columns);
    move(rows-1,0);
    printw("<-Press Any Key->");//Exit the screen on pressing a key

    getch();
    move(rows-1,0);
    clrtoeol();//Resetting the cursor
}




