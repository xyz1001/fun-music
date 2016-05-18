#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDialog>
#include <QWidget>

namespace Ui {
class AndroidOptions;
class Options;
}

class Options : public QDialog
{
    Q_OBJECT

public:
    explicit Options(QWidget *parent = 0);
    ~Options();

    QString chooseMusicDownloadDir();
    void loadSettings(QString musicSaveDir, bool autoDownload, bool autoHide);
private:
    Ui::Options *ui;
    //Ui::AndroidOptions *ui;
    QString musicSaveDir;
    bool autoDownload;
    bool autoHide;

    void apply();
signals:
    void signalUpdateSettings(QString musicSaveDir, bool autoDownload, bool autoHide);
public slots:
    void onBtnSelectClicked();
    void onBtnOKClicked();
    void onBtnApplyClicked();
    void onBtnCancelClicked();
};

#endif // OPTIONS_H
