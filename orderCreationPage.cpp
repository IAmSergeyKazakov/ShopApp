#include "mainwindow.h"
#include "ui_mainwindow.h"


void MainWindow::on_pushButton_createOrder_onOrderCreation_clicked()
{
    if(db.open()){
        adressCity = ui->lineEdit_city_onOrderCreation->text(); adressStreet = ui->lineEdit_street_onOrderCreation->text();
        adressHouse = ui->spinBox_house_onOrderCreation->text(); adressEntrance = ui->spinBox_entrance_onOrderCreation->text();
        adressBuilding = ui->spinBox_building_onOrderCreation->text(); adressAppartament = ui->spinBox_appartament_onOrderCreation->text();
        adressFloor = ui->spinBox_floor_onOrderCreation->text();
        todayDate = QDate::currentDate();
        QString todayDateString = todayDate.toString("dd.MM.yyyy");
        QString availableProductQuantity, queryForGetProductQuantity, currentCartProductQuantity, currentProductTitle;

        int availableProductQuantityINT, currentCartProductQuantityINT, currentInteration;
        currentInteration = 0;
        int rowQuantity = ui->tableView_cart_onMain->model()->rowCount();
        QSqlQuery query(db);

        if(currentCheckBoxState == 0)
            adressBuilding = "Нет";

        if (adressCity.size()>0 && adressHouse.size() > 0 && adressStreet.size() >0 && adressHouse.size() > 0 && adressEntrance.size() > 0 && adressBuilding.size() > 0 && adressAppartament.size() > 0 && adressFloor.size() > 0)
        {
                QString getLastOrderIDQuery = "SELECT MAX(order_id) FROM SH_orders";
                QString lastOrderID = getValueFromDB(getLastOrderIDQuery);
                int lastOrderIDInt = lastOrderID.toInt();
                lastOrderIDInt++;
                QString currentProductID;

                QModelIndex productTitleIndex;

                for (int x = 0; x < rowQuantity; x++)
                {

                    currentProductID = getTableViewValue(tableModel, x, 1, *ui->tableView_cart_onMain);

                    currentProductTitle = getTableViewValue(tableModel, x, 2, *ui->tableView_cart_onMain);

                    queryForGetProductQuantity = "SELECT quantity FROM SH_products WHERE id = "+currentProductID;
                    availableProductQuantity = getValueFromDB(queryForGetProductQuantity);

                    currentCartProductQuantity = getTableViewValue(tableModel, x, 4, *ui->tableView_cart_onMain);

                    availableProductQuantityINT = availableProductQuantity.toInt();
                    currentCartProductQuantityINT = currentCartProductQuantity.toInt();


                    if(availableProductQuantityINT >= currentCartProductQuantityINT){
                        query.prepare("INSERT INTO SH_orders (order_id, user_id, product_id, creation_date, city_name, street_name ,house_number, building_number, appartment_number, floor_number, entrance_number, status, quantity) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
                        query.addBindValue(QString::number(lastOrderIDInt));query.addBindValue(global_userID);query.addBindValue(currentProductID);query.addBindValue(todayDateString);
                        query.addBindValue(adressCity);query.addBindValue(adressStreet);query.addBindValue(adressHouse);query.addBindValue(adressBuilding);
                        query.addBindValue(adressAppartament);query.addBindValue(adressFloor);query.addBindValue(adressEntrance);query.addBindValue("На обработке");query.addBindValue(currentCartProductQuantity);
                        query.exec();

                        query.prepare("UPDATE SH_products SET quantity = " + QString::number(availableProductQuantityINT-currentCartProductQuantityINT) + " WHERE id = " + currentProductID);
                        query.exec();
                        currentInteration++;
                    }
                    else{
                        query.prepare("DELETE FROM SH_orders WHERE order_id = " + (QString::number(lastOrderIDInt)));
                        query.exec();
                        break;
                    }
                }
        }
        else{
                ui->statusbar->showMessage("Некорректные данные");
                return;
        }

        qDebug()<<"cI: "<<currentInteration;
        qDebug()<<"rQ: "<<rowQuantity;
        if(currentInteration == rowQuantity){
            query.prepare("DELETE FROM SH_carts WHERE user_id = "+global_userID);
            query.exec();

            msgBox.setWindowTitle("Сохранить данные об адресе?");
            msgBox.setText("Сохранить данный адрес для следующих заказов?\n В случае согласия в следующий раз вам не придется заново вводить данные");
            msgBox.setStandardButtons(QMessageBox::Yes);
            msgBox.addButton(QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::No);

            if(msgBox.exec() == QMessageBox::Yes){
                    query.prepare("DELETE FROM SH_adresses WHERE user_id = " + global_userID);
                    query.exec();
                    query.prepare("INSERT INTO SH_adresses (user_id, city_name, street_name, house_number, building_number, appartment_number, floor_number, entrance_number) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
                    query.addBindValue(global_userID);query.addBindValue(adressCity);query.addBindValue(adressStreet);query.addBindValue(adressHouse);
                    query.addBindValue(adressBuilding);query.addBindValue(adressAppartament);query.addBindValue(adressFloor);query.addBindValue(adressEntrance);
                    query.exec();
            }
            setCart();
            ui->stackedWidget->setCurrentIndex(3);
            ui->statusbar->clearMessage();
        }
        else{
            ui->statusbar->showMessage("К сожалению у нас осталось всего " + availableProductQuantity+ " единиц товара "+ currentProductTitle);
        }

    }

}


void MainWindow::on_checkBox_isBuilding_onOrderCreation_stateChanged(int currentState){
    if(db.open()){
        currentCheckBoxState = currentState;
        if(currentState == 0){
            ui->spinBox_building_onOrderCreation->setVisible(false);
            ui->label_building_onOrderCreation->setVisible(false);
        }
        else{
            ui->spinBox_building_onOrderCreation->setVisible(true);
            ui->label_building_onOrderCreation->setVisible(true);
        }
    }
}



