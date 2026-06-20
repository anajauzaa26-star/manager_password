/**
 * ============================================================
 *  SISTEM MANAJEMEN PASSWORD - CAPSTONE PROJECT STRUKTUR DATA
 * ============================================================
 * Kelompok 2 - Program Studi Sistem Informasi
 * Mata Kuliah : Struktur Data (Semester Genap 2025/2026)
 *
 * Materi Struktur Data yang Diimplementasikan:
 *  1. Array              - Buffer sorting & searching
 *  2. Pointer            - Digunakan pada semua struktur dinamis
 *  3. Struct             - Akun, NodeLL, NodeDLL, NodeStack, NodeBST
 *  4. Single Linked List - Penyimpanan utama akun
 *  5. Double Linked List - Navigasi maju/mundur antar akun
 *  6. Stack              - Riwayat aksi (fitur Undo)
 *  7. Queue              - Log aktivitas pengguna
 *  8. Tree (BST)         - Indeks pencarian cepat berdasarkan nama
 *  9. Graph              - Relasi antar kategori akun
 * 10. Sorting            - Bubble Sort & Quick Sort
 * 11. Searching          - Sequential Search & Binary Search
 *
 * Referensi / Daftar Pustaka:
 *  [1] Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2022).
 *      Introduction to Algorithms (4th ed.). MIT Press.
 *  [2] Drozdek, A. (2012). Data Structures and Algorithms in C++ (4th ed.).
 *      Cengage Learning.
 *  [3] Malik, D. S. (2021). C++ Programming: Program Design Including
 *      Data Structures (9th ed.). Cengage Learning.
 *  [4] Sedgewick, R., & Wayne, K. (2011). Algorithms (4th ed.).
 *      Addison-Wesley Professional.
 *  [5] Horowitz, E., Sahni, S., & Mehta, D. (2007). Fundamentals of
 *      Data Structures in C++ (2nd ed.). Silicon Press.
 * ============================================================
 */

#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>
#include <cstdlib>
using namespace std;

// ============================================================
//  KONSTANTA
// ============================================================
#define MAX_ARRAY  100  // Kapasitas buffer sorting & searching
#define MAX_QUEUE  50   // Kapasitas log aktivitas
#define MAX_GRAPH  20   // Kapasitas vertex graf kategori

// ============================================================
//  STRUCT AKUN - Materi: Struct & Pointer
//  Referensi: [3] Malik (2021), Chapter 9
// ============================================================
struct Akun {
    int    id;
    string namaAkun;
    string username;
    string password;   // plain text sesuai batasan masalah
    string kategori;
    string tanggal;
};

// ============================================================
//  1. SINGLE LINKED LIST - Penyimpanan utama data akun
//     Setiap node hanya punya pointer ke node berikutnya
//     Referensi: [2] Drozdek (2012), Chapter 3
// ============================================================
struct NodeLL {
    Akun    data;
    NodeLL* next;      // pointer ke node berikutnya
    NodeLL(Akun a) : data(a), next(nullptr) {}
};

struct SingleLinkedList {
    NodeLL* head;
    int     jumlah;
    SingleLinkedList() : head(nullptr), jumlah(0) {}

    // Tambah di belakang (append): O(n)
    void tambah(Akun akun) {
        NodeLL* baru = new NodeLL(akun);
        if (!head) { head = baru; }
        else {
            NodeLL* curr = head;
            while (curr->next) curr = curr->next;
            curr->next = baru;
        }
        jumlah++;
    }

    // Hapus berdasarkan ID: O(n)
    bool hapus(int id) {
        if (!head) return false;
        if (head->data.id == id) {
            NodeLL* temp = head;
            head = head->next;
            delete temp;
            jumlah--;
            return true;
        }
        NodeLL* curr = head;
        while (curr->next && curr->next->data.id != id)
            curr = curr->next;
        if (!curr->next) return false;
        NodeLL* temp = curr->next;
        curr->next = temp->next;
        delete temp;
        jumlah--;
        return true;
    }

    // Cari berdasarkan ID: O(n)
    NodeLL* cariByID(int id) {
        NodeLL* curr = head;
        while (curr) {
            if (curr->data.id == id) return curr;
            curr = curr->next;
        }
        return nullptr;
    }

    // Salin ke array (buffer untuk sorting & searching): O(n)
    void keArray(Akun arr[], int& n) {
        n = 0;
        NodeLL* curr = head;
        while (curr && n < MAX_ARRAY) {
            arr[n++] = curr->data;
            curr = curr->next;
        }
    }

    // Tampilkan dalam format tabel: O(n)
    void tampilkan() {
        if (!head) {
            cout << "  [Belum ada akun tersimpan]\n";
            return;
        }
        cout << "  " << string(66, '-') << "\n";
        cout << "  " << left
             << setw(4)  << "ID"
             << setw(16) << "Nama Akun"
             << setw(22) << "Username"
             << setw(12) << "Kategori"
             << "Tanggal\n";
        cout << "  " << string(66, '-') << "\n";
        NodeLL* curr = head;
        while (curr) {
            cout << "  "
                 << setw(4)  << curr->data.id
                 << setw(16) << curr->data.namaAkun
                 << setw(22) << curr->data.username
                 << setw(12) << curr->data.kategori
                 << curr->data.tanggal << "\n";
            curr = curr->next;
        }
        cout << "  " << string(66, '-') << "\n";
    }

    ~SingleLinkedList() {
        NodeLL* curr = head;
        while (curr) {
            NodeLL* temp = curr;
            curr = curr->next;
            delete temp;
        }
    }
};

// ============================================================
//  2. DOUBLE LINKED LIST - Navigasi maju/mundur antar akun
//     Setiap node punya pointer ke node sebelum DAN sesudah
//     Referensi: [2] Drozdek (2012), Chapter 3
// ============================================================
struct NodeDLL {
    Akun     data;
    NodeDLL* prev;     // pointer ke node sebelumnya
    NodeDLL* next;     // pointer ke node berikutnya
    NodeDLL(Akun a) : data(a), prev(nullptr), next(nullptr) {}
};

struct DoubleLinkedList {
    NodeDLL* head;
    NodeDLL* tail;
    NodeDLL* current;  // posisi kursor navigasi saat ini
    int      jumlah;
    DoubleLinkedList() : head(nullptr), tail(nullptr), current(nullptr), jumlah(0) {}

    // Tambah di belakang: O(1) karena ada pointer tail
    void tambah(Akun akun) {
        NodeDLL* baru = new NodeDLL(akun);
        if (!head) {
            head = tail = current = baru;
        } else {
            baru->prev = tail;
            tail->next = baru;
            tail = baru;
        }
        jumlah++;
    }

    // Hapus berdasarkan ID: O(n)
    bool hapus(int id) {
        NodeDLL* curr = head;
        while (curr) {
            if (curr->data.id == id) {
                if (curr->prev) curr->prev->next = curr->next;
                else head = curr->next;
                if (curr->next) curr->next->prev = curr->prev;
                else tail = curr->prev;
                if (current == curr) current = curr->next ? curr->next : curr->prev;
                delete curr;
                jumlah--;
                return true;
            }
            curr = curr->next;
        }
        return false;
    }

    // Update data berdasarkan ID: O(n)
    bool update(int id, Akun akunBaru) {
        NodeDLL* curr = head;
        while (curr) {
            if (curr->data.id == id) {
                curr->data = akunBaru;
                return true;
            }
            curr = curr->next;
        }
        return false;
    }

    // Navigasi ke node berikutnya: O(1)
    bool maju() {
        if (!current || !current->next) return false;
        current = current->next;
        return true;
    }

    // Navigasi ke node sebelumnya: O(1)
    bool mundur() {
        if (!current || !current->prev) return false;
        current = current->prev;
        return true;
    }

    // Reset kursor ke awal: O(1)
    void keAwal() { current = head; }

    // Tampilkan akun saat ini (posisi kursor): O(1)
    void tampilkanCurrent() {
        if (!current) { cout << "  [Tidak ada akun]\n"; return; }
        cout << "  Posisi : " << (jumlah > 0 ? "Ada" : "-") << "\n";
        cout << "  ID     : " << current->data.id << "\n";
        cout << "  Nama   : " << current->data.namaAkun << "\n";
        cout << "  User   : " << current->data.username << "\n";
        cout << "  Kateg  : " << current->data.kategori << "\n";
        cout << "  Prev   : " << (current->prev ? current->prev->data.namaAkun : "(awal)") << "\n";
        cout << "  Next   : " << (current->next ? current->next->data.namaAkun : "(akhir)") << "\n";
    }

    ~DoubleLinkedList() {
        NodeDLL* curr = head;
        while (curr) {
            NodeDLL* temp = curr;
            curr = curr->next;
            delete temp;
        }
    }
};

// ============================================================
//  3. STACK - Riwayat aksi untuk fitur Undo (LIFO)
//     Referensi: [2] Drozdek (2012), Chapter 4
// ============================================================
struct NodeStack {
    Akun       data;
    string     aksi;   // "TAMBAH" atau "HAPUS"
    NodeStack* next;
    NodeStack(Akun a, string ak) : data(a), aksi(ak), next(nullptr) {}
};

struct Stack {
    NodeStack* top;
    int        ukuran;
    Stack() : top(nullptr), ukuran(0) {}

    bool isEmpty() { return top == nullptr; }

    // Push: O(1)
    void push(Akun akun, string aksi) {
        NodeStack* baru = new NodeStack(akun, aksi);
        baru->next = top;
        top = baru;
        ukuran++;
    }

    // Pop: O(1)
    NodeStack* pop() {
        if (isEmpty()) return nullptr;
        NodeStack* temp = top;
        top = top->next;
        ukuran--;
        return temp;
    }

    ~Stack() {
        while (top) {
            NodeStack* temp = top;
            top = top->next;
            delete temp;
        }
    }
};

// ============================================================
//  4. QUEUE (CIRCULAR ARRAY) - Log aktivitas (FIFO)
//     Referensi: [5] Horowitz et al. (2007), Chapter 4
// ============================================================
struct Queue {
    string data[MAX_QUEUE];
    int    front, rear, jumlah;
    Queue() : front(0), rear(-1), jumlah(0) {}

    bool isEmpty() { return jumlah == 0; }
    bool isFull()  { return jumlah >= MAX_QUEUE; }

    // Enqueue: O(1) - overwrite-oldest jika penuh
    void enqueue(string pesan) {
        if (isFull()) {
            front = (front + 1) % MAX_QUEUE;
            jumlah--;
        }
        rear = (rear + 1) % MAX_QUEUE;
        data[rear] = pesan;
        jumlah++;
    }

    // Tampilkan log: O(n)
    void tampilkan() {
        if (isEmpty()) { cout << "  [Belum ada log aktivitas]\n"; return; }
        int idx = front, n = jumlah, no = 1;
        while (n--) {
            cout << "  " << no++ << ". " << data[idx] << "\n";
            idx = (idx + 1) % MAX_QUEUE;
        }
    }
};

// ============================================================
//  5. SORTING - Bubble Sort & Quick Sort
//     Referensi: [1] Cormen et al. (2022), Chapter 2 & 7
// ============================================================

// Bubble Sort: O(n) best, O(n^2) avg/worst
void bubbleSort(Akun arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        bool sudahUrut = true;
        for (int j = 0; j < n - 1 - i; j++) {
            if (arr[j].namaAkun > arr[j + 1].namaAkun) {
                swap(arr[j], arr[j + 1]);
                sudahUrut = false;
            }
        }
        if (sudahUrut) break;
    }
}

// Partisi untuk Quick Sort: O(n)
int partisi(Akun arr[], int low, int high) {
    string pivot = arr[high].namaAkun;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j].namaAkun <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Quick Sort: O(n log n) avg, O(n^2) worst
void quickSort(Akun arr[], int low, int high) {
    if (low < high) {
        int pi = partisi(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// ============================================================
//  6. SEARCHING - Sequential Search & Binary Search
//     Referensi: [1] Cormen et al. (2022), Chapter 2
// ============================================================

// Sequential Search: O(n) - tidak perlu data terurut
int sequentialSearch(Akun arr[], int n, string target) {
    for (int i = 0; i < n; i++)
        if (arr[i].namaAkun == target) return i;
    return -1;
}

// Binary Search: O(log n) - data harus terurut
int binarySearch(Akun arr[], int n, string target) {
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid].namaAkun == target)      return mid;
        else if (arr[mid].namaAkun < target)  low  = mid + 1;
        else                                   high = mid - 1;
    }
    return -1;
}

// ============================================================
//  7. TREE (Binary Search Tree) - Indeks pencarian nama akun
//     Referensi: [1] Cormen et al. (2022), Chapter 12
// ============================================================
struct NodeBST {
    string   namaAkun; // kunci pencarian
    int      id;       // referensi ke Single Linked List
    NodeBST* kiri;
    NodeBST* kanan;
    NodeBST(string n, int i) : namaAkun(n), id(i), kiri(nullptr), kanan(nullptr) {}
};

struct BST {
    NodeBST* root;
    BST() : root(nullptr) {}

    // Sisipkan: O(log n) avg
    NodeBST* sisipkan(NodeBST* node, string nama, int id) {
        if (!node) return new NodeBST(nama, id);
        if (nama < node->namaAkun)       node->kiri  = sisipkan(node->kiri,  nama, id);
        else if (nama > node->namaAkun)  node->kanan = sisipkan(node->kanan, nama, id);
        return node;
    }
    void sisipkan(string nama, int id) { root = sisipkan(root, nama, id); }

    // Cari: O(log n) avg
    NodeBST* cari(NodeBST* node, string nama) {
        if (!node || node->namaAkun == nama) return node;
        if (nama < node->namaAkun) return cari(node->kiri,  nama);
        else                       return cari(node->kanan, nama);
    }
    NodeBST* cari(string nama) { return cari(root, nama); }

    // Hapus node dari BST: O(log n) avg
    NodeBST* hapus(NodeBST* node, string nama) {
        if (!node) return nullptr;
        if (nama < node->namaAkun)      node->kiri  = hapus(node->kiri,  nama);
        else if (nama > node->namaAkun) node->kanan = hapus(node->kanan, nama);
        else {
            if (!node->kiri)  { NodeBST* t = node->kanan; delete node; return t; }
            if (!node->kanan) { NodeBST* t = node->kiri;  delete node; return t; }
            NodeBST* succ = node->kanan;
            while (succ->kiri) succ = succ->kiri;
            node->namaAkun = succ->namaAkun;
            node->id       = succ->id;
            node->kanan    = hapus(node->kanan, succ->namaAkun);
        }
        return node;
    }
    void hapus(string nama) { root = hapus(root, nama); }

    // In-Order traversal: menampilkan urutan alfabetis O(n)
    void inOrder(NodeBST* node) {
        if (!node) return;
        inOrder(node->kiri);
        cout << "    [" << node->id << "] " << node->namaAkun << "\n";
        inOrder(node->kanan);
    }
    void tampilkanInOrder() {
        if (!root) { cout << "  [Belum ada data di BST]\n"; return; }
        cout << "  Daftar Akun (Urutan Alfabetis - In-Order BST):\n";
        inOrder(root);
    }

    ~BST() { hapusSemua(root); }
    void hapusSemua(NodeBST* node) {
        if (!node) return;
        hapusSemua(node->kiri);
        hapusSemua(node->kanan);
        delete node;
    }
};

// ============================================================
//  8. GRAPH - Relasi antar kategori akun (Adjacency Matrix)
//     Referensi: [1] Cormen et al. (2022), Chapter 22
// ============================================================
struct Graf {
    string namaVertex[MAX_GRAPH];
    int    matriks[MAX_GRAPH][MAX_GRAPH];
    int    jumlahVertex;

    Graf() : jumlahVertex(0) {
        for (int i = 0; i < MAX_GRAPH; i++)
            for (int j = 0; j < MAX_GRAPH; j++)
                matriks[i][j] = 0;
    }

    // Tambah vertex (kategori): O(V)
    int tambahVertex(string nama) {
        for (int i = 0; i < jumlahVertex; i++)
            if (namaVertex[i] == nama) return i;
        if (jumlahVertex >= MAX_GRAPH) return -1;
        namaVertex[jumlahVertex] = nama;
        return jumlahVertex++;
    }

    // Tambah sisi (relasi): O(1)
    void tambahSisi(int u, int v) {
        if (u < 0 || v < 0 || u >= MAX_GRAPH || v >= MAX_GRAPH) return;
        matriks[u][v] = 1;
        matriks[v][u] = 1; // undirected
    }

    // BFS traversal: O(V^2) dengan adjacency matrix
    void bfs(int start) {
        if (jumlahVertex == 0) return;
        bool dikunjungi[MAX_GRAPH] = {false};
        int  antrian[MAX_GRAPH], depan = 0, belakang = 0;
        dikunjungi[start] = true;
        antrian[belakang++] = start;
        cout << "  BFS dari '" << namaVertex[start] << "': ";
        while (depan < belakang) {
            int v = antrian[depan++];
            cout << namaVertex[v];
            for (int i = 0; i < jumlahVertex; i++) {
                if (matriks[v][i] && !dikunjungi[i]) {
                    dikunjungi[i] = true;
                    antrian[belakang++] = i;
                    cout << " -> " << namaVertex[i];
                }
            }
        }
        cout << "\n";
    }

    // Tampilkan seluruh relasi: O(V^2)
    void tampilkanRelasi() {
        if (jumlahVertex == 0) { cout << "  [Belum ada relasi kategori]\n"; return; }
        cout << "  Kategori terdaftar:\n";
        for (int i = 0; i < jumlahVertex; i++) {
            cout << "  [" << i << "] " << namaVertex[i] << " -> ";
            bool ada = false;
            for (int j = 0; j < jumlahVertex; j++) {
                if (matriks[i][j]) { cout << namaVertex[j] << " "; ada = true; }
            }
            if (!ada) cout << "(tidak ada relasi)";
            cout << "\n";
        }
    }
};

// ============================================================
//  GENERATOR PASSWORD RANDOM
// ============================================================
string generatePassword(int panjang = 12) {
    string kar = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*";
    string hasil = "";
    for (int i = 0; i < panjang; i++)
        hasil += kar[rand() % kar.size()];
    return hasil;
}

// ============================================================
//  TANGGAL SAAT INI
// ============================================================
string tanggalSekarang() {
    time_t now = time(0);
    tm* t = localtime(&now);
    char buf[20];
    snprintf(buf, sizeof(buf), "%02d/%02d/%04d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
    return string(buf);
}

// ============================================================
//  TAMPILAN UI
// ============================================================
void garisTebal() { cout << "  " << string(64, '=') << "\n"; }
void garisTipis() { cout << "  " << string(64, '-') << "\n"; }

void headerAplikasi() {
    system("cls || clear");
    garisTebal();
    cout << "       SISTEM MANAJEMEN PASSWORD\n";
    cout << "       Capstone Project - Struktur Data | Kelompok 2\n";
    garisTebal();
}

void menuUtama() {
    cout << "\n  MENU UTAMA\n";
    garisTipis();
    cout << "  [1]  Tambah Akun Baru\n";
    cout << "  [2]  Lihat Semua Akun (Single Linked List)\n";
    cout << "  [3]  Lihat Password Akun\n";
    cout << "  [4]  Hapus Akun\n";
    cout << "  [5]  Edit Akun\n";
    cout << "  [6]  Cari Akun (Sequential & Binary Search)\n";
    cout << "  [7]  Urutkan Akun (Bubble Sort / Quick Sort)\n";
    cout << "  [8]  Navigasi Akun (Double Linked List)\n";
    cout << "  [9]  Indeks BST (Tree - Urutan Alfabetis)\n";
    cout << "  [10] Relasi Kategori (Graph - BFS)\n";
    cout << "  [11] Generate Password Random\n";
    cout << "  [12] Undo Aksi Terakhir (Stack)\n";
    cout << "  [13] Log Aktivitas (Queue)\n";
    cout << "  [0]  Keluar\n";
    garisTipis();
    cout << "  Pilihan: ";
}

// ============================================================
//  MAIN PROGRAM
// ============================================================
int main() {
    srand((unsigned int)time(0));

    SingleLinkedList daftarAkun;   // penyimpanan utama
    DoubleLinkedList navigasiAkun; // navigasi maju/mundur
    Stack            riwayat;      // undo
    Queue            logAktivitas; // log
    BST              indeks;       // indeks pencarian
    Graf             grafKategori; // relasi kategori

    int    idCounter = 1;
    string masterPW  = "UASKelompok2";

    // --- Login ---
    headerAplikasi();
    cout << "\n  Masukkan Master Password: ";
    string inputPW;
    cin >> inputPW;

    if (inputPW != masterPW) {
        cout << "\n  [!] Password salah. Akses ditolak.\n\n";
        return 1;
    }

    logAktivitas.enqueue("[" + tanggalSekarang() + "] Login berhasil.");
    cout << "\n  [OK] Login berhasil!\n";

    int pilihan;
    do {
        headerAplikasi();
        menuUtama();
        cin >> pilihan;
        cin.ignore();

        // ---- 1. TAMBAH AKUN ----
        if (pilihan == 1) {
            headerAplikasi();
            cout << "\n  TAMBAH AKUN BARU\n";
            garisTipis();
            Akun baru;
            baru.id = idCounter++;

            cout << "  Nama Akun   (contoh: Gmail): "; getline(cin, baru.namaAkun);
            cout << "  Username    : ";                getline(cin, baru.username);
            cout << "  Password    : ";                getline(cin, baru.password);
            cout << "  Kategori    (Email/Sosmed/Bank/Lain): "; getline(cin, baru.kategori);
            baru.tanggal = tanggalSekarang();

            // Tambah ke Single Linked List
            daftarAkun.tambah(baru);
            // Tambah ke Double Linked List (untuk navigasi)
            navigasiAkun.tambah(baru);
            // Tambah ke BST (indeks pencarian)
            indeks.sisipkan(baru.namaAkun, baru.id);
            // Tambah ke Graf (relasi kategori)
            int vKat = grafKategori.tambahVertex(baru.kategori);
            int vPwd = grafKategori.tambahVertex("Password Manager");
            grafKategori.tambahSisi(vKat, vPwd);
            // Catat ke Stack & Queue
            riwayat.push(baru, "TAMBAH");
            logAktivitas.enqueue("[" + tanggalSekarang() + "] Tambah akun: " + baru.namaAkun);

            cout << "\n  [OK] Akun '" << baru.namaAkun << "' berhasil ditambahkan!\n";
        }

        // ---- 2. LIHAT SEMUA AKUN (Single Linked List) ----
        else if (pilihan == 2) {
            headerAplikasi();
            cout << "\n  DAFTAR SEMUA AKUN - Single Linked List (" << daftarAkun.jumlah << " akun)\n";
            garisTipis();
            daftarAkun.tampilkan();
        }

        // ---- 3. LIHAT PASSWORD ----
        else if (pilihan == 3) {
            headerAplikasi();
            cout << "\n  LIHAT PASSWORD AKUN\n";
            garisTipis();
            daftarAkun.tampilkan();
            cout << "  Masukkan ID akun: ";
            int id; cin >> id; cin.ignore();
            NodeLL* node = daftarAkun.cariByID(id);
            if (node) {
                cout << "\n  Nama Akun : " << node->data.namaAkun << "\n";
                cout << "  Username  : " << node->data.username << "\n";
                cout << "  Password  : " << node->data.password << "\n";
                cout << "  Kategori  : " << node->data.kategori << "\n";
                cout << "  Ditambah  : " << node->data.tanggal << "\n";
                logAktivitas.enqueue("[" + tanggalSekarang() + "] Lihat password: " + node->data.namaAkun);
            } else {
                cout << "\n  [!] ID tidak ditemukan.\n";
            }
        }

        // ---- 4. HAPUS AKUN ----
        else if (pilihan == 4) {
            headerAplikasi();
            cout << "\n  HAPUS AKUN\n";
            garisTipis();
            daftarAkun.tampilkan();
            cout << "  Masukkan ID akun yang ingin dihapus (0 = batal): ";
            int id; cin >> id; cin.ignore();

            if (id == 0) {
                cout << "\n  [INFO] Dibatalkan.\n";
            } else {
                NodeLL* node = daftarAkun.cariByID(id);
                if (node) {
                    cout << "\n  Akun yang akan dihapus: " << node->data.namaAkun << "\n";
                    cout << "  Yakin? (y = ya, n = batal): ";
                    char k; cin >> k; cin.ignore();
                    if (k == 'y' || k == 'Y') {
                        riwayat.push(node->data, "HAPUS");
                        logAktivitas.enqueue("[" + tanggalSekarang() + "] Hapus akun: " + node->data.namaAkun);
                        indeks.hapus(node->data.namaAkun);
                        navigasiAkun.hapus(id);
                        daftarAkun.hapus(id);
                        cout << "\n  [OK] Akun berhasil dihapus.\n";
                    } else {
                        cout << "\n  [INFO] Penghapusan dibatalkan.\n";
                    }
                } else {
                    cout << "\n  [!] ID tidak ditemukan.\n";
                }
            }
        }

        // ---- 5. EDIT AKUN ----
        else if (pilihan == 5) {
            headerAplikasi();
            cout << "\n  EDIT AKUN\n";
            garisTipis();
            daftarAkun.tampilkan();
            cout << "  Masukkan ID akun yang ingin diedit (0 = batal): ";
            int id; cin >> id; cin.ignore();

            if (id == 0) {
                cout << "\n  [INFO] Dibatalkan.\n";
            } else {
                NodeLL* node = daftarAkun.cariByID(id);
                if (node) {
                    cout << "\n  Edit akun: " << node->data.namaAkun << "\n";
                    cout << "  (Enter kosong = tidak diubah)\n";
                    garisTipis();

                    Akun temp = node->data;
                    string input;

                    cout << "  Nama Akun [" << temp.namaAkun << "]: ";
                    getline(cin, input); if (!input.empty()) temp.namaAkun = input;
                    cout << "  Username  [" << temp.username << "]: ";
                    getline(cin, input); if (!input.empty()) temp.username = input;
                    cout << "  Password  [" << temp.password << "]: ";
                    getline(cin, input); if (!input.empty()) temp.password = input;
                    cout << "  Kategori  [" << temp.kategori << "]: ";
                    getline(cin, input); if (!input.empty()) temp.kategori = input;

                    cout << "\n  Simpan perubahan? (y = ya, n = batal): ";
                    char k; cin >> k; cin.ignore();
                    if (k == 'y' || k == 'Y') {
                        // Update BST jika nama berubah
                        if (node->data.namaAkun != temp.namaAkun) {
                            indeks.hapus(node->data.namaAkun);
                            indeks.sisipkan(temp.namaAkun, temp.id);
                        }
                        node->data = temp;
                        navigasiAkun.update(id, temp);
                        logAktivitas.enqueue("[" + tanggalSekarang() + "] Edit akun: " + temp.namaAkun);
                        cout << "\n  [OK] Perubahan berhasil disimpan.\n";
                    } else {
                        cout << "\n  [INFO] Perubahan dibatalkan.\n";
                    }
                } else {
                    cout << "\n  [!] ID tidak ditemukan.\n";
                }
            }
        }

        // ---- 6. CARI AKUN ----
        else if (pilihan == 6) {
            headerAplikasi();
            cout << "\n  CARI AKUN\n";
            garisTipis();
            cout << "  Masukkan nama akun yang dicari: ";
            string target; getline(cin, target);

            Akun arr[MAX_ARRAY]; int n;
            daftarAkun.keArray(arr, n);

            // Sequential Search
            cout << "\n  -- Sequential Search (O(n)) --\n";
            int idx = sequentialSearch(arr, n, target);
            if (idx != -1)
                cout << "  Ditemukan! ID: " << arr[idx].id << " | User: " << arr[idx].username << "\n";
            else
                cout << "  Tidak ditemukan.\n";

            // Binary Search (perlu sorting dulu)
            cout << "\n  -- Binary Search (O(log n), data diurutkan dahulu) --\n";
            bubbleSort(arr, n);
            int idxB = binarySearch(arr, n, target);
            if (idxB != -1)
                cout << "  Ditemukan! ID: " << arr[idxB].id << " | User: " << arr[idxB].username << "\n";
            else
                cout << "  Tidak ditemukan.\n";

            // BST Search
            cout << "\n  -- BST Search (O(log n) rata-rata) --\n";
            NodeBST* hasilBST = indeks.cari(target);
            if (hasilBST)
                cout << "  Ditemukan di BST! ID: " << hasilBST->id << "\n";
            else
                cout << "  Tidak ditemukan di BST.\n";

            logAktivitas.enqueue("[" + tanggalSekarang() + "] Pencarian: " + target);
        }

        // ---- 7. URUTKAN AKUN ----
        else if (pilihan == 7) {
            headerAplikasi();
            cout << "\n  URUTKAN AKUN\n";
            garisTipis();
            cout << "  [1] Bubble Sort (O(n^2))\n";
            cout << "  [2] Quick Sort  (O(n log n))\n";
            cout << "  Pilih algoritma: ";
            int alg; cin >> alg; cin.ignore();

            Akun arr[MAX_ARRAY]; int n;
            daftarAkun.keArray(arr, n);

            if (n == 0) {
                cout << "\n  [!] Tidak ada data.\n";
            } else {
                if (alg == 1) { bubbleSort(arr, n); cout << "\n  Hasil Bubble Sort (A-Z):\n"; }
                else          { quickSort(arr, 0, n-1); cout << "\n  Hasil Quick Sort (A-Z):\n"; }
                cout << "  " << string(62, '-') << "\n";
                cout << "  " << left << setw(4) << "ID" << setw(16) << "Nama Akun"
                     << setw(20) << "Username" << "Kategori\n";
                cout << "  " << string(62, '-') << "\n";
                for (int i = 0; i < n; i++)
                    cout << "  " << setw(4) << arr[i].id << setw(16) << arr[i].namaAkun
                         << setw(20) << arr[i].username << arr[i].kategori << "\n";
            }
        }

        // ---- 8. NAVIGASI (Double Linked List) ----
        else if (pilihan == 8) {
            headerAplikasi();
            cout << "\n  NAVIGASI AKUN - Double Linked List\n";
            garisTipis();
            if (navigasiAkun.jumlah == 0) {
                cout << "  [!] Belum ada akun untuk dinavigasi.\n";
            } else {
                navigasiAkun.keAwal();
                char nav;
                do {
                    headerAplikasi();
                    cout << "\n  NAVIGASI AKUN - Double Linked List\n";
                    garisTipis();
                    navigasiAkun.tampilkanCurrent();
                    cout << "\n  [f] Maju  [b] Mundur  [q] Kembali ke menu\n";
                    cout << "  Pilihan: ";
                    cin >> nav; cin.ignore();
                    if (nav == 'f' || nav == 'F') {
                        if (!navigasiAkun.maju()) cout << "  [INFO] Sudah di akun terakhir.\n";
                    } else if (nav == 'b' || nav == 'B') {
                        if (!navigasiAkun.mundur()) cout << "  [INFO] Sudah di akun pertama.\n";
                    }
                } while (nav != 'q' && nav != 'Q');
            }
        }

        // ---- 9. INDEKS BST (Tree) ----
        else if (pilihan == 9) {
            headerAplikasi();
            cout << "\n  INDEKS BST - Tree (In-Order Traversal)\n";
            garisTipis();
            indeks.tampilkanInOrder();
        }

        // ---- 10. RELASI KATEGORI (Graph) ----
        else if (pilihan == 10) {
            headerAplikasi();
            cout << "\n  RELASI KATEGORI - Graph (Adjacency Matrix + BFS)\n";
            garisTipis();
            grafKategori.tampilkanRelasi();
            if (grafKategori.jumlahVertex > 0) {
                cout << "\n";
                grafKategori.bfs(0);
            }
        }

        // ---- 11. GENERATE PASSWORD ----
        else if (pilihan == 11) {
            headerAplikasi();
            cout << "\n  GENERATE PASSWORD RANDOM\n";
            garisTipis();
            cout << "  Panjang password (min 8, maks 32): ";
            int pj; cin >> pj; cin.ignore();
            if (pj < 8)  pj = 8;
            if (pj > 32) pj = 32;
            cout << "\n  Password yang dihasilkan: " << generatePassword(pj) << "\n";
            cout << "  (Silakan copy dan simpan password ini!)\n";
        }

        // ---- 12. UNDO (Stack) ----
        else if (pilihan == 12) {
            headerAplikasi();
            cout << "\n  UNDO AKSI TERAKHIR - Stack (LIFO)\n";
            garisTipis();
            if (riwayat.isEmpty()) {
                cout << "  [!] Tidak ada aksi yang bisa di-undo.\n";
            } else {
                NodeStack* top = riwayat.pop();
                if (top->aksi == "TAMBAH") {
                    indeks.hapus(top->data.namaAkun);
                    navigasiAkun.hapus(top->data.id);
                    daftarAkun.hapus(top->data.id);
                    cout << "  [OK] Undo: Akun '" << top->data.namaAkun << "' berhasil dihapus.\n";
                } else if (top->aksi == "HAPUS") {
                    daftarAkun.tambah(top->data);
                    navigasiAkun.tambah(top->data);
                    indeks.sisipkan(top->data.namaAkun, top->data.id);
                    cout << "  [OK] Undo: Akun '" << top->data.namaAkun << "' dikembalikan.\n";
                }
                logAktivitas.enqueue("[" + tanggalSekarang() + "] Undo: " + top->aksi + " " + top->data.namaAkun);
                delete top;
            }
        }

        // ---- 13. LOG AKTIVITAS (Queue) ----
        else if (pilihan == 13) {
            headerAplikasi();
            cout << "\n  LOG AKTIVITAS - Queue (FIFO)\n";
            garisTipis();
            logAktivitas.tampilkan();
        }

        if (pilihan != 0) {
            cout << "\n  Tekan Enter untuk kembali ke menu...";
            cin.get();
        }

    } while (pilihan != 0);

    headerAplikasi();
    cout << "\n  Terima kasih telah menggunakan Sistem Manajemen Password.\n";
    cout << "  Program selesai.\n\n";
    return 0;
}
