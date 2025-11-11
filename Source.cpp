#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <algorithm>
using namespace std;

// ================== CẤU TRÚC THUỐC ==================
struct Thuoc {
    string maThuoc;
    string tenThuoc;
    double donGia;

    friend istream& operator>>(istream& in, Thuoc& t) {
        cout << "Nhap ma thuoc: ";
        getline(in, t.maThuoc);
        cout << "Nhap ten thuoc: ";
        getline(in, t.tenThuoc);
        cout << "Nhap don gia: ";
        in >> t.donGia;
        in.ignore();
        return in;
    }

    friend ostream& operator<<(ostream& out, const Thuoc& t) {
        out << left << setw(10) << t.maThuoc
            << setw(25) << t.tenThuoc
            << setw(10) << fixed << setprecision(2) << t.donGia;
        return out;
    }
};

// ================== CẤU TRÚC ĐƠN THUỐC ==================
struct DonThuoc {
    string maBenhNhan;
    string tenBenhNhan;
    vector<pair<string, int>> danhSachThuoc; // <maThuoc, soLuong>
    DonThuoc* next;
};

DonThuoc* makeNode(string maBN, string tenBN) {
    DonThuoc* p = new DonThuoc;
    p->maBenhNhan = maBN;
    p->tenBenhNhan = tenBN;
    p->next = NULL;
    return p;
}

void themDonThuoc(DonThuoc*& head, DonThuoc* don) {
    if (!head) head = don;
    else {
        DonThuoc* tmp = head;
        while (tmp->next) tmp = tmp->next;
        tmp->next = don;
    }
}

// ================== IN DANH SÁCH ĐƠN THUỐC ==================
double inMotDon(DonThuoc* don, unordered_map<string, Thuoc>& bangThuoc) {
    cout << "\nMa BN: " << don->maBenhNhan
        << " | Ten: " << don->tenBenhNhan << endl;
    double tong = 0;
    cout << left << setw(10) << "MaThuoc" << setw(25) << "TenThuoc"
        << setw(10) << "SL" << setw(10) << "DonGia" << setw(10) << "ThanhTien\n";
    for (auto& x : don->danhSachThuoc) {
        auto it = bangThuoc.find(x.first);
        if (it != bangThuoc.end()) {
            double tt = it->second.donGia * x.second;
            tong += tt;
            cout << left << setw(10) << it->second.maThuoc
                << setw(25) << it->second.tenThuoc
                << setw(10) << x.second
                << setw(10) << it->second.donGia
                << setw(10) << tt << endl;
        }
    }
    cout << "Tong tien: " << tong << "\n------------------------\n";
    return tong;
}

void inTatCa(DonThuoc* head, unordered_map<string, Thuoc>& bangThuoc) {
    if (!head) {
        cout << "Danh sach don thuoc rong!\n";
        return;
    }
    while (head) {
        inMotDon(head, bangThuoc);
        head = head->next;
    }
}

// ================== TÌM, SỬA, XÓA ==================
DonThuoc* timDonTheoTen(DonThuoc* head, string tenBN) {
    while (head) {
        if (head->tenBenhNhan == tenBN) return head;
        head = head->next;
    }
    return NULL;
}

DonThuoc* timDonTheoMa(DonThuoc* head, string maBN) {
    while (head) {
        if (head->maBenhNhan == maBN) return head;
        head = head->next;
    }
    return NULL;
}

void suaDon(DonThuoc* head, unordered_map<string, Thuoc>& bangThuoc) {
    string ma;
    cout << "Nhap ma benh nhan can sua: ";
    getline(cin, ma);
    DonThuoc* don = timDonTheoMa(head, ma);
    if (!don) {
        cout << "Khong tim thay don thuoc!\n";
        return;
    }

    cout << "Nhap so luong thuoc moi: ";
    int m; cin >> m; cin.ignore();
    don->danhSachThuoc.clear();
    for (int i = 0; i < m; i++) {
        string maThuoc; int sl;
        cout << "Ma thuoc: "; getline(cin, maThuoc);
        cout << "So luong: "; cin >> sl; cin.ignore();
        don->danhSachThuoc.push_back({ maThuoc, sl });
    }
    cout << "Da cap nhat don thuoc thanh cong!\n";
}

void xoaDon(DonThuoc*& head) {
    string ma;
    cout << "Nhap ma benh nhan can xoa: ";
    getline(cin, ma);
    if (!head) return;

    if (head->maBenhNhan == ma) {
        DonThuoc* tmp = head;
        head = head->next;
        delete tmp;
        cout << "Da xoa don thuoc!\n";
        return;
    }

    DonThuoc* prev = head;
    DonThuoc* curr = head->next;
    while (curr) {
        if (curr->maBenhNhan == ma) {
            prev->next = curr->next;
            delete curr;
            cout << "Da xoa don thuoc!\n";
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    cout << "Khong tim thay don thuoc!\n";
}

// ================== SẮP XẾP (theo tổng tiền) ==================
void sapXepTheoTongTien(DonThuoc*& head, unordered_map<string, Thuoc>& bangThuoc) {
    vector<DonThuoc*> ds;
    for (DonThuoc* p = head; p; p = p->next) ds.push_back(p);

    sort(ds.begin(), ds.end(), [&](DonThuoc* a, DonThuoc* b) {
        double ta = 0, tb = 0;
        for (auto& x : a->danhSachThuoc)
            if (bangThuoc.count(x.first))
                ta += bangThuoc[x.first].donGia * x.second;
        for (auto& x : b->danhSachThuoc)
            if (bangThuoc.count(x.first))
                tb += bangThuoc[x.first].donGia * x.second;
        return ta < tb;
        });

    for (size_t i = 0; i < ds.size() - 1; i++) ds[i]->next = ds[i + 1];
    ds.back()->next = NULL;
    head = ds.front();
    cout << "Da sap xep danh sach theo tong tien!\n";
}

// ================== MAIN ==================
int main() {
    unordered_map<string, Thuoc> bangThuoc;
    DonThuoc* dsDon = NULL;

    while (true) {
        cout << "\n===== QUAN LY DON THUOC =====\n";
        cout << "1. Nhap danh sach thuoc\n";
        cout << "2. Them don thuoc moi\n";
        cout << "3. Tim don thuoc theo ten\n";
        cout << "4. In danh sach don thuoc\n";
        cout << "5. Sua don thuoc\n";
        cout << "6. Xoa don thuoc\n";
        cout << "7. Sap xep don thuoc theo tong tien\n";
        cout << "8. Tim don thuoc theo ma benh nhan\n";
        cout << "0. Thoat\n";
        cout << "Lua chon: ";
        int c; cin >> c; cin.ignore();

        if (c == 0) break;

        if (c == 1) {
            int n; cout << "Nhap so luong thuoc: "; cin >> n; cin.ignore();
            for (int i = 0; i < n; i++) {
                Thuoc t; cin >> t;
                bangThuoc[t.maThuoc] = t;
            }
        }
        else if (c == 2) {
            string maBN, tenBN;
            cout << "Nhap ma benh nhan: "; getline(cin, maBN);
            cout << "Nhap ten benh nhan: "; getline(cin, tenBN);
            DonThuoc* don = makeNode(maBN, tenBN);
            int m; cout << "Nhap so loai thuoc: "; cin >> m; cin.ignore();
            for (int i = 0; i < m; i++) {
                string ma; int sl;
                cout << "Ma thuoc: "; getline(cin, ma);
                cout << "So luong: "; cin >> sl; cin.ignore();
                don->danhSachThuoc.push_back({ ma, sl });
            }
            themDonThuoc(dsDon, don);
        }
        else if (c == 3) {
            string ten;
            cout << "Nhap ten benh nhan can tim: ";
            getline(cin, ten);
            DonThuoc* don = timDonTheoTen(dsDon, ten);
            if (don) inMotDon(don, bangThuoc);
            else cout << "Khong tim thay don thuoc!\n";
        }
        else if (c == 4) inTatCa(dsDon, bangThuoc);
        else if (c == 5) suaDon(dsDon, bangThuoc);
        else if (c == 6) xoaDon(dsDon);
        else if (c == 7) sapXepTheoTongTien(dsDon, bangThuoc);
        else if (c == 8) {
            string ma;
            cout << "Nhap ma benh nhan can tim: ";
            getline(cin, ma);
            DonThuoc* don = timDonTheoMa(dsDon, ma);
            if (don) inMotDon(don, bangThuoc);
            else cout << "Khong tim thay don thuoc!\n";
        }
        else cout << "Lua chon khong hop le!\n";
    }
}
