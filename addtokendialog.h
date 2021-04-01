#ifndef ADDTOKENDIALOG_H
#define ADDTOKENDIALOG_H

#include <QDialog>
#include "core/encryptservice.h"
using Mode = int;

#define CREATE 1
#define EDIT 2

namespace Ui {
class AddTokenDialog;
}

class AddTokenDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddTokenDialog(EncryptService *encryptService, QWidget *parent = nullptr);
    ~AddTokenDialog();
    void setMode(Mode mode);
    void setData(const Token &token, const DecryptedData &decryptedData);

private slots:
    void on_saveButton__clicked();

    void on_cancelButton__clicked();
protected:
    void closeEvent(QCloseEvent *) override;
private:
    Mode mode =  CREATE;
    Ui::AddTokenDialog *ui;
    EncryptService *encryptService;
    void closeDialog();

};

#endif // ADDTOKENDIALOG_H
