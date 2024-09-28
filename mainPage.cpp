#include "mainwindow.h"
#include "ui_mainwindow.h"

QString selectedRowCartID;
void MainWindow::on_pushButton_addProductFromCart_onMain_clicked()
{
    changeProductsQuantityCart(true);
}


void MainWindow::on_tableView_cart_onMain_clicked(const QModelIndex &index)
{
    QItemSelectionModel *sectionModel = ui->tableView_cart_onMain->selectionModel();
    QItemSelection selection(index, index);
    sectionModel->select(selection, QItemSelectionModel::Rows);
    if(db.open()){
        currentRow = index.row();
        QModelIndex productIDIndex = tableModel->index(currentRow, 1, QModelIndex());
        selectedProductID = ui->tableView_cart_onMain->model()->data(productIDIndex).toString();

        QString query = "SELECT quantity FROM SH_carts WHERE product_id = "+selectedProductID;
        currentProductQuantity = getValueFromDB(query).toInt();

        qDebug()<<currentProductQuantity;


        productIDIndex = tableModel->index(currentRow, 0, QModelIndex());
        selectedRowCartID = ui->tableView_cart_onMain->model()->data(productIDIndex).toString();
    }
}


void MainWindow::on_pushButton_deleteProductFromCart_onMain_clicked()
{
    changeProductsQuantityCart(false);
}



