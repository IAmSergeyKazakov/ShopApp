#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlQueryModel>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>
#include <QItemSelectionModel>
#include <QItemSelection>
#include <QLabel>
#include <QTableView>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_CreateAcc_onRegistration_clicked();

    void on_pushButton_Entrance_onEntrance_clicked();

    void on_pushButton_adminRegistration_onEntrance_clicked();

    void on_pushButton_createAcc_onAdminRegistration_clicked();

    void on_pushButton_addProducts_onAdminPanel_clicked();

    void on_pushButton_addNewProduct_onAddingNewProduct_clicked();

    void on_pushButton_backToEntranc_onRegistration_clicked();

    void on_pushButton_registration_onEntrance_clicked();

    void on_pushButton_AdminAuth_onMain_clicked();

    void on_pushButton_assortment_onMain_clicked();

    void on_tableView_assortementList_onAssortement_clicked(const QModelIndex &index);

    void on_comboBox_productType_onAssortment_activated(int index);

    void on_pushButton_addProductToCart_onAssortement_clicked();

    void on_pushButton_backToMain_onAssortment_clicked();

    void on_pushButton_addProductFromCart_onMain_clicked();

    void on_tableView_cart_onMain_clicked(const QModelIndex &index);

    void on_pushButton_deleteProductFromCart_onMain_clicked();

    void on_pushButton_createOrder_onMain_clicked();

    void on_pushButton_createOrder_onOrderCreation_clicked();

    void on_checkBox_isBuilding_onOrderCreation_stateChanged(int arg1);

    void on_pushButton_userOrders_onMain_clicked();

    void on_pushButton_backToMain_onOrdersList_clicked();

    void on_tableView_ordersList_onOrdersList_clicked(const QModelIndex &index);

    void on_comboBox_orderFilter_onOrdersList_activated(int index);

    void on_checkBox_isOrderGot_onOrdersList_stateChanged(int currentState);

    void on_pushButton_orders_onAdminPanel_clicked();

    void on_tableView_ordersList_onOrdersListAdmin_clicked(const QModelIndex &index);

    void on_comboBox_orderStatus_onOrdersListAdmin_activated();

    void on_pushButton_saveComboBoxVal_onOrdersListAdmin_clicked();

    void on_pushButton_sendMessage_onOrdersListAdmin_clicked();

    void on_pushButton_backToAdminPanel_onOrdersListAdmin_clicked();

    void on_pushButton_backToMain_onAdminPanel_clicked();

    void on_pushButton_backToAdminPanel_onAddingNewProduct_clicked();

    void on_pushButton_backToMain_onOrderCreation_clicked();

    void on_pushButton_backToUserAuth_onAdminRegistration_clicked();

private:

    Ui::MainWindow *ui;
    void setDatabase();
    bool checkIsValueAvailable(QString& query);
    QString getValueFromDB(QString& queryText);
    bool isAdminAcc(QString& userName);
    void setCart();
    void setOrdersListWidgetsVisibility(bool status, int numOfPage);
    void changeProductsQuantityCart(bool isAdding);
    void setUIOperations();
    void getUserAdress(QLabel& label);
    QString getTableViewValue(QSqlQueryModel* model, int& rowIndex, int columnIndex, QTableView& tableView);

    //productTitleIndex = tableModel->index(x, 1, QModelIndex());
    //currentProductID = ui->tableView_cart_onMain->model()->data(productTitleIndex).toString();

    QSqlQueryModel *tableModel = new QSqlQueryModel(this);
    QSqlQueryModel *productsOrdersPageTableModel = new QSqlQueryModel(this);

    QSqlDatabase db;

    QString global_userName, global_userID, global_userPassword,selectedProductTitle, selectedProductID, adressCity, adressStreet, adressHouse,
        adressEntrance, adressBuilding, adressAppartament, adressFloor, selectedOrderID, cartSum;

    int currentAssortmentTaleViewRow, currentCheckBoxState, currentRow, currentProductQuantity;
    QString const secretPassword = "slozhni parol";
    bool isAssortmentTableViewClicked = false;
    bool isUserAdressNotSaved;
    QDate todayDate;
    QMessageBox msgBox;

};
#endif // MAINWINDOW_H
