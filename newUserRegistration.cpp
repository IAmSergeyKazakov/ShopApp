#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_pushButton_CreateAcc_onRegistration_clicked()
{
    QString name = ui->lineEdit_Name_onRegistration->text();
    QString password = ui->lineEdit_Password_onRegistration->text();
    QString confirmPassword = ui->lineEdit_ConfirmPassword_onRegistration->text();
    QSqlQuery addDataQuery(db);
    QString userID;
    bool isAvailableName;
    QString query = "SELECT name FROM SH_users WHERE name = " + QString("\""+name+"\"");

    if (db.open())
    {

        isAvailableName = checkIsValueAvailable(query);
        if (isAvailableName == true && confirmPassword == password && confirmPassword.size()>0 && password.size()>0 && name.size()>0 && password.size()>=6)
        {
            addDataQuery.prepare("INSERT INTO SH_users (name,password,isAdmin) VALUES (?, ?, ?)");
            addDataQuery.addBindValue(name);addDataQuery.addBindValue(password);addDataQuery.addBindValue("0");
            addDataQuery.exec();

            QString query = "SELECT id FROM SH_users WHERE name = "+QString("\""+name+"\"")  +" AND isAdmin = 0";
            userID = getValueFromDB(query);

            ui->statusbar->showMessage("Успешно!");
        }
        else if (isAvailableName == false)
            ui->statusbar->showMessage("Такое имя уже есть");
        else if (confirmPassword != password)
            ui->statusbar->showMessage("Пароли не совпадают");
        else if (password.size()<6)
            ui->statusbar->showMessage("Пароль должен состоять минимум из 6 символов");
        else
            ui->statusbar->showMessage("Некорректные данные");
    }
    else{
        ui->statusbar->showMessage("Ошибка. База данных не подключена");
    }

}


void MainWindow::on_pushButton_createAcc_onAdminRegistration_clicked()
{
    QString name = ui->lineEdit_name_onAdminRegistration->text();
    QString password = ui->lineEdit_password_onAdminRegistration->text();
    QString userSecretPassword = ui->lineEdit_secretCode_onAdminRegistration->text();
    QSqlQuery addDataQuery(db);
    QString userID;
    bool isAvailableName;
    bool isAdminAccount;
    QString realUserPassword;
    if (db.open())
    {
    QString query = "SELECT password FROM SH_users WHERE name = "+QString("\""+name+"\"");
    realUserPassword = getValueFromDB(query);
    qDebug()<<realUserPassword;
    query = "SELECT name FROM SH_users WHERE name = " + QString("\""+name+"\"");

        isAvailableName = checkIsValueAvailable(query);
        if (realUserPassword == password && password.size()>0 && name.size()>0 && userSecretPassword == secretPassword && !isAvailableName)
        {
            isAdminAccount = isAdminAcc(name);

                if (isAdminAccount){
                    qDebug()<<"Ваш аккаунт уже имеет админ-права";
                }
                else{
                    addDataQuery.prepare("UPDATE SH_users SET isAdmin = 1  WHERE name = " + QString("\""+name+"\""));addDataQuery.exec();
                    ui->statusbar->showMessage("Успешно!");
                }

        }
        else if (isAvailableName)
            ui->statusbar->showMessage("Такого аккаунта не существует");
        else if (realUserPassword != password)
            ui->statusbar->showMessage("Пароль неверный. Админ-права не выданы");
        else if(userSecretPassword != secretPassword)
            ui->statusbar->showMessage("Секретный пароль неверен");

        else
            ui->statusbar->showMessage("Некорректные данные");
    }
    else{
        ui->statusbar->showMessage("Ошибка. База данных не подключена");
    }
}
