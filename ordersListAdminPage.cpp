#include "mainwindow.h"
#include "ui_mainwindow.h"

QString currentOrderStatus;

void MainWindow::on_tableView_ordersList_onOrdersListAdmin_clicked(const QModelIndex &index)
{
    if(db.open()){
        int currentRow = index.row();
        QString query, userName;

        QModelIndex orderIDIndex = tableModel->index(currentRow, 0, QModelIndex());
        selectedOrderID = ui->tableView_ordersList_onOrdersListAdmin->model()->data(orderIDIndex).toString();

        selectedProductID = getTableViewValue(tableModel, currentRow, 0, *ui->tableView_ordersList_onOrdersListAdmin);


        getUserAdress(*ui->label_lookAdress_onOrdersListAdmin);


        productsOrdersPageTableModel->setQuery("SELECT SH_products.title, SH_products.cost, SH_carts.quantity FROM SH_products INNER JOIN SH_orders ON SH_products.id = SH_orders.product_id WHERE SH_orders.order_id = " + selectedOrderID);
        ui->tableView_productList_onOrdersListAdmin->setModel(productsOrdersPageTableModel);

        ui->label_orderNumber_onOrdersListAdmin->setText("Заказ №" +selectedOrderID);
        setOrdersListWidgetsVisibility(true,2);

        query = "SELECT DISTINCT SH_users.name FROM SH_users INNER JOIN SH_orders ON SH_orders.user_id = SH_users.id WHERE SH_orders.order_id = "+ selectedOrderID;
        userName = getValueFromDB(query);
        ui->label_userName_onOrdersListAdmin->setText("Имя пользователя: "+userName);

    }
}


void MainWindow::on_comboBox_orderStatus_onOrdersListAdmin_activated(){currentOrderStatus = ui->comboBox_orderStatus_onOrdersListAdmin->currentText();}


void MainWindow::on_pushButton_saveComboBoxVal_onOrdersListAdmin_clicked()
{
    if(db.open()){
        QSqlQuery query(db);
        query.prepare("UPDATE SH_orders SET status = " + QString("\""+currentOrderStatus+"\"") + " WHERE order_id = " + selectedOrderID);
        query.exec();
        tableModel->setQuery("SELECT DISTINCT order_id, creation_date, status FROM SH_orders WHERE status != 'Завершенные (полученные)'");
        ui->tableView_ordersList_onOrdersListAdmin->setModel(tableModel);
    }

}


void MainWindow::on_pushButton_sendMessage_onOrdersListAdmin_clicked()
{
    if(db.open()){
        QString queryForGetMessage = "SELECT messageText FROM SH_messages WHERE order_id = "+selectedOrderID;
        QString messageFromDB = getValueFromDB(queryForGetMessage);
        QString message = ui->plainTextEdit_messageToUser_onOrdersListAdmin->toPlainText();
        QSqlQuery query(db);
        if(messageFromDB.size() == 0){
            query.prepare("INSERT INTO SH_messages (order_id, messageText) VALUES (?, ?)");
            query.addBindValue(selectedOrderID);query.addBindValue(message);
            query.exec();
        }
        else{
            query.prepare("UPDATE SH_messages SET messageText = "+QString("\""+message+"\"") +" WHERE order_id = "+selectedOrderID);
            query.exec();
        }
    }
}
