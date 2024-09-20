local face = nil
local velocity = vec2.new(0.4, 0.6)

function ready()
	face = scene:get_gameobject("Face")
end

function update()
	local position = face:get_position()

	if position.x >= 2.5 or position.x <= -2.5 then
		velocity.x = velocity.x * -1
	end

	if position.y >= 2.5 or position.y <= -2.5 then
		velocity.y = velocity.y * -1
	end

	face:translate(velocity)
end
