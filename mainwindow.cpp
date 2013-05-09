/*
    mainwindow.cpp
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

#include <QUdpSocket>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "computer.h"
#include "loginrequestform.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showContextMenu(QPoint)));
    connect(ui->treeWidget, SIGNAL(itemSelectionChanged()),
            this, SLOT(selectionChanged()));
    menu = new QMenu;
    menu->addAction(tr("Включить"), this, SLOT(powerup()),
                    QKeySequence(tr("Ctrl+U")));
    menu->addAction(tr("Выключить"), this, SLOT(powerdown()),
                    QKeySequence(tr("Ctrl+D")));
    menu->addAction(tr("Выключить через два часа"), this, SLOT(powerdown()));
    addComputers(QString("computers.txt"));
    ui->treeWidget->expandAll();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete menu;
}

void MainWindow::addComputers(QString fileName)
{
    QFile file(fileName);
    if (file.open(QFile::ReadOnly)) {
        QTextStream stream(&file);
        QTreeWidgetItem *currentItem = 0;
        do {
            QStringList line = stream.readLine().split('\t');
            if (line.at(0) == "c") {
                Computer *comp = new Computer(line.at(2), line.at(1));
                connect(comp, SIGNAL(offline()), this, SLOT(computerOffline()));
                connect(comp, SIGNAL(online()), this, SLOT(computerOnline()));
                if (currentItem == 0)
                    ui->treeWidget->addTopLevelItem(comp);
                else
                    currentItem->addChild(comp);
            } else {
                if (line.at(0).length() == 1) {
                    currentItem = new QTreeWidgetItem(QStringList(line.at(1)));
                    ui->treeWidget->addTopLevelItem(currentItem);
                } else {
                    for (int i = 0; i < line.at(0).length() - 1; ++i) {
                        QString num = QString(line.at(0).at(i));
                        if (i)
                            currentItem = currentItem->child(num.toInt());
                        else
                            currentItem
                                    = ui->treeWidget->topLevelItem(num.toInt());
                    }
                    QTreeWidgetItem *prevItem = currentItem;
                    currentItem = new QTreeWidgetItem(QStringList(line.at(1)));
                    prevItem->addChild(currentItem);
                }
            }
        } while (!stream.atEnd());
        file.close();
    } else {
        QMessageBox::critical(this, tr("Критическая ошибка"),
                              tr("Не удалось прочитать данные о компьютерах из "
                                 "файла. Убедитесь, что в директории приложения"
                                 " присутствует файл «computers.txt» и у вас ес"
                                 "ть права на его чтение или укажите файл в руч"
                                 "ную. Указать местоположение файла в ручную?"),
                              QMessageBox::Ok);
    }
}

void MainWindow::selectionChanged()
{
    QList<QTreeWidgetItem *> selected = ui->treeWidget->selectedItems();
    for (int i = 0; i < selected.count(); ++i)
        for (int j = 0; j < selected.at(i)->childCount(); ++j)
            if (!selected.at(i)->child(j)->isSelected())
                selected.at(i)->child(j)->setSelected(true);
}

void MainWindow::showContextMenu(QPoint pos)
{
    if (ui->treeWidget->itemAt(pos))
        menu->exec(ui->treeWidget->mapToGlobal(pos));
}

void MainWindow::computerOnline()
{
    Computer *comp = static_cast<Computer *>(sender());
    comp->setIcon(0, QIcon(QPixmap(":/images/images/computer.png"))
                  .pixmap(QSize(22, 22), QIcon::Normal));
}

void MainWindow::computerOffline()
{
    Computer *comp = static_cast<Computer *>(sender());
    comp->setIcon(0, QIcon(QPixmap(":/images/images/computer.png"))
                  .pixmap(QSize(22, 22), QIcon::Disabled));
}

void MainWindow::powerup()
{
    QList<QTreeWidgetItem *> items = ui->treeWidget->selectedItems();
    for (int i = 0; i < items.count(); ++i) {
        if (items.at(i)->childCount() == 0) {
            Computer *comp = static_cast<Computer *>(items.at(i));
            comp->poweron();
        }
    }
}

void MainWindow::powerdown()
{
    QList<QTreeWidgetItem *> items = ui->treeWidget->selectedItems();
    for (int i = 0; i < items.count(); ++i) {
        if (items.at(i)->childCount() == 0) {
            Computer *comp = static_cast<Computer *>(items.at(i));
            QAction *sndr = static_cast<QAction *>(sender());
            QString l;
            QString p;
            if (login.isEmpty()) {
                LoginRequestForm *form = new LoginRequestForm(this);
                form->show();
                l = form->getLogin();
                p = form->getPassword();
                delete form;
            }
            if (sndr->text() == tr("Выключить компьютер через два часа"))
                comp->shutdown(l, p, 60 * 120);
            else
                comp->shutdown(l, p);
        }
    }
}
