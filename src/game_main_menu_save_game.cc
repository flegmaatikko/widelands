/*
 * Copyright (C) 2002-4 by the Widelands Development Team
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

#include "constants.h"
#include "game.h"
#include "game_loader.h"
#include "game_main_menu_save_game.h"
#include "game_preload_data_packet.h"
#include "game_saver.h"
#include "i18n.h"
#include "interactive_player.h"
#include "layeredfilesystem.h"
#include "profile.h"
#include "ui_button.h"
#include "ui_editbox.h"
#include "ui_listselect.h"
#include "ui_modal_messagebox.h"
#include "ui_textarea.h"

/*
===============
Game_Main_Menu_Save_Game::Game_Main_Menu_Save_Game

Create all the buttons etc...
===============
*/
Game_Main_Menu_Save_Game::Game_Main_Menu_Save_Game(Interactive_Player* parent, UIUniqueWindowRegistry* registry)
  : UIUniqueWindow(parent,registry,400,270,_("Save Game")) {
   m_parent=parent;

   // Caption
   UITextarea* tt=new UITextarea(this, 0, 0, _("Save Game"), Align_Left);
   tt->set_pos((get_inner_w()-tt->get_w())/2, 5);

   int spacing=5;
   int offsx=spacing;
   int offsy=30;
   int posx=offsx;
   int posy=offsy;

   // listselect
   m_ls=new UIListselect(this, posx, posy, get_inner_w()/2-spacing, get_inner_h()-spacing-offsy-60);
   m_ls->selected.set(this, &Game_Main_Menu_Save_Game::selected);
   m_ls->double_clicked.set(this, &Game_Main_Menu_Save_Game::double_clicked);
   // Filename editbox
   m_editbox=new UIEdit_Box(this, posx, posy+get_inner_h()-spacing-offsy-60+3, get_inner_w()/2-spacing, 20, 1, 0);
   m_editbox->changed.set(this, &Game_Main_Menu_Save_Game::edit_box_changed);

   // the descriptive areas
   // Name
   posx=get_inner_w()/2+spacing;
   posy+=20;
   new UITextarea(this, posx, posy, 150, 20, _("Map Name: "), Align_CenterLeft);
   m_name=new UITextarea(this, posx+90, posy, 200, 20, "---", Align_CenterLeft);
   posy+=20+spacing;

   // Author
   new UITextarea(this, posx, posy, 150, 20, _("Game Time: "), Align_CenterLeft);
   m_gametime=new UITextarea(this, posx+90, posy, 200, 20, "---", Align_CenterLeft);
   posy+=20+spacing;

   // Buttons
   posx=5;
   posy=get_inner_h()-30;
   UIButton* but= new UIButton(this, get_inner_w()/2-spacing-80, posy, 80, 20, 4, 1);
   but->clickedid.set(this, &Game_Main_Menu_Save_Game::clicked);
   but->set_title(_("OK").c_str());
   but->set_enabled(false);
   m_ok_btn=but;
   but= new UIButton(this, get_inner_w()/2+spacing, posy, 80, 20, 4, 0);
   but->clickedid.set(this, &Game_Main_Menu_Save_Game::clicked);
   but->set_title(_("Cancel").c_str());

   m_basedir="ssave";
   m_curdir="ssave";

   fill_list();

   center_to_parent();
   move_to_top();
}

/*
===============
Game_Main_Menu_Save_Game::~Game_Main_Menu_Save_Game

Unregister from the registry pointer
===============
*/
Game_Main_Menu_Save_Game::~Game_Main_Menu_Save_Game()
{
}

/*
===========
called when the ok button has been clicked
===========
*/
void Game_Main_Menu_Save_Game::clicked(int id) {
   if(id==1) {
      // Ok
      std::string filename=m_editbox->get_text();

      if(save_game(filename, ! g_options.pull_section("global")->get_bool("nozip", false)))
         die();
   } else if(id==0) {
      // Cancel
      die();
   }
}

/*
 * called when a item is selected
 */
void Game_Main_Menu_Save_Game::selected(int i) {
   const char* name=static_cast<const char*>(m_ls->get_selection());


   FileSystem* fs = g_fs->MakeSubFileSystem( name );
   Game_Loader gl(fs, m_parent->get_game());
   Game_Preload_Data_Packet gpdp;
   gl.preload_game(&gpdp); // This has worked before, no problem

   char* fname = strdup(FS_Filename(name));
   FS_StripExtension(fname);
   m_editbox->set_text(fname);
   free(fname);
   m_ok_btn->set_enabled(true);

   m_name->set_text(gpdp.get_mapname());

   char buf[200];
   uint gametime = gpdp.get_gametime();

   int hours = gametime / 3600000;
   gametime -= hours * 3600000;
   int minutes = gametime / 60000;

   sprintf(buf, "%02i:%02i", hours, minutes);
   m_gametime->set_text(buf);

   delete fs;
}

/*
 * An Item has been doubleclicked
 */
void Game_Main_Menu_Save_Game::double_clicked(int) {
   clicked(1);
}

/*
 * fill the file list
 */
void Game_Main_Menu_Save_Game::fill_list(void) {
   // Fill it with all files we find.
   g_fs->FindFiles(m_curdir, "*", &m_gamefiles, 1);

   Game_Preload_Data_Packet gpdp;

   for(filenameset_t::iterator pname = m_gamefiles.begin(); pname != m_gamefiles.end(); pname++) {
      const char *name = pname->c_str();

      FileSystem* fs = 0;
      Game_Loader* gl = 0;
      try {
         fs = g_fs->MakeSubFileSystem( name );
         gl = new Game_Loader( fs,m_parent->get_game());
         gl->preload_game(&gpdp);
         char* fname = strdup(FS_Filename(name));
         FS_StripExtension(fname);
         m_ls->add_entry(fname, reinterpret_cast<void*>(const_cast<char*>(name)));
         free(fname);
      } catch(wexception& ) {
         // we simply skip illegal entries
      }
      if( gl )
         delete gl;
      if( fs )
         delete fs;
   }

   if(m_ls->get_nr_entries())
      m_ls->select(0);
}

/*
 * The editbox was changed. Enable ok button
 */
void Game_Main_Menu_Save_Game::edit_box_changed(void) {
   m_ok_btn->set_enabled(true);
}

/*
 * Save the game
 *
 * returns true if dialog should close, false if it
 * should stay open
 */
bool Game_Main_Menu_Save_Game::save_game(std::string filename, bool binary) {
   // Make sure that the base directory exists
   g_fs->EnsureDirectoryExists(m_basedir);

   // ok, first check if the extension matches (ignoring case)
   bool assign_extension=true;
   if(filename.size() >= strlen(WLGF_SUFFIX)) {
      char buffer[10]; // enough for the extension
      filename.copy(buffer, sizeof(WLGF_SUFFIX), filename.size()-strlen(WLGF_SUFFIX));
      if(!strncasecmp(buffer, WLGF_SUFFIX, strlen(WLGF_SUFFIX)))
         assign_extension=false;
   }
   if(assign_extension)
      filename+=WLGF_SUFFIX;

   // Now append directory name
   std::string complete_filename=m_curdir;
   complete_filename+="/";
   complete_filename+=filename;

   // Check if file exists, if it does, show a warning
   if(g_fs->FileExists(complete_filename)) {
      std::string s=_("A File with the name ");
      s+=FS_Filename(filename.c_str());
      s+=_(" already exists. Overwrite?");
      UIModal_Message_Box* mbox= new UIModal_Message_Box(m_parent, _("Save Game Error!!"), s, UIModal_Message_Box::YESNO);
      bool retval=mbox->run();
      delete mbox;
      if(!retval)
         return false;

      // Delete this
      g_fs->Unlink( complete_filename );
   }

   // Make a filesystem out of this
   FileSystem* fs = 0;
   if( !binary ) {
      fs = g_fs->CreateSubFileSystem( complete_filename, FileSystem::FS_DIR );
   } else {
      fs = g_fs->CreateSubFileSystem( complete_filename, FileSystem::FS_ZIP );
   }

   Game_Saver* gs=new Game_Saver(fs, m_parent->get_game());
   try {
      gs->save();
   } catch(std::exception& exe) {
      std::string s=_("Game Saving Error!\nSaved Game-File may be corrupt!\n\nReason given:\n");
      s+=exe.what();
      UIModal_Message_Box* mbox= new UIModal_Message_Box(m_parent, _("Save Game Error!!"), s, UIModal_Message_Box::OK);
      mbox->run();
      delete mbox;
   }
   delete gs;
   delete fs;
   die();

   return true;
}


