typedef struct item {
	int ID;
	char tag[16];
	char itemName[80];
	float costPrice;
	float listPrice;
	int quantity;
	char company[20];
} item;

item* displayMainMenu(item* data, int *size);
void listAllItems(item* data, int size);
item* addItem(item* data, int size);
item* updateItemByID(item* data, int size);
void filterByTags(item* data, int size);
void findByID(item* data, int size);
item* deleteByID(item* data, int *size);
void initialisation();
//sort functions
void sortData(item* data,int size);
void sortFunction(item* data, int size, int (*func) (item a, item b));
void findByName(item* data, int size);
int comparatorCostPrice(item a, item b);
int comparatorID(item a, item b);
int comparatorTag(item a, item b);
int comparatorListPrice(item a, item b);
int comparatorName(item a, item b);
int comparatorCompany(item a, item b);
int comparatorQuantity(item a, item b);
void swapItems(item* a,item* b);



