#include<cstdio>
#include<iostream>
#include<cstring>
//dqb area
#define state_up 0
#define state_down 1
#define state_die -1
#define ID_0 0
#define ID_1 1
#define ID_2 2
#define ID_3 3
//Soft Ginger area
#define mode_start 0
#define mode_right 1
#define mode_up 2
#define mode_left 3
#define mode_down 4
#define mode_halt 5
//dqb area
using namespace std;
int map[20][20];
int now[4];
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
			road[num]->x=x;
			road[num]->y=y;
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
						if (0==map[i][j])
						{
							map[i][j]=now[ID];
							now[ID]++;
							size++;
						}
						
					}
				}
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
				if (ID_0==ID) {if (state_down==player0->state) player0->paint();player0->up();}
				if (ID_1==ID) {if (state_down==player1->state) player1->paint();player1->up();}
				if (ID_2==ID) {if (state_down==player2->state) player2->paint();player2->up();}
				if (ID_3==ID) {if (state_down==player3->state) player3->paint();player3->up();}
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
//dqb area
int m,n,i,j,k;
int x[4],y[4],state[4],trappedleft[4],scoredecline[4];
int now_id;
point bind[200];
int timeleft[200];
enclosure* game;
//Soft Ginger area
int mode=mode_start;
void brain()
{
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
	//Finite State Machine
	if(mode==mode_start)
	{
		mode=mode_left;
	}
	else if(mode==mode_left)
	{
		if(now_x==0&&now_y==0) mode=mode_down;
	}
	else if(mode==mode_down)
	{
		if(now_x==0&&now_y==10) mode=mode_right;
	}
	else if(mode==mode_right)
	{
		if(now_x==10&&now_y==10) mode=mode_up;
	}
	else if(mode==mode_up)
	{
		if(now_x==10&&now_y==0) mode=mode_halt;
	}
	//State Processing
	if(mode==mode_left&&now_y==0)
	{
		cout<<"[ACTION] r 1"<<endl;
	}
	else if(mode==mode_right)
	{
		cout<<"[ACTION] r 0"<<endl;
	}
	else if(mode==mode_up)
	{
		cout<<"[ACTION] u 0"<<endl;
	}
	else if(mode==mode_left)
	{
		cout<<"[ACTION] l 0"<<endl;
	}
	else if(mode==mode_down)
	{
		cout<<"[ACTION] d 0"<<endl;
	}
	else if(mode==mode_halt)
	{
		cout<<"[ACTION] s 0"<<endl;
	}
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
	cin>>label;
	bool started=false; //Update: variable name changed from visited to started
	while (true)
	{
		for (i=0;i<=3;i++)
    	{
	    	cin>>x[i]>>y[i]>>state[i]>>trappedleft[i]>>scoredecline[i];
        }
        if (!started)
		{
			started=true;
			game=new enclosure(x[0],y[0],x[1],y[1],x[2],y[2],x[3],y[3]);
			continue;
		}
		game->pass(ID_0,x[0],y[0],state[0]);
		game->pass(ID_1,x[1],y[1],state[1]);
		game->pass(ID_2,x[2],y[2],state[2]);
		game->pass(ID_3,x[3],y[3],state[3]);
		int tot;
		cin>>tot;
		for (i=1;i<=tot;i++)
		{
			cin>>bind[i].x>>bind[j].y>>timeleft[i];
		}
		//Soft Ginger area
		brain();
		//dqb area
	}
}
