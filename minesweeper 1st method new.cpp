#include<iostream>
#include<iomanip>
#include<stdlib.h>
#include<graphics.h>
#include<conio.h>
#include<string.h>
#include<ctime>
using namespace std;


int nbombs = 20;
int length=0, width=0;
int *mainarr;
int *userarr;
bool *flagarr;
float *probabilityarr;
bool *digarr;
bool *safe_cell_arr;

int digc = 0, safe_cell_count = 0, nflags=0;

int safe = 1;

void initialise_arrays()
{
	cout<<"\nEnter the length: ";
	cin>>length;
	cout<<"\nEnter the width: ";
	cin>>width;
	
	mainarr = new int[length*width];
	userarr = new int[length*width];
	flagarr = new bool[length*width];
	probabilityarr = new float[length*width];
	digarr = new bool[length*width];
	safe_cell_arr = new bool[length*width];
	int i, j, k;
	for(i=0;i<length;i++)
	{
		k = i*width;
		for(j=0;j<width;j++)
		{
			mainarr[k+j] = 0;
			userarr[k+j] = 0;
			flagarr[k+j] = 0;
			probabilityarr[k+j] = -1;
			digarr[k+j] = 0;
			safe_cell_arr[k+j] = 0;
			
		}
	}
	
}


void add_to_safe(int r, int c)
{
	if(safe_cell_arr[r*width+c] == 0)
	{
		safe_cell_arr[r*width+c] = 1;
		safe_cell_count++;
	}
}

void remove_from_safe(int x, int y)
{
	if(safe_cell_arr[x*width+y] == 1)
	{
		safe_cell_arr[x*width+y] = 0;
		safe_cell_count--;	
	}
	
}



void flagcell(int r, int c)
{
	if(flagarr[r*width+c] == 0)
	{
		flagarr[r*width+c] = 1;
		nflags++;
	}
	
}

void unflagcell(int r, int c)
{
	if(flagarr[r*width+c]==1)
	{
		flagarr[r*width+c] = 0;
		nflags--;
	}
	 
}

int is_dug(int r, int c)
{
	if(digarr[r*width+c]==1)
	{
		return 1;
	}
	
	return 0;
	
}


void dig_add(int r, int c)
{
	if(digarr[r*width+c] == 0)
	{
		digarr[r*width+c] = 1;
		digc++;
	}
	
}


int dig(int r, int c)
{
	dig_add(r, c);
	
	if(mainarr[r*width+c] == -1)
	{
		safe = 0;
		return 0;
	}
	
	if(mainarr[r*width+c] > 0)
	{
		return 1;
	}
	
	int i=0, j=0;
	for(i = max(0, r-1); i < min(length-1, r+1)+1; i++)
	{
		for(j = max(0, c-1); j < min(width-1, c+1)+1; j++)
		{
			if(is_dug(i,j)==1)
			{
				continue;			
			}
			
			dig(i, j);
		}
	}
	
	return 1;
}


void print(int *arr)
{
	int i=0,j=0;
	cout<<setw(5);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			if(arr[i*width+j]<-1)
			{
				cout<<" "<<setw(5);
			}
			else
			{
				cout<<arr[i*width+j]<<setw(5);
			}
		}
		cout<<endl<<endl;
	}
}

int get_number_of_bombs(int x, int y)
{
	int i=0, j=0, val=0;
	for(i = max(0, x-1); i < min(length-1, x+1)+1; i++)
	{
		for(j = max(0, y-1); j < min(width-1, y+1)+1; j++)
		{
			if(!(i==x && j==y))
			{
				if(mainarr[i*width+j] == -1)
				{
					val+=1;
				}
			}
		}
	}
	return val;
}

void createmines(unsigned int seed)
{
	int i=0, j=0;
	cout<<"Creating mines...\n\n";
	nbombs=0;
	srand(seed);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			mainarr[i*width+j] = -(rand()%2 && rand()%2 && rand()%2);
			if(mainarr[i*width+j]<0)
			{
				nbombs++;
			}
			
		}
	}
	//print(mainarr);
}

void createmines1()
{
	int i=0, j=0, k=0, loc=0, x=0;
	x = length*width;
	while(k<nbombs)
	{
		loc = rand()%x;
		j = loc%10;
		i = (loc - j)/10;
		if(mainarr[i*width+j]!=-1)
		{
			mainarr[i*width+j] = -1;
		}
		k++;
	}
}


void assignvalues()
{
	int i=0, j=0;
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			if(mainarr[i*width+j] == -1)
			{
				continue;
			}
			mainarr[i*width+j] = get_number_of_bombs(i, j);
		}
	}
}


int min(int x, int y)
{
	if(x<y)
	{
		return x;
	}
	else
	{
		return y;
	}
}

int max(int x, int y)
{
	if(x>y)
	{
		return x;
	}
	else
	{
		return y;
	}
}

void update_user_board()
{
	int i=0, j=0;
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			if(flagarr[i*width+j] == 0)
			{
				if(is_dug(i, j)==1)
				{
					userarr[i*width+j] = mainarr[i*width+j];
				}
				else
				{
					userarr[i*width+j] = -2;
				}
			}
			else
			{
				userarr[i*width+j] = -1;
			}
	}
	}
}


void initdisplay()
{
	//int gd=DETECT, gm;
	//initgraph(&gd, &gm, (char*)"");
	initwindow(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), "", -3, -3);
}


void makecell(int x, int y, int val)
{
	int i=0, j=0;
	setcolor(WHITE);
	rectangle(x,y,20+x,20+y);
	if(val==0)
	{
		setfillstyle(SOLID_FILL, CYAN);
		floodfill(x+1, y+1, WHITE);
	}
	if(val>0)
	{
		char c[1];
		sprintf(c, "%d", val);
		outtextxy(x+7, y+2, c);
		return;
	}
	if(val==-1)
	{
		setcolor(YELLOW);
		outtextxy(x+7, y+2, "F");
		return;
	}
	if(val==-2)
	{
		setfillstyle(SOLID_FILL, WHITE);
		floodfill(x+1, y+1, WHITE);
	}
	
}

void displayboard()
{
	cleardevice();
	int i=0,j=0,boardspace=1, cm = 22,tx = 0, ty = 0;
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			makecell(cm*j+tx,cm*i+ty, userarr[i*width+j]);
		}
	}
	
}


void get_user_input()
{
	int val=0, x, y;
	cout<<"\nFLAG, UNFLAG, DIG? (0,1,2) ";
	cin>>val;
	cout<<"\nENTER THE ROW: ";
	cin>>x;
	cout<<"\nENTER THE COLUMN: ";
	cin>>y;
	
	while(x<0 || y<0 || x>=length || y>=width)
	{
		cout<<"\nINVALID LOCATION\n";
		cout<<"\nENTER THE ROW: ";
		cin>>x;
		cout<<"\nENTER THE COLUMN: ";
		cin>>y;
	}
	
	cout<<"\nSelected Location is ("<<x<<","<<y<<")\n";
	if(val==0)
	{
		flagcell(x, y);
	}
	if(val==1)
	{
		unflagcell(x, y);
	}
	if(val==2)
	{
		dig(x, y);
	}
	

}


void change_cell_prob(int x, int y, float p)
{
	if(probabilityarr[x*width+y]<0)
	{
		probabilityarr[x*width+y] = 1;
	}
	probabilityarr[x*width+y] = probabilityarr[x*width+y] + p;
}

void select_random_cell(int &r, int &c)
{
	do
	{
		r = rand()%length;
		c = rand()%width;
	}while(is_dug(r,c)==1);
	
}


void reset_probabilityarr()
{
	int i=0,j=0;
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			probabilityarr[i*width+j] = -1;
		}
	}
}

void select_min_prob()
{
	int i=0,j=0,r,c,counter=0, x=0;
	float min = 10;
	
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			if(probabilityarr[i*width+j]<0)
			{
				counter++;
			}
			else
			{
				if(probabilityarr[i*width+j]<min)
				{
					r = i;
					c = j;
					min = probabilityarr[i*width+j];
				}
			}
		}
	}
	
	x = length*width;
	
	if(counter==x)
	{
		srand((int) time(0));
		select_random_cell(r, c);
	}
	
	
	dig(r, c);
	reset_probabilityarr();
	
}

void calculate_cell_prob(int x, int y)
{
	int i = 0, j = 0, covered_cell_count=0, mine_count=0;
	int arr[8][2];
	for(i = max(0, x-1); i < min(length-1, x+1)+1; i++)
	{
		for(j = max(0, y-1); j < min(width-1, y+1)+1; j++)
		{
			if(i==x && j==y)
			{
				continue;
			}
			if(userarr[i*width+j]==-2)
			{
				arr[covered_cell_count][0] = i;
				arr[covered_cell_count][1] = j;
				covered_cell_count++;
			}
			if(userarr[i*width+j]==-1)
			{
				mine_count++;
			}
			
		}
	}
	
	if(covered_cell_count==0)
	{
		return;
	}
	float p = 0;
	j = userarr[x*width+y]-mine_count;
	if(j<0)
	{
		cout<<"Probability error!\n";
	}
	p = (float)(j)/(float)covered_cell_count;
	for(i=0;i<covered_cell_count;i++)
	{
		if(j==0)
		{
			add_to_safe(arr[i][0], arr[i][1]);
		}
		else
		{
			if(p==1)
			{
				flagcell(arr[i][0], arr[i][1]);
			}
			else
			{
				change_cell_prob(arr[i][0], arr[i][1], p);
			}
		}
	}
	
	
	
}

void find_mine_prob()
{
	int i=0, j=0;
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			if(userarr[i*width+j]>0)
			{
				calculate_cell_prob(i, j);
			}
		}
	}
}

void make_decision()
{
	if(safe_cell_count>0)
	{
		int i, j;
		for(i=0;i<length;i++)
		{
			for(j=0;j<width;j++)
			{
				if(safe_cell_arr[i*width+j]==1)
				{
					dig(i, j);
					remove_from_safe(i, j);
				}
			}
		}
	}
	else
	{
		select_min_prob();
	}
}



int main()
{
	initialise_arrays();
	int i=0, j=0 ,k = length*width, val=0;
	int mode = 0;
	createmines((int) time(0));
	assignvalues();
	//print(mainarr);
	cout<<"\n\n\n";
	//print(userarr);
	reset_probabilityarr();
	initdisplay();
	int page = 0;
	while(digc<k-nbombs)
	{
		setactivepage(page);
		setvisualpage(1-page);
		update_user_board();
		displayboard();
		page = 1 - page;
		setactivepage(page);
		setvisualpage(1-page);
		cout<<"\nAuto or manual? (0,1) ";
		cin>>mode;
		if(mode==0)
		{
			find_mine_prob();
			make_decision();
		}
		else
		{
			get_user_input();
		}
		
		if(safe == 0)
		{
			break;
		}
	}
	
	if(safe)
	{
		cout<<"\n\nYOU WON!\n\n";
	}
	else
	{
		cout<<"\n\nYOU LOSE!\n\n";
	}
}
