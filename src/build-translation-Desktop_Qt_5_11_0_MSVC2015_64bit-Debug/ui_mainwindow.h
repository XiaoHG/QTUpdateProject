/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLabel *label;
    QPushButton *pushButton;
    QLabel *label_2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(426, 376);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 10, 261, 221));
        label->setStyleSheet(QStringLiteral("font: 14pt \"Sans Serif\";"));
        label->setScaledContents(false);
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(240, 290, 99, 22));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(30, 270, 161, 21));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 426, 23));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QApplication::translate("MainWindow", "        \343\200\212\345\205\263\345\261\261\346\234\210\343\200\213\n"
"\n"
"\346\230\216\346\234\210\345\207\272\345\244\251\345\261\261\357\274\214\350\213\215\350\214\253\344\272\221\346\265\267\351\227\264\343\200\202\n"
"\351\225\277\351\243\216\345\207\240\344\270\207\351\207\214\357\274\214\345\220\271\345\272\246\347\216\211\351\227\250\345\205\263\343\200\202\n"
"\346\261\211\344\270\213\347\231\275\347\231\273\351\201\223\357\274\214\350\203\241\347\252\245\351\235\222\346\265\267\346\271\276\343\200\202\n"
"\"\347\224\261\346\235\245\345\276\201\346\210\230\345\234\260\357\274\214\344\270\215\350\247\201\346\234\211\344\272\272\350\277\230\343\200\202\n"
"\346\210\215\345\256\242\346\234\233\350\276\271\351\202\221\357\274\214\346\200\235\345\275\222\345\244\232\350\213\246\351\242\234\343\200\202\n"
"\351\253\230\346\245\274\345\275\223\346\255\244\345\244\234\357\274\214\345\217\271\346\201\257\346\234\252\345\272\224\351\227\262\343\200\202", nullptr));
        pushButton->setText(QApplication::translate("MainWindow", "\347\277\273\350\257\221", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "\344\273\212\345\244\251\346\230\257\344\270\252\345\245\275\346\227\245\345\255\220", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
