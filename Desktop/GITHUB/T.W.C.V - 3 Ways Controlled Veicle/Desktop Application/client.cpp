#include "client.h"
#include <QDebug>
#include <string>
#include <unistd.h>
#include <math.h>

#define PI 3.14159265358979323846
#define RAD_TO_DEG 57.3

int VALUE=0, US_VALUE=0, count=0, countUS=0;

QString MessageReceived, Infrared, X_Axis,Y_Axis, Ultrassonic;

Client::Client(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
}

bool Client::connectToHost(QString host, int port)
{
    socket->connectToHost(host, port);
    return socket->waitForConnected();
}

QString Client::gui_Update(int sensor){

    switch(sensor)
    {
        case 1: return X_Axis;      break;
        case 2: return Y_Axis;      break;
        case 3: return Ultrassonic; break;
        case 4: return Infrared;    break;
    }

}

void Client::readData()
{
  MessageReceived = socket->readAll();


  for(QChar & i : MessageReceived){

    unsigned char val=i.toLatin1();                  int x,y,z; float AccXangle=0,AccYangle=0;

    if (val==1)Infrared=" FRONT OBSTACLE!";
    if (val==2)Infrared="";
    if (val>=192) {VALUE=val-192;           count=1;}
    if (val>=64 && val<128) {if(val==127) Ultrassonic= ">=63"; else Ultrassonic=QString::number(val-64);}
    if (val>=128 && val<192) switch (count)
        {
        case 1: VALUE+=(val-128)*64;     count=2;                               break;
        case 2: VALUE+=(val-128)*4096;   count=3;/*X_Axis=QString::number(VALUE);*/      x=VALUE-1000;break;
        case 3: VALUE=(val-128);         count=4;                               break;
        case 4: VALUE+=(val-128)*64;     count=5;                               break;
        case 5: VALUE+=(val-128)*4096;   count=6;/*Y_Axis=QString::number(VALUE);*/ y=VALUE-1000;
        //Y_Axis=QString::number((float)(atan2(x,y)+3.14)*57.3);
        break;

        case 6: VALUE=(val-128);         count=7;                               break;
        case 7: VALUE+=(val-128)*64;     count=8;                               break;
        case 8: VALUE+=(val-128)*4096;   count=0;/*Y_Axis=QString::number(VALUE);*/ z=VALUE-1000;
        AccYangle = (float) (atan2(x,z)+M_PI)*RAD_TO_DEG;
        AccXangle = (float) (atan2(y,z)+M_PI)*RAD_TO_DEG;
        //X_Axis=QString::number(x);Y_Axis=QString::number(z);
        if (AccXangle >180) AccXangle -= (float)360.0;
        if (AccYangle >180) AccYangle -= (float)360.0;
        Y_Axis=QString::number(AccYangle);
        X_Axis=QString::number(AccXangle);

        break;
        }


        qDebug() << "x="<<x<<"y="<<y<<"z="<<z<< "val=" << val << "count"<< count << "VALUE" << VALUE;/*i=256;*/

      }
}

bool Client::writeData(QByteArray data)
{
    if(socket->state() == QAbstractSocket::ConnectedState)
    {
        socket->write(data); //write the data itself
        return socket->waitForBytesWritten();
    }
    else
        return false;
}
