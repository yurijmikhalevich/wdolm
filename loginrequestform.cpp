/*
    loginrequestform.cpp
    Copyright (C) 2012 Mikhalevich Yurij <count@ypsilon.me>
    
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

#include "loginrequestform.h"
#include "ui_loginrequestform.h"

LoginRequestForm::LoginRequestForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginRequestForm)
{
    ui->setupUi(this);
}

LoginRequestForm::~LoginRequestForm()
{
    delete ui;
}

QString LoginRequestForm::getLogin()
{
    return ui->login->text();
}

QString LoginRequestForm::getPassword()
{
    return ui->password->text();
}
