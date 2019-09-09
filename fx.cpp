#include <bits/stdc++.h>
using namespace std ;
#define p2i pair<int,int>
#define p2p pair<p2i,p2i>
#define t3p tuple<p2i,p2i,p2i>

vector< vector<int> > board ;
vector<p2p> validMoves ;
vector<pair<t3p,t3p>> cannonCaptures ;
vector<pair<t3p,p2i>> cannonAttacks ;
vector<t3p> allCannons;
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

void executeMove(string move, bool player){                                //true indicates we are the player executing
    stringstream ss(move) ;
    int ix,iy,fx,fy ;
    char bomb;
    ss>>ix>>ix>>iy>>bomb>>fx>>fy ;
    if(id==2&&!player){
		ix=cols+1-ix ; iy=rows+1-iy ; fx=cols+1-fx; fy=rows+1-fy ;
    }
    if(bomb=='B'){
		board[fy][fx]=0 ;
    }
    else{
		board[iy][ix]=0;
		board[fy][fx]=(player)?1:-1 ;
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
	vector<p2p> validmoves ;
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			if(board[i][j] == 1){
				//forward
				if(validPosition(i-1,j)&& board[i-1][j]!=1) validmoves.push_back(make_pair(make_pair(i,j),make_pair(i-1,j))) ;
				if(validPosition(i-1,j+1)&& board[i-1][j+1]!=1) validmoves.push_back(make_pair(make_pair(i,j),make_pair(i-1,j+1))) ;
				if(validPosition(i-1,j-1)&& board[i-1][j-1]!=1) validmoves.push_back(make_pair(make_pair(i,j),make_pair(i-1,j-1))) ;
				
				//sideways
				if(validPosition(i,j-1)&& (board[i][j-1]==-1||board[i][j-1]==-2)) validmoves.push_back(make_pair(make_pair(i,j),make_pair(i,j-1))) ;
				if(validPosition(i,j+1)&& (board[i][j+1]==-1||board[i][j+1]==-2)) validmoves.push_back(make_pair(make_pair(i,j),make_pair(i,j+1))) ;
				
				//backward
				bool back=((validPosition(i-1,j-1)&& board[i-1][j-1]==-1)||
				   (validPosition(i-1,j)&& board[i-1][j]==-1)||
				   (validPosition(i-1,j+1)&& board[i-1][j+1]==-1)||
				   (validPosition(i,j-1)&& board[i][j-1]==-1)||
				   (validPosition(i,j+1)&& board[i][j+1]==-1)) ;
				if(back){
					if(validPosition(i+2,j+2)&& board[i+2][j+2]!=1 && board[i+2][j+2]!=2) validmoves.push_back(make_pair(make_pair(i,j),make_pair(i+2,j+2))) ;
					if(validPosition(i+2,j)&& board[i+2][j]!=1 && board[i+2][j]!=2) validmoves.push_back(make_pair(make_pair(i,j),make_pair(i+2,j))) ;
					if(validPosition(i+2,j-2)&& board[i+2][j-2]!=1 && board[i+2][j-2]!=2) validmoves.push_back(make_pair(make_pair(i,j),make_pair(i+2,j-2))) ;
				}
			}
		}
	}
	validMoves = validmoves ;
}

void getCannons(){
	vector<t3p> cannons;
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			if(validPosition(i,j-1) && validPosition(i,j) && validPosition(i,j+1) && board[i][j-1] == 1 && board[i][j] == 1 && board[i][j+1] == 1)
				{cannons.push_back(make_tuple(make_pair(i,j-1),make_pair(i,j),make_pair(i,j+1)));}
			if(validPosition(i-1,j) && validPosition(i,j) && validPosition(i+1,j) && board[i-1][j] == 1 && board[i][j] == 1 &&board[i+1][j] == 1)
				{cannons.push_back(make_tuple(make_pair(i-1,j),make_pair(i,j),make_pair(i+1,j)));}
			if(validPosition(i+1,j-1) && validPosition(i,j) && validPosition(i-1,j+1) && board[i+1][j-1] == 1 && board[i][j] == 1 &&board[i-1][j+1] == 1)
				{cannons.push_back(make_tuple(make_pair(i+1,j-1),make_pair(i,j),make_pair(i-1,j+1)));}
		}
	}
	allCannons = cannons;
}

void getCannonCaptures(){
	vector<pair<t3p,t3p>> cannoncap ;
	int ti0,tj0,ti2,tj2 ;
	for(auto i: allCannons){
		ti0 = 2*get<0>(i).first-get<1>(i).first ;
		tj0= 2*get<0>(i).second-get<1>(i).second ;
		ti2 = 2*get<2>(i).first-get<1>(i).first ;
		tj2= 2*get<2>(i).second-get<1>(i).second ;
		if(validPosition(ti0,tj0)&&board[ti0][tj0]==0) cannoncap.push_back(make_pair(i,make_tuple(make_pair(ti0,tj0),get<0>(i),get<1>(i)))) ;
		if(validPosition(ti2,tj2)&&board[ti2][tj2]==0) cannoncap.push_back(make_pair(i,make_tuple(make_pair(ti2,tj2),get<0>(i),get<1>(i)))) ;
	}
	cannonCaptures = cannoncap ;
}
   
void getCannonAttacks(){
	vector<pair<t3p,p2i>> almostCannonAttacks ;
	int di,dj,d1i,d1j,d2i,d2j ;
	for(auto cc: allCannons){
		di = 2*get<0>(cc).first - get<1>(cc).first ;
		dj = 2*get<0>(cc).second - get<1>(cc).second ;
		if(validPosition(di,dj)&&board[di][dj]==0){
			d1i = 2*di-get<0>(cc).first ;
			d1j = 2*dj-get<0>(cc).second ;
			d2i = 2*d1i-d1i ;
			d2j = 2*d1j-d1j ;
			if(validPosition(d1i,d1j)&&board[d1i][d1j]!=1 && board[d1i][d1j]!=2){almostCannonAttacks.push_back(make_pair(cc,make_pair(d1i,d1j))) ;}
			if(validPosition(d2i,d2j)&&board[d2i][d2j]!=1 && board[d2i][d2j]!=2){almostCannonAttacks.push_back(make_pair(cc,make_pair(d2i,d2j))) ;}
		}
		di = 2*get<2>(cc).first - get<1>(cc).first ;
		dj = 2*get<2>(cc).second - get<1>(cc).second ;
		if(validPosition(di,dj)&&board[di][dj]==0){
			d1i = 2*di-get<2>(cc).first ;
			d1j = 2*dj-get<2>(cc).second ;
			d2i = 2*d1i-d1i ;
			d2j = 2*d1j-d1j ;
			if(validPosition(d1i,d1j)&&board[d1i][d1j]!=1 && board[d1i][d1j]!=2){almostCannonAttacks.push_back(make_pair(cc,make_pair(d1i,d1j))) ;}
			if(validPosition(d2i,d2j)&&board[d2i][d2j]!=1 && board[d2i][d2j]!=2){almostCannonAttacks.push_back(make_pair(cc,make_pair(d2i,d2j))) ;}
		}
	}
	cannonAttacks = almostCannonAttacks ;
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
