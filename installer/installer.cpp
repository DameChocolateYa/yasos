#include "installer.h"

void Installer::start() {
    manager = new QNetworkAccessManager(this);

    QUrl url("https://api.github.com/repos/DameChocolateYa/yasos/releases/latest");
    QNetworkRequest request(url);

    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished,
            this, &Installer::onGithubReply);
}

void Installer::onGithubReply() {
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    QByteArray data = reply->readAll();
    reply->deleteLater();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    QJsonArray assets = obj["assets"].toArray();

    for (const QJsonValue &v : assets)
    {
        QJsonObject asset = v.toObject();
        QString name = asset["name"].toString();

        if (name.startsWith("yasos-") &&
            name.contains("x86_64") &&
            name.endsWith(".tar.gz"))
        {
            downloadUrl = asset["browser_download_url"].toString();
            break;
        }
    }

    if (downloadUrl.isEmpty())
    {
        qDebug() << "Compatible release not found";
        return;
    }

    downloadFile(downloadUrl);
}

void Installer::downloadFile(const QString &url) {
    QNetworkReply *reply =
        manager->get(QNetworkRequest(QUrl(url)));

    QFile *file = new QFile(fileName, this);

    if (!file->open(QIODevice::WriteOnly)) {
        emit finished(false);
        return;
    }

    connect(reply, &QNetworkReply::readyRead, this, [=]() {
        file->write(reply->readAll());
    });

    connect(reply, &QNetworkReply::finished, this, [=]() {
        file->close();
        reply->deleteLater();

        extractAndInstall();
    });
}

void Installer::extractAndInstall() {
    QDir().mkpath("yasos-temp");
    int result = QProcess::execute("tar", {
                                "-xzf",
                                 fileName,
                                 "-C",
                                 "yasos-temp/"
                             });

    if (result != 0)
    {
        qDebug() << "Extraction failed";
        return;
    }

    QString tempDir = QFileInfo("yasos-temp").absoluteFilePath();
    QString script = QDir(tempDir).filePath("install.sh");

    QString command = QString("cd '%1' && bash '%2' --install")
                          .arg(tempDir, script);

    QProcess *proc = new QProcess(this);
    connect(proc, &QProcess::finished, this,
        [this, proc](int exitCode, QProcess::ExitStatus status)
        {
        proc->deleteLater();

        QDir dir("yasos-temp");
        dir.removeRecursively();
        QFile::remove("yasos.tar.gz");

        bool ok = (status == QProcess::NormalExit && exitCode == 0);
        emit finished(ok);
        });

    proc->start("pkexec", {
                                          "bash",
                                          "-c",
                                          command
                                      });

    qDebug() << "Installation started";
}

void Installer::onDownloadFinished() {

}

InstallationHealth Installer::checkInstallationHealth() {
    QDir binaries_dir("/usr/bin/");
    QDir headers_dir("/usr/include/");
    QDir libraries_dir("/usr/lib/");
    QDir share_dir("/usr/share/");

    bool is_yasos_binary = binaries_dir.exists("yasos");
    bool are_yasos_headers = headers_dir.exists("ysinclude/");
    bool is_yasos_library = libraries_dir.exists("yslib/");

    int op = is_yasos_binary + are_yasos_headers + is_yasos_library;
    return op == 3 ? InstallationHealth::RIGHT : op == 0 ? InstallationHealth::NOT_INSTALLED : InstallationHealth::WRONG;
}

void Installer::uninstall() {
    if (checkInstallationHealth() == InstallationHealth::NOT_INSTALLED) return;

    QProcess *proc = new QProcess(this);
    connect(proc, &QProcess::finished, this,
            [this, proc](int exitCode, QProcess::ExitStatus status)
            {
                proc->deleteLater();

                // ...

                bool ok = (status == QProcess::NormalExit && exitCode == 0);
                emit finished(ok);
            });

    QString command = QString("/usr/share/yasos/install.sh --remove");
    proc->start("pkexec", {
                              "bash",
                              "-c",
                              command
                          });

    qDebug() << "Installation started";

}