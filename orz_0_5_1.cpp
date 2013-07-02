#include<cstdio>
#include<iostream>
#include<cstring>
#include<fstream>
#include<iomanip>
#include<cstdlib>
#include<queue>
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
int map[15][15];
int minimap[4][15][15];
int now[4],tot[4],area[4];
int m,n,j,k;
int x[4],y[4],state[4],trappedleft[4],scoredecline[4];
int now_id;
int edge_color[15][15][15][15];
int timeleft[200];
int mode=mode_start;
unsigned int relative_distance[4][15][15][15][15];
class dot
{
	public:
		int x;
		int y;
};
class player
{
	public:
		dot* position;
		int ID;
		int score;
		int state;
		int trappedleft;
		int num;
		dot* road[300];
		player(int ID,int x,int y)
		{
			position=new dot;
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
			road[num]=new dot;
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
			for(i=1;i<=10;i++)
				for(j=1;j<=10;j++)
				{
					bool left=false;
					bool right=false;
					bool top=false;
					bool bottom=false;
					for(k=2;k<=num;k++)
					{
						if(road[k-1]->x==road[k]->x&&road[k-1]->y==road[k]->y) continue;
						if(road[k-1]->x==road[k]->x)
						{
							if(road[k-1]->y<road[k]->y)
							{
								if(j==road[k]->y&&i>road[k]->x) top=true;
								if(j==road[k]->y&&i<=road[k]->x) bottom=true;
							}
							if(road[k-1]->y>road[k]->y)
							{
								if(j==road[k-1]->y&&i>road[k]->x) top=true;
								if(j==road[k-1]->y&&i<=road[k]->x) bottom=true;
							}
							continue;
						}
						if(road[k-1]->y==road[k]->y)
						{
							if(road[k-1]->x<road[k]->x)
							{
								if(i==road[k]->x&&j>road[k]->y) left=true;
								if(i==road[k]->x&&j<=road[k]->y) right=true;
							}
							if(road[k-1]->x>road[k]->x)
							{
								if(i==road[k-1]->x&&j>road[k]->y) left=true;
								if(i==road[k-1]->x&&j<=road[k]->y) right=true;
							}
						}
					}
					if(top&&bottom&&left&&right)
					{
						if(-1==map[i][j])
						{
							minimap[ID][i][j]=1;
						}
					}
				}
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
			if(state_die==state)
			{
				if(ID_0==ID) {player0->die();if(player0->road[1]->x==x&&player0->road[1]->y==y){player0->insert(x,y);player0->paint();player0->clear();}}
				if(ID_1==ID) {player1->die();if(player1->road[1]->x==x&&player1->road[1]->y==y){player1->insert(x,y);player1->paint();player1->clear();}}
				if(ID_2==ID) {player2->die();if(player2->road[1]->x==x&&player2->road[1]->y==y){player2->insert(x,y);player2->paint();player2->clear();}}
				if(ID_3==ID) {player3->die();if(player3->road[1]->x==x&&player3->road[1]->y==y){player3->insert(x,y);player3->paint();player3->clear();}}
			}
			if(state_up==state)
			{
				if(ID_0==ID)
				{
					if(state_down==player0->state) player0->insert(x,y);
					player0->paint();
					player0->up();
					player0->clear();
					player0->insert(x,y);
				}
				if(ID_1==ID)
				{
					if(state_down==player1->state) player1->insert(x,y);
					player1->paint();
					player1->up();
					player1->clear();
					player1->insert(x,y);
				}
				if(ID_2==ID)
				{
					if(state_down==player2->state) player2->insert(x,y);
					player2->paint();
					player2->up();
					player2->clear();
					player2->insert(x,y);
				}
				if(ID_3==ID)
				{
					if(state_down==player3->state) player3->insert(x,y);
					player3->paint();
					player3->up();
					player3->clear();
					player3->insert(x,y);
				}
			}
			if(state_down==state)
			{
				if(ID_0==ID)
				{
					player0->down();
					player0->insert(x,y);
				}
				if(ID_1==ID)
				{
					player1->down();
					player1->insert(x,y);
				}
				if(ID_2==ID)
				{
					player2->down();
					player2->insert(x,y);
				}
				if(ID_3==ID)
				{
					player3->down();
					player3->insert(x,y);
				}
			}
			if(ID_0==ID) player0->change(x,y);
			if(ID_1==ID) player1->change(x,y);
			if(ID_2==ID) player2->change(x,y);
			if(ID_3==ID) player3->change(x,y);
		}
		void make()
		{
			int i,j,k;
			for(i=0;i<=3;i++)
				for(j=1;j<=10;j++)
					for(k=1;k<=10;k++)
					{
						if(1==minimap[i][j][k]) tot[i]++;
					}

			for(j=1;j<=10;j++)
				for(k=1;k<=10;k++)
				{
					int mini=10000000;
					int minID=-1;
					for(int s=0;s<=3;s++)
						if(tot[s]<mini&&1==minimap[s][j][k])
						{
							mini=tot[s];
							minID=s;
						}
					if(-1==minID) continue;
					map[j][k]=now[minID];
					area[minID]++;
				}
			player0->score=player0->score+area[0]*area[0]*10;
			player1->score=player1->score+area[1]*area[1]*10;
			player2->score=player2->score+area[2]*area[2]*10;
			player3->score=player3->score+area[3]*area[3]*10;
			for(i=0;i<=3;i++)
			if(0!=area[i]) now[i]++;
			memset(minimap,0,sizeof(minimap));
			memset(area,0,sizeof(area));
			memset(tot,0,sizeof(tot));
		}
};
dot bind[200];
enclosure* game;
ofstream fout1(".\\output1.txt");
ofstream fout2(".\\output2.txt");
ofstream fout3(".\\output3.txt");
ofstream fout4(".\\output4.txt");
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
int update_distance(dot* start_point,int ID)
{
    //Todo: 
	//1. Get unpassable lines
	//2. Do a bfs
	//Note:
	//1. An edge label 400 means specially defined unpassable lines
	//2. This update process is only valid for a certain ID, i.e. the defined player
	unsigned int tmp_distance[15][15];
	bool in_queue[15][15];
	memset(tmp_distance,0xff,sizeof(tmp_distance));
	memset(in_queue,0,sizeof(in_queue));
	queue<dot*>* q=new queue<dot*>;
	q->push(start_point);
	tmp_distance[start_point->x][start_point->y]=0;
	while((!q->empty()))
	{
		dot* tmp=q->front();
		q->pop();
		in_queue[tmp->x][tmp->y]=false;
		if((tmp->x>0)&&(edge_color[tmp->x][tmp->y][tmp->x-1][tmp->y]==-1)&&(tmp_distance[tmp->x-1][tmp->y]>tmp_distance[tmp->x][tmp->y]+1))
		{
			tmp_distance[tmp->x-1][tmp->y]=tmp_distance[tmp->x][tmp->y]+1;
			dot* tmp2=new dot;
			tmp2->x=tmp->x-1;
			tmp2->y=tmp->y;
			if(!in_queue[tmp->x-1][tmp->y])
			{
				q->push(tmp2);
				in_queue[tmp->x-1][tmp->y]=true;
			}
		}
		if((tmp->x<10)&&(edge_color[tmp->x][tmp->y][tmp->x+1][tmp->y]==-1)&&(tmp_distance[tmp->x+1][tmp->y]>tmp_distance[tmp->x][tmp->y]+1))
		{
			tmp_distance[tmp->x+1][tmp->y]=tmp_distance[tmp->x][tmp->y]+1;
			dot* tmp2=new dot;
			tmp2->x=tmp->x+1;
			tmp2->y=tmp->y;
			if(!in_queue[tmp->x+1][tmp->y])
			{
				q->push(tmp2);
				in_queue[tmp->x+1][tmp->y]=true;
			}
		}
		if((tmp->y>0)&&(edge_color[tmp->x][tmp->y][tmp->x][tmp->y-1]==-1)&&(tmp_distance[tmp->x][tmp->y-1]>tmp_distance[tmp->x][tmp->y]+1))
		{
			tmp_distance[tmp->x][tmp->y-1]=tmp_distance[tmp->x][tmp->y]+1;
			dot* tmp2=new dot;
			tmp2->x=tmp->x;
			tmp2->y=tmp->y-1;
			if(!in_queue[tmp->x][tmp->y-1])
			{
				q->push(tmp2);
				in_queue[tmp->x][tmp->y-1]=true;
			}
		}
		if((tmp->y<10)&&(edge_color[tmp->x][tmp->y][tmp->x][tmp->y+1]==-1)&&(tmp_distance[tmp->x][tmp->y+1]>tmp_distance[tmp->x][tmp->y]+1))
		{
			tmp_distance[tmp->x][tmp->y+1]=tmp_distance[tmp->x][tmp->y]+1;
			dot* tmp2=new dot;
			tmp2->x=tmp->x;
			tmp2->y=tmp->y+1;
			if(!in_queue[tmp->x][tmp->y+1])
			{
				q->push(tmp2);
				in_queue[tmp->x][tmp->y+1]=true;
			}
		}
	}
	for(int i=0;i<=10;i++) for(int j=0;j<=10;j++) relative_distance[ID][start_point->x][start_point->y][i][j]=tmp_distance[i][j];
	return -1;
}
void brain()
{
    update_edge_status();
	update_distance(game->player0->position,0);
	update_distance(game->player1->position,1);
	update_distance(game->player2->position,2);
	update_distance(game->player3->position,3);
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
	//Output Area
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
	//Debug Output
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
	fout3<<"---HORIZONTAL ROAD---"<<endl;
	for(int i=0;i<=10;i++)
	{
		for(int j=0;j<10;j++) fout3<<setiosflags(ios::fixed)<<setw(4)<<edge_color[j][i][j+1][i];
		fout3<<endl;
	}
	fout3<<"---------END---------"<<endl;
	fout3<<endl;
	fout4<<"---RELATIVE DISTANCE---"<<endl;
	for(int i=0;i<=10;i++)
	{
		for(int j=0;j<=10;j++) fout4<<setiosflags(ios::fixed)<<setw(4)<<relative_distance[now_id][now_x][now_y][j][i];
		fout4<<endl;
	}
	fout4<<"----------END----------"<<endl;
	fout4<<endl;
    return;
}
int main()
{
	for(int i=0;i<4;i++) now[i]=100*i;
    string label;
    cin>>label;
    cin>>now_id;
    cout<<"[POS] 10 0"<<endl;
    memset(map,0xff,sizeof(map));
    memset(edge_color,0xff,sizeof(edge_color));
	memset(relative_distance,0xff,sizeof(relative_distance));
    bool started=false; //Update: variable name changed from visited to started
    while (true)
    {
        cin>>label;
        for(int i=0;i<=3;i++)
        {
            cin>>x[i]>>y[i]>>state[i]>>trappedleft[i]>>scoredecline[i];
        }
        if(!started)
        {
            started=true;
            game=new enclosure(x[0],y[0],x[1],y[1],x[2],y[2],x[3],y[3]);
        }
        game->pass(ID_0,x[0],y[0],state[0]);
        game->pass(ID_1,x[1],y[1],state[1]);
        game->pass(ID_2,x[2],y[2],state[2]);
        game->pass(ID_3,x[3],y[3],state[3]);
        game->make();
        int total;
        cin>>total;
        for(int i=1;i<=total;i++)
        {
            cin>>bind[i].x>>bind[i].y>>timeleft[i];
        }
        brain();
    }
}
