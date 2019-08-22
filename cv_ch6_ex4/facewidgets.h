#ifndef FACEWIDGETS_H
#define FACEWIDGETS_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>

class FaceWidget: public QWidget
{
    Q_OBJECT
public:
    FaceWidget(const QString& feature_name,
               const int heimin, const int heimax, const int widmin, const int widmax,
               QWidget* = nullptr);
    int getWidth() const { return this->widthSl->value(); }
    int getHeight() const { return this->heightSl->value(); }
protected:
    QLabel *heightLbl, *widthLbl;
    QSlider *heightSl, *widthSl;
    QHBoxLayout *heightLay, *widthLay;
    QVBoxLayout *mainLay;
protected slots:
    void valueChangedSlot() { emit valuesChangedSig(); }
signals:
    void valuesChangedSig();
};

class IFaceFeathure: public FaceWidget
{
    Q_OBJECT
public:
    IFaceFeathure(const QString& feature_name,
                  const int heimin, const int heimax, const int widmin, const int widmax,
                  const int vposmin, const int vposmax,
                  QWidget* = nullptr);
    int getVPos() const {return this->vPosSl->value(); }
protected:
    QLabel *vPosLbl;
    QSlider *vPosSl;
    QHBoxLayout *vPosLay;
};


#endif // FACEWIDGETS_H
