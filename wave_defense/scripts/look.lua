local player, mouse = nilgobj(), nilgobj()

function ready()
   player = s.scene:get_gameobject("player")
   mouse = s.scene:get_gameobject("mouse")
end

function update()
   local mouse_pos = get_mouse_position()
   local ndc = screen_to_ndc(mouse_pos)

   mouse:set_position(ndc)
   player:rotate_towards(ndc)
end
