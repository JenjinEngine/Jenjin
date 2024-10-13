local player = nil

local rotation_speed = 5
local movement_speed = 0.2
local acceleration = 0.02
local deceleration = 0.97

local direction = { x = 0, y = 0 }
local velocity = 0

function READY()
	player = scene:GetGameObject("Player")
end

function UPDATE()
	handleRotation()
	handleMovement()

	player:Translate(vec2.new(direction.x, direction.y) * movement_speed * velocity)
end

function handleRotation()
	a = input.IsKeyDown("a")
	d = input.IsKeyDown("d")

	if a or d then
		local rot = player:GetRotation()

		if a then
			player:SetRotation((rot - rotation_speed) % 360)
		else
			player:SetRotation((rot + rotation_speed) % 360)
		end
	end
end

function handleMovement()
	if input.IsKeyDown("w") then
		local radians = math.rad(player:GetRotation())
		direction.y = math.cos(radians)
		direction.x = math.sin(radians)

		if velocity < 1 then
			velocity = velocity + acceleration
		end
	else
		velocity = velocity * deceleration
	end
end
