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
    QByteArray localMsg = msg.toLocal8Bit();
#ifdef __QTDEBUG__
    const char *file     = context.file ? context.file : "";
    const char *function = context.function ? context.function : "";
#endif
    switch (type) {
    case QtDebugMsg:
#ifdef __QTDEBUG__
        fprintf(stdout, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), file,
                context.line, function);
#else
        fprintf(stdout, "Debug: %s\n", localMsg.constData());
#endif
        break;
    case QtInfoMsg:
#ifdef __QTDEBUG__
        fprintf(stdout, "Info: %s (%s:%u, %s)\n", localMsg.constData(), file,
                context.line, function);
#else
        fprintf(stdout, "Info: %s\n", localMsg.constData());
#endif
        break;
    case QtWarningMsg:
#ifdef __QTDEBUG__
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), file,
                context.line, function);
#else
        fprintf(stderr, "Warning: %s\n", localMsg.constData());
#endif
        break;
    case QtCriticalMsg:
#ifdef __QTDEBUG__
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(),
                file, context.line, function);
#else
        fprintf(stderr, "Critical: %s\n", localMsg.constData());
#endif
        break;
    case QtFatalMsg:
#ifdef __QTDEBUG__
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), file,
                context.line, function);
#else
        fprintf(stderr, "Fatal: %s\n", localMsg.constData());
#endif
        break;
    }
}

int main(int argc, char *argv[]) {
    qInstallMessageHandler(RovUIMessageHandler);
    setenv("QT_QPA_PLATFORM","xcb",1);

    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    return a.exec();
}
