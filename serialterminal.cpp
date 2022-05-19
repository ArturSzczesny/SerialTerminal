#include "serialterminal.h"
#include "ui_serialterminal.h"
#include <QSerialPortInfo>
#include <QDebug>
#include <QString>
#include <QList>
#include <QDateTime>
#include <QFileDialog>
#include <QTextStream>

SerialTerminal::SerialTerminal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SerialTerminal)
{
    ui->setupUi(this);
    this->device = new QSerialPort(this);
}

SerialTerminal::~SerialTerminal()
{
    if(this->device->isOpen())
        this->device->close();
    delete ui;
}


void SerialTerminal::on_pushButtonSearch_clicked()
{
    QList<QSerialPortInfo> devices;
    devices = QSerialPortInfo::availablePorts();
    ui->comboBoxDevices->clear();
    for(int i = 0; i < devices.count(); i++)
    {
      ui->comboBoxDevices->addItem(devices.at(i).portName() + " " + devices.at(i).description());
    }
}


void SerialTerminal::on_pushButtonConnect_clicked()
{
    if((!(ui->pushButtonConnect->text().compare("Connect"))) && (ui->comboBoxDevices->count() != 0))
    {
        QString portName = ui->comboBoxDevices->currentText().split(" ").first();
        this->device->setPortName(portName);

        if(device->open(QSerialPort::ReadWrite))
        {
            this->device->setBaudRate(ui->comboBoxBaud->currentText().toInt());
            this->device->setDataBits(valueToDataBits(ui->comboBoxData->currentIndex()));
            this->device->setParity(valueToParity(ui->comboBoxParity->currentIndex()));
            this->device->setStopBits(valueToStopBits(ui->comboBoxStop->currentIndex()));
            this->device->setFlowControl(valueToFlowControl(ui->comboBoxFlowControl->currentIndex()));

            connect(this->device, SIGNAL(readyRead()), this, SLOT(readPort()));

            ui->pushButtonConnect->setText("Disconnect");
        }
    }
    else if(!(ui->pushButtonConnect->text().compare("Disconnect")))
    {
        if(this->device->isOpen())
        {
            this->device->close();
            ui->pushButtonConnect->setText("Connect");
        }
    }
}


void SerialTerminal::on_pushButtonClear_clicked()
{
    ui->textEditLogs->clear();
}


void SerialTerminal::on_pushButtonSaveToFile_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "", QDir::homePath(), "Text file (*.txt)");
    QFile writeFile(filePath);

    if(writeFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream write(&writeFile);
        write << ui->textEditLogs->toPlainText();
        writeFile.close();
    }

}


void SerialTerminal::on_pushButtonSend_clicked()
{
    if((this->device->isOpen()) && (this->device->isWritable()) && (ui->lineEditSend->cursorPosition() > 0))
    {
        this->device->write(ui->lineEditSend->text().toStdString().c_str());
    }
}


void SerialTerminal::addToLogs(QString message)
{
    QString date = QDateTime::currentDateTime().toString("yyyy.MM.dd");
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss");


    if((ui->checkBoxDate->isChecked()) && (ui->checkBoxTime->isChecked()))
    {
        ui->textEditLogs->append(date + " " + time + "\t" + message);
    }
    else if(ui->checkBoxDate->isChecked())
    {
        ui->textEditLogs->append(date + "\t" + message);
    }
    else if(ui->checkBoxTime->isChecked())
    {
        ui->textEditLogs->append(time + "\t" + message);
    }
    else
    {
        ui->textEditLogs->append(message);
    }
}

void SerialTerminal::readPort()
{
    while(this->device->canReadLine())
    {
        QString line = this->device->readLine();
        int positionOfTerminator = line.lastIndexOf("\r");

        this->addToLogs(line.left(positionOfTerminator));
    }
}


QSerialPort::DataBits SerialTerminal::valueToDataBits(int value)
{
    switch(value)
    {
        case 0:
            return QSerialPort::Data5;
        case 1:
            return QSerialPort::Data6;
        case 2:
            return QSerialPort::Data7;
        case 3:
            return QSerialPort::Data8;
        default:
            return QSerialPort::Data8;
    }
}
QSerialPort::StopBits SerialTerminal::valueToStopBits(int value)
{
    switch(value)
    {
        case 0:
            return QSerialPort::OneStop;
        case 1:
            return QSerialPort::OneAndHalfStop;
        case 2:
            return QSerialPort::TwoStop;
        default:
            return QSerialPort::OneStop;
    }
}
QSerialPort::Parity SerialTerminal::valueToParity(int value)
{
    switch(value)
    {
        case 0:
            return QSerialPort::NoParity;
        case 1:
            return QSerialPort::EvenParity;
        case 2:
            return QSerialPort::OddParity;
        case 3:
            return QSerialPort::SpaceParity;
        case 4:
            return QSerialPort::MarkParity;
        default:
            return QSerialPort::NoParity;
    }
}
QSerialPort::FlowControl SerialTerminal::valueToFlowControl(int value)
{
    switch(value)
    {
        case 0:
            return QSerialPort::NoFlowControl;
        case 1:
            return QSerialPort::HardwareControl;
        case 2:
            return QSerialPort::SoftwareControl;
        default:
            return QSerialPort::NoFlowControl;
    }
}
