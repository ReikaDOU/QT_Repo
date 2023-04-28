#include "wzqwidget.h"
#include "ui_wzqwidget.h"
wzqWidget::wzqWidget(QWidget *parent)
    : QWidget(parent)
    ,ui(new Ui::wzqWidget)
    ,m_board(WZQ_COLS,std::vector<int>(WZQ_ROWS,0))
    ,m_Color(Black)
    ,m_isOver(false)
{
    ui->setupUi(this);

    m_pieceColor[None] = QBrush(QColor(0,0,0,0));
    m_pieceColor[Black] = QBrush(QColor(0,0,0));
    m_pieceColor[White] = QBrush(QColor(255,255,255));

    connect(&m_timer,SIGNAL(timeout()),this,SLOT(repaint()));//重绘
    m_timer.start(1000/120);

    connect(this,SIGNAL(SIG_pieceDown(int,int,int)),this,SLOT(slot_pieceDown(int,int,int)));

    slot_startGame();
}

wzqWidget::~wzqWidget()
{
    m_timer.stop();
    delete ui;
}

void wzqWidget::paintEvent(QPaintEvent *event)
{
     //绘制棋盘
     QPainter painer(this);
     painer.setBrush(QBrush(QColor(36,211,159)));
     painer.drawRect(WZQ_MARGIN_WIDTH-WZQ_DISTANCE,
                     WZQ_MARGIN_HEIGHT-WZQ_DISTANCE,
                     (WZQ_COLS+1)*WZQ_SPACE+2*WZQ_DISTANCE,
                     (WZQ_ROWS+1)*WZQ_SPACE+2*WZQ_DISTANCE);
     //网格线
     for(int i=1; i<=WZQ_COLS;++i)
     {
         painer.setBrush(QBrush(QColor(0,0,0)));
         QPoint p1(WZQ_MARGIN_WIDTH + WZQ_SPACE*i,
                   WZQ_MARGIN_HEIGHT + WZQ_SPACE);
         QPoint p2(WZQ_MARGIN_WIDTH + WZQ_SPACE*i,
                   WZQ_MARGIN_HEIGHT + (WZQ_ROWS)*WZQ_SPACE);
         painer.drawLine(p1,p2);
     }
     for(int i=1; i<=WZQ_ROWS;++i)
     {
         painer.setBrush(QBrush(QColor(0,0,0)));
         QPoint p1(WZQ_MARGIN_WIDTH + WZQ_SPACE,
                   WZQ_MARGIN_HEIGHT + WZQ_SPACE*i);
         QPoint p2(WZQ_MARGIN_WIDTH + WZQ_SPACE*(WZQ_COLS),
                   WZQ_MARGIN_HEIGHT + i*WZQ_SPACE);
         painer.drawLine(p1,p2);
     }
     //中心点
     painer.setBrush(QBrush(QColor(0,0,0)));
     painer.drawEllipse(QPoint(290,290),6/2,6/2);

     //画棋子
     for(int x=0;x<WZQ_COLS;++x)
     {
         for(int y=0;y<WZQ_ROWS;++y)
         {
             if(m_board[x][y] != None)
             {
                 painer.setBrush(m_pieceColor[m_board[x][y]]);
                 painer.drawEllipse(//画圆 ， 中心点坐标，半径x,y
                             QPoint(WZQ_MARGIN_WIDTH + WZQ_SPACE+ WZQ_SPACE*x,
                                    WZQ_MARGIN_HEIGHT +WZQ_SPACE + WZQ_SPACE*y),
                                    WZQ_PIECE_SIZE/2,WZQ_PIECE_SIZE/2);
             }
         }
     }
}

void wzqWidget::mouseReleaseEvent(QMouseEvent *event)
{
    //落子四舍五入
    int x=0;
    int y=0;
    float fx = (float)event->pos().x();
    float fy = (float)event->pos().y();

    //加入结束判断
    if(m_isOver)
        goto quit;

    fx = (fx-WZQ_MARGIN_WIDTH - WZQ_SPACE) / WZQ_SPACE;
    fy = (fy-WZQ_MARGIN_HEIGHT - WZQ_SPACE) / WZQ_SPACE;

    //四舍五入
    x = (fx-(int)fx)>0.5 ? ((int)fx+1):(int) fx;
    y = (fy-(int)fy)>0.5 ? ((int)fy+1):(int) fy;

    //是否越界
    if(isOverLine(x,y)) return;

    Q_EMIT SIG_pieceDown(getBlackOrWhite(),x,y);

quit:
    event->accept();
}

int wzqWidget::getBlackOrWhite() const
{
     return m_Color;
}

void wzqWidget::changeBlackOrWhite()
{
    m_Color = m_Color + 1;
    if(m_Color == 3)
    {
        m_Color = Black;
    }
}

bool wzqWidget::isOverLine(int x, int y)
{
    if(x<0||x>=15||y<0||y>=15)
    {
        return true;
    }
    return false;
}

bool wzqWidget::isWin(int x, int y)
{
    int num=1;
    for(int fx = d_z;fx<8;fx+=2)
    {
        for(int i=1;i<=4;++i)
        {
            int xx=dx[fx]*i+x;
            int yy=dy[fx]*i+y;

            if(isOverLine(xx,yy))
            {
                break;
            }
            if(m_board[xx][yy]==m_board[x][y])
            {
                num++;
            }
            else
            {
                break;
            }
        }
        if(num>=5)
            break;
        else
            num=1;
    }
    if(num>=5)
    {
        m_isOver = true;
        return true;
    }
    return false;
}

void wzqWidget::clear()
{
    //状态位
    m_isOver = true;
    m_Color = Black;
    //界面
    for(int x=0;x<WZQ_COLS;++x)
    {
        for(int y=0;y<WZQ_ROWS;++y)
        {
            m_board[x][y]=None;
        }
    }
}
//落子槽，谁的回合，坐标
void wzqWidget::slot_pieceDown(int color, int x,int y)
{
    if(color !=getBlackOrWhite())
    {
        return ;
    }
    if(m_board[x][y]==None)
    {
        m_board[x][y]=color;

        if(isWin(x,y))
        {
            QString str =getBlackOrWhite()==Black? "黑胜":"白胜";
            QMessageBox::about(this,"提示",str);
            clear();
        }
        else
        {
            changeBlackOrWhite();
        }
    }
}
void wzqWidget::slot_startGame()
{
    clear();
    m_isOver = false;
}
