#ifndef LABEL_H
#define LABEL_H

#include <QLabel>
#include <mainwindow.h>
#include <QMainWindow>

namespace Ui {
class Label;
}

class Label : public MainWindow
{
    Q_OBJECT

public:
    explicit Label();
    ~Label();
    void Set_label_visible(QLabel *label);
    void Set_label_invisible(QLabel *label);

};

#endif // LABEL_H
