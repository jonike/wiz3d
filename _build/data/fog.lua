NUM_CUBES = 16 * 50
ROTATION_SPEED = 32

-- make screen resizable
screen.set(800, 600, false, true)

-- set fog
fog.setenabled(true)
fog.setmaxdistance(20)

-- setup camera
camera.setclearcolor(0, _COLOR_DARKGRAY)
camera.setposition(0, 0, 0, -7)
camera.setrotation(0, 0, 0, 0)
camera.setmaxdistance(0, 5000)

-- create rows of cubes
local x = -7
local z = NUM_CUBES / 16 * 2 - 2
for i = 0, NUM_CUBES-1, 2 do
  if i == 0 then
    object.createcube(i)
    object.setcolor(i, 0, _COLOR_BROWN)
  else
    object.instantiate(i, 0)
  end
  object.setposition(i, x, -1.5, z)
  object.instantiate(i+1, 0)
  object.setposition(i+1, x, 1.5, z)
  x = x + 2
  if x > 7 then x = -7; z = z - 2; end
end

while screen.opened() and not input.keydown(_KEY_ESC) do
  for i = 0, NUM_CUBES-1 do
    object.turn(i, 0, ROTATION_SPEED * screen.delta(), 0)
  end
  screen.drawtext(math.floor(screen.fps()) .. " FPS", 4, 4)
  screen.sync()
end
