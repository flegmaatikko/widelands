dirname = path.dirname (__file__)

tribes:new_militarysite_type {
   msgctxt = "amazons_building",
   name = "amazons_warriors_dwelling",
   -- TRANSLATORS: This is a building name used in lists of buildings
   descname = pgettext ("amazons_building", "Warriors Dwelling"),
   helptext_script = dirname .. "helptexts.lua",
   icon = dirname .. "menu.png",
   size = "medium",
   vision_range = 11,

   buildcost = {
      ironwood = 2,
      granite = 3,
   },
   return_on_dismantle = {
      ironwood = 1,
      granite = 2,

   },
   animations = {
      idle = {
         pictures = path.list_files (dirname .. "idle_??.png"),
         hotspot = {90, 127},
         fps = 10,
      },
      unoccupied = {
         pictures = path.list_files (dirname .. "unoccupied_?.png"),
         hotspot = {90, 127},
      },
   },

   aihints = {
      fighting = true,
      mountain_conqueror = true,
      prohibited_till = 500
   },

   max_soldiers = 5,
   heal_per_second = 150,
   conquers = 8
   prefer_heroes = true,

   messages = {
      occupied = _"Your soldiers have occupied your warriors delling.",
      aggressor = _"Your warriors dwelling discovered an aggressor.",
      attack = _"Your warriors dwelling is under attack.",
      defeated_enemy = _"The enemy defeated your soldiers at the warriors dwelling.",
      defeated_you = _"Your soldiers defeated the enemy at the warriors dwelling."
   },
}
