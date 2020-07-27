#include "FritzBoxData.hpp"
#include <pera_software/aidkit/qt/core/Strings.hpp>

using namespace std;
using namespace pera_software::aidkit::qt;

const QString FritzBoxData::CALLER = "017624025482"_qs;
const QString FritzBoxData::CALLEE = "90969248"_qs;

const QString FritzBoxData::RING_DATA       = "29.10.16 18:23:02;RING;0;"_qs + CALLER + ";"_qs + CALLEE + ";SIP1;\n"_qs;
const QString FritzBoxData::CONNECT_DATA    = "03.11.16 13:17:16;CONNECT;0;1;"_qs + CALLER + ";\n"_qs;
const QString FritzBoxData::DISCONNECT_DATA = "29.10.16 18:23:04;DISCONNECT;0;201;\n"_qs;
const QString FritzBoxData::CALLING_DATA    = "21.11.16 22:51:49;CALL;0;1;"_qs + CALLER + ";"_qs + CALLEE + ";SIP1;\n"_qs;

