#include<cstdio>
#include<iostream>
#include<cstring>
#include<fstream>
#include<iomanip>
#include<cstdlib>
#include<queue>
#include<stack>
#include<cmath>
#include<climits>
#define state_up 0
#define state_down 1
#define state_die -1
#define mode_start 0
#define mode_right 1
#define mode_up 2
#define mode_left 3
#define mode_down 4
#define mode_halt 5
#define mode_idle 6
#define mode_draw 7
using namespace std;
int map[15][15];
int minimap[4][15][15];
int now[4],tot[4],area[4];
int m,n,bind_total;
int x[4],y[4],state[4],trappedleft[4],scoredecline[4];
int now_id;
int edge_color[15][15][15][15];
int timeleft[200];
int mode=mode_start;
unsigned int relative_distance[4][15][15][15][15];
ofstream fout1(".\\output1.txt");
ofstream fout2(".\\output2.txt");
ofstream fout3(".\\output3.txt");
ofstream fout4(".\\output4.txt");
ofstream fout5(".\\output5.txt");
ofstream fout6(".\\output6.txt");
class dot
{
	public:
		int x;
		int y;
};
class exdot
{
	public:
		int x;
		int y;
		int meta[10];
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
		void cut()
		{
			num--;
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
			int i,j,k,s=1;
			for(i=1;i<=num-1;i++)
				if(road[i]->x==road[num]->x&&road[i]->y==road[num]->y) s=i;
			fout5<<"---------ROAD----------"<<endl;
			fout5<<"num "<<num<<endl;
			fout5<<"s "<<s<<endl;
			for(i=1;i<=num;i++)
				fout5<<road[i]->x<<" "<<road[i]->y<<endl;
			fout5<<"---------END-----------"<<endl;
			fout5<<endl;
			for(i=1;i<=10;i++)
				for(j=1;j<=10;j++)
				{
					bool left=false;
					bool right=false;
					bool top=false;
					bool bottom=false;
					for(k=s+1;k<=num;k++)
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
		player* players[4];
		enclosure(int* x,int* y)
		{
			for(int i=0;i<4;i++) players[i]=new player(i,x[i],y[i]);
		}
		enclosure(){}
		void pass(int ID,int x,int y,int state)
		{
			if(state_die==state)
			{
				players[ID]->die();
				players[ID]->insert(x,y);
				players[ID]->paint();
				players[ID]->clear();
			}
			if(state_up==state)
			{
				if(state_down==players[ID]->state) players[ID]->insert(x,y);
				players[ID]->paint();
				players[ID]->up();
				players[ID]->clear();
				players[ID]->insert(x,y);
			}
			if(state_down==state)
			{
				players[ID]->down();
				players[ID]->insert(x,y);
			}
			players[ID]->change(x,y);
		}
		void make()
		{
			for(int i=0;i<=3;i++)
				for(int j=1;j<=10;j++)
					for(int k=1;k<=10;k++)
					{
						if(1==minimap[i][j][k]) tot[i]++;
					}

			for(int j=1;j<=10;j++)
				for(int k=1;k<=10;k++)
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
			for(int i=0;i<=3;i++) players[i]->score+=area[i]*area[i]*10;
			for(int i=0;i<=3;i++) if(0!=area[i]) now[i]++;
			memset(minimap,0,sizeof(minimap));
			memset(area,0,sizeof(area));
			memset(tot,0,sizeof(tot));
		}
};
dot* bind[200];
dot* bestroad[300];
enclosure* game;

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
                if(j==0||j==10||map[i][j]!=map[i][j+1]||map[i][j]==-1||map[i][j+1]==-1) edge_color[i][j][i-1][j]=-1;
                else edge_color[i][j][i-1][j]=map[i][j];
            }
            if(i<10)
            {
                if(j==0||j==10||map[i+1][j]!=map[i+1][j+1]||map[i+1][j]==-1||map[i+1][j+1]==-1) edge_color[i][j][i+1][j]=-1;
                else edge_color[i][j][i+1][j]=map[i+1][j+1];
            }
            if(j>0)
            {
                if(i==0||i==10||map[i][j]!=map[i+1][j]||map[i][j]==-1||map[i+1][j]==-1) edge_color[i][j][i][j-1]=-1;
                else edge_color[i][j][i][j-1]=map[i+1][j];
            }
            if(j<10)
            {
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
    //1. An edge label 400 or 500 means specially defined unpassable lines
    //2. This update process is only valid for a certain ID, i.e. the defined player
    unsigned int tmp_distance[15][15];
    bool in_queue[15][15];
    memset(tmp_distance,0xff,sizeof(tmp_distance));
    memset(in_queue,0,sizeof(in_queue));
    queue<dot*>* q=new queue<dot*>;
    q->push(start_point);
    tmp_distance[start_point->x][start_point->y]=0;
    while ((!q->empty()))
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
int calculate_area(dot** x,int dot_num)
{
    int i,j,k,s=1;
    int ans=0;
    for(i=1;i<=dot_num-1;i++)
        if(x[i]->x==x[dot_num]->x&&x[i]->y==x[dot_num]->y) s=i;
    for(i=1;i<=10;i++)
        for(j=1;j<=10;j++)
        {
            bool left=false;
            bool right=false;
            bool top=false;
            bool bottom=false;
            for(k=s+1;k<=dot_num;k++)
            {
                if(x[k-1]->x==x[k]->x&&x[k-1]->y==x[k]->y) continue;
                if(x[k-1]->x==x[k]->x)
                {
                    if(x[k-1]->y<x[k]->y)
                    {
                        if(j==x[k]->y&&i>x[k]->x) top=true;
                        if(j==x[k]->y&&i<=x[k]->x) bottom=true;
                    }
                    if(x[k-1]->y>x[k]->y)
                    {
                        if(j==x[k-1]->y&&i>x[k]->x) top=true;
                        if(j==x[k-1]->y&&i<=x[k]->x) bottom=true;
                    }
                    continue;
                }
                if(x[k-1]->y==x[k]->y)
                {
                    if(x[k-1]->x<x[k]->x)
                    {
                        if(i==x[k]->x&&j>x[k]->y) left=true;
                        if(i==x[k]->x&&j<=x[k]->y) right=true;
                    }
                    if(x[k-1]->x>x[k]->x)
                    {
                        if(i==x[k-1]->x&&j>x[k]->y) left=true;
                        if(i==x[k-1]->x&&j<=x[k]->y) right=true;
                    }
                }
            }
            if(left&&right&&top&&bottom&&-1==map[i][j]) ans++;
        }
    return ans;
}
inline int get_distance(int ID,dot* p1,dot* p2,double heuristic)
{
	int tmp_distance=relative_distance[ID][p1->x][p1->y][p2->x][p2->y];
	double h_distance=(double)tmp_distance*heuristic;
	//if((*p1)==(*(game->players[ID]->position))&&(*p2)==(*(game->players[ID]->position))) return 0;
	if(p1->x==game->players[ID]->position->x&&p1->y==game->players[ID]->position->y&&p2->x==game->players[ID]->position->x&&p2->y==game->players[ID]->position->y) return 0;
	else return (floor(h_distance)+game->players[ID]->trappedleft);
}
int calculate_maximum_safe_enclosure(int ID,int road_id,double heuristic)
{
	if((int)(relative_distance[ID][game->players[ID]->position->x][game->players[ID]->position->y][game->players[ID]->road[road_id]->x][game->players[ID]->road[road_id]->y])==-1) return;
	int tmp_num=0,max_length=INT_MAX;
	dot* tmp_road[300];
	for(int i=0;i<300;i++) tmp_road[i]=new dot;
	for(int i=road_id;i<game->players[ID]->num;i++) tmp_road[++tmp_num]=game->players[ID]->road[i];
	fout6<<"-----AI LOG------"<<endl;
	fout6<<"CURRENT POSITION: "<<"("<<game->players[ID]->position->x<<","<<game->players[ID]->position->y<<")"<<endl;
	for(int i=1;i<game->players[ID]->num;i++)
	{
		int tmp_min_length=INT_MAX;
		for(int j=0;j<4;j++)
		{	
			if(j==ID) continue;
			int real_distance=get_distance(j,game->players[j]->position,game->players[ID]->road[i],heuristic);
			if(real_distance<tmp_min_length) tmp_min_length=real_distance;
		}
		if(tmp_min_length<max_length) max_length=tmp_min_length;
	}
	fout6<<"ESTIMATED TURN LEFT: "<<max_length<<endl;
	int maximum_area=-1;
	for(int i=(int)relative_distance[ID][game->players[ID]->position->x][game->players[ID]->position->y][game->players[ID]->road[1]->x][game->players[ID]->road[1]->y];i<=max_length;i++)
	{
		fout6<<"TESTING LENGTH: "<<i<<endl;
		exdot* s[300];
		dot* target=new dot;
		target=game->players[ID]->road[road_id];
		for(int j=0;j<300;j++) s[j]=new exdot;
		bool visited[15][15];
		memset(visited,0,sizeof(visited));
		int s_size=0;
		exdot* tmp=new exdot;
		tmp->x=game->players[ID]->position->x;
		tmp->y=game->players[ID]->position->y;
		tmp->meta[0]=0;
		s[s_size++]=tmp;
		visited[tmp->x][tmp->y]=true;
		fout6<<"DFS INITIALIZED"<<endl;
		while(s_size>0)
		{
			exdot* tmp1=new exdot;
			tmp1=s[s_size-1];
			if((s_size>i&&!(tmp1->x==target->x&&tmp1->y==target->y))||(s_size!=i+1&&tmp1->x==target->x&&tmp1->y==target->y))
			{
				//fout6<<"STACK OVERFLOW AT POINT ("<<tmp1->x<<","<<tmp1->y<<")"<<endl;
				s_size--;
				visited[tmp1->x][tmp1->y]=false;
				continue;
			}
			//fout6<<"APPROACHED POINT ("<<tmp1->x<<","<<tmp1->y<<")"<<endl;
			if(tmp1->x==target->x&&tmp1->y==target->y&&s_size==i+1)
			{
				fout6<<"TARGET APPROACHED"<<endl;
				dot* sp[300];
				for(int j=0;j<300;j++) sp[j]=new dot;
				for(int j=1;j<=tmp_num;j++) sp[j]=tmp_road[j];
				for(int j=0;j<s_size;j++)
				{
					dot* tmp2=new dot;
					tmp2->x=s[j]->x;
					tmp2->y=s[j]->y;
					sp[tmp_num+j+1]=tmp2;
				}
				fout6<<"CALCULATING AREA..."<<endl;
				int tmp_area=calculate_area(sp,tmp_num+s_size);
				fout6<<"EXPECTED AREA: "<<tmp_area<<endl;
				fout6<<"CURRENT MAXIMUM EXPECTED AREA: "<<maximum_area<<endl;
				if(tmp_area>maximum_area) 
				{
					fout6<<"REPLACING MAXIMUM EXPECTED AREA... "<<endl;
					maximum_area=tmp_area;
					fout6<<"TRACING...
					for(int j=0;j<s_size;j++)
					{
						bestroad[j]->x=s[j-1]->x;
						bestroad[j]->y=s[j-1]->y;
					}
				}
				s_size--;
				visited[tmp1->x][tmp1->y]=false;
			}
			else if(tmp1->meta[0]==0)
			{
				tmp1->meta[0]++;
				if(tmp1->y<10&&edge_color[tmp1->x][tmp1->y][tmp1->x][tmp1->y+1]==-1&&!visited[tmp1->x][tmp1->y+1])
				{
					bool safe=true;
					for(int j=0;j<4;j++)
					{
						if(j!=now_id)
						{
							dot* p=new dot;
							p->x=tmp1->x;
							p->y=tmp1->y+1;
							int tmp_distance=get_distance(j,game->players[j]->position,p,heuristic);
							if(tmp_distance<i) safe=false;

						}
					}
					if(safe)
					{
						exdot* tmp2=new exdot;
						tmp2->x=tmp1->x;
						tmp2->y=tmp1->y+1;
						tmp2->meta[0]=0;
						visited[tmp2->x][tmp2->y]=true;
						s[s_size++]=tmp2;
					}
				}
			}
			else if(tmp1->meta[0]==1)
			{
				tmp1->meta[0]++;
				if(tmp1->y>0&&edge_color[tmp1->x][tmp1->y][tmp1->x][tmp1->y-1]==-1&&!visited[tmp1->x][tmp1->y-1])
				{
					bool safe=true;
					for(int j=0;j<4;j++)
					{
						if(j!=now_id)
						{
							dot* p=new dot;
							p->x=tmp1->x;
							p->y=tmp1->y-1;
							int tmp_distance=get_distance(j,game->players[j]->position,p,heuristic);
							if(tmp_distance<i) safe=false;

						}
					}
					if(safe)
					{
						exdot* tmp2=new exdot;
						tmp2->x=tmp1->x;
						tmp2->y=tmp1->y-1;
						tmp2->meta[0]=0;
						visited[tmp2->x][tmp2->y]=true;
						s[s_size++]=tmp2;
					}
				}
			}
			else if(tmp1->meta[0]==2)
			{
				tmp1->meta[0]++;
				if(tmp1->x<10&&edge_color[tmp1->x][tmp1->y][tmp1->x+1][tmp1->y]==-1&&!visited[tmp1->x+1][tmp1->y])
				{
					bool safe=true;
					for(int j=0;j<4;j++)
					{
						if(j!=now_id)
						{
							dot* p=new dot;
							p->x=tmp1->x+1;
							p->y=tmp1->y;
							int tmp_distance=get_distance(j,game->players[j]->position,p,heuristic);
							if(tmp_distance<i) safe=false;

						}
					}
					if(safe)
					{
						exdot* tmp2=new exdot;
						tmp2->x=tmp1->x+1;
						tmp2->y=tmp1->y;
						tmp2->meta[0]=0;
						visited[tmp2->x][tmp2->y]=true;
						s[s_size++]=tmp2;
					}
				}
			}
			else if(tmp1->meta[0]==3)
			{
				tmp1->meta[0]++;
				if(tmp1->x>0&&edge_color[tmp1->x][tmp1->y][tmp1->x-1][tmp1->y]==-1&&!visited[tmp1->x-1][tmp1->y])
				{
					bool safe=true;
					for(int j=0;j<4;j++)
					{
						if(j!=now_id)
						{
							dot* p=new dot;
							p->x=tmp1->x-1;
							p->y=tmp1->y;
							int tmp_distance=get_distance(j,game->players[j]->position,p,heuristic);
							if(tmp_distance<i) safe=false;

						}
					}
					if(safe)
					{
						exdot* tmp2=new exdot;
						tmp2->x=tmp1->x-1;
						tmp2->y=tmp1->y;
						tmp2->meta[0]=0;
						visited[tmp2->x][tmp2->y]=true;
						s[s_size++]=tmp2;
					}
				}
			}
			else if(tmp1->meta[0]==4)
			{
				visited[tmp1->x][tmp1->y]=false;
				s_size--;
			}
		}
	}
	return maximum_area;
}
void isolate_bind()
{
	for(int j=1;j<=bind_total;j++)
	{
		if(bind[j]->x>0) edge_color[bind[j]->x-1][bind[j]->y][bind[j]->x][bind[j]->y]=edge_color[bind[j]->x][bind[j]->y][bind[j]->x-1][bind[j]->y]=500;
		if(bind[j]->x<=10) edge_color[bind[j]->x+1][bind[j]->y][bind[j]->x][bind[j]->y]=edge_color[bind[j]->x][bind[j]->y][bind[j]->x+1][bind[j]->y]=500;
		if(bind[j]->y>0) edge_color[bind[j]->x][bind[j]->y+1][bind[j]->x][bind[j]->y]=edge_color[bind[j]->x][bind[j]->y][bind[j]->x][bind[j]->y-1]=500;
		if(bind[j]->y<=10) edge_color[bind[j]->x][bind[j]->y-1][bind[j]->x][bind[j]->y]=edge_color[bind[j]->x][bind[j]->y][bind[j]->x][bind[j]->y+1]=500;
	}
}
void brain()
{
	dot* tmp_bestroad[300];
	for(int i=0;i<300;i++) tmp_bestroad[i]=new dot;
    update_edge_status();
	int original_edge_color[15][15][15][15];
	memcpy(original_edge_color,edge_color,sizeof(original_edge_color));
	for(int i=0;i<4;i++)
	{
		memcpy(edge_color,original_edge_color,sizeof(edge_color));
		isolate_bind();
		if(game->players[i]->state==state_down)
		{
			for(int j=1;j<game->players[i]->num;j++)
			{
				dot* p1=new dot;
				dot* p2=new dot;
				p1=game->players[i]->road[j];
				p2=game->players[i]->road[j+1];
				edge_color[p1->x][p1->y][p2->x][p2->y]=edge_color[p2->x][p2->y][p1->x][p1->y]=400;
			}
		}
		else if(game->players[i]->state==state_up)
		{
			//Downward Road
			for(int j=0;j<10;j++) for(int k=0;k<=10;k++) if((edge_color[j][k][j+1][k]/100)==i) edge_color[j][k][j+1][k]=-1;
			//Upward Road
			for(int j=1;j<=10;j++) for(int k=0;k<=10;k++) if((edge_color[j][k][j-1][k]/100)==i) edge_color[j][k][j-1][k]=-1;
			//Rightward Road
			for(int j=0;j<=10;j++) for(int k=0;k<10;k++) if((edge_color[j][k][j][k+1]/100)==i) edge_color[j][k][j][k+1]=-1;
			//Leftward Road
			for(int j=0;j<=10;j++) for(int k=1;k<=10;k++) if((edge_color[j][k][j][k-1]/100)==i) edge_color[j][k][j][k-1]=-1;
		}
		update_distance(game->players[i]->position,i);
	}
	memcpy(edge_color,original_edge_color,sizeof(edge_color));
	if(game->players[now_id]->state==state_down)
	{
		isolate_bind();
		for(int j=1;j<game->players[now_id]->num;j++)
		{
			dot* p1=new dot;
			dot* p2=new dot;
			p1=game->players[now_id]->road[j];
			p2=game->players[now_id]->road[j+1];
			edge_color[p1->x][p1->y][p2->x][p2->y]=edge_color[p2->x][p2->y][p1->x][p1->y]=400;
		}
		for(int j=1;j<game->players[now_id]->num;j++) calculate_maximum_safe_enclosure(now_id,j,1);
	}
	memcpy(edge_color,original_edge_color,sizeof(edge_color));
    int now_x[4],now_y[4];
	for(int i=0;i<4;i++)
	{
		now_x[i]=game->players[i]->position->x;
		now_y[i]=game->players[i]->position->y;
	}
    //Finite State Machine
	/*
    if(mode==mode_start)
    {
        mode=mode_right;
    }
    else if(mode==mode_right)
    {
        if(now_x[now_id]==10&&now_y[now_id]==10) mode=mode_up;
    }
    else if(mode==mode_up)
    {
        if(now_x[now_id]==0&&now_y[now_id]==10) mode=mode_left;
    }
    else if(mode==mode_left)
    {
        if(now_x[now_id]==0&&now_y[now_id]==0) mode=mode_down;
    }
    else if(mode==mode_down)
    {
        if(now_x[now_id]==10&&now_y[now_id]==0) mode=mode_halt;
    }
    //Output Area
    if(mode==mode_right&&now_y[now_id]==0)
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
	*/
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
        for(int j=0;j<=10;j++)
        {
            int rd=relative_distance[now_id][now_x[now_id]][now_y[now_id]][j][i];
            fout4<<setiosflags(ios::fixed)<<setw(4)<<rd;
        }
        fout4<<endl;
    }
    fout4<<"----------END----------"<<endl;
    fout4<<endl;
    return;
}
int main()
{
    for(int i=0;i<4;i++) now[i]=100*i;
	for(int j=0;j<300;j++) bestroad[j]=new dot;
	for(int i=0;i<200;i++) bind[i]=new dot;
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
            //game=new enclosure(x[0],y[0],x[1],y[1],x[2],y[2],x[3],y[3]);
			game=new enclosure(x,y);
        }
        game->pass(0,x[0],y[0],state[0]);
        game->pass(1,x[1],y[1],state[1]);
        game->pass(2,x[2],y[2],state[2]);
        game->pass(3,x[3],y[3],state[3]);
        game->make();
        cin>>bind_total;
        for(int i=1;i<=bind_total;i++)
        {
            cin>>bind[i]->x>>bind[i]->y>>timeleft[i];
        }
        brain();
    }
}
