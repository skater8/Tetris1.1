#include "Tetris.h"
#include <time.h>//unsure
#include <stdlib.h>
#include <iostream>
#include "Block.h"
#include <conio.h>
#include <mmsystem.h>
#include <fstream>//�ļ���д��
#include <graphics.h>
#pragma comment(lib,"winmm.lib") 


#define MAX_LEVEL 5
#define RECORDER_FILE "recorder.txt"

const int SPEED_NORMAL[MAX_LEVEL] = {500,400,300,200,100};//�ٶ�
const int SPEED_QUICK = 50;


Tetris::Tetris(int rows, int cols, int left, int top, int blockSize)//���캯��
{
	//��ͼ�� ����,���ϱ߽�,�����С ֻ���ʼ��һ�Σ����Է��ڹ��캯����
	this->rows = rows;
	this->cols = cols;
	leftMargin = left;
	topMargin = top;
	this->blockSize = blockSize;
	for (int i = 0;i < rows;i++) {
		vector<int> mapRow;//�������
		for (int j = 0;j < cols;j++ ) {
			mapRow.push_back(0);//���������������һ����
		}
		map.push_back(mapRow);//��һά����mapRow��һ�У��ӵ�map��ͼ��
	}
	

}

void Tetris::init()
{
	
	mciSendString("play res/bg1.mp3 repeat", 0, 0, 0);//��������

	delay = SPEED_NORMAL[0];
	srand(time(NULL));
	//������Ϸ����
	initgraph(938,896);//����д����

	//����ͼƬ
	loadimage(&imgBg, "res/bg2.png");//����
	loadimage(&imgWin, "res/win.png");//Ӯ
	loadimage(&imgOver, "res/over.png");//ʧ��
	loadimage(&imgPause, "res/pause.png",80,80);/*������ַ��ͼƬ��ַ  ��Ե�ַ��./����Ŀ¼�µ��ļ����з���   ͼƬչʾ������pngҲ������jpg ���Ŵ�С*/
	
	loadimage(&imgClear[0], "res/tx/0.png", 36 * 10,36);//������Чͼ
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


	//������һ��ʱ�����³�ʼ����Ϸ��������
	char data[20][10];
	for (int i = 0;i < rows;i++){
		for (int j = 0;j < cols;j++) {
			map[i][j] = 0;
		}
	}

	//��������־�������ĳ�ʼ����
	//��ͷ�ļ���ʼ���Ļ�û�취ÿ�䶼���³�ʼ��������Ҫд��init������
	score = 0;
	lineCount = 0;
	level = 1;
	

	//��ʼ����߷�
	ifstream file(RECORDER_FILE);//�ļ���
	if (!file.is_open()) {//û��
		cout << RECORDER_FILE << "��ʧ��" << endl;
		highestScore = 0;
	}
	else {
		file >> highestScore;//���ļ���������
	}
	file.close();//�ر��ļ�

	gameOver = false;
	clear = false;
}

void Tetris::play()
{
	init();

	//��������
	nextBlock = new Block();
	curBlock = nextBlock;
	nextBlock = new Block();

	int timer = 0;
	while (1) {
		 //�����û�������
		keyEvent();

		
		//ÿ��30ms��Ⱦһ�λ���
		timer += getDelay();
		if (timer > delay) {
			timer = 0;//������֤�´��ָ�30ms��Ⱦ
			drop();//�����½�
			// ��Ⱦ��Ϸ����
			update = true;
		}
		if (update) {//��update����ǿ�����keyEvnet�û�����ʱҲʵ����Ⱦ����
			//������Ϸ����
			update = false;
			updateWindow();
			//������Ϸ����
			//clearLine();
		}
		if (gameOver) {//��Ϸ����
			//�������
			saveScore();
			//������Ϸ��������
			displayOver();

			system("pause");//��ͣ

			init();//���¿�ʼ������һ��

		}//���Ҫ����ʱ�����·��鸲�Ǿɷ���Ͱ����if(gameOver���жϷ���if(update)ǰ
		
	}
}

void Tetris::startGame()
{

}

void Tetris::keyEvent()
{
	unsigned char ch;//�����û������ַ�,char���͵ķ�Χ��-128,127��224�����ⷶΧ��,���ڷ�Χ��0-255
	bool rotateFlag = false;
	int dx = 0;//�����ƶ�����xƫ����
	
	//�ж��������ް�������,û����жϵĻ���û��������ʱ�Ῠ����δ���
	if (_kbhit()) {
		//getchar();Ҫ�û����·�����󰴻س������������
		ch=_getch();//�û�����һ�����������ٰ��س�����������Ӧ

		//������·���ť�����Զ����������ַ�
		//������� ���Ϸ���������Ⱥ󷵻��磺224��72
		//������� ���·���������Ⱥ󷵻��磺224��80
		//������� ������������Ⱥ󷵻��磺224��75
		//������� ���ҷ���������Ⱥ󷵻��磺224��77
		//���·�����󣬻�����Ҫ���ϸ��£������ǵ�delayʱ�����£�
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
		if (ch == 32 )//�����ȡ���ַ�Ϊ�ո�ͬʱiΪ�����Ļ�����ô����ͣ
		{
			pause();//��ͣ��������ͣ���ֺͻ�����ͣͼƬ��ϵͳ����ͣ	
		}
		mciSendString("resume res/bg1.mp3", 0, 0, 0);//���¿�ʼ����
	}

	if (rotateFlag) {
		//����ת
		rotate();
		update = true;//���ϸ�����Ⱦ
	}
	if (dx != 0) {
		moveLeftRight(dx);
		update = true;//���ϸ�����Ⱦ
	}
}

void Tetris::updateWindow()
{
	
	//�������˸����Ϊ����ûһ��һ���Դ�ӡ��Ⱦ,����һǰһ����Ⱦ��

	/*���䵽�ײ��Ĺ̶�������Ⱦ*/
	IMAGE** imgs = Block::getImages();//imgs����������Block���imgs�����ÿ��Ԫ�أ���ЩԪ�ر�����ָ�����ͣ�


	//imgs��һ�����飬ÿ��Ԫ����һ������ָ��,����imgs������IMAGE**,
	//����imgs������7��Ԫ�� 
	BeginBatchDraw();//������ͼ,������˸,�� FlushBatchDraw �� EndBatchDraw ����ʹ��

	putimage(0, 0, &imgBg);//���Ʊ���ͼƬ

	for (int i = 0;i < rows;i++) {
		for (int j = 0;j < cols;j++) {
			if (map[i][j] == 0) continue;//��i�е�j�е�λ���ǿհ�λ�ã�˵��û����
			int x = j * blockSize + leftMargin;//�����ˮƽx���꣬�õڼ��м���õ�
			int y = i * blockSize + topMargin;//�����y����
			putimage(x, y,imgs[map[i][j]-1]);//x���꣬y���꣬�ĸ�������״�ķ���
			//�������imgs��updateWindow��ľֲ�����imgs,��ȡ������������
			//imgs�±��0��ʼ����map��ֵֻ��1-7������Ҫ��1
		}
	}

	//��ǰ������Ⱦ
	curBlock->draw(leftMargin, topMargin);//д���ˣ�43������ҵı�topMargin�ߵ����
	//��draw(leftMargin, topMargin)���leftMargin��Ӱ�������ƶ���topMargin��Ӱ���½�����ƶ�
	
	//Ԥ�淽����Ⱦ��д����
	nextBlock->draw(689,150);

	clearLine();//���У�����û�������棬���Ǻ�updateWindowһ�����play��
	
	drawScore();//���Ʒ���

	//������Ч
	if (clear) {
		for (int j = 0;j < 12;j++) {
			for (int i = 0;i < 20;i++) {
				if (returnLines[i] != 0) {
					//drawAlpha(&imgClear[j], 263, 133 + 36 * returnLines[i]);
					putimage(263, 133 + 36 * returnLines[i], &imgClear[j]);
				}
				returnLines[i] = 0;
			}
			FlushBatchDraw();//���ϼ���ͼƬ
			Sleep(50);//0.05s����һ֡ͼƬ
		}
		clear = false;//�ָ�
	}
	
	EndBatchDraw();

}
//��һ�ε��ã�����0��֮�󷵻ؾ����ϴε��ü���˶���ms
int Tetris::getDelay()
{
	static unsigned long long lastTime = 0;//��һ�� ,��̬����ֻ�ܳ�ʼ��һ��
	
	unsigned long long currentTime=GetTickCount();
	if (lastTime == 0) {//�ǵ�һ�ε���
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
	//���︴�ƶ���Ч�����ã����Զ���һ�����ƹ��캯��
	//�������������Ǿ������Զ������ƹ��캯��
	backBlock = *curBlock;//curBlock��ָ�룬ǰ��*��������ָ����﷨
	curBlock->drop();
	/*���ֿ����½����̻���Ҫ�Ĵ���*/  //int solidFlag = 0;
	/*�÷��鲻���½�����ͼ*/
	if (!curBlock->blockInMap(map)) {//�Ƿ�λ�ã�Ҫô1.����ǽ��2.�����ף�3.���������ѵ��÷����ϣ�
		//�ѷ���̻�
		backBlock.solidify(map);
		/*���ֿ����½����̻���Ҫ�Ĵ���*/  //solidFlag = 1;
		//��ֹ�ڴ�й©
		delete curBlock;
		//ȷ����һ�ֵķ�����״���ٸ���һ�ֵ�nextBlock
		curBlock = nextBlock;
		nextBlock = new Block;

		//�����Ϸ�Ƿ����
		checkOver();
		
	}
	/*�����¼�����һֱ��ס�ͻᱣ�ֿ����½�����غ���һ���·���ָ������ٶ�*/
	//if(solidFlag==1) delay = SPEED_NORMAL;
	delay = SPEED_NORMAL[level-1];//�½��ٶȻָ��ɵ�ǰ�ؿ��������ٶ�
}

void Tetris::clearLine()
{
	//returnLines[20] = 0;
	int n=0;
	int lines = 0;//�������˼���
	int k = rows - 1;//k�±꣺�洢���ݵ�����
	for (int i = rows - 1;i >= 0;i--) {//i�±꣺ɨ�����ݵ�����
		//����i���Ƿ�����
		int count = 0;
		for (int j = 0;j < cols;j++) {
			if (map[i][j]) {
				count++;
			}
			map[k][j] = map[i][j];//��ɨ��ߴ洢,����ʱkֵ�����������մ�Ĳ��������е�����
		}
		if (count < cols) {//û����,�洢
			k--;//�洢�±�--������
		}
		else {
			lines++;//����
			returnLines[n++]=i;
		}
	}
	if (lines > 0) {
		//����÷�
		int addScore[4] = { 10,30,60,80 };//�ӷֻ��ƣ�ͬʱ��1��10�֣�2��30��...
		score += addScore[lines - 1];
		clear = true;
		mciSendString("play res/xiaochu2.mp3", 0, 0, 0);
	
		update = true;

		//ÿ100��һ���ȼ� 0-100 ��һ�أ� 101-200�ڶ���
		level = (score + 99) / 100;//���е���ʱ����score=10,���score����0ʱ��Ϊlevel��ʼֵ=1
		//����д���߽�ֵ��100+99��/100=1��(101+99)/100�պ�=2

		if (level > MAX_LEVEL) {//��ʱ����>500��ͨ��
			gameOver = true;
		}

		lineCount += lines;//��������������
	}

}

void Tetris::moveLeftRight(int offset)
{

	backBlock = *curBlock;
	curBlock->moveLeftRight(offset);//��������������ƶ�
	//�����ƶ����ܻ��Ƴ��߽�
	if (!curBlock->blockInMap(map)) {//�Ƿ�λ��
		*curBlock=backBlock;//��ԭ����һ�εĺϷ�λ��
	}
}

void Tetris::rotate()
{
	if (curBlock->getBlockType() == 7) return;
	//��������������ξ�ֱ��return;������ת����������ô��ת����һ��

	backBlock = *curBlock;
	curBlock->rotate();

	if (!curBlock->blockInMap(map)) {//�Ƿ�λ��
		*curBlock = backBlock;//��ԭ����һ�εĺϷ�λ��
	}
}

void Tetris::drawScore()
{
	char scoreText[32];//�������飬������������������ؿ���
	
	//��ѧ����������������
	//����
	sprintf_s(scoreText, sizeof(scoreText), "%d", score);
	setcolor(RGB(180, 180, 180));
	//��������ϸ��
	LOGFONT f;
	gettextstyle(&f);//��ȡ��ǰ����
	f.lfHeight = 60;//�߶�
	f.lfWidth = 30;//���
	f.lfQuality = ANTIALIASED_QUALITY;//��������Ϊ"�����"Ч���������Ե������������
	
	//ǿת��ǰ����
	strcpy_s(f.lfFaceName, sizeof(f.lfFaceName), _T("Segoe UI Black"));
	settextstyle(&f);//��ɵ�ǰ����
	setbkmode(TRANSPARENT);//����ı�����Ϊ͸��
	//����������ʾ��д����
	outtextxy(670, 727, scoreText);//x,y��ʼλ��


	//��������
	sprintf_s(scoreText, sizeof(scoreText), "%d", lineCount);
	gettextstyle(&f);//��ȡ���壬Ϊ������ȡ����Ŀ��
	int xPos = 224 - f.lfWidth * strlen(scoreText);//д����
	outtextxy(xPos,817, scoreText);//д����


	//�ؿ�
	sprintf_s(scoreText, sizeof(scoreText), "%d",level);
	outtextxy(224-30, 727, scoreText);//д����

	//��߷�
	sprintf_s(scoreText, sizeof(scoreText), "%d", highestScore);
	outtextxy(670, 817, scoreText);//д����


}

void Tetris::checkOver()
{
	gameOver=(curBlock->blockInMap(map) == false);//���ںϷ�λ��->��Ϸ����
}

void Tetris::saveScore()
{
	if (score > highestScore) {
		highestScore = score;
		ofstream file(RECORDER_FILE);//ifstream--ofstream��д
		file << highestScore;//д��
		file.close();
	}
}

void Tetris::displayOver()
{

	mciSendString("stop res/bg.mp3", 0, 0, 0);
	
	//ʤ����������ʧ�ܽ���
	if (level <= MAX_LEVEL) {
		putimage(262, 361, &imgOver);//д����
		mciSendString("play res/over.mp3", 0, 0, 0);
	}
	else {
		putimage(262, 361, &imgWin);//д����
		mciSendString("play res/win.mp3", 0, 0, 0);
	}
}

void Tetris::pause()
{
	mciSendString("pause res/bg1.mp3", 0, 0, 0);
	putimage(leftMargin+4*36, 361, &imgPause);//д����
	system("pause");
}