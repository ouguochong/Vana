/*
Copyright (C) 2008-2009 Vana Development Team

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
#include "BuffsPacket.h"
#include "MapleSession.h"
#include "Maps.h"
#include "PacketCreator.h"
#include "Player.h"
#include "SendHeader.h"
#include "Buffs.h"

void BuffsPacket::useDash(Player *player, int32_t time, SkillActiveInfo pskill) {
	PacketCreator packet;
	int16_t castedtime = static_cast<int16_t>(time);
	packet.add<int16_t>(SEND_USE_SKILL);
	packet.add<int64_t>(0);
	for (int8_t i = 0; i < 8; i++)
		packet.add<int8_t>(pskill.types[i]);
	packet.add<int16_t>(0);
	for (size_t i = 0; i < pskill.vals.size(); i++) {
		packet.add<int16_t>(pskill.vals[i]);
		packet.add<int16_t>(0);
		packet.add<int32_t>(5001005);
		packet.add<int32_t>(880689251); // No idea, hate pirates
		packet.add<int16_t>(castedtime);
	}
	packet.add<int16_t>(0);
	packet.add<int8_t>(0); // Number of times you've been buffed total - only certain skills have this part
	player->getSession()->send(packet);
	if (player->getActiveBuffs()->getActiveSkillLevel(9101004) > 0)
		return;
	packet = PacketCreator();
	packet.add<int16_t>(SEND_SHOW_OTHERS_SKILL);
	packet.add<int32_t>(player->getId());
	packet.add<int64_t>(0);
	for (int8_t i = 0; i < 8; i++)
		packet.add<int8_t>(pskill.types[i]);
	packet.add<int16_t>(0);
	for (size_t i = 0; i < pskill.vals.size(); i++) {
		packet.add<int16_t>(pskill.vals[i]);
		packet.add<int16_t>(0);
		packet.add<int32_t>(5001005);
		packet.add<int32_t>(880689251); // No idea, hate pirates
		packet.add<int16_t>(castedtime);
	}
	packet.add<int16_t>(0);
	Maps::getMap(player->getMap())->sendPacket(packet, player);
}

void BuffsPacket::useMount(Player *player, int32_t skillid, int32_t time, SkillActiveInfo pskill, int16_t addedinfo, int32_t mountid) {
	time *= 1000;
	PacketCreator packet;
	packet.add<int16_t>(SEND_USE_SKILL);
	packet.add<int64_t>(0);
	for (int8_t i = 0; i < 8; i++)
		packet.add<int8_t>(pskill.types[i]);
	packet.add<int16_t>(0);
	packet.add<int32_t>(mountid);
	packet.add<int32_t>(skillid);
	packet.add<int32_t>(0); // Server tick value
	packet.add<int16_t>(0);
	packet.add<int8_t>(0); // Number of times you've been buffed total
	player->getSession()->send(packet);
	if (player->getActiveBuffs()->getActiveSkillLevel(9101004) > 0)
		return;
	packet = PacketCreator();
	packet.add<int16_t>(SEND_SHOW_OTHERS_SKILL);
	packet.add<int32_t>(player->getId());
	packet.add<int64_t>(0);
	for (int8_t i = 0; i < 8; i++)
		packet.add<int8_t>(pskill.types[i]);
	packet.add<int16_t>(0);
	packet.add<int32_t>(mountid);
	packet.add<int32_t>(skillid);
	packet.add<int32_t>(0);
	packet.add<int16_t>(0);
	Maps::getMap(player->getMap())->sendPacket(packet, player);
}

void BuffsPacket::useSkill(Player *player, int32_t skillid, int32_t time, SkillActiveInfo pskill, int16_t addedinfo, bool ismorph, bool isitem) {
	if (skillid != 1111002)
		time *= 1000;
	PacketCreator packet;
	packet.add<int16_t>(SEND_USE_SKILL);
	packet.add<int64_t>(0);
	for (int8_t i = 0; i < 8; i++)
		packet.add<int8_t>(pskill.types[i]);
	for (size_t i = 0; i < pskill.vals.size(); i++) {
		packet.add<int16_t>(pskill.vals[i]);
		packet.add<int32_t>(skillid);
		packet.add<int32_t>(time);
	}
	packet.add<int16_t>(0);
	packet.add<int16_t>(addedinfo);
	packet.add<int8_t>(0); // Number of times you've been buffed total - only certain skills have this part
	player->getSession()->send(packet);
	if (player->getActiveBuffs()->getActiveSkillLevel(9101004) > 0)
		return;
	if (!isitem || (isitem && ismorph)) {
		packet = PacketCreator();
		packet.add<int16_t>(SEND_SHOW_OTHERS_SKILL);
		packet.add<int32_t>(player->getId());
		packet.add<int64_t>(0);
		for (int8_t i = 0; i < 8; i++)
			packet.add<int8_t>(pskill.types[i]);
		for (size_t i = 0; i < pskill.vals.size(); i++)
			packet.add<int16_t>(pskill.vals[i]);
		packet.add<int16_t>(0);
		if (ismorph)
			packet.add<int16_t>(0);
		Maps::getMap(player->getMap())->sendPacket(packet, player);
	}
}

void BuffsPacket::useSpeedInfusion(Player *player, int32_t time, SkillActiveInfo pskill, int16_t addedinfo) {
	int32_t castedvalue = static_cast<int32_t>(pskill.vals[0]);
	int16_t castedtime = static_cast<int16_t>(time);
	PacketCreator packet;
	packet.add<int16_t>(SEND_USE_SKILL);
	packet.add<int64_t>(0);
	for (int8_t i = 0; i < 8; i++)
		packet.add<int8_t>(pskill.types[i]);
	packet.add<int16_t>(0);
	packet.add<int32_t>(castedvalue);
	packet.add<int32_t>(5121009);
	packet.add<int32_t>(0);
	packet.add<int32_t>(0);
	packet.add<int16_t>(castedtime);
	packet.add<int16_t>(addedinfo);
	player->getSession()->send(packet);
	if (player->getActiveBuffs()->getActiveSkillLevel(9101004) > 0)
		return;
	packet = PacketCreator();
	packet.add<int16_t>(SEND_SHOW_OTHERS_SKILL);
	packet.add<int32_t>(player->getId());
	packet.add<int64_t>(0);
	for (int8_t i = 0; i < 8; i++)
		packet.add<int8_t>(pskill.types[i]);
	packet.add<int16_t>(0);
	packet.add<int32_t>(castedvalue);
	packet.add<int32_t>(5121009);
	packet.add<int32_t>(0);
	packet.add<int32_t>(0);
	packet.add<int16_t>(castedtime);
	packet.add<int16_t>(addedinfo);
	Maps::getMap(player->getMap())->sendPacket(packet, player);
}

void BuffsPacket::endSkill(Player *player, SkillActiveInfo pskill) {
	PacketCreator packet;
	packet.add<int16_t>(SEND_CANCEL_SKILL);
	packet.add<int64_t>(0);
	for (int8_t i = 0; i < 8; i++)
		packet.add<int8_t>(pskill.types[i]);
	packet.add<int8_t>(0);
	player->getSession()->send(packet);
	if (player->getActiveBuffs()->getActiveSkillLevel(9101004) > 0)
		return;
	packet = PacketCreator();
	packet.add<int16_t>(SEND_CANCEL_OTHERS_BUFF);
	packet.add<int32_t>(player->getId());
	packet.add<int64_t>(0);
	for (int8_t i = 0; i < 8; i++)
		packet.add<int8_t>(pskill.types[i]);
	Maps::getMap(player->getMap())->sendPacket(packet, player);
}