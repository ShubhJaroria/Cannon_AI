#include <bits/stdc++.h>
using namespace std ;
#define p2i pair<int,int>
#define p2p pair<p2i,p2i>
#define SHORTMAX 32767
#define mdp(a,b,c,d) make_pair(make_pair(a,b),make_pair(c,d))
#define t3p tuple<p2i,p2i,p2i>


class gameBoard{ 
public:
	int rows ;	//done
	int cols ;	//done
	int player ; //1 for US, 0 for Enemy
	
	vector< vector<int> > board ; //done
	vector<p2p> validMoves ; //Done
	vector<p2p> validMoves_Enemy ; 	//done
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
	
	gameBoard(vector<vector<int>> initialBoard, string implmove, bool chance){
		board=initialBoard ;
		player=chance ;
		rows=board.size() ;
		cols=board[0].size() ;
		executeMove(implmove) ;
	}
	
	void executeMove(string move){                                //true indicates we are the player executing
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
		initSoldiers() ;
		initCannons() ;
		printStats() ;
		player=!player ;
	}
	
	void executeMove(){
		string move ;
		if(player) cout<<"US will now make a move"<<endl ;
		else cout<<"Enemy will now make a move"<<endl ;
		while(!(move.size()==11&&move[0]=='S'&&move[1]==' '&&move[3]==' '&&move[5]==' '&&(move[6]=='B'||move[6]=='M')&&move[7]==' '&&move[9]==' ')){
			getline(cin,move) ;
		}
		executeMove(move) ;
	}
	
	vector<vector<int>> executeMoveBoard(string move){
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
									
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					di = 2*get<0>(i).first - get<1>(i).first ;
					dj = 2*get<0>(i).second - get<1>(i).second ;
					if(validPosition(di,dj)){
							d1i = 2*di-get<0>(i).first ;
							d1j = 2*dj-get<0>(i).second ;
							d2i = 2*d1i-di ;
							d2j = 2*d1j-dj ;
							
							if(board[get<0>(i).first][get<0>(i).second] == 1&&validPosition(d1i,d1j)&&board[d1i][d1j]==0){	//edited
									if(validPosition(d1i,d1j)){
											if(board[d1i][d1j]!=1 && board[d1i][d1j]!=2){
													almostCannonAttacks.push_back(make_pair(i,make_pair(d1i,d1j))) ;
											}
											if(board[d1i][d1j]==-1)cannon2soldier_psd++; 	//edited
											check = false;
											for(auto j: cannons_Enemy){
													if((get<0>(j).first = d1i && get<0>(j).second == d1j)||
													(get<1>(j).first = d1i && get<1>(j).second == d1j)||
													(get<2>(j).first = d1i && get<2>(j).second == d1j)){check = true;}  
											}
											if(check){cannon2cannon_psd++;}
											//else{cannon2soldier_psd++;}	edited
											
									}              
									//similar edits to be done
									if(validPosition(d2i,d2j)){
											if(board[d2i][d2j]!=1 && board[d2i][d2j]!=2){
													almostCannonAttacks.push_back(make_pair(i,make_pair(d2i,d2j))) ;
											}
											check = false;
											if(board[d1i][d1j]==-1)cannon2soldier_psd++; 
											for(auto j: cannons_Enemy){
													if((get<0>(j).first = d2i && get<0>(j).second == d2j)||
													(get<1>(j).first = d2i && get<1>(j).second == d2j)||
													(get<2>(j).first = d2i && get<2>(j).second == d2j)){check = true;}  
											}
											if(check){cannon2cannon_psd++;}
											//else{cannon2soldier_psd++;}
											
									}
									
									if(validPosition(d1i,d1j)&&board[d1i][d1j]==-2){killTownHallMovesByCannon_psd++ ;}
									if(validPosition(d2i,d2j)&&board[d2i][d2j]==-2){killTownHallMovesByCannon_psd++ ;}
							}     
					//all edits to be done							
					di = 2*get<2>(i).first - get<1>(i).first ;
					dj = 2*get<2>(i).second - get<1>(i).second ;
					if(validPosition(di,dj)&&board[di][dj]==0){
							d1i = 2*di-get<2>(i).first ;
							d1j = 2*dj-get<2>(i).second ;
							d2i = 2*d1i-di ;
							d2j = 2*d1j-dj ;
							if(board[get<0>(i).first][get<0>(i).second] == 1&&validPosition(d1i,d1j)&&board[d1i][d1j]==0){
									if(validPosition(d1i,d1j)){
											if(board[d1i][d1j]!=1 && board[d1i][d1j]!=2){
													almostCannonAttacks.push_back(make_pair(i,make_pair(d1i,d1j))) ;
											}
											if(board[d1i][d1j]==-1) cannon2soldier_psd_enemy++;
											check = false;
											for(auto j: cannons_Enemy){
													if((get<0>(j).first = d1i && get<0>(j).second == d1j)||
													(get<1>(j).first = d1i && get<1>(j).second == d1j)||
													(get<2>(j).first = d1i && get<2>(j).second == d1j)){check = true;}  
											}
											if(check){cannon2cannon_psd++;}
											//else{cannon2soldier_psd++;}
											
									}                                
									if(validPosition(d2i,d2j)){
											if(board[d2i][d2j]!=1 && board[d2i][d2j]!=2){
													almostCannonAttacks.push_back(make_pair(i,make_pair(d2i,d2j))) ;
											}
											check = false;
											if(board[d1i][d1j]==-1) cannon2soldier_psd++;
											for(auto j: cannons_Enemy){
													if((get<0>(j).first = d2i && get<0>(j).second == d2j)||
													(get<1>(j).first = d2i && get<1>(j).second == d2j)||
													(get<2>(j).first = d2i && get<2>(j).second == d2j)){check = true;}  
											}
											if(check){cannon2cannon_psd++;}
											//else{cannon2soldier_psd++;}
											
									}
									
									if(validPosition(d1i,d1j)&&board[d1i][d1j]==-2){killTownHallMovesByCannon_psd++ ;}
									if(validPosition(d2i,d2j)&&board[d2i][d2j]==-2){killTownHallMovesByCannon_psd++ ;}
							}                        
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
							if(board[get<0>(i).first][get<0>(i).second] == -1 &&validPosition(d1i,d1j)&&board[d1i][d1j]==0){
									if(validPosition(d1i,d1j)){
											if(board[d1i][d1j]!=-1 && board[d1i][d1j]!=-2){
													almostCannonAttacks_enemy.push_back(make_pair(i,make_pair(d1i,d1j))) ;
											}
											check = false;
											if(board[d1i][d1j]==1) cannon2soldier_psd_enemy++;
											for(auto j: cannons){
													if((get<0>(j).first = d1i && get<0>(j).second == d1j)||
													(get<1>(j).first = d1i && get<1>(j).second == d1j)||
													(get<2>(j).first = d1i && get<2>(j).second == d1j)){check = true;}  
											}
											if(check){cannon2cannon_psd_enemy++;}
											//else{cannon2soldier_psd_enemy++;}
											
									}                                
									if(validPosition(d2i,d2j)){
											if(board[d2i][d2j]!=-1 && board[d2i][d2j]!=-2){
													almostCannonAttacks_enemy.push_back(make_pair(i,make_pair(d2i,d2j))) ;
											}
											check = false;
											if(board[d1i][d1j]==1) cannon2soldier_psd_enemy++;
											for(auto j: cannons){
													if((get<0>(j).first = d2i && get<0>(j).second == d2j)||
													(get<1>(j).first = d2i && get<1>(j).second == d2j)||
													(get<2>(j).first = d2i && get<2>(j).second == d2j)){check = true;}  
											}
											if(check){cannon2cannon_psd_enemy++;}
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
							if(board[get<0>(i).first][get<0>(i).second] == -1  &&validPosition(d1i,d1j)&&board[d1i][d1j]==0){
									if(validPosition(d1i,d1j)){
											if(board[d1i][d1j]!=1 && board[d1i][d1j]!=-2){
													almostCannonAttacks_enemy.push_back(make_pair(i,make_pair(d1i,d1j))) ;
											}
											check = false;
											if(board[d1i][d1j]==-1) cannon2soldier_psd_enemy++;
											for(auto j: cannons){
													if((get<0>(j).first = d1i && get<0>(j).second == d1j)||
													(get<1>(j).first = d1i && get<1>(j).second == d1j)||
													(get<2>(j).first = d1i && get<2>(j).second == d1j)){check = true;}  
											}
											if(check){cannon2cannon_psd_enemy++;}
											else{cannon2soldier_psd_enemy;}
											
									}                                
									if(validPosition(d2i,d2j)){
											if(board[d2i][d2j]!=-1 && board[d2i][d2j]!=-2){
													almostCannonAttacks_enemy.push_back(make_pair(i,make_pair(d2i,d2j))) ;
											}
											check = false;
											if(board[d1i][d1j]==1) cannon2soldier_psd_enemy++;
											for(auto j: cannons){
													if((get<0>(j).first = d2i && get<0>(j).second == d2j)||
													(get<1>(j).first = d2i && get<1>(j).second == d2j)||
													(get<2>(j).first = d2i && get<2>(j).second == d2j)){check = true;}  
											}
											if(check){cannon2cannon_psd_enemy++;}
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
			cannonCaptures = cannoncap_enemy;
			blockedCannonsUS = ourBlockedCannons;
			blockedCannonsEnemy = enemyBlockedCannons;
			killTownHallMovesByCannon = killTownHallMovesByCannon_psd;
			killTownHallMovesByCannon_Enemy = killTownHallMovesByCannon_Enemy_psd;
			cannon2cannon = cannon2cannon_psd;
			cannon2cannon_Enemy = cannon2cannon_psd_enemy;
			cannon2soldier = cannon2soldier_psd;
			cannon2soldier_Enemy = cannon2soldier_psd_enemy;

	}    
};

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
