#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    installer = new Installer(this);

    ui->setupUi(this);
    ui->previousButton->setEnabled((false));

    ui->page_2->setLayout(ui->verticalLayout_5);
    ui->page_3->setLayout(ui->verticalLayout_6);

    auto installation_type = installer->checkInstallationHealth();
    if (installation_type != InstallationHealth::NOT_INSTALLED) {
        ui->nextButton->setEnabled(false);

        ui->fixButton->setEnabled(true);
        ui->uninstallButton->setEnabled(true);
    }

    connect(ui->previousButton, &QPushButton::clicked, this, [this]() {
        int i = ui->stackedWidget->currentIndex();
        ui->stackedWidget->setCurrentIndex((i - 1));

        ui->nextButton->setText("Next");

        ui->nextButton->setEnabled(true);
        if (i - 1 <= 0)
            ui->previousButton->setEnabled(false);
    });

    connect(ui->nextButton, &QPushButton::clicked, this, [this]() {
        int i = ui->stackedWidget->currentIndex();
        ui->stackedWidget->setCurrentIndex(i + 1);

        ui->nextButton->setText("Next");

        if (i + 1 >= ui->stackedWidget->count() - 1)
            ui->nextButton->setEnabled(false);
        ui->previousButton->setEnabled(true);

        if (i + 1 == 1) {
            ui->nextButton->setEnabled(ui->checkBox->isChecked());
            ui->nextButton->setText("Install");
        }

        if (i + 1 == 2) {
            ui->previousButton->setEnabled(false);
            ui->nextButton->setEnabled(false);
            connect(installer, &Installer::finished,
                    this, [this](bool success)
                    {
                        ui->nextButton->setEnabled(true);

                        if (success) {
                            ui->nextButton->setText("Finish");
                            ui->installLabel->setText("Successful Installation");
                        } else {
                            ui->nextButton->setText("Finish");
                            ui->installLabel->setText("Unsuccessful Installation");
                        }
                    });
            installer->start();
        }

        if (i + 1 == 3 || i + 1 == 4) {
            QCoreApplication::quit();
        }
    });

    connect(ui->uninstallButton, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex((3));
        connect(installer, &Installer::finished,
                this, [this](bool success)
                {
                    ui->nextButton->setEnabled(true);

                    if (success) {
                        ui->nextButton->setText("Finish");
                        ui->uninstallLabel->setText("Successful Uninstalled");
                    } else {
                        ui->nextButton->setText("Finish");
                        ui->uninstallLabel->setText("Could not uninstall");
                    }
                });
        installer->uninstall();
    });

    /*
        SURPRISE!
        Fix mode is literally the same of install
        The unique difference is the aspect
        Just to make it look more useful than it is...
        And I could even do the same with uninstall mode...
    */
    connect(ui->fixButton, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex((2));
        ui->installLabel->setText("Fixing...");
        connect(installer, &Installer::finished,
                this, [this](bool success)
                {
                    ui->nextButton->setEnabled(true);

                    if (success) {
                        ui->nextButton->setText("Finish");
                        ui->installLabel->setText("Successful Fixed");
                    } else {
                        ui->nextButton->setText("Finish");
                        ui->installLabel->setText("Unsuccessful Fixed. Manual intervention is required");
                    }
                });
        installer->start();
    });

    connect(ui->checkBox, &QCheckBox::clicked, this, [this]() {
        ui->nextButton->setEnabled(ui->checkBox->isChecked());
    });
}

MainWindow::~MainWindow() {
    delete ui;
}
