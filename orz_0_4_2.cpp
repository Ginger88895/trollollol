#include<cstdio>
#include<iostream>
#include<cstring>
#include<fstream>
#include<iomanip>
#include<cstdlib>
#define state_up 0
#define state_down 1
#define state_die -1
#define ID_0 0
#define ID_1 1
#define ID_2 2
#define ID_3 3
#define mode_start 0
#define mode_right 1
#define mode_up 2
#define mode_left 3
#define mode_down 4
#define mode_halt 5
using namespace std;
int map[20][20][5];
int now[4];
int edge_color[20][20][20][20];
class point
{
	public:
		int x;
		int y;
};
class player
{
	public:
		point* position;
		int ID;
		int score;
		int state;
		int trappedleft;
		int num;
		point* road[300];
		player(int ID,int x,int y)
		{
			position=new point;
			position->x=x;
			position->y=y;
			this->ID=ID;
			score=4-ID;
			state=state_up;
			trappedleft=0;
			num=0;
		}
		void insert(int x,int y)
		{
			num++;
			road[num]=new point;
			road[num]->x=x;
			road[num]->y=y;
		}
		void clear()
		{
			num=0;
		}
		void change(int x,int y)
		{
			position->x=x;
			position->y=y;
		}
		void down()
		{
			this->state=state_down;
		}
		void up()
		{
			this->state=state_up;
		}
		void die()
		{
			this->state=state_die;
		}
		void paint()
		{
			int i,j,k;
			int size=0;
			for (i=1;i<=10;i++)
				for (j=1;j<=10;j++)
				{
					bool left=false;
					bool right=false;
					bool top=false;
					bool bottom=false;
					for (k=2;k<=num;k++)
					{
						if (road[k-1]->x==road[k]->x&&road[k-1]->y==road[k]->y) continue;
						if (road[k-1]->x==road[k]->x)
						{
							if (road[k-1]->y<road[k]->y)
							{
								if (j==road[k]->y&&i>road[k]->x) top=true;
								if (j==road[k]->y&&i<=road[k]->x) bottom=true;
							}
							if (road[k-1]->y>road[k]->y)
							{
								if (j==road[k-1]->y&&i>road[k]->x) top=true;
								if (j==road[k-1]->y&&i<=road[k]->x) bottom=true;
							}
							continue;
						}
						if (road[k-1]->y==road[k]->y)
						{
							if (road[k-1]->x<road[k]->x)
							{
								if (i==road[k]->x&&j>road[k]->y) left=true;
								if (i==road[k]->x&&j<=road[k]->y) right=true;
							}
							if (road[k-1]->x>road[k]->x)
							{
								if (i==road[k-1]->x&&j>road[k]->y) left=true;
								if (i==road[k-1]->x&&j<=road[k]->y) right=true;
							}
						}
					}
					if (top&&bottom&&left&&right)
					{
						if (-1==map[i][j])
						{
							map[i][j]=now[ID];
							size++;
						}
						
					}
				}
			 if (0!=size) now[ID]++; 
			 score=score+size*size*10;
			 num=0;       
		}
};
class enclosure
{
	public:
		player* player0;
		player* player1;
		player* player2;
		player* player3;
		enclosure(int x0,int y0,int x1,int y1,int x2,int y2,int x3,int y3)
		{
			player0=new player(ID_0,x0,y0);
			player1=new player(ID_1,x1,y1);
			player2=new player(ID_2,x2,y2);
			player3=new player(ID_3,x3,y3);
		}
		enclosure(){}
		void pass(int ID,int x,int y,int state)
		{
			if (state_die==state) 
			{
				if (ID_0==ID) player0->die();
				if (ID_1==ID) player1->die();
				if (ID_2==ID) player2->die();
				if (ID_3==ID) player3->die();
			}
			if (state_up==state)
			{
				if (ID_0==ID) {if (state_down==player0->state) player0->insert(x,y);player0->paint();player0->up();player0->clear();player0->insert(x,y);}
				if (ID_1==ID) {if (state_down==player1->state) player1->insert(x,y);player1->paint();player1->up();player1->clear();player1->insert(x,y);}
				if (ID_2==ID) {if (state_down==player2->state) player2->insert(x,y);player2->paint();player2->up();player2->clear();player2->insert(x,y);}
				if (ID_3==ID) {if (state_down==player3->state) player3->insert(x,y);player3->paint();player3->up();player3->clear();player3->insert(x,y);}
			}
			if (state_down==state)
			{
				if (ID_0==ID) {player0->down();player0->insert(x,y);}
				if (ID_1==ID) {player1->down();player1->insert(x,y);}
				if (ID_2==ID) {player2->down();player2->insert(x,y);}
				if (ID_3==ID) {player3->down();player3->insert(x,y);}
			}
			if (ID_0==ID) player0->change(x,y);
			if (ID_1==ID) player1->change(x,y);
			if (ID_2==ID) player2->change(x,y);
			if (ID_3==ID) player3->change(x,y);
		}
};
int m,n,i,j,k;
int x[4],y[4],state[4],trappedleft[4],scoredecline[4];
int now_id;
point bind[200];
int timeleft[200];
enclosure* game;
ofstream fout1(".\\output1.txt");
ofstream fout2(".\\output2.txt");
int mode=mode_start;
void generate_output(int diff_x,int diff_y,int other_action)
{
	cout<<"[ACTION] ";
	if(diff_x==0&&diff_y==0) cout<<"s ";
	else if(diff_x==0&&diff_y==1) cout<<"d ";
	else if(diff_x==0&&diff_y==-1) cout<<"u ";
	else if(diff_x==1&&diff_y==0) cout<<"r ";
	else if(diff_x==-1&&diff_y==0) cout<<"l ";
	cout<<other_action<<endl;
}
void update_edge_status()
{
	for(int i=0;i<=10;i++)
	{
		for(int j=0;j<=10;j++)
		{
			if(i>0)
			{
				//Upward Road
				if(j==0||j==10||map[i][j]!=map[i][j+1]||map[i][j]==-1||map[i][j+1]==-1) edge_color[i][j][i-1][j]=-1;
				else edge_color[i][j][i-1][j]=map[i][j];
			}
			if(i<10)
			{
				//Downward Road
				if(j==0||j==10||map[i+1][j]!=map[i+1][j+1]||map[i+1][j]==-1||map[i+1][j+1]==-1) edge_color[i][j][i+1][j]=-1;
				else edge_color[i][j][i+1][j]=map[i+1][j+1];
			}
			if(j>0)
			{
				//Leftward Road
				if(i==0||i==10||map[i][j]!=map[i+1][j]||map[i][j]==-1||map[i+1][j]==-1) edge_color[i][j][i][j-1]=-1;
				else edge_color[i][j][i][j-1]=map[i+1][j];
			}
			if(j<10)
			{
				//Rightward Road
				if(i==0||i==10||map[i][j+1]!=map[i+1][j+1]||map[i][j+1]==-1||map[i+1][j+1]==-1) edge_color[i][j][i][j+1]=-1;
				else edge_color[i][j][i][j+1]=map[i][j+1];
			}
		}
	}
}
int enclosure_distance(point p1,point p2,int PID)
{
	//Todo:  
	return -1;
}
void brain()
{
	update_edge_status();
	int now_x,now_y;
	switch(now_id)
	{
		case 0:	
			now_x=game->player0->position->x;
			now_y=game->player0->position->y;
			break;
		case 1:	
			now_x=game->player1->position->x;
			now_y=game->player1->position->y;
			break;
		case 2:	
			now_x=game->player2->position->x;
			now_y=game->player2->position->y;
			break;
		case 3:	
			now_x=game->player3->position->x;
			now_y=game->player3->position->y;
			break;
		default:	
			break;
	}

	if(mode==mode_start)
	{
		mode=mode_right;
	}
	else if(mode==mode_right)
	{
		if(now_x==10&&now_y==10) mode=mode_up;
	}
	else if(mode==mode_up)
	{
		if(now_x==0&&now_y==10) mode=mode_left;
	}
	else if(mode==mode_left)
	{
		if(now_x==0&&now_y==0) mode=mode_down;
	}
	else if(mode==mode_down)
	{
		if(now_x==10&&now_y==0) mode=mode_halt;
	}

	if(mode==mode_right&&now_y==0)
	{
		generate_output(0,1,1);
	}
	else if(mode==mode_right)
	{
		generate_output(0,1,0);
	}
	else if(mode==mode_up)
	{
		generate_output(-1,0,0);
	}
	else if(mode==mode_left)
	{
		generate_output(0,-1,0);
	}
	else if(mode==mode_down)
	{
		generate_output(1,0,0);
	}
	else if(mode==mode_halt)
	{
		generate_output(0,0,0);
	}
	fout1<<"---MAP---"<<endl;
	for(int i=1;i<=10;i++)
	{
		//This statement has been fixed to adapt EnclosureJudge
		for(int j=1;j<=10;j++) fout1<<setiosflags(ios::fixed)<<setw(4)<<map[j][i]; //map[i][j]
		fout1<<endl;
	}
	fout1<<"---END---"<<endl;
	fout1<<endl;
	fout2<<"---VERTICAL ROAD---"<<endl;
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<=10;j++) fout2<<setiosflags(ios::fixed)<<setw(4)<<edge_color[j][i][j][i+1];
		fout2<<endl;
	}
	fout2<<"--------END--------"<<endl;
	fout2<<endl;
	return;
}
int main()
{
	now[0]=0;
	now[1]=100;
    now[2]=200;
    now[3]=300;
	string label;
	cin>>label;
	cin>>now_id;
	cout<<"[POS] 10 0"<<endl;
	memset(map,0xff,sizeof(map));
	memset(edge_color,0xff,sizeof(edge_color));
	bool started=false; //Update: variable name changed from visited to started
	while (true)
	{
		cin>>label;
		for (i=0;i<=3;i++)
    	{
	    	cin>>x[i]>>y[i]>>state[i]>>trappedleft[i]>>scoredecline[i];
        }
        if (!started)
		{
			started=true;
			game=new enclosure(x[0],y[0],x[1],y[1],x[2],y[2],x[3],y[3]);
		}
		game->pass(ID_0,x[0],y[0],state[0]);
		game->pass(ID_1,x[1],y[1],state[1]);
		game->pass(ID_2,x[2],y[2],state[2]);
		game->pass(ID_3,x[3],y[3],state[3]);
		int tot;
		cin>>tot;
		for (i=1;i<=tot;i++)
		{
			cin>>bind[i].x>>bind[i].y>>timeleft[i];
		}
		brain();
	}
}
