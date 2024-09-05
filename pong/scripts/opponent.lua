local ball = nil
local paddle = nil

function ready()
	ball = s.scene:get_gameobject("ball")
	paddle = s.scene:get_gameobject("right_paddle")
	print("Opponent script ready")
end

function update()
	if ball.transform.position.y > paddle.transform.position.y then
		paddle:translate(vec3:new(0, 1 * s.dt, 0))
	elseif ball.transform.position.y < paddle.transform.position.y then
		paddle:translate(vec3:new(0, -1 * s.dt, 0))
	end
end
