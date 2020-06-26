#pragma once

#include <QByteArray>

class FritzBoxData {
	public:
		static const QByteArray CALLER;
		static const QByteArray CALLEE;

		static const QByteArray RING_DATA;
		static const QByteArray CONNECT_DATA;
		static const QByteArray DISCONNECT_DATA;
		static const QByteArray CALLING_DATA;
};
