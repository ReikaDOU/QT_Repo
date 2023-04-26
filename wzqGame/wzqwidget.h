#ifndef WZQWIDGET_H
#define WZQWIDGET_H

#include <QWidget>
#include"config.h"
QT_BEGIN_NAMESPACE
namespace Ui { class wzqWidget; }
QT_END_NAMESPACE

class wzqWidget : public QWidget
{
    Q_OBJECT

public:
    wzqWidget(QWidget *parent = nullptr);
    ~wzqWidget();
    //重绘事件，棋盘，棋子
    void paintEvent(QPaintEvent* event);
    //松开落子
    void mouseReleaseEvent(QMouseEvent* event);
    //获取当前回合
    int getBlackOrWhite() const;
    //切换回合
    void changeBlackOrWhite();
    //判断出界
    bool isOverLine(int x,int y);
    //判断输赢
    bool isWin(int x,int y);
    //结束清空
    void clear();
signals:
    //落子信号，谁的回合，坐标
    void SIG_pieceDown(int color,int x,int y);
public slots:
    //落子槽，谁的回合，坐标
    void slot_pieceDown(int color, int x,int y);
    void slot_startGame();
private:
    Ui::wzqWidget *ui;
    //当前回合
    int m_Color;
    //枚举棋子
    enum ENUM_BLACK_OR_WHITE{None=0,Black,White};
    //判断输赢的方向
    enum enum_direction{d_z,d_y,d_s,d_x,d_zs,d_yx,d_zx,d_ys};
    //存棋盘上棋子的二维数组
    std::vector<std::vector<int>> m_board;
    //棋子颜色
    QBrush m_pieceColor[3];
    //刷新定时器
    QTimer m_timer;
    //结束标志
    bool m_isOver;
    //根据方向对坐标的偏移，每次是一个单位
    int dx[8] = {-1,1,0,0,-1,1,-1,1};
    int dy[8] = {0,0,-1,1,-1,1,1,-1};
};
#endif // WZQWIDGET_H
