#include "AlgorithmDisplay.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    AlgorithmDisplay w;
    w.show();
    return app.exec();
}
