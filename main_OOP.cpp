/*
Đề bài:
3. Hệ thống thuế đất (hình học)
Lớp cơ sở: HinhHoc (Tên hình) có thuộc tính KhuVuc (int).
Lớp kế thừa: HinhTron (Bán kính), HinhChuNhat (Chiều dài, Chiều rộng), TamGiac (Cạnh a, Cạnh b, Cạnh c).
Đa hình: Phương thức TinhDienTich() và TinhChuVi() được định nghĩa lại cho từng hình cụ thể. Phương thức TinhThue() được tính dựa trên thuộc tính diện tích và thuế khu vực riêng kèm mỗi loại làm thay đổi công thức tính.

*/
#include <windows.h>

#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <iomanip>
#include <sstream>

#include "termcolor.hpp"
#include "tabulate.hpp"
using namespace std;

// === class HinhHoc
class HinhHoc {
protected:
    int khuVuc; // nông thôn, thành thị
    string mucDichSuDung; // nhà ở, kinh doanh, nông nghiệp, công nghiệp
    float soThueNo; // số tiền thuế còn nợ sau khi nộp
public:
    // Constructor hh
    HinhHoc(int khuVuc = 0, string mucDichSuDung = " ", float soThueNo = 0) {
        this->khuVuc = khuVuc;
        this->mucDichSuDung = mucDichSuDung;
        this->soThueNo = soThueNo;
    }

    HinhHoc(const HinhHoc& hh) {
        this->khuVuc = hh.khuVuc;
        this->mucDichSuDung = hh.mucDichSuDung;
        this->soThueNo = hh.soThueNo;
    }
    // Destructor hh
    virtual ~HinhHoc() {}

    // getter và setter hh
    int getKhuVuc() const { return this->khuVuc; }
    string getMDSD() const { return this->mucDichSuDung; }
    float getSoThueNo() const { return this->soThueNo; }

    void setKhuVuc(int khuVuc) { this->khuVuc = khuVuc; }
    void setMDSD(string MucDichSuDung) { this->mucDichSuDung = MucDichSuDung; }
    void setSoThueNo(float soThueNo) { this->soThueNo = soThueNo; }

    // hàm tính toán hh
    virtual float TinhDienTich() const = 0;
    virtual float TinhChuVi() const = 0;
    virtual string getLoaiHinh() const = 0;

    // hàm tính thuế
    virtual float TinhThue() const {
        float donGia = 1000; // giả sử đơn giá là 1000
        return TinhDienTich() * donGia * heSoMucDich(); // công thức tính thuế
    }

    // hàm tính hệ số mục đích sử dụng
    float heSoMucDich() const {
        if (mucDichSuDung == "nha_o") return 1.0; // đóng đúng 100% số thuế 
        if (mucDichSuDung == "kinh_doanh") return 1.5; // đóng 150% số thuế
        if (mucDichSuDung == "nong_nghiep") return 0.7; // đóng 70% số thuế
        if (mucDichSuDung == "cong_nghiep") return 1.3; // đóng 130% số thuế
        return 1.0;
    }

    string moTaMucDich() const {
        if (mucDichSuDung == "nha_o") return "Nhà ở";
        if (mucDichSuDung == "kinh_doanh") return "Kinh doanh";
        if (mucDichSuDung == "nong_nghiep") return "Nông nghiệp";
        if (mucDichSuDung == "cong_nghiep") return "Công nghiệp";
        return "Khác";
    }

    // hàm giảm thuế
    float giamThue() const {
        float thue = TinhThue(); // tính thuế trước giảm
        float giam = 0; // số tiền được giảm

        // các điều kiện giảm thuế
        if (TinhDienTich() < 50) giam += thue * 0.1; // giảm 10% nếu diện tích < 50
        if (khuVuc == 1) giam += thue * 0.15; // giảm 15% nếu khu vực nông thôn
        if (mucDichSuDung == "nong_nghiep") giam += thue * 0.2; // giảm 20% nếu mục đích sử dụng là nông nghiệp

        return giam;
    }

    // hàm tính thuế sau giảm
    float thueSauGiam() const {
        return TinhThue() - giamThue(); // thuế sau khi đã giảm
    }

    // hàm nộp thuế
    void nopThue(float soTienDaNop) {
        float conNo = round(thueSauGiam()) - round(soTienDaNop); // tính số tiền còn nợ sau khi nộp thuế

        if (conNo > 0) {
            cout << termcolor::on_red << "Còn nợ: " << termcolor::bold << (long long)conNo << " đồng" << termcolor::reset << endl; // nếu còn nợ thì cập nhật số tiền nợ
            soThueNo = conNo;
        }
        else if (conNo == 0) {
            cout << termcolor::on_green << "Đã nộp đủ thuế!" << termcolor::reset << endl; // nếu nộp đủ thì không còn nợ
            soThueNo = 0;
        }
        else {
            float du = (long long)abs(conNo);
            cout << termcolor::cyan << "Nộp dư: " << termcolor::bold << du << " đồng (đã hoàn trả)!" << termcolor::reset << endl; // nếu nộp dư thì hoàn trả
            soThueNo = 0;
        }
    }

    float conNoThue() const { // trả về số tiền còn nợ sau khi đã nộp
        if (soThueNo > 0) return soThueNo;
        return 0;
    }

	// chi tiết giảm thuế để giải thích lý do giảm thuế
    string chiTietGiamThue() const {
        string lyDo = "";
        float thue = TinhThue();

        if (TinhDienTich() < 50)
            lyDo += "- Diện tích < 50m2 (giảm 10%)\n";
        if (khuVuc == 1)
            lyDo += "- Khu vực nông thôn (giảm 15%)\n";
        if (mucDichSuDung == "nong_nghiep")
            lyDo += "- Mục đích nông nghiệp (giảm 20%)\n";

        if (lyDo == "") return "--";
        return lyDo;
    }
};

// === class HinhTron
class HinhTron : public HinhHoc {
private:
    float banKinh;

public:
    // Constructor hình tròn
    HinhTron(int khuVuc = 0, string mucDich = "", float soThueNo = 0, float banKinh = 0)
        : HinhHoc(khuVuc, mucDich, soThueNo), banKinh(banKinh) {
    }

    HinhTron(const HinhTron& ht) : HinhHoc(ht) {
        this->banKinh = ht.banKinh;
    }
    // Destructor hình tròn
    ~HinhTron() override {}

    // getter và setter hình tròn
    float getBanKinh() const { return this->banKinh; }
    string getLoaiHinh() const override {
        return "Hình tròn";
    }

    void setBanKinh(float banKinh) {
        if (banKinh > 0) this->banKinh = banKinh;
    }

    // hàm tính toán hình tròn
    float TinhDienTich() const override {
        return 3.14f * banKinh * banKinh;
    }

    float TinhChuVi() const override {
        return 2 * 3.14f * banKinh;
    }

    // quá tải nhập/xuất hình tròn
    friend istream& operator>>(istream& in, HinhTron& ht) {
        cout << "Nhập bán kính: ";
        in >> ht.banKinh;
        return in;
    }

    friend ostream& operator<<(ostream& out, const HinhTron& ht) {
        out << "\n===== HÌNH TRÒN =====\n";
        out << "Bán kính: " << ht.banKinh << endl;

        out << "Khu vực: " << (ht.getKhuVuc() == 1 ? "Nông thôn" : "Thành thị") << endl;
        out << "Mục đích: " << ht.moTaMucDich() << endl;

        out << "Diện tích: " << ht.TinhDienTich() << endl;
        out << "Chu vi: " << ht.TinhChuVi() << endl;

        out << "Thuế trước giảm: " << round(ht.TinhThue()) << endl;
        out << "Giảm thuế: " << round(ht.giamThue()) << endl;
        out << "Thuế sau giảm: " << round(ht.thueSauGiam()) << endl;

        out << "Còn nợ: " << ht.conNoThue() << endl;

        out << "=====================\n";
        return out;
    }
};

// === class HinhChuNhat
class HinhChuNhat : public HinhHoc {
private:
    float dai;
    float rong;

public:
    // Constructor hcn
    HinhChuNhat(int khuVuc = 0, string mucDich = "", float soThueNo = 0,
        float dai = 0, float rong = 0)
        : HinhHoc(khuVuc, mucDich, soThueNo), dai(dai), rong(rong) {
    }

    HinhChuNhat(const HinhChuNhat& hcn) : HinhHoc(hcn) {
        this->dai = hcn.dai;
        this->rong = hcn.rong;
    }
    // Destructor hcn
    ~HinhChuNhat() override {}

    // hàm tính toán hcn
    float TinhDienTich() const override {
        return dai * rong;
    }

    float TinhChuVi() const override {
        return 2 * (dai + rong);
    }

    // getter và setter hcn
    float getDai() const { return this->dai; }
    float getRong() const { return this->rong; }
    string getLoaiHinh() const override {
        return "Hình chữ nhật";
    }

    void setDai(float dai) {
        if (dai > 0) this->dai = dai;
    }
    void setRong(float rong) {
        if (rong > 0) this->rong = rong;
    }

    // quá tải nhập/xuất hcn
    friend istream& operator>>(istream& in, HinhChuNhat& hcn) {
        cout << "Nhập dài: ";
        in >> hcn.dai;
        cout << "Nhập rộng: ";
        in >> hcn.rong;
        return in;
    }

    friend ostream& operator<<(ostream& out, const HinhChuNhat& hcn) {
        out << "\n===== HÌNH CHỮ NHẬT =====\n";
        out << "Dài: " << hcn.dai << endl;
        out << "Rộng: " << hcn.rong << endl;

        out << "Khu vực: " << (hcn.getKhuVuc() == 1 ? "Nông thôn" : "Thành thị") << endl;
        out << "Mục đích: " << hcn.moTaMucDich() << endl;

        out << "Diện tích: " << hcn.TinhDienTich() << endl;
        out << "Chu vi: " << hcn.TinhChuVi() << endl;

        out << "Thuế trước giảm: " << round(hcn.TinhThue()) << endl;
        out << "Giảm thuế: " << round(hcn.giamThue()) << endl;
        out << "Thuế sau giảm: " << round(hcn.thueSauGiam()) << endl;

        out << "Còn nợ: " << hcn.conNoThue() << endl;

        out << "=========================\n";
        return out;
    }
};

// === class TamGiac
class TamGiac : public HinhHoc {
private:
    float canh1, canh2, canh3;

public:
    // Constructor tam giác
    TamGiac(int khuVuc = 0, string mucDich = "", float soThueNo = 0,
        float c1 = 0, float c2 = 0, float c3 = 0)
        : HinhHoc(khuVuc, mucDich, soThueNo),
        canh1(c1), canh2(c2), canh3(c3) {
    }

    TamGiac(const TamGiac& tg) : HinhHoc(tg) {
        this->canh1 = tg.canh1;
        this->canh2 = tg.canh2;
        this->canh3 = tg.canh3;
    }

    // Destructor tam giác
    ~TamGiac() override {}

    // hàm tính toán tam giác
    float TinhChuVi() const override {
        return canh1 + canh2 + canh3;
    }

    float TinhDienTich() const override {
        float p = TinhChuVi() / 2;
        float kqua = p * (p - canh1) * (p - canh2) * (p - canh3);
        if (kqua <= 0) return 0; // tránh trường hợp tam giác không hợp lệ hoặc diện tích âm do lỗi làm tròn
        return sqrt(kqua);
    }

    // getter và setter tam giác
    float getCanh1() const { return this->canh1; }
    float getCanh2() const { return this->canh2; }
    float getCanh3() const { return this->canh3; }
    string getLoaiHinh() const override {
        return "Tam giác";
    }

    void setCanh1(float c1) {
        if (c1 > 0) this->canh1 = c1;
    }
    void setCanh2(float c2) {
        if (c2 > 0) this->canh2 = c2;
    }
    void setCanh3(float c3) {
        if (c3 > 0) this->canh3 = c3;
    }

    // quá tải nhập/xuất tam giác
    friend istream& operator>>(istream& in, TamGiac& tg) {
        cout << "Nhập 3 cạnh: ";
        in >> tg.canh1 >> tg.canh2 >> tg.canh3;
        return in;
    }

    friend ostream& operator<<(ostream& out, const TamGiac& tg) {
        out << "\n===== TAM GIÁC =====\n";
        out << "Cạnh: " << tg.canh1 << ", " << tg.canh2 << ", " << tg.canh3 << endl;

        out << "Khu vực: " << (tg.getKhuVuc() == 1 ? "Nông thôn" : "Thành thị") << endl;
        out << "Mục đích: " << tg.moTaMucDich() << endl;

        out << "Diện tích: " << tg.TinhDienTich() << endl;
        out << "Chu vi: " << tg.TinhChuVi() << endl;

        out << "Thuế trước giảm: " << round(tg.TinhThue()) << endl;
        out << "Giảm thuế: " << round(tg.giamThue()) << endl;
        out << "Thuế sau giảm: " << round(tg.thueSauGiam()) << endl;

        out << "Còn nợ: " << tg.conNoThue() << endl;

        out << "=====================\n";
        return out;
    }
};

// định dạng số thập phân thành chuỗi
string formatNum(float num) {
    stringstream ss;
    ss << num;
    return ss.str();
}

// === MAIN
int main() {

	SetConsoleOutputCP(CP_UTF8); // hiển thị tiếng việt có dấu trên console
	SetConsoleCP(CP_UTF8); // nhập tiếng việt có dấu trên console

    // Ascii art
    cout << "H   H EEEEE    TTTTT H   H  OOO  N   N  GGGG    TTTTT H   H U   U EEEEE    DDDD    A   TTTTT" << endl;
    cout << "H   H E          T   H   H O   O NN  N G          T   H   H U   U E        D   D  A A    T" << endl;
    cout << "HHHHH EEE        T   HHHHH O   O N N N G GGG      T   HHHHH U   U EEE      D   D AAAAA   T" << endl;
    cout << "H   H E          T   H   H O   O N  NN G   G      T   H   H U   U E        D   D A   A   T" << endl;
    cout << "H   H EEEEE      T   H   H  OOO  N   N  GGG       T   H   H  UUU  EEEEE    DDDD  A   A   T" << endl;

    vector<HinhHoc*> ds; // lưu trữ các hình học đã tạo
    int choice; // lựa chọn từ menu


    // menu chính để chọn loại hình đất
    do {
        while (true) {
            cout << termcolor::cyan << "\n===== MENU =====\n" << termcolor::reset; 
            cout << termcolor::cyan << "Chọn loại hình: " << termcolor::reset << endl;
            cout << "1. Hình tròn\n";
            cout << "2. Hình chữ nhật\n";
            cout << "3. Tam giác\n";
            cout << "0. Thoát\n";
            cout << termcolor::yellow << "Chọn: " << termcolor::reset;
            cin >> choice;
            if (choice >= 0 && choice <= 3) break; // Nhập đúng thì thoát vòng lặp này
            cout << termcolor::on_red << "Lựa chọn không hợp lệ! Vui lòng chọn lại (0-3)." << termcolor::reset << endl;
        }

        if (choice == 0) {
            cout << termcolor::on_red << "Đã thoát ứng dụng!" << termcolor::reset;
            break;
        }
        cout << termcolor::green;
        switch (choice) {
        case 1:
            cout << "Đã chọn hình tròn!";
            break;
        case 2:
            cout << "Đã chọn hình chữ nhật!";
            break;
        case 3:
            cout << "Đã chọn hình tam giác!";
            break;
        }
        cout << termcolor::reset << endl << endl;

        // nhập thông tin khu vực
        int khuVuc;
        while (true) {
            cout << termcolor::cyan << "Chọn khu vực: " << termcolor::reset << endl;
            cout << "1: nông thôn" << endl;
            cout << "2: thành thị" << endl;
            cout << termcolor::yellow << "Chọn: " << termcolor::reset;
            cin >> khuVuc;
            cin.ignore();
            if (khuVuc == 1 || khuVuc == 2) break;
            cout << termcolor::on_red << "Lỗi: Chỉ được chọn 1 hoặc 2!" << termcolor::reset << endl;
        }

        cout << termcolor::green;
        switch (khuVuc) {
        case 1:
            cout << "Đã chọn nông thôn!";
            break;
        case 2:
            cout << "Đã chọn thành thị";
            break;
        }
        cout << termcolor::reset << endl << endl;

        // thông tin mục đích sử dụng
        int md;
        string mucDich;
        while (true) {
            cout << termcolor::cyan << "Chọn mục đích sử dụng:\n" << termcolor::reset;
            cout << "1. Nhà ở\n";
            cout << "2. Kinh doanh\n";
            cout << "3. Nông nghiệp\n";
            cout << "4. Công nghiệp\n";
            cout << termcolor::yellow << "Chọn: " << termcolor::reset;

            // Kiểm tra đầu vào có phải là số và nằm trong khoảng 1-4 không
            if (cin >> md && md >= 1 && md <= 4) {
                // Nhập đúng: Gán giá trị và thoát vòng lặp
                if (md == 1) mucDich = "nha_o";
                else if (md == 2) mucDich = "kinh_doanh";
                else if (md == 3) mucDich = "nong_nghiep";
                else if (md == 4) mucDich = "cong_nghiep";
                break;
            }
            else {
                // Nhập sai: Thông báo và xóa bộ nhớ đệm để nhập lại
                cout << termcolor::on_red << "Lỗi: Chỉ được chọn 1, 2, 3 hoặc 4!" << termcolor::reset << endl;
                cin.clear(); // Xóa trạng thái lỗi của cin
                cin.ignore(1000, '\n'); // Loại bỏ các ký tự thừa trong bộ đệm
            }
        }

        bool validMucDich = true; // kiểm tra mục đích sử dụng hợp lệ

        cout << termcolor::green;
        switch (md) {
        case 1:
            mucDich = "nha_o";
            cout << "Đã chọn nhà ở!";
            break;
        case 2:
            mucDich = "kinh_doanh";
            cout << "Đã chọn kinh doanh!";
            break;
        case 3:
            mucDich = "nong_nghiep";
            cout << "Đã chọn nông nghiệp!";
            break;
        case 4:
            mucDich = "cong_nghiep";
            cout << "Đã chọn công nghiệp";
            break;
        default:
            cout << termcolor::on_red << "Lựa chọn không hợp lệ!\n" << termcolor::reset;
            validMucDich = false;
        }
        cout << termcolor::reset << endl;

        if (!validMucDich) continue; // nếu mục đích sử dụng không hợp lệ, quay lại menu

        HinhHoc* h = nullptr; // trỏ đến hình học sẽ tạo

        // tạo hình học dựa trên lựa chọn
        switch (choice) {
        case 1: {
            cout << termcolor::cyan << endl << endl << "===== THÔNG TIN HÌNH TRÒN =====" << termcolor::reset << endl;
            float r;
            cout << termcolor::yellow << "Nhập bán kính: " << termcolor::reset;
            cin >> r;
            h = new HinhTron(khuVuc, mucDich, 0, r);
            break;
        }
        case 2: {
            cout << termcolor::cyan << endl << endl << "===== THÔNG TIN HÌNH CHỮ NHẬT =====" << termcolor::reset << endl;
            float d, r;
            cout << termcolor::yellow << "Nhập chiều dài: " << termcolor::reset;
            cin >> d;
            cout << termcolor::yellow << "Nhập chiều rộng: " << termcolor::reset;
            cin >> r;
            h = new HinhChuNhat(khuVuc, mucDich, 0, d, r);
            break;
        }
        case 3: {
            cout << termcolor::cyan << endl << endl << "===== THÔNG TIN HÌNH TAM GIÁC =====" << termcolor::reset << endl;
            float a, b, c;
            cout << termcolor::yellow << "Nhập độ dài cạnh #1: " << termcolor::reset;
            cin >> a;
            cout << termcolor::yellow << "Nhập độ dài cạnh #2: " << termcolor::reset;
            cin >> b;
            cout << termcolor::yellow << "Nhập độ dài cạnh #3: " << termcolor::reset;
            cin >> c;

            if (a + b <= c || a + c <= b || b + c <= a) {
                cout << termcolor::on_red << "Tam giác không hợp lệ!\n" << termcolor::reset;
                break;
            }

            h = new TamGiac(khuVuc, mucDich, 0, a, b, c);
            break;
        }
        default:
            cout << "Lựa chọn không hợp lệ!\n";
            continue;
        }
        if (h == nullptr) continue; // nếu hình học không được tạo (do lỗi), quay lại menu
        ds.push_back(h); // lưu hình học vào danh sách

        bool hoanTatLuotNay = false;
        while (!hoanTatLuotNay) {
            // in kết quả đã nhập
            tabulate::Table output;

            output.add_row({ "Loại", h->getLoaiHinh() });
            output.add_row({ "Diện tích", formatNum(h->TinhDienTich()) });
            output.add_row({ "Chu vi", formatNum(h->TinhChuVi()) });
            output.add_row({ "Thuế trước giảm", formatNum(round(h->TinhThue())) + " đồng" });
            output.add_row({ "Giảm thuế", formatNum(round(h->giamThue())) + " đồng" });
            output.add_row({ "Lý do giảm", h->chiTietGiamThue() });
            output.add_row({ "Thuế sau giảm", formatNum(round(h->thueSauGiam())) + " đồng" });
            // định dạng bảng kết quả
            output.format()
                .multi_byte_characters(true)
                .border_left(" ")
                .border_right(" ")
                .corner(" ")
                .border_left_color(tabulate::Color::yellow)
                .border_right_color(tabulate::Color::yellow)
                .border_top_color(tabulate::Color::yellow)
                .border_bottom_color(tabulate::Color::yellow);
            // định dạng cột
            output.column(0).format().width(20).background_color(tabulate::Color::magenta);
            output.column(1).format().width(35) 
                .font_align(tabulate::FontAlign::left) 
                .font_style({ tabulate::FontStyle::bold })
                .font_color(tabulate::Color::magenta)
                .background_color(tabulate::Color::white);

            // Lưu ý: vị trí hàng "Thuế sau giảm" bây giờ là hàng số 6 (index 6)
            output.row(6).format().font_style({ tabulate::FontStyle::bold });

            cout << output << endl;

            // Nhập số tiền nộp thuế
            cout << termcolor::cyan << endl << endl << "===== THÔNG TIN THUẾ ĐẤT =====" << termcolor::reset << endl;
            float tienNop;
            cout << termcolor::yellow << "Nhập số tiền nộp thuế (đồng): " << termcolor::reset;
            cin >> tienNop;

            h->nopThue(tienNop); // cập nhật số tiền nợ sau khi nộp thuế

            // in kết quả sau khi nộp thuế
            /*
            printRow("Còn nợ", formatNum(h->conNoThue()) + " đồng");
            printRow("Trạng thái", h->conNoThue() > 0 ? "Còn nợ" : "Đã nộp đủ thuế");
            printLine();
            */

            // tính năng cập nhật thông tin
            string cmd;
            cout << termcolor::bold << "\nNhấn Enter để tiếp tục hoặc nhập u để cập nhật: " << termcolor::reset;
            cin.ignore(1000, '\n'); // Dọn bộ đệm
            getline(cin, cmd);

            if (cmd == "u" || cmd == "U") { 
                cout << termcolor::cyan << "\n=== CẬP NHẬT THÔNG TIN ===" << termcolor::reset << endl;
                cout <<"1. Sửa Khu vực" << endl
                    << "2. Sửa Mục đích" << endl
                    << "3. Nhập lại tiền thuế" << endl;
				cout << termcolor::yellow << "Chọn: " << termcolor::reset;
                int editOption;
                cin >> editOption;

                if (editOption == 1) {
                    int kv_new;
                    while (true) {
                        cout << termcolor::cyan << "Nhập lại Khu vực" << termcolor::reset << endl;
						cout << "1. nông thôn" << endl
							<< "2. thành thị" << endl;
						cout << termcolor::yellow << "Chọn: " << termcolor::reset;
                        cin >> kv_new;
                        if (kv_new == 1 || kv_new == 2) { h->setKhuVuc(kv_new); break; }
                    }
                }
                else if (editOption == 2) {
                    int md_new;
                    while (true) {
                        cout << termcolor::cyan << "Nhập lại Mục đích" << termcolor::reset << endl; 
                        cout << "1. Nhà ở" << endl
                             << "2. Kinh doanh" << endl
                             << "3. Nông nghiệp" << endl
                             << "4. Công nghiệp" << endl;
						cout <<termcolor::yellow <<"Chọn: " << termcolor::reset;
                        cin >> md_new;
                        if (md_new >= 1 && md_new <= 4) {
                            string listMD[] = { "", "nha_o", "kinh_doanh", "nong_nghiep", "cong_nghiep" };
                            h->setMDSD(listMD[md_new]); break;
                        }
                    }
                }
                else if (editOption == 3) {                 
                }
            }
            else {
                hoanTatLuotNay = true; // Thoát vòng lặp để chọn hình mới
            }
        }

    } while (true); // lặp lại menu cho đến khi người dùng chọn thoát

    for (HinhHoc* h : ds) delete h; // giải phóng bộ nhớ 

    return 0;
}

