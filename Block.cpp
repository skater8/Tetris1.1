#include "Block.h"
#include <stdlib.h>
//#include <bits/stdc++.h>啊？为什么不能用万能头文件

IMAGE* Block::imgs[7] = { NULL, };//把block类里面的imgs图片数组都设置为空
int Block::size = 36;

Block::Block()
{
    if (imgs[0] == NULL) {
        //进行一个初始化，用大图片是为了节省空间，利用代码去切割进行优化
        IMAGE imgTmp;
        loadimage(&imgTmp, "res/tiles.png");

        SetWorkingImage(&imgTmp);//切割
        for (int i = 0; i < 7; i++) {
            imgs[i] = new IMAGE;
            getimage(imgs[i], i * size, 0, size, size);//切割点的坐标，纵向都是0，然后长和宽都是size
        }
        SetWorkingImage();//恢复工作区
    }

    //1、先把俄罗斯方块的样子写出来
    int blocks[7][4] = { //用一个二维数组就可以把最基本的七种形态表示出来
    1,3,5,7, // I
    2,4,5,7, // Z 1
    3,5,4,6, // Z 2
    3,5,4,7, // T
    2,3,5,7, // L
    3,5,7,6, // J
    2,3,4,5, // 田
    };

    //2、接下来要随机生成一种俄罗斯方块
    blockType = 1 + rand() % 7;// 因为一共七种方块，所以对7取余数;+1只是因为让他变成1~7看着更顺眼一点

    // 初始化 smallBlocks
    for (int i = 0; i < 4; i++) {
        int value = blocks[blockType - 1][i];
        smallBlocks[i].row = value / 2;
        smallBlocks[i].col = value % 2;
    }

    img = imgs[blockType - 1];
}

void Block::drop()
{
    //把每种类型的方块的四个小方块改行标row,实现下降
    for (int i = 0;i < 4;i++) {
        smallBlocks[i].row++;
    }
    //c++ 11写法
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
    Point p = smallBlocks[1];//Point旋转中心
    for (int i = 0;i < 4;i++) {
        Point tmp = smallBlocks[i];//每个小方块原来的位置，备份，计算会用到
        // 新的位置，数学公式结论直接拿来用
        smallBlocks[i].col = p.col - tmp.row + p.row;
        smallBlocks[i].row = p.row + tmp.col - p.col;
    }
}

void Block::draw(int leftMargin, int topMargin)
{
    for (int i = 0; i < 4; i++) {
        int x = leftMargin + smallBlocks[i].col * size;//x坐标
        int y = topMargin+ smallBlocks[i].row * size;//y坐标
        putimage(x, y, img);//直接绘制图像
    }
}

IMAGE** Block::getImages()
{
    return imgs;
}

Block& Block::operator=(const Block& other)
{
    if (this == &other) return *this;
    //防止自己复制自己，当另一个对象等于当前对象
    
    /*其他情况*/
    //复制方块类型
    this->blockType = other.blockType;
    //复制smallBlocks数组
    for (int i = 0;i < 4;i++) {
        this->smallBlocks[i] = other.smallBlocks[i];
    }
    return *this;
}

bool Block::blockInMap(const vector<vector<int>>& map)
{
    int rows = map.size();
    int cols = map[0].size();
    for (int i = 0;i < 4;i++) {//依次遍历四个小方块位置
        if (smallBlocks[i].col < 0 || smallBlocks[i].col >= cols ||
            smallBlocks[i].row < 0 || smallBlocks[i].row >= rows ||
            map[smallBlocks[i].row][smallBlocks[i].col] != 0)
            //如果在地图外,即前四个判断条件，
            //或者覆盖到已降落的方块上,即最后一个判断条件
            //（map上为0是没方块不为0是已有方块）
            return false;//直接false，判断为违法位置
    }
    return true;//四个小方块的位置都合法，那么方块在合法位置
}

void Block::solidify(vector<vector<int>>& map)
{
    for (int i = 0;i < 4;i++) {
        //设置标记，固化对应的位置
        map[smallBlocks[i].row][smallBlocks[i].col] = blockType;

    }
}

int Block::getBlockType()
{
    return blockType;
}
