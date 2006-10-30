/*
 * Copyright (C) 2002-2004, 2006 by the Widelands Development Team
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

#include "editorinteractive.h"
#include "editor_game_base.h"
#include "error.h"
#include "filesystem.h"
#include "game.h"
#include "map.h"
#include "player.h"
#include "tribe.h"
#include "widelands_map_allowed_buildings_data_packet.h"
#include "widelands_map_data_packet_ids.h"
#include "world.h"
#include "profile.h"

#define CURRENT_PACKET_VERSION 1

/*
 * Destructor
 */
Widelands_Map_Allowed_Buildings_Data_Packet::~Widelands_Map_Allowed_Buildings_Data_Packet(void) {
}

/*
 * Read Function
 */
void Widelands_Map_Allowed_Buildings_Data_Packet::Read
(FileSystem & fs,
 Editor_Game_Base* egbase,
 const bool skip,
 Widelands_Map_Map_Object_Loader * const)
throw (_wexception)
{
   if( skip )
      return;

   Profile prof;
   try {
      prof.read("allowed_buildings", 0, fs );
   } catch( ... ) {
      // Packet wasn't save. Same as skip
      return;
   }
   Section* s = prof.get_section("global");

   // read packet version
   int packet_version = s->get_int("packet_version");

   char buf[256];
   if(packet_version==CURRENT_PACKET_VERSION) {
      // First of all: if we shouldn't skip, all buildings default to false in the game (!not editor)
      if (dynamic_cast<Game * const>(egbase)) {
         int i=0;
         for(i=1; i<=egbase->get_map()->get_nrplayers(); i++) {
            Player* plr=egbase->get_player(i);
            if(!plr) continue;
            const Tribe_Descr* t=plr->get_tribe();

            int b;
            for(b=0; b<t->get_nrbuildings(); b++) {
               plr->allow_building(b, false);
            }
         }
      }

      // Now read all players and buildings
      int i=0;
      for(i=1; i<=egbase->get_map()->get_nrplayers(); i++) {
         Player* plr=egbase->get_safe_player(i);
         if(!plr) continue; // skip this player, is data can not be saved
         const Tribe_Descr* t;

         assert(plr);
         t=plr->get_tribe();

         sprintf( buf, "player_%i", i );
         s = prof.get_safe_section( buf );

         // Write for all buildings if it is enabled
         const char* name;
         while( (name=s->get_next_bool(0,0))) {
            bool allowed = s->get_bool(name);
            int index=t->get_building_index(name);
            if(index==-1)
               throw wexception("Unknown building found in map (Allowed_Buildings_Data): %s is not in tribe %s", name, t->get_name().c_str());
            plr->allow_building(index, allowed);
         }
      }

      // DONE
      return;
   } else {
      throw wexception("Unknown version %i Allowed_Building_Data_Packet in map!\n", packet_version);
   }
   assert(0); // never here
}


/*
 * Write Function
 */
void Widelands_Map_Allowed_Buildings_Data_Packet::Write
(FileSystem & fs,
 Editor_Game_Base* egbase,
 Widelands_Map_Map_Object_Saver * const)
throw (_wexception)
{
   Profile prof;
   Section* s = prof.create_section("global");

   s->set_int("packet_version", CURRENT_PACKET_VERSION );

   char buf[256];
   int i=0;
   for(i=1; i<=egbase->get_map()->get_nrplayers(); i++) {
      Player* plr=egbase->get_player(i);
      if(!plr) continue; // skip this player, is data can not be saved
      const Tribe_Descr* t;
      if( plr )
         t = plr->get_tribe();
      else
         t = egbase->get_tribe(egbase->get_map()->get_scenario_player_tribe(i).c_str());

      sprintf( buf, "player_%i", i );
      s = prof.create_section( buf );

      // Write for all buildings if it is enabled
      int b;
      for(b=0; b<t->get_nrbuildings(); b++) {
         Building_Descr* building=t->get_building_descr(b);
         std::string name=building->get_name();
         bool val;
         if(plr)
            val = plr->is_building_allowed(b);
         else
            val = true; // All known buildings are allowed
         s->set_bool(name.c_str(), val);
      }
   }

   prof.write("allowed_buildings", false, fs );
   // Done
}
