#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    setDatabase();

    setUIOperations();



}


MainWindow::~MainWindow()
{
    delete tableModel;
    tableModel = nullptr;

    delete productsOrdersPageTableModel;
    productsOrdersPageTableModel = nullptr;

    delete ui;
    ui = nullptr;

    db.close();
}


void MainWindow::setUIOperations()
{
    ui->pushButton_AdminAuth_onMain->setVisible(false);

    ui->spinBox_productQuantity_onAddingNewProducts->setMaximum(1000000);
    ui->spinBox_appartament_onOrderCreation->setMaximum(1000000);
    ui->spinBox_building_onOrderCreation->setMaximum(1000000);
    ui->spinBox_entrance_onOrderCreation->setMaximum(1000000);
    ui->spinBox_floor_onOrderCreation->setMaximum(1000000);
    ui->spinBox_house_onOrderCreation->setMaximum(1000000);
    ui->doubleSpinBox_productCost_onAddingNewProducts->setMaximum(1000000.0);

    ui->plainTextEdit_productDescription_onAddingNewProduct->setMaximumBlockCount(10);
    ui->plainTextEdit_productDescription_onAssortement->setReadOnly(true);

    ui->plainTextEdit_productDescription_onAddingNewProduct->setMaximumBlockCount(10);
    ui->plainTextEdit_adminMessage_onOrdersList->setReadOnly(true);

    ui->tableView_cart_onMain->verticalHeader()->setVisible(false);
    ui->tableView_cart_onMain->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->tableView_assortementList_onAssortement->verticalHeader()->setVisible(false);
    ui->tableView_assortementList_onAssortement->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->tableView_orderProducts_onOrdersList->verticalHeader()->setVisible(false);
    ui->tableView_orderProducts_onOrdersList->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->tableView_ordersList_onOrdersList->verticalHeader()->setVisible(false);
    ui->tableView_ordersList_onOrdersList->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->tableView_ordersList_onOrdersListAdmin->verticalHeader()->setVisible(false);
    ui->tableView_ordersList_onOrdersListAdmin->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->tableView_productList_onOrdersListAdmin->verticalHeader()->setVisible(false);
    ui->tableView_productList_onOrdersListAdmin->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);



}


void MainWindow::getUserAdress(QLabel& label)
{
    QString query;
    query = "SELECT street_name FROM SH_orders WHERE order_id = "+selectedOrderID; adressStreet = getValueFromDB(query);
    query = "SELECT city_name FROM SH_orders WHERE order_id = "+selectedOrderID; adressCity = getValueFromDB(query);
    query = "SELECT house_number FROM SH_orders WHERE order_id = "+selectedOrderID; adressHouse = getValueFromDB(query);
    query = "SELECT building_number FROM SH_orders WHERE order_id = "+selectedOrderID; adressBuilding = getValueFromDB(query);
    query = "SELECT appartment_number FROM SH_orders WHERE order_id = "+selectedOrderID; adressAppartament = getValueFromDB(query);
    query = "SELECT floor_number FROM SH_orders WHERE order_id = "+selectedOrderID; adressFloor = getValueFromDB(query);
    query = "SELECT entrance_number FROM SH_orders WHERE order_id = "+selectedOrderID; adressEntrance = getValueFromDB(query);


    label.setText("Город: "+adressCity+"\n"
                                               "Улица: "+adressStreet+"\n"
                                               "Дом: "+adressHouse+"\n"
                                               "Корпус: "+adressBuilding+"\n"
                                               "Подъезд: "+ adressEntrance+"\n"
                                               "Квартира: "+ adressAppartament);
}


bool MainWindow::checkIsValueAvailable(QString& query)
{
    if(db.open()){
        QString userNameFromDB;

        bool isAvailableName = true;

            userNameFromDB = getValueFromDB(query);
            if (userNameFromDB.size() != 0)
                isAvailableName = false;
        return isAvailableName;
    }
}


QString MainWindow::getValueFromDB(QString& queryText)
{
    if(db.open()){
        QSqlQuery query(db);
        QString stringResult;

        query.prepare(queryText);
        query.exec();
        while (query.next()){
            stringResult = query.value(0).toString();
        }
        return stringResult;
    }
}


bool MainWindow::isAdminAcc(QString& userName){
    if(db.open()){
        QString query, isAdminAcc;
        query = "SELECT isAdmin FROM SH_users WHERE name = "+QString("\""+userName+"\"");
        isAdminAcc = getValueFromDB(query);
        qDebug()<<isAdminAcc;

        if (isAdminAcc == "1")
            return true;
        else
            return false;
    }
}

void MainWindow::setCart()
{
    if(db.open()){
        tableModel->setQuery("SELECT SH_carts.id, SH_products.id, SH_products.title, SH_products.cost, SH_carts.quantity FROM SH_products JOIN SH_carts ON SH_products.id = SH_carts.product_id WHERE SH_carts.user_id = " + global_userID);
        tableModel->setHeaderData(2, Qt::Horizontal, "Наименование"); tableModel->setHeaderData(3, Qt::Horizontal, "Цена"); tableModel->setHeaderData(4, Qt::Horizontal, "Количество                                                 ");
        ui->tableView_cart_onMain->setModel(tableModel);

        ui->tableView_cart_onMain->hideColumn(0); ui->tableView_cart_onMain->hideColumn(1);

        QString cartSumQuery = "SELECT SUM(SH_products.cost * SH_carts.quantity) FROM SH_products JOIN SH_carts ON SH_products.id = SH_carts.product_id WHERE SH_carts.user_id = "+ global_userID;
        cartSum = getValueFromDB(cartSumQuery);
        ui->label_cartSum_onMain->setText(cartSum);
    }
}



void MainWindow::setOrdersListWidgetsVisibility(bool status, int numOfPage)
{
    if(numOfPage == 0){
        ui->tableView_orderProducts_onOrdersList->setVisible(status);
        ui->label_orderNumber_onOrdersList->setVisible(status);
        ui->tableView_orderProducts_onOrdersList->setVisible(status);
        ui->label_orderSummary_onOrdersList->setVisible(status);
        ui->label_orderStatus_onOrdersList->setVisible(status);
        ui->checkBox_isOrderGot_onOrdersList->setVisible(status);
        ui->label_adminMessage_onOrdersList->setVisible(status);
        ui->plainTextEdit_adminMessage_onOrdersList->setVisible(status);
        ui->label_lookAdress_onOrdersList->setVisible(status);
    }
    else{
        ui->comboBox_orderStatus_onOrdersListAdmin->setVisible(status);
        ui->pushButton_saveComboBoxVal_onOrdersListAdmin->setVisible(status);
        ui->label_messageToUser_onOrdersListAdmin->setVisible(status);
        ui->label_orderNumber_onOrdersListAdmin->setVisible(status);
        ui->label_orderStatus_onOrdersListAdmin->setVisible(status);
        ui->label_userName_onOrdersListAdmin->setVisible(status);
        ui->plainTextEdit_messageToUser_onOrdersListAdmin->setVisible(status);
        ui->pushButton_saveComboBoxVal_onOrdersListAdmin->setVisible(status);
        ui->label_lookAdress_onOrdersListAdmin->setVisible(status);
    }
}


void MainWindow::changeProductsQuantityCart(bool isAdding)
{
    if(db.open()){
        QSqlQuery queryForAddingProduct(db);
        if (isAdding)
        {
            queryForAddingProduct.prepare("UPDATE SH_carts SET quantity = "+ QString::number(currentProductQuantity+1) + " WHERE product_id = "+selectedProductID);
            queryForAddingProduct.exec();
        }
        else
        {
            if(currentProductQuantity - 1 != 0){
                queryForAddingProduct.prepare("UPDATE SH_carts SET quantity = "+ QString::number(currentProductQuantity-1) + " WHERE product_id = "+selectedProductID);
                queryForAddingProduct.exec();
            }
            else{
                queryForAddingProduct.prepare("DELETE FROM SH_carts WHERE product_id = " + selectedProductID);
                queryForAddingProduct.exec();
            }
        }
    }
    setCart();
}

QString MainWindow::getTableViewValue(QSqlQueryModel* model, int &rowIndex, int columnIndex, QTableView &tableView)
{
    QModelIndex index = model->index(rowIndex, columnIndex, QModelIndex());
    QString result = tableView.model()->data(index).toString();

    return result;
}
