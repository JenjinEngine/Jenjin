local ball, paddle = nilgobj(), nilgobj()

function ready()
   ball = s.scene:get_gameobject("ball")
   paddle = s.scene:get_gameobject("right_paddle")

   print("Opponent script ready")
end

function update()
   if ball.transform.position.y > paddle.transform.position.y then
      paddle:translate(vec2:new(0, 1 * s.dt))
   elseif ball.transform.position.y < paddle.transform.position.y then
      paddle:translate(vec2:new(0, -1 * s.dt))
   end
end
