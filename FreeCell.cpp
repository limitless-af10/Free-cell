// Project6.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<fstream>
#include<cstdlib>
#include<time.h>
#include<algorithm>
#include<string>
using namespace::std;

class game
{
	struct card
	{
		char color;   
		char suit;   
		char rank;

		card()
		{
			color = ' ';
			suit = ' ';
			rank = ' ';
		}
	};

	card * deck;
	card ** columns;
	card ** home;
	card * free_cell;
	int column_size[8];
	int home_size[4];
	string load = "false";


	void growsize(card **& array, int size, int size_ ,int no_move)   //function to grow array size
	{
		card** temp = new card*[size_]; //temp 2d array with same no of rows

		for (int i = 0; i < size_; i++)
		{
			//each 2d columns is given +no_move+1 more rows
			temp[i] = new card[size + no_move+1];
		}

		//copying original array's data to temp array
		for (int i = 0; i < size_; i++)
		{
			for (int j = 0; j < size ; j++)
			{
				temp[i][j] = array[i][j];
				
			}

		}

		delete [] array;

		//reassigning temp back to array
		array = temp;
	}

	void disp_deck(card** array, bool c, int size)
	{     
		int x;

		if (c)
			x = 4;

		else
			x = 8;
		
		for(int i=0;i<size;i++)
		{
			//if (c)
				//cout << "_______     _______     _______     _______";
			
			if(!c)
				cout << "\n _______     _______     _______     _______     _______     _______     _______     _______\n";

			for (int j = 0; j < x; j++)
			{
				cout << "| " << array[j][i].suit; 
				
				if (array[j][i].rank >= 49 && array[j][i].rank <= 57)
				{
					cout << (array[j][i].rank) - 47;
				}

				else
					cout << array[j][i].rank;
				
				cout<< array[j][i].color << "   ";
				
				if ((array[j][i].rank) - 47 == 10)
					cout << "\b";
				
				cout << "|   ";
			
			}

			if (c)
			cout<<"\n|       |   |       |   |       |   |       |   ";
			
		}
	}

	void disp()
	{
		cout << "*****FREE CELL Game*****" << endl<<endl<<endl;
		cout << "              { Free Cell }                                         { Home }                           ";

		cout << "\n _______     _______     _______     _______     _______     _______     _______     _______\n";

		for (int i = 0; i < 4; i++)
		{
			cout << "| " << free_cell[i].suit;

			if (free_cell[i].rank >= 49 && free_cell[i].rank <= 57)
			{
				cout << (free_cell[i].rank) - 47;
			}

			else
				cout << free_cell[i].rank;

			cout << free_cell[i].color << "   ";

			if ((free_cell[i].rank) - 47 == 10)
				cout << "\b";

			cout << "|   ";
		}

		disp_deck(home,1,*max_element(home_size,home_size+4));  
		

		cout << endl << endl;

		cout << "\n _______     _______     _______     _______     _______     _______     _______     _______\n";

		
		disp_deck(columns,0,*max_element(column_size,column_size+8)); cout << "  ";

		cout << endl << endl;		
	}

	int rank_assign(char temp) //function to assign ranks to cards
	{
		if (temp == 'K')
			return 12;

		if (temp == 'Q')
			return 11;

		if (temp == 'J')
			return 10;

		if (temp == 'A')
			return 0;

		else return temp - 48;
	}

	void column_column()
	{
		int source, destination, no_move;
		
		cout << "Enter the column number as source: ";
		cin >> source;

		cout << "Enter the column number as destination: ";
		cin >> destination;

		cout << "Enter the no of items to move: ";
		cin >> no_move;

		source-- ;
		destination-- ;

		int index_s = column_size[source] - no_move;   //index of source column

		//the following loop checks if weather the multiple cards selected to move are of alternating color
		char color_check = columns[source][index_s].color;

		for (int i = index_s; i < column_size[source]; i++)
		{
			if (columns[source][i].color != color_check)
			{
				cout << "the multiple cards are not of alternating color";
				return;
			}

			if (color_check == 'b')
				color_check = 'r';

			else if (color_check == 'r')
				color_check = 'b';
        }


		//loop to check if the multiple selected cards are sorted
		for (int i = index_s ; i < column_size[source]-1; i++)
		{
			if (rank_assign(columns[source][i].rank) != rank_assign(columns[source][i+1].rank)+1)
			{
				cout << "The Cards you tried to move are not sorted in immediate descending order";
				return;
			}
			
		}

		int index_d = column_size[destination] - 1; //index of last element of destination column

		//condition for checking that the destination and top source card are of alternating color
		if (columns[source][index_s].color == columns[destination][index_d].color)
		{
			cout << "The Card you tried to move is not of alternating color";
			return;
		}

		//condition to check if top source card is immediate descendent of destiantion card
		if (columns[destination][0].color != ' ')
		{
			if (rank_assign(columns[source][index_s].rank) != rank_assign(columns[destination][index_d].rank) - 1)
			{
				cout << "The Card you tried to move was not immediate descendent of destination";
				return;
			}
		}
		
		

		//condition to check if the destination array size needs to be increased
		if (*max_element(column_size,column_size+7)< column_size[destination] + no_move)
		{
			//function call to grow array size
			growsize(columns, column_size[destination], 8, no_move);
			
		}

		//loop to move cards/card from source to destination
		for (int z=0, i=column_size[destination],j=index_s ; z < no_move ; i++,j++,z++)
		{
			columns[destination][i].color = columns[source][j].color;
			columns[destination][i].rank = columns[source][j].rank;
			columns[destination][i].suit = columns[source][j].suit;

			columns[source][j].color = ' ';
			columns[source][j].rank = ' ';
			columns[source][j].suit = ' ';

			column_size[destination] = column_size[destination] + no_move;
			column_size[source] = column_size[source] - no_move;

		}

	}

	void column_free()
	{
		int source, cell;
		
		cout << "Enter source column to move card: ";
		cin >> source;

		cout << "Enter free cell slot to move to: ";
		cin >> cell;

		source--;
		cell--;

		//condition to check if user is trying to move 'A' card to free cell
		if (columns[source][column_size[source] - 1].rank == 'A')
		{
			cout << "cant move ace card to free cell";
			return;
		}

		//condition to check if destination free cell is empty
		if (free_cell[cell].color != ' ')
		{
			cout << "Given free cell not empty";
			return;
		}

		else //moving column card to free cell
		{
			free_cell[cell].color = columns[source][column_size[source] -1].color;
			free_cell[cell].rank = columns[source][column_size[source] - 1].rank;
			free_cell[cell].suit = columns[source][column_size[source] - 1].suit;

			columns[source][column_size[source] - 1].color = ' ';
			columns[source][column_size[source] - 1].rank = ' ';
			columns[source][column_size[source] - 1].suit = ' ';

			column_size[source] = column_size[source] - 1;
		}
	}

	void free_cloumn()
	{
		int source, destination;
		
		cout << "\nEnter source free cell: ";
		cin >> source;

		cout << "\nEnter destination column: ";
		cin >> destination;

		source--;
		destination--;

		//to check if free cell is empty
		//the function is returned in case user tries to move from an empty column
		if (free_cell[source].color == ' ')
		{
			cout << "the source free cell is empty";
			return;
		}

		//condition to check if the free cell card is of alternating color to top destination column card
		else if (free_cell[source].color == columns[destination][column_size[destination] - 1].color)
		{
			cout << "The Card you tried to move is not of alternating color";
			return;
		}

		//condition to check if the free cell card is immediate descendent of top destination column card
		else if (columns[destination][0].color !=' '  && 
			rank_assign(free_cell[source].rank) != rank_assign(columns[destination][column_size[destination] - 1].rank) - 1)
		{
			cout << "The Card you tried to move was not immediate descendent of destination";
			return;
		}

		else
		{
			//condition to check if the destination array size needs to be increased
			if (*max_element(column_size, column_size + 7) < column_size[destination] + 1)
			{
				//function call to grow array size
				growsize(columns, column_size[destination], 8, 1);

			}
			
			
			//adding free cell card to destination column
			columns[destination][column_size[destination]].color = free_cell[source].color;
			columns[destination][column_size[destination]].rank= free_cell[source].rank;;
			columns[destination][column_size[destination]].suit = free_cell[source].suit;

			free_cell[source].color = ' ';
			free_cell[source].rank = ' ';
			free_cell[source].suit = ' ';

			column_size[destination] = column_size[destination] + 1;

		}

	}

	void column_home()
	{
		int source, home_cell, no_move;

		cout << "Enter source column: ";
		cin >> source;

		cout << "Enter home destination cell: ";
		cin >> home_cell;

		cout << "No of cards to move";
		cin >> no_move;

		source--;
		home_cell--;

		int index_s = column_size[source] - no_move;

		//to check if the top card of source is A when cell of home is empty
		if (home[home_cell][0].color == ' '&& columns[source][index_s].rank != 'A')
		{
			cout << "First card on home cell should be of rank A";
			return;
		}

		
		if (home[home_cell][0].color != ' ')
		{
			//to check if source card is immediate ascendent of top home card
			if (columns[source][index_s].rank != 'A'&& rank_assign(columns[source][index_s].rank) != rank_assign(home[home_cell][home_size[home_cell] - 1].rank) + 1)
			{
				cout << "the source card is not immediate ascendent of last home card";
				return;
			}
			
			//to check if all cards of source are of same suit and color and also match the home cell's color, suit
			for (int i = index_s; i < column_size[source]; i++)
			{
				if (columns[source][i].color != home[home_cell][0].color || columns[source][i].suit != home[home_cell][0].suit)
				{
					cout << "\nThe Cards you tried to move are not of the same suit and color to the home cell card or each other";
					return;
				}

			}
		}
		

		//to check if the multiple cards of source are sorted
		for (int i = index_s ; i < column_size[source] - 1; i++)
		{
			if (rank_assign(columns[source][i].rank) != (rank_assign(columns[source][i+1].rank)-1) )
			{
				cout << "\nThe Cards you tried to move are not sorted";
				return;
			}
			
		}

		//to check if the home cell array has enough space for source cards
		if (*max_element(home_size, home_size + 3) < home_size[home_cell] + no_move)
		{
			//to grow the size of home cell array
			growsize(home, home_size[home_cell], 4, no_move);
			
		}

		//loop to move the source cards/card to home cell
		for (int z = 0, i = home_size[home_cell], j = column_size[source] - no_move; z < no_move; i++, j++, z++)
		{

			home[home_cell][i].color = columns[source][j].color;
			home[home_cell][i].rank = columns[source][j].rank;
			home[home_cell][i].suit = columns[source][j].suit;


			columns[source][j].color = ' ';
			columns[source][j].rank = ' ';
			columns[source][j].suit = ' ';
			
			home_size[home_cell] = home_size[home_cell] + no_move;
			column_size[source] = column_size[source] - no_move;

		}

	}

	void free_home()
	{
		int source, home_cell;

		cout << "\nEnter source free cell: ";
		cin >> source;

		cout << "\nEnter destination home column: ";
		cin >> home_cell;

		//to check if free cell is empty
		//the function is returned in case user tries to move from an empty column
		if (free_cell[source - 1].color == ' ')
		{
			cout << "the source free cell is empty";
			return;
		}

		//to check if user tries to put a non 'A' card on empty home cell
		if (home[home_cell - 1][0].color == ' '&& free_cell[source - 1].rank != 'A')
		{
			cout << "First card on home cell should be of rank A";
			return;
		}

		if (home[home_cell - 1][0].color != ' ')
		{
			//condition to check if the free cell card is immediate ascendent of top most home cell card
			if (free_cell[source - 1].rank != 'A'&&rank_assign(free_cell[source - 1].rank) != rank_assign(home[home_cell - 1][home_size[home_cell - 1]-1].rank) + 1)
			{
				cout << "the source card is not immediate ascendent of last home card";
				return;
			}
		}
	
		//condition to check if free cell card is the same color and suit of the home cell cards
		if (home[home_cell-1][0].color!=' ' && free_cell[source - 1].color != home[home_cell - 1][0].color || free_cell[source - 1].suit != home[home_cell - 1][0].suit)
		{
			cout << "The Card suit and color do not match";
			return;
		}


		//to check if the home cell array has enough space for source cards
		if (*max_element(home_size, home_size + 3) < home_size[home_cell - 1] + 1)
		{
			//to grow the size of home cell array
			growsize(home, home_size[home_cell - 1], 4, 1);

		}

		//adding free cell card to home cell card after all rules fullfilled
		home[home_cell - 1][home_size[home_cell - 1]].color = free_cell[source - 1].color;
		home[home_cell - 1][home_size[home_cell - 1]].rank = free_cell[source - 1].rank;;
		home[home_cell - 1][home_size[home_cell - 1]].suit = free_cell[source - 1].suit;

		free_cell[source - 1].color = ' ';
		free_cell[source - 1].rank = ' ';
		free_cell[source - 1].suit = ' ';

		home_size[home_cell - 1] = home_size[home_cell - 1] + 1;

	}

	void save()
	{
		string name;

		if (this->load == "false")
		{
			cout << "\n Enter file name to save to";
			cout << "\n Name: ";
			cin >> name;
			name = name + ".txt";
		}

		else
			name = load;

		ofstream fout;

		fout.open(name);

		for (int i = 0; i < 4; i++)
		{
			fout << this->free_cell[i].color << this->free_cell[i].rank << this->free_cell[i].suit;
		}

		fout << endl;

		for (int i = 0; i < 4; i++)
		{
			fout << home_size[i]<<" ";
		}

		fout << endl;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < *max_element(home_size, home_size + 3); j++)
			{
				fout << home[i][j].color<< home[i][j].rank << home[i][j].suit ;
			}

			fout << endl;
		}

		for (int i = 0; i < 8; i++)
		{
			fout << column_size[i]<<" ";
		}

		fout << endl;

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < *max_element(column_size, column_size + 7); j++)
			{
				fout << columns[i][j].color << columns[i][j].rank << columns[i][j].suit;
			}

			fout << endl;
		}

	}

	bool check_win()
	{
		for (int i = 0; i < 4; i++)
		{
			if (home_size[i] != 13)
				return 0;
		}

		return 1;
	}

public:

	~game()
	{
		delete[] free_cell;
		delete[] home;
		delete[] columns;

	}

	game() //default constructor
	{
		deck = new card[52];

		for (int i = 0; i < 8; i++)
		{
			int x = 7;

			if (i > 3)
				x = 6;

			column_size[i] = x;
		}

		for (int i = 0; i < 4; i++)
		{
			home_size[i] = 0;
		}

		columns = new card*[8];
		for (int i = 0; i < 8; i++)
		{
			columns[i] = new card[7];
		}

		home = new card*[4];
		for (int i = 0; i < 4; i++)
		{
			home[i] = new card[1];
		}

		free_cell = new card[4];

		char colour;
		char suit;

		for (int i = 0, j = 0; i < 52; i++)
		{
			if (i < 13)
			{
				colour = 'r';
				suit = 'h';
			}

			else if (i >= 13 && i < 26)
			{
				suit = 'd';
			}

			else if (i >= 26 && i < 39)
			{
				colour = 'b';
				suit = 's';
			}

			else if (i >= 39 && i < 52)
			{
				suit = 'c';
			}

			if (j == 12)
			{
				deck[i].rank = 'K';
			}

			else if (j == 11)
			{
				deck[i].rank = 'Q';
			}

			else if (j == 10)
			{
				deck[i].rank = 'J';
			}

			else if (j == 0)
			{
				deck[i].rank = 'A';
			}

			else
			{
				deck[i].rank = (char)j + 48;
			}

			j = (j + 1) % 13;

			deck[i].color = colour;
			deck[i].suit = suit;

		}

		int index_count = 0;
		int rand_arr[52];
		bool repeat = false;

		srand(time(NULL));

		while (true)
		{
			repeat = false;

			int random;
			random = rand() % 52;
			//cout << random;

			for (int i = 0; i < index_count; i++)
			{
				if (random == rand_arr[i])
				{
					//cout << random;
					//system("pause");
					repeat = true;
					break;
				}
			}

			if (repeat == true)
				continue;

			rand_arr[index_count] = random;

			if (index_count == 51)
				break;

			else
				index_count++;
		}

		index_count = 0;


		for (int i = 0; i < 8; i++)
		{
			int bs = 0;

			if (i > 3)
				bs = 1;


			for (int j = 0; j < 7 - bs; j++)
			{
				columns[i][j].color = deck[rand_arr[index_count]].color;
				columns[i][j].suit = deck[rand_arr[index_count]].suit;
				columns[i][j].rank = deck[rand_arr[index_count]].rank;

				index_count++;
			}
		}


		for (int i = 0; i < 8; i++)
		{
			int bs = 0;

			if (i > 3)
				bs = 1;

			for (int j = 0; j < 7 - bs; j++)
			{
				cout << columns[i][j].color;
				cout << columns[i][j].rank;
				cout << columns[i][j].suit;
			}

			cout << endl;
		}

	}

	game(string name)
	{
		string name_ = name;

		columns = new card*[8];
		for (int i = 0; i < 8; i++)
		{
			columns[i] = new card[7];
		}

		home = new card*[4];
		for (int i = 0; i < 4; i++)
		{
			home[i] = new card[1];
		}

		free_cell = new card[4];

		while (true)
		{
			name_ = name_ + ".txt";

			ifstream fin(name_);

			if (fin.fail())
			{
				cout << "\n wrong file name (the name of file you entered does not exist)";
				cout << "\n Enter file name again: ";
				cin >> name_;
				continue;
			}

			else
			{
				for (int i = 0; i < 4; i++)
				{
					fin.get(this->free_cell[i].color);
					fin.get(this->free_cell[i].rank);
					fin.get(this->free_cell[i].suit);
				}

				fin.ignore();

				for (int i = 0; i < 4; i++)
				{
					fin >> home_size[i];
				}

				fin.ignore();
				fin.ignore();


				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < *max_element(home_size, home_size + 3); j++)
					{

						fin.get(home[i][j].color);
						fin.get(home[i][j].rank);
						fin.get(home[i][j].suit);

					}

					fin.ignore();

				}

				for (int i = 0; i < 8; i++)
				{
					fin >> column_size[i];
				}

				fin.ignore();
				fin.ignore();


				for (int i = 0; i < 8; i++)
				{
					for (int j = 0; j < *max_element(column_size, column_size + 7); j++)
					{

						fin.get(columns[i][j].color);
						fin.get(columns[i][j].rank);
						fin.get(columns[i][j].suit);
					}

					fin.ignore();

				}

				this->load = name_;
				break;
			}
		}

	}

	void menu()
	{
		int option;

		do
		{
			system("cls");

			this->disp();

			cout << "\n Enter 1 for column to column move";
			cout << "\n Enter 2 for column to free cell move";
			cout << "\n Enter 3 for free cell to column move";
			cout << "\n Enter 4 for for column to home move";
			cout << "\n Enter 5 for free cell to home move";
			cout << "\n Enter 6 to save and exit";
			cout << "\n option: ";
			cin >> option;

			if (option == 1)
			{
				this->column_column();

				cout << endl << endl << " ";
				system("pause");
			}

			else if (option == 2)
			{
				this->column_free();

				cout << endl << endl << " ";
				system("pause");

			}

			else if (option == 3)
			{
				this->free_cloumn();

				cout << endl << endl << " ";
				system("pause");
			}

			else if (option == 4)
			{
				this->column_home();

				if (this->check_win())
				{
					cout << "\n You Won";
					option = 6;
				}

				cout << endl << endl << " ";
				system("pause");
			}

			else if (option == 5)
			{
				this->free_home();

				if (this->check_win())
				{
					cout << "\n You Won";
					option = 6;
				}

				cout << endl << endl << " ";
				system("pause");
			}

			else if (option == 6)
			{
				this->save();
			}

		} while (option != 6);
	}

};

int main() //main funtion of the program
{
	int option;

	cout << "\n Enter 1 to start new game";
	cout << "\n Enter 2 to load saved game";
	cout << "\n option: ";
	cin >> option;

	if (option == 1)
	{
		game g1;
		g1.menu();
	}

	else if(option==2)
	{
		string name;

		cout << "\n Enter name of game file: ";
		cin >> name;
		game g1(name);
		g1.menu();
	}
	
}
