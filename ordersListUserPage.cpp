#include "mainwindow.h"
#include "ui_mainwindow.h"


void MainWindow::on_tableView_ordersList_onOrdersList_clicked(const QModelIndex &index)
{
    if(db.open()){
        int currentRow = index.row();
        QString orderSummary, orderStatus, query;

        setOrdersListWidgetsVisibility(true, 0);
        ui->plainTextEdit_adminMessage_onOrdersList->setVisible(false);

        selectedOrderID = getTableViewValue(tableModel, currentRow, 0, *ui->tableView_ordersList_onOrdersList);

        getUserAdress(*ui->label_lookAdress_onOrdersList);

        productsOrdersPageTableModel->setQuery("SELECT SH_products.title, SH_products.cost, SH_orders.quantity FROM SH_products INNER JOIN SH_orders ON SH_products.id = SH_orders.product_id WHERE SH_orders.order_id = " + selectedOrderID);
        productsOrdersPageTableModel->setHeaderData(0, Qt::Horizontal, "Наименование"); productsOrdersPageTableModel->setHeaderData(1, Qt::Horizontal, "Цена"); productsOrdersPageTableModel->setHeaderData(2, Qt::Horizontal, "Количество                                        ");
        ui->tableView_orderProducts_onOrdersList->setModel(productsOrdersPageTableModel);

        ui->label_orderNumber_onOrdersList->setText("Заказ №" +selectedOrderID);
        query = "SELECT SUM(SH_products.cost * SH_orders.quantity) FROM SH_products JOIN SH_orders ON SH_products.id = SH_orders.product_id WHERE SH_orders.order_id = "+selectedOrderID;

        orderSummary = getValueFromDB(query);
        ui->label_orderSummary_onOrdersList->setText("Общая сумма: "+orderSummary);

        query = "SELECT status FROM SH_orders WHERE order_id = "+selectedOrderID;
        orderStatus = getValueFromDB(query);
        ui->label_orderStatus_onOrdersList->setText("Статус заказа: "+orderStatus);

        QString queryForGetMessage = "SELECT messageText FROM SH_messages WHERE order_id = "+selectedOrderID;
        QString messageFromDB = getValueFromDB(queryForGetMessage);

        if(messageFromDB.size()>0){
            ui->plainTextEdit_adminMessage_onOrdersList->setPlainText(messageFromDB);
            ui->plainTextEdit_adminMessage_onOrdersList->setVisible(true);
        }
        else{
            ui->plainTextEdit_adminMessage_onOrdersList->setVisible(false);
        }



        if(orderStatus != "Готов к выдаче")
            ui->checkBox_isOrderGot_onOrdersList->setVisible(false);
        else
             ui->checkBox_isOrderGot_onOrdersList->setVisible(true);
    }
}


void MainWindow::on_comboBox_orderFilter_onOrdersList_activated(int index)
{
    if(db.open()){
        QString query, orderStatus, currentOrderStatusComboBox;

        currentOrderStatusComboBox = ui->comboBox_orderFilter_onOrdersList->currentText();
        if(currentOrderStatusComboBox != "Без фильтра"){
            tableModel->setQuery("SELECT DISTINCT order_id, creation_date FROM SH_orders WHERE user_id = "+global_userID + " AND status =  " + QString("\""+currentOrderStatusComboBox+"\""));
            ui->tableView_ordersList_onOrdersList->setModel(tableModel);
        }
        else{
            tableModel->setQuery("SELECT DISTINCT order_id, creation_date FROM SH_orders WHERE user_id = "+global_userID);
            ui->tableView_ordersList_onOrdersList->setModel(tableModel);
        }
    }
}


void MainWindow::on_checkBox_isOrderGot_onOrdersList_stateChanged(int currentState)
{
    if(db.open()){
        QSqlQuery query(db);
        query.prepare("UPDATE SH_orders SET status = 'Завершенные (полученные)' WHERE order_id = " + selectedOrderID); query.exec();
        tableModel->setQuery("SELECT DISTINCT order_id, creation_date FROM SH_orders WHERE user_id = "+global_userID + " AND status != 'Завершенные (полученные)'");
        ui->tableView_ordersList_onOrdersList->setModel(tableModel);
        ui->checkBox_isOrderGot_onOrdersList->setChecked(false);
        setOrdersListWidgetsVisibility(false,0);
    }
}
