/****************************************************************************
**
** Copyright (C) 2015-2016 Oleg Shparber
** Copyright (C) 2013-2014 Jerzy Kozera
** Contact: https://go.zealdocs.org/l/contact
**
** This file is part of Zeal.
**
** Zeal is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Zeal is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with Zeal. If not, see <https://www.gnu.org/licenses/>.
**
****************************************************************************/

#ifndef DOCSETSDIALOG_H
#define DOCSETSDIALOG_H

#include <registry/docsetmetadata.h>

#include <QDialog>
#include <QHash>
#include <QMap>

class QListWidgetItem;
class QNetworkReply;
class QTemporaryFile;
class QUrl;

namespace Ui {
class DocsetsDialog;
}

namespace Zeal {

namespace Registry {
class DocsetRegistry;
} // namespace Registry

namespace Core {
class Application;
}

class DocsetsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DocsetsDialog(Core::Application *app, QWidget *parent = nullptr);
    ~DocsetsDialog() override;

    void reject() override;

private slots:
    void addDashFeed();
    void updateSelectedDocsets();
    void updateAllDocsets();
    void removeSelectedDocsets();
    void updateDocsetFilter(const QString &filterString);

    void downloadSelectedDocsets();

    void downloadCompleted();
    void downloadProgress(qint64 received, qint64 total);

    void extractionCompleted(const QString &filePath);
    void extractionError(const QString &filePath, const QString &errorString);
    void extractionProgress(const QString &filePath, qint64 extracted, qint64 total);

    void loadDocsetList();

private:
    enum DownloadType {
        DownloadDashFeed,
        DownloadDocset,
        DownloadDocsetList
    };

    Ui::DocsetsDialog *ui = nullptr;
    Core::Application *m_application = nullptr;
    Registry::DocsetRegistry *m_docsetRegistry = nullptr;

    QList<QNetworkReply *> m_replies;
    qint64 m_combinedTotal = 0;
    qint64 m_combinedReceived = 0;

    // TODO: Create a special model
    QMap<QString, Registry::DocsetMetadata> m_availableDocsets;
    QMap<QString, Registry::DocsetMetadata> m_userFeeds;

    QHash<QString, QTemporaryFile *> m_tmpFiles;
    QStringList m_docsetsBeingDeleted;

    QListWidgetItem *findDocsetListItem(const QString &name) const;
    bool updatesAvailable() const;

    QNetworkReply *download(const QUrl &url);
    void cancelDownloads();

    void downloadDocsetList();
    void processDocsetList(const QJsonArray &list);

    void downloadDashDocset(const QModelIndex &index);
    void removeDocset(const QString &name);

    void updateCombinedProgress();
    void resetProgress();

    // FIXME: Come up with a better approach
    QString docsetNameForTmpFilePath(const QString &filePath) const;

    static inline int percent(qint64 fraction, qint64 total);

    static QString cacheLocation(const QString &fileName);
};

} // namespace Zeal

#endif // DOCSETSDIALOG_H
