#include <QCoreApplication>
#include <QMutex>
#include <QFile>
#include <QDateTime>
#include <QDir>
#include <QTextStream>
#include <QDebug>
void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();

    QString  currentDateTimeString = QDateTime::currentDateTime().toLocalTime().toString(Qt::DefaultLocaleLongDate);
    QString ctxInfo = context.file + QLatin1Char(':') + QString::number(context.line) + QLatin1Char(':') + context.function;
    QString message = currentDateTimeString + QLatin1Char(' ')+ msg + QLatin1Char('[') + ctxInfo + QLatin1Char(']');

    static QString localDirPath = QDir::toNativeSeparators(qApp->applicationDirPath())/*QDir::separator() + QLatin1String("log")*/;
    static QString fileName = localDirPath + QDir::separator() + qApp->applicationName();

    QFile file(fileName);
    if (file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append))
    {
        if (file.size() >= Q_INT64_C(10240 * 128))//1M*128
            file.resize(0);

        QTextStream in(&file);
        in << message << endl;
    }
    mutex.unlock();

}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qInstallMessageHandler(messageOutput);
    qDebug() <<"something is happen";
    return a.exec();
}
