local bullets = {}
local player = nil

local bulletCount = 0

local fromLeftGun = true
local timeSinceLastShot = 11

function READY()
	player = scene:GetGameObject("Player")
end

function UPDATE()
	if input.IsKeyDown("q") then
		timeSinceLastShot = timeSinceLastShot + 1
		if timeSinceLastShot > 1 then
			timeSinceLastShot = 0
			local obj = GameObject.new("bullet"..bulletCount, helpers.CreateQuad())
			bulletCount = bulletCount + 1

			fromLeftGun = not fromLeftGun

			local left = vec2.new(math.sin(math.rad(player:GetRotation() - 90)), math.cos(math.rad(player:GetRotation() - 90)))
			local up = vec2.new(math.sin(math.rad(player:GetRotation())), math.cos(math.rad(player:GetRotation())))

			if fromLeftGun then
				obj:SetPosition(player:GetPosition() + left * 1.1 + up * 1.2)
			else
				obj:SetPosition(player:GetPosition() - left * 1.1 + up * 1.2)
			end

			obj:SetScale(vec2.new(0.1, 0.2))
			obj:SetRotation(player:GetRotation())
			obj:SetColor(vec3.new(1, 0, 0))

			scene:AddGameObject(obj)
			scene:Build()

			table.insert(bullets, obj)

			player:Translate(up * -0.025)
		end
	end

	for i, bullet in ipairs(bullets) do
		local radians = math.rad(bullet:GetRotation())
		local direction = vec2.new(math.sin(radians), math.cos(radians))
		bullet:Translate(direction * 0.5)

		if bullet:GetPosition():Distance(player:GetPosition()) > 250 then
			scene:RemoveGameObject(bullet)
			table.remove(bullets, i)
		end
	end
end
