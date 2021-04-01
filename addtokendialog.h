#ifndef ADDTOKENDIALOG_H
#define ADDTOKENDIALOG_H

#include <QDialog>
#include "core/encryptservice.h"

namespace Ui {
class AddTokenDialog;
}

class AddTokenDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddTokenDialog(EncryptService *encryptService, QWidget *parent = nullptr);
    ~AddTokenDialog();

private slots:
    void on_saveButton__clicked();

    void on_cancelButton__clicked();
protected:
    void closeEvent(QCloseEvent *) override;
private:
    Ui::AddTokenDialog *ui;
    EncryptService *encryptService;
    void closeDialog();

};

#endif // ADDTOKENDIALOG_H
