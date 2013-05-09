/*
    computer.h
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

#ifndef COMPUTER_H
#define COMPUTER_H

#include <QTreeWidgetItem>
#include <QThread>

class Computer : public QThread, public QTreeWidgetItem
{
    Q_OBJECT
public:
    explicit Computer(QString mac, QString name, QTreeWidgetItem *parent = 0);
    void setMac(QString m);
    QString mac() { return macString; }

private:
    QString macString;
    bool shutdownFlag;
    bool poweronFlag;
    bool checkStateFlag;
    quint32 shutdownTimeout;
    QString login;
    QString password;
    void execShutdown(QString login, QString password, quint32 timeout);
    void execPoweron();
    void execCheckState();
    void run();

private slots:
    void action();

signals:
    void offline();
    void online();

public slots:
    void shutdown(QString login, QString password, quint32 timeout = 5);
    void poweron();
    void checkState();
};

#endif // COMPUTER_H
