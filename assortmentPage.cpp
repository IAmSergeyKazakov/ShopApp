#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_tableView_assortementList_onAssortement_clicked(const QModelIndex &index)
{
    if(db.open()){

        isAssortmentTableViewClicked = true;
        currentAssortmentTaleViewRow = index.row();

        selectedProductTitle = getTableViewValue(tableModel, currentAssortmentTaleViewRow, 1, *ui->tableView_assortementList_onAssortement);

        selectedProductID = getTableViewValue(tableModel, currentAssortmentTaleViewRow, 0, *ui->tableView_assortementList_onAssortement);

        QString query = "SELECT description FROM SH_products WHERE id = " + selectedProductID;
        QString productDescription = getValueFromDB(query);
        ui->plainTextEdit_productDescription_onAssortement->setVisible(true);
        ui->label_productDescription_onAssortement->setVisible(true);
        ui->plainTextEdit_productDescription_onAssortement->setPlainText(productDescription);
    }
}


void MainWindow::on_comboBox_productType_onAssortment_activated(int index)
{
    if(db.open()){
        QString currentTypeText = ui->comboBox_productType_onAssortment->currentText();
        if(currentTypeText != "Без фильтра"){
            tableModel->setQuery("SELECT SH_products.id, SH_products.title, SH_products.cost FROM SH_products WHERE type = " + QString("\""+currentTypeText+"\""));
            tableModel->setHeaderData(1, Qt::Horizontal, "Наименование"); tableModel->setHeaderData(2, Qt::Horizontal, "Цена");
            ui->tableView_assortementList_onAssortement->setModel(tableModel);
        }
        else{
            tableModel->setQuery("SELECT SH_products.id, SH_products.title, SH_products.cost FROM SH_products");
            tableModel->setHeaderData(1, Qt::Horizontal, "Наименование"); tableModel->setHeaderData(2, Qt::Horizontal, "Цена");
            ui->tableView_assortementList_onAssortement->setModel(tableModel);
        }
    }
}


void MainWindow::on_pushButton_addProductToCart_onAssortement_clicked()
{
    if(db.open()){
        QString query = "SELECT quantity FROM SH_products WHERE title = " + QString("\""+selectedProductTitle+"\"");
        QString productQuantity = getValueFromDB(query);

        query = "SELECT quantity FROM SH_carts WHERE product_id = "+selectedProductID;
        QString currentProductQuantityInCart = getValueFromDB(query);
        qDebug()<<currentProductQuantityInCart;

        if(productQuantity.toInt()>0){
            QSqlQuery productQuery(db);
            if(currentProductQuantityInCart.size() == 0)
            {
                productQuery.prepare("INSERT INTO SH_carts (user_id, product_id, quantity) VALUES (?,?,?)");
                productQuery.addBindValue(global_userID); productQuery.addBindValue(selectedProductID);productQuery.addBindValue("1");
                productQuery.exec();
            }
            else{
                int currentQuantity = currentProductQuantityInCart.toInt();currentQuantity++;
                productQuery.prepare("UPDATE SH_carts SET quantity =  " + QString::number(currentQuantity) + " WHERE product_id = " + selectedProductID);
                productQuery.addBindValue(global_userID); productQuery.addBindValue(selectedProductID);
                productQuery.exec();
            }
            ui->statusbar->showMessage("Вы добавили товар: " +selectedProductTitle);
        }
        else{
            ui->statusbar->showMessage("К сожалению товар закончился");
        }
    }

}



