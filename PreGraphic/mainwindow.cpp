#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pb_clearResult->setCheckable(true);

    connect(this, &MainWindow::dataReadyForPlot, this, &MainWindow::plotData);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QVector<uint32_t> MainWindow::ReadFile(QString path, uint8_t numberChannel)
{
    QFile file(path);
    file.open(QIODevice::ReadOnly);

    if(!file.isOpen()){
        if(pathToFile.isEmpty()){
            QMessageBox mb;
            mb.setWindowTitle("Ошибка");
            mb.setText("Ошибка открытия файла");
            mb.exec();
        }
    }

    QDataStream dataStream;
    dataStream.setDevice(&file);
    dataStream.setByteOrder(QDataStream::LittleEndian);

    QVector<uint32_t> readData;
    readData.clear();
    uint32_t currentWorld = 0, sizeFrame = 0;

    while(!dataStream.atEnd()){
        dataStream >> currentWorld;

        if(currentWorld == 0xFFFFFFFF){
            dataStream >> currentWorld;

            if(currentWorld < 0x80000000){
                dataStream >> sizeFrame;

                if(sizeFrame > 1500){
                    continue;
                }

                for(int i = 0; i < sizeFrame/sizeof(uint32_t); i++){
                    dataStream >> currentWorld;

                    if((currentWorld >> 24) == numberChannel){
                        readData.append(currentWorld);
                    }
                }
            }
        }
    }
    ui->chB_readSucces->setChecked(true);
    return readData;
}

QVector<double> MainWindow::ProcessFile(const QVector<uint32_t> dataFile)
{
    QVector<double> resultData;
    resultData.clear();

    foreach (int word, dataFile) {
        word &= 0x00FFFFFF;
        if(word > 0x800000){
            word -= 0x1000000;
        }

        double res = ((double)word/6000000)*10;
        resultData.append(res);
    }
    ui->chB_procFileSucces->setChecked(true);

    return resultData;
}

QVector<double> MainWindow::FindMax(QVector<double> resultData)
{
    double max = 0, sMax = 0;
    foreach (double num, resultData){
        if(num > max){
            max = num;
        }
    }

    foreach (double num, resultData){
        if(num > sMax && !qFuzzyCompare(num, max)){
            sMax = num;
        }
    }

    QVector<double> maxs = {max, sMax};
    ui->chB_maxSucess->setChecked(true);
    return maxs;
}

QVector<double> MainWindow::FindMin(QVector<double> resultData)
{
    double min = 0, sMin = 0;
    foreach (double num, resultData){
        if(num < min){
            min = num;
        }
    }

    foreach (double num, resultData){
        if(num < sMin && !qFuzzyCompare(num, min)){
            sMin = num;
        }
    }

    QVector<double> mins = {min, sMin};
    ui->chB_minSucess->setChecked(true);
    return mins;
}

void MainWindow::DisplayResult(QVector<double> mins, QVector<double> maxs)
{
    ui->te_Result->append("Расчет закончен!");
    ui->te_Result->append("Первый минимум " + QString::number(mins.first()));
    ui->te_Result->append("Второй минимум " + QString::number(mins.at(1)));
    ui->te_Result->append("Первый максимум " + QString::number(maxs.first()));
    ui->te_Result->append("Второй максимум " + QString::number(maxs.at(1)));
}

void MainWindow::plotData(const QVector<double>& data)
{
    QWidget *chartWindow = new QWidget();
    chartWindow->setWindowTitle("График данных");
    chartWindow->resize(800, 600);

    QChart *chart = new QChart();
    QLineSeries *series = new QLineSeries();

    int pointsToShow = qMin(data.size(), 1000);
    for(int i = 0; i < pointsToShow; ++i) {
        series->append(i, data[i]);
    }

    chart->addSeries(series);
    chart->legend()->hide();

    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("Отсчеты");
    axisX->setRange(0, pointsToShow);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Амплитуда, В");

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *layout = new QVBoxLayout(chartWindow);
    layout->addWidget(chartView);
    chartWindow->setLayout(layout);

    chartWindow->show();
}

void MainWindow::on_pb_path_clicked()
{
    pathToFile = "";
    ui->le_path->clear();
    pathToFile = QFileDialog::getOpenFileName(this,
                                              tr("Открыть файл"),
                                              "/home/",
                                              tr("ADC Files (*.adc)"));
    ui->le_path->setText(pathToFile);
}

void MainWindow::on_pb_start_clicked()
{
    if(pathToFile.isEmpty()){
        QMessageBox mb;
        mb.setWindowTitle("Ошибка");
        mb.setText("Выберите файл!");
        mb.exec();
        return;
    }

    ui->chB_maxSucess->setChecked(false);
    ui->chB_procFileSucces->setChecked(false);
    ui->chB_readSucces->setChecked(false);
    ui->chB_minSucess->setChecked(false);

    int selectIndex = ui->cmB_numCh->currentIndex();
    if(selectIndex == 0){
        numberSelectChannel = 0xEA;
    }
    else if(selectIndex == 1){
        numberSelectChannel = 0xEF;
    }
    else if(selectIndex == 2){
        numberSelectChannel = 0xED;
    }

    auto read = [&]{ return ReadFile(pathToFile, numberSelectChannel); };
    auto process = [&](QVector<uint32_t> res){ return ProcessFile(res); };
    auto findMax = [&](QVector<double> res){
        maxs = FindMax(res);
        mins = FindMin(res);
        DisplayResult(mins, maxs);
        emit dataReadyForPlot(res);
    };

    QtConcurrent::run(read)
        .then(process)
        .then(findMax);
}
