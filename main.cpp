#include "mainwindow.h"

#include <QApplication>
void setMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QString infoHtml = "<font color=\"Lime\">";
    QString alertHtml = "<font color=\"DeepPink\">";
    QString criticalHtml = "<font color=\"Red\">";
    QString debugHtml = "<font color=\"Aqua\">";
    QString endHtml = "</font>";
    QString infoMsg = msg;
//    QString time = QDateTime::currentDateTime().toString("hh.mm.ss.z ");
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtInfoMsg:
        fprintf(stdout, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtDebugMsg:
        fprintf(stdout, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(stdout, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        abort();
    }
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(setMessageOutput);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
