#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(tr("ImageTransformer"));

    QMenu* menuG = findChild<QMenu*>("menu_G");
    if (menuG)
    {
        // 创建一个菜单项
        //QAction* menuItem = new QAction(tr("Show Message"), this);

        // 将菜单项添加到菜单中
        //menuG->addAction(menuItem);

        // 连接菜单项的触发信号到槽函数
        connect(ui->action_4, &QAction::triggered, this, &MainWindow::on_action_4_clicked);
        connect(ui->action_5, &QAction::triggered, this, &MainWindow::on_action_5_clicked);

    }
    else
    {
        qDebug() << "无法找到名为 'menu_G' 的菜单";
    }
    p = new method();
    connect(ui->action, &QAction::triggered, this, &MainWindow::openActionSlot);
    connect(ui->action_2, &QAction::triggered, this, &MainWindow::saveActionSlot);
    connect(ui->action_3, &QAction::triggered, this, &MainWindow::saveAction2Slot);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete p;
}

// 假设你的 MainWindow 类中有一个 QLabel 指针 label_5
void MainWindow::openActionSlot()
{
    QString fileName = QFileDialog::getOpenFileName(this, QStringLiteral("选择一张图片"), ("/home"), (tr("Images (*.png *.xpm *.jpg *.bmp)")));
    if (fileName.isEmpty())
    {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("请选择一个文件"));
    }
    else
    {
        QPixmap pixmap;
        if (!pixmap.load(fileName))
        {
            QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("无法加载图片"));
            return;
        }

        // 将读取到的图片设置到 label_5 上
        ui->label_5->setPixmap(pixmap);

        // 调整 QLabel 的大小以适应图片（可选）
        ui->label_5->setScaledContents(true);
        p->origImg= imread(fileName.toLocal8Bit().toStdString(), 0);
        p->w = p->origImg.cols;
        p->h = p->origImg.rows;
    }
}

void MainWindow::saveActionSlot()
{
    // 打开文件保存对话框，让用户选择保存的文件路径和文件名
    QString fileName = QFileDialog::getSaveFileName(this, QStringLiteral("保存图片"), "/home", tr("Images (*.png *.xpm *.jpg *.bmp)"));

    // 检查用户是否选择了文件
    if (fileName.isEmpty())
    {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("请选择一个文件"));
        return;
    }

    // 获取 QLabel 上显示的 QPixmap 对象指针
    QPixmap pixmap = ui->label_3->pixmap();

    // 检查 QPixmap 对象是否有效
    if (!pixmap || pixmap.isNull())
    {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("没有可保存的图片"));
        return;
    }

    // 保存图片到指定文件
    if (!pixmap.save(fileName))
    {
        QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("无法保存图片"));
    }
}

void MainWindow::saveAction2Slot()
{
    // 打开文件保存对话框，让用户选择保存的文件路径和文件名
    QString fileName = QFileDialog::getSaveFileName(this, QStringLiteral("保存图片"), "/home", tr("Images (*.png *.xpm *.jpg *.bmp)"));

    // 检查用户是否选择了文件
    if (fileName.isEmpty())
    {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("请选择一个文件"));
        return;
    }

    // 获取 QLabel 上显示的 QPixmap 对象指针
    QPixmap pixmap = ui->label_9->pixmap();

    // 检查 QPixmap 对象是否有效
    if (!pixmap || pixmap.isNull())
    {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("没有可保存的图片"));
        return;
    }

    // 保存图片到指定文件
    if (!pixmap.save(fileName))
    {
        QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("无法保存图片"));
    }
}

void MainWindow::on_action_4_clicked()
{
    // 弹出消息框
    QMessageBox::information(this, tr("帮助"), tr("选择STC算法时，四个参数均要输入。其余算法只需输入参数：同类快阈值。"));
}

void MainWindow::on_action_5_clicked()
{
    // 弹出消息框
    QMessageBox::information(this, tr("关于"), tr("此软件实现于2024年6月19日，算法实现由章子由完成，QT设计由雷满完成，论文分析由简子航完成，QT调试由宋晨轩完成。\n版本号：1.0.1"));
}

void MainWindow::on_cin1_editingFinished() {
    ui->cin1->setMaxLength(2);
    p->xigema = ui->cin1->text().toInt();
}

void MainWindow::on_cin2_editingFinished() {
    ui->cin2->setMaxLength(2);
    p->thresU_ = ui->cin2->text().toDouble();
}

void MainWindow::on_cin3_editingFinished() {
    ui->cin3->setMaxLength(3);
    p->thresVar_ = ui->cin3->text().toDouble();
}

void MainWindow::on_cin4_editingFinished() {
    ui->cin4->setMaxLength(1);
    p->num_ = ui->cin4->text().toInt();
}

void MainWindow::on_radioButton_clicked() {
    p->methodId = 5;
}

void MainWindow::on_radioButton_2_clicked() {
    p->methodId = 6;
}

void MainWindow::on_radioButton_3_clicked() {
    p->methodId = 4;
}

void MainWindow::on_radioButton_4_clicked() {
    p->methodId = 2;
}

void MainWindow::on_radioButton_5_clicked() {
    p->methodId = 3;
}

void MainWindow::on_radioButton_6_clicked() {
    p->methodId = 1;
}

void MainWindow::on_radioButton_7_clicked() {
    p->methodId = 7;
}

void MainWindow::on_radioButton_8_clicked() {
    p->methodId = 8;
}

void MainWindow::on_pushButton_QPushButton() {
    if (p->methodId == -1) {
        QMessageBox::warning(this, ("警告"), QStringLiteral("请选择一个算法"));
        return;
    }
    else if(p->methodId == 8){
        stc* p_ = new stc(p);
        p_->process();
        p->enTime = p_->enTime;
        p->deTime = p_->deTime;
        p->blocknum = p_->blocknum;
        p->PSNR = p_->PSNR;
        p->CR = p_->CR;
        p->BPP = p_->BPP;
        p->compressedImg = p_->compressedImg;
        p->segImg = p_->segImg;
        delete p_;
    }
    else {
        rnam* p_ = new rnam(p);
        p_->process();
        p->enTime = p_->enTime;
        p->deTime = p_->deTime;
        p->blocknum = p_->blocknum;
        p->PSNR = p_->PSNR;
        p->CR = p_->CR;
        p->BPP = p_->BPP;
        p->compressedImg = p_->compressedImg;
        p->segImg = p_->segImg;
        delete p_;
    }
    
    QString str;
    QString tmpstr;

    tmpstr = QString::number(p->enTime);
    str = "编码所用时间: " + tmpstr + "ms";
    ui->encodetime->setText(str);

    tmpstr = QString::number(p->deTime);
    str = "解码所用时间: " + tmpstr + "ms";
    ui->decodetime->setText(str);

    tmpstr = QString::number(p->blocknum);
    str = "分块数: " + tmpstr;
    ui->numseg->setText(str);

    tmpstr = QString::number(p->PSNR);
    str = "PSNR: " + tmpstr;
    ui->PSNR->setText(str);

    tmpstr = QString::number(p->CR);
    str = "CR: " + tmpstr;
    ui->SNR->setText(str);

    tmpstr = QString::number(p->BPP);
    str = "BPP: " + tmpstr;
    ui->BPP->setText(str);

    QImage d, s;
    d = cvMatToQImage(p->compressedImg);
    s= cvMatToQImage(p->segImg);
    QPixmap d_= QPixmap::fromImage(d);
    QPixmap s_ = QPixmap::fromImage(s);
    
    // 将读取到的图片设置到 label_3 上
    ui->label_3->setPixmap(d_);
    ui->label_3->setScaledContents(true);

    // 将读取到的图片设置到 label_9 上
    ui->label_9->setPixmap(s_);
    ui->label_9->setScaledContents(true);
}