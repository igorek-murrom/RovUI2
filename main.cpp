#include "mainwindow.h"

#include <QApplication>

/**
 * \brief Me
 * \param type Type of message
 * \param context Mesffs redo thissage context
 * \param msg Message
 */
void RovUIMessageHandler(QtMsgType type, const QMessageLogContext &context,
                         const QString &msg) {
    QByteArray  localMsg = msg.toLocal8Bit();
    const char *file     = context.file ? context.file : "";
    const char *function = context.function ? context.function : "";
    switch (type) {
    case QtDebugMsg:
        fprintf(stdout, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), file,
                context.line, function);
        break;
    case QtInfoMsg:
        fprintf(stdout, "Info: %s (%s:%u, %s)\n", localMsg.constData(), file,
                context.line, function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), file,
                context.line, function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(),
                file, context.line, function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), file,
                context.line, function);
        break;
    }
}

int main(int argc, char *argv[]) {
    qInstallMessageHandler(RovUIMessageHandler);

    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    return a.exec();
}
