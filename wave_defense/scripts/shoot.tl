local count = 0

global function update()
	if not is_key_pressed(KEY_E) then
		return
	end

	print("Creating new bullet")
	local new_mesh = create_quad(0.01, 0.01)
	warn("made mesh")

	count = count + 1
	local new_gobj = GameObject:new(new_mesh, tostring("bullet"..tostring(count)))
	warn("new gobj made with name: "..new_gobj.name)
	new_gobj:set_position(vec2:new(0, 0))
	new_gobj:set_rotation(0)
	new_gobj:set_scale(vec2:new(1, 1))
	new_gobj:set_color(vec3:new(1, 1, 1))

	s.scene:add_gameobject(new_gobj)
	s.scene:build()

	warn("new gobj added to scene")
end
