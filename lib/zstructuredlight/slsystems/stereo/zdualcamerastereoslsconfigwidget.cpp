#include "zdualcamerastereoslsconfigwidget.h"
#include "ui_zdualcamerastereoslsconfigwidget.h"

#include "zcameracalibratorwidget.h"
#include "zcamerapreviewer.h"
#include "zmulticameracalibratorwidget.h"
#include "zdualcamerastereosls.h"

#include <QMenu>

namespace Z3D {

ZDualCameraStereoSLSConfigWidget::ZDualCameraStereoSLSConfigWidget(ZDualCameraStereoSLS *stereoSLS, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ZDualCameraStereoSLSConfigWidget)
    , m_stereoSLS(stereoSLS)
{
    ui->setupUi(this);

    /// Update UI
    connect(ui->calibrateSystemButton, &QPushButton::clicked, [&](){
        std::vector<Z3D::ZCalibratedCamera::Ptr> cameras;
        cameras.push_back(m_stereoSLS->leftCamera());
        cameras.push_back(m_stereoSLS->rightCamera());
        Z3D::ZMultiCameraCalibratorWidget *calibWidget = new Z3D::ZMultiCameraCalibratorWidget(cameras);
        calibWidget->show();
    });

    connect(m_stereoSLS, &ZDualCameraStereoSLS::maxValidDistanceChanged,
            ui->maxValidDistanceSpinBox, &QDoubleSpinBox::setValue);
    connect(ui->maxValidDistanceSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            m_stereoSLS, &ZDualCameraStereoSLS::setMaxValidDistance);

    connect(m_stereoSLS, &ZDualCameraStereoSLS::debugShowDecodedImagesChanged,
            ui->debugShowDecodedImagesCheckBox, &QCheckBox::setChecked);
    connect(ui->debugShowDecodedImagesCheckBox, &QCheckBox::toggled,
            m_stereoSLS, &ZDualCameraStereoSLS::setDebugShowDecodedImages);

    connect(m_stereoSLS, &ZDualCameraStereoSLS::debugShowFringesChanged,
            ui->debugShowFringesCheckBox, &QCheckBox::setChecked);
    connect(ui->debugShowFringesCheckBox, &QCheckBox::toggled,
            m_stereoSLS, &ZDualCameraStereoSLS::setDebugShowFringes);

    ui->maxValidDistanceSpinBox->setValue(m_stereoSLS->maxValidDistance());
    ui->debugShowDecodedImagesCheckBox->setChecked(m_stereoSLS->debugShowDecodedImages());
    ui->debugShowFringesCheckBox->setChecked(m_stereoSLS->debugShowFringes());

    /// Left camera
    QMenu *leftCameraMenu = new QMenu(this);
    QAction *leftPreviewAction = leftCameraMenu->addAction(tr("Preview..."));
    connect(leftPreviewAction, &QAction::triggered, [&](){
        auto camera = m_stereoSLS->leftCamera()->camera();
        auto *previewDialog = new Z3D::ZCameraPreviewer(camera);
        previewDialog->show();
    });
    QAction *leftSettingsAction = leftCameraMenu->addAction(tr("Settings..."));
    connect(leftSettingsAction, &QAction::triggered, [&](){
        auto camera = m_stereoSLS->leftCamera()->camera();
        camera->showSettingsDialog();
    });
    QAction *leftCalibrationAction = leftCameraMenu->addAction(tr("Calibration..."));
    connect(leftCalibrationAction, &QAction::triggered, [&](){
        auto camera = m_stereoSLS->leftCamera();
        auto *calibrator = new Z3D::ZCameraCalibratorWidget(camera);
        calibrator->show();
    });
    ui->leftCameraButton->setMenu(leftCameraMenu);

    /// Right camera
    QMenu *rightCameraMenu = new QMenu(this);
    QAction *rightPreviewAction = rightCameraMenu->addAction(tr("Preview..."));
    connect(rightPreviewAction, &QAction::triggered, [&](){
        auto camera = m_stereoSLS->rightCamera()->camera();
        auto *previewDialog = new Z3D::ZCameraPreviewer(camera);
        previewDialog->show();
    });
    QAction *rightSettingsAction = rightCameraMenu->addAction(tr("Settings..."));
    connect(rightSettingsAction, &QAction::triggered, [&](){
        auto camera = m_stereoSLS->rightCamera()->camera();
        camera->showSettingsDialog();
    });
    QAction *rightCalibrationAction = rightCameraMenu->addAction(tr("Calibration..."));
    connect(rightCalibrationAction, &QAction::triggered, [&](){
        auto camera = m_stereoSLS->rightCamera();
        auto *calibrator = new Z3D::ZCameraCalibratorWidget(camera);
        calibrator->show();
    });
    ui->rightCameraButton->setMenu(rightCameraMenu);
}

ZDualCameraStereoSLSConfigWidget::~ZDualCameraStereoSLSConfigWidget()
{
    delete ui;
}

} // namespace Z3D