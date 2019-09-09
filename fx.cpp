#include <bits/stdc++.h>
#define p2p pair<pair<int,int>,pair<int,int>>
using namespace std ;

vector< vector<int> > board ;
set<pair<pair<int,int>,pair<int,int> > > validMoves ;
int rows ;
int cols ; 
int id ;
int duration ;

void initialize(){
        for(int i=0;i<cols;i++){
                if(i%2==0) {
                        board[cols-1][i] =1;
                        board[cols-2][i] =1;
                        board[cols-3][i] =1;
                        board[0][i]=-2 ;
                }
                else{
                        board[cols-1][i]=2 ;
                        board[0][i] =-1;
                        board[1][i] =-1;
                        board[2][i] =-1;
                }
        }
}

void executeEnemyMove(string move){
    stringstream ss(move) ;
    int ix,iy,fx,fy ;
    char bomb;
    ss>>ix>>ix>>iy>>bomb>>fx>>fy ;
    if(id==2){
		ix=cols+1-ix ; iy=rows+1-iy ; fx=cols+1-fx; fy=rows+1-fy ;
    }
    if(bomb=='B'){
		board[fy][fx]=0 ;
    }
    else{
		board[iy][ix]=0;
		board[fy][fx]=-1 ;
    }	
}

string reverseMove(string move){
	string rev="" ;
	stringstream ss(move) ;
	int ix,iy,fx,fy ;
	char bomb;
	ss>>ix>>ix>>iy>>bomb>>fx>>fy ;
	rev=move ;
	rev[1]=(char) (cols+1-ix);
	rev[2] = (char) (rows+1-iy) ;
	rev[4] =(char) (cols+1-fx) ;
	rev[5]=(char)(rows+1-fy) ;
	return rev ; 
}

void moveSoldier(p2p positions){
	pair<int,int> soldier_pos = positions.first;
	pair<int,int> final_pos = positions.second;
	int player = board[soldier_pos.first][soldier_pos.second];
	board[soldier_pos.first][soldier_pos.second] = 0;
	board[final_pos.first][final_pos.second] = player;
}

bool validPosition(int i, int j){
return ((i < rows) && (j < cols) && (i>=0) && (j>=0));     
}

vector<p2p> getValidMoveSingle(int i,int j){
	//forward
	vector<p2p> validmoves ;
	if(validPosition(i-1,j)&& board[i-1][j]!=1) validmoves.push_back(make_pair(make_pair(i,j),make_pair(i-1,j))) ;
	if(validPosition(i-1,j+1)&& board[i-1][j+1]!=1) validmoves.push_back(make_pair(make_pair(i,j),make_pair(i-1,j+1))) ;
	if(validPosition(i-1,j-1)&& board[i-1][j-1]!=1) validmoves.push_back(make_pair(make_pair(i,j),make_pair(i-1,j-1))) ;
	
	//sideways
	if(validPosition(i,j-1)&& (board[i][j-1]==-1||board[i][j-1]==-2)) validmoves.push_back(make_pair(make_pair(i,j),make_pair(i,j-1))) ;
	if(validPosition(i,j+1)&& (board[i][j+1]==-1||board[i][j+1]==-2)) validmoves.push_back(make_pair(make_pair(i,j),make_pair(i,j+1))) ;
	
	//backward
	bool back=false ;
	if((validPosition(i-1,j-1)&& board[i-1][j-1]==-1)||
	   (validPosition(i-1,j)&& board[i-1][j]==-1)||
	   (validPosition(i-1,j+1)&& board[i-1][j+1]==-1)||
	   (validPosition(i,j-1)&& board[i][j-1]==-1)||
	   (validPosition(i,j+1)&& board[i][j+1]==-1)){
		if(validPosition(i+2,j+2)&& board[i+2][j+2]!=1 && board[i+2][j+2]!=2) validmoves.push_back(make_pair(make_pair(i,j),make_pair(i+2,j+2))) ;
		if(validPosition(i+2,j)&& board[i+2][j]!=1 && board[i+2][j]!=2) validmoves.push_back(make_pair(make_pair(i,j),make_pair(i+2,j))) ;
		if(validPosition(i+2,j-2)&& board[i+2][j-2]!=1 && board[i+2][j-2]!=2) validmoves.push_back(make_pair(make_pair(i,j),make_pair(i+2,j-2))) ;
	}
	return validmoves; 
}

void getValidMoves(){
	set<p2p> validmoves ;
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			if(board[i][j] == 1){
				//forward
				if(validPosition(i-1,j)&& board[i-1][j]!=1) validmoves.insert(make_pair(make_pair(i,j),make_pair(i-1,j))) ;
				if(validPosition(i-1,j+1)&& board[i-1][j+1]!=1) validmoves.insert(make_pair(make_pair(i,j),make_pair(i-1,j+1))) ;
				if(validPosition(i-1,j-1)&& board[i-1][j-1]!=1) validmoves.insert(make_pair(make_pair(i,j),make_pair(i-1,j-1))) ;
				
				//sideways
				if(validPosition(i,j-1)&& (board[i][j-1]==-1||board[i][j-1]==-2)) validmoves.insert(make_pair(make_pair(i,j),make_pair(i,j-1))) ;
				if(validPosition(i,j+1)&& (board[i][j+1]==-1||board[i][j+1]==-2)) validmoves.insert(make_pair(make_pair(i,j),make_pair(i,j+1))) ;
				
				//backward
				bool back=((validPosition(i-1,j-1)&& board[i-1][j-1]==-1)||
				   (validPosition(i-1,j)&& board[i-1][j]==-1)||
				   (validPosition(i-1,j+1)&& board[i-1][j+1]==-1)||
				   (validPosition(i,j-1)&& board[i][j-1]==-1)||
				   (validPosition(i,j+1)&& board[i][j+1]==-1)) ;
				if(back){
					if(validPosition(i+2,j+2)&& board[i+2][j+2]!=1 && board[i+2][j+2]!=2) validmoves.insert(make_pair(make_pair(i,j),make_pair(i+2,j+2))) ;
					if(validPosition(i+2,j)&& board[i+2][j]!=1 && board[i+2][j]!=2) validmoves.insert(make_pair(make_pair(i,j),make_pair(i+2,j))) ;
					if(validPosition(i+2,j-2)&& board[i+2][j-2]!=1 && board[i+2][j-2]!=2) validmoves.insert(make_pair(make_pair(i,j),make_pair(i+2,j-2))) ;
				}
			}
		}
	}
	validMoves = validmoves ;
}

int main(){
        string config ;
        getline(cin,config) ;
        stringstream ss(config) ;
        ss>>id ; ss>>rows ; ss>>cols ; ss>>duration ;
        board = vector<vector<int> >(rows,vector<int>(cols,0)) ;
        initialize() ;
        return 0 ;
}