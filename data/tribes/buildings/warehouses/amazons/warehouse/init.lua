dirname = path.dirname (__file__)

tribes:new_warehouse_type {
   msgctxt = "amazons_building",
   name = "amazons_warehouse",
   -- TRANSLATORS: This is a building name used in lists of buildings
   descname = pgettext ("amazons_building", "Warehouse"),
   helptext_script = dirname .. "helptexts.lua",
   icon = dirname .. "menu.png",
   size = "medium",

   buildcost = {
      balsa = 3,
      log = 5,
      rubber = 1,
      rope = 3
   },
   return_on_dismantle = {
      balsa = 2,
      log = 3,
      rope = 1
   },

   animation_directory = dirname,
   animations = {
      idle = {hotspot = {43, 62}},
      build = {hotspot = {43, 62}},
   },

   aihints = {},

   heal_per_second = 170,
}
