local leftPaddle = nilgobj()

function ready()
	leftPaddle = s.scene:get_gameobject("left_paddle")
	print("Paddle script ready")
end

function update()
	if not leftPaddle then
		error("Left paddle not found")
		return
	end

	leftPaddle:set_rotation(0.0)

	if is_key_pressed(GLFW_KEY_W) then
		leftPaddle:translate(vec2:new(0, 1 * s.dt))
		leftPaddle:set_rotation(2)
	elseif is_key_pressed(GLFW_KEY_S) then
		leftPaddle:translate(vec2:new(0, -1 * s.dt))
		leftPaddle:set_rotation(-2)
	end
end

