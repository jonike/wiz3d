ROTATION_SPEED = 32
TEXT = "Hover Bike by Ptis is licensed under CC Attribution-NonCommercial"

-- make screen resizable
screen.set(800, 600, false, true)

-- setup camera
camera.setclearcolor(0, _COLOR_DARKGRAY)
camera.setposition(0, 0, 4, -8)
camera.setrotation(0, 20, 0, 0)

-- load hoverbike
object.load(0, "hoverbike.msh")

while screen.opened() and not input.keydown(_KEY_ESC) do
  -- turn model
  object.turn(0, 0, ROTATION_SPEED * screen.delta(), 0)

  -- draw
  screen.drawtext(TEXT, (screen.width() - screen.textwidth(TEXT)) / 2, 2)
  screen.sync()
end
