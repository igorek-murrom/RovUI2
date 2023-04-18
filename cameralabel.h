#ifndef CAMERALABEL_H
#define CAMERALABEL_H

#include <QLabel>
#include <QObject>
#include <QWidget>
/*!
 * \brief The CameraLabel class is a helper class used to simplify resizing the
 * camera
 */
class CameraLabel : public QLabel {
    Q_OBJECT
  public:
    /*!
     * \brief Default constructor
     * \param parent Parent
     */
    CameraLabel(QWidget *parent = nullptr);

    /*!
     * \brief Modified QLabel::heightForWidth() function to help resizing the
     * label \param w Width \return Height
     */
    int heightForWidth(int w) const;

    int widthForHeight(int w) const;
};

#endif // CAMERALABEL_H
