#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QList>
#include <QMessageBox>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    void enmuComPort();

private slots:
    void on_pushButtonOpenClosePort_clicked();

    void on_pushButtonSend_clicked();

    void readDataSlot();

    void on_pushButtonClear_clicked();

private:
    Ui::Widget *ui;
    QList<QString> seialList;
    QSerialPort *serialPort;
};

#endif // WIDGET_H
