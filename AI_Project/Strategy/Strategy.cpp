#include <iostream>
#include <math.h>
#include <time.h>
#include <stdlib.h>//Sleep...
#include "Point.h"
#include "Strategy.h"
#include "Judge.h"//add...

//for print...
#include <conio.h>
#include <atlstr.h>

using namespace std;

int alltot = 0, allwin = 0, alllose = 0, alltie = 0;

const int MAXTREE = 9700000;
int nox, noy, rows, cols, index, findex;
int **nowBoard;
int *nowTop;
const double epsilon = 1e-6;
const double c = 1.0;
int allocConsole = 0;
time_t starttime;

Node nodes[MAXTREE];



void _cprintNode(int n){
	_cprintf("\n-----Node[%d]-----\n", n);
	_cprintf("mlchild = %d, mrchild = %d, father = %d\n", nodes[n].mlchild, nodes[n].mrchild, nodes[n].father);
	_cprintf("lastx = %d, lasty = %d\n", nodes[n].lastx, nodes[n].lasty);
	_cprintf("flag = %d\n", nodes[n].flag);
	_cprintf("user = %d\n", nodes[n].user);
	_cprintf("tot = %d, win = %d\n", nodes[n].tot, nodes[n].win);
	_cprintf("-----Node[%d]-----\n", n);
}

void _cprintBT(){
	_cprintf("\nnowboard:\n");
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++)
			_cprintf("%d ", nowBoard[i][j]);
		_cprintf("\n");
	}
	_cprintf("nowtop:\n");
	for(int j = 0; j < cols; j++){
		_cprintf("%d ", nowTop[j]);
	}
}

void printNode(int n){
	printf("\n-----Node[%d]-----\n", n);
	printf("mlchild = %d, mrchild = %d, father = %d\n", nodes[n].mlchild, nodes[n].mrchild, nodes[n].father);
	printf("lastx = %d, lasty = %d\n", nodes[n].lastx, nodes[n].lasty);
	printf("flag = %d\n", nodes[n].flag);
	printf("user = %d\n", nodes[n].user);
	printf("tot = %d, win = %d\n", nodes[n].tot, nodes[n].win);
	printf("-----Node[%d]-----\n", n);
}

void printBT(){
	printf("\nnowboard:\n");
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++)
			printf("%d ", nowBoard[i][j]);
		printf("\n");
	}
	printf("nowtop:\n");
	for(int j = 0; j < cols; j++){
		printf("%d ", nowTop[j]);
	}
}

void printLastTwo(){
	_cprintNode(0);
	for(int i = nodes[0].mlchild; i < nodes[0].mrchild; i++)
		_cprintNode(i);
}

bool must(int ni){
	//...
	int special = 0;
	nowBoard[nodes[ni].lastx][nodes[ni].lasty] = 2;
	if(machineWin(nodes[ni].lastx, nodes[ni].lasty, rows, cols, nowBoard)){
		special = 1;
	}
	
	nowBoard[nodes[ni].lastx][nodes[ni].lasty] = 1;
	if(userWin(nodes[ni].lastx, nodes[ni].lasty, rows, cols, nowBoard)){
		special = 1;
	}
	
	nowBoard[nodes[ni].lastx][nodes[ni].lasty] = 0;

	if(special) return true;
	return false;
}

double getvalue(int ni, int nf){
	if(!nodes[ni].tot || must(ni))
		return 100000.0;
	if(nodes[ni].user)
		return (double)nodes[ni].win / (double)nodes[ni].tot + c * (double)sqrt(2.0 * (double)log((double)nodes[nf].tot) / ((double)nodes[ni].tot));
	else
		return (1.0 - (double)nodes[ni].win / (double)nodes[ni].tot) + c * (double)sqrt(2.0 * (double)log((double)nodes[nf].tot) / ((double)nodes[ni].tot));
}

int bestchild(int nf){
	//printf("------bestchild start--------\nnodes[%d]\n", nf);
	double max = -1.0;
	int nmax = -1;
	for(int i = nodes[nf].mlchild; i < nodes[nf].mrchild; i++){
		double tmp = getvalue(i, nf);
		//printf("i = %d  :  win = %d, tot = %d  :  value = %lf\n", i, nodes[i].win, nodes[i].tot, tmp);
		//Sleep(2000);
		if(tmp > max){
			max = tmp;
			nmax = i;
		}
	}
	/*
	printf("------bestchild end--------\n");
	printf("++++++ expand best +++++++++\n ");
	printNode(nmax);
	*/
	return nmax;
}

int finalBestChild(int nf){
	//printf("------finalBestChild start--------\n");
	double max = -1.0;
	int nmax = -1;
	//printNode(nf);
	for(int i = nodes[nf].mlchild; i < nodes[nf].mrchild; i++){
		//Sleep(2000);
		double value = (double)nodes[i].win / (double)nodes[i].tot;
		//printf("i = %d  :  win = %d, tot = %d  :  value = %lf\n", i, nodes[i].win, nodes[i].tot, value);
		if(value > max){
			max = value;
			nmax = i;
		}
	}
	//printf("------finalBestChild end--------\n");
	return nmax;
}

int expand(int nf){
	//printf("------expand start--------\n");
	if(nodes[nf].flag != -1) return nf;
	nodes[nf].mlchild = index;
	nodes[nf].flag = 0;
	for(int i = 0; i < cols; i++){
		if(nowTop[i]){
			nodes[index].father = nf;
			nodes[index].user = !nodes[nf].user;
			nodes[index].lastx = nowTop[i] - 1;
			nodes[index].lasty = i;
			index++;
		}
	}
	nodes[nf].mrchild = index;
	//printf("------expand start--------\n");
	//int best = bestchild(nf);
	int best = nodes[nf].mlchild;
	return best;
}

void updateNowBoardTop(int nf){
	if(nodes[nf].lastx == nox + 1 && nodes[nf].lasty == noy)
		nowTop[nodes[nf].lasty]--;
	nowTop[nodes[nf].lasty]--;
	if(nodes[nf].user)
		nowBoard[nodes[nf].lastx][nodes[nf].lasty] = 2;
	else
		nowBoard[nodes[nf].lastx][nodes[nf].lasty] = 1;
	//printf("------** updateNowBoardTop **-------");
	//printBT();
	return;
}

int treePolicy(){
	//printf("------treePolicy start--------\n");
	while(nodes[findex].flag < 1){
		if(nodes[findex].flag == -1){
			//printf("treePolicy flag = -1\n");
			int nf = expand(findex);
			updateNowBoardTop(nf);
	//printf("------treePolicy end--------\n");
			return nf;
		}
		else{
			//nodes[findex].flag = 0...
			//printf("treePolicy flag = 0\n");
			findex = bestchild(findex);
			updateNowBoardTop(findex);
			if(nodes[findex].flag != 0)	return findex;
		}
	}
	//printf("------treePolicy end--------\n");
	return findex;
}

int defaultPolicy(int pi){
	//printf("------defaultPolicy start--------\n");
	int k = 0;
	//if(pi > 8500000) printf("###########\nlastx = %d, lasty = %d, rows = %d, cols = %d \n nowBT:\n", nodes[pi].lastx, nodes[pi].lasty, rows, cols);
	//if(pi > 8500000) printBT();
	//if(pi > 8500000) printf("###########\n");
	if(nodes[pi].user){
		bool userwin = machineWin(nodes[pi].lastx, nodes[pi].lasty, rows, cols, nowBoard);
		//printf("***me*** userwin = %d\n", userwin);
		if(userwin){k = 2;}
		else if(isTie(cols, nowTop)){k = 3;}
	}
	else{
		bool machinewin = userWin(nodes[pi].lastx, nodes[pi].lasty, rows, cols, nowBoard);
		//printf("***you*** machinewin = %d\n", machinewin);
		if(machinewin){k = 1;}
		else if(isTie(cols, nowTop)){k = 3;}
	}
	//k = 0...
	if(!k){
		pi = expand(pi);
		updateNowBoardTop(pi);
		//printNode(pi);
		//printBT();
		//Sleep(200);
		return defaultPolicy(pi);
	}
	//k != 0...
	nodes[pi].flag = k;
	//printf("------defaultPolicy end--------\n");
	return pi;
}

void retract(int pi){
	if(nodes[pi].lastx == nox + 1 && nodes[pi].lasty == noy)
		nowTop[nodes[pi].lasty]++;
	nowTop[nodes[pi].lasty]++;
	nowBoard[nodes[pi].lastx][nodes[pi].lasty] = 0;
}

void backup(int pi){
	//printf("------backup start--------\n");
	int k;
	alltot++;
	//printNode(pi);
	if(nodes[pi].flag == 2){
		k = 1;
		allwin++;
	}
	else if(nodes[pi].flag == 1){
		k = 0;
		alllose++;
	}
	else if(nodes[pi].flag == 3){
		k = 0;
		alltie++;
	}
	//if(pi < 0) _cprintf("!!!police!!! pi = %d\n", pi);
	while(pi >= 0){
		//printf("pi = %d ", pi);
		nodes[pi].win += k;
		nodes[pi].tot++;
		if(pi){
			retract(pi);
		}
		pi = nodes[pi].father;
	}
	pi++;
	/*
	printf("\nhahaha\n");
	printNode(pi);
	printf("------backup end--------\n");
	*/
}



/*
	���Ժ����ӿ�,�ú������Կ�ƽ̨����,ÿ�δ��뵱ǰ״̬,Ҫ�����������ӵ�,�����ӵ������һ��������Ϸ��������ӵ�,��Ȼ�Կ�ƽ̨��ֱ����Ϊ��ĳ�������
	
	input:
		Ϊ�˷�ֹ�ԶԿ�ƽ̨ά����������ɸ��ģ����д���Ĳ�����Ϊconst����
		M, N : ���̴�С M - ���� N - ���� ����0��ʼ�ƣ� ���Ͻ�Ϊ����ԭ�㣬����x��ǣ�����y���
		top : ��ǰ����ÿһ���ж���ʵ��λ��. e.g. ��i��Ϊ��,��_top[i] == M, ��i������,��_top[i] == 0
		_board : ���̵�һά�����ʾ, Ϊ�˷���ʹ�ã��ڸú����տ�ʼ���������Ѿ�����ת��Ϊ�˶�ά����board
				��ֻ��ֱ��ʹ��board���ɣ����Ͻ�Ϊ����ԭ�㣬�����[0][0]��ʼ��(����[1][1])
				board[x][y]��ʾ��x�С���y�еĵ�(��0��ʼ��)
				board[x][y] == 0/1/2 �ֱ��Ӧ(x,y)�� ������/���û�����/�г������,�������ӵ㴦��ֵҲΪ0
		lastX, lastY : �Է���һ�����ӵ�λ��, ����ܲ���Ҫ�ò�����Ҳ������Ҫ�Ĳ������ǶԷ�һ����
				����λ�ã���ʱ��������Լ��ĳ����м�¼�Է������ಽ������λ�ã�����ȫȡ�������Լ��Ĳ���
		noX, noY : �����ϵĲ������ӵ�(ע:��ʵ���������top�Ѿ����㴦���˲������ӵ㣬Ҳ����˵���ĳһ��
				������ӵ�����ǡ�ǲ������ӵ㣬��ôUI�����еĴ�����Ѿ������е�topֵ�ֽ�����һ�μ�һ������
				��������Ĵ�����Ҳ���Ը�����ʹ��noX��noY��������������ȫ��Ϊtop������ǵ�ǰÿ�еĶ�������,
				��Ȼ�������ʹ��lastX,lastY�������п��ܾ�Ҫͬʱ����noX��noY��)
		���ϲ���ʵ���ϰ����˵�ǰ״̬(M N _top _board)�Լ���ʷ��Ϣ(lastX lastY),��Ҫ���ľ�������Щ��Ϣ�¸������������ǵ����ӵ�
	output:
		������ӵ�Point
*/
extern "C" __declspec(dllexport) Point* getPoint(const int M, const int N, const int* top, const int* _board, 
	const int lastX, const int lastY, const int noX, const int noY){
	/*
		��Ҫ������δ���
	*/
	int x = -1, y = -1;//���ս�������ӵ�浽x,y��
	int** board = new int*[M];
	for(int i = 0; i < M; i++){
		board[i] = new int[N];
		for(int j = 0; j < N; j++){
			board[i][j] = _board[i * N + j];
		}
	}
	
	/*
		�������Լ��Ĳ������������ӵ�,Ҳ���Ǹ�����Ĳ�����ɶ�x,y�ĸ�ֵ
		�ò��ֶԲ���ʹ��û�����ƣ�Ϊ�˷���ʵ�֣�����Զ����Լ��µ��ࡢ.h�ļ���.cpp�ļ�
	*/
	//Add your own code below
	/*
     //a naive example
	for (int i = N-1; i >= 0; i--) {
		if (top[i] > 0) {
			x = top[i] - 1;
			y = i;
			break;
		}
	}
    */
	
	//special
	int special = 0;
	
	for(int i = 0; i < N; i++){
		if(!(top[i] - 1 == noX && i == noY)){
			board[top[i]-1][i] = 2;
			if(machineWin(top[i]-1, i, M, N, board)){
				x = top[i] - 1;
				y = i;
				special = 1;
				break;
			}
			board[top[i]-1][i] = 0;
		}
	}
	for(int i = 0; i < N; i++){
		if(!(top[i] - 1 == noX && i == noY)){
			board[top[i]-1][i] = 1;
			if(userWin(top[i]-1, i, M, N, board)){
				x = top[i] - 1;
				y = i;
				special = 1;
				break;
			}
			board[top[i]-1][i] = 0;
		}
	}
	//special end
	if(!special){

	if(!allocConsole){
		AllocConsole();//for print
		allocConsole++;
	}
	//freopen("out.txt", "w", stdout);//wenjian

	starttime = clock();
	for(int i = 0; i < MAXTREE; i++){
		nodes[i].unit();
	}
	nox = noX;
	noy = noY;
	rows = M;
	cols = N;
	index = 1;
	findex = 0;
	alltot = allwin = alllose = 0;

	nowBoard = board;
	nowTop = new int[N];
	for(int i = 0; i < N; i++)
		nowTop[i] = top[i];
	//_cprintBT();

	int pindex = 0;
	int cnt = 0;//
	while(clock() - starttime < 3000 && pindex < 9654000){
		//printf("...%d running...\n", cnt);
		cnt++;//
		findex = 0;
		pindex = treePolicy();
		//printf("---------1 treePolicy pindex = %d\n", pindex);
		//printf("*** 1 ***");
		pindex = defaultPolicy(pindex);
		//printf("---------2 defaultPolicy pindex = %d\n", pindex);
		//printf("*** 2 ***");
		backup(pindex);
		//printf("---------3 backup pindex = %d\n", pindex);
		//printf("*** 3 ***");
		//_cprintf("=========================================================\ntime = %d\n", clock() - starttime);
		//if(cnt % 10000 == 0)	printf("cnt = %d", cnt);
	}
	
	//printf("**********************************************\ntime = %d\n", clock() - starttime);
	int bestChild = finalBestChild(0);
	x = nodes[bestChild].lastx;
	y = nodes[bestChild].lasty;

	//printLastTwo();

	//printf("bestchild = %d\nx = %d\ny = %d\n", bestChild, x, y);
	
	//printf("alltot = %d  :  allwin = %d  :  alllose = %d  :  alltie = %d\n", alltot, allwin, alllose, alltie);
	}//special
	/*
		��Ҫ������δ���
	*/
	clearArray(M, N, board);
	
	return new Point(x, y);
}


/*
	getPoint�������ص�Pointָ�����ڱ�dllģ���������ģ�Ϊ��������Ѵ���Ӧ���ⲿ���ñ�dll�е�
	�������ͷſռ䣬����Ӧ�����ⲿֱ��delete
*/
extern "C" __declspec(dllexport) void clearPoint(Point* p){
	delete p;
	return;
}

/*
	���top��board����
*/
void clearArray(int M, int N, int** board){
	for(int i = 0; i < M; i++){
		delete[] board[i];
	}
	delete[] board;
}


/*
	������Լ��ĸ�������������������Լ����ࡢ����������µ�.h .cpp�ļ�������ʵ������뷨
*/
