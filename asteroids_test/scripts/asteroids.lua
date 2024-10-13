local asteroids = {}

function READY() end

function UPDATE()
	if input.IsKeyDown("r") then
		local quad = helpers.CreateQuad()
		local name = tostring("asteroid_"..#asteroids)
		local obj = GameObject.new(name, quad)
		obj:SetPosition(vec2.new(math.random(-20, 20), math.random(-10, 10)))
		obj:SetRotation(math.random(0, 360))

		local r = math.random() / 5
		obj:SetColor(vec3.new(r, r, r))

		scene:AddGameObject(obj)
		scene:Build()

		asteroids[#asteroids+1] = obj
	end
end
