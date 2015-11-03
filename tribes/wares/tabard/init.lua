dirname = path.dirname(__file__)

tribes:new_ware_type {
   msgctxt = "ware",
   name = "tabard",
   -- TRANSLATORS: This is a ware name used in lists of wares
   descname = pgettext("ware", "Tabard"),
   helptext_script = dirname .. "helptexts.lua",
   icon = dirname .. "menu.png",
   default_target_quantity = {
		atlanteans = 30
	},
   preciousness = {
		atlanteans = 1
	},

   animations = {
      idle = {
         template = "idle",
         directory = dirname,
         hotspot = { 5, 10 },
      },
   }
}
