 #include "mainwindow.h"
#include "ui_mainwindow.h"
#include "label.h"

#include <QPushButton>
#include <QMessageBox>
#include <QLabel>
#include <QProcess>
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QGraphicsScene>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <QThread>

std::thread com_;
std::mutex mutex_disp;
std::mutex mutex_letter;
std::condition_variable cv_;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Phrase_recorded->setText(" ");
    ui->Recording_state->setText(" "); //set labels to a space in initialisation

    Recording_increment = 1;
    Letter_increment = 1;

    //timer_ = new QTimer(this);
    dis_im = new QThread(this);
    connect(dis_im, &QThread::started, this, &MainWindow::Timer); //Timer to show images at the same time as the real hand
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene; //scene created in Display_image()
    delete dis_im; //thread created to display images
}

void MainWindow::SendStrToOpenCR() //Function sends string to openCR and plots the right character in interface
{
    if (trad_string != NULL)
    {
        QString path = "/home/pi/env/SLIRUS_interface/FindPort.py";
        QProcess *process = new QProcess();
        connect(process, &QProcess::readyReadStandardOutput, [process, this]()
        {
            QString output = process->readAllStandardOutput();
            OutputOpenCR = output;
            qDebug() << "output: " << output;
            if(output.contains("Interface:start"))
            {
                QString qs = output;
                qs.remove(0,16);
                qs.remove(1,qs.length());
                mutex_letter.lock();
                trad_string = qs;
                mutex_letter.unlock();
                dis_im->start();
            }
            if(output.contains("IM DONE"))
            {
                mutex_letter.lock();
                trad_string = "";
                mutex_letter.unlock();
                dis_im->start();
                //Display_image("", false);
            }

        });

        connect(process, &QProcess::readyReadStandardError, [process, this]()
        {
            QString err = process->readAllStandardError();
            qDebug() << "error: " << err;
            OutputError = err;
        });

        connect(process, &QProcess::started, [process, this]()->void
        {
            process->write((trad_string + QString("\n")).toLatin1());
        });
        process->start("/usr/bin/python3", QStringList() << path);

        process->waitForFinished();
        process->close();
        SetEnabled_ui(true);
    }
    else
    {
        MainWindow::MessageBoxError("No phrase was recorded");
    }
}

void MainWindow::Record_sequence() //Function that records the speech-to-text
{
    int Record_time = MainWindow::GetRecordingTime();

    if (Record_time == -1)
        QCoreApplication::exit();

    else {
        //Commencement de l'enregistrement
        QString path = "/home/pi/Documents/Projets_S4/Enregistrement.py";
        //Modify previous line for a different system
        QProcess *process = new QProcess();
        connect(process, &QProcess::readyReadStandardOutput, [process, this]()
        {
            QString output = process->readAllStandardOutput();
            OutputSpeech = output;
            qDebug() << "output: " << output;
            if (Recording_increment != -1)
            {
                Display_Recording_Labels();
                Recording_increment = Recording_increment - 1;
            }

        });

        connect(process, &QProcess::readyReadStandardError, [process, this]()
        {
            QString err = process->readAllStandardError();
            qDebug() << "error: " << err;
            OutputError = err;
        });

        connect(process, &QProcess::started, [process, this]()->void
        {
            char numberstring[(((sizeof time_) * CHAR_BIT) + 2)/3 + 2];
            sprintf(numberstring, "%d", time_);
            process->write((QString(numberstring) + QString("\n")).toLatin1());
        });
        process->start("/usr/bin/python3", QStringList() << path);

        process->waitForFinished();
        process->close();
        SetEnabled_ui(true);
        Message_toTranscript();
    }


}

int MainWindow::GetRecordingTime() //Gets time from the text line edit in the interface. Base time is 5 seconds
{
    if (ui->Recording_time->text().toInt() > 0)
    {
        time_ = ui->Recording_time->text().toInt();
        return ui->Recording_time->text().toInt();
    }
    else if (ui->Recording_time->text().isEmpty())
    {
        time_ = 5;
        return 5;
    }
    else
    {
        MainWindow::MessageBoxError("You did not write an appropriate recording time");
        time_ = -1;
        return -1;
    }
}


void MainWindow::Message_toTranscript() //Separates the receiving string to keep only the real message
{
    Display_Recording_Labels();
    if (OutputError != NULL)
    {
        com_.join();
        MainWindow::MessageBoxError("No words were recorded");
    }
    else
    {
        OutputSpeechList = OutputSpeech.split(QRegExp("\\b"));
        int index = OutputSpeechList.QStringList::lastIndexOf(QString("\n"),-2);
        OutputSpeech.clear();
        for (int i=0;i<=index;i++)
        {
            OutputSpeechList.QStringList::removeFirst();
        }
        OutputSpeechList.QStringList::removeLast();
        OutputSpeech = OutputSpeechList.QStringList::join("");
        ui->Phrase_recorded->setText(OutputSpeech);
        qDebug() << OutputSpeech << endl;
        Recording_increment = 1;
    }
}

void MainWindow::Display_image(QString lettre, bool choice)
{
    if (lettre == "")
        scene->clear();
        
    qDebug() << "lettre a display: " << lettre << endl;
    //Displays the image related to the character in the QGraphicView
    mutex_disp.lock();
    if (choice)
    {
        QString qstr_temp = "/home/pi/env/SLIRUS_interface/Images/";
        qstr_temp.append(lettre);
        qstr_temp.append(".png");
        image.load(qstr_temp);
        image.scaled(400,400);
        scene = new QGraphicsScene(this);
        scene->addPixmap(image);
        scene->setSceneRect(image.rect());
        ui->MainWindow::mainImage->setScene(scene);
    }
    else
        scene->clear();
    mutex_disp.unlock();
    if (dis_im->isRunning())
            dis_im->quit();
}

void MainWindow::MessageBoxError(QString message)
{
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.exec();
}

void MainWindow::on_Start_traduction_pressed() //Button to start traduction
{
    if (com_.joinable())
        com_.join();
    SetEnabled_ui(false);
    mutex_letter.lock();
    trad_string = OutputSpeech;
    mutex_letter.unlock();
    //timer_->start();

    com_ = std::thread(&MainWindow::SendStrToOpenCR,this);
    //timer_->start(3000);
}

void MainWindow::on_See_charac_returnPressed() //when somebody press "enter" to see a specific character on screen
{
    QString str = "all";
    if(ui->See_charac->text() == str)
        MainWindow::Display_image("Complete_image", true);
    else if (ui->See_charac->text().length() > 1)
        MainWindow::MessageBoxError("The line has more than one character");
    else
        MainWindow::Display_image(ui->See_charac->text(), true);
}

const char* MainWindow::QStringtoChar(QString qs)
{
    std::string utf8_text = qs.toUtf8().constData();
    const char* temp_char = utf8_text.c_str();
    return temp_char;
}

void MainWindow::on_transcript_charac_returnPressed() //when escape is pressed to transcript only certain characters
{
    SetEnabled_ui(false);
    if (com_.joinable())
        com_.join();
    trad_string = ui->transcript_charac->text();
    com_ = std::thread(&MainWindow::SendStrToOpenCR,this);
    //timer_->start(4000);
}

bool MainWindow::startsWith(const char *pre, const char *str)
{
    size_t lenpre = strlen(pre);
    size_t lenstr = strlen(str);
    return lenstr < lenpre ? false : memcmp(pre,str,lenpre) == 0;
}

void MainWindow::Display_Recording_Labels() //displays a label when the system is ready to record
{
    if (MainWindow::Recording_increment == 1)
         ui->Recording_state->setText("Now Recording...");

     else if (MainWindow::Recording_increment == 0)
         ui->Recording_state->setText("Stopped recording");

    else if (MainWindow::Recording_increment == -1)
        ui->Recording_state->setText(" ");
}

void MainWindow::on_Start_record_button_clicked() //this function is called when the user is ready to use the speech-to-text
{
    SetEnabled_ui(false);
    OutputSpeech = "";
    if (com_.joinable())
        com_.join();

    com_ = std::thread(&MainWindow::Record_sequence,this);
}

void MainWindow::SetEnabled_ui(bool state)
{
    ui->Start_traduction->setEnabled(state);
    ui->See_charac->setEnabled(state);
    ui->Recording_time->setEnabled(state);
    ui->transcript_charac->setEnabled(state);
    ui->transcript_charac->setEnabled(state);
    ui->Start_record_button->setEnabled(state);
}


void MainWindow::Timer() //Called by the timer... checks which letter needs to be displayed
{
    mutex_letter.lock();
    QString qstemp = trad_string;
    mutex_letter.unlock();
    MainWindow::Display_image(qstemp,true);
}
