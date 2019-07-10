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
#include "enterpriselist.h"
#include "enterprise.h"

class EnterpriseListTests : public TestBase
{
    Q_OBJECT
public:
    EnterpriseListTests() : TestBase() {}

private:
    EnterpriseList* m_enterpriseList;
    QSignalSpy* m_readEnterprisesFinishedSpy;

private slots:

    void init() {
        m_enterpriseList = new EnterpriseList();
        m_readEnterprisesFinishedSpy = new QSignalSpy(m_enterpriseList, &iot::EnterpriseList::getEnterprisesFinished);
    }

    void cleanup() {
        delete m_enterpriseList; m_enterpriseList = nullptr;
        delete m_readEnterprisesFinishedSpy; m_readEnterprisesFinishedSpy = nullptr;
    }

    void verifyRequestPathOnReadRootEnterprises();
    void verifyRequestPathOnReadSubEnterprises();
    void readEnterprisesReturnZeroEntries();
    void readEnterprisesReturnSeveralEntries();
    void readEnterprisesFailesWithNetworkError();
    void readEnterprisesFailesWithServerErrorResponse();
    void readEnterprisesFailesWithHttpStatus_400();
    void readEnterprisesErrorClearesAfterSuccess();
};

void EnterpriseListTests::verifyRequestPathOnReadRootEnterprises()
{
    m_enterpriseList->readEnterprises();
    QString expectedPath = "/enterprises?limit=100";

    QCOMPARE(m_requestHandler->getRequestPath(), expectedPath);
}

void EnterpriseListTests::verifyRequestPathOnReadSubEnterprises()
{
    m_enterpriseList->readEnterprises("1234");
    QString expectedPath = "/enterprises/1234?limit=100";

    QCOMPARE(m_requestHandler->getRequestPath(), expectedPath);
}

void EnterpriseListTests::readEnterprisesReturnZeroEntries()
{
    m_enterpriseList->readEnterprises();

    QJsonObject returnedJson = QJsonObject();
    returnedJson["fullSize"] = 0;
    returnedJson["limit"] = 100;
    returnedJson["offset"] = 0;

    simulateGetResponse(200, QJsonDocument(returnedJson));

    QCOMPARE(m_readEnterprisesFinishedSpy->count(), 1);
    QCOMPARE(m_readEnterprisesFinishedSpy->takeFirst().at(0).toBool(), true);
    QVERIFY(m_enterpriseList->enterprises().isEmpty());

}

void EnterpriseListTests::readEnterprisesReturnSeveralEntries()
{
    m_enterpriseList->readEnterprises();

    QJsonObject enterprise1;
    enterprise1["href"] = "foo";
    enterprise1["name"] = "name1";
    enterprise1["resourceId"] = "E0001";
    enterprise1["hasSubEnterprises"] = true;

    QJsonObject enterprise2;
    enterprise2["href"] = "foo";
    enterprise2["name"] = "name2";
    enterprise2["resourceId"] = "E0002";
    enterprise2["hasSubEnterprises"] = false;

    QJsonArray items;
    items << enterprise1;
    items << enterprise2;

    QJsonObject returnedJson;
    returnedJson["fullSize"] = 2;
    returnedJson["limit"] = 100;
    returnedJson["offset"] = 0;
    returnedJson["items"] = items;

    simulateGetResponse(200, QJsonDocument(returnedJson));

    QCOMPARE(m_readEnterprisesFinishedSpy->count(), 1);
    QCOMPARE(m_readEnterprisesFinishedSpy->takeFirst().at(0).toBool(), true);
    QCOMPARE(m_enterpriseList->enterprises().size(), 2);

    QCOMPARE(m_enterpriseList->enterprises().at(0)->name(), "name1");
    QCOMPARE(m_enterpriseList->enterprises().at(0)->href(), "foo");
    QCOMPARE(m_enterpriseList->enterprises().at(0)->resourceId(), "E0001");
    QCOMPARE(m_enterpriseList->enterprises().at(0)->hasSubEnterprises(), true);

    QCOMPARE(m_enterpriseList->enterprises().at(1)->name(), "name2");
    QCOMPARE(m_enterpriseList->enterprises().at(1)->href(), "foo");
    QCOMPARE(m_enterpriseList->enterprises().at(1)->resourceId(), "E0002");
    QCOMPARE(m_enterpriseList->enterprises().at(1)->hasSubEnterprises(), false);

}

void EnterpriseListTests::readEnterprisesFailesWithNetworkError()
{
    m_enterpriseList->readEnterprises();

    simulateGetResponse(QNetworkReply::TimeoutError);

    QCOMPARE(m_readEnterprisesFinishedSpy->count(), 1);
    QCOMPARE(m_readEnterprisesFinishedSpy->takeFirst().at(0).toBool(), false);
    QVERIFY(m_enterpriseList->enterprises().isEmpty());
    QCOMPARE(m_enterpriseList->getError()->errorType(), Error::ErrorType::NetworkError);
    QCOMPARE(m_enterpriseList->getError()->networkError(), QNetworkReply::TimeoutError);
}

void EnterpriseListTests::readEnterprisesFailesWithServerErrorResponse()
{
    m_enterpriseList->readEnterprises();
    QJsonDocument doc( QJsonObject( { {"description", "desc"}, {"code", 8000}, {"moreInfo", "info"}, {"apiver", 1} } ) );

    simulateGetResponse(400, doc);

    QCOMPARE(m_readEnterprisesFinishedSpy->count(), 1);
    QCOMPARE(m_readEnterprisesFinishedSpy->takeFirst().at(0).toBool(), false);

    QVERIFY(m_enterpriseList->getError()->errorType() == Error::ErrorType::HttpError);
    QVERIFY(m_enterpriseList->getError()->httpStatusCode() == 400);
    QVERIFY(m_enterpriseList->getError()->description() == "desc");
    QVERIFY(m_enterpriseList->getError()->moreInfo() == "info");
    QVERIFY(m_enterpriseList->getError()->serverErrorCode() == 8000);
}

void EnterpriseListTests::readEnterprisesFailesWithHttpStatus_400()
{
    m_enterpriseList->readEnterprises();
    simulateGetResponse(400);
    QCOMPARE(m_readEnterprisesFinishedSpy->count(), 1);
    QCOMPARE(m_readEnterprisesFinishedSpy->takeFirst().at(0).toBool(), false);

    QCOMPARE(m_enterpriseList->enterprises().count(), 0);
    QCOMPARE(m_enterpriseList->getError()->errorType(), Error::ErrorType::HttpError);
    QCOMPARE(m_enterpriseList->getError()->httpStatusCode(), 400);
}

void EnterpriseListTests::readEnterprisesErrorClearesAfterSuccess()
{
    m_enterpriseList->readEnterprises();
    simulateGetResponse(400);
    QCOMPARE(m_readEnterprisesFinishedSpy->count(), 1);
    QCOMPARE(m_readEnterprisesFinishedSpy->takeFirst().at(0).toBool(), false);

    m_readEnterprisesFinishedSpy->clear();

    QJsonObject enterprise1;
    enterprise1["href"] = "foo";
    enterprise1["name"] = "name1";
    enterprise1["resourceId"] = "E0001";
    enterprise1["hasSubEnterprises"] = true;

    QJsonObject enterprise2;
    enterprise2["href"] = "foo";
    enterprise2["name"] = "name2";
    enterprise2["resourceId"] = "E0002";
    enterprise2["hasSubEnterprises"] = false;

    QJsonArray items;
    items << enterprise1;
    items << enterprise2;

    QJsonObject returnedJson;
    returnedJson["fullSize"] = 2;
    returnedJson["limit"] = 100;
    returnedJson["offset"] = 0;
    returnedJson["items"] = items;

    m_enterpriseList->readEnterprises();
    simulateGetResponse(200, QJsonDocument(returnedJson));

    QCOMPARE(m_readEnterprisesFinishedSpy->count(), 1);
    QCOMPARE(m_readEnterprisesFinishedSpy->takeFirst().at(0).toBool(), true);
    QCOMPARE(m_enterpriseList->enterprises().count(), 2);

    QCOMPARE( m_enterpriseList->getError()->errorType(), Error::ErrorType::NoError );
    QCOMPARE( m_enterpriseList->getError()->networkError(), QNetworkReply::NoError );

    QCOMPARE(m_enterpriseList->enterprises().at(0)->name(), "name1");
    QCOMPARE(m_enterpriseList->enterprises().at(0)->href(), "foo");
    QCOMPARE(m_enterpriseList->enterprises().at(0)->resourceId(), "E0001");
    QCOMPARE(m_enterpriseList->enterprises().at(0)->hasSubEnterprises(), true);

    QCOMPARE(m_enterpriseList->enterprises().at(1)->name(), "name2");
    QCOMPARE(m_enterpriseList->enterprises().at(1)->href(), "foo");
    QCOMPARE(m_enterpriseList->enterprises().at(1)->resourceId(), "E0002");
    QCOMPARE(m_enterpriseList->enterprises().at(1)->hasSubEnterprises(), false);

}


QTEST_APPLESS_MAIN(EnterpriseListTests)

#include "tst_enterpriselisttests.moc"

