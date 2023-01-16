#include "label.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"

#include <QLabel>
#include <QDebug>


Label::Label()
{

}

Label::~Label()
{

}

void Label::Set_label_visible(QLabel *label)
{
   label->setVisible(true);
}

void Label::Set_label_invisible(QLabel *label)
{
   label->setVisible(false);
}



