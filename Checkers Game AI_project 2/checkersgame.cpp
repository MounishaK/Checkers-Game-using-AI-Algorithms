#include <iostream>
#include <vector>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>

using namespace std;

int choice,play_choice;
int algo_choice1,algo_choice2,eval_choice1,eval_choice2;
int moveCount = 0;
int NodeCount = 0, NC1 = 0;
int NodeExpanded = 0;
int options;
clock_t start, endd;

//int wcount4 = 0,rcount4 = 0;
//int wcount3 = 0,rcount3 = 0;
//int wcount2 = 0,rcount2 = 0;
static int wcount = 0,rcount = 0;


class Node
{
public:

	int Eval1();
	int Eval2();
	int Eval3();
	int Eval4();
	char board[8][8],player;
	vector<Node>childs;
	int v;
	int move;
	void setBoard(char b[][8]);
	void displayBoard();
	void setPlayer(char);
	void printActions();
	void MoveGen();
	int  utility();

	bool check_King(int row,char player);
	bool left_End(char board[][8],int row,int col,char player);
	bool compare_Matrix(char [][8],char [][8]);
	bool check_Left(char board[][8],int row,int col,char player);
	bool right_End(char board[][8],int row,int col,char player);
	bool check_Right(char board[][8],int row,int col,char player);

	void king_jump(char [][8],int,int,char);
	void jump(char [][8],int,int,char);
	void jump_left(char newboard[][8],int row,int col,char player,int &,int &);
	void jump_right(char newboard[][8],int row,int col,char player,int &,int &);
	void king_jump_left(char newboard[][8],int row,int col,char player,int &crow,int &ccol);
	void king_top_left(char newboard[][8],int row,int col,char player,int &crow,int &ccol);
	void king_jump_right(char newboard[][8],int row,int col,char player,int &crow,int &ccol);
	void king_top_right(char newboard[][8],int row,int col,char player,int &crow,int &ccol);

	bool king_left_end(char board[][8],int row,int col,char player);
	bool king_top_left_end(char board[][8],int row,int col,char player);
	bool king_right_end(char board[][8],int row,int col,char player);
	bool king_top_right_end(char board[][8],int row,int col,char player);
	bool terminal();
};

vector<Node> bestPath;
void alpha_beta_search(Node n,char [][8]);
int max_value(Node &n, int alpha, int beta);
int min_value(Node &n, int alpha, int beta);
void printBoard(char s[][8]);


//For setting color
void SetColor(int ForgC)
{
     WORD wColor;
     //This handle is needed to get the current background attribute

     HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
     CONSOLE_SCREEN_BUFFER_INFO csbi;
     //csbi is used for wAttributes word

     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
          //To mask out all but the background attribute, and to add the color
          wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
          SetConsoleTextAttribute(hStdOut, wColor);
     }
     return;
}

int Node::utility()
{   int vval;
	if(options==1)
	{
		if(childs.size() == 0)
		{
			if(player == 'R')
				vval= -20;
			else
				vval= 20;
		}
	}
	else if(options==2)
	{
		if(childs.size() == 0)
		{
			if(player == 'R')
				vval= 20;
			else
				vval= -20;
		}
	}
	/* if(eval_choice1==1 || eval_choice2==1)
	    return Eval1();
      else if(eval_choice1==2 || eval_choice2==2)
        return Eval2();
      else if(eval_choice1==3 || eval_choice2==3)
        return Eval3();
    else if(eval_choice1==4 || eval_choice2==4)
        return Eval4();*/
        return vval;
}

bool Node::terminal()
{
	if(childs.size() == 0)
		return true;
	if(move >= 5)
	{
		return true;
	}
	return false;
}

void Node::printActions()
{
	if(childs.size() == 0)
	{
        //
	}
	else
	{
		cout<<endl<<"***************************************"<<endl;
		cout<<"Printing Actions Start";
		cout<<endl<<"***************************************"<<endl;
		for(int i = 0;i < childs.size();i++)
		{
			childs[i].displayBoard();
		}
		cout<<endl<<"***************************************"<<endl;
		cout<<"Printing Actions End";
		cout<<endl<<"***************************************"<<endl;
	}
}

void Node::displayBoard()
{
	cout<<endl<<"********************"<<endl;
	for(int i = 0;i < 8; i++)
	{
		if(i == 0)
		{
			cout<<"-------------------"<<endl;
			cout<<"R/C 0 1 2 3 4 5 6 7"<<endl;
			cout<<"-------------------";
			cout<<endl;
		}
		cout<<" |"<<i<<"|";
		for(int j = 0;j < 8;j++)
		{
			cout<<board[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl<<"********************"<<endl;
}

void Node::setBoard(char b[][8])
{
	for(int i=0;i<8;i++)
	{
		for(int j=0;j<8;j++)
		{
			board[i][j] = b[i][j];
		}
	}
}

void Node::setPlayer(char p)
{
	player = p;
}

bool Node::check_King(int row,char player)
{
	if(player == 'R' && row == 7)
	{
			return true;
	}
	if(player == 'W' && row == 0)
	{
		return true;
	}
	return false;
}

bool Node::left_End(char board[][8],int row,int col,char player)
/*
Returns true if you cannot make a left jump from the current
position (row,col) else false
*/
{
	int nrow,ncol;
	if(player == 'R')
	{
		nrow = row + 2;
		ncol = col - 2;
		if((nrow >= 0 && nrow <= 7 && ncol >= 0 && ncol <= 7 && board[nrow][ncol] == '_'))
		{
			if(board[nrow-1][ncol+1] == 'W' || board[nrow-1][ncol+1] == 'Z')
			{
				return false;
			}
		}
	}
	else if(player == 'W')
	{
		nrow = row - 2;
		ncol = col - 2;
		if((nrow >= 0 && nrow <= 7 && ncol >= 0 && ncol <= 7 && board[nrow][ncol] == '_'))
		{
			if(board[nrow+1][ncol+1] == 'R' || board[nrow+1][ncol+1] == 'S')
			{
				return false;
			}
		}
	}
	return true;
}

bool Node::right_End(char board[][8],int row,int col,char player)
/*
Returns true if you cannot make a right jump from the current
position (row,col) else false
*/
{
	int nrow,ncol;
	if(player == 'R')
	{
		nrow = row + 2;
		ncol = col + 2;
		if((nrow >= 0 && nrow <= 7 && ncol >= 0 && ncol <= 7 && board[nrow][ncol] == '_'))
		{
			if(board[nrow-1][ncol-1] == 'W' || board[nrow-1][ncol-1] == 'Z')
			{
				return false;
			}
		}
	}
	else if(player == 'W')
	{
		nrow = row - 2;
		ncol = col + 2;
		if((nrow >= 0 && nrow <= 7 && ncol >= 0 && ncol <= 7 && board[nrow][ncol] == '_'))
		{
			if(board[nrow+1][ncol-1] == 'R' || board[nrow+1][ncol-1] == 'S')
			{
				return false;
			}
		}
	}
	return true;
}

void Node::MoveGen()
{
	for(int i = 0;i < 8;i++)
	{
		for(int j = 0;j < 8;j++)
		{
			if(board[i][j] != '_')
			{
				if(player == 'R' && board[i][j] == 'R')//Computer normal player
				{

					char newboard[8][8];
					copy(&board[0][0],&board[0][0]+8*8,&newboard[0][0]);


					if(check_Left(board,i,j,player))
					{
						//I can move left
						if(check_King(i+1,player))
							newboard[i+1][j-1] = 'S';
						else
							newboard[i+1][j-1] = 'R';
						newboard[i][j] = '_';
						Node child1;
						child1.setBoard(newboard);
						char newplayer = (player == 'R')?'W':'R';
						child1.setPlayer(newplayer);
						NodeExpanded++;
						child1.move = move+1;
						childs.push_back(child1);
						NodeCount++; NC1++;
						copy(&board[0][0],&board[0][0]+8*8,&newboard[0][0]);
					}
					//Check right diagnoal
					if(check_Right(board,i,j,player))
					{
						//I can move right
						if(check_King(i+1,player))
							newboard[i+1][j+1] = 'S';
						else
							newboard[i+1][j+1] = 'R';
						newboard[i][j] = '_';
						Node child1;
						child1.setBoard(newboard);
						char newplayer = (player == 'R')?'W':'R';
						child1.setPlayer(newplayer);
						NodeExpanded++;
						child1.move = move+1;
						childs.push_back(child1);
						NodeCount++; NC1++;
						copy(&board[0][0],&board[0][0]+8*8,&newboard[0][0]);
					}
					//Check jumps
					jump(board,i,j,player);
				}
				else if(player == 'W' && board[i][j] == 'W') //user normal player
				{

					char newboard[8][8];
					copy(&board[0][0],&board[0][0]+8*8,&newboard[0][0]);

					if(check_Left(board,i,j,player))
					{
						//I can move left
						if(check_King(i-1,player))
							newboard[i-1][j-1] = 'Z';
						else
							newboard[i-1][j-1] = 'W';
						newboard[i][j] = '_';
						Node child1;
						child1.setBoard(newboard);
						char newplayer = (player == 'R')?'W':'R';
						child1.setPlayer(newplayer);
						NodeExpanded++;
						child1.move = move+1;
						childs.push_back(child1);
						NodeCount++; NC1++;
						copy(&board[0][0],&board[0][0]+8*8,&newboard[0][0]);
					}
					//Check right diagnoal
					if(check_Right(board,i,j,player))
					{
						//I can move right
						if(check_King(i-1,player))
							newboard[i-1][j+1] = 'Z';
						else
							newboard[i-1][j+1] = 'W';
						newboard[i][j] = '_';
						Node child1;
						child1.setBoard(newboard);
						char newplayer = (player == 'R')?'W':'R';
						child1.setPlayer(newplayer);
						NodeExpanded++;
						child1.move = move+1;
						childs.push_back(child1);
						NodeCount++; NC1++;
						copy(&board[0][0],&board[0][0]+8*8,&newboard[0][0]);
					}
					//Check jumps
					jump(board,i,j,player);
				}
				if(player == 'R' && board[i][j] == 'S') //Computer player turned king
				{
					//cout<<"Computer player turned king"<<endl;
					char newboard[8][8];
					copy(&board[0][0],&board[0][0]+8*8,&newboard[0][0]);
					if(check_Left(board,i,j,'R'))
					{
						//I can move left
						newboard[i+1][j-1] = 'S';
						newboard[i][j] = '_';
						Node child1;
						child1.setBoard(newboard);
						char newplayer = (player == 'R')?'W':'R';
						child1.setPlayer(newplayer);
						NodeExpanded++;
						child1.move = move+1;
						childs.push_back(child1);
						NodeCount++; NC1++;
						copy(&board[0][0],&board[0][0]+8*8,&newboard[0][0]);
					}
					if(check_Right(board,i,j,'R'))
					{
						//I can move right
						newboard[i+1][j+1] = 'S';
						newboard[i][j] = '_';
						Node child1;
						child1.setBoard(newboard);
						char newplayer = (player == 'R')?'W':'R';
						child1.setPlayer(newplayer);
						NodeExpanded++;
						child1.move = move+1;
						childs.push_back(child1);
						NodeCount++; NC1++;
						copy(&board[0][0],&board[0][0]+8*8,&newboard[0][0]);
					}
					if(check_Left(board,i,j,'W'))
					{
						//I can move left
						newboard[i-1][j-1] = 'S';
						newboard[i][j] = '_';
						Node child1;
						child1.setBoard(newboard);
						char newplayer = (player == 'R')?'W':'R';
						child1.setPlayer(newplayer);
						NodeExpanded++;
						child1.move = move+1;
						childs.push_back(child1);
						NodeCount++; NC1++;
						copy(&board[0][0],&board[0][0]+8*8,&newboard[0][0]);
					}
					if(check_Right(board,i,j,'W'))
					{
						//I can move right
						newboard[i-1][j+1] = 'S';
						newboard[i][j] = '_';
						Node child1;
						child1.setBoard(newboard);
						char newplayer = (player == 'R')?'W':'R';
						child1.setPlayer(newplayer);
						NodeExpanded++;
						child1.move = move+1;
						childs.push_back(child1);
						NodeCount++;
						copy(&board[0][0],&board[0][0]+8*8,&newboard[0][0]);
					}
					//Check jumps
					//cout<<"Calling kjump"<<endl;
					king_jump(board,i,j,player);
				}
				if(player == 'W' && board[i][j] == 'Z') //User player turned king
				{
					char newboard[8][8];
					copy(&board[0][0],&board[0][0]+8*8,&newboard[0][0]);
					if(check_Left(board,i,j,'R'))
					{
						//I can move left
						newboard[i+1][j-1] = 'Z';
						newboard[i][j] = '_';
						Node child1;
						child1.setBoard(newboard);
						char newplayer = (player == 'R')?'W':'R';
						child1.setPlayer(newplayer);
						NodeExpanded++;
						child1.move = move+1;
						childs.push_back(child1);
						NodeCount++;
						copy(&board[0][0],&board[0][0]+8*8,&newboard[0][0]);
					}
					if(check_Right(board,i,j,'R'))
					{
						//I can move right
						newboard[i+1][j+1] = 'Z';
						newboard[i][j] = '_';
						Node child1;
						child1.setBoard(newboard);
						char newplayer = (player == 'R')?'W':'R';
						child1.setPlayer(newplayer);
						NodeExpanded++;
						child1.move = move+1;
						childs.push_back(child1);
						NodeCount++;
						copy(&board[0][0],&board[0][0]+8*8,&newboard[0][0]);
					}
					if(check_Left(board,i,j,'W'))
					{
						//I can move left
						newboard[i-1][j-1] = 'Z';
						newboard[i][j] = '_';
						Node child1;
						child1.setBoard(newboard);
						char newplayer = (player == 'R')?'W':'R';
						child1.setPlayer(newplayer);
						NodeExpanded++;
						child1.move = move+1;
						childs.push_back(child1);
						NodeCount++;
						copy(&board[0][0],&board[0][0]+8*8,&newboard[0][0]);
					}
					if(check_Right(board,i,j,'W'))
					{
						//I can move right
						newboard[i-1][j+1] = 'Z';
						newboard[i][j] = '_';
						Node child1;
						child1.setBoard(newboard);
						char newplayer = (player == 'R')?'W':'R';
						child1.setPlayer(newplayer);
						NodeExpanded++;
						child1.move = move+1;
						childs.push_back(child1);
						NodeCount++;
						copy(&board[0][0],&board[0][0]+8*8,&newboard[0][0]);
					}
					//Check jumps
					//cout<<"Calling kjump in user player turned king"<<endl;
					king_jump(board,i,j,player);
				}
			}
		}
	}
}
bool Node::compare_Matrix(char s[][8],char d[][8])
{
	for(int i = 0;i < 8;i++)
	{
		for(int j = 0;j < 8;j++)
		{
			if(s[i][j] != d[i][j])
				return false;
		}
	}
	return true;
}

void Node::king_jump(char board[][8],int row,int col,char player)
{
	int crow,ccol;
	crow = row;
	ccol = col;
	char newboard[8][8];
	copy(&board[0][0],&board[0][0]+8*8,&newboard[0][0]);

	king_jump_left(newboard,row,col,player,crow,ccol);
	if(!compare_Matrix(board,newboard) && king_left_end(newboard,crow,ccol,player) && king_right_end(newboard,crow,ccol,player) && king_top_left_end(newboard,crow,ccol,player) && king_top_right_end(newboard,crow,ccol,player))
	{
		Node child1;
		child1.setBoard(newboard);
		char newplayer = (player == 'R')?'W':'R';
		child1.setPlayer(newplayer);
		NodeExpanded++;
		child1.move = move+1;
		childs.push_back(child1);
		NodeCount++;
	}
	copy(&board[0][0],&board[0][0]+8*8,&newboard[0][0]);

	king_jump_right(newboard,row,col,player,crow,ccol);
	if(!compare_Matrix(board,newboard) && king_left_end(newboard,crow,ccol,player) && king_right_end(newboard,crow,ccol,player) && king_top_left_end(newboard,crow,ccol,player) && king_top_right_end(newboard,crow,ccol,player))
	{

		Node child1;
		child1.setBoard(newboard);
		char newplayer = (player == 'R')?'W':'R';
		child1.setPlayer(newplayer);
		NodeExpanded++;
		child1.move = move+1;
		childs.push_back(child1);
		NodeCount++;
	}
	copy(&board[0][0],&board[0][0]+8*8,&newboard[0][0]);

	king_top_left(newboard,row,col,player,crow,ccol);
	if(!compare_Matrix(board,newboard)  && king_left_end(newboard,crow,ccol,player) && king_right_end(newboard,crow,ccol,player) && king_top_left_end(newboard,crow,ccol,player) && king_top_right_end(newboard,crow,ccol,player))
	{
		Node child1;
		child1.setBoard(newboard);
		char newplayer = (player == 'R')?'W':'R';
		child1.setPlayer(newplayer);
		NodeExpanded++;
		child1.move = move+1;
		childs.push_back(child1);
		NodeCount++;
	}
	copy(&board[0][0],&board[0][0]+8*8,&newboard[0][0]);

	king_top_right(newboard,row,col,player,crow,ccol);
	if(!compare_Matrix(board,newboard)  && king_left_end(newboard,crow,ccol,player) && king_right_end(newboard,crow,ccol,player) && king_top_left_end(newboard,crow,ccol,player) && king_top_right_end(newboard,crow,ccol,player))
	{
        Node child1;
		child1.setBoard(newboard);
		char newplayer = (player == 'R')?'W':'R';
		child1.setPlayer(newplayer);
		NodeExpanded++;
		child1.move = move+1;
		childs.push_back(child1);
		NodeCount++;
	}
	copy(&board[0][0],&board[0][0]+8*8,&newboard[0][0]);
}

bool Node::king_left_end(char newboard[][8],int row,int col,char player)
{

	int nrow,ncol;
	if(player == 'R')
	{
		nrow = row + 2;
		ncol = col - 2;
		if((nrow >= 0 && nrow <= 7 && ncol >= 0 && ncol <= 7 && newboard[nrow][ncol] == '_'))
		{
				if(newboard[nrow-1][ncol+1] == 'W' || newboard[nrow-1][ncol+1] == 'Z')
				{
					return false;
				}
		}
	}
	else if(player == 'W')
	{
		nrow = row + 2;
		ncol = col - 2;
		if((nrow >= 0 && nrow <= 7 && ncol >= 0 && ncol <= 7 && newboard[nrow][ncol] == '_'))
		{
			if(newboard[nrow-1][ncol+1] == 'R' || newboard[nrow-1][ncol+1] == 'S')
			{
				return false;
			}
		}
	}
	return true;
}

void Node::king_jump_left(char newboard[][8],int row,int col,char player,int &crow,int &ccol)
{
    int nrow,ncol;
	if(player == 'R')
	{
		nrow = row + 2;
		ncol = col - 2;
		if((nrow >= 0 && nrow <= 7 && ncol >= 0 && ncol <= 7 && newboard[nrow][ncol] == '_'))
		{
			if(newboard[nrow-1][ncol+1] == 'W' || newboard[nrow-1][ncol+1] == 'Z')
			{
				newboard[nrow-1][ncol+1] = '_'; //remove opponent coin
				newboard[row][col] = '_'; //remove current coin
				crow = nrow;
				ccol = ncol;
				newboard[nrow][ncol] = 'S';
				king_jump(newboard,nrow,ncol,player);
			}
		}
	}
	else if(player == 'W')
	{
		nrow = row + 2;
		ncol = col - 2;
		if((nrow >= 0 && nrow <= 7 && ncol >= 0 && ncol <= 7 && newboard[nrow][ncol] == '_'))
		{
			if(newboard[nrow-1][ncol+1] == 'R' || newboard[nrow-1][ncol+1] == 'S')
			{
				//Then i can make a left jump
				newboard[nrow-1][ncol+1] = '_'; //remove opponent coin
				newboard[row][col] = '_'; //remove current coin
				crow = nrow;
				ccol = ncol;
				newboard[nrow][ncol] = 'Z';
				king_jump(newboard,nrow,ncol,player);
			}
		}
	}
}

bool Node::king_right_end(char newboard[][8],int row,int col,char player)
{
	int nrow,ncol;
	if(player == 'R')
	{
		nrow = row + 2;
		ncol = col + 2;
		if((nrow >= 0 && nrow <= 7 && ncol >= 0 && ncol <= 7 && newboard[nrow][ncol] == '_'))
		{
			if(newboard[nrow-1][ncol-1] == 'W' || newboard[nrow-1][ncol-1] == 'Z')
			{
				return false;
			}
		}
	}
	else if(player == 'W')
	{
		nrow = row + 2;
		ncol = col + 2;
		if((nrow >= 0 && nrow <= 7 && ncol >= 0 && ncol <= 7 && newboard[nrow][ncol] == '_'))
		{
			if(newboard[nrow-1][ncol-1] == 'R' || newboard[nrow-1][ncol-1] == 'S')
			{
				return false;
			}
		}
	}
	return true;
}

void Node::king_jump_right(char newboard[][8],int row,int col,char player,int &crow,int &ccol)
{

	int nrow,ncol;
	if(player == 'R')
	{
		nrow = row + 2;
		ncol = col + 2;
		if((nrow >= 0 && nrow <= 7 && ncol >= 0 && ncol <= 7 && newboard[nrow][ncol] == '_'))
		{
			if(newboard[nrow-1][ncol-1] == 'W' || newboard[nrow-1][ncol-1] == 'Z')
			{
				newboard[nrow-1][ncol-1] = '_'; //remove opponent coin
				newboard[row][col] = '_'; //remove current coin
				crow = nrow;
				ccol = ncol;
				newboard[nrow][ncol] = 'S';
				king_jump(newboard,nrow,ncol,player);
			}
		}
	}
	else if(player == 'W')
	{
		nrow = row + 2;
		ncol = col + 2;
		if((nrow >= 0 && nrow <= 7 && ncol >= 0 && ncol <= 7 && newboard[nrow][ncol] == '_'))
		{
			if(newboard[nrow-1][ncol-1] == 'R' || newboard[nrow-1][ncol-1] == 'S')
			{
				newboard[nrow-1][ncol-1] = '_'; //remove opponent coin
				newboard[row][col] = '_'; //remove current coin
				crow = nrow;
				ccol = ncol;
				newboard[nrow][ncol] = 'Z';
				king_jump(newboard,nrow,ncol,player);
			}
		}
	}
}

bool Node::king_top_left_end(char newboard[][8],int row,int col,char player)
{
    int nrow,ncol;
	if(player == 'R')
	{
		nrow = row - 2;
		ncol = col - 2;
		if((nrow >= 0 && nrow <= 7 && ncol >= 0 && ncol <= 7 && newboard[nrow][ncol] == '_'))
		{
			if(newboard[nrow+1][ncol+1] == 'W' || newboard[nrow+1][ncol+1] == 'Z')
			{
				return false;
			}
		}
	}
	else if(player == 'W')
	{
		nrow = row - 2;
		ncol = col - 2;
		if((nrow >= 0 && nrow <= 7 && ncol >= 0 && ncol <= 7 && newboard[nrow][ncol] == '_'))
		{
			if(newboard[nrow+1][ncol+1] == 'R' || newboard[nrow+1][ncol+1] == 'S')
			{
				return false;
			}
		}
	}
	return true;
}

void Node::king_top_left(char newboard[][8],int row,int col,char player,int &crow,int &ccol)
{
	int nrow,ncol;
	if(player == 'R')
	{
		nrow = row - 2;
		ncol = col - 2;
		if((nrow >= 0 && nrow <= 7 && ncol >= 0 && ncol <= 7 && newboard[nrow][ncol] == '_'))
		{
			if(newboard[nrow+1][ncol+1] == 'W' || newboard[nrow+1][ncol+1] == 'Z')
			{
				newboard[nrow+1][ncol+1] = '_'; //remove opponent coin
				newboard[row][col] = '_'; //remove current coin
				crow = nrow;
				ccol = ncol;
				newboard[nrow][ncol] = 'S';
				king_jump(newboard,nrow,ncol,player);
			}
		}
	}
	else if(player == 'W')
	{
		nrow = row - 2;
		ncol = col - 2;
		if((nrow >= 0 && nrow <= 7 && ncol >= 0 && ncol <= 7 && newboard[nrow][ncol] == '_'))
		{
			if(newboard[nrow+1][ncol+1] == 'R' || newboard[nrow+1][ncol+1] == 'S')
			{
				//Then i can make a left jump
				newboard[nrow+1][ncol+1] = '_'; //remove opponent coin
				newboard[row][col] = '_'; //remove current coin
				crow = nrow;
				ccol = ncol;
				newboard[nrow][ncol] = 'Z';
				king_jump(newboard,nrow,ncol,player);
			}
		}
	}
}

bool Node::king_top_right_end(char newboard[][8],int row,int col,char player)
{
	int nrow,ncol;
	if(player == 'R')
	{
		nrow = row - 2;
		ncol = col + 2;
		if((nrow >= 0 && nrow <= 7 && ncol >= 0 && ncol <= 7 && newboard[nrow][ncol] == '_'))
		{
			if(newboard[nrow+1][ncol-1] == 'W' || newboard[nrow+1][ncol-1] == 'Z')
			{
				return false;
			}
		}
	}
	else if(player == 'W')
	{
		nrow = row - 2;
		ncol = col + 2;
		if((nrow >= 0 && nrow <= 7 && ncol >= 0 && ncol <= 7 && newboard[nrow][ncol] == '_'))
		{
			if(newboard[nrow+1][ncol-1] == 'R' || newboard[nrow+1][ncol-1] == 'S')
			{
				return false;
			}
		}
	}
	return true;
}

void Node::king_top_right(char newboard[][8],int row,int col,char player,int &crow,int &ccol)
{
	int nrow,ncol;
	if(player == 'R')
	{
		nrow = row - 2;
		ncol = col + 2;
		if((nrow >= 0 && nrow <= 7 && ncol >= 0 && ncol <= 7 && newboard[nrow][ncol] == '_'))
		{
			if(newboard[nrow+1][ncol-1] == 'W' || newboard[nrow+1][ncol-1] == 'Z')
			{
				newboard[nrow+1][ncol-1] = '_'; //remove opponent coin
				newboard[row][col] = '_'; //remove current coin
				crow = nrow;
				ccol = ncol;
				newboard[nrow][ncol] = 'S';
				king_jump(newboard,nrow,ncol,player);
			}
		}
	}
	else if(player == 'W')
	{
		nrow = row - 2;
		ncol = col + 2;
		if((nrow >= 0 && nrow <= 7 && ncol >= 0 && ncol <= 7 && newboard[nrow][ncol] == '_'))
		{
			if(newboard[nrow+1][ncol-1] == 'R' || newboard[nrow+1][ncol-1] == 'S')
			{
				//Then i can make a left jump
				newboard[nrow+1][ncol-1] = '_'; //remove opponent coin
				newboard[row][col] = '_'; //remove current coin
				crow = nrow;
				ccol = ncol;
				newboard[nrow][ncol] = 'Z';
				king_jump(newboard,nrow,ncol,player);
			}
		}
	}
}

void Node::jump(char board[][8],int row,int col,char player)
{
	//create a copy of board
	int crow,ccol;
	crow = row;
	ccol = col;
	char newboard[8][8];
	copy(&board[0][0],&board[0][0]+8*8,&newboard[0][0]);
	jump_left(newboard,row,col,player,crow,ccol);
	//Check if the newboard is changed --- compare newboard with board
	if(!compare_Matrix(board,newboard) && left_End(newboard,crow,ccol,player) && right_End(newboard,crow,ccol,player))
	//add more conditions that the board cannot have any more jumps (left and right)
	{
		Node child1;
		child1.setBoard(newboard);
		char newplayer = (player == 'R')?'W':'R';
		child1.setPlayer(newplayer);
		NodeExpanded++;
		child1.move = move+1;
		childs.push_back(child1);
		NodeCount++;
	}
	copy(&board[0][0],&board[0][0]+8*8,&newboard[0][0]);
	jump_right(newboard,row,col,player,crow,ccol);
	if(!compare_Matrix(board,newboard) && left_End(newboard,crow,ccol,player) && right_End(newboard,crow,ccol,player))
	//add more conditions that the board cannot have any more jumps (left and right)
	{
		Node child1;
		child1.setBoard(newboard);
		char newplayer = (player == 'R')?'W':'R';
		child1.setPlayer(newplayer);
		NodeExpanded++;
		child1.move = move+1;
		NodeExpanded++;
		childs.push_back(child1);
		NodeCount++;
	}
	copy(&board[0][0],&board[0][0]+8*8,&newboard[0][0]);
}

void Node::jump_left(char newboard[][8],int row,int col,char player,int &crow,int &ccol)
{
	int nrow,ncol;
	if(player == 'R')
	{
		nrow = row + 2;
		ncol = col - 2;
		if((nrow >= 0 && nrow <= 7 && ncol >= 0 && ncol <= 7 && newboard[nrow][ncol] == '_'))
		{
			if(newboard[nrow-1][ncol+1] == 'W' || newboard[nrow-1][ncol+1] == 'Z')
			{
				//Then i can make a left jump
				newboard[nrow-1][ncol+1] = '_'; //remove opponent coin
				newboard[row][col] = '_'; //remove current coin
				//Checking if the new position will make the current player a king or not
				crow = nrow;
				ccol = ncol;
				if(check_King(nrow,player))
					newboard[nrow][ncol] = 'S';
				else
				{
					newboard[nrow][ncol] = 'R';
					jump(newboard,nrow,ncol,player);
				}
			}
		}
	}
	else if(player == 'W')
	{
		nrow = row - 2;
		ncol = col - 2;
		if((nrow >= 0 && nrow <= 7 && ncol >= 0 && ncol <= 7 && newboard[nrow][ncol] == '_'))
		{
			if(newboard[nrow+1][ncol+1] == 'R' || newboard[nrow+1][ncol+1] == 'S')
			{
				//Then i can make a left jump
				newboard[nrow+1][ncol+1] = '_'; //remove opponent coin
				newboard[row][col] = '_'; //remove current coin
				//Checking if the new position will make the current player a king or not
				crow = nrow;
				ccol = ncol;
				if(check_King(nrow,player))
					newboard[nrow][ncol] = 'Z';
				else
				{
					newboard[nrow][ncol] = 'W';
					jump(newboard,nrow,ncol,player);
				}
			}
		}
	}
}

void Node::jump_right(char newboard[][8],int row,int col,char player,int &crow,int &ccol)
{
	int nrow,ncol;
	if(player == 'R')
	{
		nrow = row + 2;
		ncol = col + 2;
		if((nrow >= 0 && nrow <= 7 && ncol >= 0 && ncol <= 7 && newboard[nrow][ncol] == '_'))
		{
			if(newboard[nrow-1][ncol-1] == 'W' || newboard[nrow-1][ncol-1] == 'Z')
			{
				//Then i can make a left jump
				newboard[nrow-1][ncol-1] = '_'; //remove opponent coin
				newboard[row][col] = '_'; //remove current coin
				crow = nrow;
				ccol = ncol;
				//Checking if the new position will make the current player a king or not
				if(check_King(nrow,player))
					newboard[nrow][ncol] = 'S';
				else
				{
					newboard[nrow][ncol] = 'R';
					jump(newboard,nrow,ncol,player);
				}
			}
		}
	}
	else if(player == 'W')
	{
		nrow = row - 2;
		ncol = col + 2;
		if((nrow >= 0 && nrow <= 7 && ncol >= 0 && ncol <= 7 && newboard[nrow][ncol] == '_'))
		{
			if(newboard[nrow+1][ncol-1] == 'R' || newboard[nrow+1][ncol-1] == 'S')
			{
				//Then i can make a left jump
				newboard[nrow+1][ncol-1] = '_'; //remove opponent coin
				newboard[row][col] = '_'; //remove current coin
				crow = nrow;
				ccol = ncol;
				//Checking if the new position will make the current player a king or not
				if(check_King(nrow,player))
					newboard[nrow][ncol] = 'Z';
				else
				{
					newboard[nrow][ncol] = 'W';
					jump(newboard,nrow,ncol,player);
				}
			}
		}
	}
}

bool Node::check_Left(char board[][8],int row,int col,char player)
{
	int lrow,lcol;
	if(player == 'R')
	{
		lrow = row+1;
		lcol = col-1;
	}
	else if(player == 'W')
	{
		lrow = row-1;
		lcol = col-1;
	}
	if(!(lrow >= 0 && lrow <= 7 && lcol >= 0 && lcol <= 7 && board[lrow][lcol] =='_'))
		return false;
	return true;
}

bool Node::check_Right(char board[][8],int row,int col,char player)
{

	int lrow,lcol;
	if(player == 'R')
	{
		lrow = row+1;
		lcol = col+1;
	}
	else if(player == 'W')
	{
		lrow = row-1;
		lcol = col+1;
	}
	if(!(lrow >= 0 && lrow <= 7 && lcol >= 0 && lcol <= 7 && board[lrow][lcol] =='_'))
		return false;
	return true;
}

void printBoard(char array2D[][8])
{
	SetColor(34);
	cout << endl << "***************" << endl;
	for(int i = 0;i < 8; i++)
	{
		if(i == 0)
		{
			cout<<"-------------------"<<endl;
			cout<<"R/C 0 1 2 3 4 5 6 7"<<endl;
			cout<<"-------------------";
			cout<<endl;
		}
		cout<<" |"<<i<<"|";
		for(int j = 0;j < 8;j++)
		{
		    if(array2D[i][j]=='R' || array2D[i][j]=='S' )
               {SetColor(4);
                cout<<array2D[i][j]<<" ";
                SetColor(34);
               }
            else if(array2D[i][j]=='W' || array2D[i][j]=='Z')
               { SetColor(15);
                 cout<<array2D[i][j]<<" ";
                 SetColor(34);
               }
            else
            {
                SetColor(34);
                cout<<array2D[i][j]<<" ";
            }

		}
		cout<<endl;
	}
	cout << endl << "***************" << endl;
	SetColor(15);
}

bool checkPlayersEquality(char u[][8],char a[][8])
{
    for(int i=0;i<8;i++)
	{
		for(int j=0;j<8;j++)
		{
			if((a[i][j] == 'W' || a[i][j] == 'Z') && (u[i][j] != a[i][j]))
			{
				return false;
			}
		}
	}
	return true;
}

/*
Pawn = 1            //Vinitha
King = 2
*/
int Node::Eval1()
{

	for(int i=0;i<8;i++)
	{
		for(int j=0;j<8;j++)
		{
			if(board[i][j] == 'W')
				wcount++;
			else if(board[i][j] == 'R')
				rcount++;
			if(board[i][j] == 'S')
				rcount+= 2;
			else if (board[i][j] == 'Z')
				wcount+= 2;
		}
	}
	if(options==1)
		return (wcount - rcount);
	return (rcount - wcount);
}

/*
Normal pawn = 1
King in corner = 4 (King is )      //Archana
*/

int Node::Eval2()
{

	for(int i=0;i<8;i++)
	{
		for(int j=0;j<8;j++)
		{
			if( i == 0 || j == 0 || i == 7 || j == 7)
			{
				if(board[i][j] == 'R')
					rcount++;
				else if(board[i][j] == 'W')
					wcount++;
				if(board[i][j] == 'S')
					rcount += 4;
				else if(board[i][j] == 'Z')
				 	wcount += 4;
			}
		}
	}
	if(options==1)
		return (wcount - rcount);
	return (rcount - wcount);
}

/*
Pawn in the opponent's half of the board value = 2   //Mounisha
Pawn in the player's half of the board value = 1
King’s value = 3
*/
int Node::Eval3()
{

	for(int i=0;i<8;i++)
	{
		for(int j=0;j<8;j++)
		{

			if(board[i][j] == 'S')
				wcount += 3;
			else if (board[i][j] == 'Z')
				rcount += 3;

			if(i >= 0 && i <= 3)
			{
			//Giving advantage to player if they are in other players half
				if(board[i][j] == 'R')
					rcount += 1;
				else if (board[i][j] == 'W')
					wcount += 2;
			}
			else if(i >= 4 && i <= 7)
			{
				//Giving advantage to player if they are in other players half
				if(board[i][j] == 'R')
					rcount += 2;
				else if (board[i][j] == 'W')
					wcount += 1;
			}
		}
	}
	if(options==1)
		return (wcount - rcount);
	return (rcount - wcount);
}

// Archana's

int Node::Eval4()
{

	int eval[8][8] =
	{
		{10,9,9,9,9,9,9,10},
		{9,8,7,6,6,7,8,9},
		{8,5,4,3,3,4,5,8},
		{7,3,2,1,1,2,3,7},
		{7,3,2,1,1,2,3,7},
		{8,5,4,3,3,4,5,8},
		{9,8,7,6,6,7,8,9},
		{10,9,9,9,9,9,9,10}
	};

	for(int i=0;i<8;i++)
	{
		for(int j=0;j<8;j++)
		{

			if(i >= 0 && i <= 3)
			{

				if(board[i][j] == 'W'|| board[i][j] == 'Z' )
				{

                    wcount = wcount+eval[i][j];
				}
				else if (board[i][j] == 'R')
                {

					rcount = rcount+1;
                }
                else if (board[i][j] == '_')
                {
                wcount=wcount+0;
                rcount=rcount+0;
                }
			}
			else if(i >= 4 && i <= 7)
			{

            if(board[i][j] == 'W')
                wcount = wcount+1;
            else if (board[i][j] == 'R'||board[i][j] == 'S')
                rcount = rcount+eval[i][j];
			}
			else if (board[i][j] == '_')
            {
                wcount=wcount+0;
                rcount=rcount+0;
            }
        }
}
     if(options==1)
        return wcount;
    return rcount;
}

bool DeepEnough(int d)
{
	return d >= 3;
}

Node MinMaxAB(Node &b,int depth,char player,int ut,int pt)
{
	if(DeepEnough(depth))
	{
		b.MoveGen();
		b.v = b.utility();
		if(options == 1)
		{
			if(b.player == 'W')
				b.v *= -1;
		}
		if(options == 2)
		{

			if(b.player == 'R')
				b.v *= -1;
		}
		return b;
	}
	b.MoveGen();
	if(b.childs.size() == 0)
	{
		b.v = b.utility();
		if(options == 1)
		{

			if(b.player == 'W')
				b.v *= -1;
		}
		if(options == 2)
		{

			if(b.player == 'R')
				b.v *= -1;
		}
		return b;
	}
	for(int i = 0;i<b.childs.size();i++)
	{
		Node ResultSucc = MinMaxAB(b.childs[i],b.childs[i].move,b.childs[i].player,-1*pt,-1*ut);
		int newValue = -1 * ResultSucc.v;
		if(newValue > pt)
		{
			pt = newValue;
			if(depth == 0)
				bestPath.push_back(b.childs[i]);
		}
		if(pt >= ut)
		{
			b.childs[i].v = pt;
			return b.childs[i];
		}
	}
	b.v = pt;
	return b;
}

void MinMaxABcomVsMinMaxABcom(char &player,int &row,int &column,int &nrow,int &ncolumn,bool &flag,bool &flag2,char board[][8],char newboard[][8])
{  start = clock();
	while(flag2)
	{
		bestPath.clear();
		options = 1;
		Node test;
		test.setBoard(board);
		test.setPlayer('R');
		test.move = 0;

		Node comp1 = MinMaxAB(test,test.move,test.player,100,-120);

		for(int i=0;i<8;i++)
		{
			for(int j=0;j<8;j++)
			{

				board[i][j] = bestPath[bestPath.size() -1].board[i][j];
			}
		}

		cout<<"After Player1-Red's Turn board is:"<<endl;
		moveCount++;
		printBoard(board);
		Node test1;
		test1.setBoard(board);
		test1.setPlayer('W');
		NodeExpanded = test1.childs.size();
		test1.move = 0;
		test1.MoveGen();
		NodeCount -= test1.childs.size();
		NC1-= test1.childs.size();
		if(test1.terminal())
		{
			  endd = clock();
            double time_taken = double(endd - start) / double(CLOCKS_PER_SEC);
            cout << "Time taken by program is : " << fixed
                 << time_taken << setprecision(5);
            cout << " sec " << endl;
			cout<<"Length of Game Path:"<<moveCount<<endl;
			cout<<"No of Nodes Generated:"<<NC1<<endl;
			cout<<"No of Nodes Expanded :"<<NodeExpanded<<endl;
			int w = test1.utility();
			 if(eval_choice1==1) w=test1.Eval1();
             else if(eval_choice1==2)  w=test1.Eval2();
             else if(eval_choice1==3)  w=test1.Eval3();
             else if(eval_choice1==4)  w=test1.Eval4();
			if(w == 20)
			{
				cout<<"Player1-Red Won the game"<<endl;
				exit(0);
			}
		}
		if(moveCount >=52)
		{
			//cout<<"rcount"<<rcount<<endl;
			//cout<<"wcount"<<wcount<<endl;
			if(wcount>rcount)
            {
                cout<<"Computer W Won the game"<<endl;
            }
             else if (wcount==rcount)
            {
                cout<<"Game Tie"<<endl;
            }
            else{
                cout<<"Computer R Won the game"<<endl;
            }
			//cout<<"Game Tie"<<endl;
			cout<<"Length of Game Path:"<<moveCount<<endl;
			cout<<"No of Nodes Generated:"<<NC1<<endl;
			cout<<"No of Nodes Expanded:"<<NodeExpanded<<endl;
			exit(0);
		}
		bestPath.clear();
		options = 2;
		Node test2;
		test2.setBoard(board);
		test2.setPlayer('W');
		test2.move = 0;
		comp1 = MinMaxAB(test2,test2.move,test2.player,120,-100);
		for(int k = bestPath.size() - 1;k>=0;k--)
		{
			if(bestPath[k].move == 1)
			{
				for(int i=0;i<8;i++)
				{
					for(int j=0;j<8;j++)
					{
						board[i][j] = bestPath[k].board[i][j];
					}
				}
				break;
			}
		}
		cout<<"After Player2-White's Turn board is:"<<endl;
		moveCount++;
		printBoard(board);
		Node test3;
		test3.setBoard(board);
		test3.setPlayer('R');
        NodeExpanded -= test3.childs.size();
		test3.move = 0;
		test3.MoveGen();
		NodeCount -= test3.childs.size();
		if(test3.terminal())
		{
			  endd = clock();
            double time_taken = double(endd - start) / double(CLOCKS_PER_SEC);
            cout << "Time taken by program is : " << fixed
                 << time_taken << setprecision(5);
            cout << " sec " << endl;
			cout<<"Length of Game Path:"<<moveCount<<endl;
			cout<<"No of Nodes Generated:"<<NC1<<endl;
			cout<<"No of Nodes Expanded:"<<NodeExpanded<<endl;
			int w = test3.utility();
			if(eval_choice2==1) w=test2.Eval1();
             else if(eval_choice2==2)  w=test2.Eval2();
             else if(eval_choice2==3)  w=test2.Eval3();
             else if(eval_choice2==4)  w=test2.Eval4();
			if(w == 20)
			{
				cout<<"Player2-White won the game"<<endl;
				exit(0);
			}
		}
		if(moveCount >= 52)
		{
			//cout<<"rcount"<<rcount<<endl;
			//cout<<"wcount"<<wcount<<endl;
			if(wcount>rcount)
            {
                cout<<"Computer W Won the game"<<endl;
            }
            else if (wcount==rcount)
            {
                cout<<"Game Tie"<<endl;
            }
            else{
                cout<<"Computer R Won the game"<<endl;
            }
			//cout<<"Game Tie"<<endl;
			cout<<"Length of Game Path:"<<moveCount<<endl;
			cout<<"No of Nodes Generated:"<<NodeCount-NC1<<endl;

			//cout<<"Game Tie"<<endl;
			//cout<<"Length of Game Path:"<<moveCount<<endl;
			//cout<<"No of Nodes Generated:"<<NodeCount<<endl;
			cout<<"No of Nodes Expanded:"<<NodeExpanded<<endl;
			exit(0);
			//exit(0);
		}
	}
}
void MinMaxABcomVsMinMaxABuser(char &player,int &row,int &column,int &nrow,int &ncolumn,bool &flag,bool &flag2,char board[][8],char newboard[][8])
{
	start = clock();
	while(flag2)
	{
		bestPath.clear();
		options = 1;
		Node test;
		test.setBoard(board);
		test.setPlayer('R');
		test.move = 0;
		Node comp1 = MinMaxAB(test,test.move,test.player,100,-120);
		for(int i=0;i<8;i++)
		{
			for(int j=0;j<8;j++)
			{
				board[i][j] = bestPath[bestPath.size() -1].board[i][j];
			}
		}
		cout<<"After Player1-Red's turn board is:"<<endl;
		moveCount++;
		printBoard(board);
		Node test1;
		test1.setBoard(board);
		test1.setPlayer('W');
		test1.move = 0;
		test1.MoveGen();
		NodeCount -= test1.childs.size();
		if(test1.terminal())
		{
		      endd = clock();
            double time_taken = double(endd - start) / double(CLOCKS_PER_SEC);
            cout << "Time taken by program is : " << fixed
                 << time_taken << setprecision(5);
            cout << " sec " << endl;
			cout<<"Length of Game Path:"<<moveCount<<endl;
			cout<<"No of Nodes Generated:"<<NodeCount<<endl;
			cout<<"No of Nodes Expanded :"<<NodeExpanded<<endl;
			int w = test1.utility();
			if(eval_choice1==1) w=test1.Eval1();
             else if(eval_choice1==2)  w=test1.Eval2();
             else if(eval_choice1==3)  w=test1.Eval3();
             else if(eval_choice1==4)  w=test1.Eval4();
			if(w == 20)
			{
				cout<<"Player1-Red won the game"<<endl;
				exit(0);
			}
		}
		if(moveCount >= 5)
		{
			//cout<<"rcount"<<rcount<<endl;
			//cout<<"wcount"<<wcount<<endl;
			if(wcount>rcount)
            {
                cout<<"User W Won the game"<<endl;
            }
             else if (wcount==rcount)
            {
                cout<<"Game Tie"<<endl;
            }
            else{
                cout<<"Computer R Won the game"<<endl;
            }
			//cout<<"Game Tie"<<endl;
			cout<<"Length of Game Path:"<<moveCount<<endl;
			cout<<"No of Nodes Generated:"<<NodeCount<<endl;

			//cout<<"Game Tie"<<endl;
			//cout<<"Length of Game Path:"<<moveCount<<endl;
			//cout<<"No of Nodes Generated:"<<NodeCount<<endl;
			cout<<"No of Nodes Expanded:"<<NodeExpanded<<endl;
			exit(0);
			//exit(0);
		}
		options = 2;
		cout << "Your Turn"<<endl;

		int tempNodeCount = NodeCount;
		for(int i=0;i<8;i++)
		{
			for(int j=0;j<8;j++)
			{
				newboard[i][j] = board[i][j];
			}
		}
		do
		{
			do
			{
				cout << "Choose a Row to move from(0-7): ";
				cin >> row;
				cout << "Choose a Column to move from(0-7): "<<endl;
				cin >> column;
				cout <<"Choose a Row to make a move to that position(0-7): ";
				cin>>nrow;
				cout<<"Choose a Column to make a move to that position(0-7): ";
				cin>>ncolumn;
				if(!((row >=0 && row <=7) && (column >=0 && column <=7) && (nrow >=0 && nrow <=7) && (ncolumn >=0 && ncolumn <=7)))
				{
					cout<<"Invalid row and Column Selected"<<endl;
					flag = true;
				}
				else
				{
					flag = false;
				}
			}while(flag);
			Node test2;
			test2.setBoard(newboard);
			test2.setPlayer('W');
			test2.move = 0;
			test2.MoveGen();
			NodeCount -= test2.childs.size();
			if(nrow == 0)
			{
				newboard[nrow][ncolumn] = 'Z';
			}
			else
			{
				newboard[nrow][ncolumn] = newboard[row][column];
			}
			newboard[row][column] = '_';
			int count = 0;
			for(int i=0;i<test2.childs.size();i++)
			{
				if(checkPlayersEquality(newboard,test2.childs[i].board))
				{
					for(int j=0;j<8;j++)
					{
						for(int k=0;k<8;k++)
							board[j][k] = test2.childs[i].board[j][k];
					}
					count++;
					break;
				}
			}
			if(count == 0)
			{
				flag = true;
				cout<<"Action Cannot be made.Choose a Valid Action"<<endl;
				for(int i=0;i<8;i++)
				{
					for(int j=0;j<8;j++)
					{
						newboard[i][j] = board[i][j];
					}
				}
			}
			else
			{
				flag = false;
			}
		}while(flag);
		cout<<"After Your Turn board is:"<<endl;
		NodeCount -= tempNodeCount;
		moveCount++;
		printBoard(board);
		Node test3;
		test3.setBoard(board);
		test3.setPlayer('R');
		test3.move = 0;
		test3.MoveGen();
		NodeCount -= test3.childs.size();
		if(test3.terminal())
		{
			  endd = clock();
            double time_taken = double(endd - start) / double(CLOCKS_PER_SEC);
            cout << "Time taken by program is : " << fixed
                 << time_taken << setprecision(5);
            cout << " sec " << endl;
            cout<<"Length of Game Path:"<<moveCount<<endl;
			cout<<"No of Nodes Generated:"<<NodeCount<<endl;
			cout<<"No of Nodes Expanded :"<<NodeExpanded<<endl;
			int w = test3.utility();
			if(eval_choice2==1) w=test3.Eval1();
             else if(eval_choice2==2)  w=test3.Eval2();
             else if(eval_choice2==3)  w=test3.Eval3();
             else if(eval_choice2==4)  w=test3.Eval4();
			if(w == 20)
			{
				cout<<"You Won the game"<<endl;
				exit(0);
			}
		}
		if(moveCount >= 5)
		{
			//cout<<"rcount"<<rcount<<endl;
			//cout<<"wcount"<<wcount<<endl;
			if(wcount>rcount)
            {
                cout<<"User W Won the game"<<endl;
            }
             else if (wcount==rcount)
            {
                cout<<"Game Tie"<<endl;
            }
            else{
                cout<<"Computer R Won the game"<<endl;
            }
			//cout<<"Game Tie"<<endl;
			cout<<"Length of Game Path:"<<moveCount<<endl;
			cout<<"No of Nodes Generated:"<<NodeCount<<endl;

			//cout<<"Game Tie"<<endl;
			//cout<<"Length of Game Path:"<<moveCount<<endl;
			//cout<<"No of Nodes Generated:"<<NodeCount<<endl;
			cout<<"No of Nodes Expanded:"<<NodeExpanded<<endl;
			exit(0);
			//exit(0);
		}
	}
}
void MinMaxABVsAlpha_Beta(char &player,int &row,int &column,int &nrow,int &ncolumn,bool &flag,bool &flag2,char board[][8],char newboard[][8])
{
	start = clock();
	while(flag2)
	{
		options = 1;
		Node test;
		test.setBoard(board);
		test.setPlayer('R');
		test.move = 0;
		alpha_beta_search(test,board);

		cout<<"After Player1-Red's turn board is:"<<endl;
		moveCount++;
		printBoard(board);
		Node test1;
		test1.setBoard(board);
		test1.setPlayer('W');
		NodeExpanded = test1.childs.size();
		test1.move = 0;
		test1.MoveGen();
		NodeCount -= test1.childs.size();
		if(test1.terminal())
		{
			  endd = clock();
            double time_taken = double(endd - start) / double(CLOCKS_PER_SEC);
            cout << "Time taken by program is : " << fixed
                 << time_taken << setprecision(5);
            cout << " sec " << endl;
			cout<<"Length of Game Path:"<<moveCount<<endl;
			cout<<"No of Nodes Generated:"<<NodeCount<<endl;
			cout<<"No of Nodes Expanded :"<<NodeExpanded<<endl;
			int w = test1.utility();
			if(eval_choice1==1) w=test1.Eval1();
             else if(eval_choice1==2)  w=test1.Eval2();
             else if(eval_choice1==3)  w=test1.Eval3();
             else if(eval_choice1==4)  w=test1.Eval4();
			if(w == 20)
			{
				cout<<"Player1-Red Won the game"<<endl;
				exit(0);
			}
		}
		if(moveCount >= 80)
		{
			//cout<<"rcount"<<rcount<<endl;
			//cout<<"wcount"<<wcount<<endl;
			if(wcount>rcount)
            {
                cout<<"Computer W Won the game"<<endl;
            }
            else{
                cout<<"Computer R Won the game"<<endl;
            }
			//cout<<"Game Tie"<<endl;
			cout<<"Length of Game Path:"<<moveCount<<endl;
			cout<<"No of Nodes Generated:"<<NodeCount<<endl;

			//cout<<"Game Tie"<<endl;
			//cout<<"Length of Game Path:"<<moveCount<<endl;
			//cout<<"No of Nodes Generated:"<<NodeCount<<endl;
			cout<<"No of Nodes Expanded:"<<NodeExpanded<<endl;
			exit(0);
		}
		bestPath.clear();
		options = 2;
		Node test2;
		test2.setBoard(board);
		test2.setPlayer('W');
		test2.move = 0;
		Node comp1 = MinMaxAB(test2,test2.move,test2.player,120,-100);
		for(int k = bestPath.size() - 1;k>=0;k--)
		{
			if(bestPath[k].move == 1)
			{
				for(int i=0;i<8;i++)
				{
					for(int j=0;j<8;j++)
					{
						board[i][j] = bestPath[k].board[i][j];
					}
				}
				break;
			}
		}
		cout<<"After Player2-White's Turn board is:"<<endl;
		moveCount++;
		printBoard(board);
		Node test3;
		test3.setBoard(board);
		test3.setPlayer('R');
		test3.move = 0;
		test3.MoveGen();
		NodeExpanded -= test3.childs.size();
		NodeCount -= test3.childs.size();
		if(test3.terminal())
		{
			  endd = clock();
            double time_taken = double(endd - start) / double(CLOCKS_PER_SEC);
            cout << "Time taken by program is : " << fixed
                 << time_taken << setprecision(5);
            cout << " sec " << endl;
			cout<<"Length of Game Path:"<<moveCount<<endl;
			cout<<"No of Nodes Generated:"<<NodeCount<<endl;
			cout<<"No of Nodes Expanded :"<<NodeExpanded<<endl;
			int w = test3.utility();
			if(eval_choice2==1) w=test3.Eval1();
             else if(eval_choice2==2)  w=test3.Eval2();
             else if(eval_choice2==3)  w=test3.Eval3();
             else if(eval_choice2==4)  w=test3.Eval4();
			if(w == 20)
			{
				cout<<"Player2-White Won the game"<<endl;
				exit(0);
			}
		}
		if(moveCount >= 80)
		{
			//cout<<"rcount"<<rcount<<endl;
			//cout<<"wcount"<<wcount<<endl;
			if(wcount>rcount)
            {
                cout<<"Computer W Won the game"<<endl;
            }
            else{
                cout<<"Computer R Won the game"<<endl;
            }
			//cout<<"Game Tie"<<endl;
			cout<<"Length of Game Path:"<<moveCount<<endl;
			cout<<"No of Nodes Generated:"<<NodeCount<<endl;

			//cout<<"Game Tie"<<endl;
			//cout<<"Length of Game Path:"<<moveCount<<endl;
			//cout<<"No of Nodes Generated:"<<NodeCount<<endl;
			cout<<"No of Nodes Expanded:"<<NodeExpanded<<endl;
			exit(0);
			//exit(0);
		}
	}
}
void Alpha_BetacomVsAlpha_Betacom(char &player,int &row,int &column,int &nrow,int &ncolumn,bool &flag,bool &flag2,char board[][8],char newboard[][8])
{
	start = clock();
	while(flag2)
	{
		options = 1;
		Node test;
		test.setBoard(board);
		test.setPlayer('R');
		test.move = 0;
		alpha_beta_search(test,board);


		cout<<"After Player1-Red's Turn board is:"<<endl;
		moveCount++;
		printBoard(board);
		Node test1;
		test1.setBoard(board);
		test1.setPlayer('W');
		NodeExpanded = test1.childs.size();
		test1.move = 0;
		test1.MoveGen();
		NodeCount -= test1.childs.size();
		if(test1.terminal())
		{

			  endd = clock();
            double time_taken = double(endd - start) / double(CLOCKS_PER_SEC);
            cout << "Time taken by program is : " << fixed
                 << time_taken << setprecision(5);
            cout << " sec " << endl;
			cout<<"Length of Game Path:"<<moveCount<<endl;
			cout<<"No of Nodes Generated:"<<NodeCount<<endl;
			cout<<"No of Nodes Expanded:"<<NodeExpanded<<endl;
			int w = test1.utility();
			if(eval_choice1==1) w=max(w,test1.Eval1());
             else if(eval_choice1==2)  w=max(w,test1.Eval2());
             else if(eval_choice1==3)  w=max(w,test1.Eval3());
             else if(eval_choice1==4)  w=max(w,test1.Eval4());
			if(w == 20)
			{
				cout<<"Player1-Red Won the game"<<endl;
				exit(0);
			}
		}
		if(moveCount >= 80)
		{
            //cout<<"rcount"<<rcount<<endl;
			//cout<<"wcount"<<wcount<<endl;
			if(wcount>rcount)
            {
                cout<<"Computer W Won the game"<<endl;
            }
            else{
                cout<<"Computer R Won the game"<<endl;
            }
			//cout<<"Game Tie"<<endl;
			cout<<"Length of Game Path:"<<moveCount<<endl;
			cout<<"No of Nodes Generated:"<<NodeCount<<endl;

			//cout<<"Game Tie"<<endl;
			//cout<<"Length of Game Path:"<<moveCount<<endl;
			//cout<<"No of Nodes Generated:"<<NodeCount<<endl;
			cout<<"No of Nodes Expanded:"<<NodeExpanded<<endl;
			exit(0);
		}
		options = 2;
		Node test2;
		test2.setBoard(board);
		test2.setPlayer('W');
		test2.move = 0;
		alpha_beta_search(test2,board);

		cout<<"After Player2-White's Turn board is:"<<endl;
		moveCount++;
		printBoard(board);
		Node test3;
		test3.setBoard(board);
		test3.setPlayer('R');
		test3.move = 0;
		test3.MoveGen();
		NodeExpanded -= test3.childs.size();
		NodeCount -= test3.childs.size();
		if(test3.terminal())
		{
            endd = clock();
            double time_taken = double(endd - start) / double(CLOCKS_PER_SEC);
            cout << "Time taken by program is : " << fixed
                 << time_taken << setprecision(5);
            cout << " sec " << endl;
			cout<<"Length of Game Path:"<<moveCount<<endl;
			cout<<"No of Nodes Generated:"<<NodeCount<<endl;
			cout<<"No of Nodes Expanded :"<<NodeExpanded<<endl;
			int w = test3.utility();
			if(eval_choice2==1) w=max(w,test3.Eval1());
             else if(eval_choice2==2)  w=max(w,test3.Eval2());
             else if(eval_choice2==3)  w=max(w,test3.Eval3());
             else if(eval_choice2==4)  w=max(w,test3.Eval4());
			if(w == 20)
			{
				cout<<"Player2-White Won the game"<<endl;
				exit(0);
			}
            }
            if(moveCount >= 80)
            {
			//cout<<"rcount"<<rcount<<endl;
			//cout<<"wcount"<<wcount<<endl;
			if(wcount>rcount)
            {
                cout<<"Computer W Won the game"<<endl;
            }
            else{
                cout<<"Computer R Won the game"<<endl;
            }
			//cout<<"Game Tie"<<endl;
			cout<<"Length of Game Path:"<<moveCount<<endl;
			cout<<"No of Nodes Generated:"<<NodeCount<<endl;

			//cout<<"Game Tie"<<endl;
			//cout<<"Length of Game Path:"<<moveCount<<endl;
			//cout<<"No of Nodes Generated:"<<NodeCount<<endl;
			cout<<"No of Nodes Expanded:"<<NodeExpanded<<endl;
			exit(0);
            }
	}
}
void Alpha_BetacomVsAlpha_Betauser(char &player,int &row,int &column,int &nrow,int &ncolumn,bool &flag,bool &flag2,char board[][8],char newboard[][8])
{
	start = clock();
	while(flag2)
	{
		options = 1;
		Node test;
		test.setBoard(board);
		test.setPlayer('R');
		test.move = 0;
		alpha_beta_search(test,board);
		cout<<"After Player1-Red's Turn board is:"<<endl;
		moveCount++;
		printBoard(board);
		Node test1;
		test1.setBoard(board);
		test1.setPlayer('W');
		test1.move = 0;
		test1.MoveGen();
		NodeCount -= test1.childs.size();
		if(test1.terminal())
		{
			  endd = clock();
            double time_taken = double(endd - start) / double(CLOCKS_PER_SEC);
            cout << "Time taken by program is : " << fixed
                 << time_taken << setprecision(5);
            cout << " sec " << endl;
			cout<<"Length of Game Path:"<<moveCount<<endl;
			cout<<"No of Nodes Generated:"<<NodeCount<<endl;
			cout<<"No of Nodes Expanded :"<<NodeExpanded<<endl;
			int w = test1.utility();
			if(eval_choice1==1) w=max(w,test1.Eval1());
             else if(eval_choice1==2)  w=max(w,test1.Eval2());
             else if(eval_choice1==3)  w=max(w,test1.Eval3());
             else if(eval_choice1==4)  w=max(w,test1.Eval4());
			if(w == 20)
			{
				cout<<"Player1-Red Won the game"<<endl;
				exit(0);
			}
		}
		if(moveCount >= 5)
		{
			//cout<<"rcount"<<rcount<<endl;
			//cout<<"wcount"<<wcount<<endl;
			if(wcount>rcount)
            {
                cout<<"Computer W Won the game"<<endl;
            }
            else{
                cout<<"Computer R Won the game"<<endl;
            }
			//cout<<"Game Tie"<<endl;
			cout<<"Length of Game Path:"<<moveCount<<endl;
			cout<<"No of Nodes Generated:"<<NodeCount<<endl;

			//cout<<"Game Tie"<<endl;
			//cout<<"Length of Game Path:"<<moveCount<<endl;
			//cout<<"No of Nodes Generated:"<<NodeCount<<endl;
			cout<<"No of Nodes Expanded:"<<NodeExpanded<<endl;
			exit(0);
		}
		options = 2;
		cout << "Your Turn"<<endl;

		int tempNodeCount = NodeCount;
		for(int i=0;i<8;i++)
		{
			for(int j=0;j<8;j++)
			{
				newboard[i][j] = board[i][j];
			}
		}
		do
		{
			do
			{
				cout << "Choose a Row to move from(0-7): ";
				cin >> row;
				cout << "Choose a Column to move from(0-7): "<<endl;
				cin >> column;
				cout <<"Choose a Row to make a move to that position(0-7): ";
				cin>>nrow;
				cout<<"Choose a Column to make a move to that position(0-7): ";
				cin>>ncolumn;
				if(!((row >=0 && row <=7) && (column >=0 && column <=7) && (nrow >=0 && nrow <=7) && (ncolumn >=0 && ncolumn <=7)))
				{
					cout<<"Invalid row and Column Selected"<<endl;
					flag = true;
				}
				else
				{
					flag = false;
				}
			}while(flag);
			Node test2;
			test2.setBoard(newboard);
			test2.setPlayer('W');
			test2.move = 0;
			test2.MoveGen();
			NodeCount -= test2.childs.size();
			if(nrow == 0)
			{
				newboard[nrow][ncolumn] = 'Z';
			}
			else
			{
				newboard[nrow][ncolumn] = newboard[row][column];
			}
			newboard[row][column] = '_';
			int count = 0;
			for(int i=0;i<test2.childs.size();i++)
			{
				if(checkPlayersEquality(newboard,test2.childs[i].board))
				{
					for(int j=0;j<8;j++)
					{
						for(int k=0;k<8;k++)
							board[j][k] = test2.childs[i].board[j][k];
					}
					count++;
					break;
				}
			}
			if(count == 0)
			{
				flag = true;
				cout<<"Action Cannot be made.Choose a Valid Action"<<endl;
				for(int i=0;i<8;i++)
				{
					for(int j=0;j<8;j++)
					{
						newboard[i][j] = board[i][j];
					}
				}
			}
			else
			{
				flag = false;
			}
		}while(flag);
		cout<<"After Your Turn board is:"<<endl;
		NodeCount -= tempNodeCount;
		moveCount++;
		printBoard(board);
		Node test3;
		test3.setBoard(board);
		test3.setPlayer('R');
		test3.move = 0;
		test3.MoveGen();
		NodeCount -= test3.childs.size();
		if(test3.terminal())
		{
			  endd = clock();
            double time_taken = double(endd - start) / double(CLOCKS_PER_SEC);
            cout << "Time taken by program is : " << fixed
                 << time_taken << setprecision(5);
            cout << " sec " << endl;
			cout<<"Length of Game Path:"<<moveCount<<endl;
			cout<<"No of Nodes Generated:"<<NodeCount<<endl;
			cout<<"No of Nodes Expanded :"<<NodeExpanded<<endl;
			int w = test3.utility();
			if(eval_choice2==1) w=max(w,test3.Eval1());
             else if(eval_choice2==2)  w=max(w,test3.Eval2());
             else if(eval_choice2==3)  w=max(w,test3.Eval3());
             else if(eval_choice2==4)  w=max(w,test3.Eval4());
			if(w == 20)
			{
				cout<<"You Won the game"<<endl;
				exit(0);
			}
		}
		if(moveCount >= 5)
		{
			//cout<<"rcount"<<rcount<<endl;
			//cout<<"wcount"<<wcount<<endl;
			if(wcount>rcount)
            {
                cout<<"Computer W Won the game"<<endl;
            }
            else{
                cout<<"Computer R Won the game"<<endl;
            }
			//cout<<"Game Tie"<<endl;
			cout<<"Length of Game Path:"<<moveCount<<endl;
			cout<<"No of Nodes Generated:"<<NodeCount<<endl;

			//cout<<"Game Tie"<<endl;
			//cout<<"Length of Game Path:"<<moveCount<<endl;
			//cout<<"No of Nodes Generated:"<<NodeCount<<endl;
			cout<<"No of Nodes Expanded:"<<NodeExpanded<<endl;
			exit(0);
		}
	}
}

void alpha_beta_search(Node n,char array2D[][8])
{
	n.v = max_value(n,-100,100);

	for (int i = 0; i < n.childs.size(); i++)
	{
		if (n.childs[i].v == n.v)
		{
			for(int h = 0; h < 8 ;h++)
			{
				for(int w = 0; w < 8; w++)
				{
					array2D[h][w] = n.childs[i].board[h][w];
				}
			}
			return ;
		}
	}
}
int max_value(Node &n,int alpha,int beta)
{
	n.MoveGen();
	if(n.terminal() == true)
	{
		n.v = n.utility();
		return n.utility();
	}
	n.v = -30;
    for(int i = 0; i < n.childs.size(); i++)
	{
		n.v = max(n.v,min_value(n.childs[i],alpha,beta));
		if (n.v >= beta)
		{
			return n.v;
		}
		alpha = max(alpha,n.v);
	}
	return n.v;
}
int min_value(Node &n,int alpha,int beta)
{
	n.MoveGen();
	if(n.terminal())
	{
		n.v = n.utility();
		return n.utility();
	}
	n.v = 30;
	for(int i = 0; i < n.childs.size(); i++)
	{
		n.v = min(n.v,max_value(n.childs[i],alpha,beta));
		if (n.v <= alpha)
		{
			return n.v;
		}
		beta = min(beta,n.v);
	}
	return n.v;
}


int main()
{

    char player = 'R';
	int row,column,nrow,ncolumn;
	bool flag = false;
	bool flag2 = true;
	char newboard[8][8];
	int  count = 0;
	char board[8][8] =
	{
		{'_','R','_','R','_','R','_','R'},
		{'R','_','R','_','R','_','R','_'},
		{'_','R','_','R','_','R','_','R'},
		{'_','_','_','_','_','_','_','_'},
		{'_','_','_','_','_','_','_','_'},
		{'W','_','W','_','W','_','W','_'},
		{'_','W','_','W','_','W','_','W'},
		{'W','_','W','_','W','_','W','_'}
	};

    do {
    cout<<"*********************************************************************************"<<endl;
    cout<<endl;
    cout<<"                           CHECKERS GAME ANALYSIS USING                      "<<endl;
    cout<<"                       ALPHA BETA SEARCH & MINIMAX-AB ALGORITHMS                         "<<endl;
    cout<<endl;

    cout<<"*********************************************************************************"<<endl;

    cout<<endl;
    cout<<"                                  MAIN MENU  "<<endl;
    cout<<endl<<"*********************************************************************************";
	cout<<endl<<"1.Select players for the game: (computer vs computer / user vs computer)";
	cout<<endl<<"2.Select the algorithms for the players 1 and 2";
	cout<<endl<<"3.Select the evaluation functions for players 1 and 2";
	cout<<endl<<"4.Play Game";
	cout<<endl<<"5.EXIT.";
	cout<<endl<<"Select your option:";

	cin>>choice;

	switch(choice)
	{
	case 1: cout<<endl<<"1.Computer vs computer\n";
            cout<<endl<<"2.User vs computer\n";
            cout<<endl<<"Enter the choice for players:";
            cin>>play_choice;
            cout<<endl<<"Now select the algorithms combination in option 2";
            getch();
		    break;
	case 2: cout<<endl<<"1.Alpha Beta search \n";
            cout<<endl<<"2.MinimaxAB\n";
            cout<<endl<<"Enter the choice for player 1:";
            cin>>algo_choice1;
            cout<<endl<<"Enter the choice for player 2:";
            cin>>algo_choice2;
            cout<<endl<<"Now select the evaluation functions combination for the players in option 3";
            getch();
		    break;
	case 3: cout<<endl<<"1.Evaluation 1\n";
            cout<<endl<<"2.Evaluation 2\n";
            cout<<endl<<"3.Evaluation 3\n";
            cout<<endl<<"4.Evaluation 4\n";
            cout<<endl<<"Enter the choice for player 1:";
            cin>>eval_choice1;
            cout<<endl<<"Enter the choice for player 2:";
            cin>>eval_choice2;
            cout<<endl<<"Now your ready to play checkers, go for option 4";
            getch();
		    break;
    case 4: switch(play_choice)
            {
                case 1: cout<<endl<<"comp vs comp";
                         if(algo_choice1==1 && algo_choice2==1)
                         {
                             Alpha_BetacomVsAlpha_Betacom(player,row,column,nrow,ncolumn,flag,flag2,board,newboard);
                         }
                         else if(algo_choice1==2 && algo_choice2==2)
                         {
                             MinMaxABcomVsMinMaxABcom(player,row,column,nrow,ncolumn,flag,flag2,board,newboard);
                         }
                         else
                         {
                             MinMaxABVsAlpha_Beta(player,row,column,nrow,ncolumn,flag,flag2,board,newboard);
                         }
                getch();
                        break;
                case 2: cout<<endl<<"comp vs user";
                         if(algo_choice1==1 && algo_choice2==1)
                         {
                             Alpha_BetacomVsAlpha_Betauser(player,row,column,nrow,ncolumn,flag,flag2,board,newboard);
                         }
                         else if(algo_choice1==2 && algo_choice2==2)
                         {
                             MinMaxABcomVsMinMaxABuser(player,row,column,nrow,ncolumn,flag,flag2,board,newboard);
                         }
                         else
                         {
                             MinMaxABVsAlpha_Beta(player,row,column,nrow,ncolumn,flag,flag2,board,newboard);
                         }
                getch();
                        break;
            }

		    break;
	case 5: exit(0);
            break;
    default: cout<<" Please select a valid choice!"<<endl;
             getch();
             break;
	  }
    system("cls");

	}while(choice!=5);


 return 0;

}
