#include "pdfvv.h"
#include "ui_pdfvv.h"
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWebEngineCore/QWebEngineSettings>
#include <QGroupBox>
#include <QScrollArea>
#include <QVBoxLayout>
#include <filesystem>

uint64_t timeSinceEpochMillisec() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

PDFvv::PDFvv(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    QVBoxLayout* FOLDER_BOX = new QVBoxLayout();
    QGroupBox* FOLDER_GROUPBOX = new QGroupBox();
    QScrollArea* FOLDER_SCROLLAREA = new QScrollArea();
        
    FOLDER_BOX->setObjectName("FOLDER_BOX");
    FOLDER_GROUPBOX->setLayout(FOLDER_BOX);
    FOLDER_SCROLLAREA->setWidget(FOLDER_GROUPBOX);
    FOLDER_SCROLLAREA->setWidgetResizable(true);
    FOLDER_SCROLLAREA->setMaximumWidth(320);

    ui.verticalLayout->addWidget(FOLDER_SCROLLAREA);


    // UPDATE 1000/40ms마다
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000 / 40);

    ui.WEBVIEW->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    ui.WEBVIEW->settings()->setAttribute(QWebEngineSettings::PdfViewerEnabled, true);


    ui.PDFvv_LOGO->setFont(QFont(Pretendard_ExtraBold, 14));

    ui.View_Button->setFont(QFont(Pretendard_Medium, 11));
    connect(ui.View_Button, SIGNAL(clicked()), this, SLOT(View_Button_Clicked()));

    ui.Images_to_PDF_Button->setFont(QFont(Pretendard_Medium, 11));
    connect(ui.Images_to_PDF_Button, SIGNAL(clicked()), this, SLOT(Images_to_PDF_Button_Clicked()));

    ui.PDF_to_Images_Button->setFont(QFont(Pretendard_Medium, 11));
    connect(ui.PDF_to_Images_Button, SIGNAL(clicked()), this, SLOT(PDF_to_Images_Button_Clicked()));
        
    connect(ui.ADD_Folder_Button, SIGNAL(clicked()), this, SLOT(ADD_Folder_Button_Clicked()));
    connect(ui.RESET_Button, SIGNAL(clicked()), this, SLOT(RESET_Button_Clicked()));
    connect(ui.IMPORT_PDF_Button, SIGNAL(clicked()), this, SLOT(IMPORT_PDF_Button_Clicked()));
    connect(ui.REMOVE_Button, SIGNAL(clicked()), this, SLOT(REMOVE_Button_Clicked()));
    connect(ui.LIST_Button, SIGNAL(clicked()), this, SLOT(LIST_Button_Clicked()));
    connect(ui.LINEEDIT_Button, SIGNAL(clicked()), this, SLOT(LINEEDIT_Button_Clicked()));
    connect(ui.UP_Button, SIGNAL(clicked()), this, SLOT(UP_Button_Clicked()));
    connect(ui.DOWN_Button, SIGNAL(clicked()), this, SLOT(DOWN_Button_Clicked()));
    ui.LineEdit->setFont(QFont(Pretendard_Medium, 11));
    UPDATE_FOLDER();
}

PDFvv::~PDFvv()
{
}

void PDFvv::UPDATE_FOLDER()
{
    FOLDER_Q_LIST = {};
    while ((wItem = findChild<QVBoxLayout*>("FOLDER_BOX")->takeAt(0)) != 0)
    {
        if (wItem->widget())
        {
            wItem->widget()->setParent(nullptr);
            delete wItem;
        }
    }

    for (int i = 0; i < FOLDER_LIST.size(); i++)
    {
        QPushButton* button = new QPushButton(QString::fromStdString(FOLDER_LIST[i][0]), this);
        button->setIcon(QIcon(":/PDFvv/assets/icons/ionicons/folder.svg"));
        button->setFont(QFont(Pretendard_Medium, 11));
        button->setObjectName(std::to_string(timeSinceEpochMillisec()));
        connect(button, SIGNAL(clicked()), this, SLOT(FOLDER_CLICKED()));
        FOLDER_Q_LIST.push_back(button);

        findChild<QVBoxLayout*>("FOLDER_BOX")->addWidget(button);
        if (FOLDER_LIST[i][1] == "OPENED")
        {
            button->setStyleSheet("text-align: left; border-bottom: 2px solid #f72585");
            for (int j = 2; j < FOLDER_LIST[i].size(); j++)
            {
                QPushButton* button_x = new QPushButton(QString::fromStdString(std::filesystem::path(FOLDER_LIST[i][j]).stem().string()), this);
                button_x->setIcon(QIcon(":/PDFvv/assets/icons/ionicons/document.svg"));
                button_x->setFont(QFont(Pretendard_Medium, 11));
                button_x->setStyleSheet("text-align: left; padding: 4px 40px");
                connect(button_x, SIGNAL(clicked()), this, SLOT(FILE_CLICKED()));
                findChild<QVBoxLayout*>("FOLDER_BOX")->addWidget(button_x);
            }
        }
        else {
            button->setStyleSheet("text-align: left; border-bottom: 2px solid #000000");
        }
    }
    findChild<QVBoxLayout*>("FOLDER_BOX")->addStretch(1);
}

void PDFvv::update()
{
    if (CURRENT_TAB == "VIEW") {
        ui.WEBVIEW->setUrl(QUrl("https://cshtarn.github.io/PDFvv/"));
        ui.View_Button->setStyleSheet("border-bottom: 2px solid #f72585");
        ui.Images_to_PDF_Button->setStyleSheet("border-bottom: 2px solid #fafbfb");
        ui.PDF_to_Images_Button->setStyleSheet("border-bottom: 2px solid #fafbfb");
    }
    if (CURRENT_TAB == "Images_to_PDF") {
        ui.WEBVIEW->setUrl(QUrl("https://www.youtube.com/channel/UCuxqlCs2LbHu8XSkjN-UPdQ?view_as=subscriber"));
        ui.View_Button->setStyleSheet("border-bottom: 2px solid #fafbfb");
        ui.Images_to_PDF_Button->setStyleSheet("border-bottom: 2px solid #f72585");
        ui.PDF_to_Images_Button->setStyleSheet("border-bottom: 2px solid #fafbfb");
    }
    if (CURRENT_TAB == "PDF_to_Images") {
        ui.WEBVIEW->setUrl(QUrl("https://www.youtube.com/"));
        ui.View_Button->setStyleSheet("border-bottom: 2px solid #fafbfb");
        ui.Images_to_PDF_Button->setStyleSheet("border-bottom: 2px solid #fafbfb");
        ui.PDF_to_Images_Button->setStyleSheet("border-bottom: 2px solid #f72585");
    }
    CURRENT_TAB = NONE_STR;
}

void PDFvv::LIST_Button_Clicked()
{
    if (IS_CURRENT_FOLDER == true)
    {
        for (int i = 0; i < FOLDER_Q_LIST.size(); i++)
        {
            if (FOLDER_Q_LIST[i] == CURRENT_FOLDER)
            {
                if (FOLDER_LIST[i][1] == "OPENED")
                {
                    FOLDER_LIST[i][1] = "CLOSED";
                }
                else {
                    FOLDER_LIST[i][1] = "OPENED";
                }
                break;
            }
        }
        UPDATE_FOLDER();
    }
}
void PDFvv::FOLDER_CLICKED()
{
    QPushButton* obj = qobject_cast<QPushButton*>(sender());
    CURRENT_FOLDER = obj;
    IS_CURRENT_FOLDER = true;
}

void PDFvv::FILE_CLICKED()
{
    QPushButton* obj = qobject_cast<QPushButton*>(sender());
    CURRENT_FILE = obj;
    IS_CURRENT_FOLDER = false;

    std::string document_loc;
    for (int i = 0; i < FILE_LIST.size(); i++)
    {
        if (std::filesystem::path(FILE_LIST[i]).stem().string() == obj->text().toStdString())
        {
            document_loc = FILE_LIST[i];
            ui.WEBVIEW->load(QUrl::fromUserInput(QString::fromStdString("file:///" + document_loc)));
            break;
        }
    }
}

void PDFvv::ADD_Folder_Button_Clicked()
{
    FOLDER_LIST.push_back({ "New Folder", "OPENED"});
    UPDATE_FOLDER();
}

void PDFvv::View_Button_Clicked()
{
    CURRENT_TAB = "VIEW";
}

void PDFvv::Images_to_PDF_Button_Clicked()
{
    CURRENT_TAB = "Images_to_PDF";
}

void PDFvv::PDF_to_Images_Button_Clicked()
{
    CURRENT_TAB = "PDF_to_Images";
}
void PDFvv::IMPORT_PDF_Button_Clicked()
{
    QString fname = FILE_DIALOG->getOpenFileName(this, QString::fromStdString("Open File"), QString::fromStdString(""), QString::fromStdString("pdf(*.pdf)"));
    if (fname.size() > 0 && std::find(FILE_LIST.begin(), FILE_LIST.end(), fname.toStdString()) == FILE_LIST.end())
    {
        FOLDER_LIST[0].push_back(fname.toStdString());
        FILE_LIST.push_back(fname.toStdString());
        UPDATE_FOLDER();
    }

}
void PDFvv::UP_Button_Clicked()
{
    if (IS_CURRENT_FOLDER == true)
    {
        for (int i = 2; i < FOLDER_Q_LIST.size(); i++)
        {
            if (FOLDER_Q_LIST[i] == CURRENT_FOLDER)
            {
                std::vector<std::string> temp = FOLDER_LIST[i-1];
                FOLDER_LIST[i - 1] = FOLDER_LIST[i];
                FOLDER_LIST[i] = temp;
                break;
            }
        }
        UPDATE_FOLDER();
    }
    else {
        for (int i = 1; i < FOLDER_Q_LIST.size(); i++)
        {
            for (int j = 2; j < FOLDER_LIST[i].size(); j++)
            {
                if (std::filesystem::path(FOLDER_LIST[i][j]).stem().string() == CURRENT_FILE->text().toStdString())
                {
                    qDebug("%d", i - 1);
                    FOLDER_LIST[i - 1].push_back(FOLDER_LIST[i][j]);
                    FOLDER_LIST[i].erase(FOLDER_LIST[i].begin() + j);
                    break;
                }
            }
        }
        UPDATE_FOLDER();
    }
}
void PDFvv::DOWN_Button_Clicked()
{
    if (IS_CURRENT_FOLDER == true)
    {
        for (int i = 1; i < FOLDER_Q_LIST.size()-1; i++)
        {
            if (FOLDER_Q_LIST[i] == CURRENT_FOLDER)
            {
                std::vector<std::string> temp = FOLDER_LIST[i + 1];
                FOLDER_LIST[i + 1] = FOLDER_LIST[i];
                FOLDER_LIST[i] = temp;
                break;
            }
        }
        UPDATE_FOLDER();
    }
    else {
        for (int i = FOLDER_Q_LIST.size() - 2; i > -1; i--)
        {
            for (int j = 2; j < FOLDER_LIST[i].size(); j++)
            {
                if (std::filesystem::path(FOLDER_LIST[i][j]).stem().string() == CURRENT_FILE->text().toStdString())
                {
                    FOLDER_LIST[i + 1].push_back(FOLDER_LIST[i][j]);
                    FOLDER_LIST[i].erase(FOLDER_LIST[i].begin() + j);
                    break;
                }
            }
        }
        UPDATE_FOLDER();
    }
}
void PDFvv::LINEEDIT_Button_Clicked()
{
    if (IS_CURRENT_FOLDER == true)
    {
        for (int i = 1; i < FOLDER_Q_LIST.size(); i++)
        {
            if (FOLDER_Q_LIST[i] == CURRENT_FOLDER && ui.LineEdit->text().size() > 0)
            {
                FOLDER_LIST[i][0] = ui.LineEdit->text().toStdString();
            }
        }
        UPDATE_FOLDER();
    }
}
void PDFvv::REMOVE_Button_Clicked()
{
    if (IS_CURRENT_FOLDER == true)
    {
        for (int i = 1; i < FOLDER_Q_LIST.size(); i++)
        {
            if (FOLDER_Q_LIST[i] == CURRENT_FOLDER)
            {
                FOLDER_LIST.erase(FOLDER_LIST.begin() + i);
                break;
            }
        }
        UPDATE_FOLDER();
    }
    else
    {
        for (int i = 0; i < FOLDER_Q_LIST.size(); i++)
        {
            for (int j = 2; j < FOLDER_LIST[i].size(); j++)
            {
                if (std::filesystem::path(FOLDER_LIST[i][j]).stem().string() == CURRENT_FILE->text().toStdString())
                {

                    FOLDER_LIST[i].erase(FOLDER_LIST[i].begin() + j);
                    for (int k = 0; k < FILE_LIST.size(); k++)
                    {
                        if (std::filesystem::path(FILE_LIST[k]).stem().string() == CURRENT_FILE->text().toStdString())
                        {
                            FILE_LIST.erase(FILE_LIST.begin() + k);
                            ui.WEBVIEW->setUrl(QUrl("https://cshtarn.github.io/PDFvv/"));
                        }
                        break;
                    }
                }
            }
        }
        UPDATE_FOLDER();
    }
}
void PDFvv::RESET_Button_Clicked()
{
    FOLDER_LIST = { {"Default", "OPENED"}};
    FILE_LIST = {};
    CURRENT_FOLDER = nullptr;
    CURRENT_FILE = nullptr;
    IS_CURRENT_FOLDER = true;
    UPDATE_FOLDER();
}
