#include "pdfvv.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PDFvv w;
    w.show();
    return a.exec();
}
