#pragma once
#include <graphics.h> //需要先安装easyx图形库！！
#include <vector>
using namespace std;

struct Point {//定义一个结构体数组，表示出方块下落的位置在哪里，第几行第几列，写结构体是为了让开发更简单
    int row;
    int col;
};

class Block
{
    //写出几个对外的接口分别是：下降，左右移动，旋转，画图。
public:
    Block();//无参构造函数，默认构造函数，并且只能有一个默认构造函数
    void drop();
    void moveLeftRight(int offset);//左右移
    void rotate();//旋转，视频就这么写！！
    void draw(int leftMargin, int topMargin);
    static IMAGE** getImages();//get取 Block里的私有imgs[7]那个数组
    Block& operator=(const Block& other);//复制构造函数
    bool blockInMap(const vector<vector<int>>&map);//写引用&开销比较小
    void solidify(vector<vector<int>>& map);//固化
    int getBlockType();//放回方块类型

private:
    int blockType;//定义一下方块的类型
    Point smallBlocks[4];//最开始的四个小方块(每种类型的方块都由四个小方块组成)的位置
    IMAGE* img;//因为很多图片都是一样的，所以搞一个指针可以优化代码

    static IMAGE* imgs[7]; //整个方块类共有的图片数组
    static int size;//图片大小，小方块大小

};
