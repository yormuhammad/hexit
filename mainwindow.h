#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void onBtnClearClick();
    void onBtnClearNonHexClick();
    void onBtnConvertFromCClick();
    void onBtnCopyAsCClick();
    void onBtnCopyAsJClick();
    void onBtnCopyAsB64Click();
    void onBtnPasteClick();
    void onBtnCloseClick();
    void onComboColumnCountChanged(const QString &text);
private:
    Ui::MainWindow *ui;
    QClipboard *mClipboard;
    int mColCount;
    static bool isHexChar(const QChar &c);
    static QString clearNonHex(const QString &text);
};

#endif // MAINWINDOW_H
