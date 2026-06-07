#pragma once

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QFile>
#include <QProcess>
#include <QDir>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

/*
    It is used after intallation (to check if the installation process was ok)
    And to fix/uninstall
*/
typedef enum {
    RIGHT,
    NOT_INSTALLED,
    WRONG // This only happens if there is a missing file or something like that
} InstallationHealth;

class Installer : public QObject
{
    Q_OBJECT

public:
    Installer(QObject *parent = nullptr) {}

    void start();
    InstallationHealth checkInstallationHealth();
    void uninstall();

signals:
    void finished(bool success);

private slots:
    void onGithubReply();
    void onDownloadFinished();

private:
    QNetworkAccessManager *manager;
    QString downloadUrl;
    QString fileName = "yasos.tar.gz";

    void downloadFile(const QString &url);
    void extractAndInstall();
};