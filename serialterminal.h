#ifndef SERIALTERMINAL_H
#define SERIALTERMINAL_H

#include <QMainWindow>
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class SerialTerminal; }
QT_END_NAMESPACE

class SerialTerminal : public QMainWindow
{
    Q_OBJECT

public:
    SerialTerminal(QWidget *parent = nullptr);
    ~SerialTerminal();

private slots:
    void on_pushButtonSearch_clicked();

    void on_pushButtonConnect_clicked();

    void readPort();

    void on_pushButtonClear_clicked();

    void on_pushButtonSaveToFile_clicked();

    void on_pushButtonSend_clicked();

private:
    Ui::SerialTerminal *ui;
    QSerialPort *device;
    void addToLogs(QString message);
    QSerialPort::DataBits valueToDataBits(int value);
    QSerialPort::StopBits valueToStopBits(int value);
    QSerialPort::Parity valueToParity(int value);
    QSerialPort::FlowControl valueToFlowControl(int value);
};
#endif // SERIALTERMINAL_H
