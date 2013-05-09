/*
    loginrequestform.h
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

#ifndef LOGINREQUESTFORM_H
#define LOGINREQUESTFORM_H

#include <QWidget>

namespace Ui {
class LoginRequestForm;
}

class LoginRequestForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit LoginRequestForm(QWidget *parent = 0);
    ~LoginRequestForm();
    QString getLogin();
    QString getPassword();
    QString getDomain();
    
private:
    Ui::LoginRequestForm *ui;
};

#endif // LOGINREQUESTFORM_H
