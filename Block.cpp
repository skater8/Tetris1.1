#include "Block.h"
#include <stdlib.h>
//#include <bits/stdc++.h>����Ϊʲô����������ͷ�ļ�

IMAGE* Block::imgs[7] = { NULL, };//��block�������imgsͼƬ���鶼����Ϊ��
int Block::size = 36;

Block::Block()
{
    if (imgs[0] == NULL) {
        //����һ����ʼ�����ô�ͼƬ��Ϊ�˽�ʡ�ռ䣬���ô���ȥ�и�����Ż�
        IMAGE imgTmp;
        loadimage(&imgTmp, "res/tiles.png");

        SetWorkingImage(&imgTmp);//�и�
        for (int i = 0; i < 7; i++) {
            imgs[i] = new IMAGE;
            getimage(imgs[i], i * size, 0, size, size);//�и������꣬������0��Ȼ�󳤺Ϳ���size
        }
        SetWorkingImage();//�ָ�������
    }

    //1���ȰѶ���˹���������д����
    int blocks[7][4] = { //��һ����ά����Ϳ��԰��������������̬��ʾ����
    1,3,5,7, // I
    2,4,5,7, // Z 1
    3,5,4,6, // Z 2
    3,5,4,7, // T
    2,3,5,7, // L
    3,5,7,6, // J
    2,3,4,5, // ��
    };

    //2��������Ҫ�������һ�ֶ���˹����
    blockType = 1 + rand() % 7;// ��Ϊһ�����ַ��飬���Զ�7ȡ����;+1ֻ����Ϊ�������1~7���Ÿ�˳��һ��

    // ��ʼ�� smallBlocks
    for (int i = 0; i < 4; i++) {
        int value = blocks[blockType - 1][i];
        smallBlocks[i].row = value / 2;
        smallBlocks[i].col = value % 2;
    }

    img = imgs[blockType - 1];
}

void Block::drop()
{
    //��ÿ�����͵ķ�����ĸ�С������б�row,ʵ���½�
    for (int i = 0;i < 4;i++) {
        smallBlocks[i].row++;
    }
    //c++ 11д��
    /*
    for (auto& block : smallBlocks) {
        block.row++;
    }
    */
}

void Block::moveLeftRight(int offset)
{
    for (int i = 0;i < 4;i++) {
        smallBlocks[i].col += offset;
    }
}

void Block::rotate()
{
    Point p = smallBlocks[1];//Point��ת����
    for (int i = 0;i < 4;i++) {
        Point tmp = smallBlocks[i];//ÿ��С����ԭ����λ�ã����ݣ�������õ�
        // �µ�λ�ã���ѧ��ʽ����ֱ��������
        smallBlocks[i].col = p.col - tmp.row + p.row;
        smallBlocks[i].row = p.row + tmp.col - p.col;
    }
}

void Block::draw(int leftMargin, int topMargin)
{
    for (int i = 0; i < 4; i++) {
        int x = leftMargin + smallBlocks[i].col * size;//x����
        int y = topMargin+ smallBlocks[i].row * size;//y����
        putimage(x, y, img);//ֱ�ӻ���ͼ��
    }
}

IMAGE** Block::getImages()
{
    return imgs;
}

Block& Block::operator=(const Block& other)
{
    if (this == &other) return *this;
    //��ֹ�Լ������Լ�������һ��������ڵ�ǰ����
    
    /*�������*/
    //���Ʒ�������
    this->blockType = other.blockType;
    //����smallBlocks����
    for (int i = 0;i < 4;i++) {
        this->smallBlocks[i] = other.smallBlocks[i];
    }
    return *this;
}

bool Block::blockInMap(const vector<vector<int>>& map)
{
    int rows = map.size();
    int cols = map[0].size();
    for (int i = 0;i < 4;i++) {//���α����ĸ�С����λ��
        if (smallBlocks[i].col < 0 || smallBlocks[i].col >= cols ||
            smallBlocks[i].row < 0 || smallBlocks[i].row >= rows ||
            map[smallBlocks[i].row][smallBlocks[i].col] != 0)
            //����ڵ�ͼ��,��ǰ�ĸ��ж�������
            //���߸��ǵ��ѽ���ķ�����,�����һ���ж�����
            //��map��Ϊ0��û���鲻Ϊ0�����з��飩
            return false;//ֱ��false���ж�ΪΥ��λ��
    }
    return true;//�ĸ�С�����λ�ö��Ϸ�����ô�����ںϷ�λ��
}

void Block::solidify(vector<vector<int>>& map)
{
    for (int i = 0;i < 4;i++) {
        //���ñ�ǣ��̻���Ӧ��λ��
        map[smallBlocks[i].row][smallBlocks[i].col] = blockType;

    }
}

int Block::getBlockType()
{
    return blockType;
}
