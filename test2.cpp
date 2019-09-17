#include <bits/stdc++.h>
using namespace std ;
#define p2i pair<int,int>
#define p2p pair<p2i,p2i>
#define SHORTMAX 32767
#define SHORTMIN -32767
#define DOUBLEMAX 100000000.0
#define DOUBLEMIN -100000000.0
#define mdp(a,b,c,d) make_pair(make_pair(a,b),make_pair(c,d))
#define t3p tuple<p2i,p2i,p2i>

vector< vector<int> > booard ;
vector<p2p> validMoves ;
vector<p2p> cannonCaptures ;
vector<pair<t3p,p2i>> cannonAttacks ;
vector<t3p> allCannons;
vector<t3p> allCannonsEnemy ;
vector<string> allMovesString ;
vector<p2p> killerTownHallMoves ; //editing
vector<p2p> killerPlayerMoves ; //will store moves which would kill a player 
vector<p2p> killerCannonMoves ;
vector<p2p> killableSoldiers; //ours
vector<p2p> attackableTownHalls; //editing
int PLAYERID ; //0 for us being player 2, 1 for us being player 1
int rows ;
int cols ; 
int countSoldierUS ;
int countSoldierEnemy ;
int countTHUS ;
int countTHEnemy ;
int duration ;
int DEPTH=3 ;

double soldier_near_soldier = -2.0 ; //done
double soldier_near_townhall = 2.5 ; //done
//double defensive_benefits=0.1 ; //in th
double soldier_support=0.75 ; //done
double living_soldier = 7.0 ; //done
double mobility_score=0.1; //done
//double being_attacked_by_cannon = -5.0 ;
//double blocking_cannon_benefits= 1.2 ; //in sp
double oneblock_factor = 2.0 ; //done
double twoblock_factor = 1.5 ; //done

double living_cannon=2.5 ;
double soldier_in_zone= 6.0 ;
double townhall_in_zone= 15.0 ;
double townhall_in_approach= 0.0 ;
double soldier_in_vicinity=0.7 ;
double blocked_factor = 0.2 ;

double alive_townhall=30.0 ;
double defending_soldiers_1=2.2 ;
double defending_soldiers_2=1.5 ;
double defending_soldiers_ex=0.5 ;
double defense_boost= 2.0 ;
///round off mean 
///Don;t forget to reinitialize all variables once their main functions are called
///alpha beta pruning mein sort
///Redundant moves ko manually hatao
///Scoring function ko set karo

void initialize(vector<vector<int>> &board){
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
                       board[2][i] = -1;
                }
        }
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

class gameBoard{ 
public:
	int rows ;	//done
	int cols ;	//done
	bool player ; //1 for US, 0 for Enemy
	bool chance ; //1 for US, 0 for Enemy 
	
	gameBoard* parent ;
	vector< vector<int> > board ; //done
	vector<p2p> validMoves ; //Done
	vector<p2p> validMoves_Enemy ;	//done
	vector<p2p> cannonCaptures ; //done	
	vector<p2p> cannonCaptures_Enemy ; //done
	vector<pair<t3p,p2i>> cannonAttacks ; //done
	vector<pair<t3p,p2i>> cannonAttacks_Enemy ; //done
		int soldierMatrix[8][8] ;	
	vector<t3p> cannons; //done
	vector<t3p> cannons_Enemy ; //done
	int blockedCannonsUS ; //done
	int blockedCannonsEnemy ; //done
	int soldiers ; //done
	int soldiers_Enemy ; //done
	int townhall ;
	int townhall_Enemy ;
	int diffSoldiers ;	//tobedone US-Enemy
	
	int soldier2soldier ; //done
	int cannon2soldier ; //done
	int cannon2cannon ; //done
	int soldier2cannon ;
	int soldier2soldier_Enemy ;	//done
	int cannon2soldier_Enemy ; //done
	int cannon2cannon_Enemy ; //done
	int soldier2cannon_Enemy ;

	int cannonScore ;
	int cannonScore_Enemy ;
	int soldierScore_Enemy ; //should include support score
	int soldierScore ;
	int townHallScore ;
	int townHallScore_Enemy ;
	
	int killTownHallMovesBySoldier ; //done
	int killTownHallMovesBySoldier_Enemy ; //done
	int killTownHallMovesByCannon ; //done
	int killTownHallMovesByCannon_Enemy ; //done
	
	//done all
	vector<int> distsoldierUS_townhallUS ;
	vector<int> distsoldierUS_townhallEnemy ;
	vector<int> distsoldierEnemy_townhallUS ;
	vector<int> distsoldierEnemy_townhallEnemy ;
	
	int scattering ; //tobedone
	int scattering_Enemy ; //tobedone
	
	p2i meanUS ;	//done
	p2i meanEnemy ;	//done
	vector<p2p> supportingSoldiers ; //done
	vector<p2p> supportingSoldiers_Enemy ; //done
	int totalSupport ; //tobedone
	int totalSupport_Enemy ; //tobedone
	double Heuristic=0.0 ;	//edited 2:30
	
	vector<double> weights{ 1,2,3,4,5,6 };
    double heuristicFunction2(){
			return soldierScore+cannonScore+townHallScore-soldierScore_Enemy-cannonScore_Enemy-townHallScore_Enemy ;
	}
	double heuristicFunction(){

			//int local = -20*soldiers_Enemy+20*soldiers -40*townhall_Enemy+50*townhall +pow(-1,!chance)*4*(soldier2soldier+soldier2soldier_Enemy) + 12*cannon2soldier-12*cannon2soldier_Enemy ;
			int local = -25*soldiers_Enemy+25*soldiers -80*townhall_Enemy+120*townhall+0*soldier2soldier+12*cannon2soldier-12*cannon2soldier_Enemy-25*killTownHallMovesByCannon_Enemy-15*killTownHallMovesBySoldier_Enemy+40*killTownHallMovesByCannon+15*killTownHallMovesBySoldier;
			/*int local= -12*blockedCannonsUS+12*blockedCannonsEnemy + 4*(12-soldiers_Enemy) -4*(12-soldiers) +7*cannons.size()-7*cannons_Enemy.size()
			+10*(cannon2cannon-cannon2cannon_Enemy)+40*killTownHallMovesByCannon-40*killTownHallMovesByCannon_Enemy+15*killTownHallMovesBySoldier
			-15*killTownHallMovesBySoldier_Enemy+6*(cannon2soldier-cannon2soldier_Enemy)+3*soldier2soldier-3*soldier2soldier_Enemy;*/
			return local*1.0;
	}

	gameBoard(int m,int n, bool chance):rows(m),cols(n),player(!chance),chance(chance){ //nochange
	//in this constructor you are not giving move this means... the chance is parameter corresponding to next player to the player's move you will give..
		board= vector<vector<int>>(rows,vector<int>(cols,0)) ;
		for(int i=0;i<cols;i++){
			if(i%2==0){
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
		initSoldiers() ;
		initCannons() ;
	}
				
	gameBoard(gameBoard* initialBoard, string implmove, bool chance){ //edited 2:23 chance is opposite	 of player, i.e. move and player is of same
		//You will give move... player is whose move is going to be executed ASAP.. All parameters are after the execution.. chance is the next player
		parent=initialBoard ;
		board = parent->board ;
		this->chance = chance ;
		//player=1, i.e You should be giving input 1 in chance and our move in implmove
		this->player=!chance ;
		rows=board.size() ;
		cols=board[0].size() ;
		executeMove(implmove) ;
	}
	
	void executeMove(string move){	 //edited 2:23
		//cout<<"IN execute move with move: "<<move<<endl ;
		stringstream ss(move) ;
		int ix,iy,fx,fy ;
		char bomb;
		ss>>bomb>>ix>>iy>>bomb>>fx>>fy ;
		if(bomb=='B'){
			board[fy][fx]=0 ;
		}
		else{
			//cout<<"In move"<<endl ;
			board[iy][ix]=0;
			board[fy][fx]=(player) ? 1 : -1 ;
		}
		//if(player) cout<<"It was my move"<<endl;
		//else cout<<"It was Enemy move"<<endl ;
		//cout<<"call" ;
		initSoldiers() ;
		//cout<<" called call" ;
		initCannons() ;
		//cout<<" called"<<endl ;
	}
	
	void swap(){
		this->chance=!this->chance ;
		this->player=!this->player ;
	}
	//IN following function adjusting move is important ;
	string minimax_move(){
		if(chance!=1) throw "Not Applicable for Enemy! Only one sided player" ;
		//cout<<chance<<endl ;
		return minimaxCall(this,DEPTH,chance,DOUBLEMIN,DOUBLEMAX).second ;
	}
	
	
	void printBoard(){
			for(auto i:board){
					for(auto j:i) cout<<j<<" ";
					cout<<endl ;
			}
	}
	
	void printStats(){
		printBoard() ;
		cout<<"Player: "<<player<<endl ;
		cout<<"Chance: "<<chance<<endl ;
		cout<<"Dimensions: "<<"rows="<<rows<<" cols="<<cols<<endl ;
		cout<<"Soldiers: "<<soldiers<<endl ;
		cout<<"EnemySoldiers: "<<soldiers_Enemy<<endl ;
		cout<<"soldier2soldier: "<<soldier2soldier<<endl ;
		cout<<"soldier2soldier_Enemy: "<<soldier2soldier_Enemy<<endl ;
		cout<<"Enemy_US:"<<distsoldierEnemy_townhallUS[0]<<", Enemy_Enemy:"<<distsoldierEnemy_townhallEnemy[0]<<", US_Enemy:"<<distsoldierUS_townhallEnemy[0]<<", US_US:"<<distsoldierUS_townhallUS[0]<<endl ;
		cout<<"MeanUS: "<<meanUS.second<<","<<meanUS.first<<endl;		 
		cout<<"MeanEnemy: "<<meanEnemy.second<<","<<meanEnemy.first<<endl;
		//cout<<"CannonAttacks: "<<cannonAttacks<<endl ;
		//cout<<"CannonCaptures: "<<cannonCaptures<<endl;
		//cout<<"CannonAttacksEnemy: "<<cannonAttacks_Enemy<<endl;
		//cout<<"CannonCapturesEnemy: "<<cannonCaptures<<endl;
		cout<<"ALL cannons: "<<cannons.size() ;
		cout<<"Enemy cannons: "<<cannons_Enemy.size() ;
		cout<<"blockedCannonsUS: "<<blockedCannonsUS<<endl;
		cout<<"blockedCannonsEnemy: "<<blockedCannonsEnemy<<endl;
		cout<<"killTownHallMovesbyCannon: "<<killTownHallMovesByCannon<<endl ;
		cout<<"killTownHallMovesbyCannonEnemy: "<<killTownHallMovesByCannon_Enemy<<endl ;
		cout<<"cannon2cannon: "<<cannon2cannon<<endl;
		cout<<"cannon2cannonEnemy: "<<cannon2cannon_Enemy<<endl;
		cout<<"cannon2soldier: "<<cannon2soldier <<endl;
		cout<<"cannon2soldier_enemy: "<<cannon2soldier_Enemy<<endl;
					
	}
	
	bool validPosition(int i, int j){
	return ((i < rows) && (j < cols) && (i>=0) && (j>=0));	   
	}

	int computeDistance(int i, int j, int i2, int j2){
		return abs(i-i2)+ abs(j-j2) ;
	}
	
	void initSoldiers(){
		//cout<<"In soldiersinit"<<endl ;
		soldiers=0 ;
		soldiers_Enemy=0 ;
		townhall=0;
		townhall_Enemy=0;
		diffSoldiers=0 ;	//tobedone US-Enemy	
		soldier2soldier=0 ;
		soldier2soldier_Enemy=0 ;
		killTownHallMovesBySoldier=0;
		killTownHallMovesBySoldier_Enemy=0;
		scattering=0 ;	
		scattering_Enemy=0 ;	
		supportingSoldiers.clear() ;
		supportingSoldiers_Enemy.clear() ;
		validMoves.clear() ;
		validMoves_Enemy.clear() ;
		distsoldierEnemy_townhallEnemy.clear() ;
		distsoldierEnemy_townhallUS.clear() ;
		distsoldierUS_townhallUS.clear() ;
		distsoldierUS_townhallEnemy.clear() ;
		meanUS = make_pair(0,0) ;
		meanEnemy = make_pair(0,0) ;
		distsoldierUS_townhallUS = vector<int>(cols/2,0) ;
		distsoldierUS_townhallEnemy = vector<int>(cols/2,0) ;
		distsoldierEnemy_townhallUS = vector<int>(cols/2,0) ;
		distsoldierEnemy_townhallEnemy = vector<int>(cols/2,0) ;
		
		for(int i=0;i<rows;i++){
			for(int j=0;j<cols;j++){
				if(board[i][j]==1){
						soldiers++ ;
				//distances, townHallwale
					for(int k=0;k<cols/2;k++){
						if(board[0][2*k]==-2) distsoldierUS_townhallEnemy[k]+=computeDistance(i,j,0,2*k) ;
						if(board[rows-1][2*k+1]==2) distsoldierUS_townhallUS[k]+=computeDistance(i,j,rows-1,2*k+1) ;
					}
					
				//means - mean,meanEnemy
					meanUS.first+=i ;
					meanUS.second+=j ;
				// moves calculation ,soldier2soldier, KileerTownhallmoves, supporting soldiers
					
					//forward
					if(validPosition(i-1,j)&& board[i-1][j]!=1&&board[i-1][j]!=2) {
						validMoves.push_back(mdp(i,j,i-1,j)) ;
						if(board[i-1][j]==-1) soldier2soldier++ ;
						if(board[i-1][j]==-2) killTownHallMovesBySoldier++ ;
					}
					if(validPosition(i-1,j+1)&& board[i-1][j+1]!=1&&board[i-1][j+1]!=2){
						validMoves.push_back(mdp(i,j,i-1,j+1)) ;
						if(board[i-1][j+1]==-1) soldier2soldier++ ;
						if(board[i-1][j+1]==-2) killTownHallMovesBySoldier++ ;
					}
					if(validPosition(i-1,j-1)&& board[i-1][j-1]!=1&&board[i-1][j-1]!=2){
						validMoves.push_back(mdp(i,j,i-1,j-1)) ;
						if(board[i-1][j-1]==-1) soldier2soldier++ ;
						if(board[i-1][j-1]==-2) killTownHallMovesBySoldier++ ;
					}
				
				//sideways
					if(validPosition(i,j-1)&&board[i][j-1]!=2&&board[i][j-1]!=0){
						if(board[i][j-1]!=1){
							validMoves.push_back(mdp(i,j,i,j-1)) ;
							if(board[i][j-1]==-1) soldier2soldier++ ;
							if(board[i][j-1]==-2) killTownHallMovesBySoldier++ ;				
						}
						else{
							supportingSoldiers.push_back(mdp(i,j,i,j-1)) ;
						}
					}
					if(validPosition(i,j+1)&&board[i][j+1]!=2&&board[i][j+1]!=0){
						if(board[i][j+1]!=1){
							validMoves.push_back(mdp(i,j,i,j+1)) ;
							if(board[i][j+1]==-1) soldier2soldier++ ;
							if(board[i][j+1]==-2) killTownHallMovesBySoldier++ ;
						}
						else{
							supportingSoldiers.push_back(mdp(i,j,i,j+1)) ;
						}
					}
					
				//backward
					if((validPosition(i-1,j-1)&& board[i-1][j-1]==-1)||
					   (validPosition(i-1,j)&& board[i-1][j]==-1)||
					   (validPosition(i-1,j+1)&& board[i-1][j+1]==-1)||
					   (validPosition(i,j-1)&& board[i][j-1]==-1)||
					   (validPosition(i,j+1)&& board[i][j+1]==-1)){
							if(validPosition(i+2,j+2)&& board[i+2][j+2]!=1 && board[i+2][j+2]!=2){
								validMoves.push_back(mdp(i,j,i+2,j+2)) ;
								if(board[i+2][j+2]==-1) soldier2soldier++ ;
							}
							if(validPosition(i+2,j)&& board[i+2][j]!=1 && board[i+2][j]!=2){
								validMoves.push_back(mdp(i,j,i+2,j)) ;
								if(board[i+2][j]==-1) soldier2soldier++ ;							
							}
							if(validPosition(i+2,j-2)&& board[i+2][j-2]!=1 && board[i+2][j-2]!=2){
								validMoves.push_back(mdp(i,j,i+2,j-2)) ;
								if(board[i+2][j-2]==-1) soldier2soldier++ ;							
							}
					}
				//backSupport
					if((validPosition(i+1,j-1)&& board[i+1][j-1]==1)) supportingSoldiers.push_back(mdp(i,j,i+1,j-1)) ;
					if((validPosition(i+1,j+1)&& board[i+1][j+1]==1)) supportingSoldiers.push_back(mdp(i,j,i+1,j+1)) ;
					if((validPosition(i+1,j)&& board[i+1][j]==1)) supportingSoldiers.push_back(mdp(i,j,i+1,j)) ;
				}
				else if(board[i][j]==-1){
					//cout<<i<<","<<j<<endl ;
					soldiers_Enemy++ ;
				//distances, townHallwale
				for(int k=0;k<cols/2;k++){
					if(board[0][2*k]==-2) distsoldierEnemy_townhallEnemy[k]+=computeDistance(i,j,0,2*k) ;
					if(board[rows-1][2*k+1]==2) distsoldierEnemy_townhallUS[k]+=computeDistance(i,j,rows-1,2*k+1) ;
				}
				//means - mean,meanEnemy
					meanEnemy.first+=i ;
					meanEnemy.second+=j ;
				// moves calculation ,soldier2soldier, KileerTownhallmoves, supporting soldiers
				//forward
					if(validPosition(i+1,j)&& board[i+1][j]!=-1&&board[i+1][j]!=-2) {
						validMoves_Enemy.push_back(mdp(i,j,i+1,j)) ;
						if(board[i+1][j]==1) soldier2soldier_Enemy++ ;
						if(board[i+1][j]==2) killTownHallMovesBySoldier_Enemy++ ;
					}
					if(validPosition(i+1,j+1)&& board[i+1][j+1]!=-1&&board[i+1][j+1]!=-2) {
						validMoves_Enemy.push_back(mdp(i,j,i+1,j+1)) ;
						if(board[i+1][j+1]==1) soldier2soldier_Enemy++ ;
						if(board[i+1][j+1]==2) killTownHallMovesBySoldier_Enemy++ ;
					}
					if(validPosition(i+1,j-1)&& board[i+1][j-1]!=-1&&board[i+1][j-1]!=-2) {
						validMoves_Enemy.push_back(mdp(i,j,i+1,j-1)) ;
						if(board[i+1][j-1]==1) soldier2soldier_Enemy++ ;
						if(board[i+1][j-1]==2) killTownHallMovesBySoldier_Enemy++ ;
					}
				
				//sideways
					if(validPosition(i,j-1)&&board[i][j-1]!=-2&&board[i][j-1]!=0){
						if(board[i][j-1]!=-1){
							validMoves_Enemy.push_back(mdp(i,j,i,j-1)) ;
							if(board[i][j-1]==1) soldier2soldier_Enemy++ ;
							if(board[i][j-1]==2) killTownHallMovesBySoldier_Enemy++ ;				
						}
						else{
							supportingSoldiers_Enemy.push_back(mdp(i,j,i,j-1)) ;
						}
					}
					if(validPosition(i,j+1)&&board[i][j+1]!=-2&&board[i][j-1]!=0){
						if(board[i][j+1]!=-1){
							validMoves_Enemy.push_back(mdp(i,j,i,j+1)) ;
							if(board[i][j+1]==1) soldier2soldier_Enemy++ ;
							if(board[i][j+1]==2) killTownHallMovesBySoldier_Enemy++ ;				
						}
						else{
							supportingSoldiers_Enemy.push_back(mdp(i,j,i,j+1)) ;
						}
					}
					
				//backward
					if((validPosition(i+1,j-1)&& board[i+1][j-1]==1)||
					   (validPosition(i+1,j)&& board[i+1][j]==1)||
					   (validPosition(i+1,j+1)&& board[i+1][j+1]==1)||
					   (validPosition(i,j-1)&& board[i][j-1]==1)||
					   (validPosition(i,j+1)&& board[i][j+1]==1)){
							if(validPosition(i-2,j+2)&& board[i-2][j+2]!=-1 && board[i-2][j+2]!=-2){
								validMoves_Enemy.push_back(mdp(i,j,i-2,j+2)) ;
								if(board[i-2][j+2]==1) soldier2soldier_Enemy++ ;
							}
							if(validPosition(i-2,j)&& board[i-2][j]!=-1 && board[i-2][j]!=-2){
								validMoves_Enemy.push_back(mdp(i,j,i-2,j)) ;
								if(board[i-2][j]==1) soldier2soldier_Enemy++ ;							
							}
							if(validPosition(i-2,j-2)&& board[i-2][j-2]!=-1 && board[i-2][j-2]!=-2){
								validMoves_Enemy.push_back(mdp(i,j,i-2,j-2)) ;
								if(board[i-2][j-2]==1) soldier2soldier_Enemy++ ;							
							}
					}
				//backSupport
					if((validPosition(i-1,j-1)&& board[i-1][j-1]==1)) supportingSoldiers_Enemy.push_back(mdp(i,j,i-1,j-1)) ;
					if((validPosition(i-1,j+1)&& board[i-1][j+1]==1)) supportingSoldiers_Enemy.push_back(mdp(i,j,i-1,j+1)) ;
					if((validPosition(i-1,j)&& board[i-1][j]==1)) supportingSoldiers_Enemy.push_back(mdp(i,j,i-1,j)) ;
				}
				if(board[i][j]==2) townhall++ ;
				if(board[i][j]==-2) townhall_Enemy++ ;
			}
		}	 
		//cout<<"RUN_CHECK"<<endl ;
		
		meanUS.first/=max(soldiers,1)  ;
		meanUS.second/=max(soldiers,1) ;
		//meanUs/=soldiers ;
		meanEnemy.first/=max(soldiers_Enemy,1) ;
		meanEnemy.second/=max(soldiers_Enemy,1)  ;
		//cout<<"Exiting soldiersinit"<<endl ;
	}	
		
        void initCannons(){
                vector<t3p> allCannons,allCannons_enemy;
                for(int i = 0; i < rows; i++){
                        for(int j = 0; j < cols; j++){
                                if(validPosition(i,j-1) && validPosition(i,j) && validPosition(i,j+1) && board[i][j-1] == 1 && board[i][j] == 1 && board[i][j+1] == 1)
                                                                {allCannons.push_back(make_tuple(make_pair(i,j-1),make_pair(i,j),make_pair(i,j+1)));}
                                if(validPosition(i-1,j) && validPosition(i,j) && validPosition(i+1,j) && board[i-1][j] == 1 && board[i][j] == 1 &&board[i+1][j] == 1)
                                                                {allCannons.push_back(make_tuple(make_pair(i-1,j),make_pair(i,j),make_pair(i+1,j)));}
                                if(validPosition(i+1,j-1) && validPosition(i,j) && validPosition(i-1,j+1) && board[i+1][j-1] == 1 && board[i][j] == 1 &&board[i-1][j+1] == 1)
                                                                {allCannons.push_back(make_tuple(make_pair(i+1,j-1),make_pair(i,j),make_pair(i-1,j+1)));}
                                if(validPosition(i,j-1) && validPosition(i,j) && validPosition(i,j+1) && board[i][j-1] == -1 && board[i][j] == -1 && board[i][j+1] == -1)
                                                                {allCannons_enemy.push_back(make_tuple(make_pair(i,j-1),make_pair(i,j),make_pair(i,j+1)));}
                                if(validPosition(i-1,j) && validPosition(i,j) && validPosition(i+1,j) && board[i-1][j] == -1 && board[i][j] == -1 &&board[i+1][j] == -1)
                                                                {allCannons_enemy.push_back(make_tuple(make_pair(i-1,j),make_pair(i,j),make_pair(i+1,j)));}
                                if(validPosition(i+1,j-1) && validPosition(i,j) && validPosition(i-1,j+1) && board[i+1][j-1] == -1 && board[i][j] == -1 &&board[i-1][j+1] == -1)
                                                                {allCannons_enemy.push_back(make_tuple(make_pair(i+1,j-1),make_pair(i,j),make_pair(i-1,j+1)));}
                        }
                }
				cannonScore=0.0 ;
				cannonScore_Enemy=0.0 ;
                cannons = allCannons;
                cannons_Enemy = allCannons_enemy;
                vector<p2p> cannoncap,cannoncap_enemy ;
                int killTownHallMovesByCannon_psd=0,killTownHallMovesByCannon_Enemy_psd=0;
                int cannon2soldier_psd=0, cannon2cannon_psd=0, cannon2soldier_psd_enemy=0 , cannon2cannon_psd_enemy=0 ;                   
                int ti0,tj0,ti2,tj2 ;
                vector<pair<t3p,p2i>> almostCannonAttacks,almostCannonAttacks_enemy ;
                int di,dj,d1i,d1j,d2i,d2j,d3i,d3j,sx,sy ;
                int ourBlockedCannons=0, enemyBlockedCannons=0;
                bool check = true;
				bool blocked=false ;
                double single_score=0.0 ;
				int dir= 0;
                for(auto i: cannons){
                        //cout<<"Cannon: "<<get<0>(i).first<<","<<get<0>(i).second<<" "<<get<1>(i).first<<","<<get<1>(i).second<<" "<<get<2>(i).first<<","<<get<2>(i).second<<endl ;
						single_score+=living_cannon ;
						
                        ti0 = 2*get<0>(i).first-get<1>(i).first ;
                        tj0= 2*get<0>(i).second-get<1>(i).second ;
                        ti2 = 2*get<2>(i).first-get<1>(i).first ;
                        tj2= 2*get<2>(i).second-get<1>(i).second ;
						if(validPosition(ti0,tj0)&&board[ti0][tj0]==0) cannoncap.push_back(make_pair(get<2>(i),make_pair(ti0,tj0))) ;
						if(validPosition(ti2,tj2)&&board[ti2][tj2]==0) cannoncap.push_back(make_pair(get<0>(i),make_pair(ti2,tj2))) ;
						if(validPosition(ti0,tj0)&&board[ti0][tj0]!=0) ourBlockedCannons++ ;
						if(validPosition(ti2,tj2)&&board[ti2][tj2]!=0) ourBlockedCannons++ ;               
				
				
                        di = 2*get<0>(i).first - get<1>(i).first ;
                        dj = 2*get<0>(i).second - get<1>(i).second ;
                        //cout<<" dpair1 "<<di<<","<<dj<<endl ;
                        if(validPosition(di,dj)){
							blocked= board[di][dj]!=0 ;
							d1i = 2*di-get<0>(i).first ;
							d1j = 2*dj-get<0>(i).second ;
							d2i = 2*d1i-di ;
							d2j = 2*d1j-dj ;
							d3i = 2*d2i-d1i ;
							d3j = 2*d2j-d1j ;
                                //cout<<d1i<<","<<d1j<<" "<<d2i<<","<<d2j<<endl ;               //edited
							if(validPosition(d1i,d1j)){
									if(board[d1i][d1j]==-1){
										if(!blocked)almostCannonAttacks.push_back(make_pair(i,make_pair(d1i,d1j))) ;
										if(!blocked) cannon2soldier_psd++;
										single_score+=soldier_in_zone*(1+soldierMatrix[d1i][d1j]/4.0) ;
									}
									if(board[d1i][d1j]==-2){
										if(!blocked) almostCannonAttacks.push_back(make_pair(i,make_pair(d1i,d1j))) ;
										if(!blocked) killTownHallMovesByCannon_psd++ ;
										single_score+=townhall_in_zone ;
									} 
							}                           
							if(validPosition(d2i,d2j)){
									if(board[d2i][d2j]==-1){
										if(!blocked) almostCannonAttacks.push_back(make_pair(i,make_pair(d2i,d2j))) ;
										if(!blocked) cannon2soldier_psd++; 
										single_score+=soldier_in_zone*(1+soldierMatrix[d2i][d2j]/4.0) ;
									}
									if(board[d2i][d2j]==-2){
										if(!blocked) almostCannonAttacks.push_back(make_pair(i,make_pair(d2i,d2j))) ;
										if(!blocked) killTownHallMovesByCannon_psd++ ;
										single_score+=townhall_in_zone ;										
									}                                                            
							}
							if(validPosition(d3i,d3j)&&board[d3i][d3j]==-2){single_score+=townhall_in_approach ;}
							dir=0 ; // 1 for vertical, -1 for horizontal, 0 for diagonal 
							if(di==d1i) dir=-1 ;
							if(dj==d1j) dir=1 ;
							if(dir==0){
								sx=d1i+1 ; sy=d1j-1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==-1) single_score+=soldier_in_vicinity ;
								sx=d1i-1 ; sy = d1j+1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==-1) single_score+=soldier_in_vicinity ;
								sx=d2i-1 ; sy = d2i+1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==-1) single_score+=soldier_in_vicinity ;
								sx=d2i+1 ; sy = d2i-1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==-1) single_score+=soldier_in_vicinity ;
							}
							if(dir==1){
								sx=d1i ; sy=d1j-1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==-1) single_score+=soldier_in_vicinity ;
								sx=d1i ; sy = d1j+1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==-1) single_score+=soldier_in_vicinity ;
								sx=d2i ; sy = d2i+1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==-1) single_score+=soldier_in_vicinity ;
								sx=d2i ; sy = d2i-1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==-1) single_score+=soldier_in_vicinity ;
							}
							if(dir==2){
								sx=d1i+1 ; sy=d1j ;
								if(validPosition(sx,sy)&&board[sx][sy]==-1) single_score+=soldier_in_vicinity ;
								sx=d1i-1 ; sy = d1j ;
								if(validPosition(sx,sy)&&board[sx][sy]==-1) single_score+=soldier_in_vicinity ;
								sx=d2i-1 ; sy = d2i ;
								if(validPosition(sx,sy)&&board[sx][sy]==-1) single_score+=soldier_in_vicinity ;
								sx=d2i+1 ; sy = d2i ;
								if(validPosition(sx,sy)&&board[sx][sy]==-1) single_score+=soldier_in_vicinity ;
							}
							if(!blocked){
								check = false;
								for(auto j: cannons_Enemy){
										if((get<0>(j).first == d1i && get<0>(j).second == d1j)||
										(get<1>(j).first == d1i && get<1>(j).second == d1j)||
										(get<2>(j).first == d1i && get<2>(j).second == d1j)||
										(get<0>(j).first == d2i && get<0>(j).second == d2j)||
										(get<1>(j).first == d2i && get<1>(j).second == d2j)||
										(get<2>(j).first == d2i && get<2>(j).second == d2j)){check = true;}         
//if(check)cout<<"THIS IS THAT PAIR"<<get<0>(i).first<<","<<get<0>(i).second<<" "<<get<1>(i).first<<","<<get<1>(i).second<<" "<<get<2>(i).first<<","<<get<2>(i).second<<"   "<<get<0>(j).first<<","<<get<0>(j).second<<" "<<get<1>(j).first<<","<<get<1>(j).second<<" "<<get<2>(j).first<<","<<get<2>(j).second<<"     "<<d1i<<","<<d1j<<" "<<d2i<<","<<d2j<<endl ;								
								}
								if(check){cannon2cannon_psd++;}  
							}
						if(blocked) single_score*=blocked_factor ;
						cannonScore+=single_score ;
                        }
						
						single_score=0.0 ;
                        //all edits to be done                                                                                                                  
                        di = 2*get<2>(i).first - get<1>(i).first ;
                        dj = 2*get<2>(i).second - get<1>(i).second ;
                        //cout<<" dpair2 "<<di<<","<<dj<<endl ;
                        if(validPosition(di,dj)){
							blocked= board[di][dj]!=0 ;
							d1i = 2*di-get<2>(i).first ;
							d1j = 2*dj-get<2>(i).second ;
							d2i = 2*d1i-di ;
							d2j = 2*d1j-dj ;
							d3i = 2*d2i-d1i ;
							d3j = 2*d2j-d1j ;
                                //cout<<d1i<<","<<d1j<<" "<<d2i<<","<<d2j<<endl ;               //edited
							if(validPosition(d1i,d1j)){
									if(board[d1i][d1j]==-1){
										if(!blocked)almostCannonAttacks.push_back(make_pair(i,make_pair(d1i,d1j))) ;
										if(!blocked) cannon2soldier_psd++;
										single_score+=soldier_in_zone*(1+soldierMatrix[d1i][d1j]/4.0) ;
									}
									if(board[d1i][d1j]==-2){
										if(!blocked) almostCannonAttacks.push_back(make_pair(i,make_pair(d1i,d1j))) ;
										if(!blocked) killTownHallMovesByCannon_psd++ ;
										single_score+=townhall_in_zone ;
									} 
							}                           
							if(validPosition(d2i,d2j)){
									if(board[d2i][d2j]==-1){
										if(!blocked) almostCannonAttacks.push_back(make_pair(i,make_pair(d2i,d2j))) ;
										if(!blocked) cannon2soldier_psd++; 
										single_score+=soldier_in_zone*(1+soldierMatrix[d2i][d2j]/4.0) ;
									}
									if(board[d2i][d2j]==-2){
										if(!blocked) almostCannonAttacks.push_back(make_pair(i,make_pair(d2i,d2j))) ;
										if(!blocked) killTownHallMovesByCannon_psd++ ;
										single_score+=townhall_in_zone ;										
									}                                                            
							}
							if(validPosition(d3i,d3j)&&board[d3i][d3j]==-2){single_score+=townhall_in_approach ;}
							dir=0 ; // 1 for vertical, -1 for horizontal, 0 for diagonal 
							if(di==d1i) dir=-1 ;
							if(dj==d1j) dir=1 ;
							if(dir==0){
								sx=d1i+1 ; sy=d1j-1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==-1) single_score+=soldier_in_vicinity ;
								sx=d1i-1 ; sy = d1j+1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==-1) single_score+=soldier_in_vicinity ;
								sx=d2i-1 ; sy = d2i+1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==-1) single_score+=soldier_in_vicinity ;
								sx=d2i+1 ; sy = d2i-1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==-1) single_score+=soldier_in_vicinity ;
							}
							else if(dir==1){
								sx=d1i ; sy=d1j-1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==-1) single_score+=soldier_in_vicinity ;
								sx=d1i ; sy = d1j+1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==-1) single_score+=soldier_in_vicinity ;
								sx=d2i ; sy = d2i+1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==-1) single_score+=soldier_in_vicinity ;
								sx=d2i ; sy = d2i-1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==-1) single_score+=soldier_in_vicinity ;
							}
							else if(dir==2){
								sx=d1i+1 ; sy=d1j ;
								if(validPosition(sx,sy)&&board[sx][sy]==-1) single_score+=soldier_in_vicinity ;
								sx=d1i-1 ; sy = d1j ;
								if(validPosition(sx,sy)&&board[sx][sy]==-1) single_score+=soldier_in_vicinity ;
								sx=d2i-1 ; sy = d2i ;
								if(validPosition(sx,sy)&&board[sx][sy]==-1) single_score+=soldier_in_vicinity ;
								sx=d2i+1 ; sy = d2i ;
								if(validPosition(sx,sy)&&board[sx][sy]==-1) single_score+=soldier_in_vicinity ;
							}
							if(!blocked){
								check = false;
								for(auto j: cannons_Enemy){
										if((get<0>(j).first == d1i && get<0>(j).second == d1j)||
										(get<1>(j).first == d1i && get<1>(j).second == d1j)||
										(get<2>(j).first == d1i && get<2>(j).second == d1j)||
										(get<0>(j).first == d2i && get<0>(j).second == d2j)||
										(get<1>(j).first == d2i && get<1>(j).second == d2j)||
										(get<2>(j).first == d2i && get<2>(j).second == d2j)){check = true;
//if(check)cout<<"THIS IS THAT PAIR"<<get<0>(i).first<<","<<get<0>(i).second<<" "<<get<1>(i).first<<","<<get<1>(i).second<<" "<<get<2>(i).first<<","<<get<2>(i).second<<"   "<<get<0>(j).first<<","<<get<0>(j).second<<" "<<get<1>(j).first<<","<<get<1>(j).second<<" "<<get<2>(j).first<<","<<get<2>(j).second<<"     "<<d1i<<","<<d1j<<" "<<d2i<<","<<d2j<<endl ;										
										}         
								}
								if(check){
									cannon2cannon_psd++;}  
							}
						
						if(blocked) single_score*=blocked_factor ;
						cannonScore+=single_score ;
                        }						
                }
                single_score=0.0 ;
				for(auto i: cannons_Enemy){
                        //cout<<"Cannon: "<<get<0>(i).first<<","<<get<0>(i).second<<" "<<get<1>(i).first<<","<<get<1>(i).second<<" "<<get<2>(i).first<<","<<get<2>(i).second<<endl ;
						single_score+=living_cannon ;
						
                        ti0 = 2*get<0>(i).first-get<1>(i).first ;
                        tj0= 2*get<0>(i).second-get<1>(i).second ;
                        ti2 = 2*get<2>(i).first-get<1>(i).first ;
                        tj2= 2*get<2>(i).second-get<1>(i).second ;
						if(validPosition(ti0,tj0)&&board[ti0][tj0]==0) cannoncap_enemy.push_back(make_pair(get<2>(i),make_pair(ti0,tj0))) ;
						if(validPosition(ti2,tj2)&&board[ti2][tj2]==0) cannoncap_enemy.push_back(make_pair(get<0>(i),make_pair(ti2,tj2))) ;
						if(validPosition(ti0,tj0)&&board[ti0][tj0]!=0) enemyBlockedCannons++ ;
						if(validPosition(ti2,tj2)&&board[ti2][tj2]!=0) enemyBlockedCannons++ ;                
				
                        di = 2*get<0>(i).first - get<1>(i).first ;
                        dj = 2*get<0>(i).second - get<1>(i).second ;
                        //cout<<" dpair1 "<<di<<","<<dj<<endl ;
                        if(validPosition(di,dj)){
						blocked= board[di][dj]!=0 ;
							d1i = 2*di-get<0>(i).first ;
							d1j = 2*dj-get<0>(i).second ;
							d2i = 2*d1i-di ;
							d2j = 2*d1j-dj ;
							d3i = 2*d2i-d1i ;
							d3j = 2*d2j-d1j ;
                                //cout<<d1i<<","<<d1j<<" "<<d2i<<","<<d2j<<endl ;               //edited
							if(validPosition(d1i,d1j)){
									if(board[d1i][d1j]==1){
										if(!blocked)almostCannonAttacks_enemy.push_back(make_pair(i,make_pair(d1i,d1j))) ;
										if(!blocked) cannon2soldier_psd_enemy++;
										single_score+=soldier_in_zone*(1+soldierMatrix[d1i][d1j]/4.0) ;
									}
									if(board[d1i][d1j]==2){
										if(!blocked) almostCannonAttacks_enemy.push_back(make_pair(i,make_pair(d1i,d1j))) ;
										if(!blocked) killTownHallMovesByCannon_Enemy_psd++ ;
										single_score+=townhall_in_zone ;
									} 
							}                           
							if(validPosition(d2i,d2j)){
									if(board[d2i][d2j]==1){
										if(!blocked) almostCannonAttacks_enemy.push_back(make_pair(i,make_pair(d2i,d2j))) ;
										if(!blocked) cannon2soldier_psd_enemy++; 
										single_score+=soldier_in_zone*(1+soldierMatrix[d2i][d2j]/4.0) ;
									}
									if(board[d2i][d2j]==2){
										if(!blocked) almostCannonAttacks_enemy.push_back(make_pair(i,make_pair(d2i,d2j))) ;
										if(!blocked) killTownHallMovesByCannon_Enemy_psd++ ;
										single_score+=townhall_in_zone ;										
									}                                                            
							}
							if(validPosition(d3i,d3j)&&board[d3i][d3j]==2){single_score+=townhall_in_approach ;}
							dir=0 ; // 1 for vertical, -1 for horizontal, 0 for diagonal 
							if(di==d1i) dir=-1 ;
							if(dj==d1j) dir=1 ;
							if(dir==0){
								sx=d1i+1 ; sy=d1j-1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==1) single_score+=soldier_in_vicinity ;
								sx=d1i-1 ; sy = d1j+1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==1) single_score+=soldier_in_vicinity ;
								sx=d2i-1 ; sy = d2i+1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==1) single_score+=soldier_in_vicinity ;
								sx=d2i+1 ; sy = d2i-1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==1) single_score+=soldier_in_vicinity ;
							}
							if(dir==1){
								sx=d1i ; sy=d1j-1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==1) single_score+=soldier_in_vicinity ;
								sx=d1i ; sy = d1j+1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==1) single_score+=soldier_in_vicinity ;
								sx=d2i ; sy = d2i+1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==1) single_score+=soldier_in_vicinity ;
								sx=d2i ; sy = d2i-1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==1) single_score+=soldier_in_vicinity ;
							}
							if(dir==2){
								sx=d1i+1 ; sy=d1j ;
								if(validPosition(sx,sy)&&board[sx][sy]==1) single_score+=soldier_in_vicinity ;
								sx=d1i-1 ; sy = d1j ;
								if(validPosition(sx,sy)&&board[sx][sy]==1) single_score+=soldier_in_vicinity ;
								sx=d2i-1 ; sy = d2i ;
								if(validPosition(sx,sy)&&board[sx][sy]==1) single_score+=soldier_in_vicinity ;
								sx=d2i+1 ; sy = d2i ;
								if(validPosition(sx,sy)&&board[sx][sy]==1) single_score+=soldier_in_vicinity ;
							}
							if(!blocked){
								check = false;
								for(auto j: cannons){
										if((get<0>(j).first == d1i && get<0>(j).second == d1j)||
										(get<1>(j).first == d1i && get<1>(j).second == d1j)||
										(get<2>(j).first == d1i && get<2>(j).second == d1j)||
										(get<0>(j).first == d2i && get<0>(j).second == d2j)||
										(get<1>(j).first == d2i && get<1>(j).second == d2j)||
										(get<2>(j).first == d2i && get<2>(j).second == d2j)){check = true;
//if(check)cout<<"THIS IS THAT PAIR"<<get<0>(i).first<<","<<get<0>(i).second<<" "<<get<1>(i).first<<","<<get<1>(i).second<<" "<<get<2>(i).first<<","<<get<2>(i).second<<"   "<<get<0>(j).first<<","<<get<0>(j).second<<" "<<get<1>(j).first<<","<<get<1>(j).second<<" "<<get<2>(j).first<<","<<get<2>(j).second<<"     "<<d1i<<","<<d1j<<" "<<d2i<<","<<d2j<<endl ;									
									}         
								}
								if(check){cannon2cannon_psd_enemy++;}  
							}
                        }
						if(blocked) single_score*=blocked_factor ;
						cannonScore_Enemy+=single_score ;
						
						single_score=0.0 ;
                        //all edits to be done                                                                                                                  
                        di = 2*get<2>(i).first - get<1>(i).first ;
                        dj = 2*get<2>(i).second - get<1>(i).second ;
                        //cout<<" dpair2 "<<di<<","<<dj<<endl ;
                        if(validPosition(di,dj)){
						blocked= board[di][dj]!=0 ;
							d1i = 2*di-get<2>(i).first ;
							d1j = 2*dj-get<2>(i).second ;
							d2i = 2*d1i-di ;
							d2j = 2*d1j-dj ;
							d3i = 2*d2i-d1i ;
							d3j = 2*d2j-d1j ;
                                //cout<<d1i<<","<<d1j<<" "<<d2i<<","<<d2j<<endl ;               //edited
							if(validPosition(d1i,d1j)){
									if(board[d1i][d1j]==1){
										if(!blocked)almostCannonAttacks_enemy.push_back(make_pair(i,make_pair(d1i,d1j))) ;
										if(!blocked) cannon2soldier_psd_enemy++;
										single_score+=soldier_in_zone*(1+soldierMatrix[d1i][d1j]/4.0) ;
									}
									if(board[d1i][d1j]==2){
										if(!blocked) almostCannonAttacks_enemy.push_back(make_pair(i,make_pair(d1i,d1j))) ;
										if(!blocked) killTownHallMovesByCannon_Enemy_psd++ ;
										single_score+=townhall_in_zone ;
									} 
							}                           
							if(validPosition(d2i,d2j)){
									if(board[d2i][d2j]==1){
										if(!blocked) almostCannonAttacks_enemy.push_back(make_pair(i,make_pair(d2i,d2j))) ;
										if(!blocked) cannon2soldier_psd_enemy++; 
										single_score+=soldier_in_zone*(1+soldierMatrix[d2i][d2j]/4.0) ;
									}
									if(board[d2i][d2j]==2){
										if(!blocked) almostCannonAttacks_enemy.push_back(make_pair(i,make_pair(d2i,d2j))) ;
										if(!blocked) killTownHallMovesByCannon_Enemy_psd++ ;
										single_score+=townhall_in_zone ;										
									}                                                            
							}
							if(validPosition(d3i,d3j)&&board[d3i][d3j]==2){single_score+=townhall_in_approach ;}
							dir=0 ; // 1 for vertical, -1 for horizontal, 0 for diagonal 
							if(di==d1i) dir=-1 ;
							if(dj==d1j) dir=1 ;
							if(dir==0){
								sx=d1i+1 ; sy=d1j-1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==1) single_score+=soldier_in_vicinity ;
								sx=d1i-1 ; sy = d1j+1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==1) single_score+=soldier_in_vicinity ;
								sx=d2i-1 ; sy = d2i+1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==1) single_score+=soldier_in_vicinity ;
								sx=d2i+1 ; sy = d2i-1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==1) single_score+=soldier_in_vicinity ;
							}
							if(dir==1){
								sx=d1i ; sy=d1j-1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==1) single_score+=soldier_in_vicinity ;
								sx=d1i ; sy = d1j+1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==1) single_score+=soldier_in_vicinity ;
								sx=d2i ; sy = d2i+1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==1) single_score+=soldier_in_vicinity ;
								sx=d2i ; sy = d2i-1 ;
								if(validPosition(sx,sy)&&board[sx][sy]==1) single_score+=soldier_in_vicinity ;
							}
							if(dir==2){
								sx=d1i+1 ; sy=d1j ;
								if(validPosition(sx,sy)&&board[sx][sy]==1) single_score+=soldier_in_vicinity ;
								sx=d1i-1 ; sy = d1j ;
								if(validPosition(sx,sy)&&board[sx][sy]==1) single_score+=soldier_in_vicinity ;
								sx=d2i-1 ; sy = d2i ;
								if(validPosition(sx,sy)&&board[sx][sy]==1) single_score+=soldier_in_vicinity ;
								sx=d2i+1 ; sy = d2i ;
								if(validPosition(sx,sy)&&board[sx][sy]==1) single_score+=soldier_in_vicinity ;
							}
							if(!blocked){
								check = false;
								for(auto j: cannons){
										if((get<0>(j).first == d1i && get<0>(j).second == d1j)||
										(get<1>(j).first == d1i && get<1>(j).second == d1j)||
										(get<2>(j).first == d1i && get<2>(j).second == d1j)||
										(get<0>(j).first == d2i && get<0>(j).second == d2j)||
										(get<1>(j).first == d2i && get<1>(j).second == d2j)||
										(get<2>(j).first == d2i && get<2>(j).second == d2j)){check = true;}         
//if(check)cout<<"THIS IS THAT PAIR"<<get<0>(i).first<<","<<get<0>(i).second<<" "<<get<1>(i).first<<","<<get<1>(i).second<<" "<<get<2>(i).first<<","<<get<2>(i).second<<"   "<<get<0>(j).first<<","<<get<0>(j).second<<" "<<get<1>(j).first<<","<<get<1>(j).second<<" "<<get<2>(j).first<<","<<get<2>(j).second<<"     "<<d1i<<","<<d1j<<" "<<d2i<<","<<d2j<<endl ;									
								}
								if(check){cannon2cannon_psd_enemy++;}  
							}
                        }
						if(blocked) single_score*=blocked_factor ;
						cannonScore_Enemy+=single_score ;
	                }            
                cannonAttacks = almostCannonAttacks ;
                cannonCaptures = cannoncap ;
                cannonAttacks_Enemy = almostCannonAttacks_enemy;
                cannonCaptures_Enemy = cannoncap_enemy;
                blockedCannonsUS = ourBlockedCannons;
                blockedCannonsEnemy = enemyBlockedCannons;
                killTownHallMovesByCannon = killTownHallMovesByCannon_psd;
                killTownHallMovesByCannon_Enemy = killTownHallMovesByCannon_Enemy_psd;
                cannon2cannon = cannon2cannon_psd;
                cannon2cannon_Enemy = cannon2cannon_psd_enemy;
                cannon2soldier = cannon2soldier_psd;
                cannon2soldier_Enemy = cannon2soldier_psd_enemy;
                
                }  

	vector<string> getAllMovesString(){
		
		vector<string> s_moves ;
		if(chance==1){
			for(auto i:cannonAttacks){
					p2i base=get<1>(i.first) ;
					string loc="" ;
					loc+="S "+to_string(base.second)+" "+to_string(base.first)+" B "+to_string(i.second.second)+" "+to_string(i.second.first) ;
					s_moves.push_back(loc) ;
			}
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
		}
		else{
			for(auto i:cannonAttacks_Enemy){
					p2i base=get<1>(i.first) ;
					string loc="" ;
					loc+="S "+to_string(base.second)+" "+to_string(base.first)+" B "+to_string(i.second.second)+" "+to_string(i.second.first) ;
					s_moves.push_back(loc) ;
			}
			for(auto i:validMoves_Enemy){
					string loc="" ;
					loc+="S "+to_string(i.first.second)+" "+to_string(i.first.first)+" M "+to_string(i.second.second)+" "+to_string(i.second.first) ;
					s_moves.push_back(loc) ;
			}
			for(auto i:cannonCaptures_Enemy){
					string loc="" ;
					loc+="S "+to_string(i.first.second)+" "+to_string(i.first.first)+" M "+to_string(i.second.second)+" "+to_string(i.second.first) ;
					s_moves.push_back(loc) ;				
			}		
		}
		return s_moves ;
	}

	pair<double, string> static minimaxCall(gameBoard* gb,int depth, int herechance,double alpha, double beta){
		//cout<<depth<<endl ;
		if(depth==0) {
			double val =gb->heuristicFunction() ; 
			return make_pair(val,"TRASHMOVE") ;
		}
		// if(herechance&&depth%2!=0){
			// depth=depth-1 ;
		// }
		if(herechance==1){
			//max node
			vector<string> allmoves = gb->getAllMovesString() ;
			pair<double,string> localpair ;
			double maxval=DOUBLEMIN;
			double lesslevelbest=DOUBLEMIN ;
			string retmove=allmoves[0] ;
			for(auto i:allmoves){
				gameBoard* tempboard = new gameBoard(gb,i,!gb->chance) ;
				if(depth==2) {
					//cout<< i<<" "<<tempboard->heuristicFunction()<<tempboard->soldiers<<" "<<tempboard->soldiers_Enemy<<" "<<tempboard->townhall<<" "<<tempboard->townhall_Enemy<<" "<<tempboard->soldier2soldier<<" "<<tempboard->cannon2soldier<<" "<<tempboard->cannon2soldier_Enemy<<endl ;
					//tempboard->printStats() ;
				}
				if(tempboard->soldiers_Enemy==0) return make_pair(maxval/10.0,i) ;
				if(tempboard->getAllMovesString().size()==0) return make_pair(maxval/10.0,i) ;
				localpair = minimaxCall(tempboard,depth-1,!herechance,alpha,beta) ;
				beta= (beta>localpair.first)? beta:localpair.first;
				if(alpha>beta) return make_pair(localpair.first,i);
				if(localpair.first>maxval) {
					lesslevelbest=tempboard->heuristicFunction() ;
					maxval = localpair.first ;
					retmove = i ;
				}
				else if(localpair.first==maxval&tempboard->heuristicFunction()>lesslevelbest){
					lesslevelbest=tempboard->heuristicFunction() ;
					maxval = localpair.first ;
					retmove = i ;
				}
				delete tempboard ;
			}
			return make_pair(maxval,retmove) ;
		}
		else{
			//cout<<"here" ;
			vector<string> allmoves = gb->getAllMovesString() ;
			pair<double,string> localpair ;
			double minval=DOUBLEMAX ;
			double lesslevelbest=DOUBLEMAX ;
			string retmove=allmoves[0] ;
			for(auto i:allmoves){
			//	cout<<"iside loop"<<endl ;
				gameBoard* tempboard = new gameBoard(gb,i,!gb->chance) ;
				if(tempboard->soldiers==0) return make_pair(minval/10.0,i) ;
				if(tempboard->getAllMovesString().size()==0) return make_pair(minval/10.0,i) ;				
				localpair = minimaxCall(tempboard,depth-1,!herechance,alpha,beta) ;
				alpha=(alpha<localpair.first)?alpha:localpair.first ;
				if(alpha>beta) return make_pair(localpair.first,i) ;
				if(localpair.first<minval) {
					lesslevelbest=tempboard->heuristicFunction() ;
					minval = localpair.first ;
					retmove = i ;
				}
				else if(localpair.first==minval&tempboard->heuristicFunction()<lesslevelbest){
					lesslevelbest=tempboard->heuristicFunction() ;
					minval = localpair.first ;
					retmove = i ;
				}
				delete tempboard ;
			}
			return make_pair(minval,retmove) ;			
		}
	}
};


int main(){
		string config ;
		int x = unsigned(time(0))%32767; srand(x) ;
		getline(cin,config) ;
		stringstream ss(config) ;
		int i1=2,j1=2 ;
		int i2=1,j2=1 ;
		ss>>PLAYERID ; ss>>rows ; ss>>cols ; ss>>duration;
		bool firstchance=(PLAYERID==1)? 1:0  ;
        gameBoard* obji = new gameBoard(rows,cols,firstchance) ;
        string move="" ;
        int count= 0;
        gameBoard* obj2=obji;
        gameBoard* obj1=obji;
        while(true){
                count++ ;
                if(obj1->chance==0) {
                        getline(cin,move) ;
                        if(!firstchance) move=reverseMove(move) ;
                        obj2= new gameBoard(obj1, move, 1) ;
                        obj1=obj2 ;
						//for(auto i:obj1->getAllMovesString()) cout<<i<<endl ;
						//cout<<"SIZE"<<obj1->cannonAttacks.size()<<endl  ;
						//obj1->printStats() ;
                        //obj1->printBoard() ;
                }
                else{
                        move = obj2->minimax_move() ;
                        if(!firstchance) cout<<reverseMove(move)<<endl ;
                        else cout<<move<<endl ;
                        obj1= new gameBoard(obj2, move, 0) ;
						//obj1->printBoard() ;
                }
        }  
		return 0 ;
}
