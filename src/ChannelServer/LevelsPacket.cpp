/*
Copyright (C) 2008-2014 Vana Development Team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; version 2
of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#include "LevelsPacket.hpp"
#include "Maps.hpp"
#include "PacketCreator.hpp"
#include "Player.hpp"
#include "Session.hpp"
#include "SmsgHeader.hpp"

auto LevelsPacket::showExp(Player *player, int32_t exp, bool white, bool inChat) -> void {
	PacketCreator packet;
	packet.add<header_t>(SMSG_NOTICE);
	packet.add<int8_t>(3);
	packet.add<bool>(white);
	packet.add<int32_t>(exp);
	packet.add<bool>(inChat);
	packet.add<int32_t>(0);
	packet.add<int8_t>(0);
	packet.add<int8_t>(0);
	packet.add<int32_t>(0);
	if (inChat) {
		packet.add<int8_t>(0);
	}
	packet.add<int8_t>(0);
	packet.add<int32_t>(0);

	player->getSession()->send(packet);
}

auto LevelsPacket::levelUp(Player *player) -> void {
	PacketCreator packet;
	packet.add<header_t>(SMSG_SKILL_SHOW);
	packet.add<int32_t>(player->getId());
	packet.add<int8_t>(0);
	player->getMap()->sendPacket(packet, player);
}

auto LevelsPacket::statOk(Player *player) -> void {
	PacketCreator packet;
	packet.add<header_t>(SMSG_PLAYER_UPDATE);
	packet.add<int16_t>(1);
	packet.add<int32_t>(0);
	player->getSession()->send(packet);
}

auto LevelsPacket::jobChange(Player *player) -> void {
	PacketCreator packet;
	packet.add<header_t>(SMSG_SKILL_SHOW);
	packet.add<int32_t>(player->getId());
	packet.add<int8_t>(8);
	player->getMap()->sendPacket(packet, player);
}