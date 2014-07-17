#include "registrationform.h"

RegistrationForm::RegistrationForm(ClientSocket *socket, QWidget *parent):QDialog(parent) {
    pSocket = socket;
    setWindowTitle("Registration");
    QRect sizeOfMonitor = QApplication::desktop()->screenGeometry();
    const QSize sizeOfWindow = QSize(300, 200);
    //const QRect positonOfWindow = QRect(sizeOfMonitor.width() / 2 - sizeOfWindow.width() / 2 - 100, 100, sizeOfWindow.width(), sizeOfWindow.height());
    //setGeometry(positonOfWindow);

    lblPseud = new QLabel("Pseudonym:", this);
    tePseudonym = new QLineEdit(this);
    lblLoginReg = new QLabel("Login:", this);
    teLoginReg = new QLineEdit(this);
    lblPasswordReg = new QLabel("Password:", this);
    tePasswordReg = new QLineEdit(this);
    tePasswordReg->setEchoMode(QLineEdit::Password);
    lblConfPasswordReg = new QLabel("Confirm password:", this);
    teConfPasswordReg = new QLineEdit(this);
    teConfPasswordReg->setEchoMode(QLineEdit::Password);
    QPushButton *butRegisterSubmit = new QPushButton("Register", this);
    QVBoxLayout *lyt = new QVBoxLayout(this);
    lyt->addWidget(lblPseud);
    lyt->addWidget(tePseudonym);
    lyt->addWidget(lblLoginReg);
    lyt->addWidget(teLoginReg);
    lyt->addWidget(lblPasswordReg);
    lyt->addWidget(tePasswordReg);
    lyt->addWidget(lblConfPasswordReg);
    lyt->addWidget(teConfPasswordReg);
    lyt->addWidget(butRegisterSubmit);
    setLayout(lyt);
    setFixedSize(lyt->sizeHint());

    connect(butRegisterSubmit, SIGNAL(clicked()), this, SLOT(slotRegister()));
    //connect(pSocket->listener(), SIGNAL(tryRegister(quint16)), this, SLOT(slotStatusRegistration(quint16)), Qt::QueuedConnection);
}

void RegistrationForm::closeEvent(QCloseEvent *e) {
    tePseudonym->clear();
    teLoginReg->clear();
    tePasswordReg->clear();
    teConfPasswordReg->clear();
    emit closed();
}

void RegistrationForm::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
        slotRegister();
}

void RegistrationForm::slotRegister() {
    qDebug() << "slot register";
    if (tePseudonym->text().size() < 3) {
        QMessageBox::critical(0, "RegistrationError", "Pseudonym is shorter than three characters!");
        return;
    }

    if (teLoginReg->text().size() < 3) {
        QMessageBox::critical(0, "RegistrationError", "Login is shorter than three characters!");
        return;
    }

    if (tePasswordReg->text().size() < 3) {
        QMessageBox::critical(0, "RegistrationError", "Password is shorter than three characters!");
        return;
    }

    if (tePasswordReg->text() != teConfPasswordReg->text()) {
        QMessageBox::critical(0, "RegistrationError", "Confirm password again!");
        teConfPasswordReg->clear();
        return;
    }
    quint16 userId = pSocket->registerUser(teLoginReg->text(), tePseudonym->text(), tePasswordReg->text());
    qDebug() << userId;
    if (userId == 0)
        QMessageBox::critical(0, "RegistrationError", "This login already exists!");
    else
        emit registered(userId, tePseudonym->text());
}
