#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QStringList>
#include <QGraphicsScene>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int GetRecordingTime();
    void Message_toTranscript();
    void MessageBoxError(QString message);
    const char* QStringtoChar(QString qs);
    void Display_image(QString lettre, bool choice);
    void Record_sequence();
    bool startsWith(const char *pre, const char *str);
    void Display_Recording_Labels();
    void SetEnabled_ui(bool state);
    void SendStrToOpenCR();




private slots:

    void on_Start_traduction_pressed();
    void on_See_charac_returnPressed();
    void on_transcript_charac_returnPressed();
    void on_Start_record_button_clicked();
    void Timer();


public slots:







private:
    Ui::MainWindow *ui;
    int Recording_increment;
    int time_;
    int Letter_increment;
    QString OutputSpeech;
    QString OutputError;
    QString OutputOpenCR;
    QString trad_string;
    QStringList OutputSpeechList;
    QGraphicsScene *scene;
    QPixmap image;
    QThread *dis_im;
    //QTimer *timer_;
    QString Disp_letter;
};

#endif // MAINWINDOW_H
