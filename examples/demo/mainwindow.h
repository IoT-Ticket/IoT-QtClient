#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>

#include <devicelist.h>
#include <device.h>
#include <datanode.h>

namespace Ui {
class MainWindow;
}

class QListWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void onUserNameChanged(const QString& userName);
    void onPasswordChanged(const QString& password);
    void onDeviceListUpdated(bool success);
    void onDeviceRegistrationFinished(bool success);
    void onUpdateDeviceDetails(int index);
    void onDeviceDataNodesFetched(bool success);
    void onStartSimulation();
    void onWriteDataNodeValue();
    void onReadLatestValue();
    void onLatestValueRead(bool success);

private:

    void setupReadDevicesTab();
    void setupCreateDeviceTab();
    void setupSimulationTab();

    Ui::MainWindow *ui;
    iot::DeviceList m_deviceList;
    iot::Device m_createdDevice;
    iot::DataNode m_readDataNode;
    iot::DataNode m_writeDataNode;
    QListWidget* m_deviceListWidget;
    QLabel* m_deviceDetailsWidget;
    QTimer m_simulationTimer;

};

#endif // MAINWINDOW_H
