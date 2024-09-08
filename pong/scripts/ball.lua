
utils = require("pong.scripts.utils")

local ball, left_paddle, right_paddle = nilgobj(), nilgobj(), nilgobj()
local playing = false
local score = 0
local time = 0

local DEFAULT_BALL_SPEED = vec2:new(1.1, 1.35)
local velocity = DEFAULT_BALL_SPEED

function ready()
   ball = s.scene:get_gameobject("ball")
   left_paddle = s.scene:get_gameobject("left_paddle")
   right_paddle = s.scene:get_gameobject("right_paddle")

   print("Ball script ready")
end

function update()

   time = time + s.dt
   r, g, b = utils.hsv_to_rgb((time * 25) % 360, 1, 1)
   ball.color = vec3:new(r, g, b)

   if not ball or not left_paddle or not right_paddle then
      error("Ball script: ball, left paddle, or right paddle not found")
      return
   end

   if not playing then
      if is_key_pressed(GLFW_KEY_SPACE) then
         playing = true
      end

      return
   end


   if velocity.x > 0 then
      ball:rotate(180.0 * s.dt)
   else
      ball:rotate(-180.0 * s.dt)
   end


   if ball.transform.position.y > 1.0 - 0.025 then
      velocity.y = -velocity.y
      ball.transform.position.y = 1.0 - 0.025
   elseif ball.transform.position.y < -1.0 + 0.025 then
      velocity.y = -velocity.y
      ball.transform.position.y = -1.0 + 0.025
   end



   if ball.transform.position.x < left_paddle.transform.position.x + 0.025 and
      ball.transform.position.y < left_paddle.transform.position.y + 0.15 and
      ball.transform.position.y > left_paddle.transform.position.y - 0.15 then
      ball.transform.position.x = left_paddle.transform.position.x + 0.025
      velocity.x = -velocity.x
   end


   if ball.transform.position.x > right_paddle.transform.position.x - 0.025 and
      ball.transform.position.y < right_paddle.transform.position.y + 0.15 and
      ball.transform.position.y > right_paddle.transform.position.y - 0.15 then
      ball.transform.position.x = right_paddle.transform.position.x - 0.025
      velocity.x = -velocity.x
   end


   if ball.transform.position.x > 1.0 or ball.transform.position.x < -1.0 then
      if ball.transform.position.x > 1.0 then
         score = score + 1
         print("Score: " .. score)
      else
         print("Game Over!")
         score = 0
         playing = false
      end

      ball:set_position(vec2:new(0, 0))
      ball:set_rotation(0)
   end


   ball:translate(vec2:new(velocity.x * s.dt, velocity.y * s.dt))
end
