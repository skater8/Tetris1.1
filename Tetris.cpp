#include "Tetris.h"
#include <time.h>//unsure
#include <stdlib.h>
#include <iostream>
#include "Block.h"
#include <conio.h>
#include <mmsystem.h>
#include <fstream>//文件读写的
#include <graphics.h>
#pragma comment(lib,"winmm.lib") 


#define MAX_LEVEL 5
#define RECORDER_FILE "recorder.txt"

const int SPEED_NORMAL[MAX_LEVEL] = {500,400,300,200,100};//速度
const int SPEED_QUICK = 50;


Tetris::Tetris(int rows, int cols, int left, int top, int blockSize)//构造函数
{
	//地图的 行列,左、上边界,方块大小 只需初始化一次，所以放在构造函数里
	this->rows = rows;
	this->cols = cols;
	leftMargin = left;
	topMargin = top;
	this->blockSize = blockSize;
	for (int i = 0;i < rows;i++) {
		vector<int> mapRow;//定义空行
		for (int j = 0;j < cols;j++ ) {
			mapRow.push_back(0);//在数字最后面增加一个零
		}
		map.push_back(mapRow);//把一维数组mapRow（一行）加到map地图里
	}
	

}

void Tetris::init()
{
	
	mciSendString("play res/bg1.mp3 repeat", 0, 0, 0);//背景音乐

	delay = SPEED_NORMAL[0];
	srand(time(NULL));
	//创建游戏窗口
	initgraph(938,896);//这里写死了

	//加载图片
	loadimage(&imgBg, "res/bg2.png");//背景
	loadimage(&imgWin, "res/win.png");//赢
	loadimage(&imgOver, "res/over.png");//失败
	loadimage(&imgPause, "res/pause.png",80,80);/*变量地址，图片地址  相对地址“./”本目录下的文件进行访问   图片展示可以是png也可以是jpg 缩放大小*/
	
	loadimage(&imgClear[0], "res/tx/0.png", 36 * 10,36);//消行特效图
	loadimage(&imgClear[1], "res/tx/1.png", 36 * 10,36);
	loadimage(&imgClear[2], "res/tx/2.png", 36 * 10,36);
	loadimage(&imgClear[3], "res/tx/3.png", 36 * 10,36);
	loadimage(&imgClear[4], "res/tx/4.png", 36 * 10,36);
	loadimage(&imgClear[5], "res/tx/5.png", 36 * 10,36);
	loadimage(&imgClear[6], "res/tx/6.png", 36 * 10,36);
	loadimage(&imgClear[7], "res/tx/7.png", 36 * 10,36);
	loadimage(&imgClear[8], "res/tx/8.png", 36 * 10,36);
	loadimage(&imgClear[9], "res/tx/9.png", 36 * 10,36);
	loadimage(&imgClear[10], "res/tx/10.png", 36 * 10,36);
	loadimage(&imgClear[10], "res/tx/11.png", 36 * 10,36);


	//当玩完一局时，重新初始化游戏区的数据
	char data[20][10];
	for (int i = 0;i < rows;i++){
		for (int j = 0;j < cols;j++) {
			map[i][j] = 0;
		}
	}

	//变量（标志变量）的初始化，
	//在头文件初始化的话没办法每句都重新初始化，所以要写在init函数里
	score = 0;
	lineCount = 0;
	level = 1;
	

	//初始化最高分
	ifstream file(RECORDER_FILE);//文件名
	if (!file.is_open()) {//没打开
		cout << RECORDER_FILE << "打开失败" << endl;
		highestScore = 0;
	}
	else {
		file >> highestScore;//从文件输入数据
	}
	file.close();//关闭文件

	gameOver = false;
	clear = false;
}

void Tetris::play()
{
	init();

	//创建方块
	nextBlock = new Block();
	curBlock = nextBlock;
	nextBlock = new Block();

	int timer = 0;
	while (1) {
		 //接受用户的输入
		keyEvent();

		
		//每隔30ms渲染一次画面
		timer += getDelay();
		if (timer > delay) {
			timer = 0;//清理，保证下次又隔30ms渲染
			drop();//方块下降
			// 渲染游戏画面
			update = true;
		}
		if (update) {//用update作标记可以在keyEvnet用户输入时也实现渲染更新
			//更新游戏画面
			update = false;
			updateWindow();
			//更新游戏数据
			//clearLine();
		}
		if (gameOver) {//游戏结束
			//保存分数
			saveScore();
			//更新游戏结束界面
			displayOver();

			system("pause");//暂停

			init();//重新开始，再来一局

		}//如果要结束时不会新方块覆盖旧方块就把这个if(gameOver的判断放在if(update)前
		
	}
}

void Tetris::startGame()
{

}

void Tetris::keyEvent()
{
	unsigned char ch;//接收用户输入字符,char类型的范围是-128,127，224不在这范围里,现在范围是0-255
	bool rotateFlag = false;
	int dx = 0;//左右移动——x偏移量
	
	//判断现在有无按键输入,没这个判断的话当没按键输入时会卡在这段代码
	if (_kbhit()) {
		//getchar();要用户按下方向键后按回车才能输入完成
		ch=_getch();//用户按下一个按键后不用再按回车就能做出反应

		//如果按下方向按钮，会自动返回两个字符
		//如果按下 向上方向键，会先后返回如：224，72
		//如果按下 向下方向键，会先后返回如：224，80
		//如果按下 向左方向键，会先后返回如：224，75
		//如果按下 向右方向键，会先后返回如：224，77
		//按下方向键后，画面需要马上更新（而不是等delay时间后更新）
		if (ch == 224) {
			ch = _getch();
			switch (ch) {
			case 72:
				rotateFlag = true; 
				break;
			case 80:
				delay = SPEED_QUICK; 
				//update = true;
				break;
			case 75:
				dx = -1; 
				break;
			case 77:
				dx = 1; 
				break;
			default: 
				break;
			}
		}
		if (ch == 32 )//如果获取的字符为空格，同时i为奇数的话，那么就暂停
		{
			pause();//暂停，包括暂停音乐和绘制暂停图片和系统的暂停	
		}
		mciSendString("resume res/bg1.mp3", 0, 0, 0);//重新开始音乐
	}

	if (rotateFlag) {
		//做旋转
		rotate();
		update = true;//马上更新渲染
	}
	if (dx != 0) {
		moveLeftRight(dx);
		update = true;//马上更新渲染
	}
}

void Tetris::updateWindow()
{
	
	//方块会闪烁是因为方块没一起一次性打印渲染,而是一前一后渲染的

	/*降落到底部的固定方块渲染*/
	IMAGE** imgs = Block::getImages();//imgs数组里存的是Block类的imgs数组的每个元素（这些元素本身是指针类型）


	//imgs是一个数组，每个元素是一个数组指针,所以imgs类型是IMAGE**,
	//所以imgs里存的是7种元素 
	BeginBatchDraw();//批量绘图,消除闪烁,和 FlushBatchDraw 或 EndBatchDraw 配套使用

	putimage(0, 0, &imgBg);//绘制背景图片

	for (int i = 0;i < rows;i++) {
		for (int j = 0;j < cols;j++) {
			if (map[i][j] == 0) continue;//第i行第j列的位置是空白位置，说明没方块
			int x = j * blockSize + leftMargin;//方块的水平x坐标，用第几列计算得到
			int y = i * blockSize + topMargin;//方块的y坐标
			putimage(x, y,imgs[map[i][j]-1]);//x坐标，y坐标，哪个类型形状的方块
			//函数里的imgs是updateWindow里的局部变量imgs,是取到的类型数组
			//imgs下标从0开始，而map的值只有1-7，所以要减1
		}
	}

	//当前方块渲染
	curBlock->draw(leftMargin, topMargin);//写死了，43是随便找的比topMargin高的落点
	//改draw(leftMargin, topMargin)里的leftMargin会影响左右移动，topMargin会影响下降后的移动
	
	//预告方块渲染，写死了
	nextBlock->draw(689,150);

	clearLine();//消行，本来没放这里面，而是和updateWindow一起放在play里
	
	drawScore();//绘制分数

	//消除特效
	if (clear) {
		for (int j = 0;j < 12;j++) {
			for (int i = 0;i < 20;i++) {
				if (returnLines[i] != 0) {
					//drawAlpha(&imgClear[j], 263, 133 + 36 * returnLines[i]);
					putimage(263, 133 + 36 * returnLines[i], &imgClear[j]);
				}
				returnLines[i] = 0;
			}
			FlushBatchDraw();//马上加载图片
			Sleep(50);//0.05s加载一帧图片
		}
		clear = false;//恢复
	}
	
	EndBatchDraw();

}
//第一次调用，返回0；之后返回局里上次调用间隔了多少ms
int Tetris::getDelay()
{
	static unsigned long long lastTime = 0;//第一次 ,静态变量只能初始化一次
	
	unsigned long long currentTime=GetTickCount();
	if (lastTime == 0) {//是第一次调用
		lastTime = currentTime;
		return 0;
	}
	else{
		int ret=currentTime - lastTime;
		lastTime = currentTime;
		return ret;
	}
}
void Tetris::drop()
{
	//这里复制对象效果不好，可以定义一个复制构造函数
	//定义完后下面的那句语句会自动调复制构造函数
	backBlock = *curBlock;//curBlock是指针，前加*解引用是指针的语法
	curBlock->drop();
	/*保持快速下降至固化需要的代码*/  //int solidFlag = 0;
	/*让方块不会下降出地图*/
	if (!curBlock->blockInMap(map)) {//非法位置（要么1.碰到墙，2.碰到底，3.碰到其他已调用方块上）
		//把方块固化
		backBlock.solidify(map);
		/*保持快速下降至固化需要的代码*/  //solidFlag = 1;
		//防止内存泄漏
		delete curBlock;
		//确定下一轮的方块形状，再改下一轮的nextBlock
		curBlock = nextBlock;
		nextBlock = new Block;

		//检查游戏是否结束
		checkOver();
		
	}
	/*按向下键后不用一直按住就会保持快速下降至落地后，下一个新方块恢复正常速度*/
	//if(solidFlag==1) delay = SPEED_NORMAL;
	delay = SPEED_NORMAL[level-1];//下降速度恢复成当前关卡的正常速度
}

void Tetris::clearLine()
{
	//returnLines[20] = 0;
	int n=0;
	int lines = 0;//计算消了几行
	int k = rows - 1;//k下标：存储数据的行数
	for (int i = rows - 1;i >= 0;i--) {//i下标：扫描数据的行数
		//检查第i行是否满行
		int count = 0;
		for (int j = 0;j < cols;j++) {
			if (map[i][j]) {
				count++;
			}
			map[k][j] = map[i][j];//边扫描边存储,满行时k值不变所以最终存的不会是满行的数据
		}
		if (count < cols) {//没满行,存储
			k--;//存储下标--往上走
		}
		else {
			lines++;//满行
			returnLines[n++]=i;
		}
	}
	if (lines > 0) {
		//计算得分
		int addScore[4] = { 10,30,60,80 };//加分机制，同时消1行10分，2行30分...
		score += addScore[lines - 1];
		clear = true;
		mciSendString("play res/xiaochu2.mp3", 0, 0, 0);
	
		update = true;

		//每100分一个等级 0-100 第一关， 101-200第二关
		level = (score + 99) / 100;//运行到这时至少score=10,因此score等于0时仍为level初始值=1
		//这样写，边界值（100+99）/100=1，(101+99)/100刚好=2

		if (level > MAX_LEVEL) {//这时分数>500，通关
			gameOver = true;
		}

		lineCount += lines;//更新总消除行数
	}

}

void Tetris::moveLeftRight(int offset)
{

	backBlock = *curBlock;
	curBlock->moveLeftRight(offset);//调方块类的左右移动
	//左右移动可能会移出边界
	if (!curBlock->blockInMap(map)) {//非法位置
		*curBlock=backBlock;//还原到上一次的合法位置
	}
}

void Tetris::rotate()
{
	if (curBlock->getBlockType() == 7) return;
	//如果类型是田字形就直接return;不用旋转，田字形怎么旋转都长一样

	backBlock = *curBlock;
	curBlock->rotate();

	if (!curBlock->blockInMap(map)) {//非法位置
		*curBlock = backBlock;//还原到上一次的合法位置
	}
}

void Tetris::drawScore()
{
	char scoreText[32];//数字数组，公用与分数，行数，关卡等
	
	//需学！！！！！！！！
	//分数
	sprintf_s(scoreText, sizeof(scoreText), "%d", score);
	setcolor(RGB(180, 180, 180));
	//设置字体细节
	LOGFONT f;
	gettextstyle(&f);//获取当前字体
	f.lfHeight = 60;//高度
	f.lfWidth = 30;//宽度
	f.lfQuality = ANTIALIASED_QUALITY;//设置字体为"抗锯齿"效果，不会边缘挡道其他东西
	
	//强转当前字体
	strcpy_s(f.lfFaceName, sizeof(f.lfFaceName), _T("Segoe UI Black"));
	settextstyle(&f);//设成当前字体
	setbkmode(TRANSPARENT);//字体的背景设为透明
	//分数绘制显示，写死了
	outtextxy(670, 727, scoreText);//x,y起始位置


	//消灭行数
	sprintf_s(scoreText, sizeof(scoreText), "%d", lineCount);
	gettextstyle(&f);//获取字体，为了下行取字体的宽度
	int xPos = 224 - f.lfWidth * strlen(scoreText);//写死了
	outtextxy(xPos,817, scoreText);//写死了


	//关卡
	sprintf_s(scoreText, sizeof(scoreText), "%d",level);
	outtextxy(224-30, 727, scoreText);//写死了

	//最高分
	sprintf_s(scoreText, sizeof(scoreText), "%d", highestScore);
	outtextxy(670, 817, scoreText);//写死了


}

void Tetris::checkOver()
{
	gameOver=(curBlock->blockInMap(map) == false);//不在合法位置->游戏结束
}

void Tetris::saveScore()
{
	if (score > highestScore) {
		highestScore = score;
		ofstream file(RECORDER_FILE);//ifstream--ofstream读写
		file << highestScore;//写入
		file.close();
	}
}

void Tetris::displayOver()
{

	mciSendString("stop res/bg.mp3", 0, 0, 0);
	
	//胜利结束还是失败结束
	if (level <= MAX_LEVEL) {
		putimage(262, 361, &imgOver);//写死了
		mciSendString("play res/over.mp3", 0, 0, 0);
	}
	else {
		putimage(262, 361, &imgWin);//写死了
		mciSendString("play res/win.mp3", 0, 0, 0);
	}
}

void Tetris::pause()
{
	mciSendString("pause res/bg1.mp3", 0, 0, 0);
	putimage(leftMargin+4*36, 361, &imgPause);//写死了
	system("pause");
}