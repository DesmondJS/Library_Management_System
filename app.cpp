#include	<iostream>
#include	<cstdlib>
#include	<cstdio>
#include    <fstream>
#include    <string>
#include	"List.h"
#include    "LibStudent.h"
#include    "LibBook.h"
#include <iomanip>

using namespace std;

int calculateJulianDate(int, int, int);
bool ReadFile(string, List*);
bool DeleteRecord(List*, char*);
bool Display(List*, int, int);
bool InsertBook(string, List*);
bool SearchStudent(List*, char* id, LibStudent&);
bool computeAndDisplayStatistics(List*);
bool printStuWithSameBook(List*, char*);
bool displayWarnedStudent(List*, List*, List*);
int menu();
bool Redundant(List*, LibStudent);

int main() {
	List* stulist = new List();
	string filename;
	char a = 'Y';
	char stuID[10];
	LibStudent STU;
	char call[20];
	List* list1 = new List();
	List* list2 = new List();

	do {
		switch (menu()) {
		case 1: {
			filename = "student";
			if (!ReadFile(filename, stulist)) {
				cout << "Enable to read file." << endl;
			}

			break;
		}
		case 2: {

			if (!DeleteRecord(stulist, stuID))
			{
				cout << "This Student couldn't be found." << endl;
				cout << "Please enter 1 & 4 first before delete record !" << endl;//before select user need to select case 1&4 store record
			}
			break;
		}
		case 3: {

			if (!SearchStudent(stulist, stuID, STU))
			{
				cout << "This Student couldn't be found." << endl;
				cout << "Please enter 1 & 4 first before search student !" << endl;
			}

			break;
		}
		case 4: {
			string b = "book";
			if (!InsertBook(b, stulist))
				cout << "Unable to insert." << endl;
			break;
		}
		case 5: {
			int source, detail;
			cout << "Where do you want to display the output (1 - Screen / 2 - File): ";
			cin >> source;
			cout << "Do you want to display book list for every student (1 - YES / 2 - NO): ";
			cin >> detail;
			cout << endl;
			if (!Display(stulist, source, detail))
			{
				cout << "uncable to print." << endl;
				cout << "Please enter 1 and 4 first before display the output !" << endl;
			}
			break;
		}
		case 6: {
			if (!computeAndDisplayStatistics(stulist)) {
				cout << "Unable to compute." << endl;
				cout << "Please enter 1 and 4 first before compute and display statistics !" << endl;
			}
			break;
		}
		case 7: {

			cout << endl;
			if (!printStuWithSameBook(stulist, call)) {
				cout << "Unable to print." << endl;
				cout << "Please enter 1 and 4 first before print the student who has the same book !" << endl;
			}
			break;
		}
		case 8: {
			if (!displayWarnedStudent(stulist, list1, list2))
			{
				cout << "Unable to open Stulist." << endl;
				cout << "Please enter 1 and 4 first before display warned student !" << endl;
			}
			break;
		}

		case 9: {
			cout << "Program ended." << endl;
			
			return 0;
		}
		default:
			cout << "Please key in again." << endl << endl;

		}

	} while (a == 'Y');



	cout << "\n\n";
	system("pause");
	return 0;
}
int menu() {//menu to select choice
	int a;

	cout << "Please select your choice: " << endl;
	cout << "1. Read file " << endl;
	cout << "2. Delete record " << endl;
	cout << "3. Search student " << endl;
	cout << "4. Insert book " << endl;
	cout << "5. Display output" << endl;
	cout << "6. Compute and Display Statistics" << endl;
	cout << "7. Student with Same Book " << endl;
	cout << "8. Display Warned Student" << endl;
	cout << "9. Exit " << endl;
	cin >> a;
	cout << endl;

	return a;//return choice

}

bool ReadFile(string filename, List* list) {
	LibStudent student;
	ifstream infile;
	string a;
	int count = 0;

	infile.open(filename + ".txt");

	if (infile.fail())
		return false;

	while (!infile.eof()) {
		for (int h = 0; h < 3; h++)
			infile >> a; //to get the words infront the name
		infile >> student.id;
		for (int c = 0; c < 2; c++)
			infile >> a;
		infile.get();//to get the spacing
		infile.getline(student.name, 30);
		for (int d = 0; d < 2; d++)
			infile >> a;
		infile >> student.course;
		for (int e = 0; e < 3; e++)
			infile >> a;
		infile >> student.phone_no;

		if (!Redundant(list, student)) { //to check whether the value repeated
			list->insert(student);
		}
		else {

			cout << student.id << " is already save in Node." << endl;

		}

	}
	cout << "Data Saved." << endl;

	infile.close();

	return true;
}

bool Redundant(List* stulist1, LibStudent student) {
	Node* cur;

	cur = stulist1->head;
	while (cur != NULL)
	{
		if (cur->item.compareName2(student))
			return true;

		cur = cur->next;
	}
	return false;
}

bool DeleteRecord(List* stulist, char* ID) {//to call user to key in STU ID that they want to delete
	LibStudent temp;
	if (stulist->empty())
		return false;

	cout << "Please key in student ID that you want to delete: " << endl;
	cin >> ID; //call user to key in ID

	for (int i = 1; i <= stulist->size(); i++) {
		if (stulist->get(i, temp)) { //get value and compare
			if (strcmp(temp.id, ID) == 0) {
				stulist->remove(i);
				cout << "The data of student " << ID << " have been remove." << endl;
				return true;
			}

		}
	}
	return false;

}

bool SearchStudent(List* stulist, char* id, LibStudent& STU) { //call user to key in ID to find student 

	if (stulist->empty())
		return false;

	cout << "Please key in student ID of the student that you want to search:  " << endl;
	cin >> id;

	for (int i = 1; i <= stulist->size(); i++) {

		if (stulist->get(i, STU)) {
			if (strcmp(STU.id, id) == 0) {

				STU.print(cout);
				cout << endl;
				return true;
			}
		}
	}

	return false;
}

bool InsertBook(string filename, List* list)
{
	// LibBook to be inserted into student
	LibBook book;

	char id[10]; //to store ID and compare the studentID to insert in LibStudent
	int difference_day = 0;
	ifstream inFile;
	inFile.open(filename + ".txt");
	char hold[256]; //For holding the value that insert from the inFile
	const int current = 2458938;

	if (!inFile) // when the inFile not working return false
	{
		cout << "Unavailable to open the files" << endl;
		return false;
	}
	else
	{
		while (!inFile.eof()) //must scan untill the end of file
		{
			// ID
			inFile >> hold;
			strcpy(id, hold);

			// Author
			inFile >> hold; // Tony_Gaddis/Godfrey_Mugand
			string author_hold = "";
			int author_count = 0;
			for (int i = 0; i < strlen(hold); i++)
			{
				// Get author name
				if (hold[i] != '/')
				{
					author_hold += hold[i]; //let the author_hold to hold the item form hold
					if (i == strlen(hold) - 1) //is the last word of hold
					{
						// Remove '_'
						for (int j = 0; j < author_hold.length(); j++)
						{
							if (author_hold[j] == '_')
								author_hold[j] = ' ';

						}
						// Insert to LibBook
						book.author[author_count] = new char[author_hold.length()]; //creating a new array for book.author because multiple author in one book
						strcpy(book.author[author_count++], author_hold.c_str());  //inserting the author_hold into book.author

						author_hold = ""; // clear the author_hold
					}
					book.author[author_count] = NULL; //let the book.author in loop become NULL
				}
				else if (hold[i] == '/') // Finished 1 author//&&
				{
					// Remove '_'
					for (int j = 0; j < author_hold.length(); j++)
					{
						if (author_hold[j] == '_')
							author_hold[j] = ' ';

					}

					// Insert to LibBook
					book.author[author_count] = new char[author_hold.length()];
					strcpy(book.author[author_count++], author_hold.c_str());
					author_hold = "";
				}
			}

			// Title
			inFile >> hold;
			// Remove '_'
			for (int i = 0; i < strlen(hold); i++)
			{
				if (hold[i] == '_')
					hold[i] = ' ';
			}
			strcpy(book.title, hold); // Insert to LibBook

			// Publisher
			inFile >> hold;
			for (int i = 0; i < strlen(hold); i++) 		// Remove '_'
			{
				if (hold[i] == '_')
					hold[i] = ' ';
			}
			strcpy(book.publisher, hold); // Insert to LibBook

			// ISBN
			inFile >> hold;
			strcpy(book.ISBN, hold); // Insert to LibBook

			// yearPublished
			inFile >> hold;
			book.yearPublished = atoi(hold); // Insert to LibBook

			// callNum
			inFile >> hold;
			strcpy(book.callNum, hold); // Insert to LibBook

			string hold_date = "";
			int rotation = 0;
			// borrow (12/2/2020)
			Date borrow;
			inFile >> hold;
			for (int i = 0; i < strlen(hold); i++)
			{
				if (hold[i] != '/' && i != strlen(hold) - 1)
					hold_date += hold[i];
				else
				{
					switch (rotation)
					{
					case 0: //scan the value tht before first / and store as day
						borrow.day = stoi(hold_date);
						rotation++;
						break;

					case 1: // scan the value tht before second / which is month and store as borrow.month
						borrow.month = stoi(hold_date);
						rotation++;
						break;

					case 2: // scan the value tht after second / which is month and store as borrow.year
						hold_date += hold[i];
						borrow.year = stoi(hold_date);
						rotation = 0;
						break;
					}
					hold_date = "";
				}
			}
			book.borrow = borrow; // Insert to LibBook

			// due
			Date due;
			inFile >> hold;
			for (int i = 0; i < strlen(hold); i++)
			{
				if (hold[i] != '/' && i != strlen(hold) - 1)
					hold_date += hold[i];
				else
				{
					switch (rotation)
					{
					case 0:
						due.day = stoi(hold_date);
						rotation++;
						break;

					case 1:
						due.month = stoi(hold_date);
						rotation++;
						break;

					case 2:
						hold_date += hold[i];
						due.year = stoi(hold_date);
						rotation = 0;
						break;
					}
					hold_date = "";
				}
			}
			book.due = due; // Insert to LibBook


			// How to assigned the date into here
			int julianDate = calculateJulianDate(due.year, due.month, due.day);

			difference_day = current - julianDate; // we will get the current_date in julian form and julianDate in julian form

			if (difference_day > 0) // those with posititve will be count
			{
				book.fine = static_cast<double>(difference_day) * 0.50;

			}
			else
			{
				book.fine = 0.0;
			}

			// Insert to Specified Student
			LibStudent student;
			for (int i = 0; i < list->count; i++)
			{
				list->get(i + 1, student); //get the student 
				if (strcmp(student.id, id) == 0) // comparing the id and student id if match then go in,or else keep loop
				{
					student.book[student.totalbook++] = book; // defineing the book into the the array of student.book
					for (int j = 0; j < author_count; j++) {
						//cout << book.author[j] << endl;
						student.book[student.totalbook].author[j] = book.author[j]; // assigning multiple author into book total book
					}
					student.calculateTotalFine(); // insert the fine into the student total fine
					list->set(i + 1, student); // set into the student list
				}

			}

		}
		cout << "Successfully key in. " << endl << endl;
		inFile.close();
		return true;
	}

	return 0;

}

int calculateJulianDate(int year, int month, int day)
{

	// Adjust month and year for January and February
	if (month <= 2)
	{
		year--;
		month += 12;
	}

	// Calculate the Julian date formula
	double julianDate = (1461 * (year + 4800 + (month - 14) / 12)) / 4 +
		(367 * (month - 2 - 12 * ((month - 14) / 12))) / 12 -
		(3 * ((year + 4900 + (month - 14) / 12) / 100)) / 4 +
		day - 32075;

	return julianDate;
}

bool Display(List* list, int source, int detail) {
	ofstream outfile;
	ofstream outfile2;
	Node* temp = list->head;//traverse the node one by one

	outfile.open("stu_info.txt");
	outfile2.open("stu_booklist.txt");

	if (list->empty()) {
		return false;
	}
	if (source == 1 && detail == 1) {//print out book and student details in compiler
		for (int a = 1; temp != NULL; temp = temp->next, a++) {
			cout << "=====================================" << endl;
			cout << "Student " << a << endl;
			temp->item.print(cout);
			cout << endl;
			cout << "Book List: " << endl;
			for (int i = 0; i < temp->item.totalbook; i++) {
				cout << "----------------------------------" << endl;
				cout << "Book " << i + 1 << endl;
				temp->item.book[i].print(cout);
			}

		}
		cout << "Successfully display output." << endl;
		cout << endl;
		return true;

	}
	else if (source == 1 && detail == 2) {//print out student details only in compiler
		for (int a = 1; temp != NULL; temp = temp->next, a++) {
			cout << "=====================================" << endl;
			cout << "Student " << a << endl;
			temp->item.print(cout);
			cout << endl;
		}
		cout << "Successfuly display output." << endl;
		cout << endl;
		return true;
	}

	else if (source == 2 && detail == 1) {//print out student and book in outfile"stu_booklist.txt"
		for (int a = 1; temp != NULL; temp = temp->next, a++) {
			outfile2 << "==========================================" << endl;
			outfile2 << "Student " << a << endl;
			temp->item.print(outfile2);
			for (int i = 0; i < temp->item.totalbook; i++) {
				outfile2 << "----------------------------------------" << endl;
				outfile2 << "Book " << i + 1 << endl;
				temp->item.book[i].print(outfile2);
			}
		}
		cout << "Displayed in stu_booklist.txt." << endl;
		cout << endl;
		return true;
	}
	else if (source == 2 && detail == 2) {//print out student details only in outfile"stu_info.txt"
		for (int a = 1; temp != NULL; temp = temp->next, a++) {
			outfile << "-----------------------------" << endl;
			outfile << "Student " << a << endl;
			temp->item.print(outfile);
		}
		cout << "Displayed in stu_list.txt." << endl;
		cout << endl;
		return true;
	}

}

bool computeAndDisplayStatistics(List* list)
{
	int numStudents = 0;
	int totalBooksBorrowed = 0;
	int totalOverdueBooks = 0;
	double totalOverdueFine = 0.0;

	if (list == nullptr || list->empty())
	{
		return false;
	}
	cout << left << setw(7) << "Course"; 
	cout << setw(21) << "Number of Students";
	cout << setw(21) << "Total Books Borrowed";
	cout << setw(21) << "Total Overdue Books";
	cout << setw(21) << "Total Overdue Fine (RM)" << endl;

	string courses[] = { "CS","IA","IB","CN","CT" };
	int numCourses = sizeof(courses) / sizeof(courses[0]);

	for (int i = 0; i < numCourses; i++)
	{
		numStudents = 0; //number of student for that course
		totalBooksBorrowed = 0; //total books borrowed by student
		totalOverdueBooks = 0; //total overdue books by student
		totalOverdueFine = 0.0; //total fine of student

		Node* temp = list->find(0);
		while (temp != nullptr)
		{
			if (temp->item.course == courses[i])
			{
				numStudents++;
				totalBooksBorrowed += temp->item.totalbook;
				for (int j = 0; j < temp->item.totalbook; j++) {
					if (temp->item.book[j].fine > 0)
						totalOverdueBooks++;
				}
				totalOverdueFine += temp->item.total_fine;
			}
			temp = temp->next;
		}
		cout << setw(10) << courses[i];
		cout << setw(25) << numStudents;
		cout << setw(23) << totalBooksBorrowed;
		cout << setw(23) << totalOverdueBooks;
		cout << setw(23) << fixed << setprecision(2) << totalOverdueFine << endl;
	}
	return true;
}

bool printStuWithSameBook(List* list, char* callNum)
{
	LibStudent stu;
	int count = 0;

	cout << "please key in callNum that you want to find: ";
	cin >> callNum;
	cout << endl;

	if (list->empty())
	{
		cout << "The booklist or stulist are empty" << endl;
		return false;
	}

	for (int i = 1; i <= list->size(); i++)
	{
		if (list->get(i, stu))
		{
			for (int a = 0; a < stu.totalbook; a++) {
				if (strcmp(stu.book[a].callNum, callNum) == 0)//compare callNum
				{
					count = count + 1;
				}
			}

		}

	}
	cout << "There are " << count << " students that you call with " << callNum << " below: " << endl << endl;
	for (int j = 1; j <= list->size(); j++) {
		if (list->get(j, stu)) {
			for (int b = 0; b < stu.totalbook; b++) {
				if (strcmp(stu.book[b].callNum, callNum) == 0) {//if callNum is same after compare, cout stu.info & date

					cout << "Student Id = " << stu.id << endl;
					cout << "Name = " << stu.name << endl;
					cout << "Course = " << stu.course << endl;
					cout << "Phone Number = " << stu.phone_no << endl;
					cout << "Borrow Date: " << stu.book->borrow.day << "/" << stu.book->borrow.month << "/" << stu.book->borrow.year << endl;
					cout << "Due Date: " << stu.book->due.day << "/" << stu.book->due.month << "/" << stu.book->due.year << endl;
					cout << endl;

				}
			}
		}
	}

	return true;
}

bool displayWarnedStudent(List* list, List* type1, List* type2)
{
	LibStudent stu;
	LibBook book;
	Date due;
	Node* cur = list->head;
	int countOverduebook = 0;
	int difference_day;
	int tenDaysDue = 0;
	const int current = 2458938;


	if (list->empty())
	{
		cout << "The list is empty" << endl;
		return false;
	}

	else
	{
		stu = cur->item;
		for (int i = 1; i <= list->size(); i++)
		{
			if (list->get(i, stu))
			{
				countOverduebook = 0;
				tenDaysDue = 0;
				for (int i = 0; i < stu.totalbook; i++)
				{
					int julianDate = calculateJulianDate(stu.book[i].due.year, stu.book[i].due.month, stu.book[i].due.day);
					difference_day = current - julianDate;
					if (difference_day > 0)//use difference_day to check the book overdue
					{
						countOverduebook = countOverduebook + 1;
					}

					if (difference_day >= 10) //check the difference days more than 10 days
					{
						tenDaysDue++;
					}
				}
				if (tenDaysDue > 2) //check the book is more than 2 that are overdue for >=10 days
				{
					type1->insert(stu);
				}

				if (stu.total_fine > 50.00 && stu.totalbook == countOverduebook)
				{
					type2->insert(stu);
				}

			}
			//cur->item.print(cout); //to check the item of node
			cur = cur->next;
		}

		cout << "The student who borrow > 2 books that are overdue for >= 10 days has been warned." << endl;
		if (!type1->empty())//if not empty will cout stu.info & book
		{
			for (int z = 0; z < type1->size(); z++)
			{
				cout << "============================================================================" << endl;
				cout << "Student " << z + 1 << endl;
				cout << "----------------------------------------------------------------------------" << endl;

				cur = type1->head;
				cur->item.print(cout);

				for (int i = 0; i < cur->item.totalbook; i++)
				{
					cout << "\n\nBook " << i + 1 << endl;
					cout << "------------------------------------------------------------------------" << endl;
					cur->item.book[i].print(cout);
				}
				cur = cur->next;
			}
		}
		else
		{
			cout << "List type1 is empty" << endl;
		}


		cout << "\n\nThe student whose total fine > RM50.00 and every book is overdue has been warned." << endl;
		if (!type2->empty())//if not empty will cout stu.info & book
		{
			for (int k = 0; k < type2->size(); k++)
			{
				cout << "==============================================================================" << endl;
				cout << "Student " << k + 1 << endl;
				cout << "------------------------------------------------------------------------------" << endl;

				cur = type2->head;
				cur->item.print(cout);

				for (int j = 0; j < cur->item.totalbook; j++)
				{
					cout << "\n\nBook " << j + 1 << endl;
					cout << "--------------------------------------------------------------------------" << endl;
					cur->item.book[j].print(cout);
				}
				cur = cur->next;
			}
		}
		else
		{
			cout << "List type2 is empty" << endl << endl;
		}
	}

	return true;
}



