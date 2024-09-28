#include "mainwindow.h"
#include "ui_mainwindow.h"


void MainWindow::on_pushButton_addNewProduct_onAddingNewProduct_clicked()
{
    if(db.open()){
        QString productTitle = ui->lineEdit_productTitle_onAddingNewProduct->text();
        QString productQuantity = ui->spinBox_productQuantity_onAddingNewProducts->text();
        QString productCost = ui->spinBox_productQuantity_onAddingNewProducts->text();
        QString productDescription = ui->plainTextEdit_productDescription_onAddingNewProduct->toPlainText();
        QString productType = ui->comboBox_productType_onAddingNewProduct->currentText();

        if(productTitle.size()>0&& productQuantity.size()>0 && productCost.size()>0 && productDescription.size()>0){
            QString query = "SELECT title FROM SH_products WHERE title = " + QString("\""+productTitle+"\"");
            bool isProductAvailable = checkIsValueAvailable(query);

            QSqlQuery insertNewProductQuery(db);
            if (isProductAvailable){
                insertNewProductQuery.prepare("INSERT INTO SH_products (title, cost, quantity, description, type) VALUES (?,?,?,?,?)");
                insertNewProductQuery.addBindValue(productTitle);insertNewProductQuery.addBindValue(productCost);
                insertNewProductQuery.addBindValue(productQuantity);
                insertNewProductQuery.addBindValue(productDescription);
                insertNewProductQuery.addBindValue(productType);
                insertNewProductQuery.exec();
                ui->statusbar->showMessage("Успешно");
            }

            else{
                msgBox.setWindowTitle("Такой товар уже есть");
                msgBox.setText("Такой продукт уже есть в ассоритменте.\n Добавить ли введенное количество к уже существующему?");
                msgBox.setStandardButtons(QMessageBox::Yes);
                msgBox.addButton(QMessageBox::No);
                msgBox.setDefaultButton(QMessageBox::No);
                if(msgBox.exec() == QMessageBox::Yes){
                    query = "SELECT quantity FROM SH_products WHERE title = " + QString("\""+productTitle+"\"");
                    QString currentQuantity = getValueFromDB(query);
                    int fVal = currentQuantity.toInt();
                    int sVal = productQuantity.toInt();
                    fVal+=sVal;
                    insertNewProductQuery.prepare("UPDATE SH_products SET quantity = " + QString::number(fVal) +" WHERE title = " + QString("\""+productTitle+"\""));
                    insertNewProductQuery.exec();
                }
            }
        }

        else
        {
            ui->statusbar->showMessage("Некорректные данные");

        }

    }
}




