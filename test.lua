local target, bounds, cam = nil, nil, nil
local velocity = vec2.new(2.4, 0.3)

function ready()
	target = scene:get_gameobject("Face")
	bounds = scene:get_gameobject("Container")

	cam = scene:get_default_camera()
end

function update()
	local width = (bounds:get_scale().x * 10) / 2 - (target:get_scale().x * 10) / 2
	local height = (bounds:get_scale().y * 10) / 2 - (target:get_scale().y * 10) / 2

	local left = -width + bounds:get_position().x
	local right = width + bounds:get_position().x
	local top = height + bounds:get_position().y
	local bottom = -height + bounds:get_position().y

	local position = target:get_position()

	-- random
	cam:set_rotation(math.random(-1, 1))

	if position.x >= right or position.x <= left then
		velocity.x = velocity.x * -1
	end

	if position.y >= top or position.y <= bottom then
		velocity.y = velocity.y * -1
	end

	target:translate(velocity)
end
