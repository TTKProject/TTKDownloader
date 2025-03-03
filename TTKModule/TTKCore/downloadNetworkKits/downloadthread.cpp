#include "downloadthread.h"
#include "downloadabstractnetwork.h"

#include <QFile>

DownloadThread::DownloadThread(QObject *parent)
    : QObject(parent),
      m_state(State::Stop)
{
    m_manager = new QNetworkAccessManager(this);
}

DownloadThread::~DownloadThread()
{
    delete m_manager;
}

void DownloadThread::startDownload(int index, const QString &url, QFile *file,
                                   qint64 startPoint, qint64 endPoint,
                                   qint64 readySize)
{
    if(m_state == State::Download)
    {
        Q_EMIT errorCode(m_index, "is downloading a file");
        return;
    }

    m_index = index;
    m_url = url;
    m_file = file;
    m_startPoint = startPoint;
    m_endPoint = endPoint;
    m_readySize = readySize;

    QNetworkRequest request;
    request.setUrl(m_url);
    QString range = QString("bytes=%0-%1").arg(m_startPoint + m_readySize).arg(m_endPoint);
    request.setRawHeader("Range", range.toUtf8());
    TTK::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(readyRead()), SLOT(handleReadyRead()));
    QtNetworkErrorConnect(m_reply, this, handleError, TTK_SLOT);

    m_state = State::Download;
    Q_EMIT downloadChanged();
}

void DownloadThread::pause()
{
    if(m_state != State::Download)
    {
        Q_EMIT errorCode(m_index, "is not downloading");
        return;
    }

    m_state = State::Pause;
    m_reply->abort();
    m_file->flush();
    m_reply->deleteLater();
}

void DownloadThread::restart()
{
    if(m_state != State::Pause)
    {
        Q_EMIT errorCode(m_index, "is not stoped");
        return;
    }

    startDownload(m_index, m_url, m_file, m_startPoint, m_endPoint, m_readySize);
}

void DownloadThread::downLoadFinished()
{
    if(m_state != State::Download)
    {
        return;
    }

    m_file->flush();
    m_reply->deleteLater();
    m_state = State::Finished;

    Q_EMIT finished(m_index);
}

void DownloadThread::handleReadyRead()
{
    const QByteArray &buffer = m_reply->readAll();
    m_file->seek(m_startPoint + m_readySize);
    m_file->write(buffer);
    m_readySize += buffer.length();

    Q_EMIT downloadChanged();
}

void DownloadThread::handleError(QNetworkReply::NetworkError code)
{
    if(m_state != State::Download)
    {
        return;
    }
    Q_EMIT errorCode(m_index, "QNetworkReply::NetworkError : " + QString::number(TTKStaticCast(int, code)) + " \n" + m_reply->errorString());

    m_state = State::Stop;
    m_reply->abort();
    m_file->flush();
    m_reply->deleteLater();
}
