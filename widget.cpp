#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("串口调试助手Demo");

    enmuComPort();
    serialPort =  new QSerialPort;

    connect(serialPort, SIGNAL(readyRead()),
            this, SLOT(readDataSlot()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::enmuComPort()
{
    QStringList serialPortInfoList;
    seialList.clear();
    foreach(const QSerialPortInfo &Info,QSerialPortInfo::availablePorts())
    {
        serialPortInfoList.append(Info.portName()+" "+Info.manufacturer());
        seialList.append(Info.portName());
    }
    ui->comboBoxComPort->clear();//清理之前的
    ui->comboBoxComPort->insertItems(0,serialPortInfoList);

}
void Widget::on_pushButtonOpenClosePort_clicked()
{
    QString name; //需要操作的串口号
    qint32 baudRate; //波特率
    QSerialPort::DataBits dataBits;

    QSerialPort::StopBits stopBits;
    QSerialPort::Parity parity;
    QSerialPort::FlowControl flowControl;

    /* 从界面获取串口号 */
    if(seialList.size() > 0)
        name = seialList.at(ui->comboBoxComPort->currentIndex()); //根据index找到对应的串口
    else
        return;
    /* 从界面获取波特率 */
    baudRate = ui->comboBoxBaudRate->currentText().toInt();

    /* 从界面获取数据位 */
    switch (ui->comboBoxDataBits->currentIndex())
    {
    case 0:
        dataBits = QSerialPort::Data5;
        break;
    case 1:
        dataBits = QSerialPort::Data6;
        break;
    case 2:
        dataBits = QSerialPort::Data7;
        break;
    case 3:
        dataBits = QSerialPort::Data8;
        break;
    default:
        break;
    }

    /* 从界面获取停止位 */
    switch (ui->comboBoxStopBits->currentIndex())
    {
    case 0:
        stopBits = QSerialPort::OneStop;
        break;
    case 1:
        stopBits = QSerialPort::OneAndHalfStop;
        break;
    case 2:
        stopBits = QSerialPort::TwoStop;
        break;
    default:
        break;
    }


    /* 从界面获取奇偶校验位 */
    switch (ui->comboBoxParity->currentIndex())
    {
    case 0:
        parity = QSerialPort::NoParity;
        break;
    case 1:
        parity = QSerialPort::EvenParity;
        break;
    case 2:
        parity = QSerialPort::OddParity;
        break;
    case 3:
        parity = QSerialPort::SpaceParity;
        break;
    case 4:
        parity = QSerialPort::MarkParity;
        break;
    default:
        break;
    }

    /* 从界面获取流控位 */
    switch (ui->comboBoxFlowControl->currentIndex())
    {
    case 0:
        flowControl = QSerialPort::NoFlowControl;
        break;
    case 1:
        flowControl = QSerialPort::HardwareControl;
        break;
    case 2:
        flowControl = QSerialPort::SoftwareControl;
        break;
    default:
        break;
    }


    /* 配置串口参数  参数都是从界面中获取的 */
    serialPort->setPortName(name);
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(dataBits);
    serialPort->setParity(parity);
    serialPort->setStopBits(stopBits);
    serialPort->setFlowControl(flowControl);

    if(!serialPort->isOpen())
    {
        if(!serialPort->open(QIODevice::ReadWrite))//以读写方式open
        {
            QMessageBox::warning(this, QString::fromLocal8Bit("打开失败"),
                                 name + serialPort->errorString());
            return;
        }
        else {
            ui->pushButtonOpenClosePort->setText(QStringLiteral("关闭串口"));
            ui->labelDis->setStyleSheet("background-color:#00ff00;border-radius:15px;");
        }
    }
    else{
        ui->labelDis->setStyleSheet("background-color:#808080;border-radius:15px;");
        serialPort->close();
        ui->pushButtonOpenClosePort->setText(QStringLiteral("打开串口"));
    }
}

void Widget::on_pushButtonSend_clicked()
{
    QString sendStr = ui->textEditSend->toPlainText();
//    if(sendStr.length()%2)
//        sendStr.insert(sendStr.length()-1,'0');
    QByteArray sendArry= sendStr.toLocal8Bit();
    if(serialPort->isWritable())
    {
        QString sRet;
        if(ui->radioButtonSendHex->isChecked()){
            for(int i = 0; i < sendArry.count(); i++){
                char szTmp[2] = {0};
                sprintf(szTmp, "%02x", (unsigned char)sendArry.at(i));
                sRet += szTmp;
            }
            sendArry.insert(0,sRet);
        }
        else {
            sendArry = sendStr.toLocal8Bit();
        }
        serialPort->write(sendArry);
    }

}

void Widget::readDataSlot()
{
    QByteArray recvArry;
    QString recvStr;
    if(serialPort->isReadable())
    {
        recvArry = serialPort->readAll();
        if(ui->radioButtonRecvAsii->isChecked())
        {
            recvStr = QString::fromLocal8Bit(recvArry);
        }
        else if(ui->radioButtonRecvHex->isChecked()){
            for(int i = 0; i < recvArry.count(); i++){
               QString s;
               s.sprintf("0x%02x, ", (unsigned char)recvArry.at(i));
               recvStr+=s;
            }

        }
        ui->textBrowserComLog->insertPlainText(recvStr);
    }
}


void Widget::on_pushButtonClear_clicked()
{
    ui->textBrowserComLog->clear();
}
