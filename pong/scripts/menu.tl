global function ready()
	-- check we're on the right scene
	if not s.scene:get_gameobject("placeholder") then
		error("no menu placeholder, not creating `new_gobj`")
		return
	end

	local new_gobj_mesh = create_quad(0.5, 0.5)
	local new_gobj = GameObject:new(new_gobj_mesh, "new_gobj")
	new_gobj:set_position(vec2:new(1, 0))

	s.scene:add_gameobject(new_gobj)

	s.scene:build()
end
