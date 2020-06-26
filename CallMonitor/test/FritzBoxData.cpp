#include "FritzBoxData.hpp"

using namespace std;

const QByteArray FritzBoxData::CALLER = "017624025482";
const QByteArray FritzBoxData::CALLEE = "90969248";

const QByteArray FritzBoxData::RING_DATA       = "29.10.16 18:23:02;RING;0;" + CALLER + ";" + CALLEE + ";SIP1;\n";
const QByteArray FritzBoxData::CONNECT_DATA    = "03.11.16 13:17:16;CONNECT;0;1;" + CALLER + ";\n";
const QByteArray FritzBoxData::DISCONNECT_DATA = "29.10.16 18:23:04;DISCONNECT;0;201;\n";
const QByteArray FritzBoxData::CALLING_DATA    = "21.11.16 22:51:49;CALL;0;1;" + CALLER + ";" + CALLEE + ";SIP1;\n";

