#include "img_provider.h"
#include <QDebug>
void img_provider::ChangeImg(const QImage img)
{

    delete  image;

    image = new QImage(img);

}

QImage img_provider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{

    qDebug()<<Q_FUNC_INFO<<"request : "<<id<<" size:"<<requestedSize;
    Q_UNUSED(id)
    Q_UNUSED(requestedSize)
  //  *size = image->size();
    return *image;
}
