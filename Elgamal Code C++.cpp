#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCryptographicHash>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QFileDialog>
#include <QTextStream>
#include <utility>
#include <QFileInfo>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ==================== TOÁN HỌC BỔ TRỢ ====================

long long MainWindow::modExp(long long x, long long n, long long p)
{
    long long result = 1;
    x %= p;

    while (n > 0)
    {
        if (n % 2 == 1)
            result = (__int128)result * x % p;

        x = (__int128)x * x % p;
        n /= 2;
    }

    return result;
}

bool MainWindow::isPrime(long long n)
{
    if (n < 2)
        return false;

    if (n == 2)
        return true;

    if (n % 2 == 0)
        return false;

    for (long long i = 3; i * i <= n; i += 2)
    {
        if (n % i == 0)
            return false;
    }

    return true;
}

long long MainWindow::findGenerator(long long p)
{
    if (p == 2)
        return 1;

    long long n = p - 1;
    long long factors[100]; // Đã thay thế std::vector bằng mảng tĩnh
    int factorCount = 0;
    long long temp = n;

    for (long long i = 2; i * i <= temp; ++i)
    {
        if (temp % i == 0)
        {
            factors[factorCount++] = i;

            while (temp % i == 0)
                temp /= i;
        }
    }

    if (temp > 1)
        factors[factorCount++] = temp;

    for (long long alpha = 2; alpha < p; ++alpha)
    {
        bool ok = true;

        for (int i = 0; i < factorCount; ++i)
        {
            if (modExp(alpha, n / factors[i], p) == 1)
            {
                ok = false;
                break;
            }
        }

        if (ok)
            return alpha;
    }

    return -1;
}

// ==================== NÚT SINH KHÓA ====================

void MainWindow::on_btnGenerate_clicked()
{
    long long p = ui->txtP_4->text().toLongLong();

    if (!isPrime(p))
    {
        do
        {
            p = QRandomGenerator::global()->bounded(2000, 10000);
        }
        while (!isPrime(p));

        ui->txtP_4->setText(QString::number(p));
    }

    long long alpha = findGenerator(p);

    if (alpha == -1)
    {
        QMessageBox::warning(this, "Thông báo", "Không tìm thấy phần tử sinh cho p này!");
        return;
    }

    long long a = QRandomGenerator::global()->bounded(2LL, p - 1);
    long long beta = modExp(alpha, a, p);

    ui->txtAlpha_4->setText(QString::number(alpha));
    ui->txtA_4->setText(QString::number(a));
    ui->txtBeta_4->setText(QString::number(beta));
}

// ==================== NÚT LÀM MỚI ====================

void MainWindow::on_btnClear_clicked()
{
    ui->txtP_4->clear();
    ui->txtAlpha_4->clear();
    ui->txtA_4->clear();
    ui->txtBeta_4->clear();

    ui->txtPlainText_3->clear();
    ui->txtCipherText_3->clear();

    ui->txtCipherInput_3->clear();
    ui->txtPlainOutput_3->clear();
}

// ==================== NÚT MÃ HÓA ====================

void MainWindow::on_btnEncrypt_clicked()
{
    long long p = ui->txtP_4->text().toLongLong();
    long long Alpha = ui->txtAlpha_4->text().toLongLong();
    long long beta = ui->txtBeta_4->text().toLongLong();

    QString plainText = ui->txtPlainText_3->toPlainText();

    if (!isPrime(p) || Alpha <= 0 || beta <= 0 || plainText.isEmpty())
    {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập/sinh đầy đủ thông số khóa và bản rõ hợp lệ!");
        return;
    }

    QString cipherResult = "";

    for (int i = 0; i < plainText.length(); i++)
    {
        long long m = plainText[i].unicode();

        if (m >= p)
        {
            QMessageBox::critical(this, "Lỗi", "Ký tự văn bản vượt quá giá trị p. Hãy sinh số p lớn hơn!");
            return;
        }

        long long k = QRandomGenerator::global()->bounded(1LL, p - 1);

        long long gamma = modExp(Alpha, k, p);
        long long delta = (__int128)m * modExp(beta, k, p) % p;

        cipherResult += QString("%1,%2 ").arg(gamma).arg(delta);
    }

    ui->txtCipherText_3->setPlainText(cipherResult.trimmed());
}

// ==================== NÚT GIẢI MÃ ====================

void MainWindow::on_btnDecrypt_clicked()
{
    long long p = ui->txtP_4->text().toLongLong();
    long long a = ui->txtA_4->text().toLongLong();


    QString cipherText = ui->txtCipherInput_3->toPlainText();
    cipherText.replace("\n", " ");
    cipherText.replace("\r", " ");
    cipherText.replace("\t", " ");
    cipherText = cipherText.trimmed();

    if (p <= 0 || a <= 0 || cipherText.isEmpty())
    {
        QMessageBox::warning(this, "Lỗi", "Thiếu thông tin khóa hệ thống hoặc bản mã!");
        return;
    }


    QString plainResult = "";
    QStringList pairs = cipherText.split(" ", Qt::SkipEmptyParts);

    for (const QString &pair : std::as_const(pairs))
    {
        QStringList parts = pair.split(",");
        if (parts.size() < 2)
            continue;

        long long gamma = parts[0].toLongLong();
        long long delta = parts[1].toLongLong();

        long long m = (__int128)delta * modExp(gamma, p - 1 - a, p) % p;
        plainResult += QChar((int)m);
    }

    ui->txtPlainOutput_3->setPlainText(plainResult);
}
// ==================== FILE TXT / PDF / DOCX ====================

void MainWindow::on_btnOpenFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, "Chọn file cần mã hóa", "", "Tất cả các file (*.*);;PDF (*.pdf);;Word (*.docx);;Văn bản (*.txt)"
        );

    if (!fileName.isEmpty())
    {
        QFileInfo fileInfo(fileName);
        m_originalExt = fileInfo.suffix();

        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly))
        {
            QByteArray fileData = file.readAll();
            file.close();
            QString base64Content = fileData.toBase64();
            ui->txtPlainText_3->setPlainText(base64Content);
        }
        else
        {
            QMessageBox::warning(this, "Lỗi", "Không thể mở file!");
        }
    }
}
void MainWindow::on_btnSaveCipher_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Lưu bản mã", "", "Văn bản (*.txt)");
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            QString data = ui->txtCipherText_3->toPlainText();
            QString hash = calculateHash(data);

            out << data;
            out << "\nHASH=" << hash;
            out << "\nTYPE=CIPHER";
            out << "\nEXT=" << m_originalExt;

            file.close();
        }
    }
}
void MainWindow::on_btnOpenCipher_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, "Chọn file bản mã cần giải mã", "", "Văn bản (*.txt)"
        );

    if (!fileName.isEmpty())
    {
        QFile file(fileName);

        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            QString content = in.readAll();
            QString fileType;

            if(!verifyFile(content, fileType))
            {
                QMessageBox::critical(this, "Lỗi", "File bản mã đã bị chỉnh sửa!");
                file.close();
                return;
            }

            int hashPos = content.lastIndexOf("\nHASH=");
            QString data = content.left(hashPos);
            ui->txtCipherInput_3->setPlainText(data);
            QString remain = content.mid(hashPos + 1);
            QStringList lines = remain.split('\n');
            for(const QString &l : std::as_const(lines)) {
                if(l.startsWith("EXT=")) {
                    m_originalExt = l.section('=', 1).trimmed();
                }
            }

            file.close();
        }
    }
}

void MainWindow::on_btnSavePlain_clicked()
{
    QString filter = "Tất cả (*.*)";
    if (m_originalExt == "pdf") filter = "PDF (*.pdf)";
    else if (m_originalExt == "docx") filter = "Word (*.docx)";
    else if (m_originalExt == "txt") filter = "Văn bản (*.txt)";
    QString defaultName = "KetQua_GiaiMa." + m_originalExt;
    QString fileName = QFileDialog::getSaveFileName(
        this, "Lưu bản rõ giải mã", defaultName, filter
        );
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly))
        {
            QString base64Data = ui->txtPlainOutput_3->toPlainText().trimmed();
            QByteArray decodedData = QByteArray::fromBase64(base64Data.toUtf8());

            if (decodedData.isEmpty() && !base64Data.isEmpty()) {
                QMessageBox::critical(this, "Lỗi Nghiêm Trọng", "Dữ liệu bị rác. BẠN ĐÃ DÙNG SAI KHÓA!");
                file.close();
                return;
            }

            file.write(decodedData);
            file.close();
            QMessageBox::information(this, "Thành công", "Đã xuất file giải mã thành công!");
        }
    }
}
void MainWindow::on_btnSaveKey_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(
        this, "Lưu thông tin khóa", "", "Văn bản (*.txt)"
        );

    if (!fileName.isEmpty())
    {
        QFile file(fileName);

        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            QString data;

            data += "=== HE THONG KHOA ELGAMAL ===\n";
            data += "p (so nguyen to): " + ui->txtP_4->text() + "\n";
            data += "alpha (phan tu sinh): " + ui->txtAlpha_4->text() + "\n";
            data += "a (khoa bi mat): " + ui->txtA_4->text() + "\n";
            data += "beta (khoa cong khai): " + ui->txtBeta_4->text() + "\n";

            QString hash = calculateHash(data);

            out << data;
            out << "\nHASH=" << hash;
            out << "\nTYPE=KEY";

            file.close();

            QMessageBox::information(this, "Thành công", "Đã xuất thông tin cặp khóa thành công!");
        }
    }
}

void MainWindow::on_btnLoadKey_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, "Chọn file khóa", "", "Văn bản (*.txt)"
        );

    if (fileName.isEmpty())
        return;

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Lỗi", "Không mở được file!");
        return;
    }

    QTextStream in(&file);
    QString content = in.readAll();
    QString fileType;

    if(!verifyFile(content, fileType))
    {
        QMessageBox::critical(this, "Lỗi", "File khóa đã bị chỉnh sửa!");
        file.close();
        return;
    }

    int hashPos = content.lastIndexOf("\nHASH=");
    QString data = content.left(hashPos);
    QStringList lines = data.split('\n');

    long long p = 0, alpha = 0, a = 0, beta = 0;

    for(const QString &line : std::as_const(lines)) // Đã xử lý cảnh báo
    {
        if (line.contains("p (so nguyen to):"))
            p = line.section(':',1).trimmed().toLongLong();
        else if (line.contains("alpha"))
            alpha = line.section(':',1).trimmed().toLongLong();
        else if (line.contains("a (khoa bi mat):"))
            a = line.section(':',1).trimmed().toLongLong();
        else if (line.contains("beta"))
            beta = line.section(':',1).trimmed().toLongLong();
    }

    file.close();

    ui->txtP_4->setText(QString::number(p));
    ui->txtAlpha_4->setText(QString::number(alpha));
    ui->txtA_4->setText(QString::number(a));
    ui->txtBeta_4->setText(QString::number(beta));

    QMessageBox::information(this, "Thành công", "Đã nạp khóa từ file!");
}

QString MainWindow::calculateHash(const QString &data)
{
    QByteArray hash = QCryptographicHash::hash(data.toUtf8(), QCryptographicHash::Sha256);
    return hash.toHex();
}

bool MainWindow::verifyFile(const QString &fileContent, QString &fileType)
{
    int hashPos = fileContent.lastIndexOf("\nHASH=");

    if(hashPos == -1)
        return false;

    QString data = fileContent.left(hashPos);
    QString remain = fileContent.mid(hashPos + 1);
    QStringList lines = remain.split('\n');

    if(lines.size() < 2)
        return false;

    QString storedHash = lines[0].section('=',1);
    fileType = lines[1].section('=',1);
    QString currentHash = calculateHash(data);

    return storedHash == currentHash;
}
void MainWindow::on_btnCalcBeta_clicked()
{
    long long p = ui->txtP_4->text().toLongLong();
    long long alpha = ui->txtAlpha_4->text().toLongLong();
    long long a = ui->txtA_4->text().toLongLong();

    if(p <= 0 || alpha <= 0 || a <= 0)
    {
        QMessageBox::warning(
            this,
            "Lỗi",
            "Vui lòng nhập q(p), alpha và a!"
            );
        return;
    }

    long long beta = modExp(alpha, a, p);

    ui->txtBeta_4->setText(QString::number(beta));
}
// ==================== TÍNH BETA TỰ ĐỘNG ====================

// Hàm tính toán ngầm (Không báo lỗilàm phiền người dùng)
void MainWindow::autoCalculateBeta()
{
    long long p = ui->txtP_4->text().toLongLong();
    long long alpha = ui->txtAlpha_4->text().toLongLong();
    long long a = ui->txtA_4->text().toLongLong();

    // Chỉ tính và hiển thị Beta khi người dùng đã gõ số lớn hơn 0 vào CẢ 3 Ô
    if (p > 0 && alpha > 0 && a > 0)
    {
        long long beta = modExp(alpha, a, p);
        ui->txtBeta_4->setText(QString::number(beta));
    }
    else
    {
        // Nếu người dùng lỡ tay xóa trống 1 trong 3 ô, tự động xóa ô Beta
        ui->txtBeta_4->clear();
    }
}

// Gọi hàm tính ngầm mỗi khi nội dung 1 trong 3 ô bị thay đổi
void MainWindow::on_txtP_4_textChanged(const QString &arg1)
{
    autoCalculateBeta();
}

void MainWindow::on_txtAlpha_4_textChanged(const QString &arg1)
{
    autoCalculateBeta();
}

void MainWindow::on_txtA_4_textChanged(const QString &arg1)
{
    autoCalculateBeta();
}