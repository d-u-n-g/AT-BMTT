#include <iostream>
using namespace std;

struct PublicKey
{
    long long p;      // số nguyên tố
    long long alpha;  // phần tử sinh
    long long beta;   // alpha^a mod p
};

struct PrivateKey
{
    long long a;      
};

// LUỸ THỪA MOD 
long long modExp(long long x, long long n, long long p)
{
    long long result = 1;
    x %= p;

    while(n > 0)
    {
        if(n % 2 == 1)
            result = (result * x) % p;

        x = (x * x) % p;
        n /= 2;
    }

    return result;
}

// TẠO KHÓA 
void taoKhoa(PublicKey &pub, PrivateKey &pri)
{
    cout << "\n===== TAO KHOA =====\n";

    cout << "Nhap so nguyen to p: ";
    cin >> pub.p;

    cout << "Nhap phan tu sinh alpha: ";
    cin >> pub.alpha;

    cout << "Nhap khoa bi mat a (1 <= a <= p-2): ";
    cin >> pri.a;

    pub.beta = modExp(pub.alpha, pri.a, pub.p);

    cout << "\nKhoa cong khai:\n";
    cout << "(p, alpha, beta) = ("<< pub.p << ", "<< pub.alpha << ", "<< pub.beta << ")\n";

    cout << "Khoa bi mat a = "
         << pri.a << endl;
}

// MÃ HÓA 
void maHoa(PublicKey pub)
{
    if(pub.p == 0)
    {
        cout << "Hay tao khoa truoc!\n";
        return;
    }

    long long m, k;

    cout << "\n===== MA HOA =====\n";

    cout << "Nhap ban ro m (0 <= m < p): ";
    cin >> m;

    cout << "Nhap so ngau nhien k (1 <= k <= p-2): ";
    cin >> k;

    long long gamma = modExp(pub.alpha, k, pub.p);

    long long delta = (m * modExp(pub.beta, k, pub.p)) % pub.p;

    cout << "\nBan ma c = (gamma, delta)\n";
    cout << "("
         << gamma
         << ", "
         << delta
         << ")\n";
}

// ================== GIẢI MÃ ==================
void giaiMa(PublicKey pub, PrivateKey pri)
{
    if(pub.p == 0)
    {
        cout << "Hay tao khoa truoc!\n";
        return;
    }

    long long gamma, delta;

    cout << "\n===== GIAI MA =====\n";

    cout << "Nhap gamma: ";
    cin >> gamma;

    cout << "Nhap delta: ";
    cin >> delta;

    long long m = (modExp(gamma, pub.p - 1 - pri.a, pub.p)* delta)% pub.p;

    cout << "\nBan ro m = "
         << m << endl;
}

// ================== MENU ==================
int main()
{
    PublicKey pub = {0,0,0};
    PrivateKey pri = {0};

    int chon;

    do
    {
        cout << "\n==============================";
        cout << "\n      HE ELGAMAL";
        cout << "\n==============================";
        cout << "\n1. Tao khoa";
        cout << "\n2. Ma hoa";
        cout << "\n3. Giai ma";
        cout << "\n0. Thoat";
        cout << "\n==============================";
        cout << "\nNhap lua chon: ";
        cin >> chon;

        switch(chon)
        {
            case 1:
                taoKhoa(pub, pri);
                break;

            case 2:
                maHoa(pub);
                break;

            case 3:
                giaiMa(pub, pri);
                break;

            case 0:
                cout << "\nKet thuc chuong trinh!\n";
                break;

            default:
                cout << "\nLua chon khong hop le!\n";
        }

    } while(chon != 0);

    return 0;
}