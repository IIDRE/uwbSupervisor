#ifndef IMG_PROVIDER_H
#define IMG_PROVIDER_H

#include <QQuickImageProvider>
#include <QByteArray>


class img_provider:public QQuickImageProvider
{


    QImage *image;

signals:
    void imgNameChanged(QString coord);
public:

    img_provider():QQuickImageProvider(QQuickImageProvider::Image),image(nullptr){
        image = new QImage(":/images/defImgMap.png");
    }
    void ChangeImg(const QImage img);


    // QQuickImageProvider interface
public:
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
};

#endif // IMG_PROVIDER_H
