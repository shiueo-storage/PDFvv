#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_pdfvv.h"
#include <QTimer>
#include <QFileDialog>
#include <QFontDatabase>


class PDFvv : public QMainWindow
{
    Q_OBJECT

public:
    PDFvv(QWidget *parent = nullptr);
    ~PDFvv();

public slots:
    void update();
    void View_Button_Clicked();
    void Images_to_PDF_Button_Clicked();
    void PDF_to_Images_Button_Clicked();
    void RESET_Button_Clicked();
    void ADD_Folder_Button_Clicked();
    void IMPORT_PDF_Button_Clicked();
    void REMOVE_Button_Clicked();
    void LINEEDIT_Button_Clicked();
    void UP_Button_Clicked();
    void DOWN_Button_Clicked();
    void LIST_Button_Clicked();

    void UPDATE_FOLDER();
    void FOLDER_CLICKED();
    void FILE_CLICKED();

private:
    Ui::PDFvvClass ui;
    QTimer* timer;
    std::string CURRENT_TAB = "VIEW";
    std::string NONE_STR = "NULL___NULL___NULL";

    std::vector<std::vector<std::string>> FOLDER_LIST = { {"Default", "OPENED"}};
    std::vector<QPushButton*> FOLDER_Q_LIST;
    std::vector<std::string> FILE_LIST = {};

    QLayoutItem* wItem;
    QPushButton* CURRENT_FOLDER;
    QPushButton* CURRENT_FILE;
    QFileDialog* FILE_DIALOG;
    bool IS_CURRENT_FOLDER;

    QString Pretendard_Black = QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/PDFvv/assets/fonts/Pretendard-Black.otf")).constFirst();
    QString Pretendard_Bold = QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/PDFvv/assets/fonts/Pretendard-Bold.otf")).constFirst();
    QString Pretendard_ExtraBold = QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/PDFvv/assets/fonts/Pretendard-ExtraBold.otf")).constFirst();
    QString Pretendard_ExtraLight = QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/PDFvv/assets/fonts/Pretendard-ExtraLight.otf")).constFirst();
    QString Pretendard_Medium = QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/PDFvv/assets/fonts/Pretendard-Medium.otf")).constFirst();
    QString Pretendard_Regular = QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/PDFvv/assets/fonts/Pretendard-Regular.otf")).constFirst();
    QString Pretendard_SemiBold = QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/PDFvv/assets/fonts/Pretendard-SemiBold.otf")).constFirst();
    QString Pretendard_Thin = QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/PDFvv/assets/fonts/Pretendard-Thin.otf")).constFirst();
};
