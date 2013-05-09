/*
    computer.cpp
    Copyright (C) 2011 Mikhalevich Yurij <count@ypsilon.me>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "computer.h"
#include <QUdpSocket>
#include <QTimer>
#include <QProcess>
#include <QApplication>

Computer::Computer(QString mac, QString name, QTreeWidgetItem *parent) :
    QTreeWidgetItem(parent)
{
    setMac(mac);
    setText(0, name);
    setIcon(0,
            QIcon(QPixmap(":/images/images/computer.png")).pixmap(QSize(22, 22),
                                                             QIcon::Disabled));
    shutdownFlag = false;
    poweronFlag = false;
    QTimer *timer = new QTimer;
    connect(timer, SIGNAL(timeout), this, SLOT(checkState()));
    timer->start(3000);
    start();
}

void Computer::run()
{
    QTimer timer;
    connect(&timer, SIGNAL(timeout()), this, SLOT(action()));
    timer.start(2000);
}

void Computer::action()
{
    if (shutdownFlag) {
        execShutdown(login, password, shutdownTimeout);
        shutdownFlag = false;
    }
    if (poweronFlag) {
        execPoweron();
        poweronFlag = false;
    }
    if (checkStateFlag) {
        execCheckState();
        checkStateFlag = false;
    }
}

void Computer::shutdown(QString login, QString password, quint32 timeout) {
    shutdownTimeout = timeout;
    shutdownFlag = true;
    this->login = login;
    this->password = password;
}

void Computer::poweron() {
    poweronFlag = true;
}

void Computer::checkState() {
    checkStateFlag = true;
}

void Computer::execShutdown(QString login, QString password, quint32 timeout)
{
    QProcess proc;
#ifdef Q_OS_UNIX
    proc.start("net rpc shutdown -U " + login + "%" + password + " -f -S "
               + text(0) + ".kubgau.local -t " + QString::number(timeout));
#endif
#ifdef Q_OS_WIN32
    proc.start("cmd /c \"net use \\\\" + text(0) + ".kubgau.local " + password
               + " /user:" + login + " && shutdown -s -f -m \\\\" + text(0)
               + ".kubgau.local -t" + QString::number(timeout) + "\"");
#endif
    proc.waitForFinished(-1);
    qApp->processEvents();
}

void Computer::execPoweron()
{
    QByteArray baMac = QByteArray::fromHex(macString.toLocal8Bit());
    QByteArray ba6FF = QByteArray::fromHex("FFFFFFFFFFFF");
    QByteArray ba16Mac;
    for (int i = 0; i < 16; i++)
       ba16Mac += baMac;
    QByteArray baWOLSignal = ba6FF + ba16Mac;
    QUdpSocket s;
    s.writeDatagram(baWOLSignal, baWOLSignal.size(),
                    QHostAddress::Broadcast, 9);
}

#include <QMessageBox>
#include <QWidget>
void Computer::execCheckState()
{
    QProcess proc;
#ifdef Q_OS_UNIX
    proc.start(QString("ping -c 1 " + text(0)));
#endif
#ifdef Q_OS_WIN32
    proc.start(QString("ping " + text(0) + " -n 1"));
#endif
    proc.waitForFinished(-1);
    QString output = QString(proc.readAllStandardOutput());
    QWidget *widget = new QWidget;
    widget->show();
    QMessageBox::warning(widget, "olo", QString("ping " + text(0) + " -n 1\n") + output);
    if (!output.isEmpty() && output.contains("ttl=", Qt::CaseInsensitive))
        emit online();
    else
        emit offline();
    quit();
}

void Computer::setMac(QString mac)
{
    macString = mac;
    if (macString.contains(":"))
        macString.remove(":");
    if (macString.contains("-"))
        macString.remove("-");
}
