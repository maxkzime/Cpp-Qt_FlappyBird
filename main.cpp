#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}

/* To do :
 * Scoreboard menu
 * Scoreboard scrapping in a file
 * Saving best score in a file
 *
 *
 *
 *
 *
 */
