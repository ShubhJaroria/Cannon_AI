#include <bits/stdc++.h>
using namespace std ;
#define p2i pair<int,int>
#define p2p pair<p2i,p2i>
#define t3p tuple<p2i,p2i,p2i>

vector< vector<int> > board ;
vector<p2p> validMoves ;
vector<p2p> cannonCaptures ;
vector<pair<t3p,p2i>> cannonAttacks ;
vector<t3p> allCannons;
vector<string> allMovesString ;
vector<p2p> killerTownHallMoves ; //editing
vector<p2p> killerPlayerMoves ; //editing
vector<p2p> killableSoldiers; //editing
vector<p2p> attackableTownHalls; //editing
int rows ;
int cols ; 
int id ;
int countSoldierUS ;
int countSoldierEnemy ;
int countTHUS ;
int countTHEnemy ;
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
	countSoldierUS = 3*cols/2 ;
	countSoldierEnemy = 3*cols/2 ;
	countTHUS = cols/2 ;
	countTHEnemy = cols/2 ;
}

string reverseMove(string move){
	string rev="" ;
	stringstream ss(move) ;
	int ix,iy,fx,fy ;
	char bomb;
	ss>>bomb>>ix>>iy>>bomb>>fx>>fy ;
	//cout<<ix<<iy<<fx<<fy<<endl ;
	rev=move ;
	rev[2]=(char) (cols-1-ix +48);
	rev[4] = (char) (rows-1-iy +48);
	rev[8] =(char) (cols-1-fx + 48 );
	rev[10]=(char)(rows-1-fy + 48 );
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

void getAllMovesString(){
	vector<string> s_moves ;
	for(auto i:validMoves){
		string loc="" ;
		loc+="S "+to_string(i.first.second)+" "+to_string(i.first.first)+" M "+to_string(i.second.second)+" "+to_string(i.second.first) ;
		s_moves.push_back(loc) ;
	}
	for(auto i:cannonCaptures){
		string loc="" ;
		loc+="S "+to_string(i.first.second)+" "+to_string(i.first.first)+" M "+to_string(i.second.second)+" "+to_string(i.second.first) ;
		s_moves.push_back(loc) ;		
	}
	for(auto i:cannonAttacks){
		p2i base=get<1>(i.first) ;
		string loc="" ;
		loc+="S "+to_string(base.second)+" "+to_string(base.first)+" B "+to_string(i.second.second)+" "+to_string(i.second.first) ;
		s_moves.push_back(loc) ;
	}
	allMovesString=s_moves ;
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
	vector<p2p> cannoncap ;
	int ti0,tj0,ti2,tj2 ;
	for(auto i: allCannons){
		ti0 = 2*get<0>(i).first-get<1>(i).first ;
		tj0= 2*get<0>(i).second-get<1>(i).second ;
		ti2 = 2*get<2>(i).first-get<1>(i).first ;
		tj2= 2*get<2>(i).second-get<1>(i).second ;
		if(validPosition(ti0,tj0)&&board[ti0][tj0]==0) cannoncap.push_back(make_pair(get<2>(i),make_pair(ti0,tj0))) ;
		if(validPosition(ti2,tj2)&&board[ti2][tj2]==0) cannoncap.push_back(make_pair(get<0>(i),make_pair(ti2,tj2))) ;
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
			d2i = 2*d1i-di ;
			d2j = 2*d1j-dj ;
			if(validPosition(d1i,d1j)&&board[d1i][d1j]!=1 && board[d1i][d1j]!=2){almostCannonAttacks.push_back(make_pair(cc,make_pair(d1i,d1j))) ;}
			if(validPosition(d2i,d2j)&&board[d2i][d2j]!=1 && board[d2i][d2j]!=2){almostCannonAttacks.push_back(make_pair(cc,make_pair(d2i,d2j))) ;}
		}
		di = 2*get<2>(cc).first - get<1>(cc).first ;
		dj = 2*get<2>(cc).second - get<1>(cc).second ;
		if(validPosition(di,dj)&&board[di][dj]==0){
			d1i = 2*di-get<2>(cc).first ;
			d1j = 2*dj-get<2>(cc).second ;
			d2i = 2*d1i-di ;
			d2j = 2*d1j-dj ;
			if(validPosition(d1i,d1j)&&board[d1i][d1j]!=1 && board[d1i][d1j]!=2){almostCannonAttacks.push_back(make_pair(cc,make_pair(d1i,d1j))) ;}
			if(validPosition(d2i,d2j)&&board[d2i][d2j]!=1 && board[d2i][d2j]!=2){almostCannonAttacks.push_back(make_pair(cc,make_pair(d2i,d2j))) ;}
		}
	}
	cannonAttacks = almostCannonAttacks ;
}

void update(){
	getValidMoves() ;
	getCannons() ;
	getCannonCaptures() ;
	getCannonAttacks() ;
	getAllMovesString() ;
}
void printBoard(){
	for(auto i:board){
		for(auto j:i) cout<<j<<" ";
		cout<<endl ;
	}
}
void executeMove(string move, bool player){                                //true indicates we are the player executing
    //cout<<move<<endl ;
    vector<p2p> soldiersBeingKilled, townHallsBeingDestroyed;
    stringstream ss(move) ;
    int ix,iy,fx,fy ;
    char bomb;
    ss>>bomb>>ix>>iy>>bomb>>fx>>fy ;
        //cout<<bomb<<endl ;
    if(id==2){
                ix=cols-1-ix ; iy=rows-1-iy ; fx=cols-1-fx; fy=rows-1-fy ;
    }
        if(board[fy][fx]==-2)countTHEnemy-- ;
        if(board[fy][fx]==2)countTHUS-- ;
        if(board[fy][fx]==1)countSoldierUS-- ;
        if(board[fy][fx]==-1)countSoldierEnemy-- ;
    if(bomb=='B'){
                board[fy][fx]=0 ;
    }
    else{
                board[iy][ix]=0;
                board[fy][fx]=(player) ? 1 : -1 ;
                //cout<<"Yo" ;
		if(player == false){
				//forward
				if(validPosition(fx+1,fy)&& board[fx+1][fy]==1) soldiersBeingKilled.push_back(make_pair(make_pair(fx,fy),make_pair(fx+1,fy))) ;
				if(validPosition(fx+1,fy+1)&& board[fx+1][fy+1]==1) soldiersBeingKilled.push_back(make_pair(make_pair(fx,fy),make_pair(fx+1,fy+1))) ;
				if(validPosition(fx+1,fy-1)&& board[fx+1][fy-1]==1) soldiersBeingKilled.push_back(make_pair(make_pair(fx,fy),make_pair(fx+1,fy-1))) ;

				if(validPosition(fx+1,fy)&& board[fx+1][fy]==2) townHallsBeingDestroyed.push_back(make_pair(make_pair(fx,fy),make_pair(fx+1,fy))) ;
				if(validPosition(fx+1,fy+1)&& board[fx+1][fy+1]==2) townHallsBeingDestroyed.push_back(make_pair(make_pair(fx,fy),make_pair(fx+1,fy+1))) ;
				if(validPosition(fx+1,fy-1)&& board[fx+1][fy-1]==2) townHallsBeingDestroyed.push_back(make_pair(make_pair(fx,fy),make_pair(fx+1,fy-1))) ;
				
				//sideways
				if(validPosition(fx,fy-1)&& (board[fx][fy-1]==1)) soldiersBeingKilled.push_back(make_pair(make_pair(fx,fy),make_pair(fx,fy-1))) ;
				if(validPosition(fx,fy-1)&& (board[fx][fy-1]==2)) townHallsBeingDestroyed.push_back(make_pair(make_pair(fx,fy),make_pair(fx,fy-1))) ;				

				if(validPosition(fx,fy+1)&& (board[fx][fy+1]==1)) soldiersBeingKilled.push_back(make_pair(make_pair(fx,fy),make_pair(fx,fy+1))) ;
				if(validPosition(fx,fy+1)&& (board[fx][fy+1]==2)) townHallsBeingDestroyed.push_back(make_pair(make_pair(fx,fy),make_pair(fx,fy+1))) ;				

				//backward
				bool back=((validPosition(fx+1,fy-1)&& board[fx+1][fy-1]==-1)||
				   (validPosition(fx+1,fy)&& board[fx+1][fy]==-1)||
				   (validPosition(fx+1,fy+1)&& board[fx+1][fy+1]==-1)||
				   (validPosition(fx,fy-1)&& board[fx][fy-1]==-1)||
				   (validPosition(fx,fy+1)&& board[fy][fy+1]==-1)) ;
				if(back){
					if(validPosition(fx-2,fy+2)&& (board[fx-2][fy+2]==1)) soldiersBeingKilled.push_back(make_pair(make_pair(fx,fy),make_pair(fx-2,fy+2))) ;
					if(validPosition(fx-2,fy+2)&& (board[fx-2][fy+2]==2)) townHallsBeingDestroyed.push_back(make_pair(make_pair(fx,fy),make_pair(fx-2,fy+2))) ;
					if(validPosition(fx-2,fy)&& (board[fx-2][fy]==1)) soldiersBeingKilled.push_back(make_pair(make_pair(fx,fy),make_pair(fx-2,fy))) ;
					if(validPosition(fx-2,fy-2)&& (board[fx-2][fy-2]==1)) soldiersBeingKilled.push_back(make_pair(make_pair(fx,fy),make_pair(fx-2,fy))) ;
					if(validPosition(fx-2,fy-2)&& (board[fx-2][fy-2]==2)) townHallsBeingDestroyed.push_back(make_pair(make_pair(fx,fy),make_pair(fx-2,fy-2))) ;
				}
		}
	}
	killableSoldiers = soldiersBeingKilled;
	attackableTownHalls = townHallsBeingDestroyed;	       
}

string getMove(){
	int i = rand()%allMovesString.size() ;
	return (id==1) ? allMovesString[i]: reverseMove(allMovesString[i]) ;
}

string getGreedyMove(){  //editing
	int n = validMoves.size();
	vector<p2p> killingPlayerMoves, killingTownHallMoves;
	p2p pos_pair;
	for(int i = 0; i < n; i++){
		pos_pair = validMoves[i];
		if(board[pos_pair.second.first][pos_pair.second.second] == -2){
			killingTownHallMoves.push_back(pos_pair);
		}
		if(board[validMoves[i].second.first][validMoves[i].second.second] == -1){
			killingPlayerMoves.push_back(pos_pair);
		}
	}
	killerTownHallMoves = killingTownHallMoves;
	killerPlayerMoves = killingPlayerMoves;
	if(killerTownHallMoves.size() != 0){pos_pair = killerTownHallMoves[rand()%killerTownHallMoves.size()];}
	else if(killerPlayerMoves.size() != 0){pos_pair = killerPlayerMoves[rand()%killerPlayerMoves.size()];}
	else if(cannonAttacks.size() != 0){pos_pair = make_pair(get<0>(cannonAttacks[0].first),cannonAttacks[0].second);}
	else if(cannonCaptures.size() != 0){pos_pair = cannonCaptures[rand()%cannonCaptures.size()];}
	else{pos_pair = validMoves[rand()%validMoves.size()];}
	return "S " + to_string(pos_pair.first.first) + " " + to_string(pos_pair.first.second)
				 + " M " + to_string(pos_pair.second.first) + " " + to_string(pos_pair.second.second);
}

int main(){
        string config ;
		int x = unsigned(time(0))%32767; srand(x) ;
        getline(cin,config) ;
        stringstream ss(config) ;
        ss>>id ; ss>>rows ; ss>>cols ; ss>>duration;
        board = vector<vector<int> >(rows,vector<int>(cols,0));
        initialize() ;
		update() ;
		string enemyMove,ourMove;
		if(id == 2){
			getline(cin,enemyMove);
			executeMove(enemyMove, false);
			update() ;
		}
		//printBoard() ;
		//for(auto i: allCannons) cout<<get<0>(i).second<<","<<get<0>(i).first<<" "<<get<1>(i).second<<","<<get<1>(i).first<<" "<<get<2>(i).second<<","<<get<2>(i).first<<endl ;
		//for(auto i: cannonAttacks) cout<<get<0>(i.first).second<<","<<get<0>(i.first).first<<" "<<get<1>(i.first).second<<","<<get<1>(i.first).first<<" "<<get<2>(i.first).second<<","<<get<2>(i.first).first<<":  "<<i.second.second<<","<<i.second.first<<endl ;
		//cout<<"break"<<endl ;
		//for(auto i:allMovesString) cout<<i<<endl ;
		//executeMove(getMove(),true);
		//printBoard() ;
		//cout<<reverseMove("S 0 2 M 1 4") ;
		while(countTHEnemy>2 && countSoldierUS>2){
            ourMove = getMove();
        	executeMove(ourMove,true);
            update() ;
            //printBoard() ;
            cout<<ourMove<<endl;
            getline(cin,enemyMove);
            executeMove(enemyMove, false);
            //cout<<"Enemy move: "<<enemyMove<<endl ;
            update() ;
		}
        return 0 ;
}
