#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QStringList>
#include <QMessageBox>

#include <connection.h>
#include <device.h>
#include <datanode.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setTabText(0, "Connection");
    ui->tabWidget->setTabText(2, "Devices List");
    ui->tabWidget->setTabText(1, "Create device");
    ui->tabWidget->setTabText(3, "DataNode read/write simulation");

    setupReadDevicesTab();
    setupCreateDeviceTab();
    setupSimulationTab();

    connect(ui->userNameEdit, SIGNAL(textEdited(QString)), this, SLOT(onUserNameChanged(QString)));
    connect(ui->passwordEdit, SIGNAL(textEdited(QString)), this, SLOT(onPasswordChanged(QString)));
    connect(&m_deviceList, SIGNAL(getFinished(bool)), this, SLOT(onDeviceListUpdated(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onUserNameChanged(const QString &userName)
{
    iot::Connection::instance()->setUserName(userName);
}

void MainWindow::onPasswordChanged(const QString &password)
{
    iot::Connection::instance()->setPassword(password);
}

void MainWindow::onDeviceListUpdated(bool success)
{
    qDebug() << "onDeviceListUpdated, success: " << success;
    QList<iot::Device*> deviceList = m_deviceList.devices();
    m_deviceListWidget->clear();
    QStringList deviceNames;
    foreach(iot::Device* device, deviceList) {
        deviceNames.append(device->name());
    }
    m_deviceListWidget->addItems(deviceNames);
    if (deviceNames.count() > 0) m_deviceListWidget->setCurrentRow(0);
}

void MainWindow::onDeviceRegistrationFinished(bool success)
{
    QMessageBox message;
    message.setText(success ? "Device registered" : "Device registration failed");
    message.exec();
}

void MainWindow::onUpdateDeviceDetails(int index)
{
    m_deviceDetailsWidget->clear();
    QList<iot::Device*> deviceList = m_deviceList.devices();
    if (index >= 0 && index < deviceList.count()) {
        iot::Device* focusDevice = deviceList.at(index);
        QString list;
        list += ("<b>Manufactorer:</b> " + focusDevice->manufacturer() + "<br>");
        list += ("<b>Type:</b> " + focusDevice->type() + "<br>");
        list += ("<b>Description:</b> " + focusDevice->description() + "<br>");
        list += ("<b>Registration time:</b> " + focusDevice->creationTime().toString() + "<br>");
        list += ("<b>Device Id:</b> " + focusDevice->deviceId());
        m_deviceDetailsWidget->setText(list);
        connect(focusDevice, SIGNAL(getDataNodesFinished(bool)), this, SLOT(onDeviceDataNodesFetched(bool)));
        focusDevice->getDataNodes();
    }
}

void MainWindow::onDeviceDataNodesFetched(bool success)
{
    iot::Device* device = qobject_cast<iot::Device*>(sender());
    if (success) {
        QList<iot::DataNode*> dataNodes = device->dataNodes();
        qDebug() << "device datanodes are: ";
        foreach(iot::DataNode* node, dataNodes) {
            qDebug() << node->name() << node->path();
        }
    } else {
        qDebug() << "resolving datanodes failed";
    }
}

void MainWindow::onStartSimulation()
{
    if (m_simulationTimer.isActive()) {
        ui->startSimulationButton->setText("Start simulation");
        m_simulationTimer.stop();
    } else {
        ui->startSimulationButton->setText("Stop simulation");
        m_simulationTimer.start();
    }
}

void MainWindow::onWriteDataNodeValue()
{
    int writtenValue = qrand();
    ui->writtenValueLabel->setText(QString::number(writtenValue));
    m_writeDataNode.writeValue(writtenValue);
}

void MainWindow::onReadLatestValue()
{
    m_readDataNode.readLatestValue();
}

void MainWindow::onLatestValueRead(bool success)
{
    if (success) {
        ui->readValueLabel->setText(m_readDataNode.latestValue().first.toString());
    }
}

void MainWindow::setupReadDevicesTab()
{
    // Root layout for device list tab
    QVBoxLayout* rootLayout = new QVBoxLayout();
    ui->deviceListTab->setLayout(rootLayout);
    // Button to read device list
    QHBoxLayout* layout = new QHBoxLayout();
    rootLayout->addLayout(layout);
    QPushButton* readDeviceListButton = new QPushButton("Update device list");
    layout->addWidget(readDeviceListButton);
    layout->addStretch(0);

    // Stuff to present the device list and device details
    QHBoxLayout* layout2 = new QHBoxLayout();
    m_deviceListWidget = new QListWidget();
    m_deviceDetailsWidget = new QLabel();
    m_deviceDetailsWidget->setAlignment(Qt::AlignLeft|Qt::AlignTop);
    layout2->addWidget(m_deviceListWidget);
    layout2->addWidget(m_deviceDetailsWidget);
    rootLayout->addLayout(layout2);

    connect(readDeviceListButton, SIGNAL(clicked(bool)), &m_deviceList, SLOT(get()));
    connect(m_deviceListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(onUpdateDeviceDetails(int)));
}

void MainWindow::setupCreateDeviceTab()
{
    // Root layout for device list tab
    QFormLayout* rootLayout = new QFormLayout();
    ui->createDeviceTab->setLayout(rootLayout);
    QLineEdit* deviceNameEdit = new QLineEdit();
    QLineEdit* deviceManufacturerEdit = new QLineEdit();
    QLineEdit* deviceTypeEdit = new QLineEdit();
    QLineEdit* deviceDescriptionEdit = new QLineEdit();

    rootLayout->addRow("Device name:", deviceNameEdit);
    rootLayout->addRow("Device manufacturer:", deviceManufacturerEdit);
    rootLayout->addRow("Device type:", deviceTypeEdit);
    rootLayout->addRow("Device description:", deviceDescriptionEdit);

    connect(deviceNameEdit, SIGNAL(textEdited(QString)), &m_createdDevice, SLOT(setName(QString)));
    connect(deviceManufacturerEdit, SIGNAL(textEdited(QString)), &m_createdDevice, SLOT(setManufacturer(QString)));
    connect(deviceTypeEdit, SIGNAL(textEdited(QString)), &m_createdDevice, SLOT(setType(QString)));
    connect(deviceDescriptionEdit, SIGNAL(textEdited(QString)), &m_createdDevice, SLOT(setDescription(QString)));

    QPushButton* createDeviceButton = new QPushButton("Create device");
    connect(createDeviceButton, SIGNAL(clicked(bool)), &m_createdDevice, SLOT(registerDevice()));
    rootLayout->addWidget(createDeviceButton);

    connect(&m_createdDevice, SIGNAL(registerFinished(bool)), this, SLOT(onDeviceRegistrationFinished(bool)));
}

void MainWindow::setupSimulationTab()
{
    connect(ui->dataNodeDeviceIdLineEdit, SIGNAL(textEdited(QString)), &m_writeDataNode, SLOT(setDeviceId(QString)));
    connect(ui->dataNodeDeviceIdLineEdit, SIGNAL(textEdited(QString)), &m_readDataNode, SLOT(setDeviceId(QString)));
    connect(ui->dataNodeNameEdit, SIGNAL(textEdited(QString)), &m_writeDataNode, SLOT(setName(QString)));
    connect(ui->dataNodePathLineEdit, SIGNAL(textEdited(QString)), &m_writeDataNode, SLOT(setPath(QString)));
    connect(ui->dataNodeNameEdit, SIGNAL(textEdited(QString)), &m_readDataNode, SLOT(setName(QString)));
    connect(ui->dataNodePathLineEdit, SIGNAL(textEdited(QString)), &m_readDataNode, SLOT(setPath(QString)));
    connect(ui->dataNodeUnitLineEdit, SIGNAL(textEdited(QString)), &m_writeDataNode, SLOT(setUnit(QString)));
    connect(ui->startSimulationButton, SIGNAL(clicked(bool)), this, SLOT(onStartSimulation()));

    m_simulationTimer.setInterval(3000);
    connect(&m_simulationTimer, SIGNAL(timeout()), this, SLOT(onWriteDataNodeValue()));
    connect(&m_simulationTimer, SIGNAL(timeout()), this, SLOT(onReadLatestValue()));
    connect(&m_readDataNode, SIGNAL(readFinished(bool)), this, SLOT(onLatestValueRead(bool)));
}
