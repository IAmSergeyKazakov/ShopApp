#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_pushButton_backToEntranc_onRegistration_clicked(){ui->stackedWidget->setCurrentIndex(0); ui->statusbar->clearMessage(); ui->statusbar->clearMessage();}

void MainWindow::on_pushButton_registration_onEntrance_clicked(){ui->stackedWidget->setCurrentIndex(1); ui->statusbar->clearMessage();ui->statusbar->clearMessage();}

void MainWindow::on_pushButton_adminRegistration_onEntrance_clicked(){ui->stackedWidget->setCurrentIndex(2); ui->statusbar->clearMessage();ui->statusbar->clearMessage();}

void MainWindow::on_pushButton_addProducts_onAdminPanel_clicked(){ui->stackedWidget->setCurrentIndex(5); ui->statusbar->clearMessage();ui->statusbar->clearMessage();}

void MainWindow::on_pushButton_AdminAuth_onMain_clicked(){ui->stackedWidget->setCurrentIndex(4); ui->statusbar->clearMessage();ui->statusbar->clearMessage();}

void MainWindow::on_pushButton_backToUserAuth_onAdminRegistration_clicked(){ui->stackedWidget->setCurrentIndex(0); ui->statusbar->clearMessage();}


void MainWindow::on_pushButton_assortment_onMain_clicked()
{
    if(db.open()){
        tableModel = new QSqlQueryModel(this);
        tableModel->setQuery("SELECT SH_products.id, SH_products.title, SH_products.cost FROM SH_products");
        tableModel->setHeaderData(1, Qt::Horizontal, "Наименование"); tableModel->setHeaderData(2, Qt::Horizontal, "Цена                                                                           ");
        ui->tableView_assortementList_onAssortement->setModel(tableModel);
        ui->tableView_assortementList_onAssortement->hideColumn(0);
        ui->plainTextEdit_productDescription_onAssortement->setVisible(false);
        ui->label_productDescription_onAssortement->setVisible(false);
        ui->stackedWidget->setCurrentIndex(6);
        ui->statusbar->clearMessage();

    }
}

void MainWindow::on_pushButton_backToMain_onAssortment_clicked(){setCart(); ui->stackedWidget->setCurrentIndex(3); ui->statusbar->clearMessage();}


void MainWindow::on_pushButton_createOrder_onMain_clicked()
{
    if(db.open() && cartSum.toInt()!=0){
        QString adressCity, adressStreet, adressHouse, adressEntrance, adressBuilding, adressAppartament, adressFloor;
        QString query = "SELECT user_id FROM SH_adresses WHERE user_id = "+global_userID;
        isUserAdressNotSaved = checkIsValueAvailable(query);
        ui->label_cartSum_onCreationOrder->setText("Итого: "+cartSum);
        if (!isUserAdressNotSaved){
            query = "SELECT city_name FROM SH_adresses WHERE user_id = "+ global_userID; adressCity = getValueFromDB(query);
            query = "SELECT street_name FROM SH_adresses WHERE user_id = "+global_userID; adressStreet = getValueFromDB(query);
            query = "SELECT house_number FROM SH_adresses WHERE user_id = "+global_userID; adressHouse = getValueFromDB(query);
            query = "SELECT building_number FROM SH_adresses WHERE user_id = "+global_userID; adressBuilding = getValueFromDB(query);
            query = "SELECT entrance_number FROM SH_adresses WHERE user_id = "+global_userID; adressEntrance = getValueFromDB(query);
            query = "SELECT floor_number FROM SH_adresses WHERE user_id = "+global_userID; adressFloor = getValueFromDB(query);
            query = "SELECT appartment_number FROM SH_adresses WHERE user_id = "+global_userID; adressAppartament = getValueFromDB(query);

            ui->lineEdit_city_onOrderCreation->setText(adressCity); ui->lineEdit_street_onOrderCreation->setText(adressStreet);
            ui->spinBox_appartament_onOrderCreation->setValue(adressAppartament.toInt());ui->spinBox_building_onOrderCreation->setValue(adressBuilding.toInt());
            ui->spinBox_entrance_onOrderCreation->setValue(adressEntrance.toInt()); ui->spinBox_floor_onOrderCreation->setValue(adressFloor.toInt());
            ui->spinBox_house_onOrderCreation->setValue(adressHouse.toInt());


        }
            QString adressesQuery = "SELECT building_number FROM SH_adresses WHERE user_id = "+global_userID;
            QString buildingVal = getValueFromDB(adressesQuery);

            if (buildingVal != "Нет" && !isUserAdressNotSaved){
                currentCheckBoxState = 1;
                ui->checkBox_isBuilding_onOrderCreation->setChecked(true);
                ui->spinBox_building_onOrderCreation->setVisible(true);
                ui->label_building_onOrderCreation->setVisible(true);
            }
            else{
                currentCheckBoxState = 0;
                ui->checkBox_isBuilding_onOrderCreation->setChecked(false);
                ui->spinBox_building_onOrderCreation->setVisible(false);
                ui->label_building_onOrderCreation->setVisible(false);
            }
        ui->stackedWidget->setCurrentIndex(7);
        ui->statusbar->clearMessage();
    }
    else if(cartSum.toInt() == 0){
        ui->statusbar->showMessage("Ошибка. Ваша корзина пуста");
    }

}



void MainWindow::on_pushButton_userOrders_onMain_clicked()
{
    qDebug()<<cartSum;
    if(db.open()){
        tableModel->setQuery("SELECT DISTINCT order_id, creation_date FROM SH_orders WHERE user_id = "+global_userID + " AND status != 'Завершенные (полученные)'");
        tableModel->setHeaderData(0, Qt::Horizontal, "Номер заказа"); tableModel->setHeaderData(1, Qt::Horizontal, "Дата создания заказа");
        //ui->tableView_cart_onMain->setModel(tableModel);
        ui->tableView_ordersList_onOrdersList->setModel(tableModel);
        ui->stackedWidget->setCurrentIndex(8);
        setOrdersListWidgetsVisibility(false, 0);
    }

    ui->statusbar->clearMessage();

}

void MainWindow::on_pushButton_backToMain_onOrdersList_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    setCart();
    ui->statusbar->clearMessage();
}


void MainWindow::on_pushButton_orders_onAdminPanel_clicked()
{
    qDebug()<<cartSum;
    if(db.open()){
        tableModel = new QSqlQueryModel(this);
        tableModel->setQuery("SELECT DISTINCT order_id, creation_date, status FROM SH_orders WHERE status != 'Завершенные (полученные)'");
        tableModel->setHeaderData(0, Qt::Horizontal, "Номер заказа"); tableModel->setHeaderData(1, Qt::Horizontal, "Дата создания заказа");
        ui->tableView_ordersList_onOrdersListAdmin->setModel(tableModel);
        ui->stackedWidget->setCurrentIndex(9);
        setOrdersListWidgetsVisibility(false, 2);
    }
    ui->statusbar->clearMessage();
}

void MainWindow::on_pushButton_backToAdminPanel_onOrdersListAdmin_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    ui->statusbar->clearMessage();
}


void MainWindow::on_pushButton_backToMain_onAdminPanel_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    setCart();
    ui->statusbar->clearMessage();
}


void MainWindow::on_pushButton_backToAdminPanel_onAddingNewProduct_clicked(){ui->stackedWidget->setCurrentIndex(4);ui->statusbar->clearMessage();}

void MainWindow::on_pushButton_backToMain_onOrderCreation_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    setCart();
    ui->statusbar->clearMessage();
}




