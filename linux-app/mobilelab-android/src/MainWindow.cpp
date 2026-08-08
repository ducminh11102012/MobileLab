#include "MainWindow.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QSplitter>
#include <QStyle>
#include <QVBoxLayout>
#include <QFrame>

namespace {
const char *kStyle = R"(
QMainWindow,QWidget{background:#111318;color:#e7e9ee;font-family:"Inter","Noto Sans",sans-serif;font-size:13px;}
QFrame#sidebar,QFrame#inspector,QFrame#toolbar,QFrame#console{background:#17191f;}
QLabel#brand{font-size:17px;font-weight:700;color:#ffffff;}
QLabel#muted{color:#858b98;font-size:11px;}
QPushButton#nav{border:0;border-radius:7px;text-align:left;padding:9px 12px;color:#aeb4c0;background:transparent;}
QPushButton#nav:hover{background:#22252d;color:#ffffff;}
QPushButton#nav[selected="true"]{background:#2b303a;color:#ffffff;font-weight:600;}
QPushButton#primary{background:#3f7cff;color:white;border:0;border-radius:7px;padding:7px 14px;font-weight:600;}
QPushButton#secondary{background:#242832;color:#d9dce3;border:1px solid #343945;border-radius:7px;padding:7px 12px;}
QLabel#title{font-size:20px;font-weight:700;color:#fff;}
QLabel#cardTitle{font-size:11px;font-weight:600;color:#9da4b1;}
QLabel#metric{font-size:25px;font-weight:700;color:#fff;}
QFrame#card{background:#191c23;border:1px solid #292d36;border-radius:10px;}
QListWidget,QTableWidget,QTextEdit{background:#15171d;border:1px solid #292d36;border-radius:8px;color:#dfe2e8;}
QListWidget::item{padding:10px;border-radius:6px;}
QListWidget::item:selected{background:#292f3a;color:white;}
QHeaderView::section{background:#1c1f26;color:#8f96a3;border:0;border-bottom:1px solid #292d36;padding:8px;font-size:11px;}
QTableWidget::item{padding:7px;border-bottom:1px solid #22252c;}
QProgressBar{background:#242832;border:0;border-radius:4px;height:7px;text-align:left;}
QProgressBar::chunk{background:#5b8cff;border-radius:4px;}
QLineEdit{background:#20232b;border:1px solid #30343e;border-radius:7px;padding:7px;color:#fff;}
QComboBox{background:#20232b;border:1px solid #30343e;border-radius:7px;padding:6px;color:#ddd;}
QScrollBar:vertical{background:#15171d;width:9px;margin:0;border:0;} QScrollBar::handle:vertical{background:#363b46;border-radius:4px;min-height:25px;}
)";

QLabel *label(const QString &text, const QString &name = {}) {
    auto *l = new QLabel(text);
    if (!name.isEmpty()) l->setObjectName(name);
    return l;
}

QFrame *card(const QString &title, const QString &value, const QString &sub) {
    auto *f = new QFrame;
    f->setObjectName("card");
    auto *v = new QVBoxLayout(f);
    v->setContentsMargins(15, 13, 15, 13);
    v->setSpacing(5);
    v->addWidget(label(title.toUpper(), "cardTitle"));
    v->addWidget(label(value, "metric"));
    v->addWidget(label(sub, "muted"));
    return f;
}
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("MobileLab Android");
    resize(1440, 900);
    setMinimumSize(1050, 680);
    qApp->setStyleSheet(kStyle);

    auto *root = new QWidget;
    auto *layout = new QHBoxLayout(root);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    layout->addWidget(makeSidebar());

    auto *center = new QWidget;
    auto *cv = new QVBoxLayout(center);
    cv->setContentsMargins(0, 0, 0, 0);
    cv->setSpacing(0);
    cv->addWidget(makeTopBar());

    pages_ = new QStackedWidget;
    pages_->addWidget(makeDashboard());
    pages_->addWidget(makeDevices());
    pages_->addWidget(makeMatrix());
    pages_->addWidget(makeConsole());
    cv->addWidget(pages_, 1);
    layout->addWidget(center, 1);
    layout->addWidget(makeInspector());

    setCentralWidget(root);

    pollTimer_ = new QTimer(this);
    connect(pollTimer_, &QTimer::timeout, this, [this] {
        static int tick = 0;
        ++tick;
        if (active_) active_->setText(QString("%1 Active").arg(3 + tick % 4));
        if (cpu_) cpu_->setValue(35 + tick % 30);
        if (memory_) memory_->setValue(48 + tick % 20);
    });
    pollTimer_->start(1800);
}

QPushButton *MainWindow::navButton(const QString &text, const QString &key) {
    auto *b = new QPushButton(text);
    b->setObjectName("nav");
    b->setProperty("selected", key == "devices");
    return b;
}

QWidget *MainWindow::makeSidebar() {
    auto *frame = new QFrame;
    frame->setObjectName("sidebar");
    frame->setFixedWidth(235);
    auto *v = new QVBoxLayout(frame);
    v->setContentsMargins(14, 18, 14, 14);
    v->setSpacing(4);

    auto *brand = label("MobileLab", "brand");
    v->addWidget(brand);
    v->addWidget(label("ANDROID · ARM64", "muted"));
    v->addSpacing(18);

    const QStringList nav = {"⌘  Overview", "▦  Devices", "▤  Test Matrix", "◌  Live Console", "◫  Reports", "◇  Snapshots", "⚙  Settings"};
    for (int i = 0; i < nav.size(); ++i) {
        auto *b = new QPushButton(nav[i]);
        b->setObjectName("nav");
        b->setProperty("selected", i == 0);
        connect(b, &QPushButton::clicked, this, [this, i, b] {
            selectPage(qMin(i, pages_->count() - 1));
            for (auto *w : b->parentWidget()->findChildren<QPushButton*>("nav")) w->setProperty("selected", w == b);
            b->style()->unpolish(b); b->style()->polish(b);
        });
        v->addWidget(b);
    }

    v->addStretch();
    auto *status = new QFrame;
    status->setObjectName("card");
    auto *sv = new QVBoxLayout(status);
    sv->setContentsMargins(11, 10, 11, 10);
    sv->addWidget(label("ARM64 LINUX", "cardTitle"));
    sv->addWidget(label("● Native backend", "muted"));
    sv->addWidget(label("KVM: detected at runtime", "muted"));
    v->addWidget(status);
    return frame;
}

QWidget *MainWindow::makeTopBar() {
    auto *bar = new QFrame;
    bar->setObjectName("toolbar");
    bar->setFixedHeight(58);
    auto *h = new QHBoxLayout(bar);
    h->setContentsMargins(16, 8, 16, 8);
    h->addWidget(label("Android Device Lab", "title"));
    h->addSpacing(18);
    auto *scheme = new QComboBox;
    scheme->addItems({"ARM64 Native Validation", "Android Emulator Matrix", "CI Validation Core"});
    scheme->setFixedWidth(220);
    h->addWidget(scheme);
    h->addStretch();
    connection_ = label("●  Connected · ARM64 Linux", "muted");
    h->addWidget(connection_);
    auto *run = new QPushButton("▶  Run Matrix");
    run->setObjectName("primary");
    connect(run, &QPushButton::clicked, this, [this] {
        console_->append("[MobileLab] Starting Android ARM64 validation matrix...");
        console_->append("[scheduler] Selecting native Linux execution targets.");
    });
    h->addWidget(run);
    return bar;
}

QWidget *MainWindow::makeDashboard() {
    auto *page = new QWidget;
    auto *v = new QVBoxLayout(page);
    v->setContentsMargins(22, 22, 22, 18);
    v->setSpacing(14);
    v->addWidget(label("Overview", "title"));
    v->addWidget(label("Android orchestration on ARM64 Linux", "muted"));

    auto *cards = new QHBoxLayout;
    cards->setSpacing(10);
    cards->addWidget(card("Devices", "8", "3 active · 5 idle"));
    auto *activeCard = card("Running", "3", "Android workloads");
    active_ = activeCard->findChild<QLabel*>("metric");
    cards->addWidget(activeCard);
    cards->addWidget(card("Test runs", "24", "6 passed today"));
    cards->addWidget(card("Failures", "0", "No blocking failures"));
    v->addLayout(cards);

    auto *split = new QSplitter(Qt::Horizontal);
    auto *left = new QFrame; left->setObjectName("card");
    auto *lv = new QVBoxLayout(left); lv->setContentsMargins(14,14,14,14);
    lv->addWidget(label("DEVICE POOL", "cardTitle"));
    deviceList_ = new QListWidget;
    deviceList_->addItems({"Pixel 9 Pro · Android 15 · arm64", "Pixel 8 · Android 14 · arm64", "Tablet API 35 · arm64", "Android CI Worker · native"});
    lv->addWidget(deviceList_);
    split->addWidget(left);

    auto *right = new QFrame; right->setObjectName("card");
    auto *rv = new QVBoxLayout(right); rv->setContentsMargins(14,14,14,14);
    rv->addWidget(label("HOST RESOURCES", "cardTitle"));
    rv->addWidget(label("CPU", "muted")); cpu_ = new QProgressBar; cpu_->setValue(46); rv->addWidget(cpu_);
    rv->addWidget(label("Memory", "muted")); memory_ = new QProgressBar; memory_->setValue(57); rv->addWidget(memory_);
    rv->addSpacing(12);
    rv->addWidget(label("Execution policy", "cardTitle"));
    rv->addWidget(label("Native ARM64 workloads are preferred. Virtualization-dependent workloads are rejected when KVM is unavailable.", "muted"));
    rv->addStretch();
    split->addWidget(right);
    split->setSizes({600, 450});
    v->addWidget(split, 1);

    v->addWidget(label("RECENT ACTIVITY", "cardTitle"));
    auto *activity = new QTextEdit;
    activity->setReadOnly(true);
    activity->setMaximumHeight(120);
    activity->setPlainText("10:42  Android API 35 build validation       PASSED\n10:38  ARM64 native test suite              PASSED\n10:21  Pixel 9 Pro smoke tests              PASSED\n09:56  Dependency compatibility check       PASSED");
    v->addWidget(activity);
    return page;
}

QWidget *MainWindow::makeDevices() {
    auto *page = new QWidget;
    auto *v = new QVBoxLayout(page); v->setContentsMargins(22,22,22,18);
    v->addWidget(label("Devices", "title"));
    v->addWidget(label("Managed Android targets and ARM64 execution environments", "muted"));
    auto *list = new QListWidget;
    list->addItems({"Pixel 9 Pro     Android 15     arm64     READY", "Pixel 8          Android 14     arm64     RUNNING", "Tablet API 35    Android 15     arm64     READY", "Native Worker    Ubuntu ARM64   native   RUNNING"});
    v->addWidget(list, 1);
    return page;
}

QWidget *MainWindow::makeMatrix() {
    auto *page = new QWidget;
    auto *v = new QVBoxLayout(page); v->setContentsMargins(22,22,22,18);
    v->addWidget(label("Test Matrix", "title"));
    v->addWidget(label("Cross-version Android validation", "muted"));
    matrix_ = new QTableWidget(5, 5);
    matrix_->setHorizontalHeaderLabels({"Target", "Android", "Arch", "Backend", "Status"});
    const QString data[5][5] = {
        {"Pixel 9 Pro", "15", "arm64", "native", "PASS"},
        {"Pixel 8", "14", "arm64", "native", "PASS"},
        {"Tablet API 35", "15", "arm64", "userspace", "RUNNING"},
        {"Build worker", "15", "arm64", "native", "PASS"},
        {"Compatibility", "14–15", "arm64", "native", "QUEUED"}
    };
    for (int r=0;r<5;++r) for (int c=0;c<5;++c) matrix_->setItem(r,c,new QTableWidgetItem(data[r][c]));
    matrix_->horizontalHeader()->setStretchLastSection(true);
    matrix_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    v->addWidget(matrix_,1);
    return page;
}

QWidget *MainWindow::makeConsole() {
    auto *page = new QWidget;
    auto *v = new QVBoxLayout(page); v->setContentsMargins(22,22,22,18);
    v->addWidget(label("Live Console", "title"));
    v->addWidget(label("Scheduler and Android backend output", "muted"));
    console_ = new QTextEdit;
    console_->setReadOnly(true);
    console_->setFont(QFont("JetBrains Mono", 10));
    console_->setPlainText("[MobileLab] Android ARM64 backend initialized.\n[capabilities] architecture=arm64\n[capabilities] platform=linux\n[capabilities] kvm=runtime-detected\n[scheduler] waiting for workload...");
    v->addWidget(console_,1);
    return page;
}

QWidget *MainWindow::makeInspector() {
    auto *frame = new QFrame; frame->setObjectName("inspector"); frame->setFixedWidth(270);
    auto *v = new QVBoxLayout(frame); v->setContentsMargins(14,18,14,14); v->setSpacing(10);
    v->addWidget(label("Inspector", "title"));
    v->addWidget(label("Selected device", "cardTitle"));
    auto *device = new QFrame; device->setObjectName("card");
    auto *dv = new QVBoxLayout(device); dv->setContentsMargins(12,12,12,12);
    dv->addWidget(label("Pixel 9 Pro", "brand"));
    dv->addWidget(label("Android 15 · arm64", "muted"));
    dv->addSpacing(8);
    dv->addWidget(label("STATUS", "cardTitle"));
    dv->addWidget(label("● Ready", "muted"));
    dv->addWidget(label("BACKEND", "cardTitle"));
    dv->addWidget(label("Native ARM64", "muted"));
    v->addWidget(device);
    v->addWidget(label("Actions", "cardTitle"));
    auto *shell = new QPushButton("Open Shell"); shell->setObjectName("secondary"); v->addWidget(shell);
    auto *screenshot = new QPushButton("Capture Screenshot"); screenshot->setObjectName("secondary"); v->addWidget(screenshot);
    auto *restart = new QPushButton("Restart Device"); restart->setObjectName("secondary"); v->addWidget(restart);
    v->addSpacing(12);
    v->addWidget(label("Connection", "cardTitle"));
    v->addWidget(label("Backend: 127.0.0.1:4000", "muted"));
    v->addWidget(label("Protocol: REST / MCP", "muted"));
    v->addStretch();
    return frame;
}

QLabel *MainWindow::statusLabel(const QString &icon, const QString &text) { return label(icon + "  " + text, "muted"); }

void MainWindow::selectPage(int index) {
    if (pages_) pages_->setCurrentIndex(index);
}
