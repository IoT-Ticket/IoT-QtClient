/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Wapice Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
 * to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "tst_testbase.h"

// add necessary includes here

class DataNodeReadStatisticsTests : public TestBase
{
    Q_OBJECT
public:
    DataNodeReadStatisticsTests() : TestBase(){}

private:

    DataNode* m_dataNode;
    QSignalSpy* m_readStatisticsFinishedSpy;

private slots:
    void init() {
        m_dataNode = new DataNode();
        m_dataNode->setDeviceId("1234567890");
        m_dataNode->setName("node1");
        m_readStatisticsFinishedSpy = new QSignalSpy(m_dataNode, &iot::DataNode::readStatisticsFinished);
    }

    void cleanup() {
        delete m_dataNode; m_dataNode = nullptr;
        delete m_readStatisticsFinishedSpy; m_readStatisticsFinishedSpy = nullptr;
    }

    void verifyRequestPathOnReadStatistics();
    void readStatisticsReturnValueContainsNoStatistics();
    void readStatisticsReturnSeveralEntries();
    void valuesClearedBetweenStatisticReads();
    void readStatisticsFailesWhenNotReady();
    void readyToReadAfterReadStatisticsComplete();
    void readStatisticsFailesWithNetworkError();
    void readStatisticsErrorClearedAfterSuccess();

};

void DataNodeReadStatisticsTests::verifyRequestPathOnReadStatistics()
{
    QDateTime startTime = QDateTime::fromMSecsSinceEpoch(1246293600000, Qt::UTC);
    QDateTime endTime = QDateTime::fromMSecsSinceEpoch(1546293604000, Qt::UTC);
    m_dataNode->readStatistics(startTime, endTime, DataNode::Grouping::Day);

    QString expectedPath = "/stat/read/" + m_dataNode->deviceId() + "?datanodes=/node1&grouping=Day&fromdate=1246293600000&todate=1546293604000";

    QCOMPARE(m_requestHandler->getRequestPath(), expectedPath);
}

void DataNodeReadStatisticsTests::readStatisticsReturnValueContainsNoStatistics()
{
    QDateTime startTime = QDateTime::fromMSecsSinceEpoch(1246293600000, Qt::UTC);
    QDateTime endTime = QDateTime::fromMSecsSinceEpoch(1546293604000, Qt::UTC);
    m_dataNode->readStatistics(startTime, endTime, iot::DataNode::Grouping::Day);

    QJsonObject value;
    value["count"] = 0;
    value["sum"] = 0;
    value["ts"] = startTime.toMSecsSinceEpoch();

    QJsonArray valuesArray;
    valuesArray << value;

    QJsonObject datanodeRead;
    datanodeRead["values"] = valuesArray;

    QJsonArray datanodeReadsArray;
    datanodeReadsArray << datanodeRead;

    QJsonObject obj;
    obj["datanodeReads"] = datanodeReadsArray;
    obj["href"] = "foo";

    simulateGetResponse(200, QJsonDocument(obj));
    QCOMPARE( m_readStatisticsFinishedSpy->count(), 1);
    QCOMPARE( m_readStatisticsFinishedSpy->takeFirst().at(0).toBool(), true );
    QCOMPARE( m_dataNode->statistics().count(), 1);

    QVERIFY(m_dataNode->statistics()[0].first.minimum().isNull());
    QVERIFY(m_dataNode->statistics()[0].first.maximum().isNull());
    QVERIFY(m_dataNode->statistics()[0].first.average().isNull());
    QCOMPARE(m_dataNode->statistics()[0].first.count(), 0);
    QCOMPARE(m_dataNode->statistics()[0].first.sum(), 0);
    QCOMPARE(m_dataNode->statistics()[0].second, startTime);
}

void DataNodeReadStatisticsTests::readStatisticsReturnSeveralEntries()
{
    QDateTime startTime = QDateTime::fromMSecsSinceEpoch(1246293600000, Qt::UTC);
    QDateTime endTime = QDateTime::fromMSecsSinceEpoch(1546293604000, Qt::UTC);
    m_dataNode->readStatistics(startTime, endTime, iot::DataNode::Grouping::Day);

    QJsonObject value1;
    value1["max"] = 100;
    value1["min"] = 1;
    value1["avg"] = 10;
    value1["count"] = 10;
    value1["sum"] = 1000;
    value1["ts"] = startTime.addDays(1).toMSecsSinceEpoch();

    QJsonObject value2;
    value2["count"] = 0;
    value2["sum"] = 0;
    value2["ts"] = startTime.addDays(2).toMSecsSinceEpoch();

    QJsonArray valuesArray;
    valuesArray << value1;
    valuesArray << value2;

    QJsonObject datanodeRead;
    datanodeRead["values"] = valuesArray;

    QJsonArray datanodeReadsArray;
    datanodeReadsArray << datanodeRead;

    QJsonObject obj;
    obj["datanodeReads"] = datanodeReadsArray;
    obj["href"] = "foo";

    simulateGetResponse(200, QJsonDocument(obj));
    QCOMPARE( m_readStatisticsFinishedSpy->count(), 1);
    QCOMPARE( m_readStatisticsFinishedSpy->takeFirst().at(0).toBool(), true );
    QCOMPARE( m_dataNode->statistics().count(), 2);

    QCOMPARE(m_dataNode->statistics()[0].first.maximum(), 100);
    QCOMPARE(m_dataNode->statistics()[0].first.minimum(), 1);
    QCOMPARE(m_dataNode->statistics()[0].first.average(), 10);
    QCOMPARE(m_dataNode->statistics()[0].first.count(), 10);
    QCOMPARE(m_dataNode->statistics()[0].first.sum(), 1000);
    QCOMPARE(m_dataNode->statistics()[0].second, startTime.addDays(1));

    QVERIFY(m_dataNode->statistics()[1].first.maximum().isNull());
    QVERIFY(m_dataNode->statistics()[1].first.minimum().isNull());
    QVERIFY(m_dataNode->statistics()[1].first.average().isNull());
    QCOMPARE(m_dataNode->statistics()[1].first.count(), 0);
    QCOMPARE(m_dataNode->statistics()[1].first.sum(), 0);
    QCOMPARE(m_dataNode->statistics()[1].second, startTime.addDays(2));
}

void DataNodeReadStatisticsTests::valuesClearedBetweenStatisticReads()
{
    readStatisticsReturnSeveralEntries();

    // Read second set of statistics. Statistics function should return only
    // the second set of values
    QDateTime startTime = QDateTime::fromMSecsSinceEpoch(1256293600000, Qt::UTC);

    m_dataNode->readStatistics(startTime, startTime.addDays(1), iot::DataNode::Grouping::Day);

    QJsonObject value;
    value["max"] = 300;
    value["min"] = 3;
    value["avg"] = 30;
    value["count"] = 30;
    value["sum"] = 3000;
    value["ts"] = startTime.toMSecsSinceEpoch();

    QJsonArray valuesArray;
    valuesArray << value;

    QJsonObject datanodeRead;
    datanodeRead["values"] = valuesArray;

    QJsonArray datanodeReadsArray;
    datanodeReadsArray << datanodeRead;

    QJsonObject obj;
    obj["datanodeReads"] = datanodeReadsArray;
    obj["href"] = "foo";

    // Simulate another response
    m_readStatisticsFinishedSpy->clear();
    simulateGetResponse(200, QJsonDocument(obj));

    QCOMPARE( m_readStatisticsFinishedSpy->count(), 1 );
    QCOMPARE( m_readStatisticsFinishedSpy->takeFirst().at(0).toBool(), true );
    QCOMPARE( m_dataNode->statistics().count(), 1 );

    QCOMPARE( m_dataNode->statistics()[0].first.maximum(), 300 );
    QCOMPARE( m_dataNode->statistics()[0].first.minimum(), 3 );
    QCOMPARE( m_dataNode->statistics()[0].first.average(), 30 );
    QCOMPARE( m_dataNode->statistics()[0].first.count(), 30 );
    QCOMPARE( m_dataNode->statistics()[0].first.sum(), 3000 );
    QCOMPARE( m_dataNode->statistics()[0].second, startTime );

}

void DataNodeReadStatisticsTests::readStatisticsFailesWhenNotReady()
{
    QDateTime startTime = QDateTime::fromMSecsSinceEpoch(1246293600000, Qt::UTC);
    QDateTime endTime = QDateTime::fromMSecsSinceEpoch(1546293604000, Qt::UTC);

    QCOMPARE( m_dataNode->isReadyToReadStatistics(), true );
    QCOMPARE( m_dataNode->readStatistics(startTime, endTime, DataNode::Grouping::Day), true);
    QCOMPARE( m_dataNode->isReadyToReadStatistics(), false );
    QCOMPARE( m_dataNode->readStatistics(startTime, endTime, DataNode::Grouping::Day), false);
}

void DataNodeReadStatisticsTests::readyToReadAfterReadStatisticsComplete()
{
    QDateTime startTime = QDateTime::fromMSecsSinceEpoch(1246293600000, Qt::UTC);
    QDateTime endTime = QDateTime::fromMSecsSinceEpoch(1546293604000, Qt::UTC);

    QCOMPARE( m_dataNode->readStatistics(startTime, endTime, DataNode::Grouping::Day), true);

    QJsonObject value;
    value["max"] = 300;
    value["min"] = 3;
    value["avg"] = 30;
    value["count"] = 30;
    value["sum"] = 3000;
    value["ts"] = startTime.toMSecsSinceEpoch();

    QJsonArray valuesArray;
    valuesArray << value;

    QJsonObject datanodeRead;
    datanodeRead["values"] = valuesArray;

    QJsonArray datanodeReadsArray;
    datanodeReadsArray << datanodeRead;

    QJsonObject obj;
    obj["datanodeReads"] = datanodeReadsArray;
    obj["href"] = "foo";
    simulateGetResponse(200, QJsonDocument(obj));

    QCOMPARE( m_dataNode->isReadyToReadStatistics(), true );
    QCOMPARE( m_dataNode->readStatistics(startTime, endTime, DataNode::Grouping::Day), true );
    QCOMPARE( m_dataNode->isReadyToReadStatistics(), false );

}

void DataNodeReadStatisticsTests::readStatisticsFailesWithNetworkError()
{
    QDateTime startTime = QDateTime::fromMSecsSinceEpoch(1246293600000, Qt::UTC);
    QDateTime endTime = QDateTime::fromMSecsSinceEpoch(1546293604000, Qt::UTC);

    QCOMPARE( m_dataNode->readStatistics(startTime, endTime, DataNode::Grouping::Day), true );

    simulateGetResponse(QNetworkReply::TimeoutError);

    QCOMPARE( m_readStatisticsFinishedSpy->count(), 1 );
    QCOMPARE( m_readStatisticsFinishedSpy->takeFirst().at(0).toBool(), false );
    QCOMPARE( m_dataNode->readStatisticsError()->errorType(), Error::ErrorType::NetworkError );
    QCOMPARE( m_dataNode->readStatisticsError()->networkError(), QNetworkReply::TimeoutError );
}

void DataNodeReadStatisticsTests::readStatisticsErrorClearedAfterSuccess()
{
    QDateTime startTime = QDateTime::fromMSecsSinceEpoch(1246293600000, Qt::UTC);
    QDateTime endTime = QDateTime::fromMSecsSinceEpoch(1546293604000, Qt::UTC);

    QCOMPARE( m_dataNode->readStatistics(startTime, endTime, DataNode::Grouping::Day), true );

    simulateGetResponse(QNetworkReply::TimeoutError);

    QCOMPARE(m_readStatisticsFinishedSpy->count(), 1);
    QCOMPARE(m_readStatisticsFinishedSpy->takeFirst().at(0).toBool(), false);

    m_dataNode->readStatistics(startTime, endTime, DataNode::Grouping::Day);

    QJsonObject value;
    value["max"] = 300;
    value["min"] = 3;
    value["avg"] = 30;
    value["count"] = 30;
    value["sum"] = 3000;
    value["ts"] = startTime.toMSecsSinceEpoch();

    QJsonArray valuesArray;
    valuesArray << value;

    QJsonObject datanodeRead;
    datanodeRead["values"] = valuesArray;

    QJsonArray datanodeReadsArray;
    datanodeReadsArray << datanodeRead;

    QJsonObject obj;
    obj["datanodeReads"] = datanodeReadsArray;
    obj["href"] = "foo";
    simulateGetResponse(200, QJsonDocument(obj));

    QCOMPARE( m_readStatisticsFinishedSpy->count(), 1 );
    QCOMPARE( m_readStatisticsFinishedSpy->takeFirst().at(0).toBool(), true );
    QCOMPARE( m_dataNode->readError()->errorType(), Error::ErrorType::NoError);
    QCOMPARE( m_dataNode->readError()->networkError(), QNetworkReply::NoError);

}

QTEST_APPLESS_MAIN(DataNodeReadStatisticsTests)

// To tricker mock tool because our header is in cpp file.
#include "tst_datanodereadstatisticstests.moc"
