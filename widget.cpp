#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QTableWidget>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //初始化设置端口
    ui->sentdata->setEnabled(true);
    ui->sentdata->setEnabled(false);

    Temperature_Init();
    Phosphine_Init();
// 表格初始化
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setAlternatingRowColors(true);     //设置隔行变颜色
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);       //选中整行
    //设置表头
    QStringList header;
    header<<tr("时间")<<tr("温度")<<tr("磷化氢");
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setColumnWidth(0,65);
    ui->tableWidget->setColumnWidth(1,50);
    ui->tableWidget->setColumnWidth(2,50);

    // lcd 初始化
        ui->lcdNumber->setDigitCount(4);    //显示位数
        ui->lcdNumber->setMode(QLCDNumber::Dec);    //十进制
        ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);       //显示方式
        ui->lcdNumber->setStyleSheet("background-color: yellow");
        ui->lcdNumber->setPalette(Qt::red);

            //设置背景色

    // lcd_2 初始化
        ui->lcdNumber_2->setDigitCount(4);    //显示位数
        ui->lcdNumber_2->setMode(QLCDNumber::Dec);    //十进制
        ui->lcdNumber_2->setSegmentStyle(QLCDNumber::Flat);       //显示方式
        ui->lcdNumber_2->setStyleSheet("background-color: yellow");
        ui->lcdNumber_2->setPalette(Qt::red);
}
//温度曲线图初始化
void Widget ::Temperature_Init()
{
    //绘图初始化
        // 添加曲线 1
    ui->widget->addGraph();         // 增加图层
    ui->widget->graph(0)->setPen(QPen(Qt::red));
    ui->widget->graph(0)->setBrush(QBrush(QColor(0,0,205,50)));     // 设置图层画刷颜色
    ui->widget->graph(0)->setAntialiasedFill(false);        // 设置图层反锯齿：关闭
    // 添加点
    ui->widget->addGraph();
    ui->widget->graph(1)->setPen(QPen(Qt::blue,2));       // 设置笔的颜色
    ui->widget->graph(1)->setLineStyle(QCPGraph::lsNone);   // 不画线条
    ui->widget->graph(1)->setScatterStyle(QCPScatterStyle::ssDisc);     // 设置点的形状

    //坐标轴1设置
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%m:%s");             //时间格式：  秒
    ui->widget->xAxis->setTicker(timeTicker);       // 横坐标显示时间
    ui->widget->axisRect()->setupFullAxesBox();     //坐标为矩
    // x轴设置
    ui->widget->xAxis->setLabel("时间(单位s)");         // 设置横轴标签
    ui->widget->yAxis->setRange(0,6000);       //设置温度范围
    ui->widget->xAxis->setLabel("时间");
    ui->widget->yAxis->setLabel("烟雾浓度");
    //ui->widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);    //设置属性可缩放，移动
    ui->widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);    //允许可缩放，移动
    QList < QCPAxis *>  xis;
    xis.append(ui->widget->xAxis);
    ui->widget->axisRect()->setRangeZoomAxes(xis);
}

//磷化氢曲线初始化
void Widget ::Phosphine_Init()
{
    //绘图初始化
        // 添加曲线 1
    ui->widget_1->addGraph();         // 增加图层
    ui->widget_1->graph(0)->setPen(QPen(Qt::red));
    ui->widget_1->graph(0)->setBrush(QBrush(QColor(0,0,205,50)));     // 设置图层画刷颜色
    ui->widget_1->graph(0)->setAntialiasedFill(false);        // 设置图层反锯齿：关闭
    // 添加点
    ui->widget_1->addGraph();
    ui->widget_1->graph(1)->setPen(QPen(Qt::blue,2));       // 设置笔的颜色
    ui->widget_1->graph(1)->setLineStyle(QCPGraph::lsNone);   // 不画线条
    ui->widget_1->graph(1)->setScatterStyle(QCPScatterStyle::ssDisc);     // 设置点的形状

    //坐标轴1设置
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%m:%s");             //时间格式：  秒
    ui->widget_1->xAxis->setTicker(timeTicker);       // 横坐标显示时间
    ui->widget_1->axisRect()->setupFullAxesBox();     //坐标为矩
    // x轴设置
    ui->widget_1->xAxis->setLabel("时间(单位s)");         // 设置横轴标签
    ui->widget_1->yAxis->setRange(0,50);       //设置温度范围
    ui->widget_1->xAxis->setLabel("时间");
    ui->widget_1->yAxis->setLabel("磷化氢浓度");
    //ui->widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);    //设置属性可缩放，移动
    ui->widget_1->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);    //允许可缩放，移动
    QList < QCPAxis *>  xis;
    xis.append(ui->widget_1->xAxis);
    ui->widget_1->axisRect()->setRangeZoomAxes(xis);
    // 轴随动
    connect(ui->widget->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->widget->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget->yAxis2, SLOT(setRange(QCPRange)));
    // 实时更新槽函数
    _pTimerUpdate = new QTimer(this);
    connect(_pTimerUpdate, SIGNAL(timeout()), this, SLOT(customsplot_init()));
    connect(_pTimerUpdate, SIGNAL(timeout()), this, SLOT(customsplot_init_1()));
}

//串口点击事件
void Widget::on_select_port_clicked()
{
     select_port();
}
//扫描串口
void Widget:: select_port()
{
        allPort.clear();
        foreach(const QSerialPortInfo &info, QSerialPortInfo ::availablePorts())
        {
            QSerialPort  serial;
            serial.setPort(info);
            if(serial.open(QIODevice::ReadWrite))
            {
                if(-1 == allPort.indexOf(serial.portName()))
                    allPort.append(serial.portName());
                serial.close();
            }
        }
        ui->portName->clear();
        ui->portName->addItems(allPort);


}

//CRC_16校验
int  Widget:: usMBCRC16( int  pu[], int usLen )
{

    int ucCRCHi = 0xFF;
    int ucCRCLo = 0xFF;
    int i=0;
    int iIndex;
    while( usLen-- )
     {
        iIndex = ucCRCLo ^ pu[i++] ;
        ucCRCLo =  ucCRCHi ^ aucCRCHi[iIndex]; //(int)
        ucCRCHi = aucCRCLo[iIndex];
     }
        return ( ucCRCHi << 8 | ucCRCLo );
}

Widget::~Widget()
{
    delete ui;
    delete serial;
}

//16进制接收数据
void Widget::readData()
{
    QString c,d,yan1,yan2,yan3,lin1,lin2,lin3;

    QByteArray buf;
    buf = serial->readAll();
    if(!buf.isEmpty())
    {
        if(ui->checkShowReciveHex->isChecked())
        {
            // byteArray 转 16进制
            QByteArray temp = buf.toHex();

            d=tr(temp);
            c=d.remove(0,6);
            c=c.remove(9,4);

            yan1=c.left(4);
            yan2=yan1.right(2);
            yan3=yan1.left(2);
            yan0=yan3.toInt(&OK,16)*256+yan2.toInt(&OK,16);//获取烟浓度
            //lcd显示
            ui->lcdNumber->display(yan0);

            lin1=c.right(4);
            lin2=lin1.right(2);
            lin3=lin1.left(2);
            lin0=lin3.toInt(&OK,16)*256+lin2.toInt(&OK,16);//获取磷化氢浓度
            //lcd显示
            ui->lcdNumber_2->display(lin0);

            if((yan0-oldyan >50)|(oldyan-yan0>50)|(lin0-oldlin>20)|(oldlin-lin0>20))
            {
              opeartDB(yan0,lin0);
              oldyan=yan0;
              oldlin=lin0;
            }
            insert_table(yan0,lin0);

            buf.clear();    //清空缓存区
        }
    }
}

//打开端口事件
void Widget::on_openPort_clicked()
{

    QString sDbNm = "C:/Users/22060/Desktop/Database2.mdb";
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");//设置数据库驱动
    QString dsn = QString("DRIVER={Microsoft Access Driver (*.mdb)}; FIL={MS Access};DBQ=%1;").arg(sDbNm);//连接字符串
    db.setDatabaseName(dsn);//设置连接字符串
    db.setUserName("");//设置登陆数据库的用户名
    db.setPassword("");//设置密码

    if(ui->portName->count() == 0)
    {
        QMessageBox::about(this,"打开串口失败","没有可用串口,扫描串口后重试！"); return;
    }
    if(ui->openPort->text()==tr("打开串口"))
    {
        serial = new QSerialPort;
        //设置串口名
        serial->setPortName(ui->portName->currentText());
        //打开串口
        serial->open(QIODevice::ReadWrite);
        //设置波特率
        serial->setBaudRate(ui->portBaudRate->currentText().toInt());
        //设置数据位数
        switch(ui->portDataBits->currentIndex())
        {
        case 8: serial->setDataBits(QSerialPort::Data8); break;
        default: break;
        }
        //设置奇偶校验
        switch(ui->portParity->currentIndex())
        {
        case 0: serial->setParity(QSerialPort::NoParity); break;
        default: break;
        }
        //设置停止位
        switch(ui->portStopBits->currentIndex())
        {
        case 1: serial->setStopBits(QSerialPort::OneStop); break;
        case 2: serial->setStopBits(QSerialPort::TwoStop); break;
        default: break;
        }
        //设置流控制
        serial->setFlowControl(QSerialPort::NoFlowControl);
        //关闭设置菜单使能
        ui->portName->setEnabled(false);
        ui->portBaudRate->setEnabled(false);
        ui->portDataBits->setEnabled(false);
        ui->portParity->setEnabled(false);
        ui->portStopBits->setEnabled(false);
        ui->select_port->setEnabled(false);
        ui->sentdata->setEnabled(true);
        ui->openPort->setText(tr("关闭串口"));

        bool ok = db.open();
        if (!ok)
        {
            QMessageBox messageBox;
            messageBox.setText("Database error");
            messageBox.exec();
            db.close();
        }
        _pTimerUpdate->start(1000);
        //连接信号槽
        QObject::connect(serial, &QSerialPort::readyRead, this, &Widget::readData);
    }
    else
    {
        //关闭串口
        serial->clear();
        serial->close();
        serial->deleteLater();
        //恢复设置使能
        ui->portName->setEnabled(true);
        ui->portBaudRate->setEnabled(true);
        ui->portDataBits->setEnabled(true);
        ui->portParity->setEnabled(true);
        ui->portStopBits->setEnabled(true);
        ui->select_port->setEnabled(true);
        ui->sentdata->setEnabled(false);
        ui->openPort->setText(tr("打开串口"));
        db.close();
        _pTimerUpdate->stop();
    }

}

//16进制发送数据
void Widget::on_sentdata_clicked()
{
    if(ui->checkShowSend->isChecked())
    {
        //接收两个阈值的数据进行处理

        //烟雾阈值设置
        int  a=ui->lineEdit->text().toInt()/256;
        int  b=ui->lineEdit->text().toInt()%256;
        QString hex1 = QString("%1").arg(a, 2, 16,QChar('0'));
        QString hex2 = QString("%1").arg(b, 2, 16,QChar('0'));

        //磷化氢阈值设置
        int  c = ui->lineEdit_2->text().toInt()/256;
        int  d = ui->lineEdit_2->text().toInt()%256;
        QString hex3 = QString("%1").arg(c, 2, 16,QChar('0'));
        QString hex4 = QString("%1").arg(d, 2, 16,QChar('0'));
        //设置校验位
        //添加内容
        QString hex5 = QString("%1").arg(5, 2, 16,QChar('0'));
        int ww1=hex1.toInt(&OK,16);
        int ww2=hex2.toInt(&OK,16);
        int ww3=hex3.toInt(&OK,16);
        int ww4=hex4.toInt(&OK,16);
        int ww5=hex5.toInt(&OK,16);
        int num[5] = {ww5,ww1,ww2,ww3,ww4};
        int num1  = usMBCRC16(num,5);
        QString ff= QString("%1").arg(num1,4,16,QChar('0'));        
        QString ee=ff.right(2);
        QString dd=ff.left(2);
        QString hh=" ";
        ui->result->setText(hex5+hh+hex1+hh+hex2+hh+hex3+hh+hex4+hh+ee+hh+dd);
        //发送数据
        QString str=ui->result->text();
        QString A[20]={};
        int counter=0;
        for(int i=0;i<str.length();i+=2)
        {
            if(str[i] == ' ')
            {
                --i;
                continue;
            }
            A[counter] = str.mid(i,2);
            SendBuff.append(A[counter++].toInt(&OK,16));
        }        
        serial->write(SendBuff);
        SendBuff.clear();
    }
}

//对数据的存储进行相关操作
void Widget::opeartDB(int yan,int lin)
{
    //获取时间
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy/MM/dd hh:mm:ss");

    QString time=current_date.left(10);//"yyyy/MM/dd"
    QString time1=current_date.right(8);//"hh:mm:ss"

    QSqlQuery query;
    QString select_all_sql = "select * from ["+ time +" ]";
    QString create_sql = "create table ["+ time +" ](Times  DATETIME, Nums  INTEGER  ,Data1  INTEGER)";
    QString insert_sql = "insert into ["+ time +" ] VALUES (:Times, :Nums, :Data1)";

    if(query.exec(select_all_sql))
    {
        query.prepare(insert_sql);
        query.bindValue(":Times", time1);
        query.bindValue(":Nums", yan);
        query.bindValue(":Data1", lin);
        query.exec();
    }
    else
    {
        if(query.exec(create_sql))
        {
            query.prepare(insert_sql);
            query.bindValue(":Times", time1);
            query.bindValue(":Nums", yan);
            query.bindValue(":Data1", lin);
            query.exec();
        }
          else
        {
            qDebug()<<"创建不成功";
        }
    }
}

//插入数据到表格中
void Widget::insert_table(int yan,int lin)
{
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy/MM/dd hh:mm:ss");
    QString date=current_date.right(8);//"hh:mm:ss"

    update();
    int row_count = ui->tableWidget->rowCount();   //获取总行数
    ui->tableWidget->insertRow(row_count);         //插入行
    QTableWidgetItem *item0 = new QTableWidgetItem();
    QTableWidgetItem *item1 = new QTableWidgetItem();
    QTableWidgetItem *item2 = new QTableWidgetItem();
    item0->setText(date);
    item1->setText(QString::number(yan));
    item2->setText(QString::number(lin));
    ui->tableWidget->setItem(row_count,0,item0);
    ui->tableWidget->setItem(row_count,1,item1);
    ui->tableWidget->setItem(row_count,2,item2);

    //自动清除表格中数据
    cunt++;
    if(cunt==40)
    {
        while(ui->tableWidget->rowCount())
        {
            ui->tableWidget->removeRow(0);
        }
        cunt=0;
    }

}

//绘画烟雾浓度图
void Widget::customsplot_init()
{
    static QTime time(QTime::currentTime());        //获取当前时间
    //计算新的数据点
    double key = time.elapsed()/1000.0;
    static double lastPointKey = 0;

    if (key-lastPointKey > 0.002) //大于2ms添加一个数据
    {
        ui->widget->graph(0)->addData(key, yan0);      //温度加到数据中
        ui->widget->graph(1)->addData(key, yan0);
        lastPointKey = key;

        QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
        QString date = time.toString("MM.dd hh:mm"); //设置显示格式
        QString tempe;
        tempe = QString("%1").arg(yan0);
    }

    ui->widget->xAxis->setRange(key, 10, Qt::AlignRight);        //设置x轴范围
    ui->widget->replot();       //画图

    // 缩放轴
     QList < QCPAxis *>  axis_x,axis_y,axis_xy;
     axis_x.append(ui->widget->xAxis);
     axis_y.append(ui->widget->yAxis);
     axis_xy.append(ui->widget->xAxis);
     axis_xy.append(ui->widget->yAxis);
}

//绘画磷化氢浓度图
void Widget::customsplot_init_1()
{
    static QTime time(QTime::currentTime());        //获取当前时间
    //计算新的数据点
    double key = time.elapsed()/1000.0;
    static double lastPointKey = 0;

    if (key-lastPointKey > 0.002) //大于2ms添加一个数据
    {
        ui->widget_1->graph(0)->addData(key, lin0);      //温度加到数据中
        ui->widget_1->graph(1)->addData(key, lin0);
        lastPointKey = key;

        QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
        QString date = time.toString("MM.dd hh:mm"); //设置显示格式
        QString tempe;
        tempe = QString("%1").arg(lin0);
    }

    ui->widget_1->xAxis->setRange(key, 10, Qt::AlignRight);        //设置x轴范围
    ui->widget_1->replot();       //画图

    // 缩放轴
     QList < QCPAxis *>  axis_x,axis_y,axis_xy;
     axis_x.append(ui->widget_1->xAxis);
     axis_y.append(ui->widget_1->yAxis);
     axis_xy.append(ui->widget_1->xAxis);
     axis_xy.append(ui->widget_1->yAxis);
}




















