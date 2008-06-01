/*
 * Copyright (C) 2002, 2006-2008 by the Widelands Development Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef FULLSCREEN_MENU_LAUNCHGAME_H
#define FULLSCREEN_MENU_LAUNCHGAME_H

#include "fullscreen_menu_base.h"

#include "ui_button.h"
#include "ui_textarea.h"

class ChatProvider;
struct GameChatPanel;
class GameController;
class GameSettingsProvider;
struct PlayerDescriptionGroup;

/**
 * Fullscreen menu to select map and to set map options.
 */
struct Fullscreen_Menu_LaunchGame : public Fullscreen_Menu_Base {
	Fullscreen_Menu_LaunchGame(GameSettingsProvider* settings, GameController* ctrl = 0);

	void setChatProvider(ChatProvider* chat);

	void start();
	void think();

	void refresh();

private:
	void select_map();
	void back_clicked();
	void start_clicked();


	UI::Button<Fullscreen_Menu_LaunchGame> m_select_map, m_back, m_ok;
	UI::Textarea             m_title, m_mapname;
	GameSettingsProvider   * m_settings;
	GameController         * m_ctrl; // optional
	GameChatPanel          * m_chat;
	PlayerDescriptionGroup * m_players[MAX_PLAYERS];
	bool                     m_is_scenario;
};


#endif
