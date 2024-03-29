#include <bits/stdc++.h>
using namespace std ;
#define p2i pair<int,int>
#define p2p pair<p2i,p2i>
#define SHORTMAX 32767
#define mdp(a,b,c,d) make_pair(make_pair(a,b),make_pair(c,d))
#define t3p tuple<p2i,p2i,p2i>

vector< vector<int> > board ;
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
int rows ;
int cols ; 
int id ;
int countSoldierUS ;
int countSoldierEnemy ;
int countTHUS ;
int countTHEnemy ;
int duration ;

///round off mean ;

class gameBoard{ 
public:
	int rows ;	//done
	int cols ;	//done
	bool player ; //1 for US, 0 for Enemy
	bool chance ; //1 for US, 0 for Enemy 
	
	vector< vector<int> > board ; //done
	vector<p2p> validMoves ; //Done
	vector<p2p> validMoves_Enemy ;	//done
	vector<p2p> cannonCaptures ; //done	
	vector<p2p> cannonCaptures_Enemy ; //done
	vector<pair<t3p,p2i>> cannonAttacks ; //done
	vector<pair<t3p,p2i>> cannonAttacks_Enemy ; //done
	
	vector<t3p> cannons; //done
	vector<t3p> cannons_Enemy ; //done
	int blockedCannonsUS ; //done
	int blockedCannonsEnemy ; //done
	int soldiers ; //done
	int soldiers_Enemy ; //done
	int diffSoldiers ;	//tobedone US-Enemy
	
	int soldier2soldier ; //done
	int cannon2soldier ; //done
	int cannon2cannon ; //done
	int soldier2cannon ;
	int soldier2soldier_Enemy ;	//done
	int cannon2soldier_Enemy ; //done
	int cannon2cannon_Enemy ; //done
	int soldier2cannon_Enemy ;	
	

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
	
	vector<double> weights{ 0,0,0,0,0,0 };
        
	double heuristicFunction(){
			vector<int> features ;
			features.push_back(cannonCaptures.size());
			features.push_back(cannonCaptures_Enemy.size());
			features.push_back(cannonAttacks.size());
			features.push_back(cannonAttacks_Enemy.size());
			features.push_back(cannons.size());
			features.push_back(cannons_Enemy.size());
			features.push_back(blockedCannonsUS);
			features.push_back(blockedCannonsEnemy);
			features.push_back(soldiers);
			features.push_back(soldiers_Enemy);
			features.push_back(diffSoldiers);
			features.push_back(soldier2soldier);
			features.push_back(cannon2soldier);
			features.push_back(cannon2cannon);
			features.push_back(soldier2cannon);
			features.push_back(soldier2soldier_Enemy);
			features.push_back(cannon2soldier_Enemy);
			features.push_back(cannon2cannon_Enemy);
			features.push_back(soldier2cannon_Enemy);
			features.push_back(killTownHallMovesBySoldier);
			features.push_back(killTownHallMovesBySoldier_Enemy);
			features.push_back(killTownHallMovesByCannon);
			features.push_back(killTownHallMovesByCannon_Enemy);
			features.push_back(scattering);
			features.push_back(scattering_Enemy);
			features.push_back(computeDistance(0,0,meanUS.first,meanUS.second));
			features.push_back(computeDistance(0,0,meanEnemy.first,meanEnemy.second));
			features.push_back(totalSupport);
			features.push_back(totalSupport_Enemy);
			for(int i = 0; i < cols/2; i++){
					features.push_back(distsoldierUS_townhallUS[i]);
					features.push_back(distsoldierUS_townhallEnemy[i]);
					features.push_back(distsoldierEnemy_townhallEnemy[i]);
					features.push_back(distsoldierEnemy_townhallUS[i]);
			}
			double res = 0;
			for(int i = 0; i < weights.size(); i++){
					res += weights[i]*features[i];
			}
			return res;
	}

	gameBoard(int m,int n, int chance):rows(m),cols(n),player(chance){
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
	}
				
	gameBoard(vector<vector<int>> initialBoard, string implmove, bool chance){ //edited 2:23 chance is opposite	 of player, i.e. move and player is of same
		board=initialBoard ;
		this->chance = chance ;
		//player=1, i.e You should be giving input 1 in chance and our move in implmove
		this->player=!chance ;
		rows=board.size() ;
		cols=board[0].size() ;
		executeMove(implmove) ;
		initSoldiers() ;
		initCannons() ;
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
		if(player) cout<<"It was my move"<<endl;
		else cout<<"It was Enemy move"<<endl ;
	}
	
	void executeMove(){ //edited 2:23
		string move ;
		if(player) cout<<"US will now make a move"<<endl ;
		else cout<<"Enemy will now make a move"<<endl ;
		while(!(move.size()==11&&move[0]=='S'&&move[1]==' '&&move[3]==' '&&move[5]==' '&&(move[6]=='B'||move[6]=='M')&&move[7]==' '&&move[9]==' ')){
			getline(cin,move) ;
		}
		executeMove(move) ;
		initSoldiers() ;
		initCannons() ;
		printStats() ;
		player=!player ;
	}
	
	vector<vector<int>> executeMoveBoard(string move){ //edited 2:23
		stringstream ss(move) ;
		int ix,iy,fx,fy ;
		char bomb;
		ss>>bomb>>ix>>iy>>bomb>>fx>>fy ;
		if(bomb=='B'){
			board[fy][fx]=0 ;
		}
		else{
			board[iy][ix]=0;
			board[fy][fx]=(player) ? 1 : -1 ;
		}
		return board ;		
	}
	
	void printBoard(){
			for(auto i:board){
					for(auto j:i) cout<<j<<" ";
					cout<<endl ;
			}
	}
	
	void printStats(){
		printBoard() ;
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
		cout<<"In soldiersinit"<<endl ;
		soldiers=0 ;
		soldiers_Enemy=0 ;
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
					if(validPosition(i-1,j)&& board[i-1][j]!=1) {
						validMoves.push_back(mdp(i,j,i-1,j)) ;
						if(board[i-1][j]==-1) soldier2soldier++ ;
						if(board[i-1][j]==-2) killTownHallMovesBySoldier++ ;
					}
					if(validPosition(i-1,j+1)&& board[i-1][j+1]!=1){
						validMoves.push_back(mdp(i,j,i-1,j+1)) ;
						if(board[i-1][j+1]==-1) soldier2soldier++ ;
						if(board[i-1][j+1]==-2) killTownHallMovesBySoldier++ ;
					}
					if(validPosition(i-1,j-1)&& board[i-1][j-1]!=1){
						validMoves.push_back(mdp(i,j,i-1,j-1)) ;
						if(board[i-1][j-1]==-1) soldier2soldier++ ;
						if(board[i-1][j-1]==-2) killTownHallMovesBySoldier++ ;
					}
				
				//sideways
					if(validPosition(i,j-1)){
						if(board[i][j-1]!=1){
							validMoves.push_back(mdp(i,j,i,j-1)) ;
							if(board[i][j-1]==-1) soldier2soldier++ ;
							if(board[i][j-1]==-2) killTownHallMovesBySoldier++ ;				
						}
						else{
							supportingSoldiers.push_back(mdp(i,j,i,j-1)) ;
						}
					}
					if(validPosition(i,j+1)){
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
					if(validPosition(i+1,j)&& board[i+1][j]!=-1) {
						validMoves_Enemy.push_back(mdp(i,j,i+1,j)) ;
						if(board[i+1][j]==1) soldier2soldier_Enemy++ ;
						if(board[i+1][j]==2) killTownHallMovesBySoldier_Enemy++ ;
					}
					if(validPosition(i+1,j+1)&& board[i+1][j+1]!=-1) {
						validMoves_Enemy.push_back(mdp(i,j,i+1,j+1)) ;
						if(board[i+1][j+1]==1) soldier2soldier_Enemy++ ;
						if(board[i+1][j+1]==2) killTownHallMovesBySoldier_Enemy++ ;
					}
					if(validPosition(i+1,j-1)&& board[i+1][j-1]!=-1) {
						validMoves_Enemy.push_back(mdp(i,j,i+1,j-1)) ;
						if(board[i+1][j-1]==1) soldier2soldier_Enemy++ ;
						if(board[i+1][j-1]==2) killTownHallMovesBySoldier_Enemy++ ;
					}
				
				//sideways
					if(validPosition(i,j-1)){
						if(board[i][j-1]!=-1){
							validMoves_Enemy.push_back(mdp(i,j,i,j-1)) ;
							if(board[i][j-1]==1) soldier2soldier_Enemy++ ;
							if(board[i][j-1]==2) killTownHallMovesBySoldier_Enemy++ ;				
						}
						else{
							supportingSoldiers_Enemy.push_back(mdp(i,j,i,j-1)) ;
						}
					}
					if(validPosition(i,j+1)){
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
			}
		}	 
		
		meanUS.first/=soldiers ;
		meanUS.second/=soldiers ;
		//meanUs/=soldiers ;
		meanEnemy.first/=soldiers_Enemy ;
		meanEnemy.second/=soldiers_Enemy ;
		cout<<"Exiting soldiersinit"<<endl ;
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
		cannons = allCannons;
		cannons_Enemy = allCannons_enemy;
		vector<p2p> cannoncap,cannoncap_enemy ;
		int killTownHallMovesByCannon_psd=0,killTownHallMovesByCannon_Enemy_psd=0;
		int cannon2soldier_psd=0, cannon2cannon_psd=0, cannon2soldier_psd_enemy=0 , cannon2cannon_psd_enemy=0 ;		   
		int ti0,tj0,ti2,tj2 ;
		vector<pair<t3p,p2i>> almostCannonAttacks,almostCannonAttacks_enemy ;
		int di,dj,d1i,d1j,d2i,d2j ;
		int ourBlockedCannons=0, enemyBlockedCannons=0;
		bool check = true;
		
		for(auto i: cannons){
			cout<<"Cannon: "<<get<0>(i).first<<","<<get<0>(i).second<<" "<<get<1>(i).first<<","<<get<1>(i).second<<" "<<get<2>(i).first<<","<<get<2>(i).second<<endl ;
			ti0 = 2*get<0>(i).first-get<1>(i).first ;
			tj0= 2*get<0>(i).second-get<1>(i).second ;
			ti2 = 2*get<2>(i).first-get<1>(i).first ;
			tj2= 2*get<2>(i).second-get<1>(i).second ;
			if(board[get<0>(i).first][get<0>(i).second] == 1){
				if(validPosition(ti0,tj0)&&board[ti0][tj0]==0) cannoncap.push_back(make_pair(get<2>(i),make_pair(ti0,tj0))) ;
				if(validPosition(ti2,tj2)&&board[ti2][tj2]==0) cannoncap.push_back(make_pair(get<0>(i),make_pair(ti2,tj2))) ;
				if(validPosition(ti0,tj0)&&board[ti0][tj0]!=0) ourBlockedCannons++ ;
				if(validPosition(ti2,tj2)&&board[ti2][tj2]!=0) ourBlockedCannons++ ;
			}		 
			
			di = 2*get<0>(i).first - get<1>(i).first ;
			dj = 2*get<0>(i).second - get<1>(i).second ;
			cout<<" dpair1 "<<di<<","<<dj<<endl ;
			if(validPosition(di,dj)&&board[di][dj]==0){
				d1i = 2*di-get<0>(i).first ;
				d1j = 2*dj-get<0>(i).second ;
				d2i = 2*d1i-di ;
				d2j = 2*d1j-dj ;
				//cout<<d1i<<","<<d1j<<" "<<d2i<<","<<d2j<<endl ;
					if(board[get<0>(i).first][get<0>(i).second] == 1){		  //edited
						if(validPosition(d1i,d1j)){
							if(board[d1i][d1j]!=1 && board[d1i][d1j]!=2){
								almostCannonAttacks.push_back(make_pair(i,make_pair(d1i,d1j))) ;
							}
							if(board[d1i][d1j]==-1)cannon2soldier_psd++;		 //edited
							check = false;
							for(auto j: cannons_Enemy){
								if((get<0>(j).first == d1i && get<0>(j).second == d1j)||
								(get<1>(j).first == d1i && get<1>(j).second == d1j)||
								(get<2>(j).first == d1i && get<2>(j).second == d1j)||
								(get<0>(j).first == d2i && get<0>(j).second == d2j)||
								(get<1>(j).first == d2i && get<1>(j).second == d2j)||
								(get<2>(j).first == d2i && get<2>(j).second == d2j)){check = true;}	 
							}
							if(check){cannon2cannon_psd++; cout<<d1i<<" "<<d1j<<endl;}
							//else{cannon2soldier_psd++;}		 edited
						}			   
											//similar edits to be done
						if(validPosition(d2i,d2j)){
							if(board[d2i][d2j]!=1 && board[d2i][d2j]!=2){
								almostCannonAttacks.push_back(make_pair(i,make_pair(d2i,d2j))) ;
							}
							if(board[d2i][d2j]==-1)cannon2soldier_psd++;								
						}					
						if(validPosition(d1i,d1j)&&board[d1i][d1j]==-2){killTownHallMovesByCannon_psd++ ;}
						if(validPosition(d2i,d2j)&&board[d2i][d2j]==-2){killTownHallMovesByCannon_psd++ ;}
					}	
			}	
			//all edits to be done														  
			di = 2*get<2>(i).first - get<1>(i).first ;
			dj = 2*get<2>(i).second - get<1>(i).second ;
			cout<<" dpair2 "<<di<<","<<dj<<endl ;
			if(validPosition(di,dj)&&board[di][dj]==0){
				d1i = 2*di-get<2>(i).first ;
				d1j = 2*dj-get<2>(i).second ;
				d2i = 2*d1i-di ;
				d2j = 2*d1j-dj ;
						//cout<<d1i<<","<<d1j<<" "<<d2i<<","<<d2j<<endl ;
				if(board[get<0>(i).first][get<0>(i).second] == 1){
					if(validPosition(d1i,d1j)){
						if(board[d1i][d1j]!=1 && board[d1i][d1j]!=2){
							almostCannonAttacks.push_back(make_pair(i,make_pair(d1i,d1j))) ;
						}
						if(board[d1i][d1j]==-1) cannon2soldier_psd_enemy++;
						check = false;
						for(auto j: cannons_Enemy){
							if((get<0>(j).first == d1i && get<0>(j).second == d1j)||
							(get<1>(j).first == d1i && get<1>(j).second == d1j)||
							(get<2>(j).first == d1i && get<2>(j).second == d1j)||
							(get<0>(j).first == d2i && get<0>(j).second == d2j)||
							(get<1>(j).first == d2i && get<1>(j).second == d2j)||
							(get<2>(j).first == d2i && get<2>(j).second == d2j)){check = true;}	 
						}
						if(check){cannon2cannon_psd++; }
						//else{cannon2soldier_psd++;}	
					}	
					
					if(validPosition(d2i,d2j)){
						if(board[d2i][d2j]!=1 && board[d2i][d2j]!=2){
							almostCannonAttacks.push_back(make_pair(i,make_pair(d2i,d2j))) ;
						}
						check = false;
						if(board[d2i][d2j]==-1) cannon2soldier_psd++;
						//else{cannon2soldier_psd++;}
						
					}
					
					if(validPosition(d1i,d1j)&&board[d1i][d1j]==-2){killTownHallMovesByCannon_psd++ ;}
					if(validPosition(d2i,d2j)&&board[d2i][d2j]==-2){killTownHallMovesByCannon_psd++ ;}					 
				}
			}		 
		}
		for(auto i: cannons_Enemy){
			ti0 = 2*get<0>(i).first-get<1>(i).first ;
			tj0= 2*get<0>(i).second-get<1>(i).second ;
			ti2 = 2*get<2>(i).first-get<1>(i).first ;
			tj2= 2*get<2>(i).second-get<1>(i).second ;
			if(board[get<0>(i).first][get<0>(i).second] == -1){
				if(validPosition(ti0,tj0)&&board[ti0][tj0]==0) cannoncap_enemy.push_back(make_pair(get<2>(i),make_pair(ti0,tj0))) ;
				if(validPosition(ti2,tj2)&&board[ti2][tj2]==0) cannoncap_enemy.push_back(make_pair(get<0>(i),make_pair(ti2,tj2))) ;
				if(validPosition(ti0,tj0)&&board[ti0][tj0]==1) enemyBlockedCannons++ ;
				if(validPosition(ti2,tj2)&&board[ti2][tj2]==1) enemyBlockedCannons++ ;
			}		 
														
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				di = 2*get<0>(i).first - get<1>(i).first ;
				dj = 2*get<0>(i).second - get<1>(i).second ;
				if(validPosition(di,dj)&&board[di][dj]==0){
					d1i = 2*di-get<0>(i).first ;
					d1j = 2*dj-get<0>(i).second ;
					d2i = 2*d1i-di ;
					d2j = 2*d1j-dj ;
					if(board[get<0>(i).first][get<0>(i).second] == -1){
						if(validPosition(d1i,d1j)){
							if(board[d1i][d1j]!=-1 && board[d1i][d1j]!=-2){
											almostCannonAttacks_enemy.push_back(make_pair(i,make_pair(d1i,d1j))) ;
							}
							check = false;
							if(board[d1i][d1j]==1) cannon2soldier_psd_enemy++;
							for(auto j: cannons){
											if((get<0>(j).first == d1i && get<0>(j).second == d1j)||
											(get<1>(j).first == d1i && get<1>(j).second == d1j)||
											(get<2>(j).first == d1i && get<2>(j).second == d1j)||
											(get<0>(j).first == d2i && get<0>(j).second == d2j)||
											(get<1>(j).first == d2i && get<1>(j).second == d2j)||
											(get<2>(j).first == d2i && get<2>(j).second == d2j)){check = true;}	 
							}
							if(check){cannon2cannon_psd_enemy++; }
							//else{cannon2soldier_psd_enemy+			
						}								 
						if(validPosition(d2i,d2j)){
							if(board[d2i][d2j]!=-1 && board[d2i][d2j]!=-2){
											almostCannonAttacks_enemy.push_back(make_pair(i,make_pair(d2i,d2j))) ;
							}
							check = false;
							if(board[d2i][d2j]==1) cannon2soldier_psd_enemy++;
							//else{cannon2soldier_psd_enemy++;}
										
						}
						
						if(validPosition(d1i,d1j)&&board[d1i][d1j]==2){killTownHallMovesByCannon_Enemy_psd++ ;}
						if(validPosition(d2i,d2j)&&board[d2i][d2j]==2){killTownHallMovesByCannon_Enemy_psd++ ;}
					}						 
				}
				di = 2*get<2>(i).first - get<1>(i).first ;
				dj = 2*get<2>(i).second - get<1>(i).second ;
				if(validPosition(di,dj)&&board[di][dj]==0){
					d1i = 2*di-get<2>(i).first ;
					d1j = 2*dj-get<2>(i).second ;
					d2i = 2*d1i-di ;
					d2j = 2*d1j-dj ;
					if(board[get<0>(i).first][get<0>(i).second] == -1){
						if(validPosition(d1i,d1j)){
							if(board[d1i][d1j]!=1 && board[d1i][d1j]!=-2){
											almostCannonAttacks_enemy.push_back(make_pair(i,make_pair(d1i,d1j))) ;
							}
							check = false;
							if(board[d1i][d1j]==1) cannon2soldier_psd_enemy++;
							for(auto j: cannons){
											if((get<0>(j).first == d1i && get<0>(j).second == d1j)||
											(get<1>(j).first == d1i && get<1>(j).second == d1j)||
											(get<2>(j).first == d1i && get<2>(j).second == d1j)||
											(get<0>(j).first == d2i && get<0>(j).second == d2j)||
											(get<1>(j).first == d2i && get<1>(j).second == d2j)||
											(get<2>(j).first == d2i && get<2>(j).second == d2j)){check = true;}	 
							}
							if(check){cannon2cannon_psd_enemy++; cout<<d1i<<" "<<d1j<<endl;}
							else{cannon2soldier_psd_enemy;}
										
						}
						
						if(validPosition(d2i,d2j)){
							if(board[d2i][d2j]!=-1 && board[d2i][d2j]!=-2){
								almostCannonAttacks_enemy.push_back(make_pair(i,make_pair(d2i,d2j))) ;
							}
							check = false;
							if(board[d2i][d2j]==1) cannon2soldier_psd_enemy++;

							//else{cannon2soldier_psd_enemy;}
										
						}
					
						if(validPosition(d1i,d1j)&&board[d1i][d1j]==2){killTownHallMovesByCannon_Enemy_psd++ ;}
						if(validPosition(d2i,d2j)&&board[d2i][d2j]==2){killTownHallMovesByCannon_Enemy_psd++ ;}
					}						 
				}
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
		}
		else{
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
			for(auto i:cannonAttacks_Enemy){
					p2i base=get<1>(i.first) ;
					string loc="" ;
					loc+="S "+to_string(base.second)+" "+to_string(base.first)+" B "+to_string(i.second.second)+" "+to_string(i.second.first) ;
					s_moves.push_back(loc) ;
			}		
		}
		return s_moves ;
	}

};


//string minimax(gameBoard gb, bool node, ){ //max/min node
	
//}

int main(){
		/*string config ;
		int x = unsigned(time(0))%32767; srand(x) ;
		getline(cin,config) ;
		stringstream ss(config) ;
		ss>>id ; ss>>rows ; ss>>cols ; ss>>duration;
		board = vector<vector<int> >(rows,vector<int>(cols,0));
		initialize() ;
		update() ;
		if(id == 2){
			getline(cin,enemyMove);
			executeMove(enemyMove, false);
			update() ;
		}
		while(countTHEnemy>2 && countSoldierUS>2){
			ourMove = getGreedyMove();
			executeMove(ourMove,true);
			update() ;
			//printBoard() ;
			cout<<ourMove<<endl;
			getline(cin,enemyMove);
			executeMove(enemyMove, false);
			//cout<<"Enemy move: "<<enemyMove<<endl ;
			update() ;
		}*/
		gameBoard obj(8,8,0) ;
		while(true) obj.executeMove() ;
		return 0 ;
}
