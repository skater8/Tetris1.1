#pragma once
#include <graphics.h> //��Ҫ�Ȱ�װeasyxͼ�ο⣡��
#include <vector>
using namespace std;

struct Point {//����һ���ṹ�����飬��ʾ�����������λ��������ڼ��еڼ��У�д�ṹ����Ϊ���ÿ�������
    int row;
    int col;
};

class Block
{
    //д����������Ľӿڷֱ��ǣ��½��������ƶ�����ת����ͼ��
public:
    Block();//�޲ι��캯����Ĭ�Ϲ��캯��������ֻ����һ��Ĭ�Ϲ��캯��
    void drop();
    void moveLeftRight(int offset);//������
    void rotate();//��ת����Ƶ����ôд����
    void draw(int leftMargin, int topMargin);
    static IMAGE** getImages();//getȡ Block���˽��imgs[7]�Ǹ�����
    Block& operator=(const Block& other);//���ƹ��캯��
    bool blockInMap(const vector<vector<int>>&map);//д����&�����Ƚ�С
    void solidify(vector<vector<int>>& map);//�̻�
    int getBlockType();//�Żط�������

private:
    int blockType;//����һ�·��������
    Point smallBlocks[4];//�ʼ���ĸ�С����(ÿ�����͵ķ��鶼���ĸ�С�������)��λ��
    IMAGE* img;//��Ϊ�ܶ�ͼƬ����һ���ģ����Ը�һ��ָ������Ż�����

    static IMAGE* imgs[7]; //���������๲�е�ͼƬ����
    static int size;//ͼƬ��С��С�����С

};
