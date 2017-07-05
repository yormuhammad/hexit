#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QClipboard>
#include <QMimeData>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mColCount(8)
{
    ui->setupUi(this);
    setWindowTitle("Hexit (2016, Ermat E. Kiyomov)");
    connect(ui->btnClear, SIGNAL(clicked()), this, SLOT(onBtnClearClick()));
    connect(ui->btnClearNonHex, SIGNAL(clicked()), this, SLOT(onBtnClearNonHexClick()));
    connect(ui->btnConvertFromC, SIGNAL(clicked()), this, SLOT(onBtnConvertFromCClick()));
    connect(ui->btnCopyAsC, SIGNAL(clicked()), this, SLOT(onBtnCopyAsCClick()));
    connect(ui->btnCopyAsJ, SIGNAL(clicked()), this, SLOT(onBtnCopyAsJClick()));
    connect(ui->btnCopyAsB64, SIGNAL(clicked()), this, SLOT(onBtnCopyAsB64Click()));
    connect(ui->btnPaste, SIGNAL(clicked()), this, SLOT(onBtnPasteClick()));
    connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(onBtnCloseClick()));
    mClipboard = QApplication::clipboard();
//    ui->editHex->setPlainText("5E E4 B6 35 3B E6 19 47 3D B5 D5 6D 2B 56 1C 96 1D 07 48 A7 4C D5 5C 41 9E 7A F1 55 7D 12 6A B0");
//    onBtnClearNonHexClick();
    ui->comboColumnCount->addItem("8");
    ui->comboColumnCount->addItem("12");
    ui->comboColumnCount->addItem("16");
    ui->comboColumnCount->addItem("24");
    ui->comboColumnCount->addItem("32");
    ui->comboColumnCount->setCurrentIndex(0);
    connect(ui->comboColumnCount, SIGNAL(currentIndexChanged(QString)), this, SLOT(onComboColumnCountChanged(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onBtnClearClick()
{
    ui->editHex->clear();
}

void MainWindow::onBtnClearNonHexClick()
{
    ui->editHex->setPlainText(clearNonHex(ui->editHex->toPlainText()).toLower());
}

void MainWindow::onBtnConvertFromCClick()
{
    QString s = ui->editHex->toPlainText();
    QString t = s.replace(QRegExp("unsigned char \\w+"), "").replace(QRegExp("\\[\\d+\\]"), "").replace(QRegExp("0[xX]"), "");
    ui->editHex->setPlainText(clearNonHex(t).toLower());
}

void MainWindow::onBtnCopyAsCClick()
{
    onBtnClearNonHexClick();
    QString s = ui->editHex->toPlainText();
    int len = s.length() / 2;
    QString t = QString("unsigned char raw[%1] = {\n").arg(len);
    for (int i = 0; i < len; ++i) {
        if (i % mColCount == 0)
            t += "    ";
        t += "0x" + s[i*2] + s[i*2+1];
        if (i < len - 1){
            t += ", ";
            if (i > 0 && (i % mColCount == mColCount-1)){
                t.remove(t.length()-1, 1);
                t += "\n";
            }
        }
    }
    if (t[t.length()-1] != '\n')
        t += "\n";
    t += "};\n";
    mClipboard->setText(t);
}

void MainWindow::onBtnCopyAsJClick()
{
    onBtnClearNonHexClick();
    QString s = ui->editHex->toPlainText();
    int len = s.length() / 2;
    QString t = QString("byte raw[] = {\n");
    for (int i = 0; i < len; ++i) {
        if (i % mColCount == 0)
            t += "    ";
        t += "(byte)0x" + s[i*2] + s[i*2+1];
        if (i < len - 1){
            t += ", ";
            if (i > 0 && (i % mColCount == mColCount-1)){
                t.remove(t.length()-1, 1);
                t += "\n";
            }
        }
    }
    if (t[t.length()-1] != '\n')
        t += "\n";
    t += "};\n";
    mClipboard->setText(t);
}

void MainWindow::onBtnCopyAsB64Click()
{
    onBtnClearNonHexClick();
    QString s = ui->editHex->toPlainText();
    QString t = QByteArray::fromHex(s.toLatin1()).toBase64();
    mClipboard->setText(t);
}

void MainWindow::onBtnPasteClick()
{
    if (mClipboard->mimeData()->hasText()){
        ui->editHex->paste();
    }
}

void MainWindow::onBtnCloseClick()
{
    close();
}

void MainWindow::onComboColumnCountChanged(const QString &text)
{
    mColCount = text.toInt();
}

bool MainWindow::isHexChar(const QChar &c)
{
    QChar lower = c.toLower();
    return c.isDigit() || (lower >= 'a' && lower <= 'f');
}

QString MainWindow::clearNonHex(const QString &text)
{
    QString t = "";
    for (int i = 0; i < text.length(); ++i) {
        if (isHexChar(text[i]))
            t += text[i];
    }
    if (t.length() % 2 == 1)
        t = '0' + t;
    return t;
}
