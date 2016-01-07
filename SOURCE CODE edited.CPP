/*****************

Created by Anirudh Katoch
Student of Rajesh Sethi
The Heritage School, Vasant Kunj

for CBSE class 12 board examination 2014

Do not copy

katoch.anirudh@gmail.com

******************/

/**********************************
LIBRARIES
**********************************/

#include<fstream.h>
#include<conio.h>
#include<string.h>
#include<stdio.h>

/***********************************
GLOBAL VARIABLES
Only constant global variables were used, due to security concerns.
***********************************/

const char _fg=WHITE, _sfg=YELLOW, _bg=BLUE, _sbg=RED;
//set the colour scheme: foreground, special foreground, background, special background
const char* HELPTEXT = "W and S to scroll, spacebar or enter to select";
//standard help message to display in most menus
const char* loclist[] = {"South Delhi", "North Delhi", "East Delhi","West Delhi", "Gurgaon"};
const int locations = 5;
//List of valid localities this software is supposed cater to

/*********************************************
CLASS DECLARATIONS AND FUNCTION PROTOTYPES
**********************************************/

class user;
class restaurant;

int addtocursor(int, int, int);
char* _extr(int, char[], char*);
int compareRestaurantProperty(restaurant, restaurant, int);

void setfg(int);
void setbg(int);
void _disp(char, char, char, char);
void _disp(char*, char, char, char);
void _paintscr();
void _box(int, int, int, int, char);
void _tip(char[]);
char _popup(char*, char[500]);
char* _getInput(int, char*, char*, int=0);
int _makemenu(char*, char*, int);
int dispRest(restaurant);
restaurant setValidValue(restaurant, int);

void mainmenu(user);
void dispMenu(user);
void editMenu(user);
void loginMenu(user);

restaurant makeRest();
user getAccount(char*, char*);
restaurant selectRest();
user selectUser();
int addRestaurant(restaurant, user);
int delRestaurant(restaurant, user);
int addUser(user, user);
int delUser(user, user);
int editRestaurant(restaurant, user);

/*************************************
CLASSES
The program uses 2 classes. One for storing data about different users, and the other for restaurants.
**************************************/

class user
{
	int access; //0 = guest, 1 = admin
	char name[10];
	char pass[10];
	public	:
		user(int, char*, char*);
		int getAccess();
		char* getName(char*);
		int checkPass(char*);
};

user::user(int acc=0, char* nm="guest", char* ps="")
{
	access=acc;
	if(strlen(nm)>0)
		strcpy(name,nm);
	else
		strcpy(name, "guest");
		//If passed an empty string for username, it automatically assumes guest account
	strcpy(pass,ps);
}
int user::getAccess()
{
	return access;
}
char* user::getName(char* result)
{
	strcpy(result, name);
	return result;
}
int user::checkPass(char* ps)
{
	return strcmp(ps, pass)?0:1;
}

class restaurant
{
	char null; //if this is true, the restaurant has junk values
	char name[30];
	char type[30]; //Indian, Fast Food etc.
	char location[30]; //Locality: South Delhi, Gurgaon etc.
	char address[50]; //Full address
	char phone[11];
	int rating; //1 to 5

	public:
		restaurant();
		void activate();
		int isActive();
		char* getName(char*);
		char* getType(char*);
		char* getLocation(char*);
		char* getAddress(char*);
		char* getPhone(char*);
		int getRating();
		int cmp(restaurant*);
		int setName(char*);
		int setAddress(char*);
		int setType(char*);
		int setRating(int);
		int setLocation(char*);
		int setPhone(char*);
};

restaurant::restaurant()
{
	null=1; //Restaurant is assumed junk until activated
}
void restaurant::activate()
{
	null=0;
}
int restaurant::isActive()
{
	return !null;
}
char* restaurant::getName(char* temp)
{
	strcpy(temp, name);
	return temp;
}
char* restaurant::getType(char* temp)
{
	strcpy(temp, type);
	return temp;
}
char* restaurant::getLocation(char* temp)
{
	strcpy(temp, location);
	return temp;
}
char* restaurant::getAddress(char* temp)
{
	strcpy(temp, address);
	return temp;
}
char* restaurant::getPhone(char* temp)
{
	strcpy(temp, phone);
	return temp;
}
int restaurant::getRating()
{
	return rating;
}
int restaurant::cmp(restaurant* a)
//Compares to another restaurant. Returns 1 they are similar.
{
	char temp[50];
	if(!strcmp(a->getName(temp), name) && !strcmp(a->getAddress(temp), address) && a->isActive()==isActive())
		return 1;
		//Two restaurants are assumed to be the same if they have the same name and address.
	return 0;
}
int restaurant::setName(char* nm)
{
	if(!strlen(nm))
		return 1; //error, blank string
	strcpy(name, nm);
	return 0; //success
}
int restaurant::setAddress(char* nm)
{
	if(!strlen(nm))
		return 1; //error, blank string
	strcpy(address, nm);
	return 0; //success
}
int restaurant::setType(char* temp)
{
	if(!strlen(temp))
		return 1; //error, blank string
	strcpy(type, temp);
	return 0;
}
int restaurant::setRating(int temp)
{
	rating = temp;
	if(rating>5)
		rating=5;
	if(rating<1)
		rating=1;
	return 0;
	//Rating above 5 or below 1 is automatically capped
}
int restaurant::setLocation(char* temp)
{
	int flag = 0;
	for(int i=0; i<=locations-1; i++)
		if(!strcmp(temp, loclist[i])) //Checks if input location is included in the array of possible locations
			flag=1;
	if(flag)
	{
		strcpy(location, temp);
		return 0; //success
	}
	return 1; //Invalid location
}
int restaurant::setPhone(char* temp)
{
	int flag=0;
	for(int i=0; i<=9; i++)
		switch(temp[i])
		{
			case '1'	:
			case '2'	:
			case '3'	:
			case '4'	:
			case '5'	:
			case '6'	:
			case '7'	:
			case '8'	:
			case '9'	:
			case '0'	:	break;
			default		:	flag=1;	//flag as invalid input
						i=10;	//break the loop
		}
	if(flag)
		return 1; //Error
	strcpy(phone, temp);
	return 0; //Success
}

/******************************************
MISCELLANEOUS FUNCTIONS
These functions are being used regularly
in the program for various purposes
******************************************/

int addtocursor(int cursor, int limit, int val)
//This is a function for addition on values that have an upper and lower limit beyond which they "wrap around" like a loop.
{
	cursor+=val;
	if(cursor>limit)
		cursor-=limit;
	if(cursor<=0)
		cursor+=limit;
	return cursor;
}


char* _extr(int t, char a[], char* result)
{
	//returns the string segment number 't' from a string 'a' where each segment is separated by a '\'
	int j=0, i=0, c=0;
	char temp[80];
	for(; i<=strlen(a)-1 && c<=t; i++)
	{
		if(a[i]=='\0')
			c=t+1;
		if(a[i]=='\\')
			c++;
		if(c==t&&a[i]!='\\')
		{
			temp[j]=a[i];
			j++;
		}	
	}
	temp[j]='\0'; //null character is appended to the string before returning it
	if(!strcmp(temp, "_h"))
		strcpy(temp, HELPTEXT); //if the string is "_h", it is substituted with the standard help message
	strcpy(result, temp);
	return result;
}
//This function is used by _makemenu and some other functions to extract different strings from a single string. It is more convinient than creating an array of strings.

int compareRestaurantProperty(restaurant a, restaurant b, int sort) //so we dont need to make different functions just to sort with different criteria
{
	char temps[50], temps2[50];
	switch(sort)
	{
		case 2	:	if(strcmp(a.getLocation(temps), b.getLocation(temps2))>0)
					return 1;
				return 0;
		case 3	:	if(strcmp(a.getType(temps), b.getType(temps2))>0)
					return 1;
				return 0;
		case 4	:	if(a.getRating()<b.getRating())
					return 1;
				return 0;
		default	:	if(strcmp(a.getName(temps), b.getName(temps2))>0)
					return 1;
				return 0;
	}
}

/*******************************************
INTERFACE FUNCTIONS
These functions are used by the program to display information, or even input information
******************************************/

void setfg(int set)
{
	static int fg=-1;
	if(fg!=set)
	{
		textcolor(set);
		fg=set;
	}
}

void setbg(int set)
{
	static int bg=-1;
	if(bg!=set)
	{
		textbackground(set);
		bg=set;
	}
}

void _disp(char a, char highlight = 0, char colour = 0, char blink=0)
{
	//displays a given character. May also highlight it, make it of the secondary colour, and/or add blinking effect.
	int temp;
	if(colour)
		temp=_sfg;
	else
		temp=_fg;
	if(blink)
		temp+=128;

	setfg(temp);
	if(highlight)
		setbg(_sbg);
	else
		setbg(_bg);
	putch(a);
}

void _disp(char* a, char highlight = 0, char colour = 0, char blink=0)
{
	//Overloaded function for strings
	int temp;
	if(colour)
		temp=_sfg;
	else
		temp=_fg;
	if(blink)
		temp+=128;
	setfg(temp);
	if(highlight)
		setbg(_sbg);
	else
		setbg(_bg);
	cputs(a);
}

void _paintscr() //clears the screen by painting it with spacebars.
{ 
			
		for(int var=1; var<=80; var++)
			_disp("                        ");
		//displays 80*24 spaces
}

void _box(int x1, int y1, int x2, int y2, char border=248)
{
	//Draws a box bound by the 4 given coords. You may also specify the character to use as the box edge	
	int i=x1, j=y1;
	for(; i<=x2; i++)
	{
		gotoxy(i, j);
		_disp(border, 0, 1);
	}
	i--;
	for(; j<=y2; j++)
	{
		gotoxy(i, j);
		_disp(border, 0, 1);
	}
	j--;
	for(; i>=x1; i--)
	{
		gotoxy(i, j);
		_disp(border, 0, 1);
	}
	i++;
	for(; j>=y1; j--)
	{
		gotoxy(i, j);
		_disp(border, 0, 1);
	}
}




void _tip(char tip[70])
{
	//goes to the bottom and displays the given string
	gotoxy(7, 24);
	for(int i=1; i<=70; i++)
		_disp(' ');
	gotoxy(7, 24);
	_disp(tip, 0, 1);
}

char _popup(char* head, char message[500]) //Brings up a message
{
	int lines = 0; //stores the number of lines used up, so we may make the box later
	_paintscr();
	gotoxy(3, 5);
	int i;
	for(i=3; i<=78; i++)
		_disp(' ', 1); //Paints special background for the heading
	gotoxy(40-strlen(head)/2, 5); //Goes to a suitable place to print the heading
	_disp(head, 1, 1); //prints the heading
	char temp[75]; //we store every line here before printing
	for(int lol=0; lol<=74; temp[lol]=0, lol++);
	int marker=0; //where we are on message
	for(i=2; marker<strlen(message); i++, lines++) //prints one line per loop
	{
		int endline=0; //flag
		for(int j=0; j<=73&&!endline; j++, marker++) //put ONE "line" in temp
		{
			if(message[marker]==' ') //wordwrapping
			{
				int wordlen; //the length of the next word
				for(wordlen=0; message[marker+wordlen+1]!= ' '&&message[marker+wordlen+1]!= '\0'; wordlen++); //count the length of next word
				if(73-j<wordlen) //check if the length is greated than the space left in the line
					message[marker]='\n';
			}
			if(message[marker]=='\n') //If the line is supposed to end now, or *has* to end due to wordwrap
			{
				temp[j]=0;
				endline=1; //flag
			}			
			else
				temp[j]=message[marker]; //put it in temp
		}
		gotoxy(4, 5+i); //get to the suitable place to print
		_disp(temp); //print temp
	}
	_box(2, 2, 80, 9+lines, 176);
	_box(1, 1, 79, 8+lines, 178);
	_tip("(PRESS ANY KEY TO CONTINUE)");
	return getch();
}


char* _getInput(int maxlen, char* message, char* a, int hide=0) //Brings up a diaglogue box to take input strings
{
	_paintscr();
	_box(2, 2, 80, 12, 176);
	_box(1, 1, 79, 13, 178);
	gotoxy(3, 5);
	int i;
	for(i=3; i<=78; i++)
		_disp(' ', 1); //Paints special background for the heading
	gotoxy(40-strlen(message)/2, 5); //Goes to a suitable place to print the heading
	_disp(message, 1, 1); //prints the heading
	gotoxy(20, 9);
	char temp;
	a[maxlen]=0; //makes a null at the end in anticipation
	for(i=0; i<=maxlen-1; i++)
	{
		temp = getch();
		if(temp=='\r')
		{
			a[i]=0; //if user pressed enter, enter null char
			i=maxlen; //and exit loop
		}
		else
		if(temp=='\b')
		{
			if(i>=1) //if there are characters to delete
			{
				a[i-1]=0; //shorten the currently help string by one
				_disp('\b');
				_disp(' ');
				_disp('\b'); //take it away from display
				i-=2; //setback the input taking process by one step
			}
		}
		else
		{
			a[i]=temp; //add the char to string
			if(!hide)
				_disp(temp);
			else
				_disp('*');
		}		
	}
	return a;
}



int _makemenu(char* c, char* tips, int space=2) //Makes a menu, waits for the user to make a selection, and then returns the selection number
{
	//format for c is "HEADING\\OPTION1\\OPTION2\\OPTION3..."
	//format for tips is "DEFAULT MESSAGE\\TIP FOR OPTION1\\TIP FOR OPTION2\\TIP FOR OPTION3..."
	char temp[80];
	int counter=1, i=0; //Counter stores the total number of available selections + 1
	for(; i<=strlen(c)-1; i++)
		if(c[i]=='\\')
			counter++;
	//Counter now has the correct value, by counting the number of string segments in 'c'
	if(counter<2)
		return -1; //error
	_paintscr();
	int cursor = 1; //cursor stores the currently selected option. 1 at the beginning.
	int noi = counter-1; //number of items to display
	if(noi>12/space)
		noi=12/space;
	_box(2, 2, 80, noi*space + 10, 176);
	_box(1, 1, 79, noi*space + 9, 178);
	//made a box, with length suitable for the number of possible selections available
	gotoxy(3, 5);
	for(i=3; i<=78; i++)
		_disp(' ', 1); //Paints special background for the heading
	gotoxy(40-strlen(_extr(0, c, temp))/2, 5); //Goes to a suitable place to print the heading
	_disp(_extr(0, c, temp), 1, 1); //prints the heading
	gotoxy(10, (noi<2?-space:0)+6+2*space);
	_disp(">", 0, 1, 1);
	int ch=0;
	while(ch!=' '&&ch!='\r') //bounds the user to make a selection
	{
		for(i=1; i<=noi; i++) //prints all the possible options
		{
			gotoxy(12, 6+(i)*space);
			_disp("                                     ");
			gotoxy(12, 6+(i)*space);
			_disp(_extr(addtocursor(cursor, counter-1, i-2), c, temp), 0, i==2);
		}
		if(ch=='w'||ch=='s')
			_tip(_extr(cursor, tips, temp));
		else
			_tip(_extr(0, tips, temp));
		gotoxy(1, 1); //Takes the real cursor to a corner so it doesnt look ugly
		ch=getch();
		if(ch=='s')
			cursor = addtocursor(cursor, counter-1, 1);		
		else if(ch=='w')
			cursor = addtocursor(cursor, counter-1, -1);	
	}
	_paintscr(); //blanks the screen and clears the menu
	return cursor;
}

int dispRest(restaurant a)
//Displays details about a given restaurant
{
	if(!a.isActive())
	{
		_popup("ERROR!", "Restaurants do not exist");
		return 1;
	}
	char temp[50];
	int i;
	_box(2, 2, 80, 19, 176);
	_box(1, 1, 79, 18, 178);
	//made a box with a shadow
	gotoxy(3, 5);
	for(i=3; i<=78; i++)
		_disp(' ', 1); //Paints special background for the heading
	a.getName(temp);
	gotoxy(40-strlen(temp)/2, 5); //Goes to a suitable place to print the name
	_disp(temp, 1, 1); //prints the name
	gotoxy(7, 8);
	_disp("Name\t:\t");
	_disp(a.getName(temp));
	gotoxy(7, 10);
	_disp("Type\t:\t");
	_disp(a.getType(temp));
	gotoxy(7, 12);
	_disp("Address\t:\t");
	_disp(a.getAddress(temp));
	_disp(" : ");
	_disp(a.getLocation(temp));
	gotoxy(7, 14);
	_disp("Phone\t:\t");
	_disp(a.getPhone(temp));
	gotoxy(7, 16);
	_disp("Rating\t:\t");
	for(i=1; i<=a.getRating(); i++)
		_disp("*");
	_tip("(PRESS ANY KEY TO CONTINUE)");
	getch();
	return 0;
}

restaurant setValidValue(restaurant rest, int temp)
//Forces the user to input a valid attribute for the given restaurant. The attribute depends on parameter "temp".
{
	int valid;
	char temp2[100], temps[50];
	strcpy(temp2, "The location must be one of: ");
	for(valid=0; valid<=locations-1; valid++) //valid was not meant for this :p but just a quick use
	{	
		strcat(temp2, loclist[valid]); 
		strcat(temp2, ", ");
	} //prepares the error message for wrong locations
	valid=0;
	switch(temp)
	{
		case 1	:	while(!valid)
				switch(rest.setName(_getInput(30, "Enter name:", temps))) //setName only returns 0 when the object accepts the value as valid
				{
					case 0	:	valid=1;
							break;
					case 1	:	_popup("Error", "It cannot be empty!");
							break;
					default	:	_popup("Error", "There was an unexpected error");
				}
				break;
		case 2 :	while(!valid)
				switch(rest.setType(_getInput(30, "Enter food type:", temps)))
				{
					case 0	:	valid=1;
							break;

					case 1	:	_popup("Error", "It cannot be empty!");
							break;
					default	:	_popup("Error", "There was an unexpected error");
				}
				break;
		case 3 :	while(!valid)
				switch(rest.setAddress(_getInput(50, "Enter address:", temps)))
				{
					case 0	:	valid=1;
							break;
					case 1	:	_popup("Error", "It cannot be empty!");
							break;
					default	:	_popup("Error", "There was an unexpected error");
				}
				break;
		case 4 :	while(!valid)
				switch(rest.setLocation(_getInput(30, "Enter location:", temps)))
				{
					case 0	:	valid=1;
							break;
					default	:	_popup("Error", temp2);
				}
				break;
		case 5 :	while(!valid)
				switch(rest.setPhone(_getInput(10, "Enter phone:", temps)))
				{
					case 0	:	valid=1;
							break;
					default	:	_popup("Error", "Please enter 10 DIGITS only.");
				}
				break;
		case 6 :	while(!valid)
				{
					_getInput(1, "Enter rating:" ,temps);
					switch(temps[0])
					{
						case '1'	:
						case '2'	:
						case '3'	:
						case '4'	:
						case '5'	:	valid=1;
									break;
						default		:	_popup("Error", "Please enter a number from 1 to 5");
					}
				}
				rest.setRating(temps[0]-'0'); //rating is stored as integer
	}
	return rest;
}


///////////////THE MENUS/////////////////////////


void mainMenu(user you)
{
	_paintscr();
	switch(_makemenu("Restaurant Database Project\\Display\\Login\\Edit\\Exit", "_h\\Look up a restaurant\\Login for higher access\\Edit the database\\Goodbye!"))
	{
		case 1	:	dispMenu(you);
				break;
		case 2	:	loginMenu(you);
				break;
		case 3	:	if (you.getAccess()==1)
					editMenu(you);
				else
				{
					_popup("ERROR", "You do not have permission to do that!");
					mainMenu(you);
				}
				break;
		case 4	:	break;	//exit
	}
}

void dispMenu(user you)
{
	_paintscr();
	switch(_makemenu("Restaurant list\\View list\\Go back", "_h\\View the list of restaurants\\Back to main menu"))
	{
		case 1	:	dispRest(selectRest());
				dispMenu(you);
				break;
		case 2	:	mainMenu(you);
				break;
	}
}
void editMenu(user you)
{
	_paintscr();
	restaurant temp;
	char temps[30], sel, nm[10], ps[10];
	switch(_makemenu("Administerative options\\Add restaurant\\Delete restaurant\\Edit restaurant\\Add User\\Delete User\\Go back", "WARNING: ELEVATED PRIVILLAGES\\Add an entry in the database\\Remove an entry from database\\Edit an entry in database\\Add a new user\\Delete a previous user\\Back to main menu"))
	{
		case 1	:	temp = makeRest();
				switch(addRestaurant(temp, you))
				{
					case 0	:	_popup("Success", "Restaurant Successfully Added");
							break;
					case 2 	:	_popup("Error", "File could not be created");
							break;
					default :	_popup("Error", "An unexpected error ocurred");
				}
				editMenu(you);
				break;
		case 2	:	temp = selectRest();
				switch(delRestaurant(temp, you))
				{
					case 0	:	_popup("Success", "Restaurant Deleted");
							break;
					case 2	:	_popup("Error", "Could not find the file");
							break;
					default	:	_popup("Error", "An unexpected error occured");
				}
				editMenu(you);
				break;
		case 3	:	temp = selectRest();
				editRestaurant(temp, you);
				editMenu(you);
				break;
		case 4	:	_paintscr();
				int acc = _makemenu("Select Access Type\\User\\Admin", "_h\\User can view restaurant data\\Admin can edit database, and edit user data")-1;
				_getInput(9, "Enter Username", nm);
				_getInput(9, "Enter Password", ps, 1);
				user tempuser(acc, nm, ps);
				switch(addUser(tempuser, you))
				{
					case 4	:	_popup("ERROR", "Username cannot be 'guest'");
							break;
					case 3	:	_popup("ERROR", "Username already in use");
							break;
					case 2	:	_popup("ERROR", "Could not create files");
							break;
					case 0	:	_popup("Success", "User data successfully added");
				}
				editMenu(you);
				break;
		case 5	:	user tempuser2;
				tempuser2 = selectUser();				
				switch(delUser(tempuser2, you))
				{
					case 0	:	_popup("Success", "User deleted");
							break;
					case 3	:	_popup("ERROR", "User does not exist. Technically speaking, you can't possibly reach this message. Congratulations, you are extremely talented at ruining programs.");
							break;
					case 2	:	_popup("ERROR", "Could not create files");
							break;
					default	:	_popup("ERROR", "An unexpected error occured");
				}
				editMenu(you);
				break;		
		case 6	:	mainMenu(you);
				break;
	}
}
void loginMenu(user you)
{
	_paintscr();
	char nm[10];
	char ps[10];
	char bla[] = "Logged in as: ";
	_getInput(9, "Enter Username", nm);
	_getInput(9, "Enter Password", ps, 1);
	user tempuser;
	tempuser = getAccount(nm, ps); //attempts to retrive account with the given credentials
	char temp[10];
	if(!strcmp(tempuser.getName(temp), "guest"))
	{
		_popup("Login Failed", "The username or password you provided is invalid."); //if attempt was unsuccesful, current user is not changed
	}	
	else
	{
		you = tempuser;//otherwise, you log in to the other user
	}
	_popup("Login Info", strcat(bla, you.getName(temp)));
	mainMenu(you); //return to main menu with the new user
}

/*************************************************
DATA FUNCTIONS
These functions handle the data and the files for the restaurants and the users.
************************************************/

restaurant makeRest() //takes the data for a restaurant from user, makes sure it is valid, and returns it.
{
	restaurant rest;
	rest.activate();
	for(int i=1; i<=6; i++)
		rest = setValidValue(rest, i);
	return rest;
}

user getAccount(char* nm, char* ps)
//checks if login and password were valid, and return an object from file
{
	user temp;
	ifstream fin("users.dat",ios::in|ios::binary);
	if(!fin) //if no file is found, a new file is created with default credentials.
	{
		_popup("Error", "No user data was found. Making new file...");
		ofstream fout("users.dat", ios::out|ios::binary);
		if(!fout)
		{
			_popup("Error", "Could not create file.");
			return temp; //if it was still not able to make a file, it returns a guest account and exits
		}
		_popup("NOTE!", "Since user data files not found, default admin username and password is 'admin' and 'admin'.");
		user admin(1, "admin", "admin");
		fout.write((char*)&admin, sizeof admin);
		fout.close(); //otherwise, it creates the file and adds a single user, default admin/admin with access level 1
		if(!strcmp(nm, "admin")&&!strcmp(ps, "admin")) //if supplied the correct details, admin account is returned
			return admin;
		else
			return temp; //otherwise, guest account
		
	}
	int found=0;
	char name[10];
	while(!found && fin.read((char*)&temp,sizeof temp)) //keeps traversing giving temp all the values of the stored users until a match is found
	{
		if(!strcmp(nm, temp.getName(name)) && temp.checkPass(ps))
		{
			found=1;
		}
	}
	fin.close();
	if(found)
		return temp; //if a match was obtained, that account is returned
	user guest;
	return guest; //else guest
}





restaurant selectRest() //Gives user a list of restaurants, to select one from. Returns inactive restaurant on failure.
{
	ifstream fin("rest.dat", ios::in|ios::binary);
	restaurant temp, result, arr[200];
	if(!fin)
		return result; //if file not found, return result, which is not activated
	int total, i, j;
	for(total=0; fin.read((char*)&temp, sizeof temp);) 
	{
		if(temp.isActive())
		{
			arr[total]=temp;
			total++;
		}	
	}
	fin.close();
	
	if(total<1)
		return result;
	
	//total now contains the total number of restaurants. arr contains all the restaurants.
	
	int sort = _makemenu("How would you like to view the records?\\Sort by name\\Sort by location\\Sort by food\\Sort by rating", "_h\\View in alphabetical order\\Sort by local region\\Sort by type of cuisine\\Sort by quality");

	restaurant s;
	for(i=0; i<=total-2; i++)
	{
		for(j=i+1; j>=1&&compareRestaurantProperty(arr[j-1], arr[j], sort); j--)
		{
			s=arr[j-1];
			arr[j-1]=arr[j];
			arr[j]=s;
		}
		
	}
	//Insertion Sort

	char temps[50], temps1[6000], temps2[10000];


	for(;!result.isActive();)
	{
		strcpy(temps1, "Select a restaurant");
		strcpy(temps2, "_h");
		for(i=0; i<=total-1; i++)
		{
			strcat(temps1, "\\");
			strcat(temps1, arr[i].getName(temps));
			strcat(temps2, "\\");
			switch(sort)
			{
				case 2	:	strcat(temps2, arr[i].getLocation(temps));
						break;
				case 3	:	strcat(temps2, arr[i].getType(temps));
						break;
				default	:	strcat(temps2, arr[i].getAddress(temps));
			} //The tip displayed on the bottom right depends on the criteria you decided to sort by.
		}
		result = arr[_makemenu(temps1, temps2, 1)-1];
	}
	return result;
}

user selectUser() //returns a user from the list of users. Returns guest account on failure.
{
	ifstream fin("users.dat", ios::in|ios::binary);
	user temp, arr[20];
	if(!fin)
		return temp; //if file not found, return temp, a guest account.
	int total, i;
	for(total=0; fin.read((char*)&temp, sizeof temp);) 
	{
		arr[total]=temp;
		total++;
	}
	fin.close();
	//total now contains the total number of users. arr contains all the users.
	
	char temps[10], temps1[200], temps2[100];
	strcpy(temps1, "Select a user");
	strcpy(temps2, "_h");
	for(i=0; i<=total-1; i++)
	{
		strcat(temps1, "\\");
		strcat(temps1, arr[i].getName(temps));
		strcat(temps2, "\\");
		switch(arr[i].getAccess())
		{
			case 0	:	strcat(temps2, "User");
					break;
			default	:	strcat(temps2, "Admin");
		}
	}
	temp = arr[_makemenu(temps1, temps2, 1)-1];
	return temp;
}



int addRestaurant(restaurant a, user you)
{
	if(!you.getAccess())
		return 1; //exits with error immidiately if the user does not have sufficient rights
	ofstream fout("rest.dat", ios::ate|ios::binary);
	if(!fout)
		return 2; //exits with error if unable to create file
	if(!a.isActive())
		return 3; //We dont run this business to store junk values >.>
	fout.write((char*)&a, sizeof a);
	fout.close();
	return 0;
}

int delRestaurant(restaurant a, user you)
{
	if(!you.getAccess())
		return 1; //exits with error immidiately if the user does not have sufficient rights
	ofstream fout("temp.dat", ios::out|ios::binary);
	ifstream fin("rest.dat", ios::in|ios::binary);
	if(!fin||!fout)
		return 2;
	restaurant temp;
	while(fin.read((char*)&temp, sizeof temp))
	{
		if( !a.cmp(&temp) && temp.isActive() ) //haath ke haath delete any non-active restraunts that somehow sneaked in.
			fout.write((char*)&temp, sizeof temp);
	}
	fin.close();
	fout.close();
	remove("rest.dat");
	rename("temp.dat", "rest.dat");
	return 0;
}

int addUser(user a, user you)
{
	if(!you.getAccess())
		return 1; //exits with error immidiately if the user does not have sufficient rights
	ofstream fout("users.dat", ios::ate|ios::binary);
	ifstream fin("users.dat", ios::in|ios::binary);
	if(!fout||!fin)
		return 2; //exits with error if unable to create file
	char temps[10];
	char temps2[10];	
	if(!strcmp(a.getName(temps), "guest"))
		return 4;
	user temp;
	while(fin.read((char*)&temp, sizeof temp))
	{
		if(!strcmp(a.getName(temps),temp.getName(temps2)))
			return 3; //username exists
	}
	fin.close();
	fout.write((char*)&a, sizeof a);
	fout.close();
	return 0;
}

int delUser(user a, user you)
{
	if(!you.getAccess())
		return 1; //exits with error immidiately if the user does not have sufficient rights
	ofstream fout("tempu.dat", ios::out|ios::binary);
	ifstream fin("users.dat", ios::in|ios::binary);
	if(!fin||!fout)
		return 2;
	user temp;
	char temps[10];
	char temps2[10];
	int flag=0;
	while(fin.read((char*)&temp, sizeof temp))
	{
		if(strcmp(a.getName(temps),temp.getName(temps2)))
			fout.write((char*)&temp, sizeof temp);
		else
			flag++;
	}
	fin.close();
	fout.close();
	remove("users.dat");
	rename("tempu.dat", "users.dat");
	if(!flag)
		return 3;
	return 0;
}

int editRestaurant(restaurant a, user you)
//Does not actually edit the restaurant stored in file. Instead, it replaces it with the revised version.
{
	if(!you.getAccess())
		return 1; //exits with error immidiately if the user does not have sufficient rights
	restaurant rest = a;
	int temp = _makemenu("Edit Restaurant\\Edit Name\\Edit Type\\Edit Address\\Edit Location\\Edit Phone Number\\Edit Rating\\Cancel", "_h\\Edit the name of the restaurant\\Edit the type of the restaurant\\Edit the address of the restaurant\\Edit the location of the restaurant\\Edit the phone number of the restaurant\\Edit the rating of the restaurant\\Go back");
	switch(temp)
	{
		case 7 :	return -1;
		default	:	rest = setValidValue(rest, temp);
	}
	delRestaurant(a, you);
	addRestaurant(rest, you);
	return 0;
}

/****************************************************
THE MAIN FUNCTION
****************************************************/

void main()
{
	user you;
	_popup("Welcome", "This program was created by Anirudh Katoch, student of Rajesh Sethi from The Heritage School Vasant Kunj as the project for the CBSE board 2013-2014");
	mainMenu(you);
}
