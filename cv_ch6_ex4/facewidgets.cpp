#include "facewidgets.h"

FaceWidget::FaceWidget(const QString& feature_name,
                       const int heimin, const int heimax, const int widmin, const int widmax,
                       QWidget*)
{
    heightLbl = new QLabel(feature_name + " height");
    heightSl = new QSlider(Qt::Horizontal);
        heightSl->setMinimum(heimin);
        heightSl->setMaximum(heimax);
    heightLay = new QHBoxLayout();
        heightLay->addWidget(heightLbl);
        heightLay->addWidget(heightSl);

    widthLbl = new QLabel(feature_name +" width");
    widthSl = new QSlider(Qt::Horizontal);
        widthSl->setMinimum(widmin);
        widthSl->setMaximum(widmax);
    widthLay = new QHBoxLayout();
        widthLay->addWidget(widthLbl);
        widthLay->addWidget(widthSl);

    mainLay = new QVBoxLayout(this);
        mainLay->addLayout(heightLay);
        mainLay->addLayout(widthLay);

    connect(heightSl, SIGNAL(valueChanged(int)), this, SLOT(valueChangedSlot()));
    connect(widthSl, SIGNAL(valueChanged(int)), this, SLOT(valueChangedSlot()));
}

IFaceFeathure::IFaceFeathure(const QString& feature_name,
                             const int heimin, const int heimax, const int widmin, const int widmax,
                             const int vposmin, const int vposmax,
                             QWidget* parent):
    FaceWidget(feature_name, heimin, heimax, widmin, widmax, parent)
{
    vPosSl = new QSlider(Qt::Horizontal);
        vPosSl->setMinimum(vposmin);
        vPosSl->setMaximum(vposmax);
    vPosLbl = new QLabel(feature_name + " position");
    vPosLay = new QHBoxLayout();
        vPosLay->addWidget(vPosLbl);
        vPosLay->addWidget(vPosSl);
    mainLay->addLayout(vPosLay);

    connect(vPosSl, SIGNAL(valueChanged(int)), this, SLOT(valueChangedSlot()));
}
