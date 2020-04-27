dirname = path.dirname (__file__)

tribes:new_militarysite_type {
   msgctxt = "amazons_building",
   name = "amazons_tower",
   -- TRANSLATORS: This is a building name used in lists of buildings
   descname = pgettext ("amazons_building", "Tower"),
   helptext_script = dirname .. "helptexts.lua",
   icon = dirname .. "menu.png",
   size = "medium",
   vision_range = 16,
   enhancement = "amazons_observation_tower",

   buildcost = {
      rope = 2,
      granite = 2,
      log = 2,
      ironwood = 2
   },
   return_on_dismantle = {
      rope = 1,
      granite = 1,
      log = 1,
      ironwood = 1
   },

   animation_directory = dirname,
   animations = {
      idle = {hotspot = {44, 67}},
      unoccupied = {hotspot = {44, 67}},
   },

   aihints = {
      fighting = true,
      mountain_conqueror = true,
      prohibited_till = 500
   },

   max_soldiers = 4,
   heal_per_second = 110,
   conquers = 8,
   prefer_heroes = true,

   messages = {
      occupied = _"Your soldiers have occupied your tower.",
      aggressor = _"Your tower discovered an aggressor.",
      attack = _"Your tower is under attack.",
      defeated_enemy = _"The enemy defeated your soldiers at the tower.",
      defeated_you = _"Your soldiers defeated the enemy at the tower."
   },
}
