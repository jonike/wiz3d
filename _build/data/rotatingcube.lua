ROTATION_SPEED = 90

-- load image
image.load(0, "box-texture.png", true)

-- create a cube
object.createcube(0)
object.setscale(0, 5, 5, 5)
object.setimage(0, 0, 0)

while screen.opened() and not input.keydown(_KEY_ESC) do
  object.turn(0, 0, ROTATION_SPEED * screen.delta(), 0)
  screen.drawtext(math.floor(screen.fps()) .. " FPS", 4, 4)
  screen.sync()
end

