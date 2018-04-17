#include "FritzBoxTest.hpp"
#include <CallMonitor/src/FritzBox.hpp>
#include <QTest>
#include <QSignalSpy>

using namespace std;

static FritzBoxTest fritzBoxTest;

const QByteArray CALLER = "017624025482";
const QByteArray CALLEE = "90969248";

const QByteArray FritzBoxTest::RING_DATA       = "29.10.16 18:23:02;RING;0;" + CALLER + ";" + CALLEE + ";SIP1;\n";
const QByteArray FritzBoxTest::CONNECT_DATA    = "03.11.16 13:17:16;CONNECT;0;1;" + CALLER + ";\n";
const QByteArray FritzBoxTest::DISCONNECT_DATA = "29.10.16 18:23:04;DISCONNECT;0;201;\n";
const QByteArray FritzBoxTest::CALLING_DATA    = "21.11.16 22:51:49;CALL;0;1;" + CALLER + ";" + CALLEE + ";SIP1;\n";

void FritzBoxTest::testRingingSignal() {
	FritzBox fritzBox;
	QSignalSpy spy( &fritzBox, &FritzBox::incomingCall );

	fritzBox.parseAndSignal( RING_DATA );
	QList< QVariant > arguments = spy.first();

	QCOMPARE( spy.count(), 1 );
	QCOMPARE( arguments.count(), 3 );
	QCOMPARE( arguments[ 0 ].toUInt(), 0u );
	QCOMPARE( arguments[ 1 ].toString(), QString( CALLER ));
	QCOMPARE( arguments[ 2 ].toString(), QString( CALLEE ));
}

void FritzBoxTest::testConnectedSignal() {
	FritzBox fritzBox;
	QSignalSpy spy( &fritzBox, &FritzBox::phoneConnected );

	fritzBox.parseAndSignal( CONNECT_DATA );
	QList< QVariant > arguments = spy.first();

	QCOMPARE( spy.count(), 1 );
	QCOMPARE( arguments.count(), 2 );
	QCOMPARE( arguments[ 0 ].toUInt(), 0u );
	QCOMPARE( arguments[ 1 ].toString(), QString( CALLER ));
}

void FritzBoxTest::testDisconnectedSignal() {
	FritzBox fritzBox;
	QSignalSpy spy( &fritzBox, &FritzBox::phoneDisconnected );

	fritzBox.parseAndSignal( DISCONNECT_DATA );
	QList< QVariant > arguments = spy.first();

	QCOMPARE( spy.count(), 1 );
	QCOMPARE( arguments.count(), 1 );
	QCOMPARE( arguments[ 0 ].toUInt(), 0u );
}

void FritzBoxTest::testCallingSignal() {
	FritzBox fritzBox;
	QSignalSpy spy( &fritzBox, &FritzBox::outgoingCall );

	fritzBox.parseAndSignal( CALLING_DATA );
	QList< QVariant > arguments = spy.first();

	QCOMPARE( spy.count(), 1 );
	QCOMPARE( arguments.count(), 3 );
	QCOMPARE( arguments[ 0 ].toUInt(), 0u );
	QCOMPARE( arguments[ 1 ].toString(), QString( CALLER ));
	QCOMPARE( arguments[ 2 ].toString(), QString( CALLEE ));
}
