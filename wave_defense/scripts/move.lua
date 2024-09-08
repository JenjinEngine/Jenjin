local player = nilgobj()

SPEED = 1.5

function ready()
   player = s.scene:get_gameobject("player")
end

function update()
   if not player then
      return
   end

   local direction = vec2:new(0, 0)

   if is_key_pressed(GLFW_KEY_W) then
      direction.y = 1
   end

   if is_key_pressed(GLFW_KEY_A) then
      direction.x = -1
   end

   if is_key_pressed(GLFW_KEY_S) then
      direction.y = -1
   end

   if is_key_pressed(GLFW_KEY_D) then
      direction.x = 1
   end

   if direction.x == 0 and direction.y == 0 then
      return
   end

   direction = normalize(direction)
   direction = vec2:new(direction.x * s.dt * SPEED, direction.y * s.dt * SPEED)

   player:translate(direction)
end
