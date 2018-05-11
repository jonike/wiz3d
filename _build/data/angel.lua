ROTATION_SPEED = 32
TEXT = "Frank by misterdevious is licensed under CC Attribution-NonCommercial-ShareAlike"

-- make screen resizable and set drawing color
screen.set(800, 600, false, true)
screen.setdrawcolor(color.rgb(240, 240, 240))

-- position camera
camera.setposition(0, 7, 7, -7)
camera.setrotation(0, 37.5, -45, 0)
camera.setclearcolor(0, color.rgb(15, 15, 15))

-- load object
object.load(0, "angel.msh", true)

-- setup lighting
--light.setambient(color.rgb(15, 15, 15))
light.setcolor(0, color.rgb(100, 100, 100))
light.create(1, _LIGHT_POINT)
light.setposition(1, 0, 0, -2)
light.setcolor(1, color.rgb(255, 100, 0))
light.create(2, _LIGHT_POINT)
light.setposition(2, 0, 8, 4)
light.setcolor(2, color.rgb(0, 100, 255))

value = 0
while screen.opened() and not input.keydown(_KEY_ESC) do
  -- rotate statue
  object.turn(0, 0, ROTATION_SPEED * screen.delta(), 0)

  -- update lighting
  value = value + screen.delta()
  local sin = 0.5 + math.abs(math.sin(value)) * 0.5
  local cos = 0.5 + math.abs(math.cos(value)) * 0.5
  light.setattenuation(1, 2 * sin)
  light.setattenuation(2, 10 * cos)
  
  screen.drawtext(TEXT, (screen.width() - screen.textwidth(TEXT)) / 2, 8)
  screen.sync()
end

