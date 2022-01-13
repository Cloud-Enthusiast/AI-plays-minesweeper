#include<iostream>
#include<iomanip>
#include<stdlib.h>
#include<graphics.h>
#include<conio.h>
#include<string.h>
#include<math.h>
#include <bits/stdc++.h>
using namespace std;


int nbombs = 20;
int length=0, width=0;
int *mainarr;
int *userarr;
bool *flagarr;
float *probabilityarr;
bool *digarr;
bool *safe_cell_arr;

int buttonloc[2*4] = {0}, buttoncount = 0;
int mouseoffx=0, mouseoffy = 0;
int boardtx = 600, boardty = 250, cm=22;

int digc = 0, safe_cell_count = 0, nflags=0, page = 0;
int screenx = GetSystemMetrics(SM_CXSCREEN), screeny = GetSystemMetrics(SM_CYSCREEN);

bool calibratemouseatstart=0;

int safe = 1;

POINT cpos;



void prepdisplay()
{
	setactivepage(page);
	setvisualpage(1-page);
}

void display()
{
	page = 1 - page;
	setactivepage(page);
	setvisualpage(1-page);
}

void highlight_cell(int r, int c)
{
	int x = cm*c+boardtx;
	int y = cm*r+boardty;
	setcolor(YELLOW);
	rectangle(x,y, 20+x, 20+y);
	setfillstyle(SOLID_FILL, YELLOW);
	floodfill(x+1, y+1, BLACK);
}

int get_parameters()
{
	cout<<"Enter the length: ";
	cin>>length;
	cout<<"Enter the width: ";
	cin>>width;
	cout<<"Enter the board starting X position: ";
	cin>>boardtx;
	cout<<"Enter the board starting Y position: ";
	cin>>boardty;
	cout<<"Enter the number of bombs: ";
	cin>>nbombs;
	while(nbombs>length*width)
	{
		cout<<"Number of mines are greater than the number of cells!\n";
		cout<<"Enter the number of bombs: ";
		cin>>nbombs;
	}
	cout<<"Do you want to run a benchmark? ";
	int i=0;
	cin>>i;
	if(i==1)
	{
		cout<<"How many games should the AI play? ";
		cin>>i;
		return i;
	}
	else
	{
		cout<<"Do you want to calibrate the mouse cursor? ";
		cin>>i;
		if(i==1)
		{
			calibratemouseatstart=1;
		}
	}
	return 0;
}

void createbutton(char* val,int tx, int ty, int sno,int offx=0)
{
	int rectanglelen = strlen(val)*8+offx;
	rectangle(tx, ty, rectanglelen+tx, 20+ty);
	outtextxy(tx+7, ty+2, val);
	if(sno>=buttoncount)
	{
		buttonloc[buttoncount*4] = tx;
		buttonloc[buttoncount*4+1] = ty;
		buttonloc[buttoncount*4+2] = rectanglelen+tx;
		buttonloc[buttoncount*4+3] = 20+ty;
		buttoncount++;
	}
	
}

void initialise_arrays()
{
	
	mainarr = new int[length*width];
	userarr = new int[length*width];
	flagarr = new bool[length*width];
	probabilityarr = new float[length*width];
	digarr = new bool[length*width];
	safe_cell_arr = new bool[length*width];
	
	
}

void resetarrays()
{
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
	digc = 0;
	safe_cell_count = 0;
	nflags=0;
	safe = 1;
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

void createfixedmines(unsigned int seed)
{
	int i=0, j=0, k=0;
	srand(seed);
	while(k<nbombs)
	{
		i = rand()%length;
		j = rand()%width;
		if(mainarr[i*width+j] != -1)
		{
			mainarr[i*width+j] = -1;
			k++;
		}
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
	initwindow(screenx, screeny, "", -3, -3);
	//initwindow(1000,1000, "",100,100,1,1);
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
		outtextxy(x+7, y+2, (char*)"F");
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
	int i=0,j=0;
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			makecell(cm*j+boardtx,cm*i+boardty, userarr[i*width+j]);
		}
	}
	setcolor(WHITE);
	createbutton((char*)"AI Solve", max(min(boardtx+width*cm, screenx)-150, 0), min(boardty+length*cm+3, screeny-25), 0);
	createbutton((char*)"Exit", max(min(boardtx+width*cm, screenx)-50, 100), min(boardty+length*cm+3, screeny-25), 1, 6);
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


int in_array(int x, int y,const int arr[], int c, int dimy)
{
	int i=0, j=0;
	for(i=0;i<c;i++)
	{
		if(arr[i*dimy+0] == x && arr[i*dimy+1] == y)
		{
			return i;
		}
	}
	return -1;
}


void get_neightboring_covered_cells(int x, int y, int arr[], int &ac)
{
	int i=0, j=0;
	for(i = max(0, x-1); i < min(length-1, x+1)+1; i++)
	{
		for(j = max(0, y-1); j < min(width-1, y+1)+1; j++)
		{
			if(i==x && j==y)
			{
				continue;
			}
			else
			{
				if(userarr[i*width+j] == -2 && flagarr[i*width+j]!=1 && in_array(i, j, arr, ac, 2)==-1)
				{
					arr[ac*2] = i;
					arr[ac*2+1] = j;
					ac++;
					highlight_cell(i, j);
				}
			}
		}
	}
}

bool int_to_binary(int x, bool arr[], int counter)
{
	int i = 0, val=0;
	for(i=counter-1;i>=0;i--)
	{
		arr[i] = x%2;
		x = x/2;
		if(arr[i]==1)
		{
			val++;
			if(val>nbombs-nflags)
			{
				return 0;
			}
		}
	}
	return 1;
	
	
}


int is_mine(int x, int y, const bool c[], int perimeter[], int pc)
{
	int i=0, j=0;
	i = in_array(x,y,perimeter,pc, 2);
	if(i!=-1)
	{
		if(c[i]==0)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		cout<<"\nError the point ("<<x<<","<<y<<") is not in the array\n";
		return -1;
	}
}


int check_comb(int x, int y, bool combination[], int perimeter[],int pc)
{
	int i=0,j=0,val=0, t = 0;
	for(i = max(0, x-1); i < min(length-1, x+1)+1; i++)
	{
		for(j = max(0, y-1); j < min(width-1, y+1)+1; j++)
		{
			if(i==x && j==y)
			{
				continue;
			}
			else
			{
				if(userarr[i*width+j]==-1)
				{
					val++;
				}
				else
				{
					if(userarr[i*width+j]==-2)
					{
						t = is_mine(i, j, combination, perimeter, pc);
						if(t==-1)
						{
							cout<<"\nError! value does not exist in the array perimeter!\n";
						}
						if(t==1)
						{
							val++;
						}
					}
				}
				
			}
		}
	}
	
	if(val==userarr[x*width+y])
	{
		return 1;
	}
	else
	{
		return 0;
	}
	
}

int is_combination_valid(bool combination[], int perimeter[],int pc)
{
	int i=0,j=0, k=0;
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			if(userarr[i*width+j]>0)
			{
				k = check_comb(i, j, combination, perimeter, pc);
				if(k==0)
				{
					return 0;
				}
			}
		}
	}
	return 1;
}


void make_decision(const float parr[], const int perimeter[], const int pc)
{
	if(pc==0)
	{
		int x, y;
		srand((int) time(0));
		select_random_cell(x, y);
		cout<<"Randomly selected the cell ("<<x<<","<<y<<")\n";
		dig(x, y);
		return;
	}
	int i=0, j=0,val = 0;
	float minval = 2;
	for(i=0;i<pc;i++)
	{
		if(parr[i]==1)
		{
			flagcell(perimeter[i*2], perimeter[i*2+1]);
		}
		if(parr[i]==0)
		{
			add_to_safe(perimeter[i*2], perimeter[i*2+1]);
		}
		if(parr[i]<minval)
		{
			minval = parr[i];
		}	
	}
	cout<<"Minimum probability is: "<<minval<<endl;
	
	if(safe_cell_count>0)
	{
		for(i=0;i<length;i++)
		{
			for(j=0;j<width;j++)
			{
				if(safe_cell_arr[i*width+j]==1)
				{
					cout<<"\nSelected the cell ("<<i<<","<<j<<") with 0 probability of being a mine\n";
					dig(i, j);
					remove_from_safe(i, j);
				}
			}
		}
	}
	else
	{
		int val=0;
		for(i=0;i<pc;i++)
		{
			if(parr[i]==minval)
			{
				val++;
			}
			
		}
		int points_arr[val][2];
		int counter=0;
		for(i=0;i<pc;i++)
		{
			if(parr[i]==minval)
			{
				points_arr[counter][0] = perimeter[i*2];
				points_arr[counter][1] = perimeter[i*2+1];
				counter++;
			}
		}
		srand((int) time(0));
		i = rand()%counter;
		cout<<"\nSelected the cell ("<<points_arr[i][0]<<","<<points_arr[i][1]<<") with "<<minval<<" probability of being a mine\n";
		dig(points_arr[i][0], points_arr[i][1]);
	}
	
}

unsigned int nCr(int n, int k)
{
    unsigned int C[n + 1][k + 1];
    int i, j;
 
    // Caculate value of Binomial Coefficient
    // in bottom up manner
    for (i = 0; i <= n; i++) {
        for (j = 0; j <= min(i, k); j++) {
            // Base Cases
            if (j == 0 || j == i)
                C[i][j] = 1;
 
            // Calculate value using previously
            // stored values
            else
                C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
        }
    }
 
    return C[n][k];
}


unsigned int all_combs(int n, int r)
{
	int i=0;
	unsigned int val = 0;
	for(i=0;i<=r;i++)
	{
		val += nCr(n,i);
	}
	return val;
}


void perimeter_solve()
{
	prepdisplay();
	displayboard();
	int perimeter[length*width*2] = {0};
	int pc=0;
	int i=0,j=0,k=0;
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			if(userarr[i*width+j]>0)
			{
				get_neightboring_covered_cells(i, j, perimeter, pc);
			}
		}
	}
	cout<<"\nCovered cell count in perimeter: "<<pc<<endl;
	display();
	if(pc>23)
	{
		cout<<"\nPerimeter too big! Using 1st method to solve.\n";
		find_mine_prob();
		make_decision();
		return;
	}
	
	bool combination[pc]={0};
	float parr[pc] = {0};
	int valid_comb = 0;
	unsigned int combn = 0, cn=0;
	i = nbombs-nflags;
	if(i<=pc)
	{
		combn = all_combs(pc, i);
	}
	else
	{
		combn = all_combs(pc,pc);
	}
	cout<<"\nThe number of combinations are: "<<combn<<endl;
	
	int limit = min(nbombs-nflags, pc);
	for(k=0;k<=limit;k++)
	{
		for(i=0;i<pc;i++)
		{
			combination[i] = 0;
		}
		i=pc-1;
		j = 0;
		while(j<k)
		{
			combination[i] = 1;
			i--;
			j++;
		}
		do{
			if(is_combination_valid(combination, perimeter, pc)==1)
			{
				for(i=0;i<pc;i++)
				{
					parr[i] += combination[i];
				}
				
				valid_comb++;
				
				cout<<((float)(cn+1)/(float)combn)*100<<"% complete\n";
			}
			cn++;
			
		}while(next_permutation(combination, combination+pc));
		cn=0;
	}
	
	cout<<"The number of valid combinations are: "<<valid_comb<<endl;
	for(i=0;i<pc;i++)
	{
		parr[i] /= (float)valid_comb;
		cout<<parr[i]<<" ";
	}
	cout<<endl;
	//find_mine_prob();
	//make_decision();
	//return;
	make_decision(parr, perimeter, pc);
	
}

void microdisplay()
{
	cleardevice();
	int i=0,j=0;
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			if(userarr[i*width+j]==-2)
			{
				putpixel(i, j, WHITE);
			}
		}
	}
}


void benchmark(int ngames)
{
	int i=0, j=0, k=length*width, val=0, page=0, wins = 0;
	initdisplay();
	for(val=0;val<ngames;val++)
	{
		resetarrays();
		createfixedmines(val);
		assignvalues();
		reset_probabilityarr();
		page = 0;
		while(digc<k-nbombs)
		{
			prepdisplay();
			update_user_board();
			displayboard();
			display();
			perimeter_solve();
			if(safe == 0)
			{
				break;
			}
		}
		if(safe)
		{
			cout<<"\n\nYOU WON!\n\n";
			wins++;
		}
		else
		{
			cout<<"\n\nYOU LOSE!\n\n";
		}
		
	}
	
	prepdisplay();
	cleardevice();
	display();
	cout<<"The probability of winning is: "<<(float)wins/(float)ngames<<endl;
	
}


int dist(int x1, int y1, int x2, int y2)
{
	return pow(pow(x1-x2,2)+pow(y1-y2,2),0.5);
}


bool check_button(int c, int x, int y)
{
	return (buttonloc[c*4]<=x && buttonloc[c*4+1]<=y && buttonloc[c*4+2]>=x && buttonloc[c*4+3]>=y);
}

int process_button_press()
{
	int i=0, index=-1;
	for(i=0;i<buttoncount;i++)
	{
		if(check_button(i,cpos.x,cpos.y))
		{
			index = i;
		}
	}
	
	cout<<"\nButton: "<<index<<endl;
	switch(index)
	{
		case 0:
			perimeter_solve(); 
			break;
		case 1:
			closegraph();
			break;
			
	}
	
	return index;
}

int get_button_press()
{
	int bn=0;
	while(GetAsyncKeyState(VK_LBUTTON)==0 && GetAsyncKeyState(VK_RBUTTON)==0)
	{
		delay(100);
	}
	if(GetAsyncKeyState(VK_LBUTTON)<0)
	{
		bn = 0;
	}
	else
	{
		bn = 1;
	}
	while(GetAsyncKeyState(VK_LBUTTON)<0 || GetAsyncKeyState(VK_RBUTTON)<0)
	{
		delay(100);
	}
	GetCursorPos(&cpos);
	cpos.x += mouseoffx;
	cpos.y += mouseoffy;
	
	return bn;
}

void calibrate_mouse()
{
	outtextxy(0,0, (char*)"Click at the top left corner of the screen to calibrate: ");
	get_button_press();
	mouseoffx = -cpos.x;
	mouseoffy = -cpos.y;
	
}


void select_cell(int bn)
{
	int i=boardtx, r=-1, c=-1;
	while(i<=cpos.x)
	{
		i += cm;
		c++;
	}
	i=boardty;
	while(i<=cpos.y)
	{
		i+=cm;
		r++;
	}

	cout<<"\nSelected cell ("<<r<<","<<c<<") is ";
	if(bn==0)
	{
		if(flagarr[r*width+c]==1)
		{
			cout<<"flagged! Unflag the cell first to dig it.";
		}
		else
		{
			dig(r,c);
		}
	}
	else
	{
		if(flagarr[r*width+c]==1)
		{
			cout<<"unflagged.";
			unflagcell(r, c);
		}
		else
		{
			cout<<"flagged.";
			flagcell(r, c);
		}
	}
	
	cout<<endl;
	
	
}




int main()
{
	int i=0, j=0;
	i = get_parameters();
	initialise_arrays();
	resetarrays();
	if(i>0)
	{
		benchmark(i);
		closegraph();
		cout<<"Enter any value to exit: ";
		cin>>i;
		return 0;
	}
	int k = length*width, val=0, bn=0;
	int b1 = boardtx+width*cm, b2 = boardty+length*cm;
	createfixedmines(time(0));
	assignvalues();
	reset_probabilityarr();
	initdisplay();
	if(calibratemouseatstart==1)
	{
		calibrate_mouse();
	}
	while(digc<k-nbombs)
	{
		prepdisplay();
		update_user_board();
		displayboard();
		display();
		
		bn = get_button_press();
		if(cpos.x>=boardtx && cpos.y>=boardty && cpos.x<=b1 && cpos.y<=b2 && process_button_press()==-1)
		{
			select_cell(bn);
		}
		else
		{
			val = process_button_press();
			if(val==1)
			{
				return 0;
			}
		}
		
		if(safe == 0)
		{
			break;
		}
	}
	
	prepdisplay();
	update_user_board();
	displayboard();
	display();
	
	prepdisplay();
	
	if(safe)
	{
		outtextxy(0, min(boardty+cm*length,screeny-25), (char*)"You Won! Press any key to exit.");
		cout<<"\n\nYOU WON!\n\n";
	}
	else
	{
		outtextxy(0, min(boardty+cm*length,screeny-25), (char*)"You Lose! Press any key to exit.");
		cout<<"\n\nYOU LOSE!\n\n";
	}
	display();
	
	getch();
	closegraph();
}
