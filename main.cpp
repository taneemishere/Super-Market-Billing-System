#include <iostream>	// cin, cout and so on
#include <string>	// for string, getline()
#include <fstream>	// for file handling read, write, open, close....
#include <conio.h>	 // for getch() and getche()
#include <stdlib.h> // for clearing the screen, system("CLS")
#include <iomanip> // for setw() fucntion
// #include <windows.h> // for gotoxy() function
// #include <process.h>
// #include <stdio.h>

using namespace std;

class product
{
	int pNo;
	string name;
	//	char name[50];
	float price, quantity, discount;

public:
	// function used by administrator only
	void createProduct()
	{
		char yORn;
		system("CLS");
		cout << "\n\n=========================CREATING THE PRODUCT=========================";
		do
		{
			cout << "\n\n\nPlease enter product Number: ";
			cin >> pNo;
			cout << "\nPlease enter name of product: ";
			cin >> name;
			// gets(name);
			// getline(cin, name);
			cout << "\nPlease enter price of product: ";
			cin >> price;
			cout << "\nPlease enter the discount in percent: ";
			cin >> discount;
			cout << "\n\nDo you want to create another product (y/n): ";
			cin >> yORn;
		} while (yORn == 'y' || yORn == 'Y');
	}

	// function used by customer and by admin when to show the product, whether it is specific or to show all the products
	void showProduct()
	{
		int discountPrice;
		int netPrice;

		if (discount != 0)
		{
			discountPrice = ((price * discount) / 100);
			netPrice = price - discountPrice;
		}
		else if (discount == 0)
		{
			netPrice = price;
		}

		cout << "\n\n\n========================================";
		cout << "\nProduct Number is: " << pNo << endl;
		cout << "Product name is: " << name << endl;
		// cout << "Product name is: " << puts(name);
		cout << "Product\'s price is: " << price << endl;
		cout << "Discount is: " << discount << "%" << endl;
		cout << "Net Price is: " << netPrice;
	}

	/*These functions are for the returning of pNo, price, name and discount because these are the private
		data of the product class and we cannot access it outside the product class or elsewhere*/

	int retpNo()
	{
		return pNo;
	}

	float retPrice()
	{
		return price;
	}

	string retName()
	{
		return name;
	}

	int retdis()
	{
		return discount;
	}
};


fstream fp; // object of fstream class the built in
product pr; // object of user defined "product" class


//function to write in file, runs in backend at the time of creating the product at administrator menu
void writeProduct()
{
	fp.open("Shop.dat", ios::out | ios::app);
	pr.createProduct();
	fp.write((char *)&pr, sizeof(product));
	fp.close();

	cout << "\n\nThe Product(s) has been created!";

	getch();
}

//function to read all records from file, runs in backend at the time of showing the product menu to the customer and to admin when click on display all products
void displayAll()
{
	system("CLS");
	cout << "\n\n"
		 << endl;
	cout << setw(70) << "DISPALYING ALL RECORDS\n"
		 << endl;
	fp.open("Shop.dat", ios::in);

	while (fp.read((char *)&pr, sizeof(product)))
	{
		pr.showProduct();
		cout << "\n\n========================================\n";
		getch();
	}

	fp.close();
	getch();
}

//function to read specific record from file
void displaySpecificRecord(int n)
{
	int flag = 0;
	fp.open("Shop.dat", ios::in);
	while (fp.read((char *)&pr, sizeof(product)))
	{
		if (pr.retpNo() == n)
		{
			pr.showProduct(); // loops and show the product at the specific number n
			flag = 1;
		}
	}

	fp.close();

	if (flag == 0)
	{
		cout << "\n\nThis record dosn't exist";
	}
	getch();
}

//function to modify record in file
void modifyProduct()
{
	int no, found = 0;

	cout << "\n\n\tTo Modify, Please enter the product number of the product: ";
	cin >> no;
	fp.open("Shop.dat", ios::in | ios::out);
	while (fp.read((char *)&pr, sizeof(product)) && found == 0)
	{
		if (pr.retpNo() == no)
		{
			pr.showProduct();
			getch(); // just to pause the menu on screen little bit
			cout << "\n\n\nPlease enter the new detail of product" << endl;
			getch(); // just to pause the menu on screen little bit
			pr.createProduct();
			int pos = -1 * sizeof(pr);
			fp.seekp(pos, ios::cur);
			fp.write((char *)&pr, sizeof(product));
			cout << "\n\n\t Record Updated!" << endl;
			found = 1;
		}
	}
	fp.close();
	if (found == 0)
	{
		cout << "\n\n Record not found!" << endl;
	}
	getch();
}

//function to delete record from file
void deleteProduct()
{
	int no;
	cout << "\n\n\tTo Delete, Please enter the product number of the product: ";
	cin >> no;
	fp.open("Shop.dat", ios::in | ios::out);
	fstream fp2;
	fp2.open("Temp.dat", ios::out);
	fp.seekg(0, ios::beg);
	while (fp.read((char *)&pr, sizeof(product)))
	{
		if (pr.retpNo() != no)
		{
			fp2.write((char *)&pr, sizeof(product));
		}
	}
	fp2.close();
	fp.close();
	remove("Shop.dat");
	rename("Temp.dat", "Shop.dat");
	cout << "\n\n Record Deleted...!";
	getch();
}

//function to display all products no, name and price
void mainMenu(){

	// the code segment when there isn't any product and the customer is wishing to see the list
	fp.open("Shop.dat",ios::in);
	if (fp)
	{
		cout << "\nThere aren't anything in the list to show. \n\nGo to admin menu to create file(s)." << endl;
		cout << "Program is closing...";
		getch();
		exit(0);
	}

	else if (!fp)
	{
		// the code segment when there is or are product(s) available
		cout << "\n\n\t\t PRODUCT MENU\n\n";
		cout << "====================================================" << endl;
		cout << "Product No.\t\tName\t\tPrice\n";
		cout << "====================================================\n"
			 << endl;
		while (fp.read((char *)&pr, sizeof(product)))
		{
			cout << pr.retpNo() << "\t\t" << pr.retName() << "\t\t" << pr.retPrice() << endl;
		}
		fp.close();
	}
}

// function called at the very last time to show the receipt type something, (used by the customer)
void placeOrder()
{
	system("CLS");
	int orderArray[50], quantity[50], c = 0;
	float amount, discountAmount, total = 0;
	char ch = 'Y';

	mainMenu();

	cout << "\n\n============================";
	cout << "\n    PLACE YOUR ORDER";
	cout << "\n============================\n";

	do
	{
		cout << "\n\nEnter the Product Number: ";
		cin >> orderArray[c];
		cout << "Enter the Quantity (in number): ";
		cin >> quantity[c];
		c++;

		cout << "\nDo you want to order another product? (y/n)";
		cin >> ch;
	} while (ch == 'y' || ch == 'Y');

	cout << "\n\nThank you for placing order!";
	getch();
	cout << "\n\n\n\n************************INVOICE************************\n";
	cout << "\nProduct No. \tProduct Name \tQuantity \tPrice \tAmount \tAmount after discount\n";

	for (int x = 0; x <= c; x++)
	{
		fp.open("Shop.dat", ios::in);
		fp.read((char *)&pr, sizeof(product));

		while (!fp.eof())
		{
			if (pr.retpNo() == orderArray[x])
			{
				amount = pr.retPrice() * quantity[x];
				discountAmount = amount - (amount * pr.retdis() / 100);

				cout << "\n"
					 << orderArray[x] << " \t\t" << pr.retName() << " \t" << quantity[x] << " \t\t" << pr.retPrice() << " \t" << amount << "\t" << discountAmount;
				total += discountAmount;
			}

			fp.read((char *)&pr, sizeof(product));
		}

		fp.close();
	}

	cout << "\n\n\n\n\t\t\t\tTOTAL = " << total;
	getch();
}

// Function to define the gotoxy()

// void gotoxy(int x, int y){
// 	static HANDLE h = NULL;
// 	if(!h){
// 		h = GetStdHandle(STD_OUTPUT_HANDLE);
// 		COORD c = { x, y };
// 		SetConsoleCursorPosition(h, c);
// 	}
// }

// INTRO!!!!!
void intro()
{
	cout << "\n\n"
		 << endl;
	cout << setw(70) << "SUPER MARKET BIILING PROJECT";
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	cout << setw(115) << "Developed by : Taneem Jan";
	getch();
}

// ADMIN Menu
void adminMenu()
{
	char charact;

	system("CLS");
	cout << "\n\n\n\tADMIN MENU" << endl;
	cout << "***************************" << endl;
	cout << "\n\t1.Create Product" << endl;
	cout << "\n\t2.Display All Products" << endl;
	cout << "\n\t3.Dispaly Specific Record" << endl;
	cout << "\n\t4.Modify Product" << endl;
	cout << "\n\t5.Delete Product" << endl;
	cout << "\n\t6.View Product Menu" << endl;
	cout << "\n\t7.Back To Main Menu" << endl;
	cout << "\n\tPlease enter your choice (1-7): ";
	charact = getche();

	switch (charact)
	{
	case '1':
		writeProduct();
		break;

	case '2':
		displayAll();
		break;

	case '3':
		int number;
		cout << "\n\n\tPlease enter product number: ";
		cin >> number;
		displaySpecificRecord(number);
		break;

	case '4':
		modifyProduct();
		break;

	case '5':
		deleteProduct();
		break;

	case '6':
		mainMenu();
		getch();

	case '7':
		break;

	default:
		cout << "\a";
		adminMenu();
	}
}

// password Menu
void password()
{
	string uName;
	string pass;

	system("CLS");
	/* this whlie loop is for the break statement becuase the break only works within
	the switch or the looping statements */
	while (1)
	{
		cout << "\n\n\n\tADMIN LOGIN MENU" << endl;
		cout << "********************************" << endl;
		cout << "\n\n"
			 << endl;
		cout << "  Please enter admin username: ";
		cin >> uName;
		cout << "  Please enter admin password: ";
		cin >> pass;

		if ((uName == "admin") && (pass == "adminpass"))
		{
			adminMenu();
		}
		else if ((uName != "admin") || (pass != "adminpass"))
		{
			cout << "  Username or Password is incorrect :-(" << endl;
			getch();
			break; /* this break statement will lead us back to the main menu when the password or
			username is incorrect */
		}
	}
}

// class date
// {
// 	public:
// 		int aday,amonth,ayear,ahour,amin,asec;
// 	void date_times()
// 	{
// 		SYSTEMTIME t;
//  		GetLocalTime(&t);
// 		aday=t.wDay,amonth=t.wMonth,ayear=t.wYear,ahour=t.wHour,amin=t.wMinute,asec=t.wSecond;	
// 	}
// };

// char *date_time()
// {
// 	time_t t;
// 	time(&t);
// 	char *loc=ctime(&t);
// 	return loc;	
// }

// void frame()
// {
// 	int i,j=79;
// 	for(i = 0;i<80,j>=0;i++,j--)
// 	{
// 		gotoxy(i,3);
// 		Sleep(10);
// 		cout<<"_";
// 		gotoxy(j,20);
// 		Sleep(10);
// 		cout<<"_";
// 	}
// 	gotoxy(56,2);
// 	char s[20];
// 	strcpy(s,date_time());
// 	cout<<s; 
	
// }

int main()
{

	char ch;
	intro();

	do
	{
		system("CLS");
		cout << "\n\n\n\tMAIN MENU" << endl;
		cout << "\n\t01. CUSTOMER" << endl;
		cout << "\n\t02. ADMINISTRATOR" << endl;
		cout << "\n\t03. EXIT" << endl;
		cout << "\n\tPlease select your option (1-3): ";
		ch = getche();

		switch (ch)
		{
		case '1':
			placeOrder();
			getch();
			break;

		case '2':
			// adminMenu();
			password();
			break;

		case '3':
			exit(0);

		default:
			cout << "\a";
		}

	} while (ch != '3');
	return 0;
}
