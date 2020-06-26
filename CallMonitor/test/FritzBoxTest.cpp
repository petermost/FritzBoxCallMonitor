#include "FritzBoxData.hpp"
#include <CallMonitor/src/FritzBox.hpp>
#include <QSignalSpy>
#include <gtest/gtest.h>

using namespace std;


TEST(FritzBoxTest, testRingingSignal)
{
	FritzBox fritzBox;
	QSignalSpy spy(&fritzBox, &FritzBox::incomingCall);

	fritzBox.parseAndSignal(FritzBoxData::RING_DATA);
	QList<QVariant> arguments = spy.first();

	ASSERT_EQ(spy.count(), 1);
	ASSERT_EQ(arguments.count(), 3);
	ASSERT_EQ(arguments[0].toUInt(), 0u);
	ASSERT_EQ(arguments[1].toString(), QString(FritzBoxData::CALLER));
	ASSERT_EQ(arguments[2].toString(), QString(FritzBoxData::CALLEE));
}

TEST(FritzBoxTest, testConnectedSignal)
{
	FritzBox fritzBox;
	QSignalSpy spy(&fritzBox, &FritzBox::phoneConnected);

	fritzBox.parseAndSignal(FritzBoxData::CONNECT_DATA);
	QList<QVariant> arguments = spy.first();

	ASSERT_EQ(spy.count(), 1);
	ASSERT_EQ(arguments.count(), 2);
	ASSERT_EQ(arguments[0].toUInt(), 0u);
	ASSERT_EQ(arguments[1].toString(), QString(FritzBoxData::CALLER));
}

TEST(FritzBoxTest, testDisconnectedSignal)
{
	FritzBox fritzBox;
	QSignalSpy spy(&fritzBox, &FritzBox::phoneDisconnected);

	fritzBox.parseAndSignal(FritzBoxData::DISCONNECT_DATA);
	QList<QVariant> arguments = spy.first();

	ASSERT_EQ(spy.count(), 1);
	ASSERT_EQ(arguments.count(), 1);
	ASSERT_EQ(arguments[0].toUInt(), 0u);
}

TEST(FritzBoxTest, testCallingSignal)
{
	FritzBox fritzBox;
	QSignalSpy spy(&fritzBox, &FritzBox::outgoingCall);

	fritzBox.parseAndSignal(FritzBoxData::CALLING_DATA);
	QList<QVariant> arguments = spy.first();

	ASSERT_EQ(spy.count(), 1);
	ASSERT_EQ(arguments.count(), 3);
	ASSERT_EQ(arguments[0].toUInt(), 0u);
	ASSERT_EQ(arguments[1].toString(), QString(FritzBoxData::CALLER));
	ASSERT_EQ(arguments[2].toString(), QString(FritzBoxData::CALLEE));
}
